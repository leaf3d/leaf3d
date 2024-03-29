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

#ifndef L3D_L3DLIGHT_H
#define L3D_L3DLIGHT_H
#pragma once

#include "leaf3d/L3DResource.h"

namespace l3d
{
    class L3DLight : public L3DResource
    {
    public:
        L3DLightType type;
        L3DVec3 position;
        L3DVec3 direction;
        L3DVec4 color;
        L3DLightAttenuation attenuation;

    protected:
        unsigned int m_renderLayerMask;

    public:
        L3DLight(
            L3DRenderer *renderer,
            const L3DLightType &type,
            const L3DVec3 &position,
            const L3DVec3 &direction,
            const L3DVec4 &color = L3DVec4(1, 1, 1, 1),
            const L3DLightAttenuation &attenuation = L3DLightAttenuation(1, 0.045f, 0.0075f),
            unsigned int renderLayerMask = L3D_DEFAULT_LIGHT_RENDERLAYER_MASK);
        ~L3DLight() {}

        unsigned int renderLayerMask() const { return m_renderLayerMask; }
        bool isOn() const { return (this->color.a > 0.0f); }

        void setRenderLayerMask(unsigned int renderLayerMask);

        void translate(const L3DVec3 &movement);
        void rotate(
            float radians,
            const L3DVec3 &direction = glm::vec3(0.0f, 1.0f, 0.0f));
        void lookAt(const L3DVec3 &targetPosition);

        static L3DLight *createDirectionalLight(
            L3DRenderer *renderer,
            const L3DVec3 &direction,
            const L3DVec4 &color = L3DVec4(1, 1, 1, 1),
            unsigned int renderLayerMask = L3D_DEFAULT_LIGHT_RENDERLAYER_MASK);

        static L3DLight *createPointLight(
            L3DRenderer *renderer,
            const L3DVec3 &position,
            const L3DVec4 &color = L3DVec4(1, 1, 1, 1),
            const L3DLightAttenuation &attenuation = L3DLightAttenuation(1, 0.045f, 0.0075f),
            unsigned int renderLayerMask = L3D_DEFAULT_LIGHT_RENDERLAYER_MASK);

        static L3DLight *createSpotLight(
            L3DRenderer *renderer,
            const L3DVec3 &position,
            const L3DVec3 &direction = L3DVec3(0, -1, 0),
            const L3DVec4 &color = L3DVec4(1, 1, 1, 1),
            const L3DLightAttenuation &attenuation = L3DLightAttenuation(1, 0.045f, 0.0075f),
            unsigned int renderLayerMask = L3D_DEFAULT_LIGHT_RENDERLAYER_MASK);
    };
}

#endif // L3D_L3DLIGHT_H
