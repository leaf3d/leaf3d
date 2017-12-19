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

#ifndef L3D_L3DMESH_H
#define L3D_L3DMESH_H
#pragma once

#include "leaf3d/L3DResource.h"

namespace l3d
{
    class L3DBuffer;
    class L3DMaterial;

    class L3DMesh : public L3DResource
    {
    public:
        L3DMat4             transMatrix;

    private:
        L3DBuffer*          m_vertexBuffer;
        L3DBuffer*          m_indexBuffer;
        L3DBuffer*          m_instanceBuffer;
        L3DMaterial*        m_material;
        L3DVertexFormat     m_vertexFormat;
        L3DInstanceFormat   m_instanceFormat;
        L3DDrawPrimitive    m_drawPrimitive;
        unsigned char       m_renderLayer;
        unsigned int        m_sortKey;

    public:
        L3DMesh(
            L3DRenderer* renderer,
            float* vertices,
            unsigned int vertexCount,
            unsigned int* indices,
            unsigned int indexCount,
            L3DMaterial* material,
            const L3DVertexFormat& vertexFormat,
            const L3DMat4& transMatrix = L3DMat4(),
            const L3DDrawType& drawType = L3D_DRAW_STATIC,
            const L3DDrawPrimitive& drawPrimitive = L3D_DRAW_TRIANGLES,
            unsigned char renderLayer = L3D_OPAQUE_MESH_RENDERLAYER
        );
        L3DMesh(
            L3DRenderer *renderer,
            L3DBuffer* vertexBuffer,
            L3DBuffer* indexBuffer,
            L3DMaterial* material,
            const L3DVertexFormat& vertexFormat,
            const L3DMat4& transMatrix = L3DMat4(),
            const L3DDrawType& drawType = L3D_DRAW_STATIC,
            const L3DDrawPrimitive& drawPrimitive = L3D_DRAW_TRIANGLES,
            unsigned char renderLayer = L3D_OPAQUE_MESH_RENDERLAYER
        );
        ~L3DMesh() {}

        L3DBuffer*          vertexBuffer() const { return m_vertexBuffer; }
        L3DBuffer*          indexBuffer() const { return m_indexBuffer; }
        L3DBuffer*          instanceBuffer() const { return m_instanceBuffer; }
        L3DMaterial*        material() const { return m_material; }
        L3DVertexFormat     vertexFormat() const { return m_vertexFormat; }
        L3DInstanceFormat   instanceFormat() const { return m_instanceFormat; }
        L3DDrawPrimitive    drawPrimitive() const { return m_drawPrimitive; }
        unsigned char       renderLayer() const { return m_renderLayer; }
        unsigned int        sortKey() const { return m_sortKey; }

        L3DMat3             normalMatrix() const;
        unsigned int        vertexCount() const;
        unsigned int        indexCount() const;
        unsigned int        instanceCount() const;
        unsigned int        primitiveCount() const;

        void recalculateTangents();

        void translate(const L3DVec3& movement);
        void rotate(
            float radians,
            const L3DVec3& direction = glm::vec3(0.0f, 1.0f, 0.0f)
        );
        void scale(const L3DVec3& factor);

        void setMaterial(L3DMaterial* material);
        void setRenderLayer(unsigned char renderLayer);
        void setInstances(
            L3DBuffer* instanceBuffer,
            const L3DInstanceFormat& instanceFormat
        );
        void setInstances(
            void* instances,
            unsigned int instanceCount,
            const L3DInstanceFormat& instanceFormat
        );

    protected:
        void updateSortKey();
    };
}

#endif // L3D_L3DMESH_H
