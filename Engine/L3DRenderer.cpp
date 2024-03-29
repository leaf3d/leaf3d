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
#include <sstream>
#include <leaf3d/L3DBuffer.h>
#include <leaf3d/L3DTexture.h>
#include <leaf3d/L3DShader.h>
#include <leaf3d/L3DShaderProgram.h>
#include <leaf3d/L3DFrameBuffer.h>
#include <leaf3d/L3DMaterial.h>
#include <leaf3d/L3DCamera.h>
#include <leaf3d/L3DLight.h>
#include <leaf3d/L3DMesh.h>
#include <leaf3d/L3DRenderQueue.h>
#include <leaf3d/L3DRenderer.h>

using namespace l3d;

struct l3dMeshSortFunctor
{
    bool operator()(L3DMesh *i, L3DMesh *j) { return i->sortKey() < j->sortKey(); }
};

static GLenum toOpenGL(const L3DBufferType &orig)
{
    switch (orig)
    {
    case L3D_BUFFER_VERTEX:
    case L3D_BUFFER_INSTANCE:
        return GL_ARRAY_BUFFER;
    case L3D_BUFFER_INDEX:
        return GL_ELEMENT_ARRAY_BUFFER;
    default:
        break;
    }

    return 0;
}

static GLenum toOpenGL(const L3DDrawType &orig)
{
    switch (orig)
    {
    case L3D_DRAW_STATIC:
        return GL_STATIC_DRAW;
    case L3D_DRAW_DYNAMIC:
        return GL_DYNAMIC_DRAW;
    default:
        break;
    }

    return 0;
}

static GLenum toOpenGL(const L3DTextureType &orig)
{
    switch (orig)
    {
    case L3D_TEXTURE_1D:
        return GL_TEXTURE_1D;
    case L3D_TEXTURE_2D:
        return GL_TEXTURE_2D;
    case L3D_TEXTURE_3D:
        return GL_TEXTURE_3D;
    case L3D_TEXTURE_CUBE_MAP:
        return GL_TEXTURE_CUBE_MAP;
    default:
        break;
    }

    return 0;
}

static GLenum toOpenGL(const L3DImageFormat &orig)
{
    switch (orig)
    {
    case L3D_RGB:
        return GL_RGB;
    case L3D_RGBA:
        return GL_RGBA;
    case L3D_DEPTH24_STENCIL8:
        return GL_DEPTH24_STENCIL8;
    default:
        break;
    }

    return 0;
}

static GLenum toOpenGL(const L3DPixelFormat &orig)
{
    switch (orig)
    {
    case L3D_UNSIGNED_BYTE:
        return GL_UNSIGNED_BYTE;
    case L3D_UNSIGNED_INT_24_8:
        return GL_UNSIGNED_INT_24_8;
    default:
        break;
    }

    return 0;
}

static GLenum toOpenGL(const L3DAttachmentType &orig)
{
    switch (orig)
    {
    case L3D_DEPTH_STENCIL_ATTACHMENT:
        return GL_DEPTH_STENCIL_ATTACHMENT;
    case L3D_COLOR_ATTACHMENT0:
        return GL_COLOR_ATTACHMENT0;
    case L3D_COLOR_ATTACHMENT1:
        return GL_COLOR_ATTACHMENT1;
    case L3D_COLOR_ATTACHMENT2:
        return GL_COLOR_ATTACHMENT2;
    case L3D_COLOR_ATTACHMENT3:
        return GL_COLOR_ATTACHMENT3;
    case L3D_COLOR_ATTACHMENT4:
        return GL_COLOR_ATTACHMENT4;
    case L3D_COLOR_ATTACHMENT5:
        return GL_COLOR_ATTACHMENT5;
    case L3D_COLOR_ATTACHMENT6:
        return GL_COLOR_ATTACHMENT6;
    case L3D_COLOR_ATTACHMENT7:
        return GL_COLOR_ATTACHMENT7;
    case L3D_COLOR_ATTACHMENT8:
        return GL_COLOR_ATTACHMENT8;
    case L3D_COLOR_ATTACHMENT9:
        return GL_COLOR_ATTACHMENT9;
    case L3D_COLOR_ATTACHMENT10:
        return GL_COLOR_ATTACHMENT10;
    case L3D_COLOR_ATTACHMENT11:
        return GL_COLOR_ATTACHMENT11;
    case L3D_COLOR_ATTACHMENT12:
        return GL_COLOR_ATTACHMENT12;
    case L3D_COLOR_ATTACHMENT13:
        return GL_COLOR_ATTACHMENT13;
    case L3D_COLOR_ATTACHMENT14:
        return GL_COLOR_ATTACHMENT14;
    case L3D_COLOR_ATTACHMENT15:
        return GL_COLOR_ATTACHMENT15;
    /*case L3D_COLOR_ATTACHMENT16:
        return GL_COLOR_ATTACHMENT16;
    case L3D_COLOR_ATTACHMENT17:
        return GL_COLOR_ATTACHMENT17;
    case L3D_COLOR_ATTACHMENT18:
        return GL_COLOR_ATTACHMENT18;
    case L3D_COLOR_ATTACHMENT19:
        return GL_COLOR_ATTACHMENT19;
    case L3D_COLOR_ATTACHMENT20:
        return GL_COLOR_ATTACHMENT20;
    case L3D_COLOR_ATTACHMENT21:
        return GL_COLOR_ATTACHMENT21;
    case L3D_COLOR_ATTACHMENT22:
        return GL_COLOR_ATTACHMENT22;
    case L3D_COLOR_ATTACHMENT23:
        return GL_COLOR_ATTACHMENT23;
    case L3D_COLOR_ATTACHMENT24:
        return GL_COLOR_ATTACHMENT24;
    case L3D_COLOR_ATTACHMENT25:
        return GL_COLOR_ATTACHMENT25;
    case L3D_COLOR_ATTACHMENT26:
        return GL_COLOR_ATTACHMENT26;
    case L3D_COLOR_ATTACHMENT27:
        return GL_COLOR_ATTACHMENT27;
    case L3D_COLOR_ATTACHMENT28:
        return GL_COLOR_ATTACHMENT28;
    case L3D_COLOR_ATTACHMENT29:
        return GL_COLOR_ATTACHMENT29;
    case L3D_COLOR_ATTACHMENT30:
        return GL_COLOR_ATTACHMENT30;
    case L3D_COLOR_ATTACHMENT31:
        return GL_COLOR_ATTACHMENT31;*/
    default:
        break;
    }

    return 0;
}

static GLenum toOpenGL(const L3DImageWrapMethod &orig)
{
    switch (orig)
    {
    case L3D_CLAMP_TO_EDGE:
        return GL_CLAMP_TO_EDGE;
    case L3D_CLAMP_TO_BORDER:
        return GL_CLAMP_TO_BORDER;
    case L3D_REPEAT:
        return GL_REPEAT;
    case L3D_MIRRORED_REPEAT:
        return GL_MIRRORED_REPEAT;
    case L3D_MIRROR_CLAMP_TO_EDGE:
        return GL_MIRROR_CLAMP_TO_EDGE;
    default:
        break;
    }

    return 0;
}

static GLenum toOpenGL(const L3DImageMinFilter &orig)
{
    switch (orig)
    {
    case L3D_MIN_NEAREST:
        return GL_NEAREST;
    case L3D_MIN_LINEAR:
        return GL_LINEAR;
    case L3D_MIN_NEAREST_MIPMAP_NEAREST:
        return GL_NEAREST_MIPMAP_NEAREST;
    case L3D_MIN_LINEAR_MIPMAP_NEAREST:
        return GL_LINEAR_MIPMAP_NEAREST;
    case L3D_MIN_NEAREST_MIPMAP_LINEAR:
        return GL_NEAREST_MIPMAP_LINEAR;
    case L3D_MIN_LINEAR_MIPMAP_LINEAR:
        return GL_LINEAR_MIPMAP_LINEAR;
    default:
        break;
    }

    return 0;
}

static GLenum toOpenGL(const L3DImageMagFilter &orig)
{
    switch (orig)
    {
    case L3D_MAG_NEAREST:
        return GL_NEAREST;
    case L3D_MAG_LINEAR:
        return GL_LINEAR;
    default:
        break;
    }

    return 0;
}

static GLenum toOpenGL(const L3DDrawPrimitive &orig)
{
    switch (orig)
    {
    case L3D_DRAW_TRIANGLES:
        return GL_TRIANGLES;
    case L3D_DRAW_POINTS:
        return GL_POINTS;
    case L3D_DRAW_LINES:
        return GL_LINES;
    default:
        break;
    }

    return 0;
}

static GLenum toOpenGL(const L3DDepthFactor &orig)
{
    switch (orig)
    {
    case L3D_LESS:
        return GL_LESS;
    case L3D_EQUAL:
        return GL_EQUAL;
    default:
        break;
    }

    return 0;
}

static GLenum toOpenGL(const L3DBlendFactor &orig)
{
    switch (orig)
    {
    case L3D_ZERO:
        return GL_ZERO;
    case L3D_ONE:
        return GL_ONE;
    case L3D_SRC_COLOR:
        return GL_SRC_COLOR;
    case L3D_ONE_MINUS_SRC_COLOR:
        return GL_ONE_MINUS_SRC_COLOR;
    case L3D_DST_COLOR:
        return GL_DST_COLOR;
    case L3D_ONE_MINUS_DST_COLOR:
        return GL_ONE_MINUS_DST_COLOR;
    case L3D_SRC_ALPHA:
        return GL_SRC_ALPHA;
    case L3D_ONE_MINUS_SRC_ALPHA:
        return GL_ONE_MINUS_SRC_ALPHA;
    case L3D_DST_ALPHA:
        return GL_DST_ALPHA;
    case L3D_ONE_MINUS_DST_ALPHA:
        return GL_ONE_MINUS_DST_ALPHA;
    case L3D_CONSTANT_COLOR:
        return GL_CONSTANT_COLOR;
    case L3D_ONE_MINUS_CONSTANT_COLOR:
        return GL_ONE_MINUS_CONSTANT_COLOR;
    case L3D_CONSTANT_ALPHA:
        return GL_CONSTANT_ALPHA;
    case L3D_ONE_MINUS_CONSTANT_ALPHA:
        return GL_ONE_MINUS_CONSTANT_ALPHA;
    default:
        break;
    }

    return 0;
}

static GLenum toOpenGL(const L3DCullFace &orig)
{
    switch (orig)
    {
    case L3D_FRONT_FACE:
        return GL_FRONT;
    case L3D_BACK_FACE:
        return GL_BACK;
    case L3D_BOTH_FACES:
        return GL_FRONT_AND_BACK;
    default:
        break;
    }

    return 0;
}

static GLenum toOpenGL(const L3DShaderType &orig)
{
    switch (orig)
    {
    case L3D_SHADER_VERTEX:
        return GL_VERTEX_SHADER;
    case L3D_SHADER_FRAGMENT:
        return GL_FRAGMENT_SHADER;
    case L3D_SHADER_GEOMETRY:
        return GL_GEOMETRY_SHADER;
    default:
        break;
    }

    return 0;
}

static void enableVertexAttribute(
    GLint attrib,
    GLint size,
    GLenum type,
    GLsizei stride,
    void *startPtr = 0,
    GLboolean normalized = GL_FALSE,
    unsigned int divisor = 0)
{
    if (attrib > -1)
    {
        glEnableVertexAttribArray(attrib);
        glVertexAttribPointer(attrib, size, type, normalized, stride, startPtr);
        if (divisor > 0)
            glVertexAttribDivisor(attrib, divisor);
    }
}

static void setUniform(
    GLuint shaderProgram,
    const char *name,
    const L3DUniform &uniform)
{
    GLint gl_location = glGetUniformLocation(shaderProgram, name);

    switch (uniform.type)
    {
    case L3D_UNIFORM_FLOAT:
        glUniform1f(gl_location, uniform.value.valueF);
        break;
    case L3D_UNIFORM_INT:
        glUniform1i(gl_location, uniform.value.valueI);
        break;
    case L3D_UNIFORM_UINT:
        glUniform1ui(gl_location, uniform.value.valueUI);
        break;
    case L3D_UNIFORM_BOOL:
        glUniform1ui(gl_location, uniform.value.valueB);
        break;
    case L3D_UNIFORM_VEC2:
        glUniform2fv(gl_location, 1, uniform.value.valueVec2);
        break;
    case L3D_UNIFORM_VEC3:
        glUniform3fv(gl_location, 1, uniform.value.valueVec3);
        break;
    case L3D_UNIFORM_VEC4:
        glUniform4fv(gl_location, 1, uniform.value.valueVec4);
        break;
    case L3D_UNIFORM_MAT3:
        glUniformMatrix3fv(gl_location, 1, GL_FALSE, uniform.value.valueMat3);
        break;
    case L3D_UNIFORM_MAT4:
        glUniformMatrix4fv(gl_location, 1, GL_FALSE, uniform.value.valueMat4);
        break;
    default:
        break;
    }
}

L3DRenderer::L3DRenderer()
{
}

L3DRenderer::~L3DRenderer()
{
    this->terminate();
}

int L3DRenderer::init()
{
    // Load OpenGL extensions.
    if (!gladLoadGL())
    {
        fprintf(stderr, "Failed to initialize OpenGL\n");
        return -1;
    }

    return L3D_TRUE;
}

int L3DRenderer::terminate()
{
    for (L3DCameraPool::reverse_iterator it = m_cameras.rbegin(); it != m_cameras.rend(); ++it)
        delete it->second;
    m_cameras.clear();

    for (L3DRenderQueuePool::reverse_iterator it = m_renderQueues.rbegin(); it != m_renderQueues.rend(); ++it)
        delete it->second;
    m_renderQueues.clear();

    for (L3DFrameBufferPool::reverse_iterator it = m_frameBuffers.rbegin(); it != m_frameBuffers.rend(); ++it)
        delete it->second;
    m_frameBuffers.clear();

    for (L3DMeshPool::reverse_iterator it = m_meshes.rbegin(); it != m_meshes.rend(); ++it)
        delete it->second;
    m_meshes.clear();

    for (L3DMaterialPool::reverse_iterator it = m_materials.rbegin(); it != m_materials.rend(); ++it)
        delete it->second;
    m_materials.clear();

    for (L3DShaderProgramPool::reverse_iterator it = m_shaderPrograms.rbegin(); it != m_shaderPrograms.rend(); ++it)
        delete it->second;
    m_shaderPrograms.clear();

    for (L3DShaderPool::reverse_iterator it = m_shaders.rbegin(); it != m_shaders.rend(); ++it)
        delete it->second;
    m_shaders.clear();

    for (L3DTexturePool::reverse_iterator it = m_textures.rbegin(); it != m_textures.rend(); ++it)
        delete it->second;
    m_textures.clear();

    for (L3DLightPool::reverse_iterator it = m_lights.rbegin(); it != m_lights.rend(); ++it)
        delete it->second;
    m_lights.clear();

    for (L3DBufferPool::reverse_iterator it = m_buffers.rbegin(); it != m_buffers.rend(); ++it)
        delete it->second;
    m_buffers.clear();

    return L3D_TRUE;
}

void L3DRenderer::renderFrame(L3DCamera *camera, L3DRenderQueue *renderQueue)
{
    if (renderQueue)
        renderQueue->execute(this, camera);
}

void L3DRenderer::addResource(L3DResource *resource)
{
    if (resource)
    {
        switch (resource->resourceType())
        {
        case L3D_BUFFER:
            this->addBuffer(static_cast<L3DBuffer *>(resource));
            break;
        case L3D_TEXTURE:
            this->addTexture(static_cast<L3DTexture *>(resource));
            break;
        case L3D_SHADER:
            this->addShader(static_cast<L3DShader *>(resource));
            break;
        case L3D_SHADER_PROGRAM:
            this->addShaderProgram(static_cast<L3DShaderProgram *>(resource));
            break;
        case L3D_FRAME_BUFFER:
            this->addFrameBuffer(static_cast<L3DFrameBuffer *>(resource));
            break;
        case L3D_MATERIAL:
            this->addMaterial(static_cast<L3DMaterial *>(resource));
            break;
        case L3D_CAMERA:
            this->addCamera(static_cast<L3DCamera *>(resource));
            break;
        case L3D_LIGHT:
            this->addLight(static_cast<L3DLight *>(resource));
            break;
        case L3D_MESH:
            this->addMesh(static_cast<L3DMesh *>(resource));
            break;
        case L3D_RENDER_QUEUE:
            this->addRenderQueue(static_cast<L3DRenderQueue *>(resource));
            break;
        default:
            break;
        }
    }
}

void L3DRenderer::addBuffer(L3DBuffer *buffer)
{
    if (buffer && m_buffers.find(buffer->id()) == m_buffers.end())
    {
        GLuint id = 0;
        glGenBuffers(1, &id);

        if (buffer->count())
        {
            GLenum gl_type = toOpenGL(buffer->type());
            GLenum gl_draw_type = toOpenGL(buffer->drawType());

            glBindBuffer(gl_type, id);
            glBufferData(gl_type, buffer->size(), buffer->data(), gl_draw_type);
            glBindBuffer(gl_type, 0);
        }

        buffer->setId((unsigned short int)id);

        m_buffers[id] = buffer;

        printf("Add buffer: %d\n", id);
    }
}

void L3DRenderer::addTexture(L3DTexture *texture)
{
    if (texture && m_textures.find(texture->id()) == m_textures.end())
    {
        GLuint id = 0;
        glGenTextures(1, &id);

        GLenum gl_format = toOpenGL(texture->format());
        GLenum gl_internal_format = gl_format;
        GLenum gl_type = toOpenGL(texture->type());
        GLenum gl_pixel_format = toOpenGL(texture->pixelFormat());
        GLenum gl_wrap_s = toOpenGL(texture->wrapS());
        GLenum gl_wrap_t = toOpenGL(texture->wrapT());
        GLenum gl_wrap_r = toOpenGL(texture->wrapR());
        GLenum gl_min_filter = toOpenGL(texture->minFilter());
        GLenum gl_mag_filter = toOpenGL(texture->magFilter());
        bool use_mipmaps = texture->useMipmap();

        if (gl_format == GL_DEPTH24_STENCIL8)
            gl_internal_format = GL_DEPTH_STENCIL;

        glBindTexture(gl_type, id);

        switch (texture->type())
        {
        case L3D_TEXTURE_1D:
            glTexImage1D(gl_type, 0, gl_format, texture->width(), 0, gl_internal_format, gl_pixel_format, texture->data());
            break;
        case L3D_TEXTURE_2D:
            glTexImage2D(gl_type, 0, gl_format, texture->width(), texture->height(), 0, gl_internal_format, gl_pixel_format, texture->data());
            break;
        case L3D_TEXTURE_3D:
            glTexImage3D(gl_type, 0, gl_format, texture->width(), texture->height(), texture->depth(), 0, gl_internal_format, gl_pixel_format, texture->data());
            break;
        case L3D_TEXTURE_CUBE_MAP:
        {
            unsigned int faceSize = texture->size() / 6;
            unsigned int faceWidth = texture->width();
            unsigned int faceHeight = texture->height();
            unsigned char *data = texture->data();
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, gl_format, faceWidth, faceHeight, 0, gl_internal_format, gl_pixel_format, data);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, gl_format, faceWidth, faceHeight, 0, gl_internal_format, gl_pixel_format, data + faceSize);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, gl_format, faceWidth, faceHeight, 0, gl_internal_format, gl_pixel_format, data + faceSize * 2);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, gl_format, faceWidth, faceHeight, 0, gl_internal_format, gl_pixel_format, data + faceSize * 3);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, gl_format, faceWidth, faceHeight, 0, gl_internal_format, gl_pixel_format, data + faceSize * 4);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, gl_format, faceWidth, faceHeight, 0, gl_internal_format, gl_pixel_format, data + faceSize * 5);
        }
        break;
        default:
            // Don't store id. Free resource.
            glBindTexture(gl_type, 0);
            glDeleteTextures(1, &id);
            return;
        }

        // Wrap mode for S, T, R coordinates.
        glTexParameteri(gl_type, GL_TEXTURE_WRAP_S, gl_wrap_s);

        if (gl_type > GL_TEXTURE_1D)
            glTexParameteri(gl_type, GL_TEXTURE_WRAP_T, gl_wrap_t);

        if (gl_type > GL_TEXTURE_2D)
            glTexParameteri(gl_type, GL_TEXTURE_WRAP_R, gl_wrap_r);

        // Min and mag filters.
        if (!use_mipmaps)
        {
            if (gl_min_filter == GL_NEAREST_MIPMAP_LINEAR)
                gl_min_filter = GL_LINEAR;
            else if (gl_min_filter == GL_NEAREST_MIPMAP_NEAREST)
                gl_min_filter = GL_NEAREST;
        }

        glTexParameteri(gl_type, GL_TEXTURE_MIN_FILTER, gl_min_filter);
        glTexParameteri(gl_type, GL_TEXTURE_MAG_FILTER, gl_mag_filter);

        // Generate mipmaps.
        if (use_mipmaps)
            glGenerateMipmap(gl_type);

        glBindTexture(gl_type, 0);

        texture->setId((unsigned short int)id);

        m_textures[id] = texture;

        printf("Add texture: %d\n", id);
    }
}

void L3DRenderer::addShader(L3DShader *shader)
{
    if (shader && m_shaders.find(shader->id()) == m_shaders.end())
    {
        GLuint gl_type = toOpenGL(shader->type());

        const char *code = shader->code();

        GLuint id = glCreateShader(gl_type);
        glShaderSource(id, 1, &code, L3D_NULLPTR);
        glCompileShader(id);

        GLint status;
        glGetShaderiv(id, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLchar infoLog[512];
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            fprintf(stderr, "%s", infoLog);
        }

        shader->setId((unsigned short int)id);

        m_shaders[id] = shader;

        printf("Add shader: %d\n", id);
    }
}

void L3DRenderer::addShaderProgram(L3DShaderProgram *shaderProgram)
{
    if (shaderProgram && m_shaderPrograms.find(shaderProgram->id()) == m_shaderPrograms.end())
    {
        GLuint id = glCreateProgram();

        if (shaderProgram->vertexShader())
            glAttachShader(id, shaderProgram->vertexShader()->id());

        if (shaderProgram->fragmentShader())
            glAttachShader(id, shaderProgram->fragmentShader()->id());

        if (shaderProgram->geometryShader())
            glAttachShader(id, shaderProgram->geometryShader()->id());

        glLinkProgram(id);

        GLint status;
        glGetProgramiv(id, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLchar infoLog[512];
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            fprintf(stderr, "%s", infoLog);
        }

        shaderProgram->setId((unsigned short int)id);

        m_shaderPrograms[id] = shaderProgram;

        printf("Add shader program: %d\n", id);
    }
}

void L3DRenderer::addFrameBuffer(L3DFrameBuffer *frameBuffer)
{
    if (frameBuffer && m_frameBuffers.find(frameBuffer->id()) == m_frameBuffers.end())
    {
        GLuint id = 0;
        glGenFramebuffers(1, &id);
        glBindFramebuffer(GL_FRAMEBUFFER, id);

        // Register texture attachments.
        L3DTextureAttachments textures = frameBuffer->textureAttachments();
        for (L3DTextureAttachments::iterator tex_it = textures.begin(); tex_it != textures.end(); ++tex_it)
        {
            GLenum gl_attachment_type = toOpenGL(tex_it->first);
            L3DTexture *texture = tex_it->second;

            if (texture)
            {
                this->addTexture(texture);

                GLuint id = texture->id();
                GLenum gl_type = toOpenGL(texture->type());

                switch (texture->type())
                {
                case L3D_TEXTURE_1D:
                    glFramebufferTexture1D(GL_FRAMEBUFFER, gl_attachment_type, gl_type, id, 0);
                    break;
                case L3D_TEXTURE_2D:
                    glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment_type, gl_type, id, 0);
                    break;
                case L3D_TEXTURE_3D:
                    glFramebufferTexture3D(GL_FRAMEBUFFER, gl_attachment_type, gl_type, id, 0, 0);
                    break;
                case L3D_TEXTURE_CUBE_MAP:
                {
                    glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment_type, GL_TEXTURE_CUBE_MAP_POSITIVE_X, id, 0);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment_type + 1, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, id, 0);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment_type + 2, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, id, 0);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment_type + 3, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, id, 0);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment_type + 4, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, id, 0);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment_type + 5, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, id, 0);
                }
                break;
                default:
                    break;
                }
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        frameBuffer->setId((unsigned short int)id);

        m_frameBuffers[id] = frameBuffer;

        printf("Add frame buffer: %d\n", id);
    }
}

void L3DRenderer::addMaterial(L3DMaterial *material)
{
    if (material && m_materials.find(material->id()) == m_materials.end())
    {
        GLuint id = 1;
        if (m_materials.size() > 0)
            id = m_materials.rbegin()->second->id() + 1;

        material->setId((unsigned short int)id);

        m_materials[id] = material;

        printf("Add material: %d\n", id);
    }
}

void L3DRenderer::addCamera(L3DCamera *camera)
{
    if (camera && m_cameras.find(camera->id()) == m_cameras.end())
    {
        GLuint id = 1;
        if (m_cameras.size() > 0)
            id = m_cameras.rbegin()->second->id() + 1;

        camera->setId((unsigned short int)id);

        m_cameras[id] = camera;

        printf("Add camera: %d\n", id);
    }
}

void L3DRenderer::addLight(L3DLight *light)
{
    if (light && m_lights.find(light->id()) == m_lights.end())
    {
        GLuint id = 1;
        if (m_lights.size() > 0)
            id = m_lights.rbegin()->second->id() + 1;

        light->setId((unsigned short int)id);

        m_lights[id] = light;

        printf("Add light: %d\n", id);
    }
}

void L3DRenderer::addMesh(L3DMesh *mesh)
{
    if (mesh && m_meshes.find(mesh->id()) == m_meshes.end())
    {
        GLuint id;
        glGenVertexArrays(1, &id);
        glBindVertexArray(id);

        if (mesh->vertexBuffer() && mesh->vertexCount())
        {
            this->addBuffer(mesh->vertexBuffer());

            // Binds vertex buffer.
            glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer()->id());

            if (mesh->material() && mesh->material()->shaderProgram())
            {
                L3DMaterial *material = mesh->material();
                L3DShaderProgram *shaderProgram = material->shaderProgram();
                L3DAttributeMap shaderAttributes = shaderProgram->attributes();

                // Enables vertex attributes.
                GLint posAttrib = glGetAttribLocation(shaderProgram->id(), shaderAttributes[L3D_VERTEX_POSITION].c_str());
                GLint norAttrib = glGetAttribLocation(shaderProgram->id(), shaderAttributes[L3D_VERTEX_NORMAL].c_str());
                GLint tanAttrib = glGetAttribLocation(shaderProgram->id(), shaderAttributes[L3D_VERTEX_TANGENT].c_str());
                GLint tex0Attrib = glGetAttribLocation(shaderProgram->id(), shaderAttributes[L3D_VERTEX_UV0].c_str());
                GLint tex1Attrib = glGetAttribLocation(shaderProgram->id(), shaderAttributes[L3D_VERTEX_UV1].c_str());
                GLint tex2Attrib = glGetAttribLocation(shaderProgram->id(), shaderAttributes[L3D_VERTEX_UV2].c_str());
                GLint tex3Attrib = glGetAttribLocation(shaderProgram->id(), shaderAttributes[L3D_VERTEX_UV3].c_str());

                switch (mesh->vertexFormat())
                {
                case L3D_VERTEX_POS2:
                    enableVertexAttribute(posAttrib, 2, GL_FLOAT, 2 * sizeof(GLfloat), 0);
                    break;
                case L3D_VERTEX_POS3:
                    enableVertexAttribute(posAttrib, 3, GL_FLOAT, 3 * sizeof(GLfloat), 0);
                    break;
                case L3D_VERTEX_POS2_UV2:
                    enableVertexAttribute(posAttrib, 2, GL_FLOAT, 4 * sizeof(GLfloat), 0);
                    enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
                    break;
                case L3D_VERTEX_POS3_UV2:
                    enableVertexAttribute(posAttrib, 3, GL_FLOAT, 5 * sizeof(GLfloat), 0);
                    enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
                    break;
                case L3D_VERTEX_POS3_UV3:
                    enableVertexAttribute(posAttrib, 3, GL_FLOAT, 6 * sizeof(GLfloat), 0);
                    enableVertexAttribute(tex0Attrib, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
                    break;
                case L3D_VERTEX_POS3_NOR3_UV2:
                    enableVertexAttribute(posAttrib, 3, GL_FLOAT, 8 * sizeof(GLfloat), 0);
                    enableVertexAttribute(norAttrib, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
                    enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 8 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
                    break;
                case L3D_VERTEX_POS3_NOR3_UV3:
                    enableVertexAttribute(posAttrib, 3, GL_FLOAT, 9 * sizeof(GLfloat), 0);
                    enableVertexAttribute(norAttrib, 3, GL_FLOAT, 9 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
                    enableVertexAttribute(tex0Attrib, 3, GL_FLOAT, 9 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
                    break;
                case L3D_VERTEX_POS3_NOR3_UV2_UV2:
                    enableVertexAttribute(posAttrib, 3, GL_FLOAT, 10 * sizeof(GLfloat), 0);
                    enableVertexAttribute(norAttrib, 3, GL_FLOAT, 10 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
                    enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 10 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
                    enableVertexAttribute(tex1Attrib, 2, GL_FLOAT, 10 * sizeof(GLfloat), (void *)(8 * sizeof(GLfloat)));
                    break;
                case L3D_VERTEX_POS3_NOR3_TAN3_UV2:
                    enableVertexAttribute(posAttrib, 3, GL_FLOAT, 11 * sizeof(GLfloat), 0);
                    enableVertexAttribute(norAttrib, 3, GL_FLOAT, 11 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
                    enableVertexAttribute(tanAttrib, 3, GL_FLOAT, 11 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
                    enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 11 * sizeof(GLfloat), (void *)(9 * sizeof(GLfloat)));
                    break;
                case L3D_VERTEX_POS3_NOR3_TAN3_UV3:
                    enableVertexAttribute(posAttrib, 3, GL_FLOAT, 12 * sizeof(GLfloat), 0);
                    enableVertexAttribute(norAttrib, 3, GL_FLOAT, 12 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
                    enableVertexAttribute(tanAttrib, 3, GL_FLOAT, 12 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
                    enableVertexAttribute(tex0Attrib, 3, GL_FLOAT, 12 * sizeof(GLfloat), (void *)(9 * sizeof(GLfloat)));
                    break;
                case L3D_VERTEX_POS3_NOR3_TAN3_UV2_UV2:
                    enableVertexAttribute(posAttrib, 3, GL_FLOAT, 13 * sizeof(GLfloat), 0);
                    enableVertexAttribute(norAttrib, 3, GL_FLOAT, 13 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
                    enableVertexAttribute(tanAttrib, 3, GL_FLOAT, 13 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
                    enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 13 * sizeof(GLfloat), (void *)(9 * sizeof(GLfloat)));
                    enableVertexAttribute(tex1Attrib, 2, GL_FLOAT, 13 * sizeof(GLfloat), (void *)(11 * sizeof(GLfloat)));
                    break;
                case L3D_VERTEX_POS3_NOR3_TAN3_UV2_UV2_UV2:
                    enableVertexAttribute(posAttrib, 3, GL_FLOAT, 15 * sizeof(GLfloat), 0);
                    enableVertexAttribute(norAttrib, 3, GL_FLOAT, 15 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
                    enableVertexAttribute(tanAttrib, 3, GL_FLOAT, 15 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
                    enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 15 * sizeof(GLfloat), (void *)(9 * sizeof(GLfloat)));
                    enableVertexAttribute(tex1Attrib, 2, GL_FLOAT, 15 * sizeof(GLfloat), (void *)(11 * sizeof(GLfloat)));
                    enableVertexAttribute(tex2Attrib, 2, GL_FLOAT, 15 * sizeof(GLfloat), (void *)(13 * sizeof(GLfloat)));
                    break;
                case L3D_VERTEX_POS3_NOR3_TAN3_UV2_UV2_UV2_UV2:
                    enableVertexAttribute(posAttrib, 3, GL_FLOAT, 17 * sizeof(GLfloat), 0);
                    enableVertexAttribute(norAttrib, 3, GL_FLOAT, 17 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
                    enableVertexAttribute(tanAttrib, 3, GL_FLOAT, 17 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
                    enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 17 * sizeof(GLfloat), (void *)(9 * sizeof(GLfloat)));
                    enableVertexAttribute(tex1Attrib, 2, GL_FLOAT, 17 * sizeof(GLfloat), (void *)(11 * sizeof(GLfloat)));
                    enableVertexAttribute(tex2Attrib, 2, GL_FLOAT, 17 * sizeof(GLfloat), (void *)(13 * sizeof(GLfloat)));
                    enableVertexAttribute(tex3Attrib, 2, GL_FLOAT, 17 * sizeof(GLfloat), (void *)(15 * sizeof(GLfloat)));
                    break;
                default:
                    glDeleteVertexArrays(1, &id);
                    glBindVertexArray(0);
                    return;
                }
            }
        }

        if (mesh->indexBuffer() && mesh->indexCount())
        {
            this->addBuffer(mesh->indexBuffer());

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer()->id());
        }

        if (mesh->instanceBuffer() && mesh->instanceFormat())
        {
            this->addBuffer(mesh->instanceBuffer());

            // Binds instance buffer.
            glBindBuffer(GL_ARRAY_BUFFER, mesh->instanceBuffer()->id());

            if (mesh->material() && mesh->material()->shaderProgram())
            {
                L3DMaterial *material = mesh->material();
                L3DShaderProgram *shaderProgram = material->shaderProgram();
                L3DAttributeMap shaderAttributes = shaderProgram->attributes();

                // Enables instanced attributes.
                GLint iposAttrib = glGetAttribLocation(shaderProgram->id(), shaderAttributes[L3D_INSTANCE_POSITION].c_str());
                GLint itexAttrib = glGetAttribLocation(shaderProgram->id(), shaderAttributes[L3D_INSTANCE_UV].c_str());
                GLint itransAttrib = glGetAttribLocation(shaderProgram->id(), shaderAttributes[L3D_INSTANCE_MATRIX].c_str());

                switch (mesh->instanceFormat())
                {
                case L3D_INSTANCE_POS2:
                    enableVertexAttribute(iposAttrib, 2, GL_FLOAT, 2 * sizeof(GLfloat), (void *)0, GL_FALSE, 1);
                    break;
                case L3D_INSTANCE_POS3:
                    enableVertexAttribute(iposAttrib, 3, GL_FLOAT, 3 * sizeof(GLfloat), (void *)0, GL_FALSE, 1);
                    break;
                case L3D_INSTANCE_POS2_UV2:
                    enableVertexAttribute(iposAttrib, 2, GL_FLOAT, 4 * sizeof(GLfloat), (void *)0, GL_FALSE, 1);
                    enableVertexAttribute(itexAttrib, 2, GL_FLOAT, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)), GL_FALSE, 1);
                    break;
                case L3D_INSTANCE_POS3_UV2:
                    enableVertexAttribute(iposAttrib, 3, GL_FLOAT, 5 * sizeof(GLfloat), (void *)0, GL_FALSE, 1);
                    enableVertexAttribute(itexAttrib, 2, GL_FLOAT, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)), GL_FALSE, 1);
                    break;
                case L3D_INSTANCE_TRANS4_TRANS4_TRANS4_TRANS4:
                    enableVertexAttribute(itransAttrib + 0, 4, GL_FLOAT, 16 * sizeof(GLfloat), (void *)0, GL_FALSE, 1);
                    enableVertexAttribute(itransAttrib + 1, 4, GL_FLOAT, 16 * sizeof(GLfloat), (void *)(4 * sizeof(GLfloat)), GL_FALSE, 1);
                    enableVertexAttribute(itransAttrib + 2, 4, GL_FLOAT, 16 * sizeof(GLfloat), (void *)(8 * sizeof(GLfloat)), GL_FALSE, 1);
                    enableVertexAttribute(itransAttrib + 3, 4, GL_FLOAT, 16 * sizeof(GLfloat), (void *)(12 * sizeof(GLfloat)), GL_FALSE, 1);
                    break;
                case L3D_INSTANCE_TRANS4_TRANS4_TRANS4_TRANS4_UV2:
                    enableVertexAttribute(itransAttrib + 0, 4, GL_FLOAT, 18 * sizeof(GLfloat), (void *)0, GL_FALSE, 1);
                    enableVertexAttribute(itransAttrib + 1, 4, GL_FLOAT, 18 * sizeof(GLfloat), (void *)(4 * sizeof(GLfloat)), GL_FALSE, 1);
                    enableVertexAttribute(itransAttrib + 2, 4, GL_FLOAT, 18 * sizeof(GLfloat), (void *)(8 * sizeof(GLfloat)), GL_FALSE, 1);
                    enableVertexAttribute(itransAttrib + 3, 4, GL_FLOAT, 18 * sizeof(GLfloat), (void *)(12 * sizeof(GLfloat)), GL_FALSE, 1);
                    enableVertexAttribute(itexAttrib, 2, GL_FLOAT, 18 * sizeof(GLfloat), (void *)(16 * sizeof(GLfloat)), GL_FALSE, 1);
                    break;
                default:
                    break;
                }
            }
        }

        glBindVertexArray(0);

        mesh->setId((unsigned short int)id);

        m_meshes[id] = mesh;

        this->recomputeRenderBucket();

        printf("Add mesh: %d\n", id);
    }
}

void L3DRenderer::addRenderQueue(L3DRenderQueue *renderQueue)
{
    if (renderQueue && m_renderQueues.find(renderQueue->id()) == m_renderQueues.end())
    {
        GLuint id = 1;
        if (m_renderQueues.size() > 0)
            id = m_renderQueues.rbegin()->second->id() + 1;

        renderQueue->setId((unsigned short int)id);

        m_renderQueues[id] = renderQueue;

        printf("Add render queue: %d\n", id);
    }
}

void L3DRenderer::removeResource(L3DResource *resource)
{
    if (resource)
    {
        switch (resource->resourceType())
        {
        case L3D_BUFFER:
            this->removeBuffer(static_cast<L3DBuffer *>(resource));
            break;
        case L3D_TEXTURE:
            this->removeTexture(static_cast<L3DTexture *>(resource));
            break;
        case L3D_SHADER:
            this->removeShader(static_cast<L3DShader *>(resource));
            break;
        case L3D_SHADER_PROGRAM:
            this->removeShaderProgram(static_cast<L3DShaderProgram *>(resource));
            break;
        case L3D_FRAME_BUFFER:
            this->removeFrameBuffer(static_cast<L3DFrameBuffer *>(resource));
            break;
        case L3D_MATERIAL:
            this->removeMaterial(static_cast<L3DMaterial *>(resource));
            break;
        case L3D_CAMERA:
            this->removeCamera(static_cast<L3DCamera *>(resource));
            break;
        case L3D_LIGHT:
            this->removeLight(static_cast<L3DLight *>(resource));
            break;
        case L3D_MESH:
            this->removeMesh(static_cast<L3DMesh *>(resource));
            break;
        case L3D_RENDER_QUEUE:
            this->removeRenderQueue(static_cast<L3DRenderQueue *>(resource));
            break;
        default:
            break;
        }
    }
}

void L3DRenderer::removeBuffer(L3DBuffer *buffer)
{
    if (buffer)
    {
        GLuint id = buffer->id();
        m_buffers[id] = L3D_NULLPTR;
        glDeleteBuffers(1, &id);
        buffer->setId(0);

        printf("Remove buffer: %d\n", id);
    }
}

void L3DRenderer::removeTexture(L3DTexture *texture)
{
    if (texture)
    {
        GLuint id = texture->id();
        m_textures[id] = L3D_NULLPTR;
        glDeleteTextures(1, &id);
        texture->setId(0);

        printf("Remove texture: %d\n", id);
    }
}

void L3DRenderer::removeShader(L3DShader *shader)
{
    if (shader)
    {
        GLuint id = shader->id();
        m_shaders[id] = L3D_NULLPTR;
        glDeleteShader(id);
        shader->setId(0);

        printf("Remove shader: %d\n", id);
    }
}

void L3DRenderer::removeShaderProgram(L3DShaderProgram *shaderProgram)
{
    if (shaderProgram)
    {
        GLuint id = shaderProgram->id();
        m_shaderPrograms[id] = L3D_NULLPTR;
        glDeleteProgram(id);
        shaderProgram->setId(0);

        printf("Remove shader program: %d\n", id);
    }
}

void L3DRenderer::removeFrameBuffer(L3DFrameBuffer *frameBuffer)
{
    if (frameBuffer)
    {
        GLuint id = frameBuffer->id();
        m_frameBuffers[id] = L3D_NULLPTR;
        glDeleteFramebuffers(1, &id);
        // TODO: clean frame buffer attachments.
        frameBuffer->setId(0);

        printf("Remove frame buffer: %d\n", id);
    }
}

void L3DRenderer::removeMaterial(L3DMaterial *material)
{
    if (material)
    {
        GLuint id = material->id();
        m_materials[id] = L3D_NULLPTR;
        // TODO: clean material resources.
        material->setId(0);

        printf("Remove material: %d\n", id);
    }
}

void L3DRenderer::removeCamera(L3DCamera *camera)
{
    if (camera)
    {
        GLuint id = camera->id();
        m_cameras[id] = L3D_NULLPTR;
        // TODO: clean camera resources.
        camera->setId(0);

        printf("Remove camera: %d\n", id);
    }
}

void L3DRenderer::removeLight(L3DLight *light)
{
    if (light)
    {
        GLuint id = light->id();
        m_lights[id] = L3D_NULLPTR;
        // TODO: clean light resources.
        light->setId(0);

        printf("Remove light: %d\n", id);
    }
}

void L3DRenderer::removeMesh(L3DMesh *mesh)
{
    if (mesh)
    {
        GLuint id = mesh->id();
        m_meshes[id] = L3D_NULLPTR;
        glDeleteVertexArrays(1, &id);
        mesh->setId(0);

        this->recomputeRenderBucket();

        printf("Remove mesh: %d\n", id);
    }
}

void L3DRenderer::removeRenderQueue(L3DRenderQueue *renderQueue)
{
    if (renderQueue)
    {
        GLuint id = renderQueue->id();
        m_renderQueues[id] = L3D_NULLPTR;
        // TODO: clean render queue resources.
        renderQueue->setId(0);

        printf("Remove render queue: %d\n", id);
    }
}

L3DResource *L3DRenderer::getResource(const L3DHandle &handle) const
{
    switch (handle.data.type)
    {
    case L3D_BUFFER:
        return m_buffers.find(handle.data.id)->second;
    case L3D_TEXTURE:
        return m_textures.find(handle.data.id)->second;
    case L3D_SHADER:
        return m_shaders.find(handle.data.id)->second;
    case L3D_SHADER_PROGRAM:
        return m_shaderPrograms.find(handle.data.id)->second;
    case L3D_FRAME_BUFFER:
        return m_frameBuffers.find(handle.data.id)->second;
    case L3D_MATERIAL:
        return m_materials.find(handle.data.id)->second;
    case L3D_CAMERA:
        return m_cameras.find(handle.data.id)->second;
    case L3D_LIGHT:
        return m_lights.find(handle.data.id)->second;
    case L3D_MESH:
        return m_meshes.find(handle.data.id)->second;
    case L3D_RENDER_QUEUE:
        return m_renderQueues.find(handle.data.id)->second;
    default:
        return L3D_NULLPTR;
    }
}

L3DBuffer *L3DRenderer::getBuffer(const L3DHandle &handle) const
{
    if (handle.data.type == L3D_BUFFER)
        return m_buffers.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DTexture *L3DRenderer::getTexture(const L3DHandle &handle) const
{
    if (handle.data.type == L3D_TEXTURE)
        return m_textures.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DShader *L3DRenderer::getShader(const L3DHandle &handle) const
{
    if (handle.data.type == L3D_SHADER)
        return m_shaders.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DShaderProgram *L3DRenderer::getShaderProgram(const L3DHandle &handle) const
{
    if (handle.data.type == L3D_SHADER_PROGRAM)
        return m_shaderPrograms.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DFrameBuffer *L3DRenderer::getFrameBuffer(const L3DHandle &handle) const
{
    if (handle.data.type == L3D_FRAME_BUFFER)
        return m_frameBuffers.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DMaterial *L3DRenderer::getMaterial(const L3DHandle &handle) const
{
    if (handle.data.type == L3D_MATERIAL)
        return m_materials.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DCamera *L3DRenderer::getCamera(const L3DHandle &handle) const
{
    if (handle.data.type == L3D_CAMERA)
        return m_cameras.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DLight *L3DRenderer::getLight(const L3DHandle &handle) const
{
    if (handle.data.type == L3D_LIGHT)
        return m_lights.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DMesh *L3DRenderer::getMesh(const L3DHandle &handle) const
{
    if (handle.data.type == L3D_MESH)
        return m_meshes.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

L3DRenderQueue *L3DRenderer::getRenderQueue(const L3DHandle &handle) const
{
    if (handle.data.type == L3D_RENDER_QUEUE)
        return m_renderQueues.find(handle.data.id)->second;

    return L3D_NULLPTR;
}

void L3DRenderer::switchFrameBuffer(L3DFrameBuffer *frameBuffer)
{
    // At every framebuffer switch, update mipmaps of attached textures.
    for (L3DFrameBufferPool::iterator fb_it = m_frameBuffers.begin(); fb_it != m_frameBuffers.end(); ++fb_it)
    {
        L3DFrameBuffer *fb = fb_it->second;
        L3DTextureAttachments fb_textures = fb->textureAttachments();
        for (L3DTextureAttachments::iterator tex_it = fb_textures.begin(); tex_it != fb_textures.end(); ++tex_it)
        {
            L3DTexture *texture = tex_it->second;
            if (texture && texture->useMipmap())
            {
                GLenum gl_type = toOpenGL(texture->type());
                glBindTexture(gl_type, texture->id());
                glGenerateMipmap(gl_type);
                glBindTexture(gl_type, 0);
            }
        }
    }

    // Switch to new framebuffer.
    GLuint frameBufferId = frameBuffer ? frameBuffer->id() : 0;

    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "Framebuffer %d is not completed!\n", frameBufferId);
    }
}

void L3DRenderer::clearBuffers(
    bool colorBuffer,
    bool depthBuffer,
    bool stencilBuffer,
    const L3DVec4 &clearColor)
{
    unsigned int clearMask = 0;
    if (colorBuffer)
        clearMask |= GL_COLOR_BUFFER_BIT;
    if (depthBuffer)
        clearMask |= GL_DEPTH_BUFFER_BIT;
    if (stencilBuffer)
        clearMask |= GL_STENCIL_BUFFER_BIT;

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(clearMask);
}

void L3DRenderer::setDepthTest(
    bool enable,
    const L3DDepthFactor &factor)
{
    enable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    if (enable)
        glDepthFunc(toOpenGL(factor));
}

void L3DRenderer::setDepthMask(bool enable)
{
    glDepthMask(enable ? GL_TRUE : GL_FALSE);
}

void L3DRenderer::setStencilTest(bool enable)
{
    enable ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
}

void L3DRenderer::setBlend(
    bool enable,
    const L3DBlendFactor &srcFactor,
    const L3DBlendFactor &dstFactor)
{
    enable ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    if (enable)
        glBlendFunc(toOpenGL(srcFactor), toOpenGL(dstFactor));
}

void L3DRenderer::setCullFace(
    bool enable,
    const L3DCullFace &cullFace)
{
    enable ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    if (enable)
        glCullFace(toOpenGL(cullFace));
}

void L3DRenderer::drawMeshes(
    L3DCamera *camera,
    unsigned char renderLayer)
{
    if (!camera)
        return;

    if (!m_renderBucket.count(renderLayer))
        return;

    L3DVec3 cameraPos = camera->position();
    L3DMat4 vpMat = camera->proj * camera->view;
    L3DMeshList meshList = m_renderBucket.at(renderLayer);

    // Iterate over render bucket and render each collected mesh.
    // Meshes in bucket are ordered now by material to reduce context changes.
    for (L3DMeshList::const_iterator it = meshList.begin(); it != meshList.end(); ++it)
    {
        L3DMesh *mesh = *it;
        L3DMaterial *material = mesh->material();
        L3DShaderProgram *shaderProgram = material->shaderProgram();
        GLenum gl_draw_primitive = toOpenGL(mesh->drawPrimitive());
        unsigned int index_count = mesh->indexCount();
        unsigned int instance_count = mesh->instanceCount();

        // Binds VAO.
        glBindVertexArray(mesh->id());

        // Binds shaders.
        glUseProgram(shaderProgram->id());

        // Binds uniforms.
        L3DUniformMap uniforms = shaderProgram->uniforms();
        for (L3DUniformMap::iterator unif_it = uniforms.begin(); unif_it != uniforms.end(); ++unif_it)
            setUniform(shaderProgram->id(), unif_it->first.c_str(), unif_it->second);

        // Binds matrices and vectors.
        glUniform3fv(glGetUniformLocation(shaderProgram->id(), "u_cameraPos"), 1, glm::value_ptr(cameraPos));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram->id(), "u_vpMat"), 1, GL_FALSE, glm::value_ptr(vpMat));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram->id(), "u_viewMat"), 1, GL_FALSE, glm::value_ptr(camera->view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram->id(), "u_projMat"), 1, GL_FALSE, glm::value_ptr(camera->proj));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram->id(), "u_modelMat"), 1, GL_FALSE, glm::value_ptr(mesh->transMatrix));
        glUniformMatrix3fv(glGetUniformLocation(shaderProgram->id(), "u_normalMat"), 1, GL_FALSE, glm::value_ptr(mesh->normalMatrix()));

        // Binds material:
        std::string materialName = "u_material.";

        // 1. Colors.
        for (L3DColorRegistry::iterator col_it = material->colors.begin(); col_it != material->colors.end(); ++col_it)
            setUniform(shaderProgram->id(), (materialName + col_it->first).c_str(), col_it->second);

        // 2. Parameters.
        for (L3DParameterRegistry::iterator par_it = material->params.begin(); par_it != material->params.end(); ++par_it)
            setUniform(shaderProgram->id(), (materialName + par_it->first).c_str(), par_it->second);

        // 3. Textures.
        if (material->textures.size() > 0)
        {
            std::string samplerName = "u_";

            unsigned int i = 0;
            for (L3DTextureRegistry::iterator tex_it = material->textures.begin(); tex_it != material->textures.end(); ++tex_it)
            {
                L3DTexture *texture = tex_it->second;

                if (texture)
                {
                    GLenum gl_type = toOpenGL(texture->type());
                    GLint gl_sampler = glGetUniformLocation(shaderProgram->id(), (samplerName + tex_it->first).c_str());

                    // Activate texture unit and bind sampler.
                    glActiveTexture(GL_TEXTURE0 + i);
                    glBindTexture(gl_type, texture->id());
                    glUniform1i(gl_sampler, i);

                    // Set map flag.
                    glUniform1i(glGetUniformLocation(shaderProgram->id(), (samplerName + tex_it->first + "Enabled").c_str()), GL_TRUE);

                    ++i;
                }
            }
        }
        else
        {
            glBindTexture(GL_TEXTURE_1D, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindTexture(GL_TEXTURE_3D, 0);
        }

        // Binds lights.
        int activeLightCount = 0;
        for (L3DLightPool::iterator light_it = m_lights.begin(); light_it != m_lights.end(); ++light_it)
        {
            L3DLight *light = light_it->second;

            if (light && light->isOn() && L3D_TEST_BIT(light->renderLayerMask(), renderLayer))
            {
                std::ostringstream sstream;
                sstream << "u_light[" << activeLightCount << "]";
                std::string lightName = sstream.str();

                setUniform(shaderProgram->id(), (lightName + ".type").c_str(), light->type);
                setUniform(shaderProgram->id(), (lightName + ".position").c_str(), light->position);
                setUniform(shaderProgram->id(), (lightName + ".direction").c_str(), light->direction);
                setUniform(shaderProgram->id(), (lightName + ".color").c_str(), light->color);
                setUniform(shaderProgram->id(), (lightName + ".kc").c_str(), light->attenuation.kc);
                setUniform(shaderProgram->id(), (lightName + ".kl").c_str(), light->attenuation.kl);
                setUniform(shaderProgram->id(), (lightName + ".kq").c_str(), light->attenuation.kq);

                ++activeLightCount;
            }
        }

        // Passes count of active lights.
        setUniform(shaderProgram->id(), "u_lightNr", (int)activeLightCount);

        // Renders geometry.
        if (index_count > 0)
        {
            // Renders vertices using indices.
            if (instance_count > 1)
            {
                glDrawElementsInstanced(gl_draw_primitive, index_count, GL_UNSIGNED_INT, 0, instance_count);
            }
            else
            {
                glDrawElements(gl_draw_primitive, index_count, GL_UNSIGNED_INT, 0);
            }
        }
        else
        {
            // Renders vertices without using indices.
            if (instance_count > 1)
            {
                glDrawArraysInstanced(gl_draw_primitive, 0, mesh->vertexCount(), instance_count);
            }
            else
            {
                glDrawArrays(gl_draw_primitive, 0, mesh->vertexCount());
            }
        }
    }

    glBindVertexArray(0);
}

void L3DRenderer::recomputeRenderBucket()
{
    m_renderBucket.clear();

    // Put all meshes to the correct render layer into the render bucket.
    for (L3DMeshPool::const_iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
    {
        L3DMesh *mesh = it->second;

        if (mesh && mesh->material() && mesh->material()->shaderProgram())
        {
            m_renderBucket[mesh->renderLayer()].push_back(mesh);
        }
    }

    // Sort the generated render layers in the render bucket.
    for (L3DRenderBucket::const_iterator it = m_renderBucket.begin(); it != m_renderBucket.end(); ++it)
    {
        L3DMeshList meshList = it->second;
        meshList.sort(l3dMeshSortFunctor());
    }
}