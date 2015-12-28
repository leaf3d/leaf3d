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

#include <leaf3d/leaf3d.h>
#include <leaf3d/L3DRenderer.h>
#include <leaf3d/L3DTexture.h>
#include <leaf3d/L3DShader.h>
#include <leaf3d/L3DShaderProgram.h>
#include <leaf3d/L3DMaterial.h>
#include <leaf3d/L3DCamera.h>
#include <leaf3d/L3DLight.h>
#include <leaf3d/L3DMesh.h>

using namespace l3d;

static L3DRenderer* _renderer = L3D_NULLPTR;

int l3dInit()
{
    if (_renderer == L3D_NULLPTR)
    {
        _renderer = new L3DRenderer();
        return _renderer->init();
    }

    return L3D_TRUE;
}

int l3dTerminate()
{
    if (_renderer)
    {
        delete _renderer;
        _renderer = L3D_NULLPTR;
    }

    return L3D_TRUE;
}

void l3dRenderFrame(
    const L3DHandle& camera
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    _renderer->renderFrame(
        _renderer->getCamera(camera)
    );
}

L3DHandle l3dLoadTexture(
    const TextureType& type,
    const ImageFormat& format,
    unsigned char* data,
    unsigned int width,
    unsigned int height,
    unsigned int depth
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DTexture* texture = new L3DTexture(
        _renderer,
        type,
        format,
        data,
        width,
        height,
        depth
    );

    if (texture)
        return texture->handle();

    return L3D_INVALID_HANDLE;
}

L3DHandle l3dLoadShader(
    const ShaderType& type,
    const char* code
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShader* shader = new L3DShader(
        _renderer,
        type,
        code
    );

    if (shader)
        return shader->handle();

    return L3D_INVALID_HANDLE;
}

L3DHandle l3dLoadShaderProgram(
    const L3DHandle& vertexShader,
    const L3DHandle& fragmentShader,
    const L3DHandle& geometryShader
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = new L3DShaderProgram(
        _renderer,
        _renderer->getShader(vertexShader),
        _renderer->getShader(fragmentShader),
        _renderer->getShader(geometryShader)
    );

    if (shaderProgram)
        return shaderProgram->handle();

    return L3D_INVALID_HANDLE;
}

void l3dSetShaderProgramUniformlI(
    const L3DHandle& target,
    const char* name,
    int value
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(name, value);
}

void l3dSetShaderProgramUniformF(
    const L3DHandle& target,
    const char* name,
    float value
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(name, value);
}

void l3dSetShaderProgramUniformMat4F(
    const L3DHandle& target,
    const char* name,
    const L3DMat4& value
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(name, value);
}

L3DHandle l3dLoadMaterial(
    const char* name,
    const L3DHandle& shaderProgram
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DMaterial* material = new L3DMaterial(
        _renderer,
        name,
        _renderer->getShaderProgram(shaderProgram)
    );

    if (material)
        return material->handle();

    return L3D_INVALID_HANDLE;
}

void l3dAddTextureToMaterial(
    const L3DHandle& target,
    const char* name,
    const L3DHandle& texture
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DMaterial* material = _renderer->getMaterial(target);
    if (material)
        material->textures[name] = _renderer->getTexture(texture);

    return;
}

L3DHandle l3dLoadCamera(
    const char* name,
    const L3DMat4& view,
    const L3DMat4& projection
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DCamera* camera = new L3DCamera(
        _renderer,
        name,
        view,
        projection
    );

    if (camera)
        return camera->handle();

    return L3D_INVALID_HANDLE;
}

L3DMat4 l3dGetCameraView(
    const L3DHandle& target
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DCamera* camera = _renderer->getCamera(target);

    if (camera)
        return camera->view;

    return L3DMat4();
}

void l3dSetCameraView(
    const L3DHandle& target,
    const L3DMat4& view
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DCamera* camera = _renderer->getCamera(target);

    if (camera)
        camera->view = view;
}

L3DMat4 l3dGetCameraProj(
    const L3DHandle& target
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DCamera* camera = _renderer->getCamera(target);

    if (camera)
        return camera->proj;

    return L3DMat4();
}

void l3dSetCameraProj(
    const L3DHandle& target,
    const L3DMat4& proj
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DCamera* camera = _renderer->getCamera(target);

    if (camera)
        camera->proj = proj;
}

void l3dTranslateCamera(
    const L3DHandle& target,
    const L3DVec3& trans
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DCamera* camera = _renderer->getCamera(target);

    if (camera)
        camera->translate(trans);
}

void l3dRotateCamera(
    const L3DHandle& target,
    float radians,
    const L3DVec3& direction
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DCamera* camera = _renderer->getCamera(target);

    if (camera)
        camera->rotate(radians, direction);
}

L3DHandle l3dLoadMesh(
    float* vertices,
    unsigned int vertexCount,
    unsigned int* indices,
    unsigned int indexCount,
    const L3DHandle& material,
    const VertexFormat& vertexFormat,
    const DrawType& drawType,
    const DrawPrimitive& drawPrimitive
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DMesh* mesh = new L3DMesh(
        _renderer,
        vertices,
        vertexCount,
        indices,
        indexCount,
        _renderer->getMaterial(material),
        vertexFormat,
        drawType,
        drawPrimitive
    );

    if (mesh)
        return mesh->handle();

    return L3D_INVALID_HANDLE;
}

L3DHandle l3dLoadQuad(
    const L3DHandle &material
)
{
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

    return l3dLoadMesh(vertices, 4, indices, 6, material, L3D_POS2_UV2);
}

L3DMat4 l3dGetMeshTrans(
    const L3DHandle& target
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DMesh* mesh = _renderer->getMesh(target);

    if (mesh)
        return mesh->trans;

    return L3DMat4();
}

void l3dSetMeshTrans(
    const L3DHandle& target,
    const L3DMat4& trans
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DMesh* mesh = _renderer->getMesh(target);

    if (mesh)
        mesh->trans = trans;
}

void l3dTranslateMesh(
    const L3DHandle& target,
    const L3DVec3& trans
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DMesh* mesh = _renderer->getMesh(target);

    if (mesh)
        mesh->translate(trans);
}

void l3dRotateMesh(
    const L3DHandle& target,
    float radians,
    const L3DVec3& direction
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DMesh* mesh = _renderer->getMesh(target);

    if (mesh)
        mesh->rotate(radians, direction);
}
