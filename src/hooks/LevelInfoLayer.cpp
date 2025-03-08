#include "LevelInfoLayer.hpp"
#include "hooks/FMODAudioEngine.hpp"

using namespace geode::prelude;
using namespace persistenceAPI;

size_t s_levelInfoLayerDelegate1 = 0;
size_t s_levelInfoLayerDelegate2 = 0;

// overrides

void PSLevelInfoLayer::onPlay(cocos2d::CCObject* i_sender) {
	// stupid way of not letting it unregister the keyboard delegates
	s_levelInfoLayerDelegate1 = reinterpret_cast<size_t>(&m_bTouchEnabled)-3*sizeof(void*);
	s_levelInfoLayerDelegate2 = reinterpret_cast<size_t>(&m_bTouchEnabled)-2*sizeof(void*);

	PSFMODAudioEngine* l_audioEngine = static_cast<PSFMODAudioEngine*>(FMODAudioEngine::get());
	l_audioEngine->m_fields->m_disableLoadMusic = true;
	l_audioEngine->backupMusic();

	LevelInfoLayer::onPlay(i_sender);

	l_audioEngine->restoreMusic();
	// m_disableLoadMusic gets set to false before running PlayLayer::setupHasCompleted

	s_levelInfoLayerDelegate1 = 0;
	s_levelInfoLayerDelegate2 = 0;
}