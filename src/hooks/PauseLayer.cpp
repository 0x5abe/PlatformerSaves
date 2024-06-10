#include "PauseLayer.hpp"
#include <hooks/PlayLayer.hpp>

using namespace geode::prelude;
using namespace persistencyUtils;

// overrides

void PSPauseLayer::onEdit(CCObject* i_sender) {
	PSPlayLayer* l_playLayer = static_cast<PSPlayLayer*>(PlayLayer::get());
	if (l_playLayer && l_playLayer->m_fields->m_savingState != SavingState::Ready) {
		Notification::create("Cannot edit level while the game is being saved", NotificationIcon::Warning, 1.0f)->show();
		return;
	}

	PauseLayer::onEdit(i_sender);
}

void PSPauseLayer::tryQuit(CCObject* i_sender) {
	PSPlayLayer* l_playLayer = static_cast<PSPlayLayer*>(PlayLayer::get());
	if (l_playLayer && l_playLayer->m_fields->m_savingState != SavingState::Ready) {
		Notification::create("Cannot exit level while the game is being saved", NotificationIcon::Warning, 1.0f)->show();
		return;
	}

	PauseLayer::tryQuit(i_sender);
}