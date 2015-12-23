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

#ifndef L3D_L3DBUFFER_H
#define L3D_L3DBUFFER_H
#pragma once

#include "leaf3d/L3DResource.h"

namespace l3d
{
    class L3DBuffer : public L3DResource
    {
    private:
        void*           m_data;
        BufferType      m_type;
        unsigned int    m_size;
        unsigned int    m_stride;
        DrawType        m_drawType;

    public:
        L3DBuffer(
            L3DRenderer* renderer,
            const BufferType& type,
            void* data,
            unsigned int size,
            unsigned int stride = sizeof(unsigned int),
            const DrawType& drawType = L3D_DRAW_STATIC
        );
        ~L3DBuffer();

        BufferType      type() const { return m_type; }
        DrawType        drawType() const { return m_drawType; }
        unsigned int    size() const { return m_size; }
        unsigned int    stride() const { return m_stride; }
        unsigned int    count() const { return (m_stride > 0) ? m_size / m_stride : 0; }
        void*           data() const { return m_data; }

        template<typename T>
        T*              data() const { return static_cast<T*>(m_data); }

        void setStride(unsigned int bytes) { m_stride = bytes; }
        void setDrawType(const DrawType& drawType) { m_drawType = drawType; }
    };
}

#endif // L3D_L3DBUFFER_H
