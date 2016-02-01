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

#ifndef L3D_L3DCAMERA_H
#define L3D_L3DCAMERA_H
#pragma once

#include "leaf3d/L3DResource.h"

namespace l3d
{
    class L3DCamera : public L3DResource
    {
    public:
        L3DMat4 view;
        L3DMat4 proj;

    protected:
        const char* m_name;

    public:
        L3DCamera(
            L3DRenderer* renderer,
            const char* name = "Default",
            const L3DMat4& view = glm::lookAt(
                glm::vec3(0.0f, 0.0f, 8.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            ),
            const L3DMat4& proj = glm::perspective(45.0f, 1.0f, 1.0f, 100.0f)
        );
        ~L3DCamera() {}

        const char* name() const { return m_name; }
        L3DVec3 position() const;

        void translate(const L3DVec3& movement);
        void rotate(
            float radians,
            const L3DVec3& direction = glm::vec3(0.0f, 1.0f, 0.0f)
        );
    };
}

#endif // L3D_L3DCAMERA_H
