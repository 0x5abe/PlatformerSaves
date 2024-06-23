#include "PauseLayer.hpp"
#include <hooks/PlayLayer.hpp>

using namespace geode::prelude;
using namespace persistenceAPI;

// overrides

void PSPauseLayer::customSetup() {
	PauseLayer::customSetup();

	PSPlayLayer* l_playLayer = static_cast<PSPlayLayer*>(PlayLayer::get());
	if (l_playLayer && l_playLayer->m_isPlatformer) {
		CCMenu* l_leftButtonMenu = static_cast<CCMenu*>(getChildByID("left-button-menu"));
		if (l_leftButtonMenu) {
			m_fields->m_saveCheckpointsSprite = CCSprite::create("saveButton.png"_spr);
			m_fields->m_saveCheckpointsSprite->setScale(0.35);
			m_fields->m_saveCheckpointsButton = CCMenuItemSpriteExtra::create(
				m_fields->m_saveCheckpointsSprite,
				this,
				menu_selector(PSPauseLayer::onSaveCheckpoints)
			);
			
			if (!l_playLayer->canSave()) {
				m_fields->m_saveCheckpointsSprite->setColor({127,127,127});
				m_fields->m_saveCheckpointsButton->m_bEnabled = false;
			}
			l_leftButtonMenu->addChild(m_fields->m_saveCheckpointsButton);

			l_leftButtonMenu->updateLayout();
		}
	}
}

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

void PSPauseLayer::onSaveCheckpoints(CCObject* i_sender) {
	PSPlayLayer* l_playLayer = static_cast<PSPlayLayer*>(PlayLayer::get());
	if (l_playLayer && l_playLayer->m_fields->m_savingState == SavingState::Ready) {
		if (l_playLayer && l_playLayer->startSaveGame()) {
			m_fields->m_saveCheckpointsSprite->setColor({127,127,127});
			m_fields->m_saveCheckpointsButton->m_bEnabled = false;
		}
	}
}