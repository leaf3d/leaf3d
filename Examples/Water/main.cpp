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
#define WAVE_COUNT      4
#define MOON_COLOR      L3DVec4(1, 1, 0.5f, 0.6f)
#define WATER_COLOR     L3DVec4(0.3f, 0.5f, 0.6f, 1.0f)
#define SKY_COLOR       L3DVec4(0.1f, 0.15f, 0.2f, 1.0f)
#define FOG_DENSITY     0.02f

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

    // Load a shader program for realistic water rendering.
    L3DHandle waterShaderProgram = l3dutLoadShaderProgram("Shaders/water.vert", "Shaders/water.frag");

    // Load water's textures.
    L3DHandle normalMap = l3dutLoadTexture2D("Textures/Water/water_norm.png");

    // Load a material.
    L3DHandle waterMaterial = l3dLoadMaterial("waterPlaneMaterial", waterShaderProgram, L3DVec3(0,0,0));
    l3dAddTextureToMaterial(waterMaterial, "normalMap", normalMap);

    // Load a water plane.
    L3DHandle waterPlane = l3dLoadGrid(200, waterMaterial, L3DVec2(0.05f, 0.05f), L3D_ALPHA_BLEND_MESH_RENDERLAYER);
    l3dRotateMesh(waterPlane, 1.57f, L3DVec3(-1, 0, 0));
    l3dScaleMesh(waterPlane, L3DVec3(400, 400, 1));

    l3dSetShaderProgramUniformVec4(waterShaderProgram, "u_waterColor", WATER_COLOR);
    l3dSetShaderProgramUniformF(waterShaderProgram, "u_fogDensity", FOG_DENSITY);
    l3dSetShaderProgramUniformF(waterShaderProgram, "u_waterHeight", 0);
    l3dSetShaderProgramUniformI(waterShaderProgram, "u_numWaves", WAVE_COUNT);

    const float PI = glm::pi<float>();

    for (int i = 0; i < WAVE_COUNT; ++i) {
        float amplitude = 0.5f / (i + 1);
        l3dSetShaderProgramUniformF(waterShaderProgram, "u_amplitude", amplitude, i);

        float wavelength = 10 * PI / (i + 1);
        l3dSetShaderProgramUniformF(waterShaderProgram, "u_wavelength", wavelength, i);

        float speed = 1.0f + 2*i;
        l3dSetShaderProgramUniformF(waterShaderProgram, "u_speed", speed, i);

        float angle = (float(rand() % 100) / 120.0f) + (float(rand() % 100) / 80.0f) * PI / 3;
        l3dSetShaderProgramUniformVec2(waterShaderProgram, "u_direction", L3DVec2(cos(angle), sin(angle)), i);
    }

    // Load a shader program for light bulb.
    L3DHandle basicShaderProgram = l3dutLoadShaderProgram("Shaders/basic.vert", "Shaders/basic.frag");

    // Load a spotlight.
    L3DVec3 light1Pos = L3DVec3(-2.0f, 15.0f, 0);
    L3DVec4 light1Color = L3DVec4(1, 1, 0.7f, 0.8f);
    L3DHandle light1 = l3dLoadSpotLight(light1Pos, L3DVec3(0, -1, 0), light1Color);
    l3dSetLightAttenuation(light1, 0.07f, 0.06f, 0);
    L3DHandle light1Material = l3dLoadMaterial("light1Material", basicShaderProgram, light1Color.xyz());
    L3DHandle light1Bulb = l3dLoadCube(light1Material);
    l3dTranslateMesh(light1Bulb, light1Pos);

    // Load a directional light.
    L3DHandle directionalLight = l3dLoadDirectionalLight(L3DVec3(0, -1, 0), MOON_COLOR);

    // Set the global ambient light color.
    l3dSetShaderProgramUniformVec4(waterShaderProgram, "u_ambientColor", SKY_COLOR);

    // Create a camera.
    L3DHandle camera = l3dLoadCamera(
        "Default",
        glm::lookAt(
           glm::vec3(0.0f, 15.0f, 35.0f),
           glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3(0.0f, 1.0f, 0.0f)
        ),
        glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH/(GLfloat)WINDOW_HEIGHT, 1.0f, 1000.0f)
    );

    // Create a forward rendering pipeline.
    L3DHandle renderQueue = l3dLoadForwardRenderQueue(WINDOW_WIDTH, WINDOW_HEIGHT, SKY_COLOR);

    // ---------------------------- RENDERING ------------------------------ //

    while(!glfwWindowShouldClose(window))
    {
        double time = glfwGetTime();

        // Poll window events.
        glfwPollEvents();

        // Render current frame.
        l3dRenderFrame(camera, renderQueue);

        // Apply a rotation to the camera.
        l3dRotateCamera(camera, (float)sin(time) * 0.002f);

        // Apply movement to lights.
        L3DVec3 dy1(L3DVec3(1, 0, 1) * (float)sin(time) * 0.08f);
        l3dTranslateLight(light1, dy1);
        l3dTranslateMesh(light1Bulb, dy1);

        // Animate water waves.
        l3dSetShaderProgramUniformF(waterShaderProgram, "u_time", (float)time);

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
