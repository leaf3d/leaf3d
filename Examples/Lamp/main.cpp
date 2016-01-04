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
#include <GLFW/glfw3.h>

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

    // Init leaf3dut.
    if (l3dutInit() != L3D_TRUE) {
        fprintf(stderr, "Failed to initialize leaf3dut\n");
        return -3;
    }

    // ----------------------------- RESOURCES ----------------------------- //

    // Load a shader program for light bulb.
    L3DHandle basicShaderProgram = l3dutLoadShaderProgram("basic.vert", "basic.frag");

    // Load a shader program with support for lighting (Blinn-Phong).
    L3DHandle blinnPhongShaderProgram = l3dutLoadShaderProgram("blinnphong.vert", "blinnphong.frag");

    // Load a lamp.
    unsigned int meshCount = 0;
    L3DHandle* lamp = l3dutLoadMeshes("lamp.obj", blinnPhongShaderProgram, &meshCount);

    if (meshCount > 0)
    {
        L3DHandle model = lamp[0];
        l3dTranslateMesh(model, L3DVec3(-8, 0, 0));
        l3dScaleMesh(model, L3DVec3(8, 8, 8));
    }

    // Load a floor.
    L3DHandle floorTexture = l3dutLoadTexture2D("floor.jpg");
    L3DHandle floorMaterial = l3dLoadMaterial("floorMaterial", blinnPhongShaderProgram);
    l3dAddTextureToMaterial(floorMaterial, "albedoMap", floorTexture);
    L3DHandle floor = l3dLoadQuad(floorMaterial, L3DVec2(30, 30));
    l3dRotateMesh(floor, 1.57f, L3DVec3(-1, 0, 0));
    l3dScaleMesh(floor, L3DVec3(200, 200, 1));

    // Load a cube.
    L3DHandle crateTexture = l3dutLoadTexture2D("crate.jpg");
    L3DHandle crateMaterial = l3dLoadMaterial("crateMaterial", blinnPhongShaderProgram);
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
    l3dSetShaderProgramUniformVec4(blinnPhongShaderProgram, "objectColor", L3DVec4(1, 1, 1, 1));
    l3dSetShaderProgramUniformVec4(blinnPhongShaderProgram, "ambientColor", L3DVec4(0.8, 0.8, 1, 0.3));
    l3dSetShaderProgramUniformVec4(blinnPhongShaderProgram, "lightColor", lightColor);
    l3dSetShaderProgramUniformVec3(blinnPhongShaderProgram, "lightPos", lightPos);

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

    // Terminate leaf3dut.
    l3dutTerminate();

    // Terminate leaf3d.
    l3dTerminate();

    // Terminate GLFW.
    glfwTerminate();

    return 0;
}
