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

#include <leaf3d/L3DShaderProgram.h>
#include <leaf3d/L3DMaterial.h>

using namespace l3d;

L3DMaterial::L3DMaterial(
    const char* name,
    L3DShaderProgram* shaderProgram
) : L3DResource(L3D_MATERIAL),
    m_name(name),
    m_shaderProgram(shaderProgram)
{
    // TODO: id as hash of name.
    // this->setId(hash(name));
}

L3DMaterial::~L3DMaterial()
{
}

void L3DMaterial::setUniform(const char* name, int value)
{
    L3DShaderProgram* shaderProgram = this->shaderProgram();

    if (shaderProgram)
    {
        glUseProgram(shaderProgram->id());
        GLint unifLoc = glGetUniformLocation(shaderProgram->id(), name);
        glUniform1f(unifLoc, value);
    }
}

void L3DMaterial::setUniform(const char* name, const float value)
{
    L3DShaderProgram* shaderProgram = this->shaderProgram();

    if (shaderProgram)
    {
        glUseProgram(shaderProgram->id());
        GLint unifLoc = glGetUniformLocation(shaderProgram->id(), name);
        glUniform1f(unifLoc, value);
    }
}

void L3DMaterial::setUniform(const char* name, const L3DMat4& value)
{
    L3DShaderProgram* shaderProgram = this->shaderProgram();

    if (shaderProgram)
    {
        glUseProgram(shaderProgram->id());
        GLint unifLoc = glGetUniformLocation(shaderProgram->id(), name);
        glUniformMatrix4fv(unifLoc, 1, GL_FALSE, glm::value_ptr(value));
    }
}

