#include "LevelAreaInnerLayer.hpp"
#include "hooks/FMODAudioEngine.hpp"

using namespace geode::prelude;
using namespace persistenceAPI;

// overrides

void PSLevelAreaInnerLayer::onDoor(cocos2d::CCObject* i_sender) {
	PSFMODAudioEngine* l_audioEngine = static_cast<PSFMODAudioEngine*>(FMODAudioEngine::get());
	l_audioEngine->m_fields->m_disableFadeOutMusic = true;
	l_audioEngine->m_fields->m_disableLoadMusic = true;

	LevelAreaInnerLayer::onDoor(i_sender);

	l_audioEngine->m_fields->m_disableFadeOutMusic = false;
	// m_disableLoadMusic gets set to false before running PlayLayer::setupHasCompleted or when cancelling level load
}