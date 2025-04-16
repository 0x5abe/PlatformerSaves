#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

extern size_t s_GJBaseGameLayerDelegate1;
extern size_t s_GJBaseGameLayerDelegate2;

class $modify(PSGJBaseGameLayer, GJBaseGameLayer) {
public:
    // overrides

    $override
    bool processSongState(int i_p0, float i_p1, float i_p2, int i_p3, float i_p4, float i_p5, gd::vector<SongTriggerState>* i_p6, SongTriggerGameObject* i_songTriggerObject);
};