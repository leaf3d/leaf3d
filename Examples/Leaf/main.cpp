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
#include <leaf3d/leaf3dut.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH    1024
#define WINDOW_HEIGHT   768

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
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "leaf3d", L3D_NULLPTR, L3D_NULLPTR);
    glfwMakeContextCurrent(window);

    // Init leaf3d.
    if (l3dInit() != L3D_TRUE) {
        fprintf(stderr, "Failed to initialize leaf3d\n");
        return -2;
    }

    // Init leaf3dut.
    if (l3dutInit() != L3D_TRUE) {
        fprintf(stderr, "Failed to initialize leaf3dut\n");
        return -3;
    }

    // ----------------------------- RESOURCES ----------------------------- //

    // Load a shader program.
    L3DHandle shaderProgram = l3dutLoadShaderProgram("Shaders/basic.vert", "Shaders/simpletexture.frag");

    // Load a texture.
    L3DHandle texture = l3dutLoadTexture2D("Textures/logo.png");

    // Load a material.
    L3DHandle material = l3dLoadMaterial("logo", shaderProgram);
    l3dAddTextureToMaterial(material, "diffuseMap", texture);

    // Load a simple textured quad.
    L3DHandle logo = l3dLoadQuad(material, L3DVec2(1, 1), L3D_ALPHA_BLEND_MESH_RENDERLAYER);

    // Create a camera.
    L3DHandle camera = l3dLoadCamera(
        "Default",
        glm::lookAt(
           glm::vec3(0.0f, 0.0f, 5.0f),
           glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3(0.0f, 1.0f, 0.0f)
        ),
        glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH/(GLfloat)WINDOW_HEIGHT, 1.0f, 100.0f)
    );

    // Create a forward rendering pipeline.
    L3DHandle renderQueue = l3dLoadForwardRenderQueue(WINDOW_WIDTH, WINDOW_HEIGHT);

    // ---------------------------- RENDERING ------------------------------ //

    double lastTime = glfwGetTime();
    int fps = 0;

    while(!glfwWindowShouldClose(window))
    {
        // Poll window events.
        glfwPollEvents();

        // Render current frame.
        l3dRenderFrame(camera, renderQueue);

        // Apply a rotation to the camera.
        l3dRotateCamera(camera, (float)sin(glfwGetTime()) * 0.05f);

        // Swap buffers.
        glfwSwapBuffers(window);

        // Measure speed.
        l3dutPrintFrameStats(glfwGetTime(), lastTime, fps);
    }

    // ---------------------------- TERMINATE ----------------------------- //

    // Terminate leaf3dut.
    l3dutTerminate();

    // Terminate leaf3d.
    l3dTerminate();

    // Terminate GLFW.
    glfwTerminate();

    return 0;
}
