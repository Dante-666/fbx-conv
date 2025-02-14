/*******************************************************************************
 * Copyright 2011 See AUTHORS file.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
/** @author Xoppa */
#ifdef _MSC_VER
#pragma once
#endif
#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

namespace fbxconv {

#define FILETYPE_AUTO 0x00
#define FILETYPE_FBX 0x10
#define FILETYPE_G3DB 0x20
#define FILETYPE_G3DJ 0x21
#define FILETYPE_OUT_DEFAULT FILETYPE_G3DB
#define FILETYPE_IN_DEFAULT FILETYPE_FBX
#define FILETYPE_C3B 0X30
#define FILETYPE_C3T 0X31
#define FILETYPE_ALL 0X32

enum COMPRESS_LEVEL {
    COMPRESS_LEVEL_DEFAULT,
    COMPRESS_LEVEL_1,
    COMPRESS_LEVEL_NUM
};

enum EXPORT_PART { EXPORT_PART_ALL, EXPORT_PART_MODEL, EXPORT_PART_ANIMATION };

struct Settings {
    std::string inFile;
    int inType;
    std::string outFile;
    int outType;
    /** Whether to flip the y-component of textures coordinates. */
    bool flipV;
    /** Whether to pack colors into one float. */
    bool packColors;
    bool verbose;
    /** The maximum allowed amount of bones in one nodepart, if a meshpart
     * exceeds this amount it will be split up in parts. */
    int maxNodePartBonesCount;
    /** The maximum allowed amount of bone weights in one vertex, if a vertex
     * exceeds this amount it will clipped on importance. */
    int maxVertexBonesCount;
    /** Always use maxVertexBoneCount for blendWeights, this might help merging
     * meshes. */
    bool forceMaxVertexBoneCount;
    /** The maximum allowed amount of vertices in one mesh, only used when
     * deciding to merge meshes. */
    unsigned int maxVertexCount;
    /** The maximum allowed amount of indices in one mesh, only used when
     * deciding to merge meshes. */
    unsigned int maxIndexCount;
    /** Whether to merge same attributes mesh. */
    bool needReusableMesh;
    /** average the same position vertex's normal*/
    bool normalizeVertexNormal;
    /** export normal map*/
    bool normalMap;
    /** The level of compression, you may lose some animation detail when useing
     * high compression level*/
    COMPRESS_LEVEL compressLevel;
    /** export specific part*/
    EXPORT_PART exportPart;
};

} // namespace fbxconv

#endif // SETTINGS_H
