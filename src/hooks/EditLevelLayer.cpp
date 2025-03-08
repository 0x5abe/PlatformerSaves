#include "EditLevelLayer.hpp"
#include "hooks/FMODAudioEngine.hpp"

using namespace geode::prelude;
using namespace persistenceAPI;

size_t s_editLevelLayerDelegate1 = 0;
size_t s_editLevelLayerDelegate2 = 0;

// overrides

void PSEditLevelLayer::onPlay(cocos2d::CCObject* i_sender) {
	// stupid way of not letting it unregister the keyboard delegates
	s_editLevelLayerDelegate1 = reinterpret_cast<size_t>(&m_bTouchEnabled)-3*sizeof(void*);
	s_editLevelLayerDelegate2 = reinterpret_cast<size_t>(&m_bTouchEnabled)-2*sizeof(void*);

	PSFMODAudioEngine* l_audioEngine = static_cast<PSFMODAudioEngine*>(FMODAudioEngine::get());
	l_audioEngine->m_fields->m_disableLoadMusic = true;
	l_audioEngine->backupMusic();

	EditLevelLayer::onPlay(i_sender);

	l_audioEngine->restoreMusic();
	// m_disableLoadMusic gets set to false before running PlayLayer::setupHasCompleted

	s_editLevelLayerDelegate1 = 0;
	s_editLevelLayerDelegate2 = 0;
}