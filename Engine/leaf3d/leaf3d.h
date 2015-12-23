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

int l3dInit();

int l3dTerminate();

void l3dRenderFrame(
    const L3DHandle& camera
);

L3DHandle l3dLoadTexture(
    const TextureType& type,
    const ImageFormat& format,
    unsigned char* data,
    unsigned int width,
    unsigned int height,
    unsigned int depth
);

L3DHandle l3dLoadShader(
    const ShaderType& type,
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
    int value
);

void l3dSetShaderProgramUniformF(
    const L3DHandle& target,
    const char* name,
    float value
);

void l3dSetShaderProgramUniformMat4F(
    const L3DHandle& target,
    const char* name,
    const L3DMat4& value
);

L3DHandle l3dLoadMaterial(
    const char* name,
    const L3DHandle& shaderProgram
);

void l3dAddTextureToMaterial(
    const L3DHandle& target,
    const char* name,
    const L3DHandle& texture
);

L3DHandle l3dLoadCamera(
    const char* name = "Default",
    const L3DMat4& view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    ),
    const L3DMat4& proj = glm::perspective(45.0f, 1.0f, 1.0f, 10.0f)
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
    const L3DVec3& trans
);

void l3dRotateCamera(
    const L3DHandle& target,
    float radians,
    const L3DVec3& direction = glm::vec3(0.0f, 1.0f, 0.0f)
);

L3DHandle l3dLoadMesh(
    float* vertices,
    unsigned int vertexCount,
    unsigned int* indices,
    unsigned int indexCount,
    const L3DHandle& material,
    const VertexFormat& VertexFormat,
    const DrawType& drawType = L3D_DRAW_STATIC,
    const DrawPrimitive& drawPrimitive = L3D_DRAW_TRIANGLES
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
    const L3DVec3& trans
);

void l3dRotateMesh(
    const L3DHandle& target,
    float radians,
    const L3DVec3& direction = glm::vec3(0.0f, 1.0f, 0.0f)
);

#endif // L3D_LEAF3D_H
