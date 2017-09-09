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

#include <leaf3d/L3DRenderer.h>
#include <leaf3d/L3DLight.h>

using namespace l3d;

L3DLight::L3DLight(
    L3DRenderer* renderer,
    const L3DLightType& type,
    const L3DVec3& position,
    const L3DVec3& direction,
    const L3DVec4& color,
    const L3DLightAttenuation& attenuation,
    unsigned int renderLayerMask
) : L3DResource(L3D_LIGHT, renderer),
    type(type),
    position(position),
    direction(direction),
    color(color),
    attenuation(attenuation),
    m_renderLayerMask(renderLayerMask)
{
    if (renderer) renderer->addLight(this);
}

void L3DLight::setRenderLayerMask(unsigned int renderLayerMask)
{
    m_renderLayerMask = renderLayerMask;

    // TODO: can we optimize rendering from here?
}

void L3DLight::translate(const L3DVec3& movement)
{
    this->position += movement;
}

void L3DLight::lookAt(const L3DVec3& targetPosition)
{
    this->direction = glm::normalize(this->position - targetPosition);
}

L3DLight* L3DLight::createDirectionalLight(
    L3DRenderer* renderer,
    const L3DVec3& direction,
    const L3DVec4& color,
    unsigned int renderLayerMask
)
{
    return new L3DLight(
        renderer,
        L3D_LIGHT_DIRECTIONAL,
        L3DVec3(0, 0, 0),
        glm::normalize(direction),
        color,
        L3DLightAttenuation(0, 0, 0),
        renderLayerMask
    );
}

L3DLight* L3DLight::createPointLight(
    L3DRenderer* renderer,
    const L3DVec3& position,
    const L3DVec4& color,
    const L3DLightAttenuation& attenuation,
    unsigned int renderLayerMask
)
{
    return new L3DLight(
        renderer,
        L3D_LIGHT_POINT,
        position,
        L3DVec3(0, 0, 0),
        color,
        attenuation,
        renderLayerMask
    );
}

L3DLight* L3DLight::createSpotLight(
    L3DRenderer* renderer,
    const L3DVec3& position,
    const L3DVec3& direction,
    const L3DVec4& color,
    const L3DLightAttenuation& attenuation,
    unsigned int renderLayerMask
)
{
    return new L3DLight(
        renderer,
        L3D_LIGHT_SPOT,
        position,
        direction,
        color,
        attenuation,
        renderLayerMask
    );
}
