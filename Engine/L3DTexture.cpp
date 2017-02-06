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

#include <leaf3d/L3DTexture.h>
#include <leaf3d/L3DRenderer.h>

using namespace l3d;

L3DTexture::L3DTexture(
    L3DRenderer* renderer,
    const L3DTextureType& type,
    const L3DImageFormat& format,
    unsigned char* data,
    unsigned int width,
    unsigned int height,
    unsigned int depth,
    bool mipmap,
    const L3DPixelFormat& pixelFormat,
    const L3DImageMinFilter& minFilter,
    const L3DImageMagFilter& magFilter,
    const L3DImageWrapMethod& wrapS,
    const L3DImageWrapMethod& wrapT,
    const L3DImageWrapMethod& wrapR
) : L3DResource(L3D_TEXTURE, renderer),
    m_type(type),
    m_format(format),
    m_pixelFormat(pixelFormat),
    m_data(data),
    m_width(width),
    m_height(height),
    m_depth(depth),
    m_useMipmap(mipmap),
    m_minFilter(minFilter),
    m_magFilter(magFilter),
    m_wrapS(wrapS),
    m_wrapT(wrapT),
    m_wrapR(wrapR)
{
    if (data)
    {
        unsigned int size = this->size();
        m_data = (unsigned char*)memcpy(malloc(size), data, size);
    }

    if (renderer) renderer->addTexture(this);
}

L3DTexture::~L3DTexture()
{
    free(m_data);
}

unsigned int L3DTexture::size() const
{
    unsigned int size = m_width * sizeof(unsigned char);

    if (m_height) size *= m_height;
    if (m_depth) size *= m_depth;

    switch(this->format())
    {
    case L3D_RGB:
    case L3D_DEPTH24_STENCIL8:
        size *= 3; // bytes.
        break;
    case L3D_RGBA:
        size *= 4; // bytes.
        break;
    default:
        break;
    }

    return size;
}
