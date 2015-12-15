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

#include <leaf3d/L3DShader.h>

using namespace l3d;

L3DShader::L3DShader(
    const ShaderType& type,
    const char* code
) : L3DResource(L3D_SHADER)
{
    GLuint gl_type = 0;

    switch (type)
    {
    case L3D_SHADER_VERTEX:
        gl_type = GL_VERTEX_SHADER;
        break;
    case L3D_SHADER_FRAGMENT:
        gl_type = GL_FRAGMENT_SHADER;
        break;
    case L3D_SHADER_GEOMETRY:
        gl_type = GL_GEOMETRY_SHADER;
        break;
    }

    GLuint id = glCreateShader(gl_type);
    glShaderSource(id, 1, &code, L3D_NULLPTR);
    glCompileShader(id);

    this->setId(id);
}

L3DShader::~L3DShader()
{
    glDeleteShader(this->id());
}
