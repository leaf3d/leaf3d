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
#include <leaf3d/L3DMesh.h>

using namespace l3d;

L3DMesh::L3DMesh(
    float* vertices,
    unsigned int vertexCount,
    unsigned int* indices,
    unsigned int indexCount,
    L3DMaterial* material,
    const VertexFormat& vertexFormat,
    const DrawType& drawType,
    const DrawPrimitive& drawPrimitive
) : L3DResource(L3D_MESH),
    m_material(material),
    m_vbo(0),
    m_ebo(0),
    m_vertexCount(vertexCount),
    m_indexCount(indexCount),
    m_vertexFormat(vertexFormat),
    m_drawType(drawType),
    m_drawPrimitive(drawPrimitive)
{
    GLuint vao;
    GLenum gl_drawType = GL_STATIC_DRAW;

    switch (drawType)
    {
    case L3D_DRAW_DYNAMIC:
        gl_drawType = GL_DYNAMIC_DRAW;
        break;
    default:
        break;
    }

    // Create Vertex Array Object.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it.
    if (vertices && vertexCount)
    {
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexFormat * sizeof(float), vertices, gl_drawType);
    }

    // Create an Element Buffer Object and copy the element data to it.
    if (indices && indexCount)
    {
        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, gl_drawType);
    }

    this->setId(vao);

    // Specify the layout of the vertex data.
    if (material && material->shaderProgram())
    {
        L3DShaderProgram* shaderProgram = material->shaderProgram();

        // Attributes.
        GLint posAttrib     = shaderProgram->getAttribLocation("position");
        GLint colAttrib     = shaderProgram->getAttribLocation("color");
        GLint tex0Attrib    = shaderProgram->getAttribLocation("texcoord0");
        GLint tex1Attrib    = shaderProgram->getAttribLocation("texcoord1");
        GLint tex2Attrib    = shaderProgram->getAttribLocation("texcoord2");
        GLint tex3Attrib    = shaderProgram->getAttribLocation("texcoord3");
        GLint norAttrib     = shaderProgram->getAttribLocation("normal");
        GLint tanAttrib     = shaderProgram->getAttribLocation("tan");
        GLint btanAttrib    = shaderProgram->getAttribLocation("btan");

        switch(vertexFormat)
        {
        case L3D_POS2:
            this->enableVertexAttribute(posAttrib, 2, GL_FLOAT, 2*sizeof(GLfloat), 0);
            break;
        case L3D_POS3:
            this->enableVertexAttribute(posAttrib, 3, GL_FLOAT, 3*sizeof(GLfloat), 0);
            break;
        case L3D_POS2_UV2:
            this->enableVertexAttribute(posAttrib, 2, GL_FLOAT, 4*sizeof(GLfloat), 0);
            this->enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 4*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
            break;
        case L3D_POS3_UV2:
            this->enableVertexAttribute(posAttrib, 3, GL_FLOAT, 5*sizeof(GLfloat), 0);
            this->enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
            break;
        case L3D_POS2_COL3_UV2:
            this->enableVertexAttribute(posAttrib, 2, GL_FLOAT, 7*sizeof(GLfloat), 0);
            this->enableVertexAttribute(colAttrib, 3, GL_FLOAT, 7*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
            this->enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 7*sizeof(GLfloat), (void*)(5*sizeof(GLfloat)));
            break;
        case L3D_POS3_COL3_UV2:
            this->enableVertexAttribute(posAttrib, 3, GL_FLOAT, 8*sizeof(GLfloat), 0);
            this->enableVertexAttribute(colAttrib, 3, GL_FLOAT, 8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
            this->enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 8*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
            break;
        case L3D_POS3_NOR3_COL3_UV2:
            this->enableVertexAttribute(posAttrib, 3, GL_FLOAT, 11*sizeof(GLfloat), 0);
            this->enableVertexAttribute(norAttrib, 3, GL_FLOAT, 11*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
            this->enableVertexAttribute(colAttrib, 3, GL_FLOAT, 11*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
            this->enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 11*sizeof(GLfloat), (void*)(9*sizeof(GLfloat)));
            break;
        case L3D_POS3_NOR3_TAN3_COL3_UV2:
            this->enableVertexAttribute(posAttrib, 3, GL_FLOAT, 14*sizeof(GLfloat), 0);
            this->enableVertexAttribute(norAttrib, 3, GL_FLOAT, 14*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
            this->enableVertexAttribute(tanAttrib, 3, GL_FLOAT, 14*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
            this->enableVertexAttribute(colAttrib, 3, GL_FLOAT, 14*sizeof(GLfloat), (void*)(9*sizeof(GLfloat)));
            this->enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 14*sizeof(GLfloat), (void*)(12*sizeof(GLfloat)));
            break;
        case L3D_POS3_NOR3_TAN3_BTAN3_COL3_UV2:
            this->enableVertexAttribute(posAttrib, 3, GL_FLOAT, 17*sizeof(GLfloat), 0);
            this->enableVertexAttribute(norAttrib, 3, GL_FLOAT, 17*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
            this->enableVertexAttribute(tanAttrib, 3, GL_FLOAT, 17*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
            this->enableVertexAttribute(btanAttrib, 3, GL_FLOAT, 17*sizeof(GLfloat), (void*)(9*sizeof(GLfloat)));
            this->enableVertexAttribute(colAttrib, 3, GL_FLOAT, 17*sizeof(GLfloat), (void*)(12*sizeof(GLfloat)));
            this->enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 17*sizeof(GLfloat), (void*)(15*sizeof(GLfloat)));
            break;
        case L3D_POS3_NOR3_TAN3_BTAN3_COL3_UV2_UV2:
            this->enableVertexAttribute(posAttrib, 3, GL_FLOAT, 19*sizeof(GLfloat), 0);
            this->enableVertexAttribute(norAttrib, 3, GL_FLOAT, 19*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
            this->enableVertexAttribute(tanAttrib, 3, GL_FLOAT, 19*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
            this->enableVertexAttribute(btanAttrib, 3, GL_FLOAT, 19*sizeof(GLfloat), (void*)(9*sizeof(GLfloat)));
            this->enableVertexAttribute(colAttrib, 3, GL_FLOAT, 19*sizeof(GLfloat), (void*)(12*sizeof(GLfloat)));
            this->enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 19*sizeof(GLfloat), (void*)(15*sizeof(GLfloat)));
            this->enableVertexAttribute(tex1Attrib, 2, GL_FLOAT, 19*sizeof(GLfloat), (void*)(17*sizeof(GLfloat)));
            break;
        case L3D_POS3_NOR3_TAN3_BTAN3_COL3_UV2_UV2_UV2:
            this->enableVertexAttribute(posAttrib, 3, GL_FLOAT, 21*sizeof(GLfloat), 0);
            this->enableVertexAttribute(norAttrib, 3, GL_FLOAT, 21*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
            this->enableVertexAttribute(tanAttrib, 3, GL_FLOAT, 21*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
            this->enableVertexAttribute(btanAttrib, 3, GL_FLOAT, 21*sizeof(GLfloat), (void*)(9*sizeof(GLfloat)));
            this->enableVertexAttribute(colAttrib, 3, GL_FLOAT, 21*sizeof(GLfloat), (void*)(12*sizeof(GLfloat)));
            this->enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 21*sizeof(GLfloat), (void*)(15*sizeof(GLfloat)));
            this->enableVertexAttribute(tex1Attrib, 2, GL_FLOAT, 21*sizeof(GLfloat), (void*)(17*sizeof(GLfloat)));
            this->enableVertexAttribute(tex2Attrib, 2, GL_FLOAT, 21*sizeof(GLfloat), (void*)(19*sizeof(GLfloat)));
            break;
        case L3D_POS3_NOR3_TAN3_BTAN3_COL3_UV2_UV2_UV2_UV2:
            this->enableVertexAttribute(posAttrib, 3, GL_FLOAT, 23*sizeof(GLfloat), 0);
            this->enableVertexAttribute(norAttrib, 3, GL_FLOAT, 23*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
            this->enableVertexAttribute(tanAttrib, 3, GL_FLOAT, 23*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
            this->enableVertexAttribute(btanAttrib, 3, GL_FLOAT, 23*sizeof(GLfloat), (void*)(9*sizeof(GLfloat)));
            this->enableVertexAttribute(colAttrib, 3, GL_FLOAT, 23*sizeof(GLfloat), (void*)(12*sizeof(GLfloat)));
            this->enableVertexAttribute(tex0Attrib, 2, GL_FLOAT, 23*sizeof(GLfloat), (void*)(15*sizeof(GLfloat)));
            this->enableVertexAttribute(tex1Attrib, 2, GL_FLOAT, 23*sizeof(GLfloat), (void*)(17*sizeof(GLfloat)));
            this->enableVertexAttribute(tex2Attrib, 2, GL_FLOAT, 23*sizeof(GLfloat), (void*)(19*sizeof(GLfloat)));
            this->enableVertexAttribute(tex3Attrib, 2, GL_FLOAT, 23*sizeof(GLfloat), (void*)(21*sizeof(GLfloat)));
            break;
        default:
            break;
        }
    }

    glBindVertexArray(0);
}

L3DMesh::~L3DMesh()
{
    GLuint vao = this->id();

    glDeleteBuffers(1, &m_ebo);
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &vao);
}

void L3DMesh::translate(const L3DVec3& trans)
{
    this->trans = glm::translate(this->trans, trans);
}

void L3DMesh::rotate(
    float radians,
    const L3DVec3& direction
)
{
    this->trans = glm::rotate(this->trans, radians, direction);
}

void L3DMesh::enableVertexAttribute(
    GLuint attrib,
    GLint size,
    GLenum type,
    GLsizei stride,
    void* startPtr,
    GLboolean normalized
)
{
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, size, type, normalized, stride, startPtr);
}
