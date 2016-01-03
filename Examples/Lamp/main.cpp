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
#include <leaf3d/leaf3d.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define WINDOW_SIZE 640

using namespace l3d;

int main()
{
    // -------------------------------- INIT ------------------------------- //

    // Init GLFW.
    if (glfwInit() != GL_TRUE) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Create a rendering window with OpenGL 3.2 context.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "leaf3d", L3D_NULLPTR, L3D_NULLPTR);
    glfwMakeContextCurrent(window);

    // Init leaf3d.
    if (l3dInit() != L3D_TRUE) {
        fprintf(stderr, "Failed to initialize leaf3d\n");
        return -2;
    }

    // ----------------------------- RESOURCES ----------------------------- //

    // Create and compile a basic shader program.
    L3DHandle basicVertexShader = l3dLoadShader(L3D_SHADER_VERTEX, GLSL(
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 proj;

        in vec3 position;

        void main() {
            gl_Position = proj * view * model * vec4(position, 1.0);
        }
    ));

    L3DHandle basicFragmentShader = l3dLoadShader(L3D_SHADER_FRAGMENT, GLSL(
        uniform vec4 objectColor;

        void main() {
            gl_FragColor = objectColor;
        }
    ));

    L3DHandle basicShaderProgram = l3dLoadShaderProgram(basicVertexShader, basicFragmentShader);

    // Create and compile a shader program which support lighting.
    L3DHandle vertexShader = l3dLoadShader(L3D_SHADER_VERTEX, GLSL(
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 proj;
        uniform mat3 normalMatrix;
        uniform vec3 lightPos;

        in vec3 position;
        in vec3 normal;
        in vec2 texcoord0;

        out vec2 Texcoord0;
        out vec3 Normal;
        out vec3 FragPos;
        out vec3 LightPos;

        void main() {
            Texcoord0 = texcoord0;
            Normal =  normalize(normalMatrix * normal);
            FragPos = vec3(view * model * vec4(position, 1.0));
            LightPos = vec3(view * vec4(lightPos, 1.0));
            gl_Position = proj * view * model * vec4(position, 1.0);
        }
    ));

    L3DHandle fragmentShader = l3dLoadShader(L3D_SHADER_FRAGMENT, GLSL(
        struct Material {
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
            float shininess;
        };

        uniform sampler2D albedoMap;
        uniform vec4 ambientColor;
        uniform vec4 lightColor;
        uniform Material material;

        in vec2 Texcoord0;
        in vec3 Normal;
        in vec3 FragPos;
        in vec3 LightPos;

        void main() {
            vec3 lightDir = normalize(LightPos - FragPos);

            vec3 ambientComponent = vec3(ambientColor) * ambientColor.w * material.ambient;

            float diff = max(dot(Normal, lightDir), 0.0);
            vec4 diffuseColor = diff * lightColor;
            vec3 diffuseComponent = vec3(diffuseColor) * diffuseColor.w * material.diffuse;

            vec3 viewDir = normalize(vec3(-FragPos));
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), material.shininess);
            vec3 specularComponent = vec3(lightColor * spec) * material.specular;

            vec4 finalColor = vec4(ambientComponent + diffuseComponent + specularComponent, 1.0f);
            gl_FragColor = vec4(texture(albedoMap, Texcoord0)) * finalColor;
        }
    ));

    L3DHandle shaderProgram = l3dLoadShaderProgram(vertexShader, fragmentShader);
    l3dSetShaderProgramUniformVec4(shaderProgram, "objectColor", L3DVec4(1, 1, 1, 1));

    // Load a lamp.
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("Content/lamp.obj", aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        fprintf(stderr, "Failed to load scene\n");
        return -3;
    }

    for (int i = 0; i < scene->mNumMeshes; ++i)
    {
        const aiMesh* mesh = scene->mMeshes[i];

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        for (int j = 0; j < mesh->mNumVertices; ++j)
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
            }

            if (mesh->HasTextureCoords(0))
            {
                vertices.push_back(mesh->mTextureCoords[0][j].x);
                vertices.push_back(mesh->mTextureCoords[0][j].y);
            }
        }

        for(int j = 0; j < mesh->mNumFaces; ++j)
        {
            indices.push_back(mesh->mFaces[j].mIndices[0]);
            indices.push_back(mesh->mFaces[j].mIndices[1]);
            indices.push_back(mesh->mFaces[j].mIndices[2]);
        }

        L3DHandle material;
        if(mesh->mMaterialIndex >= 0)
        {
            material = l3dLoadMaterial(mesh->mName.C_Str(), shaderProgram);

            aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

            if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                aiString path("Content/");
                aiString str;
                mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
                path.Append(str.C_Str());

                // Load a texture.
                int width, height, comp = 0;
                unsigned char* img = stbi_load(path.C_Str(), &width, &height, &comp, 3);
                L3DHandle texture = l3dLoadTexture(L3D_TEXTURE_2D, comp == 4 ? L3D_RGBA : L3D_RGB, img, width, height, 0);
                stbi_image_free(img);

                l3dAddTextureToMaterial(material, "albedoMap", texture);
            }
        }

        L3DHandle model = l3dLoadMesh(vertices.data(), mesh->mNumVertices, indices.data(), mesh->mNumFaces * 3, material, L3D_POS3_NOR3_UV2);
        l3dTranslateMesh(model, L3DVec3(-8, 0, 0));
        l3dScaleMesh(model, L3DVec3(8, 8, 8));
    }

    // Load a floor.
    int width, height, comp = 0;
    unsigned char* img = stbi_load("Content/floor.jpg", &width, &height, &comp, 0);
    L3DHandle floorTexture = l3dLoadTexture(L3D_TEXTURE_2D, comp == 4 ? L3D_RGBA : L3D_RGB, img, width, height, 0);
    stbi_image_free(img);
    L3DHandle floorMaterial = l3dLoadMaterial("floorMaterial", shaderProgram);
    l3dAddTextureToMaterial(floorMaterial, "albedoMap", floorTexture);
    L3DHandle floor = l3dLoadQuad(floorMaterial, L3DVec2(30, 30));
    l3dRotateMesh(floor, 1.57f, L3DVec3(-1, 0, 0));
    l3dScaleMesh(floor, L3DVec3(200, 200, 1));

    // Load a cube.
    img = stbi_load("Content/crate.jpg", &width, &height, &comp, 0);
    L3DHandle crateTexture = l3dLoadTexture(L3D_TEXTURE_2D, comp == 4 ? L3D_RGBA : L3D_RGB, img, width, height, 0);
    stbi_image_free(img);
    L3DHandle crateMaterial = l3dLoadMaterial("crateMaterial", shaderProgram);
    l3dAddTextureToMaterial(crateMaterial, "albedoMap", crateTexture);
    L3DHandle cube = l3dLoadCube(crateMaterial);
    l3dRotateMesh(cube, 0.75f);
    l3dTranslateMesh(cube, L3DVec3(10, 3, -2));
    l3dScaleMesh(cube, L3DVec3(6, 6, 6));

    // Load a light.
    L3DVec3 lightPos = L3DVec3(-2.0f, 12.0f, 0);
    L3DVec4 lightColor = L3DVec4(1, 1, 0.7f, 1);
    L3DHandle lightMaterial = l3dLoadMaterial("lightMaterial", basicShaderProgram);
    l3dSetShaderProgramUniformVec4(basicShaderProgram, "objectColor", lightColor);

    L3DHandle light = l3dLoadLight(lightPos, lightColor);
    L3DHandle lightBulb = l3dLoadCube(lightMaterial);
    l3dTranslateMesh(lightBulb, lightPos);
    l3dSetShaderProgramUniformVec4(shaderProgram, "ambientColor", L3DVec4(0.8, 0.8, 1, 0.3));
    l3dSetShaderProgramUniformVec4(shaderProgram, "lightColor", lightColor);
    l3dSetShaderProgramUniformVec3(shaderProgram, "lightPos", lightPos);

    // Create a camera.
    L3DHandle camera = l3dLoadCamera(
        "Default",
        glm::lookAt(
           glm::vec3(0.0f, 10.0f, 20.0f),
           glm::vec3(0.0f, 5.0f, 0.0f),
           glm::vec3(0.0f, 1.0f, 0.0f)
        )
    );

    // Create a forward rendering pipeline.
    L3DHandle renderQueue = l3dLoadForwardRenderQueue(L3DVec4(0, 0, 0.05f, 1));

    // ---------------------------- RENDERING ------------------------------ //

    while(!glfwWindowShouldClose(window))
    {
        // Poll window events.
        glfwPollEvents();

        // Render current frame.
        l3dRenderFrame(camera, renderQueue);

        // Apply a rotation to the camera.
        l3dRotateCamera(camera, (float)sin(glfwGetTime()) * 0.01f);

        // Swap buffers.
        glfwSwapBuffers(window);
    }

    // ---------------------------- TERMINATE ----------------------------- //

    // Terminate leaf3d.
    l3dTerminate();

    // Terminate GLFW.
    glfwTerminate();

    return 0;
}
