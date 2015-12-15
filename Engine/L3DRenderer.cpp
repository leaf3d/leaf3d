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
#include <leaf3d/L3DRenderer.h>
#include <leaf3d/L3DTexture.h>
#include <leaf3d/L3DShader.h>
#include <leaf3d/L3DShaderProgram.h>
#include <leaf3d/L3DMaterial.h>
#include <leaf3d/L3DCamera.h>
#include <leaf3d/L3DLight.h>
#include <leaf3d/L3DMesh.h>

using namespace l3d;

L3DRenderer::L3DRenderer()
{
}

L3DRenderer::~L3DRenderer()
{
    this->terminate();
}

int L3DRenderer::init()
{
    // Init GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    return L3D_TRUE;
}

int L3DRenderer::terminate()
{
    for (L3DTexturePool::reverse_iterator it = m_textures.rbegin(); it != m_textures.rend(); ++it)
        delete it->second;
    m_textures.clear();

    for (L3DShaderPool::reverse_iterator it = m_shaders.rbegin(); it != m_shaders.rend(); ++it)
        delete it->second;
    m_shaders.clear();

    for (L3DShaderProgramPool::reverse_iterator it = m_shaderPrograms.rbegin(); it != m_shaderPrograms.rend(); ++it)
        delete it->second;
    m_shaderPrograms.clear();

    for (L3DMaterialPool::reverse_iterator it = m_materials.rbegin(); it != m_materials.rend(); ++it)
        delete it->second;
    m_materials.clear();

    for (L3DCameraPool::reverse_iterator it = m_cameras.rbegin(); it != m_cameras.rend(); ++it)
        delete it->second;
    m_cameras.clear();

    for (L3DLightPool::reverse_iterator it = m_lights.rbegin(); it != m_lights.rend(); ++it)
        delete it->second;
    m_lights.clear();

    for (L3DMeshPool::reverse_iterator it = m_meshes.rbegin(); it != m_meshes.rend(); ++it)
        delete it->second;
    m_meshes.clear();

    return L3D_TRUE;
}

void L3DRenderer::renderFrame(L3DCamera* camera)
{
    // TODO: make any render operation as a command that could be chained
    // in a user-defined pipeline.

    if (!camera)
        return;

    // Clear the screen.
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw all the meshes.
    for (L3DMeshPool::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
    {
        L3DMesh* mesh = it->second;

        if (mesh && mesh->material())
        {
            glBindVertexArray(mesh->id());

            L3DMaterial* material = mesh->material();

            material->setUniform("view", camera->view);
            material->setUniform("proj", camera->proj);
            material->setUniform("trans", mesh->trans);

            if (mesh->indexCount() > 0)
                // Render vertices using indices.
                glDrawElements(GL_TRIANGLES, mesh->indexCount(), GL_UNSIGNED_INT, 0);
            else
                // Render vertices without using indices.
                glDrawArrays(GL_TRIANGLES, 0, mesh->vertexCount());
        }
    }

    glBindVertexArray(0);
}

L3DResource* L3DRenderer::getResource(const L3DHandle& handle) const
{
    switch(handle.data.type)
    {
    case L3D_TEXTURE:
        return m_textures.find(handle.data.id)->second;
    case L3D_SHADER:
        return m_shaders.find(handle.data.id)->second;
    case L3D_SHADER_PROGRAM:
        return m_shaderPrograms.find(handle.data.id)->second;
    case L3D_MATERIAL:
        return m_materials.find(handle.data.id)->second;
    case L3D_CAMERA:
        return m_cameras.find(handle.data.id)->second;
    case L3D_LIGHT:
        return m_lights.find(handle.data.id)->second;
    case L3D_MESH:
        return m_meshes.find(handle.data.id)->second;
    default:
        return L3D_NULLPTR;
    }
}

L3DTexture* L3DRenderer::getTexture(const L3DHandle& handle) const
{
    if (handle.data.type == L3D_TEXTURE)
        return m_textures.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DShader* L3DRenderer::getShader(const L3DHandle& handle) const
{
    if (handle.data.type == L3D_SHADER)
        return m_shaders.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DShaderProgram* L3DRenderer::getShaderProgram(const L3DHandle& handle) const
{
    if (handle.data.type == L3D_SHADER_PROGRAM)
        return m_shaderPrograms.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DMaterial* L3DRenderer::getMaterial(const L3DHandle& handle) const
{
    if (handle.data.type == L3D_MATERIAL)
        return m_materials.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DCamera* L3DRenderer::getCamera(const L3DHandle& handle) const
{
    if (handle.data.type == L3D_CAMERA)
        return m_cameras.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DLight* L3DRenderer::getLight(const L3DHandle& handle) const
{
    if (handle.data.type == L3D_LIGHT)
        return m_lights.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DMesh* L3DRenderer::getMesh(const L3DHandle& handle) const
{
    if (handle.data.type == L3D_MESH)
        return m_meshes.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DTexture* L3DRenderer::loadTexture(
    const TextureType& type,
    const ImageFormat& format,
    unsigned char* data,
    unsigned int width,
    unsigned int height,
    unsigned int depth
)
{
    L3DTexture* result = new L3DTexture(
        type,
        format,
        data,
        width,
        height,
        depth
    );

    m_textures[result->id()] = result;

    return result;
}

L3DShader* L3DRenderer::loadShader(
    const ShaderType& type,
    const char* code
)
{
    L3DShader* result = new L3DShader(
        type,
        code
    );

    m_shaders[result->id()] = result;

    return result;
}

L3DShaderProgram* L3DRenderer::loadShaderProgram(
    L3DShader* vertexShader,
    L3DShader* fragmentShader,
    L3DShader* geometryShader
)
{
    L3DShaderProgram* result = new L3DShaderProgram(
        vertexShader,
        fragmentShader,
        geometryShader
    );

    m_shaderPrograms[result->id()] = result;

    return result;
}

L3DMaterial* L3DRenderer::loadMaterial(
    const char* name,
    L3DShaderProgram* shaderProgram
)
{
    L3DMaterial* result = new L3DMaterial(
        name,
        shaderProgram
    );

    m_materials[result->id()] = result;

    return result;
}

L3DCamera* L3DRenderer::loadCamera(
    const L3DMat4& view,
    const L3DMat4& projection
)
{
    L3DCamera* result = new L3DCamera(
        view,
        projection
    );

    m_cameras[result->id()] = result;

    return result;
}

L3DLight* L3DRenderer::loadLight(
)
{
    L3DLight* result = new L3DLight(
    );

    m_lights[result->id()] = result;

    return result;
}

L3DMesh* L3DRenderer::loadMesh(
    float* vertices,
    unsigned int vertexCount,
    unsigned int* indices,
    unsigned int indexCount,
    L3DMaterial* material,
    const VertexFormat& VertexFormat,
    const DrawType& drawType,
    const DrawPrimitive& drawPrimitive
)
{
    L3DMesh* result = new L3DMesh(
        vertices,
        vertexCount,
        indices,
        indexCount,
        material,
        VertexFormat,
        drawType,
        drawPrimitive
    );

    m_meshes[result->id()] = result;

    return result;
}
