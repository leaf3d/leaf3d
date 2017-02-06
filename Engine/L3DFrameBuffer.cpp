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

#include <leaf3d/L3DRenderer.h>
#include <leaf3d/L3DFrameBuffer.h>

using namespace l3d;

L3DFrameBuffer::L3DFrameBuffer(
    L3DRenderer* renderer,
    const L3DTextureAttachments& textures
) : L3DResource(L3D_FRAME_BUFFER, renderer),
    m_textures(textures)
{
    if (renderer) renderer->addFrameBuffer(this);
}

L3DFrameBuffer::L3DFrameBuffer(
    L3DRenderer* renderer,
    L3DTexture* textureDepthStencilAttachment,
    L3DTexture* textureColorAttachment0
) : L3DResource(L3D_FRAME_BUFFER, renderer)
{
    if (textureDepthStencilAttachment) m_textures[L3D_DEPTH_STENCIL_ATTACHMENT] = textureDepthStencilAttachment;
    if (textureColorAttachment0) m_textures[L3D_COLOR_ATTACHMENT0] = textureColorAttachment0;

    if (renderer) renderer->addFrameBuffer(this);
}

L3DFrameBuffer::~L3DFrameBuffer()
{
}
