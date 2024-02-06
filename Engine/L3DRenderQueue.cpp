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
#include <leaf3d/L3DRenderQueue.h>

using namespace l3d;

L3DRenderQueue::L3DRenderQueue(
    L3DRenderer *renderer,
    const char *name) : L3DResource(L3D_RENDER_QUEUE, renderer),
                        m_name(name)
{
    if (renderer)
        renderer->addRenderQueue(this);
}

L3DRenderQueue::~L3DRenderQueue()
{
    for (L3DRenderCommandList::reverse_iterator it = m_commands.rbegin(); it != m_commands.rend(); ++it)
        delete *it;
    m_commands.clear();
}

void L3DRenderQueue::appendCommand(L3DRenderCommand *command)
{
    m_commands.push_back(command);
}

void L3DRenderQueue::appendCommands(const L3DRenderCommandList &commands)
{
    m_commands.reserve(commands.size());
    m_commands.insert(m_commands.end(), commands.begin(), commands.end());
}

void L3DRenderQueue::execute(L3DRenderer *renderer, L3DCamera *camera)
{
    for (L3DRenderCommandList::const_iterator it = m_commands.begin(); it != m_commands.end(); ++it)
        (*it)->execute(renderer, camera);
}
