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

#ifndef L3D_LEAF3DUT_H
#define L3D_LEAF3DUT_H
#pragma once

#include "leaf3d/types.h"

using namespace l3d;

/* Init & terminate ***********************************************************/

L3D_API int l3dutInit(const char* rootPath = "Content/");

L3D_API int l3dutTerminate();

/* Debug helpers **************************************************************/

L3D_API void l3dutPrintVec2(const L3DVec2& vec);
L3D_API void l3dutPrintVec3(const L3DVec3& vec);
L3D_API void l3dutPrintVec4(const L3DVec4& vec);
L3D_API void l3dutPrintMat3(const L3DMat3& mat);
L3D_API void l3dutPrintMat4(const L3DMat4& mat);

L3D_API void l3dutPrintFrameStats(double currentTime, double& lastTime, int& fps);

/* Resource loading ***********************************************************/

L3D_API L3DHandle l3dutLoadTexture2D(
    const char* filename,
    const L3DImageFormat& desiredFormat = L3D_UNKNOWN
);

L3D_API L3DHandle l3dutLoadTextureCube(
    const char* filenameRight,
    const char* filenameLeft,
    const char* filenameTop,
    const char* filenameBottom,
    const char* filenameBack,
    const char* filenameFront,
    const L3DImageFormat& desiredFormat = L3D_UNKNOWN
);

L3D_API L3DHandle l3dutLoadShader(
    const L3DShaderType& type,
    const char* filename
);

L3D_API L3DHandle l3dutLoadShaderProgram(
    const char* vertexShaderFilename,
    const char* fragmentShaderFilename,
    const char* geometryShaderFilename = 0
);

L3D_API L3DHandle* l3dutLoadMeshes(
    const char* filename,
    const L3DHandle& shaderProgram,
    unsigned int* meshCount,
    unsigned int renderLayer = L3D_OPAQUE_MESH_RENDERLAYER
);

#endif // L3D_LEAF3DUT_H
