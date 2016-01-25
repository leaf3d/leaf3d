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
#include <leaf3d/L3DRenderQueue.h>

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
    const L3DHandle& camera,
    const L3DHandle& renderQueue
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    _renderer->renderFrame(
        _renderer->getCamera(camera),
        _renderer->getRenderQueue(renderQueue)
    );
}

L3DHandle l3dLoadTexture(
    const L3DTextureType& type,
    const L3DImageFormat& format,
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
    const L3DShaderType& type,
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

void l3dSetShaderProgramUniformlUI(
    const L3DHandle& target,
    const char* name,
    unsigned int value
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(name, value);
}

void l3dSetShaderProgramUniformlB(
    const L3DHandle& target,
    const char* name,
    bool value
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(name, value);
}

void l3dSetShaderProgramUniformVec2(
    const L3DHandle& target,
    const char* name,
    const L3DVec2& value
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(name, value);
}

void l3dSetShaderProgramUniformVec3(
    const L3DHandle& target,
    const char* name,
    const L3DVec3& value
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(name, value);
}

void l3dSetShaderProgramUniformVec4(
    const L3DHandle& target,
    const char* name,
    const L3DVec4& value
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(name, value);
}

void l3dSetShaderProgramUniformMat3(
    const L3DHandle& target,
    const char* name,
    const L3DMat3& value
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(name, value);
}

void l3dSetShaderProgramUniformMat4(
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
    const L3DHandle& shaderProgram,
    const L3DVec3& diffuse,
    const L3DVec3& ambient,
    const L3DVec3& specular,
    float shininess
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DMaterial* material = new L3DMaterial(
        _renderer,
        name,
        _renderer->getShaderProgram(shaderProgram),
        diffuse,
        ambient,
        specular,
        shininess
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
    const L3DVec3& movement
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DCamera* camera = _renderer->getCamera(target);

    if (camera)
        camera->translate(movement);
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
    const L3DVertexFormat& vertexFormat,
    const L3DMat4& transMatrix,
    const L3DDrawType& drawType,
    const L3DDrawPrimitive& drawPrimitive
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
        transMatrix,
        drawType,
        drawPrimitive
    );

    if (mesh)
        return mesh->handle();

    return L3D_INVALID_HANDLE;
}

L3DHandle l3dLoadQuad(
    const L3DHandle &material,
    const L3DVec2& texMulFactor
)
{
    GLfloat vertices[] = {
    //  Position              Normal               Texcoords
        -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f          , 0.0f,           // Top-left
         0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  texMulFactor.x, 0.0f,           // Top-right
         0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  texMulFactor.x, texMulFactor.y, // Bottom-right
        -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f          , texMulFactor.y  // Bottom-left
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    return l3dLoadMesh(vertices, 4, indices, 6, material, L3D_POS3_NOR3_UV2);
}

L3DHandle l3dLoadCube(
    const L3DHandle &material,
    const L3DVec2& texMulFactor
)
{
    GLfloat vertices[] = {
    //  Position              Normal               Texcoords
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // Front Top-left
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  texMulFactor.x, 0.0f, // Front Top-right
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  texMulFactor.x, texMulFactor.y, // Front Bottom-right
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, texMulFactor.y, // Front Bottom-left

        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  texMulFactor.x, 0.0f, // Back Top-left
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // Back Top-right
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, texMulFactor.y, // Back Bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  texMulFactor.x, texMulFactor.y, // Back Bottom-left

        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // Left Top-left
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  texMulFactor.x, 0.0f, // Left Top-right
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  texMulFactor.x, texMulFactor.y, // Left Bottom-left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, texMulFactor.y, // Left Bottom-right

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, // Right Top-left
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  1.0f,  texMulFactor.x, 0.0f, // Right Top-right
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  1.0f,  texMulFactor.x, texMulFactor.y, // Right Bottom-left
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  1.0f,  0.0f, texMulFactor.y, // Right Bottom-right

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  1.0f,  0.0f, 0.0f, // Top Top-left
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  1.0f,  texMulFactor.x, 0.0f, // Top Top-right
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  1.0f,  texMulFactor.x, texMulFactor.y, // Top Bottom-left
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  1.0f,  0.0f, texMulFactor.y, // Top Bottom-right

        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  1.0f,  0.0f, 0.0f, // Bottom Top-left
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  1.0f,  texMulFactor.x, 0.0f, // Bottom Top-right
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  1.0f,  texMulFactor.x, texMulFactor.y, // Bottom Bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  1.0f,  0.0f, texMulFactor.y, // Bottom Bottom-right
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0,

        4, 5, 6,
        6, 7, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20,
    };

    return l3dLoadMesh(vertices, 24, indices, 36, material, L3D_POS3_NOR3_UV2);
}

L3DMat4 l3dGetMeshTrans(
    const L3DHandle& target
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DMesh* mesh = _renderer->getMesh(target);

    if (mesh)
        return mesh->transMatrix;

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
        mesh->transMatrix = trans;
}

void l3dTranslateMesh(
    const L3DHandle& target,
    const L3DVec3& movement
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DMesh* mesh = _renderer->getMesh(target);

    if (mesh)
        mesh->translate(movement);
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

void l3dScaleMesh(
    const L3DHandle& target,
    const L3DVec3& factor
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DMesh* mesh = _renderer->getMesh(target);

    if (mesh)
        mesh->scale(factor);
}

L3DHandle l3dLoadLight(
    const L3DVec3& position,
    const L3DVec4& color,
    const L3DLightAttenuation& attenuation
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DLight* light = new L3DLight(
        _renderer,
        position,
        color,
        attenuation
    );

    if (light)
        return light->handle();

    return L3D_INVALID_HANDLE;
}

void l3dToggleLight(
    const L3DHandle& target,
    bool on
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DLight* light = _renderer->getLight(target);

    if (light)
        light->isOn = on;
}

void l3dTranslateLight(
    const L3DHandle& target,
    const L3DVec3& movement
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DLight* light = _renderer->getLight(target);

    if (light)
        light->translate(movement);
}

L3DHandle l3dLoadForwardRenderQueue(const L3DVec4& clearColor)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DRenderQueue* renderQueue = new L3DRenderQueue(
        _renderer,
        "ForwardRendering"
    );

    renderQueue->addClearBuffersCommand(true, true, true, clearColor);
    renderQueue->addSetDepthTestCommand();
    renderQueue->addSetBlendCommand();
    renderQueue->addDrawMeshesCommand();

    if (renderQueue)
        return renderQueue->handle();

    return L3D_INVALID_HANDLE;
}
