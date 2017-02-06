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

#ifndef L3D_L3DTEXTURE_H
#define L3D_L3DTEXTURE_H
#pragma once

#include "leaf3d/L3DResource.h"

namespace l3d
{
    class L3DTexture : public L3DResource
    {
    protected:
        L3DTextureType      m_type;
        L3DImageFormat      m_format;
        L3DPixelFormat      m_pixelFormat;
        unsigned char*      m_data;
        unsigned int        m_width;
        unsigned int        m_height;
        unsigned int        m_depth;
        bool                m_useMipmap;
        L3DImageMinFilter   m_minFilter;
        L3DImageMagFilter   m_magFilter;
        L3DImageWrapMethod  m_wrapS;
        L3DImageWrapMethod  m_wrapT;
        L3DImageWrapMethod  m_wrapR;

    public:
        L3DTexture(
            L3DRenderer* renderer,
            const L3DTextureType& type,
            const L3DImageFormat& format,
            unsigned char* data,
            unsigned int width,
            unsigned int height = 0,
            unsigned int depth = 0,
            bool mipmap = true,
            const L3DPixelFormat& pixelFormat = L3D_UNSIGNED_BYTE,
            const L3DImageMinFilter& minFilter = L3D_MIN_NEAREST_MIPMAP_LINEAR,
            const L3DImageMagFilter& magFilter = L3D_MAG_LINEAR,
            const L3DImageWrapMethod& wrapS = L3D_REPEAT,
            const L3DImageWrapMethod& wrapT = L3D_REPEAT,
            const L3DImageWrapMethod& wrapR = L3D_REPEAT
        );
        ~L3DTexture();

        L3DTextureType      type() const { return m_type; }
        L3DImageFormat      format() const { return m_format; }
        L3DPixelFormat      pixelFormat() const { return m_pixelFormat; }
        unsigned char*      data() const { return m_data; }
        unsigned int        width() const { return m_width; }
        unsigned int        height() const { return m_height; }
        unsigned int        depth() const { return m_depth; }
        unsigned int        size() const;
        bool                useMipmap() const { return m_useMipmap; }
        L3DImageMinFilter   minFilter() const { return m_minFilter; }
        L3DImageMagFilter   magFilter() const { return m_magFilter; }
        L3DImageWrapMethod  wrapS() const { return m_wrapS; }
        L3DImageWrapMethod  wrapT() const { return m_wrapT; }
        L3DImageWrapMethod  wrapR() const { return m_wrapR; }

    };
}

#endif // L3D_L3DTEXTURE_H
