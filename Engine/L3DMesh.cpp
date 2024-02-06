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
    L3DRenderer *renderer,
    float *vertices,
    unsigned int vertexCount,
    unsigned int *indices,
    unsigned int indexCount,
    L3DMaterial *material,
    const L3DVertexFormat &vertexFormat,
    const L3DMat4 &transMatrix,
    const L3DDrawType &drawType,
    const L3DDrawPrimitive &drawPrimitive,
    unsigned char renderLayer) : L3DResource(L3D_MESH, renderer),
                                 transMatrix(transMatrix),
                                 m_vertexBuffer(0),
                                 m_indexBuffer(0),
                                 m_instanceBuffer(0),
                                 m_material(material),
                                 m_vertexFormat(vertexFormat),
                                 m_instanceFormat(L3D_INVALID_INSTANCE_FORMAT),
                                 m_drawPrimitive(drawPrimitive),
                                 m_renderLayer(renderLayer),
                                 m_sortKey(0)
{
    if (vertices && vertexCount)
        m_vertexBuffer = new L3DBuffer(renderer, L3D_BUFFER_VERTEX, vertices, vertexCount * vertexFormat * sizeof(float), vertexFormat * sizeof(float), drawType);

    if (indices && indexCount)
        m_indexBuffer = new L3DBuffer(renderer, L3D_BUFFER_INDEX, indices, indexCount * sizeof(unsigned int), sizeof(unsigned int), drawType);

    this->updateSortKey();

    if (renderer)
        renderer->addMesh(this);
}

L3DMesh::L3DMesh(
    L3DRenderer *renderer,
    L3DBuffer *vertexBuffer,
    L3DBuffer *indexBuffer,
    L3DMaterial *material,
    const L3DVertexFormat &vertexFormat,
    const L3DMat4 &transMatrix,
    const L3DDrawType &drawType,
    const L3DDrawPrimitive &drawPrimitive,
    unsigned char renderLayer) : L3DResource(L3D_MESH, renderer),
                                 transMatrix(transMatrix),
                                 m_vertexBuffer(0),
                                 m_indexBuffer(0),
                                 m_instanceBuffer(0),
                                 m_material(material),
                                 m_vertexFormat(vertexFormat),
                                 m_instanceFormat(L3D_INVALID_INSTANCE_FORMAT),
                                 m_drawPrimitive(drawPrimitive),
                                 m_renderLayer(renderLayer),
                                 m_sortKey(0)
{
    if (vertexBuffer && vertexBuffer->stride() == vertexFormat * sizeof(float) && vertexBuffer->drawType() == drawType)
        m_vertexBuffer = vertexBuffer;

    if (indexBuffer && indexBuffer->stride() == sizeof(unsigned int) && indexBuffer->drawType() == drawType)
        m_indexBuffer = indexBuffer;

    this->updateSortKey();

    if (renderer)
        renderer->addMesh(this);
}

L3DMat3 L3DMesh::normalMatrix() const
{
    return glm::transpose(glm::inverse(L3DMat3(this->transMatrix)));
}

unsigned int L3DMesh::vertexCount() const
{
    return m_vertexBuffer ? m_vertexBuffer->count() : 0;
}

unsigned int L3DMesh::indexCount() const
{
    return m_indexBuffer ? m_indexBuffer->count() : 0;
}

unsigned int L3DMesh::instanceCount() const
{
    return m_instanceBuffer ? m_instanceBuffer->count() : 1;
}

unsigned int L3DMesh::primitiveCount() const
{
    return m_indexBuffer ? m_indexBuffer->count() / m_drawPrimitive : 0;
}

void L3DMesh::recalculateTangents()
{
    if (this->vertexFormat() < L3D_VERTEX_POS3_NOR3_TAN3_UV2)
        return;

    std::map<unsigned int, L3DVec3> tans;

    unsigned int *indices = m_indexBuffer->data<unsigned int>();
    float *vertices = m_vertexBuffer->data<float>();

    for (unsigned int a = 0; a < this->primitiveCount(); ++a)
    {
        unsigned int primitiveOffset = a * m_drawPrimitive;

        unsigned int i1 = indices[primitiveOffset + 0];
        unsigned int i2 = indices[primitiveOffset + 1];
        unsigned int i3 = indices[primitiveOffset + 2];

        unsigned i1Offset = i1 * m_vertexFormat;
        unsigned i2Offset = i2 * m_vertexFormat;
        unsigned i3Offset = i3 * m_vertexFormat;

        // Position.
        const L3DVec3 v1(vertices[i1Offset + 0], vertices[i1Offset + 1], vertices[i1Offset + 2]);
        const L3DVec3 v2(vertices[i2Offset + 0], vertices[i2Offset + 1], vertices[i2Offset + 2]);
        const L3DVec3 v3(vertices[i3Offset + 0], vertices[i3Offset + 1], vertices[i3Offset + 2]);

        // UV.
        const L3DVec2 w1(vertices[i1Offset + 9], vertices[i1Offset + 10]);
        const L3DVec2 w2(vertices[i2Offset + 9], vertices[i2Offset + 10]);
        const L3DVec2 w3(vertices[i3Offset + 9], vertices[i3Offset + 10]);

        // Calculate tangent.
        glm::vec3 tangent1;

        glm::vec3 edge1 = v2 - v1;
        glm::vec3 edge2 = v3 - v1;
        glm::vec2 deltaUV1 = w2 - w1;
        glm::vec2 deltaUV2 = w3 - w1;

        GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent1 = glm::normalize(tangent1);

        tans[i1] += tangent1;
        tans[i2] += tangent1;
        tans[i3] += tangent1;
    }

    for (unsigned int v = 0; v < this->vertexCount(); ++v)
    {
        unsigned int vertexOffset = v * m_vertexFormat;

        L3DVec3 norm(vertices[vertexOffset + 3], vertices[vertexOffset + 4], vertices[vertexOffset + 5]);
        L3DVec3 tan = glm::normalize(tans[v] - glm::dot(tans[v], norm) * norm);

        // Updates vertex tangent (XYZ).
        vertices[vertexOffset + 6] = tan.x;
        vertices[vertexOffset + 7] = tan.y;
        vertices[vertexOffset + 8] = tan.z;
    }
}

void L3DMesh::translate(const L3DVec3 &movement)
{
    transMatrix = glm::translate(this->transMatrix, movement);
}

void L3DMesh::rotate(
    float radians,
    const L3DVec3 &direction)
{
    this->transMatrix = glm::rotate(this->transMatrix, radians, direction);
}

void L3DMesh::scale(
    const L3DVec3 &factor)
{
    this->transMatrix = glm::scale(this->transMatrix, factor);
}

void L3DMesh::setMaterial(L3DMaterial *material)
{
    if (m_material != material)
    {
        m_material = material;
        this->updateSortKey();
    }
}

void L3DMesh::setRenderLayer(unsigned char renderLayer)
{
    if (m_renderLayer != renderLayer)
    {
        m_renderLayer = renderLayer;
        this->updateSortKey();
    }
}

void L3DMesh::setInstances(
    L3DBuffer *instanceBuffer,
    const L3DInstanceFormat &instanceFormat)
{
    if (instanceBuffer && instanceFormat)
    {
        // TODO: clean previous instance buffer.
        m_instanceBuffer = instanceBuffer;
        m_instanceFormat = instanceFormat;
        this->updateSortKey();

        L3DRenderer *renderer = this->renderer();
        renderer->removeMesh(this);
        renderer->addMesh(this);
    }
}

void L3DMesh::setInstances(
    void *instances,
    unsigned int instanceCount,
    const L3DInstanceFormat &instanceFormat)
{
    if (instances && instanceCount && instanceFormat)
    {
        L3DBuffer *instanceBuffer = new L3DBuffer(
            this->renderer(),
            L3D_BUFFER_INSTANCE,
            instances,
            instanceCount * instanceFormat * sizeof(float),
            instanceFormat * sizeof(float),
            L3D_DRAW_STATIC);

        this->setInstances(instanceBuffer, instanceFormat);
    }
}

void L3DMesh::updateSortKey()
{
    m_sortKey = (m_renderLayer << 24) | ((m_material ? m_material->id() : 0) << 8);

    this->renderer()->recomputeRenderBucket();
}
