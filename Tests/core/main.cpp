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

#include <leaf3d/types.h>
#include <catch/catch.hpp>

using namespace l3d;

TEST_CASE("Test L3D_BIT", "[leaf3d][core][L3D_BIT]")
{
    REQUIRE(L3D_BIT(0) == 1);
    REQUIRE(L3D_BIT(1) == 2);
    REQUIRE(L3D_BIT(2) == 4);
    REQUIRE(L3D_BIT(3) == 8);
    REQUIRE(L3D_BIT(4) == 16);
    REQUIRE(L3D_BIT(5) == 32);
    REQUIRE(L3D_BIT(6) == 64);
}

TEST_CASE("Test L3D_SET_BIT", "[leaf3d][core][L3D_BIT][L3D_SET_BIT]")
{
    REQUIRE(L3D_SET_BIT(3, 0, false) == 2); // 0...11 -> 0...10
    REQUIRE(L3D_SET_BIT(2, 0, true) == 3);  // 0...10 -> 0...11
    REQUIRE(L3D_SET_BIT(3, 1, false) == 1); // 0...11 -> 0...01
    REQUIRE(L3D_SET_BIT(1, 1, true) == 3);  // 0...01 -> 0...11
}

TEST_CASE("Test L3D_TEST_BIT", "[leaf3d][core][L3D_BIT][L3D_TEST_BIT]")
{
    REQUIRE(L3D_TEST_BIT(3, 0) == true);  // 0...TT
    REQUIRE(L3D_TEST_BIT(2, 0) == false); // 0...TF
    REQUIRE(L3D_TEST_BIT(1, 1) == false); // 0...FT
    REQUIRE(L3D_TEST_BIT(3, 1) == true);  // 0...TT
}
