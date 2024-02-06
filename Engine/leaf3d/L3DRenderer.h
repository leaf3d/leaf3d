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

#ifndef L3D_L3DRENDERER_H
#define L3D_L3DRENDERER_H
#pragma once

#include <map>
#include <list>
#include "leaf3d/types.h"

namespace l3d
{
    class L3DResource;
    class L3DBuffer;
    class L3DTexture;
    class L3DShader;
    class L3DShaderProgram;
    class L3DFrameBuffer;
    class L3DMaterial;
    class L3DCamera;
    class L3DLight;
    class L3DMesh;
    class L3DRenderQueue;

    typedef std::map<unsigned int, L3DBuffer *> L3DBufferPool;
    typedef std::map<unsigned int, L3DTexture *> L3DTexturePool;
    typedef std::map<unsigned int, L3DShader *> L3DShaderPool;
    typedef std::map<unsigned int, L3DShaderProgram *> L3DShaderProgramPool;
    typedef std::map<unsigned int, L3DFrameBuffer *> L3DFrameBufferPool;
    typedef std::map<unsigned int, L3DMaterial *> L3DMaterialPool;
    typedef std::map<unsigned int, L3DCamera *> L3DCameraPool;
    typedef std::map<unsigned int, L3DLight *> L3DLightPool;
    typedef std::map<unsigned int, L3DMesh *> L3DMeshPool;
    typedef std::map<unsigned int, L3DRenderQueue *> L3DRenderQueuePool;
    typedef std::list<L3DMesh *> L3DMeshList;
    typedef std::map<unsigned int, L3DMeshList> L3DRenderBucket;

    class L3DRenderer
    {
    private:
        L3DBufferPool m_buffers;
        L3DTexturePool m_textures;
        L3DShaderPool m_shaders;
        L3DShaderProgramPool m_shaderPrograms;
        L3DFrameBufferPool m_frameBuffers;
        L3DMaterialPool m_materials;
        L3DCameraPool m_cameras;
        L3DLightPool m_lights;
        L3DMeshPool m_meshes;
        L3DRenderQueuePool m_renderQueues;
        L3DRenderBucket m_renderBucket;

    public:
        L3DRenderer();
        virtual ~L3DRenderer();

        // Init and clearing.
        int init();
        int terminate();

        // Rendering.
        void renderFrame(
            L3DCamera *camera,
            L3DRenderQueue *renderQueue);

        // Add resources to renderer.
        void addResource(L3DResource *resource);
        void addBuffer(L3DBuffer *buffer);
        void addTexture(L3DTexture *texture);
        void addShader(L3DShader *shader);
        void addShaderProgram(L3DShaderProgram *shaderProgram);
        void addFrameBuffer(L3DFrameBuffer *frameBuffer);
        void addMaterial(L3DMaterial *material);
        void addCamera(L3DCamera *camera);
        void addLight(L3DLight *light);
        void addMesh(L3DMesh *mesh);
        void addRenderQueue(L3DRenderQueue *renderQueue);

        // Remove resources from renderer.
        void removeResource(L3DResource *resource);
        void removeBuffer(L3DBuffer *buffer);
        void removeTexture(L3DTexture *texture);
        void removeShader(L3DShader *shader);
        void removeShaderProgram(L3DShaderProgram *shaderProgram);
        void removeFrameBuffer(L3DFrameBuffer *frameBuffer);
        void removeMaterial(L3DMaterial *material);
        void removeCamera(L3DCamera *camera);
        void removeLight(L3DLight *light);
        void removeMesh(L3DMesh *mesh);
        void removeRenderQueue(L3DRenderQueue *renderQueue);

        // Convert handle to resource pointer.
        L3DResource *getResource(const L3DHandle &handle) const;
        L3DBuffer *getBuffer(const L3DHandle &handle) const;
        L3DTexture *getTexture(const L3DHandle &handle) const;
        L3DShader *getShader(const L3DHandle &handle) const;
        L3DShaderProgram *getShaderProgram(const L3DHandle &handle) const;
        L3DFrameBuffer *getFrameBuffer(const L3DHandle &handle) const;
        L3DMaterial *getMaterial(const L3DHandle &handle) const;
        L3DCamera *getCamera(const L3DHandle &handle) const;
        L3DLight *getLight(const L3DHandle &handle) const;
        L3DMesh *getMesh(const L3DHandle &handle) const;
        L3DRenderQueue *getRenderQueue(const L3DHandle &handle) const;

        // Return size of internal resource pools.
        unsigned int bufferCount() const { return m_buffers.size(); }
        unsigned int textureCount() const { return m_textures.size(); }
        unsigned int shaderCount() const { return m_shaders.size(); }
        unsigned int shaderProgramCount() const { return m_shaderPrograms.size(); }
        unsigned int frameBufferCount() const { return m_frameBuffers.size(); }
        unsigned int materialCount() const { return m_materials.size(); }
        unsigned int cameraCount() const { return m_cameras.size(); }
        unsigned int lightCount() const { return m_lights.size(); }
        unsigned int meshCount() const { return m_meshes.size(); }
        unsigned int renderQueueCount() const { return m_renderQueues.size(); }

        // Render actions.
        void switchFrameBuffer(L3DFrameBuffer *frameBuffer = 0);
        void clearBuffers(
            bool colorBuffer = true,
            bool depthBuffer = true,
            bool stencilBuffer = true,
            const L3DVec4 &clearColor = L3DVec4(1, 1, 1, 1));
        void setDepthTest(
            bool enable = true,
            const L3DDepthFactor &factor = L3D_LESS);
        void setDepthMask(bool enable = true);
        void setStencilTest(bool enable = true);
        void setBlend(
            bool enable = true,
            const L3DBlendFactor &srcFactor = L3D_SRC_ALPHA,
            const L3DBlendFactor &dstFactor = L3D_ONE_MINUS_SRC_ALPHA);
        void setCullFace(
            bool enable = true,
            const L3DCullFace &cullFace = L3D_BACK_FACE);
        void drawMeshes(
            L3DCamera *camera,
            unsigned char renderLayer = 0);
        void recomputeRenderBucket();
    };
}

#endif // L3D_L3DRENDERER_H
