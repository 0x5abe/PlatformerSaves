#pragma once
#include <Geode/Geode.hpp>

class PlayLevelMenuPopup : public geode::Popup<> {
protected:
	bool setup() override;
	void onClose(cocos2d::CCObject* i_sender) override;

public:
    static PlayLevelMenuPopup* create();
};
