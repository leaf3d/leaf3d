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
#include "leaf3d/L3DRenderCommand.h"
#include "leaf3d/L3DResource.h"

namespace l3d
{
    class L3DRenderQueue : public L3DResource
    {
    protected:
        const char *m_name;
        L3DRenderCommandList m_commands;

    public:
        L3DRenderQueue(
            L3DRenderer *renderer,
            const char *name);
        ~L3DRenderQueue();

        const char *name() const { return m_name; }
        unsigned int commandCount() const { return m_commands.size(); }
        const L3DRenderCommandList &commands() const { return m_commands; }

        void appendCommand(L3DRenderCommand *command);
        void appendCommands(const L3DRenderCommandList &commands);

        void execute(L3DRenderer *renderer, L3DCamera *camera);
    };
}

#endif // L3D_L3DRENDERQUEUE_H
