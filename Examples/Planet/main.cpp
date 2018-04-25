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

#define WINDOW_WIDTH            1024
#define WINDOW_HEIGHT           768
#define ASTEROID_COUNT          8000
#define ASTEROID_INSTANCE_SIZE  16
#define ASTEROID_BUFFER_SIZE    ASTEROID_COUNT * ASTEROID_INSTANCE_SIZE
#define ORBIT_RADIUS            250.0f
#define ORBIT_OFFSET            30.0f

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

    // Load a basic shader program.
    L3DHandle shaderProgram = l3dutLoadShaderProgram("Shaders/basic.vert", "Shaders/simpletexture.frag");

    // Load a shader program for instancing.
    L3DHandle instancingShaderProgram = l3dutLoadShaderProgram("Shaders/instancing.vert", "Shaders/simpletexture.frag");

    // Load a shader program for sky box.
    L3DHandle skyBoxShaderProgram = l3dutLoadShaderProgram("Shaders/skyBox.vert", "Shaders/skyBox.frag");

    // Load a sky box.
    L3DHandle skyBoxCubeMap = l3dutLoadTextureCube(
        "Textures/SkyBox3/right.jpg",
        "Textures/SkyBox3/left.jpg",
        "Textures/SkyBox3/top.jpg",
        "Textures/SkyBox3/bottom.jpg",
        "Textures/SkyBox3/back.jpg",
        "Textures/SkyBox3/front.jpg"
    );
    L3DHandle skyBoxMaterial = l3dLoadMaterial("skyBoxMaterial", skyBoxShaderProgram);
    l3dAddTextureToMaterial(skyBoxMaterial, "cubeMap", skyBoxCubeMap);
    L3DHandle skyBox = l3dLoadSkyBox(skyBoxMaterial);

    // Load a planet.
    unsigned int meshCount = 0;
    L3DHandle* planet = l3dutLoadMeshes("Models/planet.obj", shaderProgram, &meshCount);
    l3dTranslateMesh(planet[0], L3DVec3(0.0f, -20.0f, 0.0f));
    l3dScaleMesh(planet[0], L3DVec3(20.0f, 20.0f, 20.0f));

    // Load instanced asteroids.
    L3DMat4 asteroidMats[ASTEROID_BUFFER_SIZE];
    srand(glfwGetTime()); // Initialize random seed.
    for (unsigned int i = 0; i < ASTEROID_COUNT; ++i)
    {
      // 1. Translation: displace along circle wORBIT_RADIUS' in range [-ORBIT_OFFSET, ORBIT_OFFSET]
      float angle = (float)i / (float)ASTEROID_COUNT * 360.0f;
      float displacement = (rand() % (int)(2 * ORBIT_OFFSET * 100)) / 100.0f - ORBIT_OFFSET;
      float x = sin(angle) * ORBIT_RADIUS + displacement;
      displacement = (rand() % (int)(2 * ORBIT_OFFSET * 100)) / 100.0f - ORBIT_OFFSET;
      float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
      displacement = (rand() % (int)(2 * ORBIT_OFFSET * 100)) / 100.0f - ORBIT_OFFSET;
      float z = cos(angle) * ORBIT_RADIUS + displacement;
      asteroidMats[i] = glm::translate(asteroidMats[i], glm::vec3(x, y, z));

      // 2. Scale: add scale variation
      float scale = (rand() % 100) / 100.0f;
      asteroidMats[i] = glm::scale(asteroidMats[i], glm::vec3(scale));

      // 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
      float rotAngle = (rand() % 360);
      asteroidMats[i] = glm::rotate(asteroidMats[i], rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
    }

    L3DHandle* asteroid = l3dutLoadMeshes("Models/rock.obj", instancingShaderProgram, &meshCount);

    // Add instancing to mesh.
    l3dSetMeshInstances(
      asteroid[0],
      asteroidMats,
      ASTEROID_COUNT,
      L3D_INSTANCE_TRANS4_TRANS4_TRANS4_TRANS4
    );

    // Create a camera.
    L3DHandle camera = l3dLoadCamera(
        "Default",
        glm::lookAt(
           glm::vec3(0.0f, 100.0f, 400.0f),
           glm::vec3(0.0f, -15.0f, 0.0f),
           glm::vec3(0.0f, 1.0f, 0.0f)
        ),
        glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH/(GLfloat)WINDOW_HEIGHT, 1.0f, 1000.0f)
    );

    // Create a forward rendering pipeline.
    L3DHandle renderQueue = l3dLoadForwardRenderQueue(WINDOW_WIDTH, WINDOW_HEIGHT, L3DVec4(0, 0, 0.05f, 1));

    // ---------------------------- RENDERING ------------------------------ //

    double lastTime = glfwGetTime();

    while(!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        double dt = now - lastTime;
        lastTime = now;

        // Measure speed.
        l3dutPrintFrameStats(dt);

        // Poll window events.
        glfwPollEvents();

        // Render current frame.
        l3dRenderFrame(camera, renderQueue);

        // Apply a rotation to the camera.
        l3dRotateCamera(camera, 0.5f * dt);

        // Swap buffers.
        glfwSwapBuffers(window);

        // Print speed.
        l3dutPrintFrameStats(dt);
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
