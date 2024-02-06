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

#ifndef L3D_L3DSHADERPROGRAM_H
#define L3D_L3DSHADERPROGRAM_H
#pragma once

#include <map>
#include <string>
#include "leaf3d/L3DResource.h"

namespace l3d
{
    class L3DShader;

    class L3DUniform
    {
    public:
        L3DUniformValue value;
        L3DUniformType type;

    public:
        L3DUniform() : type(L3D_UNIFORM_INVALID) { this->value.valueUI = 0; }
        L3DUniform(float value);
        L3DUniform(int value);
        L3DUniform(unsigned int value);
        L3DUniform(bool value);
        L3DUniform(const L3DVec2 &value);
        L3DUniform(const L3DVec3 &value);
        L3DUniform(const L3DVec4 &value);
        L3DUniform(const L3DMat3 &value);
        L3DUniform(const L3DMat4 &value);

        bool is(const L3DUniformType &type) const { return this->type == type; }
    };

    typedef std::map<std::string, L3DUniform> L3DUniformMap;
    typedef std::map<int, std::string> L3DAttributeMap;

    class L3DShaderProgram : public L3DResource
    {
    protected:
        L3DShader *m_vertexShader;
        L3DShader *m_fragmentShader;
        L3DShader *m_geometryShader;
        L3DUniformMap m_uniforms;
        L3DAttributeMap m_attributes;

    public:
        L3DShaderProgram(
            L3DRenderer *renderer,
            L3DShader *vertexShader,
            L3DShader *fragmentShader,
            L3DShader *geometryShader = L3D_NULLPTR,
            const L3DUniformMap &uniforms = L3DUniformMap(),
            const L3DAttributeMap &attributeMap = L3DAttributeMap());
        ~L3DShaderProgram() {}

        L3DShader *vertexShader() const { return m_vertexShader; }
        L3DShader *fragmentShader() const { return m_fragmentShader; }
        L3DShader *geometryShader() const { return m_geometryShader; }
        L3DUniformMap uniforms() const { return m_uniforms; }
        unsigned int uniformCount() const { return m_uniforms.size(); }
        L3DAttributeMap attributes() const { return m_attributes; }
        unsigned int attributeCount() const { return m_attributes.size(); }

        void setUniform(const char *name, const L3DUniform &value);
        void removeUniform(const char *name);

        void addAttribute(int attribute, const char *name);
        void removeAttribute(int attribute);
    };
}

#endif // L3D_L3DSHADERPROGRAM_H
