#if !defined(GEODE_IS_IOS)
#include <Geode/Geode.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>
#include <sabe.persistenceapi/include/PersistenceAPI.hpp>

using namespace geode::prelude;
using namespace persistenceAPI;

void setupKeybinds();

$on_mod(Loaded) {
    setupKeybinds();
}

void setupKeybinds() {
    using namespace keybinds;

    BindManager::get()->registerBindable({
        "save-game"_spr,
        "Save game",
        "Hotkey for quick saving",
        { Keybind::create(KEY_K, Modifier::None) },
        Category::PLAY,
    });
}
#endif