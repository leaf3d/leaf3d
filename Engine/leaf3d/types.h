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

#ifndef L3D_TYPES_H
#define L3D_TYPES_H
#pragma once

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "platform.h"

#define L3D_TRUE 1
#define L3D_FALSE 0
#define L3D_NULLPTR 0

#define GLSL(src) "#version 330 core\n" #src

namespace l3d
{
    typedef glm::vec2 L3DVec2;
    typedef glm::vec3 L3DVec3;
    typedef glm::vec4 L3DVec4;
    typedef glm::mat3 L3DMat3;
    typedef glm::mat4 L3DMat4;

    enum L3DVertexFormat
    {
        L3D_POS2 = 2,
        L3D_POS3 = 3,
        L3D_POS2_UV2 = 4,
        L3D_POS3_UV2 = 5,
        L3D_POS3_NOR3_UV2 = 8,
        L3D_POS3_NOR3_TAN3_UV2 = 11,
        L3D_POS3_NOR3_TAN3_UV2_UV2 = 13,
        L3D_POS3_NOR3_TAN3_UV2_UV2_UV2 = 15,
        L3D_POS3_NOR3_TAN3_UV2_UV2_UV2_UV2 = 17
    };

    enum L3DImageFormat
    {
        L3D_RGB = 3,
        L3D_RGBA
    };

    enum L3DDrawType
    {
        L3D_DRAW_STATIC = 0,
        L3D_DRAW_DYNAMIC
    };

    enum L3DDrawPrimitive
    {
        L3D_DRAW_POINTS = 1,
        L3D_DRAW_LINES = 2,
        L3D_DRAW_TRIANGLES = 3
    };

    enum L3DUniformType
    {
        L3D_UNIFORM_FLOAT,
        L3D_UNIFORM_INT,
        L3D_UNIFORM_UINT,
        L3D_UNIFORM_BOOL,
        L3D_UNIFORM_VEC2,
        L3D_UNIFORM_VEC3,
        L3D_UNIFORM_VEC4,
        L3D_UNIFORM_MAT3,
        L3D_UNIFORM_MAT4
    };

    union L3DUniformValue
    {
        float valueF;
        int valueI;
        unsigned int valueUI;
        bool valueB;
        float* valueVec2;
        float* valueVec3;
        float* valueVec4;
        float* valueMat3;
        float* valueMat4;
    };

    enum L3DBlendFactor
    {
        L3D_ZERO,
        L3D_ONE,
        L3D_SRC_COLOR,
        L3D_ONE_MINUS_SRC_COLOR,
        L3D_DST_COLOR,
        L3D_ONE_MINUS_DST_COLOR,
        L3D_SRC_ALPHA,
        L3D_ONE_MINUS_SRC_ALPHA,
        L3D_DST_ALPHA,
        L3D_ONE_MINUS_DST_ALPHA,
        L3D_CONSTANT_COLOR,
        L3D_ONE_MINUS_CONSTANT_COLOR,
        L3D_CONSTANT_ALPHA,
        L3D_ONE_MINUS_CONSTANT_ALPHA
    };

    enum L3DResourceType
    {
        L3D_BUFFER = 0,
        L3D_TEXTURE,
        L3D_SHADER,
        L3D_SHADER_PROGRAM,
        L3D_MATERIAL,
        L3D_CAMERA,
        L3D_LIGHT,
        L3D_MESH,
        L3D_RENDER_QUEUE
    };

    enum L3DBufferType
    {
        L3D_BUFFER_VERTEX = 0,
        L3D_BUFFER_INDEX
    };

    enum L3DTextureType
    {
        L3D_TEXTURE_1D = 0,
        L3D_TEXTURE_2D,
        L3D_TEXTURE_3D
    };

    enum L3DShaderType
    {
        L3D_SHADER_VERTEX = 0,
        L3D_SHADER_FRAGMENT,
        L3D_SHADER_GEOMETRY
    };

    enum L3DRenderCommandType
    {
        L3D_INVALID_RENDER_COMMAND = 0,
        L3D_CLEAR_BUFFERS,
        L3D_SET_DEPTH_TEST,
        L3D_SET_STENCIL_TEST,
        L3D_SET_BLEND,
        L3D_DRAW_MESHES
    };

    enum L3DLightType
    {
        L3D_LIGHT_DIRECTIONAL = 0,
        L3D_LIGHT_POINT,
        L3D_LIGHT_SPOT
    };

    struct L3DLightAttenuation
    {
        L3DLightAttenuation(
            float kc,
            float kl,
            float kq
        ) : kc(kc), kl(kl), kq(kq) {}

        float kc;
        float kl;
        float kq;
    };

    // Almost-opaque resource handle:
    //
    // x----------- repr -----------X
    // x---- type ----xx---- id ----x
    //
    // Could be used in different ways:
    //
    // > handle.repr
    // > handle.data.type
    // > handle.data.id
    typedef union
    {
        unsigned long long repr;
        struct L3DHandleData
        {
            unsigned int type;
            unsigned int id;
        } data;
    } L3DHandle;

    const L3DHandle L3D_INVALID_HANDLE = L3DHandle();
}

#endif // L3D_TYPES_H
