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
#endif //_MSC_VER
#ifndef FBXCONV_FBXCONV_H
#define FBXCONV_FBXCONV_H

#ifndef BUILD_NUMBER
#define BUILD_NUMBER "0000"
#endif

#ifndef BUILD_ID
#ifdef DEBUG
#define BUILD_ID "debug version, cocos2d-x-3.4-beta0 or later version can use"
#else
#define BUILD_ID "release version, cocos2d-x-3.4-beta0 or later version can use"
#endif
#endif

#define BIT_COUNT (sizeof(void *) * 8)

#include "FbxConvCommand.h"
#include "Settings.h"
#include "log/messages.h"
#include "modeldata/C3BFile.h"
#include "modeldata/Model.h"
#include "readers/FbxConverter.h"
#include "readers/Reader.h"
#include "json/JSONWriter.h"
#include "json/UBJSONWriter.h"
namespace fbxconv {

void simpleTextureCallback(
    std::map<std::string, readers::TextureFileInfo> &textures) {
    for (std::map<std::string, readers::TextureFileInfo>::iterator it =
             textures.begin();
         it != textures.end(); ++it) {
        // printf("Texture name: %s\nbounds: %01.2f, %01.2f, %01.2f,
        // %01.2f\ncount: %d\n", it->first.c_str(), it->second.bounds[0],
        // it->second.bounds[1], it->second.bounds[2], it->second.bounds[3],
        // it->second.nodeCount);
        it->second.path = it->first.substr(it->first.find_last_of("/\\") + 1);
    }
}

class FbxConv {
  public:
    fbxconv::log::Log *log;

    FbxConv(fbxconv::log::Log *log) : log(log) {
        log->info(log::iNameAndVersion, modeldata::VERSION_HI,
                  modeldata::VERSION_LO, /*BUILD_NUMBER,*/ BIT_COUNT, BUILD_ID);
    }

    const char *getVersionString() {
        return log->format(log::iVersion, modeldata::VERSION_HI,
                           modeldata::VERSION_LO, BUILD_NUMBER, BIT_COUNT,
                           BUILD_ID);
    }

    const char *getNameAndVersionString() {
        return log->format(log::iNameAndVersion, modeldata::VERSION_HI,
                           modeldata::VERSION_LO, /*BUILD_NUMBER,*/ BIT_COUNT,
                           BUILD_ID);
    }

    bool execute(int const &argc, const char **const &argv) {
        Settings settings;
        FbxConvCommand command(log, argc, argv, &settings);
#ifdef DEBUG
        log->filter |= log::Log::LOG_DEBUG;
#else
        log->filter &= ~log::Log::LOG_DEBUG;
#endif
        if (settings.verbose)
            log->filter |= log::Log::LOG_VERBOSE;
        else
            log->filter &= ~log::Log::LOG_VERBOSE;

        if (command.error != log::iNoError)
            command.printCommand();
        else if (!command.help)
            return execute(&settings);

        command.printHelp();
        return false;
    }

    bool execute(Settings *const &settings) {
        bool result = false;
        modeldata::Model *model = new modeldata::Model();
        if (load(settings, model)) {
            if (settings->verbose)
                info(model);
            if (save(settings, model))
                result = true;
        }
        delete model;
        return result;
    }

    readers::Reader *createReader(const Settings *const &settings) {
        return createReader(settings->inType);
    }

    readers::Reader *createReader(const int &type) {
        switch (type) {
        case FILETYPE_FBX:
            return new readers::FbxConverter(log, simpleTextureCallback);
        case FILETYPE_G3DB:
        case FILETYPE_G3DJ:
        default:
            log->error(log::eSourceLoadFiletypeUnknown);
            return 0;
        }
    }

    bool load(Settings *const &settings, modeldata::Model *model) {
        log->status(log::sSourceLoad);

        readers::Reader *reader = createReader(settings);
        if (!reader)
            return false;

        bool result = reader->load(settings);
        if (!result)
            log->error(log::eSourceLoadGeneral, settings->inFile.c_str());
        else {
            result = reader->convert(settings, model);
            log->status(log::sSourceConvert);
        }

        log->status(log::sSourceClose);

        delete reader;
        return result;
    }

    bool save(Settings *const &settings, modeldata::Model *model) {
        bool result = false;
        json::BaseJSONWriter *jsonWriter = 0;
        model->exportPart = settings->exportPart;
        if (settings->outType == FILETYPE_ALL ||
            settings->outType == FILETYPE_C3T) {
            std::string out = settings->outFile;
            int o = out.find_last_of(".");
            out = out.substr(0, o + 1) + "c3t";

            std::ofstream myfile;
            myfile.open(out.c_str(), std::ios::binary);

            log->status(log::sExportToG3DJ, out.c_str());
            jsonWriter = new json::JSONWriter(myfile);
            (*jsonWriter) << model;
            delete jsonWriter;
            result = true;
            myfile.close();
        }
        if (settings->outType == FILETYPE_ALL ||
            settings->outType == FILETYPE_C3B) {
            std::string out = settings->outFile;
            int o = out.find_last_of(".");
            out = out.substr(0, o + 1) + "c3b";
            C3BFile file;
            file.AddModel(model);
            file.saveBinary(out);
            log->status(log::sExportToG3DB, out.c_str());
        }

        log->status(log::sExportClose);
        return result;
    }

    void info(modeldata::Model *model) {
        if (!model)
            log->verbose(log::iModelInfoNull);
        else {
            log->verbose(log::iModelInfoStart);
            log->verbose(log::iModelInfoID, model->id.c_str());
            log->verbose(log::iModelInfoVersion, model->version[0],
                         model->version[1]);
            log->verbose(log::iModelInfoMeshesSummary, model->meshes.size(),
                         model->getTotalVertexCount(),
                         model->getMeshpartCount(),
                         model->getTotalIndexCount());
            log->verbose(log::iModelInfoNodesSummary, model->nodes.size(),
                         model->getTotalNodeCount(),
                         model->getTotalNodePartCount());
            log->verbose(log::iModelInfoMaterialsSummary,
                         model->materials.size(),
                         model->getTotalTextureCount());
        }
    }
};
} // namespace fbxconv

#endif // FBXCONV_FBXCONV_H
