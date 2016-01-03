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
#include <leaf3d/L3DShaderProgram.h>
#include <leaf3d/L3DMaterial.h>

using namespace l3d;

L3DMaterial::L3DMaterial(
    L3DRenderer* renderer,
    const char* name,
    L3DShaderProgram* shaderProgram,
    const L3DVec3& ambient,
    const L3DVec3& diffuse,
    const L3DVec3& specular,
    float shininess
) : L3DResource(L3D_MATERIAL, renderer),
    m_name(name),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular),
    shininess(shininess),
    m_shaderProgram(shaderProgram)
{
    if (renderer) renderer->addMaterial(this);
}

