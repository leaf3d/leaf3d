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

#include <leaf3d/L3DBuffer.h>
#include <leaf3d/L3DTexture.h>
#include <leaf3d/L3DMaterial.h>
#include <leaf3d/L3DShaderProgram.h>
#include <leaf3d/L3DRenderer.h>
#include <leaf3d/L3DMesh.h>

using namespace l3d;

L3DMesh::L3DMesh(
    L3DRenderer* renderer,
    float* vertices,
    unsigned int vertexCount,
    unsigned int* indices,
    unsigned int indexCount,
    L3DMaterial* material,
    const VertexFormat& vertexFormat,
    const L3DMat4& transMatrix,
    const DrawType& drawType,
    const DrawPrimitive& drawPrimitive
) : L3DResource(L3D_MESH, renderer),
    transMatrix(transMatrix),
    m_vertexBuffer(0),
    m_indexBuffer(0),
    m_material(material),
    m_vertexFormat(vertexFormat),
    m_drawPrimitive(drawPrimitive)
{
    if (vertices && vertexCount)
        m_vertexBuffer = new L3DBuffer(renderer, L3D_BUFFER_VERTEX, vertices, vertexCount * vertexFormat * sizeof(float), vertexFormat * sizeof(float), drawType);

    if (indices && indexCount)
        m_indexBuffer = new L3DBuffer(renderer, L3D_BUFFER_INDEX, indices, indexCount * sizeof(unsigned int), sizeof(unsigned int), drawType);

    if (renderer) renderer->addMesh(this);
}

L3DMesh::L3DMesh(
    L3DRenderer* renderer,
    L3DBuffer* vertexBuffer,
    L3DBuffer* indexBuffer,
    L3DMaterial* material,
    const VertexFormat& vertexFormat,
    const L3DMat4& transMatrix,
    const DrawType& drawType,
    const DrawPrimitive& drawPrimitive
) : L3DResource(L3D_MESH, renderer),
    transMatrix(transMatrix),
    m_vertexBuffer(0),
    m_indexBuffer(0),
    m_material(material),
    m_vertexFormat(vertexFormat),
    m_drawPrimitive(drawPrimitive)
{
    if (vertexBuffer
        && vertexBuffer->stride() == vertexFormat * sizeof(float)
        && vertexBuffer->drawType() == drawType)
        m_vertexBuffer = vertexBuffer;

    if (indexBuffer
        && indexBuffer->stride() == sizeof(unsigned int)
        && indexBuffer->drawType() == drawType)
        m_indexBuffer = indexBuffer;

    if (renderer) renderer->addMesh(this);
}

unsigned int L3DMesh::vertexCount() const
{
    return m_vertexBuffer ? m_vertexBuffer->count() : 0;
}

unsigned int L3DMesh::indexCount() const
{
    return m_indexBuffer ? m_indexBuffer->count() : 0;
}

void L3DMesh::translate(const L3DVec3& movement)
{
    transMatrix = glm::translate(this->transMatrix, movement);
}

void L3DMesh::rotate(
    float radians,
    const L3DVec3& direction
)
{
    this->transMatrix = glm::rotate(this->transMatrix, radians, direction);
}

void L3DMesh::scale(
    const L3DVec3& factor
)
{
    this->transMatrix = glm::scale(this->transMatrix, factor);
}
