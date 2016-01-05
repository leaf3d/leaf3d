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

#ifndef L3D_L3DRENDERQUEUE_H
#define L3D_L3DRENDERQUEUE_H
#pragma once

#include <queue>
#include "leaf3d/L3DResource.h"

namespace l3d
{
    class L3DRenderCommand
    {
    protected:
        L3DRenderCommandType m_type;

    public:
        L3DRenderCommandType type() const { return m_type; }

    protected:
        L3DRenderCommand() : m_type(L3D_INVALID_RENDER_COMMAND) {}
        L3DRenderCommand(const L3DRenderCommandType& type) : m_type(type) {}
    };

    typedef std::vector<L3DRenderCommand*> L3DRenderCommandList;

    class L3DClearBuffersCommand : public L3DRenderCommand
    {
    public:
        bool colorBuffer;
        bool depthBuffer;
        bool stencilBuffer;
        L3DVec4 clearColor;

    public:
        L3DClearBuffersCommand(
            bool colorBuffer = true,
            bool depthBuffer = true,
            bool stencilBuffer = true,
            const L3DVec4& clearColor = L3DVec4(1, 1, 1, 1)
        ) : L3DRenderCommand(L3D_CLEAR_BUFFERS),
            colorBuffer(colorBuffer),
            depthBuffer(depthBuffer),
            stencilBuffer(stencilBuffer),
            clearColor(clearColor) {}
    };

    class L3DSetDepthTestCommand : public L3DRenderCommand
    {
    public:
        bool enable;

    public:
        L3DSetDepthTestCommand(
            bool enable = true
        ) : L3DRenderCommand(L3D_SET_DEPTH_TEST),
            enable(enable) {}
    };

    class L3DSetStencilTestCommand : public L3DRenderCommand
    {
    public:
        bool enable;

    public:
        L3DSetStencilTestCommand(
            bool enable = true
        ) : L3DRenderCommand(L3D_SET_STENCIL_TEST),
            enable(enable) {}
    };

    class L3DSetBlendCommand : public L3DRenderCommand
    {
    public:
        bool enable;
        L3DBlendFactor srcFactor;
        L3DBlendFactor dstFactor;

    public:
        L3DSetBlendCommand(
            bool enable = true,
            const L3DBlendFactor& srcFactor = L3D_SRC_ALPHA,
            const L3DBlendFactor& dstFactor = L3D_ONE_MINUS_SRC_ALPHA
        ) : L3DRenderCommand(L3D_SET_BLEND),
            enable(enable),
            srcFactor(srcFactor),
            dstFactor(dstFactor) {}
    };

    class L3DDrawMeshesCommand : public L3DRenderCommand
    {
    public:
        L3DDrawMeshesCommand() : L3DRenderCommand(L3D_DRAW_MESHES) {}
    };

    class L3DRenderQueue : public L3DResource
    {
    protected:
        const char* m_name;
        L3DRenderCommandList m_commands;

    public:
        L3DRenderQueue(
            L3DRenderer* renderer,
            const char* name
        );
        ~L3DRenderQueue();

        const char* name() const { return m_name; }
        unsigned int commandCount() const { return m_commands.size(); }
        const L3DRenderCommandList& commands() const { return m_commands; }

        void addClearBuffersCommand(
            bool colorBuffer = true,
            bool depthBuffer = true,
            bool stencilBuffer = true,
            const L3DVec4& clearColor = L3DVec4(1, 1, 1, 1)
        );
        void addSetDepthTestCommand(
            bool enable = true
        );
        void addSetStencilTestCommand(
            bool enable = true
        );
        void addSetBlendCommand(
            bool enable = true,
            const L3DBlendFactor& srcFactor = L3D_SRC_ALPHA,
            const L3DBlendFactor& dstFactor = L3D_ONE_MINUS_SRC_ALPHA
        );
        void addDrawMeshesCommand();
    };
}

#endif // L3D_L3DRENDERQUEUE_H

