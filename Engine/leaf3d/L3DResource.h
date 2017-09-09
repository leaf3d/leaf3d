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

#ifndef L3D_L3DRESOURCE_H
#define L3D_L3DRESOURCE_H
#pragma once

#include "leaf3d/types.h"

namespace l3d
{
    class L3DRenderer;

    class L3DResource
    {
    private:
        L3DHandle       m_handle;
        L3DRenderer*    m_renderer;

    public:
        virtual ~L3DResource();

        L3DHandle           handle() const { return m_handle; }
        L3DResourceType     resourceType() const { return (L3DResourceType)m_handle.data.type; }
        unsigned short int  id() const { return m_handle.data.id; }
        unsigned char       flags() const { return m_handle.data.flags; }
        bool                hasFlag(unsigned char bit) const { return L3D_TEST_BIT(m_handle.data.flags, bit); }
        L3DRenderer*        renderer() const { return m_renderer; }

    protected:
        L3DResource(L3DRenderer* renderer = L3D_NULLPTR);
        L3DResource(
            const L3DResourceType& type,
            L3DRenderer* renderer  = L3D_NULLPTR
        );

        void setId(unsigned short int id) { m_handle.data.id = id; }
        void setFlags(unsigned char flags) { m_handle.data.flags = flags; }
        void setFlag(unsigned char flag, bool enable = true) { L3D_SET_BIT(m_handle.data.flags, flag, enable); }

        friend class L3DRenderer;
    };
}

#endif // L3D_L3DRESOURCE_H
