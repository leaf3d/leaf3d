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
#include <sstream>
#include <leaf3d/leaf3d.h>
#include <leaf3d/L3DRenderer.h>
#include <leaf3d/L3DTexture.h>
#include <leaf3d/L3DShader.h>
#include <leaf3d/L3DShaderProgram.h>
#include <leaf3d/L3DFrameBuffer.h>
#include <leaf3d/L3DMaterial.h>
#include <leaf3d/L3DCamera.h>
#include <leaf3d/L3DLight.h>
#include <leaf3d/L3DMesh.h>
#include <leaf3d/L3DRenderQueue.h>

using namespace l3d;

static L3DRenderer* _renderer = L3D_NULLPTR;

static const char* _defaultScreenVertexShader = GLSL(
    in vec2 i_position;
    in vec2 i_texcoord0;

    out vec2    o_texcoord0;

    void main()
    {
        gl_Position = vec4(i_position.x, i_position.y, 0.0f, 1.0f);
        o_texcoord0 = i_texcoord0;
    }
);

static const char* _defaultScreenFragmentShader = GLSL(
    in vec2 o_texcoord0;

    uniform sampler2D u_diffuseMap;

    void main()
    {
        gl_FragColor = vec4(texture(u_diffuseMap, o_texcoord0).rgb, 1);
    }
);

static const std::string _getUniformName(const char* name, int index)
{
    std::string _name(name);

    if (index > -1)
    {
        std::ostringstream sstream;
        sstream << "[" << index << "]";
        _name.append(sstream.str());
    }

    return _name;
}

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

L3DHandle l3dLoadForwardRenderQueue(
    unsigned int width,
    unsigned int height,
    const L3DVec4& clearColor,
    const L3DHandle& screenFragmentShader
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DRenderQueue* renderQueue = new L3DRenderQueue(
        _renderer,
        "ForwardRendering"
    );

    // A. Init framebuffer.
    L3DTexture* frameBufferColorTexture = new L3DTexture(_renderer, L3D_TEXTURE_2D, L3D_RGB, 0, width, height, 0);

    L3DFrameBuffer* backendBuffer = new L3DFrameBuffer(
        _renderer,
        new L3DTexture(_renderer, L3D_TEXTURE_2D, L3D_DEPTH24_STENCIL8, 0, width, height, 0, false, L3D_UNSIGNED_INT_24_8),
        frameBufferColorTexture
    );

    // B. Init fullscreen quad.
    L3DShader* fsQuadVertexShader = new L3DShader(
        _renderer,
        L3D_SHADER_VERTEX,
        _defaultScreenVertexShader
    );

    L3DShader* fsQuadFragmentShader = _renderer->getShader(screenFragmentShader);

    if (!fsQuadFragmentShader)
    {
        fsQuadFragmentShader = new L3DShader(
            _renderer,
            L3D_SHADER_FRAGMENT,
            _defaultScreenFragmentShader
        );
    }

    L3DShaderProgram* fsQuadShaderProgram = new L3DShaderProgram(
        _renderer,
        fsQuadVertexShader,
        fsQuadFragmentShader
    );

    L3DMaterial* fsQuadMaterial = new L3DMaterial(
        _renderer,
        "Fullscreen Quad",
        fsQuadShaderProgram,
        L3DColorRegistry(),
        L3DParameterRegistry(),
        L3DTextureRegistry()
    );

    // Assign diffuse texture of framebuffer
    // to diffuse texture of fullscreen quad material.
    fsQuadMaterial->textures["diffuseMap"] = frameBufferColorTexture;

    GLfloat vertices[] = {
    //   Position      Texcoords
        -1.0f,  1.0f,  0.0f, 1.0f,  // Top-left
         1.0f,  1.0f,  1.0f, 1.0f,  // Top-right
         1.0f, -1.0f,  1.0f, 0.0f,  // Bottom-right
        -1.0f, -1.0f,  0.0f, 0.0f   // Bottom-left
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    L3DMesh* fsQuad = new L3DMesh(
        _renderer,
        vertices, 4,
        indices, 6,
        fsQuadMaterial,
        L3D_POS2_UV2
    );

    // Draw fullscreen quad on last layer.
    fsQuad->setRenderLayer(L3D_POSTPROCESSING_RENDERLAYER);

    // 1. Clear frame buffer.
    renderQueue->addSwitchFrameBufferCommand(backendBuffer);
    renderQueue->addClearBuffersCommand(true, true, true, clearColor);
    renderQueue->addSetBlendCommand(false);
    renderQueue->addSetCullFaceCommand(true, L3D_BACK_FACE);

    // 2. Render meshes not writing on depth buffer of frame buffer.
    renderQueue->addSetDepthTestCommand(false);
    renderQueue->addSetDepthMaskCommand(false);
    renderQueue->addDrawMeshesCommand(L3D_SKYBOX_MESH_RENDERLAYER);

    // 3. Render opaque models on frame buffer.
    renderQueue->addSetDepthTestCommand(true, L3D_LESS);
    renderQueue->addSetDepthMaskCommand(true);
    renderQueue->addDrawMeshesCommand(L3D_OPAQUE_MESH_RENDERLAYER);

    // 4. Render meshes with alpha-blend.
    renderQueue->addSetCullFaceCommand(false);
    renderQueue->addSetBlendCommand(true);
    renderQueue->addDrawMeshesCommand(L3D_ALPHA_BLEND_MESH_RENDERLAYER);

    // 5. Render fullscreen quad to screen, sampling from framebuffer.
    renderQueue->addSwitchFrameBufferCommand(0);
    renderQueue->addClearBuffersCommand(true, false, false, clearColor);
    renderQueue->addSetDepthTestCommand(false);
    renderQueue->addSetBlendCommand(false);
    renderQueue->addDrawMeshesCommand(L3D_POSTPROCESSING_RENDERLAYER);

    if (renderQueue)
        return renderQueue->handle();

    return L3D_INVALID_HANDLE;
}

L3DHandle l3dLoadTexture(
    const L3DTextureType& type,
    const L3DImageFormat& format,
    unsigned char* data,
    unsigned int width,
    unsigned int height,
    unsigned int depth,
    bool mipmap,
    const L3DPixelFormat& pixelFormat,
    const L3DImageMinFilter& minFilter,
    const L3DImageMagFilter& magFilter,
    const L3DImageWrapMethod& wrapS,
    const L3DImageWrapMethod& wrapT,
    const L3DImageWrapMethod& wrapR
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
        depth,
        mipmap,
        pixelFormat,
        minFilter,
        magFilter,
        wrapS,
        wrapT,
        wrapR
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
    float value,
    int index
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(_getUniformName(name, index).c_str(), value);
}

void l3dSetShaderProgramUniformI(
    const L3DHandle& target,
    const char* name,
    int value,
    int index
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(_getUniformName(name, index).c_str(), value);
}

void l3dSetShaderProgramUniformUI(
    const L3DHandle& target,
    const char* name,
    unsigned int value,
    int index
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(_getUniformName(name, index).c_str(), value);
}

void l3dSetShaderProgramUniformB(
    const L3DHandle& target,
    const char* name,
    bool value,
    int index
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(_getUniformName(name, index).c_str(), value);
}

void l3dSetShaderProgramUniformVec2(
    const L3DHandle& target,
    const char* name,
    const L3DVec2& value,
    int index
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(_getUniformName(name, index).c_str(), value);
}

void l3dSetShaderProgramUniformVec3(
    const L3DHandle& target,
    const char* name,
    const L3DVec3& value,
    int index
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(_getUniformName(name, index).c_str(), value);
}

void l3dSetShaderProgramUniformVec4(
    const L3DHandle& target,
    const char* name,
    const L3DVec4& value,
    int index
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(_getUniformName(name, index).c_str(), value);
}

void l3dSetShaderProgramUniformMat3(
    const L3DHandle& target,
    const char* name,
    const L3DMat3& value,
    int index
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(_getUniformName(name, index).c_str(), value);
}

void l3dSetShaderProgramUniformMat4(
    const L3DHandle& target,
    const char* name,
    const L3DMat4& value,
    int index
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DShaderProgram* shaderProgram = _renderer->getShaderProgram(target);
    if (shaderProgram)
        shaderProgram->setUniform(_getUniformName(name, index).c_str(), value);
}

L3DHandle l3dLoadFrameBuffer(
    const L3DHandle& textureDepthStencilAttachment,
    const L3DHandle& textureColorAttachment0,
    const L3DHandle& textureColorAttachment1,
    const L3DHandle& textureColorAttachment2,
    const L3DHandle& textureColorAttachment3,
    const L3DHandle& textureColorAttachment4,
    const L3DHandle& textureColorAttachment5,
    const L3DHandle& textureColorAttachment6,
    const L3DHandle& textureColorAttachment7,
    const L3DHandle& textureColorAttachment8,
    const L3DHandle& textureColorAttachment9,
    const L3DHandle& textureColorAttachment10,
    const L3DHandle& textureColorAttachment11,
    const L3DHandle& textureColorAttachment12,
    const L3DHandle& textureColorAttachment13,
    const L3DHandle& textureColorAttachment14,
    const L3DHandle& textureColorAttachment15
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DTextureAttachments textures;

    if (textureDepthStencilAttachment.data.id)
    {
        textures[L3D_DEPTH_STENCIL_ATTACHMENT] = _renderer->getTexture(textureDepthStencilAttachment);
    }

    if (textureColorAttachment0.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT0] = _renderer->getTexture(textureColorAttachment0);
    }

    if (textureColorAttachment1.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT1] = _renderer->getTexture(textureColorAttachment1);
    }

    if (textureColorAttachment2.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT2] = _renderer->getTexture(textureColorAttachment2);
    }

    if (textureColorAttachment3.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT3] = _renderer->getTexture(textureColorAttachment3);
    }

    if (textureColorAttachment4.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT4] = _renderer->getTexture(textureColorAttachment4);
    }

    if (textureColorAttachment5.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT5] = _renderer->getTexture(textureColorAttachment5);
    }

    if (textureColorAttachment6.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT6] = _renderer->getTexture(textureColorAttachment6);
    }

    if (textureColorAttachment7.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT7] = _renderer->getTexture(textureColorAttachment7);
    }

    if (textureColorAttachment8.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT8] = _renderer->getTexture(textureColorAttachment8);
    }

    if (textureColorAttachment9.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT9] = _renderer->getTexture(textureColorAttachment9);
    }

    if (textureColorAttachment10.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT10] = _renderer->getTexture(textureColorAttachment10);
    }

    if (textureColorAttachment11.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT11] = _renderer->getTexture(textureColorAttachment11);
    }

    if (textureColorAttachment12.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT12] = _renderer->getTexture(textureColorAttachment12);
    }

    if (textureColorAttachment13.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT13] = _renderer->getTexture(textureColorAttachment13);
    }

    if (textureColorAttachment14.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT14] = _renderer->getTexture(textureColorAttachment14);
    }

    if (textureColorAttachment15.data.id)
    {
        textures[L3D_COLOR_ATTACHMENT15] = _renderer->getTexture(textureColorAttachment15);
    }

    L3DFrameBuffer* frameBuffer = new L3DFrameBuffer(
        _renderer,
        textures
    );

    if (frameBuffer)
        return frameBuffer->handle();

    return L3D_INVALID_HANDLE;
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

    L3DMaterial* material = L3DMaterial::createBlinnPhongMaterial(
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
    const L3DDrawPrimitive& drawPrimitive,
    unsigned int renderLayer
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
        drawPrimitive,
        renderLayer
    );

    if (mesh)
        return mesh->handle();

    return L3D_INVALID_HANDLE;
}

L3DHandle l3dLoadQuad(
    const L3DHandle& material,
    const L3DVec2& texMulFactor,
    unsigned int renderLayer
)
{
    GLfloat vertices[] = {
    //   Position             Normal               Tangent              Texcoords
        -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f          , 0.0f,           // Top-left
         0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  texMulFactor.x, 0.0f,           // Top-right
         0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  texMulFactor.x, texMulFactor.y, // Bottom-right
        -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f          , texMulFactor.y  // Bottom-left
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    return l3dLoadMesh(
        vertices, 4,
        indices, 6,
        material,
        L3D_POS3_NOR3_TAN3_UV2,
        L3DMat4(), L3D_DRAW_STATIC, L3D_DRAW_TRIANGLES,
        renderLayer
    );
}

L3DHandle l3dLoadCube(
    const L3DHandle& material,
    const L3DVec2& texMulFactor,
    unsigned int renderLayer
)
{
    GLfloat vertices[] = {
    //   Position             Normal               Tangent              Texcoords
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,                      // Front Top-left
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  texMulFactor.x, 0.0f,            // Front Top-right
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  texMulFactor.x, texMulFactor.y,  // Front Bottom-right
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, texMulFactor.y,            // Front Bottom-left

        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  texMulFactor.x, 0.0f,            // Back Top-left
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,                      // Back Top-right
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, texMulFactor.y,            // Back Bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  texMulFactor.x, texMulFactor.y,  // Back Bottom-left

        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,                      // Left Top-left
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  texMulFactor.x, 0.0f,            // Left Top-right
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  texMulFactor.x, texMulFactor.y,  // Left Bottom-left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, texMulFactor.y,            // Left Bottom-right

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,                      // Right Top-left
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  texMulFactor.x, 0.0f,            // Right Top-right
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  texMulFactor.x, texMulFactor.y,  // Right Bottom-left
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, texMulFactor.y,            // Right Bottom-right

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,                      // Top Top-left
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  texMulFactor.x, 0.0f,            // Top Top-right
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  texMulFactor.x, texMulFactor.y,  // Top Bottom-left
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, texMulFactor.y,            // Top Bottom-right

        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,                      // Bottom Top-left
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  texMulFactor.x, 0.0f,            // Bottom Top-right
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  texMulFactor.x, texMulFactor.y,  // Bottom Bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, texMulFactor.y,            // Bottom Bottom-right
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

    return l3dLoadMesh(
        vertices, 24,
        indices, 36,
        material,
        L3D_POS3_NOR3_TAN3_UV2,
        L3DMat4(), L3D_DRAW_STATIC, L3D_DRAW_TRIANGLES,
        renderLayer
    );
}

L3DHandle l3dLoadSkyBox(
    const L3DHandle& material,
    unsigned int renderLayer
)
{
    GLfloat vertices[] = {
    //   Position
         1.0f,  1.0f,  1.0f,    // Right Top-left
         1.0f,  1.0f, -1.0f,    // Right Top-right
         1.0f, -1.0f, -1.0f,    // Right Bottom-left
         1.0f, -1.0f,  1.0f,    // Right Bottom-right

        -1.0f,  1.0f, -1.0f,    // Left Top-left
        -1.0f,  1.0f,  1.0f,    // Left Top-right
        -1.0f, -1.0f,  1.0f,    // Left Bottom-left
        -1.0f, -1.0f, -1.0f,    // Left Bottom-right

        -1.0f,  1.0f, -1.0f,    // Top Top-left
         1.0f,  1.0f, -1.0f,    // Top Top-right
         1.0f,  1.0f,  1.0f,    // Top Bottom-left
        -1.0f,  1.0f,  1.0f,    // Top Bottom-right

        -1.0f, -1.0f,  1.0f,    // Bottom Top-left
         1.0f, -1.0f,  1.0f,    // Bottom Top-right
         1.0f, -1.0f, -1.0f,    // Bottom Bottom-left
        -1.0f, -1.0f, -1.0f,    // Bottom Bottom-right

        -1.0f,  1.0f, -1.0f,    // Back Top-left
         1.0f,  1.0f, -1.0f,    // Back Top-right
         1.0f, -1.0f, -1.0f,    // Back Bottom-right
        -1.0f, -1.0f, -1.0f,    // Back Bottom-left

        -1.0f,  1.0f,  1.0f,    // Front Top-left
         1.0f,  1.0f,  1.0f,    // Front Top-right
         1.0f, -1.0f,  1.0f,    // Front Bottom-right
        -1.0f, -1.0f,  1.0f,    // Front Bottom-left

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

    return l3dLoadMesh(
        vertices, 24,
        indices, 36,
        material,
        L3D_POS3,
        L3DMat4(), L3D_DRAW_STATIC, L3D_DRAW_TRIANGLES,
        renderLayer
    );
}

L3DHandle l3dLoadGrid(
    unsigned int n,
    const L3DHandle& material,
    const L3DVec2& texMulFactor,
    unsigned int renderLayer
)
{
    unsigned int numVertices = (n + 1) * (n + 1);
    unsigned int numIndices = 6 * n * n;

    GLfloat* vertices = (GLfloat*)malloc(numVertices * 11 * sizeof(GLfloat));
    GLuint* indices = (GLuint*)malloc(numIndices * sizeof(GLuint));

    GLfloat k = 1.0f / n;

    // Generate vertices.
    unsigned int v = 0;
    for (unsigned int y = 0; y <= n; ++y) {
        GLfloat ky = k * y;

        for (unsigned int x = 0; x <= n; ++x) {
            GLfloat kx = k * x;

            // Position.
            vertices[v+0]  = kx - 0.5f;
            vertices[v+1]  = ky - 0.5f;
            vertices[v+2]  = 0.0f;
            // Normal.
            vertices[v+3]  = 0.0f;
            vertices[v+4]  = 0.0f;
            vertices[v+5]  = 1.0f;
            // Tangent.
            vertices[v+6]  = 1.0f;
            vertices[v+7]  = 0.0f;
            vertices[v+8]  = 0.0f;
            // Texcoords.
            vertices[v+9]  = texMulFactor.x * x;
            vertices[v+10] = texMulFactor.y * y;

            v += 11;
        }
    }

    // Generate indices.
    unsigned int q = 0;
    for (unsigned int y = 0; y < n; ++y) {
        for (unsigned int x = 0; x < n; ++x) {
            // First triangle.
            indices[q    ] = x + y * n + y;
            indices[q + 1] = x + y * n + y + 1;
            indices[q + 2] = x + (y + 1) * n + y + 1;
            // Second triangle.
            indices[q + 3] = x + (y + 1) * n + y + 1;
            indices[q + 4] = x + (y + 1) * n + y + 2;
            indices[q + 5] = x + y * n + y + 1;

            q += 6;
        }
    }

    return l3dLoadMesh(
        vertices, numVertices,
        indices, numIndices,
        material,
        L3D_POS3_NOR3_TAN3_UV2,
        L3DMat4(), L3D_DRAW_STATIC, L3D_DRAW_TRIANGLES,
        renderLayer
    );
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

unsigned char l3dMeshRenderLayer(const L3DHandle& target)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DMesh* mesh = _renderer->getMesh(target);

    if (mesh)
        return mesh->renderLayer();

    return 0;
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

void l3dSetMeshMaterial(
    const L3DHandle& target,
    const L3DHandle& material
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DMesh* mesh = _renderer->getMesh(target);
    L3DMaterial* mat = _renderer->getMaterial(material);

    if (mesh)
        mesh->setMaterial(mat);
}

void l3dSetMeshRenderLayer(
    const L3DHandle& target,
    unsigned char renderLayer
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DMesh* mesh = _renderer->getMesh(target);

    if (mesh)
        mesh->setRenderLayer(renderLayer);
}

L3DHandle l3dLoadDirectionalLight(
    const L3DVec3& direction,
    const L3DVec4& color,
    unsigned int renderLayerMask
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DLight* light = L3DLight::createDirectionalLight(
        _renderer,
        direction,
        color,
        renderLayerMask
    );

    if (light)
        return light->handle();

    return L3D_INVALID_HANDLE;
}

L3DHandle l3dLoadPointLight(
    const L3DVec3& position,
    const L3DVec4& color,
    const L3DLightAttenuation& attenuation,
    unsigned int renderLayerMask
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DLight* light = L3DLight::createPointLight(
        _renderer,
        position,
        color,
        attenuation,
        renderLayerMask
    );

    if (light)
        return light->handle();

    return L3D_INVALID_HANDLE;
}

L3DHandle l3dLoadSpotLight(
    const L3DVec3& position,
    const L3DVec3& direction,
    const L3DVec4& color,
    const L3DLightAttenuation& attenuation,
    unsigned int renderLayerMask
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DLight* light = L3DLight::createSpotLight(
        _renderer,
        position,
        direction,
        color,
        attenuation,
        renderLayerMask
    );

    if (light)
        return light->handle();

    return L3D_INVALID_HANDLE;
}

int l3dLightType(const L3DHandle& target)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DLight* light = _renderer->getLight(target);

    if (light)
        return light->type;

    return -1;
}

unsigned int l3dLightRenderLayerMask(const L3DHandle& target)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DLight* light = _renderer->getLight(target);

    if (light)
        return light->renderLayerMask();

    return 0;
}

bool l3dIsLightOn(
    const L3DHandle& target
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DLight* light = _renderer->getLight(target);

    if (light)
        return light->isOn();

    return false;
}

void l3dSetLightRenderLayerMask(
    const L3DHandle& target,
    unsigned int renderLayerMask
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DLight* light = _renderer->getLight(target);

    if (light)
        return light->setRenderLayerMask(renderLayerMask);
}

void l3dSetLightDirection(
    const L3DHandle& target,
    const L3DVec3& direction
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DLight* light = _renderer->getLight(target);

    if (light)
        light->direction = direction;
}

void l3dSetLightAttenuation(
    const L3DHandle& target,
    float kc,
    float kl,
    float kq
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DLight* light = _renderer->getLight(target);

    if (light)
        light->attenuation = L3DLightAttenuation(kc, kl, kq);
}

void l3dSetLightColor(
    const L3DHandle& target,
    const L3DVec4& color
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DLight* light = _renderer->getLight(target);

    if (light)
        light->color = color;
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

void l3dLightLookAt(
    const L3DHandle& target,
    const L3DVec3& targetPosition
)
{
    L3D_ASSERT(_renderer != L3D_NULLPTR);

    L3DLight* light = _renderer->getLight(target);

    if (light)
        light->lookAt(targetPosition);
}
