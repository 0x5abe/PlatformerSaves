#include "LevelInfoLayer.hpp"

using namespace geode::prelude;
using namespace persistenceAPI;

unsigned long long s_levelInfoLayerDelegate1 = 0;
unsigned long long s_levelInfoLayerDelegate2 = 0;

// overrides

void PSLevelInfoLayer::onPlay(cocos2d::CCObject* i_sender) {
	// stupid way of not letting it unregister the keyboard delegates
	s_levelInfoLayerDelegate1 = reinterpret_cast<unsigned long long>(&m_bTouchEnabled)-3*sizeof(void*);
	s_levelInfoLayerDelegate2 = reinterpret_cast<unsigned long long>(&m_bTouchEnabled)-2*sizeof(void*);

	LevelInfoLayer::onPlay(i_sender);

	s_levelInfoLayerDelegate1 = 0;
	s_levelInfoLayerDelegate2 = 0;
}