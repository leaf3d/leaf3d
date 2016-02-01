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

#include <leaf3d/L3DCamera.h>
#include <catch/catch.hpp>

using namespace l3d;

TEST_CASE( "Test getting camera position from View matrix", "[leaf3d][camera][get][position][from][view][matrix]" )
{
    L3DVec3 camPos(0, 5, -6.5f);

    L3DCamera* camera = new L3DCamera(
        0, "Default",
        glm::lookAt(
            camPos,
            L3DVec3(0, 0, 0),
            L3DVec3(0, 1, 0)
        )
    );

    L3DVec3 retCamPos = camera->position();

    REQUIRE(Approx(retCamPos.x) == camPos.x);
    REQUIRE(Approx(retCamPos.y) == camPos.y);
    REQUIRE(Approx(retCamPos.z) == camPos.z);
}
