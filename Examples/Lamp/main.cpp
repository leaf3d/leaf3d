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
    L3DHandle blinnPhongShaderProgram = l3dutLoadShaderProgram("basic.vert", "blinnphong.frag");

    // Load a floor.
    L3DHandle floorTexture = l3dutLoadTexture2D("floor.png");
    L3DHandle floorSpecTexture = l3dutLoadTexture2D("floor_spec.png");
    L3DHandle floorNormTexture = l3dutLoadTexture2D("floor_norm.png");
    L3DHandle floorMaterial = l3dLoadMaterial("floorMaterial", blinnPhongShaderProgram);
    l3dAddTextureToMaterial(floorMaterial, "diffuseMap", floorTexture);
    l3dAddTextureToMaterial(floorMaterial, "specularMap", floorSpecTexture);
    l3dAddTextureToMaterial(floorMaterial, "normalMap", floorNormTexture);
    L3DHandle floor = l3dLoadQuad(floorMaterial, L3DVec2(20, 20));
    l3dRotateMesh(floor, 1.57f, L3DVec3(-1, 0, 0));
    l3dScaleMesh(floor, L3DVec3(200, 200, 1));

    // Load a lamp.
    unsigned int meshCount = 0;
    L3DHandle* lamp = l3dutLoadMeshes("lamp.obj", blinnPhongShaderProgram, &meshCount);
    for (int i=0; i<meshCount; ++i)
    {
        l3dTranslateMesh(lamp[i], L3DVec3(-8, 0, 0));
        l3dScaleMesh(lamp[i], L3DVec3(8, 8, 8));
    }

    // Load a cube.
    L3DHandle crateTexture = l3dutLoadTexture2D("crate.png");
    L3DHandle crateSpecTexture = l3dutLoadTexture2D("crate_spec.png");
    L3DHandle crateNormTexture = l3dutLoadTexture2D("crate_norm.png");
    L3DHandle crateMaterial = l3dLoadMaterial("crateMaterial", blinnPhongShaderProgram);
    l3dAddTextureToMaterial(crateMaterial, "diffuseMap", crateTexture);
    l3dAddTextureToMaterial(crateMaterial, "specularMap", crateSpecTexture);
    l3dAddTextureToMaterial(crateMaterial, "normalMap", crateNormTexture);
    L3DHandle cube1 = l3dLoadCube(crateMaterial);
    l3dRotateMesh(cube1, 0.75f);
    l3dTranslateMesh(cube1, L3DVec3(10, 3, -2));
    l3dScaleMesh(cube1, L3DVec3(6, 6, 6));

    // Load a spotlight.
    L3DVec3 light1Pos = L3DVec3(-2.0f, 12.0f, 0);
    L3DVec4 light1Color = L3DVec4(1, 1, 0.7f, 0.8f);
    L3DHandle light1 = l3dLoadSpotLight(light1Pos, L3DVec3(0, -1, 0), light1Color);
    l3dSetLightAttenuation(light1, 0.7f, 0.6f, 0);
    L3DHandle light1Material = l3dLoadMaterial("light1Material", basicShaderProgram, light1Color.xyz());
    L3DHandle light1Bulb = l3dLoadCube(light1Material);
    l3dTranslateMesh(light1Bulb, light1Pos);

    // Load some point lights.
    L3DVec3 light2Pos = L3DVec3(10, 5.0f, -4);
    L3DVec4 light2Color = L3DVec4(1, 0.2f, 0.2f, 0.8f);
    L3DHandle light2 = l3dLoadPointLight(light2Pos, light2Color);
    L3DHandle light2Material = l3dLoadMaterial("light2Material", basicShaderProgram, light2Color.xyz());
    L3DHandle light2Bulb = l3dLoadCube(light2Material);
    l3dTranslateMesh(light2Bulb, light2Pos);

    L3DVec3 light3Pos = L3DVec3(-2, 2, -14);
    L3DVec4 light3Color = L3DVec4(0.2f, 1.0f, 0.2f, 0.8f);
    L3DHandle light3 = l3dLoadPointLight(light3Pos, light3Color);
    L3DHandle light3Material = l3dLoadMaterial("light3Material", basicShaderProgram, light3Color.xyz());
    L3DHandle light3Bulb = l3dLoadCube(light3Material);
    l3dTranslateMesh(light3Bulb, light3Pos);

    L3DVec3 light4Pos = L3DVec3(-1.5f, 1, 2);
    L3DVec4 light4Color = L3DVec4(0.2f, 0.2f, 1.0f, 0.8f);
    L3DHandle light4 = l3dLoadPointLight(light4Pos, light4Color);
    L3DHandle light4Material = l3dLoadMaterial("light4Material", basicShaderProgram, light4Color.xyz());
    L3DHandle light4Bulb = l3dLoadCube(light4Material);
    l3dTranslateMesh(light4Bulb, light4Pos);

    // Load a directional light.
    L3DVec4 light5Color = L3DVec4(0.5f, 0.5f, 0.6f, 0.5f);
    L3DHandle light5 = l3dLoadDirectionalLight(L3DVec3(-5, -1, -1), light5Color);

    // Set the global ambient light color.
    l3dSetShaderProgramUniformVec4(blinnPhongShaderProgram, "u_ambientColor", L3DVec4(0.8f, 0.8f, 1, 0.2f));

    // Create a camera.
    L3DHandle camera = l3dLoadCamera(
        "Default",
        glm::lookAt(
           glm::vec3(0.0f, 10.0f, 22.0f),
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
        float sinOfTime = sin(glfwGetTime());
        l3dRotateCamera(camera, (0.5f + sinOfTime * 0.5f) * 0.01f);

        // Apply movement to lights.
        L3DVec3 dy2(L3DVec3(0, 1, 1) * sinOfTime * 0.04f);
        l3dTranslateLight(light2, dy2);
        l3dTranslateMesh(light2Bulb, dy2);;
        L3DVec3 dy3(L3DVec3(1, 0.5f, 0) * sinOfTime * 0.05f);
        l3dTranslateLight(light3, dy3);
        l3dTranslateMesh(light3Bulb, dy3);
        L3DVec3 dy4(L3DVec3(-1, 0, 1) * sinOfTime * 0.06f);
        l3dTranslateLight(light4, dy4);
        l3dTranslateMesh(light4Bulb, dy4);

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
