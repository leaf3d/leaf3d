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
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "platform.h"

#define L3D_TRUE 1
#define L3D_FALSE 0
#define L3D_NULLPTR 0

#define L3D_BIT(pos) (1<<(pos))
#define L3D_SET_BIT(var, pos, enable) (enable ? var | L3D_BIT(pos) : var & ~L3D_BIT(pos))
#define L3D_TEST_BIT(var, pos) (((var) & L3D_BIT(pos)) > 0)

#define L3D_SKYBOX_MESH_RENDERLAYER 0
#define L3D_OPAQUE_MESH_RENDERLAYER 1
#define L3D_ALPHA_BLEND_MESH_RENDERLAYER 2
#define L3D_POSTPROCESSING_RENDERLAYER 255

#define L3D_DEFAULT_LIGHT_RENDERLAYER_MASK L3D_BIT(L3D_OPAQUE_MESH_RENDERLAYER) | L3D_BIT(L3D_ALPHA_BLEND_MESH_RENDERLAYER)

#define GLSL(src) "#version 330 core\n" #src

namespace l3d
{
    typedef L3D_API glm::vec2 L3DVec2;
    typedef L3D_API glm::vec3 L3DVec3;
    typedef L3D_API glm::vec4 L3DVec4;
    typedef L3D_API glm::mat3 L3DMat3;
    typedef L3D_API glm::mat4 L3DMat4;

    enum L3D_API L3DVertexAttribute
    {
        L3D_VERTEX_POSITION = 0,
        L3D_VERTEX_NORMAL,
        L3D_VERTEX_TANGENT,
        L3D_VERTEX_UV0,
        L3D_VERTEX_UV1,
        L3D_VERTEX_UV2,
        L3D_VERTEX_UV3,
        L3D_MAX_VERTEX_ATTRIBUTE
    };

    enum L3D_API L3DInstanceAttribute
    {
        L3D_INSTANCE_POSITION = L3D_MAX_VERTEX_ATTRIBUTE,
        L3D_INSTANCE_UV,
        L3D_INSTANCE_MATRIX,
        L3D_MAX_INSTANCE_ATTRIBUTE
    };

    enum L3D_API L3DVertexFormat
    {
        L3D_INVALID_VERTEX_FORMAT = 0,
        L3D_VERTEX_POS2 = 2,
        L3D_VERTEX_POS3 = 3,
        L3D_VERTEX_POS2_UV2 = 4,
        L3D_VERTEX_POS3_UV2 = 5,
        L3D_VERTEX_POS3_UV3 = 6,
        L3D_VERTEX_POS3_NOR3_UV2 = 8,
        L3D_VERTEX_POS3_NOR3_UV3 = 9,
        L3D_VERTEX_POS3_NOR3_UV2_UV2 = 10,
        L3D_VERTEX_POS3_NOR3_TAN3_UV2 = 11,
        L3D_VERTEX_POS3_NOR3_TAN3_UV3 = 12,
        L3D_VERTEX_POS3_NOR3_TAN3_UV2_UV2 = 13,
        L3D_VERTEX_POS3_NOR3_TAN3_UV2_UV2_UV2 = 15,
        L3D_VERTEX_POS3_NOR3_TAN3_UV2_UV2_UV2_UV2 = 17,
        L3D_MAX_VERTEX_FORMAT
    };

    enum L3D_API L3DInstanceFormat
    {
      L3D_INVALID_INSTANCE_FORMAT = 0,
      L3D_INSTANCE_POS2 = 2,
      L3D_INSTANCE_POS3 = 3,
      L3D_INSTANCE_POS2_UV2 = 4,
      L3D_INSTANCE_POS3_UV2 = 5,
      L3D_INSTANCE_TRANS4_TRANS4_TRANS4_TRANS4 = 16,
      L3D_INSTANCE_TRANS4_TRANS4_TRANS4_TRANS4_UV2 = 18,
      L3D_MAX_INSTANCE_FORMAT
    };

    enum L3D_API L3DImageFormat
    {
        L3D_UNKNOWN = 0,
        L3D_RGB = 3,
        L3D_RGBA,
        L3D_DEPTH24_STENCIL8
    };

    enum L3D_API L3DPixelFormat
    {
        L3D_UNSIGNED_BYTE,
        L3D_UNSIGNED_INT_24_8
    };

    enum L3D_API L3DImageMinFilter
    {
        L3D_MIN_NEAREST = 0,
        L3D_MIN_LINEAR,
        L3D_MIN_NEAREST_MIPMAP_NEAREST,
        L3D_MIN_LINEAR_MIPMAP_NEAREST,
        L3D_MIN_NEAREST_MIPMAP_LINEAR,
        L3D_MIN_LINEAR_MIPMAP_LINEAR
    };

    enum L3D_API L3DImageMagFilter
    {
        L3D_MAG_NEAREST = 0,
        L3D_MAG_LINEAR
    };

    enum L3D_API L3DImageWrapMethod
    {
        L3D_CLAMP_TO_EDGE = 0,
        L3D_CLAMP_TO_BORDER,
        L3D_REPEAT,
        L3D_MIRRORED_REPEAT,
        L3D_MIRROR_CLAMP_TO_EDGE,
    };

    enum L3D_API L3DDrawType
    {
        L3D_DRAW_STATIC = 0,
        L3D_DRAW_DYNAMIC
    };

    enum L3D_API L3DDrawPrimitive
    {
        L3D_DRAW_POINTS = 1,
        L3D_DRAW_LINES = 2,
        L3D_DRAW_TRIANGLES = 3
    };

    enum L3D_API L3DAttachmentType
    {
        L3D_DEPTH_STENCIL_ATTACHMENT,
        L3D_COLOR_ATTACHMENT0,
        L3D_COLOR_ATTACHMENT1,
        L3D_COLOR_ATTACHMENT2,
        L3D_COLOR_ATTACHMENT3,
        L3D_COLOR_ATTACHMENT4,
        L3D_COLOR_ATTACHMENT5,
        L3D_COLOR_ATTACHMENT6,
        L3D_COLOR_ATTACHMENT7,
        L3D_COLOR_ATTACHMENT8,
        L3D_COLOR_ATTACHMENT9,
        L3D_COLOR_ATTACHMENT10,
        L3D_COLOR_ATTACHMENT11,
        L3D_COLOR_ATTACHMENT12,
        L3D_COLOR_ATTACHMENT13,
        L3D_COLOR_ATTACHMENT14,
        L3D_COLOR_ATTACHMENT15
    };

    enum L3D_API L3DUniformType
    {
        L3D_UNIFORM_INVALID = 0,
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

    union L3D_API L3DUniformValue
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

    enum L3D_API L3DDepthFactor
    {
        L3D_LESS = 0,
        L3D_EQUAL
    };

    enum L3D_API L3DBlendFactor
    {
        L3D_ZERO = 0,
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

    enum L3D_API L3DCullFace
    {
      L3D_FRONT_FACE = 0,
      L3D_BACK_FACE,
      L3D_BOTH_FACES
    };

    enum L3D_API L3DResourceType
    {
        L3D_BUFFER = 0,
        L3D_TEXTURE,
        L3D_SHADER,
        L3D_SHADER_PROGRAM,
        L3D_FRAME_BUFFER,
        L3D_MATERIAL,
        L3D_CAMERA,
        L3D_LIGHT,
        L3D_MESH,
        L3D_RENDER_QUEUE
    };

    enum L3D_API L3DBufferType
    {
        L3D_BUFFER_VERTEX = 0,
        L3D_BUFFER_INDEX,
        L3D_BUFFER_INSTANCE
    };

    enum L3D_API L3DTextureType
    {
        L3D_TEXTURE_1D = 0,
        L3D_TEXTURE_2D,
        L3D_TEXTURE_3D,
        L3D_TEXTURE_CUBE_MAP
    };

    enum L3D_API L3DShaderType
    {
        L3D_SHADER_VERTEX = 0,
        L3D_SHADER_FRAGMENT,
        L3D_SHADER_GEOMETRY
    };

    enum L3D_API L3DRenderCommandType
    {
        L3D_INVALID_RENDER_COMMAND = 0,
        L3D_SWITCH_FRAME_BUFFER,
        L3D_CLEAR_BUFFERS,
        L3D_SET_DEPTH_TEST,
        L3D_SET_DEPTH_MASK,
        L3D_SET_STENCIL_TEST,
        L3D_SET_BLEND,
        L3D_DRAW_MESHES
    };

    enum L3D_API L3DLightType
    {
        L3D_LIGHT_DIRECTIONAL = 0,
        L3D_LIGHT_POINT,
        L3D_LIGHT_SPOT
    };

    struct L3D_API L3DLightAttenuation
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
    // x-------------------- repr ---------------------X
    // |-- type --|-- flags --|---------- id ----------|
    //
    // Could be used in different ways:
    //
    // > handle.repr
    // > handle.data.type   (8bits)
    // > handle.data.flags  (8bits, bitfield)
    // > handle.data.id     (16bits)
    typedef L3D_API union
    {
        unsigned int repr;
        struct L3DHandleData
        {
            unsigned char type;
            unsigned char flags;
            unsigned short int id;
        } data;
    } L3DHandle;

    L3D_API const L3DHandle L3D_INVALID_HANDLE = L3DHandle();
}

#endif // L3D_TYPES_H
