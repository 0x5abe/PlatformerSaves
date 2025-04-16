#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <sabe.persistenceapi/include/PersistenceAPI.hpp>

extern size_t s_editLevelLayerDelegate1;
extern size_t s_editLevelLayerDelegate2;

class $modify(PSEditLevelLayer, EditLevelLayer) {
public:
    // overrides

    $override
    void onPlay(cocos2d::CCObject* i_sender);
};