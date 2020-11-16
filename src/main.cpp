#include "extern/beatsaber-hook/shared/utils/logging.hpp"

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

extern "C" void load() {
    getLogger().info("Installing QuestCore Hooks!");
    // Install hooks here
    getLogger().info("Installed QuestCore Hooks!");
}