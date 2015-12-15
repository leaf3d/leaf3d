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
#include <leaf3d/leaf3d.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
    //
    // L3DRenderer* renderer = new L3DRenderer();
    // renderer->init();
    if (l3dInit() != L3D_TRUE) {
        fprintf(stderr, "Failed to initialize leaf3d\n");
        return -2;
    }

    // ----------------------------- RESOURCES ----------------------------- //

    // Create and compile the vertex shader.
    //
    // L3DShader* vertexShader = renderer->loadShader(L3D_SHADER_VERTEX, GLSL(...));
    L3DHandle vertexShader = l3dLoadShader(L3D_SHADER_VERTEX, GLSL(
        uniform mat4 trans;
        uniform mat4 view;
        uniform mat4 proj;

        in vec2 position;
        in vec2 texcoord0;

        out vec2 Texcoord0;

        void main() {
            Texcoord0 = texcoord0;
            gl_Position = proj * view * trans * vec4(position, 0.0, 1.0);
        }
    ));

    // Create and compile the fragment shader.
    //
    // L3DShader* fragmentShader = renderer->loadShader(L3D_SHADER_FRAGMENT, GLSL(...));
    L3DHandle fragmentShader = l3dLoadShader(L3D_SHADER_FRAGMENT, GLSL(
        uniform sampler2D tex0;

        in vec2 Texcoord0;

        void main() {
            gl_FragColor = texture(tex0, Texcoord0);
        }
    ));

    // Link the final shader program.
    //
    // L3DShaderProgram* shaderProgram = renderer->loadShaderProgram(vertexShader, fragmentShader);
    L3DHandle shaderProgram = l3dLoadShaderProgram(vertexShader, fragmentShader);

    // Load a texture.
    //
    // L3DTexture* texture = renderer->loadTexture(L3D_TEXTURE_2D, ...);
    int width, height, comp = 0;
    unsigned char* img = stbi_load("Content/logo.png", &width, &height, &comp, 0);

    L3DHandle texture = l3dLoadTexture(L3D_TEXTURE_2D, comp == 4 ? L3D_RGBA : L3D_RGB, img, width, height, 0);

    stbi_image_free(img);

    // Load a material.
    //
    // L3DMaterial* material = renderer->loadMaterial("logo", shaderProgram);
    // material->setUniform("tex0", texture);
    L3DHandle material = l3dLoadMaterial("logo", shaderProgram);
    l3dSetMaterialUniformI(material, "tex0", texture.data.id);

    // Load a simple textured quad.
    //
    // L3DMesh* logo = renderer->loadMesh(vertices, 4, indices, 6, ...);
    GLfloat vertices[] = {
    //  Position      Texcoords
        -1.0f,  1.0f, 0.0f, 0.0f, // Top-left
         1.0f,  1.0f, 1.0f, 0.0f, // Top-right
         1.0f, -1.0f, 1.0f, 1.0f, // Bottom-right
        -1.0f, -1.0f, 0.0f, 1.0f  // Bottom-left
    };
    GLuint indices[] = {
         0, 1, 2,
         2, 3, 0
    };
    L3DHandle logo = l3dLoadMesh(vertices, 4, indices, 6, material, L3D_POS2_UV2);

    // Create a camera.
    //
    // L3DCamera* camera = renderer->loadCamera();
    L3DHandle camera = l3dLoadCamera();

    // ---------------------------- RENDERING ------------------------------ //

    while(!glfwWindowShouldClose(window))
    {
        // Poll window events.
        glfwPollEvents();

        // Apply a rotation to the quad.
        //
        // logo->rotate((float)sin(glfwGetTime()) * 0.05f));
        l3dRotateMesh(logo, (float)sin(glfwGetTime()) * 0.05f);

        // Render current frame.
        //
        // renderer->renderFrame(camera);
        l3dRenderFrame(camera);

        // Swap buffers.
        glfwSwapBuffers(window);
    }

    // ---------------------------- TERMINATE ----------------------------- //

    // Terminate leaf3d.
    //
    // renderer->terminate();
    l3dTerminate();

    // Terminate GLFW.
    glfwTerminate();

    return 0;
}
