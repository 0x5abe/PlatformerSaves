#include "GJBaseGameLayer.hpp"
#include "hooks/PlayLayer.hpp"

using namespace geode::prelude;

// overrides

bool PSGJBaseGameLayer::processSongState(int i_p0, float i_p1, float i_p2, int i_p3, float i_p4, float i_p5, gd::vector<SongTriggerState>* i_p6, SongTriggerGameObject* i_songTriggerObject) {
    bool l_songTriggerDontReset = false;
    bool l_defaultSongDontReset = false;
    PSPlayLayer* l_playLayer = static_cast<PSPlayLayer*>(PlayLayer::get());

    if (l_playLayer &&
        reinterpret_cast<size_t>(l_playLayer) == reinterpret_cast<size_t>(this) &&
        !l_playLayer->m_fields->m_firstResetLevelCompleted
    ) {
        if (i_songTriggerObject && i_songTriggerObject->m_dontReset) {
            l_songTriggerDontReset = true;
            i_songTriggerObject->m_dontReset = false;
        } else if (m_levelSettings && m_levelSettings->m_dontReset) {
            l_defaultSongDontReset = true;
            m_levelSettings->m_dontReset = false;
        }
    }

    bool l_return = GJBaseGameLayer::processSongState(i_p0, i_p1, i_p2, i_p3, i_p4, i_p5, i_p6, i_songTriggerObject);

    if (l_songTriggerDontReset) {
        i_songTriggerObject->m_dontReset = true;
    } else if (l_defaultSongDontReset) {
        m_levelSettings->m_dontReset = true;
    }

    return l_return;
}