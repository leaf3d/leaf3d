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

#ifndef L3D_LEAF3D_H
#define L3D_LEAF3D_H
#pragma once

#include "leaf3d/types.h"

using namespace l3d;

/* Init & terminate ***********************************************************/

int l3dInit();

int l3dTerminate();

/* Rendering ******************************************************************/

void l3dRenderFrame(
    const L3DHandle& camera,
    const L3DHandle& renderQueue
);

L3DHandle l3dLoadForwardRenderQueue(
    unsigned int width,
    unsigned int height,
    const L3DVec4& clearColor = L3DVec4(1, 1, 1, 1),
    const L3DHandle& screenFragmentShader = L3D_INVALID_HANDLE
);

/* Textures *******************************************************************/

L3DHandle l3dLoadTexture(
    const L3DTextureType& type,
    const L3DImageFormat& format,
    unsigned char* data,
    unsigned int width,
    unsigned int height,
    unsigned int depth,
    bool mipmap = true,
    const L3DPixelFormat& pixelFormat = L3D_UNSIGNED_BYTE,
    const L3DImageMinFilter& minFilter = L3D_MIN_NEAREST_MIPMAP_LINEAR,
    const L3DImageMagFilter& magFilter = L3D_MAG_LINEAR,
    const L3DImageWrapMethod& wrapS = L3D_REPEAT,
    const L3DImageWrapMethod& wrapT = L3D_REPEAT,
    const L3DImageWrapMethod& wrapR = L3D_REPEAT
);

/* Shaders ********************************************************************/

L3DHandle l3dLoadShader(
    const L3DShaderType& type,
    const char* code
);

L3DHandle l3dLoadShaderProgram(
    const L3DHandle& vertexShader,
    const L3DHandle& fragmentShader,
    const L3DHandle& geometryShader = L3D_INVALID_HANDLE
);

void l3dSetShaderProgramUniformI(
    const L3DHandle& target,
    const char* name,
    int value,
    int index = -1
);

void l3dSetShaderProgramUniformUI(
    const L3DHandle& target,
    const char* name,
    unsigned int value,
    int index = -1
);

void l3dSetShaderProgramUniformB(
    const L3DHandle& target,
    const char* name,
    bool value,
    int index = -1
);

void l3dSetShaderProgramUniformF(
    const L3DHandle& target,
    const char* name,
    float value,
    int index = -1
);

void l3dSetShaderProgramUniformVec2(
    const L3DHandle& target,
    const char* name,
    const L3DVec2& value,
    int index = -1
);

void l3dSetShaderProgramUniformVec3(
    const L3DHandle& target,
    const char* name,
    const L3DVec3& value,
    int index = -1
);

void l3dSetShaderProgramUniformVec4(
    const L3DHandle& target,
    const char* name,
    const L3DVec4& value,
    int index = -1
);

void l3dSetShaderProgramUniformMat3(
    const L3DHandle& target,
    const char* name,
    const L3DMat3& value,
    int index = -1
);

void l3dSetShaderProgramUniformMat4(
    const L3DHandle& target,
    const char* name,
    const L3DMat4& value,
    int index = -1
);

/* Framebuffers ***************************************************************/

L3DHandle l3dLoadFrameBuffer(
    const L3DHandle& textureDepthStencilAttachment,
    const L3DHandle& textureColorAttachment0,
    const L3DHandle& textureColorAttachment1 = L3D_INVALID_HANDLE,
    const L3DHandle& textureColorAttachment2 = L3D_INVALID_HANDLE,
    const L3DHandle& textureColorAttachment3 = L3D_INVALID_HANDLE,
    const L3DHandle& textureColorAttachment4 = L3D_INVALID_HANDLE,
    const L3DHandle& textureColorAttachment5 = L3D_INVALID_HANDLE,
    const L3DHandle& textureColorAttachment6 = L3D_INVALID_HANDLE,
    const L3DHandle& textureColorAttachment7 = L3D_INVALID_HANDLE,
    const L3DHandle& textureColorAttachment8 = L3D_INVALID_HANDLE,
    const L3DHandle& textureColorAttachment9 = L3D_INVALID_HANDLE,
    const L3DHandle& textureColorAttachment10 = L3D_INVALID_HANDLE,
    const L3DHandle& textureColorAttachment11 = L3D_INVALID_HANDLE,
    const L3DHandle& textureColorAttachment12 = L3D_INVALID_HANDLE,
    const L3DHandle& textureColorAttachment13 = L3D_INVALID_HANDLE,
    const L3DHandle& textureColorAttachment14 = L3D_INVALID_HANDLE,
    const L3DHandle& textureColorAttachment15 = L3D_INVALID_HANDLE
);

/* Materials ******************************************************************/

L3DHandle l3dLoadMaterial(
    const char* name,
    const L3DHandle& shaderProgram,
    const L3DVec3& diffuse = L3DVec3(1.0f, 1.0f, 1.0f),
    const L3DVec3& ambient = L3DVec3(1.0f, 1.0f, 1.0f),
    const L3DVec3& specular = L3DVec3(1.0f, 1.0f, 1.0f),
    float shininess = 32.0f
);

void l3dAddTextureToMaterial(
    const L3DHandle& target,
    const char* name,
    const L3DHandle& texture
);

/* Cameras ********************************************************************/

L3DHandle l3dLoadCamera(
    const char* name = "Default",
    const L3DMat4& view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 8.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    ),
    const L3DMat4& proj = glm::perspective(45.0f, 1.0f, 1.0f, 100.0f)
);

L3DMat4 l3dGetCameraView(
    const L3DHandle& target
);

void l3dSetCameraView(
    const L3DHandle& target,
    const L3DMat4& view
);

L3DMat4 l3dGetCameraProj(
    const L3DHandle& target
);

void l3dSetCameraProj(
    const L3DHandle& target,
    const L3DMat4& proj
);

void l3dTranslateCamera(
    const L3DHandle& target,
    const L3DVec3& movement
);

void l3dRotateCamera(
    const L3DHandle& target,
    float radians,
    const L3DVec3& direction = glm::vec3(0.0f, 1.0f, 0.0f)
);

/* Meshes *********************************************************************/

L3DHandle l3dLoadMesh(
    float* vertices,
    unsigned int vertexCount,
    unsigned int* indices,
    unsigned int indexCount,
    const L3DHandle& material,
    const L3DVertexFormat& L3DVertexFormat,
    const L3DMat4& transMatrix = L3DMat4(),
    const L3DDrawType& drawType = L3D_DRAW_STATIC,
    const L3DDrawPrimitive& drawPrimitive = L3D_DRAW_TRIANGLES,
    unsigned int renderLayer = 0
);

L3DHandle l3dLoadQuad(
    const L3DHandle& material,
    const L3DVec2& texMulFactor = L3DVec2(1, 1),
    unsigned int renderLayer = 0
);

L3DHandle l3dLoadCube(
    const L3DHandle& material,
    const L3DVec2& texMulFactor = L3DVec2(1, 1),
    unsigned int renderLayer = 0
);

L3DHandle l3dLoadGrid(
    unsigned int n,
    const L3DHandle& material,
    const L3DVec2& texMulFactor = L3DVec2(1, 1),
    unsigned int renderLayer = 0
);

L3DMat4 l3dGetMeshTrans(
    const L3DHandle& target
);

void l3dSetMeshTrans(
    const L3DHandle& target,
    const L3DMat4& trans
);

void l3dTranslateMesh(
    const L3DHandle& target,
    const L3DVec3& movement
);

void l3dRotateMesh(
    const L3DHandle& target,
    float radians,
    const L3DVec3& direction = glm::vec3(0.0f, 1.0f, 0.0f)
);

void l3dScaleMesh(
    const L3DHandle& target,
    const L3DVec3& factor
);

/* Lights *********************************************************************/

L3DHandle l3dLoadDirectionalLight(
    const L3DVec3& direction,
    const L3DVec4& color = L3DVec4(1, 1, 1, 1),
    unsigned int renderLayer = 0
);

L3DHandle l3dLoadPointLight(
    const L3DVec3& position,
    const L3DVec4& color = L3DVec4(1, 1, 1, 1),
    const L3DLightAttenuation& attenuation = L3DLightAttenuation(1, 0.045f, 0.0075f),
    unsigned int renderLayer = 0
);

L3DHandle l3dLoadSpotLight(
    const L3DVec3& position,
    const L3DVec3& direction = L3DVec3(0, -1, 0),
    const L3DVec4& color = L3DVec4(1, 1, 1, 1),
    const L3DLightAttenuation& attenuation = L3DLightAttenuation(1, 0.045f, 0.0075f),
    unsigned int renderLayer = 0
);

int l3dLightType(const L3DHandle& target);

bool l3dIsLightOn(const L3DHandle& target);

void l3dTranslateLight(
    const L3DHandle& target,
    const L3DVec3& movement
);

void l3dSetLightDirection(
    const L3DHandle& target,
    const L3DVec3& direction
);

void l3dSetLightAttenuation(
    const L3DHandle& target,
    float kc,
    float kl,
    float kq
);

void l3dSetLightColor(
    const L3DHandle& target,
    const L3DVec4& color
);

void l3dLightLookAt(
    const L3DHandle& target,
    const L3DVec3& targetPosition
);

#endif // L3D_LEAF3D_H
