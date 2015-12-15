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

using namespace l3d;

L3DTexture::L3DTexture(
    const TextureType& type,
    const ImageFormat& format,
    unsigned char* data,
    unsigned int width,
    unsigned int height,
    unsigned int depth
) : L3DResource(L3D_TEXTURE)
{
    GLuint id = 0;
    glGenTextures(1, &id);

    GLenum gl_format = GL_RGB;
    switch (format)
    {
    case L3D_RGBA:
        gl_format = GL_RGBA;
        break;
    default:
        break;
    }

    GLenum gl_type = GL_TEXTURE_1D;
    switch (type)
    {
    case L3D_TEXTURE_2D:
        gl_type = GL_TEXTURE_2D;
        break;
    case L3D_TEXTURE_3D:
        gl_type = GL_TEXTURE_3D;
        break;
    default:
        break;
    }

    glActiveTexture(GL_TEXTURE0);

    switch (type)
    {
    case L3D_TEXTURE_1D:
        glBindTexture(gl_type, id);
        glTexImage1D(gl_type, 0, gl_format, width, 0, gl_format, GL_UNSIGNED_BYTE, data);
        break;
    case L3D_TEXTURE_2D:
        glBindTexture(gl_type, id);
        glTexImage2D(gl_type, 0, gl_format, width, height, 0, gl_format, GL_UNSIGNED_BYTE, data);
        break;
    case L3D_TEXTURE_3D:
        glBindTexture(gl_type, id);
        glTexImage3D(gl_type, 0, gl_format, width, height, depth, 0, gl_format, GL_UNSIGNED_BYTE, data);
        break;
    default:
        // Don't store id. Free resource.
        glDeleteTextures(1, &id);
        return;
    };

    this->setId(id);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenerateMipmap(gl_type);
    glTexParameteri(gl_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(gl_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(gl_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(gl_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

L3DTexture::~L3DTexture()
{
    GLuint id = this->id();
    glDeleteTextures(1, &id);
}
