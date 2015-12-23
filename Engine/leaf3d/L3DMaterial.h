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

#ifndef L3D_L3DMATERIAL_H
#define L3D_L3DMATERIAL_H
#pragma once

#include <string>
#include <map>
#include "leaf3d/L3DResource.h"

namespace l3d
{
    class L3DTexture;
    class L3DShaderProgram;

    struct _strcmp
    {
       bool operator()(char const *a, char const *b)
       {
          return std::strcmp(a, b) < 0;
       }
    };

    typedef std::map<const char*, L3DTexture*, _strcmp> L3DTextureRegistry;

    class L3DMaterial : public L3DResource
    {
    private:
        const char* m_name;
        L3DShaderProgram* m_shaderProgram;

    public:
        L3DTextureRegistry textures;

    public:
        L3DMaterial(
            L3DRenderer* renderer,
            const char* name,
            L3DShaderProgram* shaderProgram
        );
        ~L3DMaterial() {}

        const char* name() const { return m_name; }
        L3DShaderProgram* shaderProgram() const { return m_shaderProgram; }
    };
}

#endif // L3D_L3DMATERIAL_H

