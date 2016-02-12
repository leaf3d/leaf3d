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

#include <stdio.h>
#include <leaf3d/L3DRenderer.h>
#include <leaf3d/L3DShader.h>
#include <leaf3d/L3DShaderProgram.h>

using namespace l3d;

L3DUniform::L3DUniform(float value)
{
    this->value.valueF = value;
    this->type = L3D_UNIFORM_FLOAT;
}

L3DUniform::L3DUniform(int value)
{
    this->value.valueI = value;
    this->type = L3D_UNIFORM_INT;
}

L3DUniform::L3DUniform(unsigned int value)
{
    this->value.valueUI = value;
    this->type = L3D_UNIFORM_UINT;
}

L3DUniform::L3DUniform(bool value)
{
    this->value.valueB = value;
    this->type = L3D_UNIFORM_BOOL;
}

L3DUniform::L3DUniform(const L3DVec2& value)
{
    int size = value.length() * sizeof(float);
    this->value.valueVec2 = (float*)malloc(size);
    memcpy(this->value.valueVec2, glm::value_ptr(value), size);
    this->type = L3D_UNIFORM_VEC2;
}

L3DUniform::L3DUniform(const L3DVec3& value)
{
    int size = value.length() * sizeof(float);
    this->value.valueVec3 = (float*)malloc(size);
    memcpy(this->value.valueVec3, glm::value_ptr(value), size);
    this->type = L3D_UNIFORM_VEC3;
}

L3DUniform::L3DUniform(const L3DVec4& value)
{
    int size = value.length() * sizeof(float);
    this->value.valueVec4 = (float*)malloc(size);
    memcpy(this->value.valueVec4, glm::value_ptr(value), size);
    this->type = L3D_UNIFORM_VEC4;
}

L3DUniform::L3DUniform(const L3DMat3& value)
{
    int size = value.length() * sizeof(float);
    this->value.valueMat3 = (float*)malloc(size);
    memcpy(this->value.valueMat3, glm::value_ptr(value), size);
    this->type = L3D_UNIFORM_MAT3;
}

L3DUniform::L3DUniform(const L3DMat4& value)
{
    int size = value.length() * sizeof(float);
    this->value.valueMat4 = (float*)malloc(size);
    memcpy(this->value.valueMat4, glm::value_ptr(value), size);
    this->type = L3D_UNIFORM_MAT4;
}

/*L3DUniform::~L3DUniform()
{
    switch (this->type)
    {
    case L3D_UNIFORM_VEC2:
        free(this->value.valueVec2);
        break;
    case L3D_UNIFORM_VEC3:
        free(this->value.valueVec3);
        break;
    case L3D_UNIFORM_VEC4:
        free(this->value.valueVec4);
        break;
    case L3D_UNIFORM_MAT3:
        free(this->value.valueMat3);
        break;
    case L3D_UNIFORM_MAT4:
        free(this->value.valueMat4);
        break;
    default:
        break;
    }
}*/

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

void L3DShaderProgram::setUniform(const char* name, const L3DUniform& value)
{
    m_uniforms[name] = value;
}
