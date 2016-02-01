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

#include <leaf3d/L3DLight.h>
#include <catch/catch.hpp>

using namespace l3d;

TEST_CASE( "Test getting L3DLight type", "[leaf3d][light][get][type]" )
{
    L3DLight* dirLight = L3DLight::createDirectionalLight(
        0, L3DVec3(0, -1, 0)
    );

    REQUIRE(dirLight->type == L3D_LIGHT_DIRECTIONAL);

    L3DLight* pointLight = L3DLight::createPointLight(
        0, L3DVec3(0, 0, 0)
    );

    REQUIRE(pointLight->type == L3D_LIGHT_POINT);

    L3DLight* spotLight = L3DLight::createSpotLight(
        0, L3DVec3(0, 0, 0)
    );

    REQUIRE(spotLight->type == L3D_LIGHT_SPOT);
}

TEST_CASE( "Test L3DLight active status", "[leaf3d][light][get][isOn]" )
{
    L3DVec4 lightColor = L3DVec4(1, 1, 1, 1);

    L3DLight* light = L3DLight::createPointLight(
        0, L3DVec3(0, 0, 0),
        lightColor
    );

    REQUIRE(light->isOn() == true);

    light->color.a = 0.6f;

    REQUIRE(light->isOn() == true);

    light->color.a = 0.2f;

    REQUIRE(light->isOn() == true);

    light->color.a = 0.01f;

    REQUIRE(light->isOn() == true);

    light->color.a = 0.0f;

    REQUIRE(light->isOn() == false);
}
