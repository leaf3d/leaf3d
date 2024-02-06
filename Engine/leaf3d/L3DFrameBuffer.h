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

#ifndef L3D_L3DFRAMEBUFFER_H
#define L3D_L3DFRAMEBUFFER_H
#pragma once

#include <map>
#include "leaf3d/L3DResource.h"

namespace l3d
{
    class L3DTexture;

    typedef std::map<L3DAttachmentType, L3DTexture *> L3DTextureAttachments;

    class L3DFrameBuffer : public L3DResource
    {
    protected:
        L3DTextureAttachments m_textures;

    public:
        L3DFrameBuffer(
            L3DRenderer *renderer,
            const L3DTextureAttachments &textures = L3DTextureAttachments());
        L3DFrameBuffer(
            L3DRenderer *renderer,
            L3DTexture *textureDepthStencilAttachment,
            L3DTexture *textureColorAttachment0);
        ~L3DFrameBuffer();

        unsigned int textureAttachmentCount() const { return m_textures.size(); }
        L3DTextureAttachments textureAttachments() const { return m_textures; }
    };
}

#endif // L3D_L3DFRAMEBUFFER_H
