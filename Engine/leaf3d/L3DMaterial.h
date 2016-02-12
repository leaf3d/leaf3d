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

    typedef std::map<std::string,L3DVec3> L3DColorRegistry;
    typedef std::map<std::string,float> L3DParameterRegistry;
    typedef std::map<std::string,L3DTexture*> L3DTextureRegistry;

    class L3DMaterial : public L3DResource
    {
    private:
        const char* m_name;
        L3DShaderProgram* m_shaderProgram;

    public:
        L3DColorRegistry colors;
        L3DParameterRegistry params;
        L3DTextureRegistry textures;

    public:
        L3DMaterial(
            L3DRenderer* renderer,
            const char* name,
            L3DShaderProgram* shaderProgram,
            const L3DColorRegistry& colors,
            const L3DParameterRegistry& params,
            const L3DTextureRegistry& textures
        );
        ~L3DMaterial() {}

        const char* name() const { return m_name; }
        L3DShaderProgram* shaderProgram() const { return m_shaderProgram; }

        static L3DMaterial* createBlinnPhongMaterial(
            L3DRenderer* renderer,
            const char* name,
            L3DShaderProgram* shaderProgram,
            const L3DVec3& diffuse = L3DVec3(1.0f, 1.0f, 1.0f),
            const L3DVec3& ambient = L3DVec3(1.0f, 1.0f, 1.0f),
            const L3DVec3& specular = L3DVec3(1.0f, 1.0f, 1.0f),
            float shininess = 32.0f,
            L3DTexture* diffuseMap = 0,
            L3DTexture* specularMap = 0,
            L3DTexture* normalMap = 0
        );
    };
}

#endif // L3D_L3DMATERIAL_H

