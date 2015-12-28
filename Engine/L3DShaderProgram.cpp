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
#include <leaf3d/L3DShader.h>
#include <leaf3d/L3DShaderProgram.h>

using namespace l3d;

L3DShaderProgram::L3DShaderProgram(
    L3DRenderer* renderer,
    L3DShader* vertexShader,
    L3DShader* fragmentShader,
    L3DShader* geometryShader,
    const L3DUniformMap& uniforms
) : L3DResource(L3D_SHADER_PROGRAM, renderer),
    m_vertexShader(vertexShader),
    m_fragmentShader(fragmentShader),
    m_geometryShader(geometryShader),
    m_uniforms(uniforms)
{
    if (renderer) renderer->addShaderProgram(this);
}

void L3DShaderProgram::setUniform(const char* name, float value)
{
    L3DUniform uniform;
    uniform.value.valueF = value;
    uniform.type = L3D_UNIFORM_FLOAT;
    m_uniforms[name] = uniform;
}

void L3DShaderProgram::setUniform(const char* name, int value)
{
    L3DUniform uniform;
    uniform.value.valueI = value;
    uniform.type = L3D_UNIFORM_INT;
    m_uniforms[name] = uniform;
}

void L3DShaderProgram::setUniform(const char* name, unsigned int value)
{
    L3DUniform uniform;
    uniform.value.valueUI = value;
    uniform.type = L3D_UNIFORM_UINT;
    m_uniforms[name] = uniform;
}

void L3DShaderProgram::setUniform(const char* name, const L3DVec2& value)
{
    L3DUniform uniform;
    memcpy(uniform.value.valueVec2, glm::value_ptr(value), 2 * sizeof(float));
    uniform.type = L3D_UNIFORM_VEC2;
    m_uniforms[name] = uniform;
}

void L3DShaderProgram::setUniform(const char* name, const L3DVec3& value)
{
    L3DUniform uniform;
    memcpy(uniform.value.valueVec3, glm::value_ptr(value), 3 * sizeof(float));
    uniform.type = L3D_UNIFORM_VEC3;
    m_uniforms[name] = uniform;
}

void L3DShaderProgram::setUniform(const char* name, const L3DVec4& value)
{
    L3DUniform uniform;
    memcpy(uniform.value.valueVec4, glm::value_ptr(value), 4 * sizeof(float));
    uniform.type = L3D_UNIFORM_VEC4;
    m_uniforms[name] = uniform;
}

void L3DShaderProgram::setUniform(const char* name, const L3DMat4& value)
{
    L3DUniform uniform;
    memcpy(uniform.value.valueMat4, glm::value_ptr(value), 16 * sizeof(float));
    uniform.type = L3D_UNIFORM_MAT4;
    m_uniforms[name] = uniform;
}
