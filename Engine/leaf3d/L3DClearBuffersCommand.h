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

#ifndef L3D_L3DCLEARBUFFERSCOMMAND_H
#define L3D_L3DCLEARBUFFERSCOMMAND_H
#pragma once

#include "leaf3d/L3DRenderCommand.h"
#include "leaf3d/types.h"

namespace l3d
{
    class L3DClearBuffersCommand : public L3DRenderCommand
    {
    protected:
        bool m_colorBuffer;
        bool m_depthBuffer;
        bool m_stencilBuffer;
        L3DVec4 m_clearColor;

    public:
        L3DClearBuffersCommand(
            bool colorBuffer = true,
            bool depthBuffer = true,
            bool stencilBuffer = true,
            const L3DVec4 &clearColor = L3DVec4(1, 1, 1, 1)) : m_colorBuffer(colorBuffer),
                                                               m_depthBuffer(depthBuffer),
                                                               m_stencilBuffer(stencilBuffer),
                                                               m_clearColor(clearColor) {}

        void execute(L3DRenderer *renderer, L3DCamera *camera);
    };
}

#endif // L3D_L3DCLEARBUFFERSCOMMAND_H
