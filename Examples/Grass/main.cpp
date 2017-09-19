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

#define WINDOW_WIDTH        1024
#define WINDOW_HEIGHT       768
#define GRASS_DENSITY       200
#define GRASS_COLOR         L3DVec3(0.85f,0.85f,0.55f)
#define GRASS_COLOR_VAR     L3DVec3(0.2f,0.2f,0.3f)
#define GRASS_HEIGHT        0.75f
#define GRASS_HEIGHT_VAR    5.0f
#define GRASS_FIELD_SIZE    1000.0f
#define GRASS_DISTANCE_LOD3 GRASS_FIELD_SIZE * 0.5 * 1.0f
#define GRASS_DISTANCE_LOD2 GRASS_FIELD_SIZE * 0.5 * 0.6f
#define GRASS_DISTANCE_LOD1 GRASS_FIELD_SIZE * 0.5 * 0.3f
#define AMBIENT_COLOR       L3DVec4(0.8f, 0.8f, 1, 0.6f)
#define SUN_LIGHT_COLOR     L3DVec4(0.8f, 0.7f, 0.6f, 1)

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

    // Load a shader program with support for lighting (Blinn-Phong).
    L3DHandle blinnPhongShaderProgram = l3dutLoadShaderProgram("Shaders/basic.vert", "Shaders/blinnphong.frag");

    // Load a shader program for sky box.
    L3DHandle skyBoxShaderProgram = l3dutLoadShaderProgram("Shaders/skyBox.vert", "Shaders/skyBox.frag");

    // Load a shader program for grass plane rendering.
    L3DHandle grassPlaneShaderProgram = l3dutLoadShaderProgram("Shaders/basic.vert", "Shaders/grassPlane.frag");

    // Load a shader program for realistic grass blades rendering.
    L3DHandle grassBladesShaderProgram = l3dutLoadShaderProgram("Shaders/basic.vert", "Shaders/grassBlades.frag", "Shaders/grassBlades.geom");

    // Load a sky box.
    L3DHandle skyBoxCubeMap = l3dutLoadTextureCube(
        "Textures/SkyBox2/right.jpg",
        "Textures/SkyBox2/left.jpg",
        "Textures/SkyBox2/top.jpg",
        "Textures/SkyBox2/bottom.jpg",
        "Textures/SkyBox2/back.jpg",
        "Textures/SkyBox2/front.jpg"
    );
    L3DHandle skyBoxMaterial = l3dLoadMaterial("skyBoxMaterial", skyBoxShaderProgram);
    l3dAddTextureToMaterial(skyBoxMaterial, "cubeMap", skyBoxCubeMap);
    L3DHandle skyBox = l3dLoadSkyBox(skyBoxMaterial);

    // Load a grass plane.
    L3DHandle grassTexture = l3dutLoadTexture2D("Textures/Grass/grass_diffuse.jpg");
    L3DHandle grassDirtTexture = l3dutLoadTexture2D("Textures/Grass/grass_dirt_diffuse.jpg");
    L3DHandle grassPlaneMaterial = l3dLoadMaterial("grassPlaneMaterial", grassPlaneShaderProgram, GRASS_COLOR);
    l3dAddTextureToMaterial(grassPlaneMaterial, "diffuseMap", grassTexture);
    l3dAddTextureToMaterial(grassPlaneMaterial, "dirtMap", grassDirtTexture);
    L3DHandle grassPlane = l3dLoadGrid(1, grassPlaneMaterial, L3DVec2(30, 30), L3D_ALPHA_BLEND_MESH_RENDERLAYER);

    l3dRotateMesh(grassPlane, 1.57f, L3DVec3(-1, 0, 0));
    l3dScaleMesh(grassPlane, L3DVec3(GRASS_FIELD_SIZE, GRASS_FIELD_SIZE, 1));

    l3dSetShaderProgramUniformF(grassPlaneShaderProgram, "u_grassDistanceLOD3", GRASS_DISTANCE_LOD3);
    l3dSetShaderProgramUniformF(grassPlaneShaderProgram, "u_grassDistanceLOD2", GRASS_DISTANCE_LOD2);
    l3dSetShaderProgramUniformF(grassPlaneShaderProgram, "u_grassDistanceLOD1", GRASS_DISTANCE_LOD1);

    // Load grass blades.
    L3DHandle grassBladesTexture = l3dutLoadTexture2D("Textures/Grass/grass_blade_diffuse.png");
    L3DHandle grassBladesMaterial = l3dLoadMaterial("grassBladeMaterial", grassBladesShaderProgram, GRASS_COLOR);
    l3dAddTextureToMaterial(grassBladesMaterial, "diffuseMap", grassBladesTexture);
    L3DHandle grassBlades = l3dLoadGrid(GRASS_DENSITY, grassBladesMaterial, L3DVec2(1, 1), L3D_ALPHA_BLEND_MESH_RENDERLAYER);

    l3dRotateMesh(grassBlades, 1.57f, L3DVec3(-1, 0, 0));
    l3dScaleMesh(grassBlades, L3DVec3(GRASS_FIELD_SIZE * 0.5, GRASS_FIELD_SIZE * 0.5, 1));

    l3dSetShaderProgramUniformF(grassBladesShaderProgram, "u_grassDistanceLOD3", GRASS_DISTANCE_LOD3 * 0.5f);
    l3dSetShaderProgramUniformF(grassBladesShaderProgram, "u_grassDistanceLOD2", GRASS_DISTANCE_LOD2 * 0.5f);
    l3dSetShaderProgramUniformF(grassBladesShaderProgram, "u_grassDistanceLOD1", GRASS_DISTANCE_LOD1 * 0.5f);
    l3dSetShaderProgramUniformF(grassBladesShaderProgram, "u_grassHeight", GRASS_HEIGHT);
    l3dSetShaderProgramUniformF(grassBladesShaderProgram, "u_grassHeightVariation", GRASS_HEIGHT_VAR);
    l3dSetShaderProgramUniformVec3(grassBladesShaderProgram, "u_grassColorVariation", GRASS_COLOR_VAR);

    // Load a crate.
    L3DHandle crateTexture = l3dutLoadTexture2D("Textures/Crate/crate.jpg");
    L3DHandle crateSpecTexture = l3dutLoadTexture2D("Textures/Crate/crate_spec.jpg");
    L3DHandle crateNormTexture = l3dutLoadTexture2D("Textures/Crate/crate_norm.jpg");
    L3DHandle crateMaterial = l3dLoadMaterial("crateMaterial", blinnPhongShaderProgram);
    l3dAddTextureToMaterial(crateMaterial, "diffuseMap", crateTexture);
    l3dAddTextureToMaterial(crateMaterial, "specularMap", crateSpecTexture);
    l3dAddTextureToMaterial(crateMaterial, "normalMap", crateNormTexture);

    L3DHandle cube1 = l3dLoadCube(crateMaterial);
    l3dRotateMesh(cube1, 0.75f);
    l3dTranslateMesh(cube1, L3DVec3(10, 3, -20));
    l3dScaleMesh(cube1, L3DVec3(6, 6, 6));

    // Load a tree.
    unsigned int meshCount = 0;
    L3DHandle* tree = l3dutLoadMeshes("Models/tree1.obj", blinnPhongShaderProgram, &meshCount);
    for (int i=0; i<meshCount; ++i)
    {
      l3dRotateMesh(tree[i], 0.75f);
      l3dTranslateMesh(tree[i], L3DVec3(-45, 0, 20));
      l3dScaleMesh(tree[i], L3DVec3(10, 10, 10));
    }

    // Load a directional light.
    L3DHandle sunLight = l3dLoadDirectionalLight(L3DVec3(-2, -1, 5), SUN_LIGHT_COLOR);

    // Set the global ambient light color.
    l3dSetShaderProgramUniformVec4(blinnPhongShaderProgram, "u_ambientColor", AMBIENT_COLOR);

    // Create a camera.
    L3DHandle camera = l3dLoadCamera(
        "Default",
        glm::lookAt(
           glm::vec3(0.0f, 15.0f, 40.0f),
           glm::vec3(0.0f, 5.0f, 0.0f),
           glm::vec3(0.0f, 1.0f, 0.0f)
        ),
        glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH/(GLfloat)WINDOW_HEIGHT, 10.0f, GRASS_FIELD_SIZE)
    );

    // Create a forward rendering pipeline.
    L3DHandle renderQueue = l3dLoadForwardRenderQueue(WINDOW_WIDTH, WINDOW_HEIGHT, L3DVec4(0, 0, 0.05f, 1));

    // ---------------------------- RENDERING ------------------------------ //

    while(!glfwWindowShouldClose(window))
    {
        // Poll window events.
        glfwPollEvents();

        // Render current frame.
        l3dRenderFrame(camera, renderQueue);

        // Apply a rotation to the camera.
        float sinOfTime = sin(glfwGetTime());
        l3dRotateCamera(camera, (0.5f + sinOfTime * 0.5f) * 0.01f, L3DVec3(0.0f, 1.0f, 0.0f));

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
