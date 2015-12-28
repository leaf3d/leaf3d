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

    // Create and compile the vertex shader.
    L3DHandle vertexShader = l3dLoadShader(L3D_SHADER_VERTEX, GLSL(
        uniform mat4 trans;
        uniform mat4 view;
        uniform mat4 proj;

        in vec3 position;
        in vec2 texcoord0;

        out vec2 Texcoord0;

        void main() {
            Texcoord0 = texcoord0;
            gl_Position = proj * view * trans * vec4(position, 1.0);
        }
    ));

    // Create and compile the fragment shader.
    L3DHandle fragmentShader = l3dLoadShader(L3D_SHADER_FRAGMENT, GLSL(
        uniform sampler2D albedoMap;

        in vec2 Texcoord0;

        void main() {
            gl_FragColor = texture(albedoMap, Texcoord0);
        }
    ));

    // Link the final shader program.
    L3DHandle shaderProgram = l3dLoadShaderProgram(vertexShader, fragmentShader);

    // Load a model.
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("Content/frankie.obj", aiProcess_Triangulate | aiProcess_FlipUVs);
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

        L3DHandle model = l3dLoadMesh(vertices.data(), mesh->mNumVertices, indices.data(), mesh->mNumFaces * 3, material, L3D_POS3_UV2);
    }

    // Create a camera.
    L3DHandle camera = l3dLoadCamera(
        "Default",
        glm::lookAt(
           glm::vec3(0.0f, 5.0f, 20.0f),
           glm::vec3(0.0f, 5.0f, 0.0f),
           glm::vec3(0.0f, 1.0f, 0.0f)
        )
    );

    // ---------------------------- RENDERING ------------------------------ //

    while(!glfwWindowShouldClose(window))
    {
        // Poll window events.
        glfwPollEvents();

        // Render current frame.
        l3dRenderFrame(camera);

        // Apply a rotation to the camera.
        l3dRotateCamera(camera, (float)sin(glfwGetTime()) * 0.05f);

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
