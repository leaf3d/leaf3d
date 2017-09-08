/*
 * This file is part of the leaf3d project.
 *
 * Copyright 2014-2015 Emanuele Bertoldi. All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * You should have received a copy of the modified BSD License along with this
 * program. If not, see <http://www.opensource.org/licenses/bsd-license.php>
 */

#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <leaf3d/leaf3d.h>
#include <leaf3d/leaf3dut.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace l3d;

static std::string _rootPath = "";

int l3dutInit(const char* rootPath)
{
    _rootPath = rootPath;

    return L3D_TRUE;
}

int l3dutTerminate()
{
    _rootPath = "";

    return L3D_TRUE;
}

L3DHandle l3dutLoadTexture2D(
  const char* filename,
  const L3DImageFormat& desiredFormat
)
{
    if (!filename)
        return L3D_INVALID_HANDLE;

    int width, height, comp = 0;
    unsigned char* img = stbi_load((_rootPath + filename).c_str(), &width, &height, &comp, desiredFormat);
    L3DHandle texture = l3dLoadTexture(L3D_TEXTURE_2D, comp == 4 ? L3D_RGBA : L3D_RGB, img, width, height, 0);
    stbi_image_free(img);

    return texture;
}

L3DHandle l3dutLoadTextureCube(
    const char* filenameRight,
    const char* filenameLeft,
    const char* filenameTop,
    const char* filenameBottom,
    const char* filenameBack,
    const char* filenameFront,
    const L3DImageFormat& desiredFormat
)
{
    if (!filenameRight || !filenameLeft || !filenameTop || !filenameBottom || !filenameBack || !filenameFront)
        return L3D_INVALID_HANDLE;

    int width, height, comp, size = 0;
    unsigned char* imgRight = stbi_load((_rootPath + filenameRight).c_str(), &width, &height, &comp, desiredFormat);
    size += width * height * comp;
    unsigned char* imgLeft = stbi_load((_rootPath + filenameLeft).c_str(), &width, &height, &comp, desiredFormat);
    size += width * height * comp;
    unsigned char* imgTop = stbi_load((_rootPath + filenameTop).c_str(), &width, &height, &comp, desiredFormat);
    size += width * height * comp;
    unsigned char* imgBottom = stbi_load((_rootPath + filenameBottom).c_str(), &width, &height, &comp, desiredFormat);
    size += width * height * comp;
    unsigned char* imgBack = stbi_load((_rootPath + filenameBack).c_str(), &width, &height, &comp, desiredFormat);
    size += width * height * comp;
    unsigned char* imgFront = stbi_load((_rootPath + filenameFront).c_str(), &width, &height, &comp, desiredFormat);
    size += width * height * comp;

    unsigned char* img = (unsigned char*)malloc(size);
    unsigned int faceSize = size / 6;
    memcpy(img, imgRight, faceSize);
    memcpy(img + faceSize, imgLeft, faceSize);
    memcpy(img + faceSize * 2, imgTop, faceSize);
    memcpy(img + faceSize * 3, imgBottom, faceSize);
    memcpy(img + faceSize * 4, imgBack, faceSize);
    memcpy(img + faceSize * 5, imgFront, faceSize);

    stbi_image_free(imgRight);
    stbi_image_free(imgLeft);
    stbi_image_free(imgTop);
    stbi_image_free(imgBottom);
    stbi_image_free(imgBack);
    stbi_image_free(imgFront);

    L3DHandle texture = l3dLoadTexture(L3D_TEXTURE_CUBE_MAP, comp == 4 ? L3D_RGBA : L3D_RGB, img, width, height, 0);

    stbi_image_free(img);

    return texture;
}

L3DHandle l3dutLoadShader(const L3DShaderType& type, const char* filename)
{
    if (!filename)
        return L3D_INVALID_HANDLE;

    std::ifstream file;

    file.open((_rootPath + filename).c_str());

    if (!file)
        return L3D_INVALID_HANDLE;

    std::stringstream stream;

    stream << file.rdbuf();

    file.close();

    std::string out = stream.str();

    return l3dLoadShader(type, out.c_str());
}

L3DHandle l3dutLoadShaderProgram(
    const char* vertexShaderFilename,
    const char* fragmentShaderFilename,
    const char* geometryShaderFilename
)
{
    if (!vertexShaderFilename || !fragmentShaderFilename)
        return L3D_INVALID_HANDLE;

    L3DHandle vertexShader = l3dutLoadShader(L3D_SHADER_VERTEX, vertexShaderFilename);
    L3DHandle fragmentShader = l3dutLoadShader(L3D_SHADER_FRAGMENT, fragmentShaderFilename);
    L3DHandle geometryShader = l3dutLoadShader(L3D_SHADER_GEOMETRY, geometryShaderFilename);

    return l3dLoadShaderProgram(vertexShader, fragmentShader, geometryShader);
}

L3DHandle* l3dutLoadMeshes(
    const char* filename,
    const L3DHandle& shaderProgram,
    unsigned int* meshCount,
    unsigned int renderLayer
)
{
    if (!filename)
    {
        if (meshCount)
            *meshCount = 0;

        return 0;
    }

    std::vector<L3DHandle> meshes;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(_rootPath + filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene)
    {
        if (meshCount)
            *meshCount = 0;

        return 0;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        const aiMesh* mesh = scene->mMeshes[i];

        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        L3DVertexFormat vertexFormat = L3D_POS3_UV2;

        for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
        {
            if (mesh->HasPositions())
            {
                vertices.push_back(mesh->mVertices[j].x);
                vertices.push_back(mesh->mVertices[j].y);
                vertices.push_back(mesh->mVertices[j].z);
            }

            if (mesh->HasNormals())
            {
                vertices.push_back(mesh->mNormals[j].x);
                vertices.push_back(mesh->mNormals[j].y);
                vertices.push_back(mesh->mNormals[j].z);

                vertexFormat = L3D_POS3_NOR3_UV2;
            }

            if (mesh->HasTangentsAndBitangents())
            {
                vertices.push_back(mesh->mTangents[j].x);
                vertices.push_back(mesh->mTangents[j].y);
                vertices.push_back(mesh->mTangents[j].z);

                vertexFormat = L3D_POS3_NOR3_TAN3_UV2;
            }

            unsigned int uvCount = mesh->GetNumUVChannels();
            for (unsigned int t = 0; t < uvCount; ++t)
            {
                if (mesh->HasTextureCoords(t))
                {
                    unsigned int coordSize = mesh->mNumUVComponents[t];

                    vertices.push_back(mesh->mTextureCoords[t][j].x);
                    vertices.push_back(mesh->mTextureCoords[t][j].y);
                    if (coordSize > 2)
                        vertices.push_back(mesh->mTextureCoords[t][j].z);

                    unsigned int vertexSize = vertexFormat + t * coordSize;
                    if (vertexSize < L3D_MAX_VERTEX_FORMAT)
                        vertexFormat = (L3DVertexFormat)vertexSize;
                }
            }
        }

        for(unsigned int j = 0; j < mesh->mNumFaces; ++j)
        {
            indices.push_back(mesh->mFaces[j].mIndices[0]);
            indices.push_back(mesh->mFaces[j].mIndices[1]);
            indices.push_back(mesh->mFaces[j].mIndices[2]);
        }

        L3DHandle material;
        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

            aiString materialName;
            mat->Get(AI_MATKEY_NAME, materialName);

            aiColor3D diffuse;
            mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);

            aiColor3D specular;
            mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);

            aiColor3D ambient;
            mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);

            float shininess;
            mat->Get(AI_MATKEY_SHININESS, shininess);

            material = l3dLoadMaterial(
                materialName.C_Str(),
                shaderProgram,
                L3DVec3(diffuse.r, diffuse.g, diffuse.b),
                L3DVec3(ambient.r, ambient.g, ambient.b),
                L3DVec3(specular.r, specular.g, specular.b),
                shininess
            );

            if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                aiString textureFilename;
                mat->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilename);
                L3DHandle texture = l3dutLoadTexture2D(textureFilename.C_Str());

                l3dAddTextureToMaterial(material, "diffuseMap", texture);
            }

            if (mat->GetTextureCount(aiTextureType_SPECULAR) > 0)
            {
                aiString textureFilename;
                mat->GetTexture(aiTextureType_SPECULAR, 0, &textureFilename);
                L3DHandle texture = l3dutLoadTexture2D(textureFilename.C_Str());

                l3dAddTextureToMaterial(material, "specularMap", texture);
            }

            if (mat->GetTextureCount(aiTextureType_OPACITY) > 0)
            {
                aiString textureFilename;
                mat->GetTexture(aiTextureType_OPACITY, 0, &textureFilename);
                L3DHandle texture = l3dutLoadTexture2D(textureFilename.C_Str());

                l3dAddTextureToMaterial(material, "alphaMap", texture);
            }

            if (mat->GetTextureCount(aiTextureType_HEIGHT) > 0)
            {
                aiString textureFilename;
                mat->GetTexture(aiTextureType_HEIGHT, 0, &textureFilename);
                L3DHandle texture = l3dutLoadTexture2D(textureFilename.C_Str());

                l3dAddTextureToMaterial(material, "normalMap", texture);
            }
        }

        L3DHandle loadedMesh = l3dLoadMesh(
            vertices.data(), mesh->mNumVertices,
            indices.data(), mesh->mNumFaces * 3,
            material,
            vertexFormat,
            L3DMat4(), L3D_DRAW_STATIC, L3D_DRAW_TRIANGLES,
            renderLayer
        );

        if (loadedMesh.repr)
            meshes.push_back(loadedMesh);
    }

    if (meshCount)
        *meshCount = meshes.size();

    unsigned int vecSize = meshes.size() * sizeof(L3DHandle);
    void* retPtr = malloc(vecSize);
    memcpy(retPtr, meshes.data(), vecSize);

    return (L3DHandle*)retPtr;
}
