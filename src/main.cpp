#include "extern/beatsaber-hook/shared/utils/logging.hpp"
#include "extern/beatsaber-hook/shared/utils/utils.h"

#include "GlobalNamespace/BeatmapData_-get_beatmapObjectsData-d__31.hpp"
#include "GlobalNamespace/BeatmapObjectData.hpp"
#include "GlobalNamespace/BeatmapData.hpp"
#include "GlobalNamespace/BeatmapLineData.hpp"
#include "System/Int32.hpp"

#include <limits>

using namespace GlobalNamespace;

static ModInfo modInfo;

const Logger& getLogger() {
    static const Logger& logger(modInfo);
    return logger;
}

extern "C" void setup(ModInfo &info) {
    info.id = "QuestCore";
    info.version = "0.1.0";
    modInfo = info;
}

MAKE_HOOK_OFFSETLESS(BeatmapObjectsDataClampPatch, bool, BeatmapData::$get_beatmapObjectsData$d__31 *self) {
    int state = self->$$1__state;
    BeatmapData *beatmapData = self->$$4__this;
    if (state != 0) {
        if (state != 1) {
            return false;
        }
        self->$$1__state = -1;
        // Increment index in idxs with clamped lineIndex
        int lineIndex = self->$minBeatmapObjectData$5__4->lineIndex;
        System::Int32 *idx = (System::Int32 *) self->$idxs$5__3->GetValue(lineIndex > 3 ? 3 : lineIndex < 0 ? 0 : lineIndex);
        idx->m_value++;
    } else {
        self->$$1__state = -1;
        self->$beatmapLinesData$5__2 = (Array<BeatmapLineData *> *) beatmapData->get_beatmapLinesData();
        self->$minBeatmapObjectData$5__4 == nullptr;
    }
    self->$minBeatmapObjectData$5__4 = nullptr;
    float num = std::numeric_limits<float>::max();
    for (int i = 0; i < self->$beatmapLinesData$5__2->Length(); i++) {
        System::Int32 *idx = (System::Int32 *) self->$idxs$5__3->GetValue(i);
        BeatmapLineData *lineData = (BeatmapLineData *) self->$beatmapLinesData$5__2->GetValue(i);
        if (idx->m_value < lineData->beatmapObjectsData->get_Count()) {
            BeatmapObjectData *beatmapObjectData = lineData->beatmapObjectsData->get_Item(idx->m_value);
            float time = beatmapObjectData->time;
            if (time < num) {
                num = time;
                self->$minBeatmapObjectData$5__4 = beatmapObjectData;
            }
        }
    }
    if (self->$minBeatmapObjectData$5__4 == nullptr) {
        return false;
    }
    self->$$2__current = self->$minBeatmapObjectData$5__4;
    self->$$1__state = 1;
    return true;
}

extern "C" void load() {
    getLogger().info("Installing QuestCore Hooks!");

    // Install hooks
    // https://github.com/Kylemc1413/SongCore/blob/master/HarmonyPatches/ClampPatches.cs#L98
    INSTALL_HOOK_OFFSETLESS(BeatmapObjectsDataClampPatch, 
        il2cpp_utils::FindMethodUnsafe("", "<get_beatmapObjectsData>d__31", "MoveNext", 0));

    getLogger().info("Installed QuestCore Hooks!");
}