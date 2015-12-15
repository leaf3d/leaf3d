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
    typedef glm::vec3 L3DVec3;

    typedef glm::mat4 L3DMat4;

    enum VertexFormat
    {
        L3D_POS2 = 2,
        L3D_POS3 = 3,
        L3D_POS2_UV2 = 4,
        L3D_POS3_UV2 = 5,
        L3D_POS2_COL3_UV2 = 7,
        L3D_POS3_COL3_UV2 = 8,
        L3D_POS3_NOR3_COL3_UV2 = 10,
        L3D_POS3_NOR3_TAN3_COL3_UV2 = 15,
        L3D_POS3_NOR3_TAN3_BTAN3_COL3_UV2 = 17,
        L3D_POS3_NOR3_TAN3_BTAN3_COL3_UV2_UV2 = 19,
        L3D_POS3_NOR3_TAN3_BTAN3_COL3_UV2_UV2_UV2 = 21,
        L3D_POS3_NOR3_TAN3_BTAN3_COL3_UV2_UV2_UV2_UV2 = 25
    };

    enum ImageFormat
    {
        L3D_RGB = 0,
        L3D_RGBA
    };

    enum DrawType
    {
        L3D_DRAW_STATIC = 0,
        L3D_DRAW_DYNAMIC
    };

    enum DrawPrimitive
    {
        L3D_DRAW_POINTS = 0,
        L3D_DRAW_LINES,
        L3D_DRAW_TRIANGLES
    };

    enum ResourceType
    {
        L3D_TEXTURE = 0,
        L3D_SHADER,
        L3D_SHADER_PROGRAM,
        L3D_MATERIAL,
        L3D_CAMERA,
        L3D_LIGHT,
        L3D_MESH
    };

    enum TextureType
    {
        L3D_TEXTURE_1D = 0,
        L3D_TEXTURE_2D,
        L3D_TEXTURE_3D
    };

    enum ShaderType
    {
        L3D_SHADER_VERTEX = 0,
        L3D_SHADER_FRAGMENT,
        L3D_SHADER_GEOMETRY
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
