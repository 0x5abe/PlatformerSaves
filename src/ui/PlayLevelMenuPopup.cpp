#include "PlayLevelMenuPopup.hpp"
#include <hooks/PlayLayer.hpp>

using namespace geode::prelude;

bool PlayLevelMenuPopup::setup() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    // convenience function provided by Popup 
    // for adding/setting a title to the popup
    this->setTitle("Hi mom!");
    auto label = CCLabelBMFont::create("this a label", "bigFont.fnt");
    label->setPosition(winSize / 2);
    this->addChild(label);
    return true;
}

void PlayLevelMenuPopup::onClose(cocos2d::CCObject* i_sender) {
    Popup<>::onClose(i_sender);

	PSPlayLayer* l_playLayer = static_cast<PSPlayLayer*>(PlayLayer::get());
	if (l_playLayer && l_playLayer->m_fields->m_loadingState == LoadingState::WaitingForPlayLevelMenuPopup) {
		//l_playLayer->m_fields->m_loadingState = LoadingState::ReadF;
		l_playLayer->m_fields->m_saveSlot = 0;
	}
}


PlayLevelMenuPopup* PlayLevelMenuPopup::create() {
    auto ret = new PlayLevelMenuPopup();
    if (ret && ret->init(240.f, 160.f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}