#include "EndLevelLayer.hpp"
#include "EndLevelLayer.hpp"
#include <hooks/PlayLayer.hpp>

using namespace geode::prelude;
using namespace persistenceAPI;

void PSEndLevelLayer::onMenu(CCObject* i_sender) {
	PSPlayLayer* l_playLayer = static_cast<PSPlayLayer*>(PlayLayer::get());
	if (l_playLayer && l_playLayer->m_isPlatformer && !l_playLayer->m_isPracticeMode && !l_playLayer->m_isTestMode && l_playLayer->validSaveExists()) {
		if (Mod::get()->getSettingValue<bool>("remove-save-on-complete")) { 
			l_playLayer->removeSaveFile();
		}
		else if (!Mod::get()->getSavedValue<bool>("has-seen-remove-save-on-complete-notice")) {
			Mod::get()->setSavedValue<bool>("has-seen-remove-save-on-complete-notice", true);
			createQuickPopup("Remove Save",
				"Do you want to <cr>remove</c> your save once you complete a level? This can be changed in the PlatformerSaves <cy>mod settings</c> page.",
				"Keep",
				"Remove",
				[&](FLAlertLayer*, bool i_btn2) {
					if (i_btn2) {
						Mod::get()->setSettingValue<bool>("remove-save-on-complete", true);
						static_cast<PSPlayLayer*>(PlayLayer::get())->removeSaveFile();
					}
					EndLevelLayer::onMenu(i_sender);
				}
			);
			return;
		}
	}
	EndLevelLayer::onMenu(i_sender);
}