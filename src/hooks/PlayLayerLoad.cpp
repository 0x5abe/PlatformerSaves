#include "PlayLayer.hpp"
#include <filesystem>
#include <ui/PlayLevelMenuPopup.hpp>
#include <util/algorithm.hpp>
#include <util/filesystem.hpp>
#include <util/platform.hpp>

using namespace geode::prelude;
using namespace persistenceAPI;
using namespace util::platform;

bool PSPlayLayer::readPSFVersionAndUpdateIfNecessary() {
	std::string l_psfMagicAndVer(sizeof(s_psfMagicAndVer), ' ');

	m_fields->m_stream.read(l_psfMagicAndVer.data(), sizeof(s_psfMagicAndVer));
	l_psfMagicAndVer = l_psfMagicAndVer.substr(5, 5);
	l_psfMagicAndVer.erase(std::remove(l_psfMagicAndVer.begin(), l_psfMagicAndVer.end(), '.'), l_psfMagicAndVer.end());
	m_fields->m_readPSFVersion = std::stoi(l_psfMagicAndVer);
	//log::info("[readPSFVersionAndUpdateIfNecessary] Read PSF Version: {}", m_fields->m_readPSFVersion);
	if (s_psfVersion != m_fields->m_readPSFVersion) {
		if (!makeBackup()) {
			return false;
		}
		bool l_updateSucceded = updatePSFFormat();
		if (l_updateSucceded) {
			m_fields->m_originalPSFVersion = m_fields->m_readPSFVersion;
		}
		return l_updateSucceded;
	}
	m_fields->m_stream.setPAVersion(2);
	return true;
}

bool PSPlayLayer::readPSFFinishedSaving() {
	bool l_finishedSaving = false;
	m_fields->m_stream.read(reinterpret_cast<char*>(&l_finishedSaving), sizeof(bool));

	return l_finishedSaving;
}

void PSPlayLayer::readPSFData() {
	PSFData l_psfData;
	l_psfData.data = 0;
	m_fields->m_stream.read(reinterpret_cast<char*>(&l_psfData.data), sizeof(l_psfData.data));
	// do this check because if file was updated during this load process we would overwrite it with 0
	if (l_psfData.m_originalVersion != 0) {
		m_fields->m_originalPSFVersion = l_psfData.m_originalVersion;
	}
	m_fields->m_updatedFromPreviousLevelVersion = l_psfData.m_updatedFromPreviousLevelVersion;
	m_fields->m_readPlatform = static_cast<PSPlatform>(l_psfData.m_platform);
	//log::info("[readPSFData] l_psfData.m_originalVersion: {}", static_cast<unsigned int>(l_psfData.m_originalVersion));
	//log::info("[readPSFData] l_psfData.m_updatedFromPreviousLevelVersion: {}", static_cast<bool>(l_psfData.m_originalVersion));
	//log::info("[readPSFData] l_psfData.m_readPlatform: {}", static_cast<unsigned int>(l_psfData.m_platform));
	//log::info("[readPSFData] m_fields->m_originalPSFVersion: {}", m_fields->m_originalPSFVersion);
	//log::info("[readPSFData] m_fields->m_updatedFromPreviousLevelVersion: {}", m_fields->m_updatedFromPreviousLevelVersion);
	//log::info("[readPSFData] m_fields->m_readPlatform: {}", static_cast<unsigned int>(m_fields->m_readPlatform));
}

bool PSPlayLayer::readLowDetailMode() {
	bool l_lowDetailMode = false;
	m_fields->m_stream.read(reinterpret_cast<char*>(&l_lowDetailMode), sizeof(bool));

	// unused bytes
	m_fields->m_stream.ignore(16-(sizeof(s_psfMagicAndVer)+sizeof(bool)+sizeof(PSFData)+sizeof(bool)));

	return l_lowDetailMode;
}

bool PSPlayLayer::readPSFLevelStringHash() {
	unsigned int l_savedLevelStringHash;

	m_fields->m_stream >> l_savedLevelStringHash;
	
	if (l_savedLevelStringHash != util::algorithm::hash_string(m_level->m_levelString.c_str())) {
		//log::info("[readPSFLevelStringHash] different levelstring hash");
		return false;
	}
	//log::info("[readPSFLevelStringHash] same levelstring hash");
	return true;
}

void PSPlayLayer::loadGame() {
	switch (m_fields->m_loadingState) {
		case LoadingState::Setup: {
			m_fields->m_loadingProgress = 0.0f;

			PlayLevelMenuPopup* l_playLevelMenuPopup = PlayLevelMenuPopup::create();
			l_playLevelMenuPopup->setID("play-level-menu-popup"_spr);
			l_playLevelMenuPopup->show();
			m_fields->m_loadingState = LoadingState::WaitingForPlayLevelMenuPopup;
			// falls through
		}
		case LoadingState::WaitingForPlayLevelMenuPopup: {
			// TODO: fix this so it's not ugly
			if (m_fields->m_saveSlot == -1) {
				break;
			}
			else if (m_fields->m_saveSlot == -2) {
				m_fields->m_saveSlot = 0;
				m_fields->m_loadingState = LoadingState::CancelLevelLoad;
				break;
			}
			else if (m_fields->m_saveSlot == -3) {
				m_fields->m_saveSlot = 0;
				m_fields->m_loadingState = LoadingState::Ready;
				break;
			}
			else {
				m_fields->m_loadingState = LoadingState::SetupFileRead;
			}
			// falls through
		}
		case LoadingState::SetupFileRead: {
			std::string l_filePath = getSaveFilePath(-1, true);
			if (l_filePath == "") {
				m_fields->m_loadingState = LoadingState::Ready;
				m_fields->m_loadingProgress = 0.99f;
				break;
			}

			LevelInfoLayer* l_levelInfoLayer = static_cast<LevelInfoLayer*>(CCScene::get()->getChildByID("LevelInfoLayer"));
			if (l_levelInfoLayer) {
				l_levelInfoLayer->m_progressTimer->setColor(ccColor3B(220, 32, 64));
			}

			m_fields->m_bytesToRead = std::filesystem::file_size(l_filePath);
			m_fields->m_bytesRead = 0;
			if(m_fields->m_bytesToRead == 0 || !m_fields->m_stream.setFile(l_filePath, &m_fields->m_bytesRead, 2)) {
				m_fields->m_loadingState = LoadingState::HandleFileError;
				break;
			}

			m_fields->m_loadingState = LoadingState::ReadVersion;
			// falls through
		}
		case LoadingState::ReadVersion: {
			if (!readPSFVersionAndUpdateIfNecessary()) {
				m_fields->m_loadingState = LoadingState::HandleIncorrectVersion;
				break;
			}
			m_fields->m_loadingState = LoadingState::ReadFinishedSaving;
			// falls through
		}
		case LoadingState::ReadFinishedSaving: {
			if (!readPSFFinishedSaving()) {
				m_fields->m_loadingState = LoadingState::HandleDidNotFinishSaving;
				break;
			}
			m_fields->m_loadingState = LoadingState::ReadPSFData;
			// falls through
		}
		case LoadingState::ReadPSFData: {
			readPSFData();
			if ((m_fields->m_readPlatform != m_fields->m_platform) && m_fields->m_readPSFVersion < 10) {
				m_fields->m_loadingState = LoadingState::ShowPlatformError;
				break;
			} else if (m_fields->m_readPlatform != m_fields->m_platform) {
				m_fields->m_loadingState = LoadingState::ShowPlatformWarning;
				break;
			}
			if (m_fields->m_updatedFromPreviousLevelVersion && Mod::get()->getSettingValue<bool>("level-version-warning")) {
				m_fields->m_loadingState = LoadingState::ShowLevelVersionWarning;
				break;
			}
			if (m_fields->m_originalPSFVersion != 0 && m_fields->m_originalPSFVersion != m_fields->m_readPSFVersion && Mod::get()->getSettingValue<bool>("psf-version-warning")) {
				m_fields->m_loadingState = LoadingState::ShowPSFVersionWarning;
				break;
			}
			m_fields->m_loadingState = LoadingState::ReadLowDetailMode;
			// falls through
		}
		case LoadingState::ReadLowDetailMode: {
			bool l_lowDetailMode = readLowDetailMode();
			if (m_level->m_lowDetailMode && l_lowDetailMode != m_level->m_lowDetailModeToggled) {
				m_fields->m_loadingState = LoadingState::HandleIncorrectLowDetailMode;
				break;
			}
			m_fields->m_loadingState = LoadingState::ReadHash;
			// falls through
		}
		case LoadingState::ReadHash: {
			if (!readPSFLevelStringHash()) {
				m_fields->m_loadingState = LoadingState::HandleIncorrectHash;
				break;
			}
			m_fields->m_loadingState = LoadingState::ReadCheckpointCount;
			// falls through
		}
		case LoadingState::ReadCheckpointCount: {
			m_fields->m_stream >> m_fields->m_remainingCheckpointLoadCount;
			m_fields->m_loadingState = LoadingState::ReadCheckpoint;
			// falls through
		}
		case LoadingState::ReadCheckpoint: {
			if (m_fields->m_remainingCheckpointLoadCount > 0) {
				loadCheckpointFromStream();
				m_fields->m_remainingCheckpointLoadCount--;
			}
			if (m_fields->m_remainingCheckpointLoadCount == 0) {
				m_fields->m_loadingState = LoadingState::ReadActivatedCheckpoints;
			}
			// falls through
		}
		case LoadingState::ReadActivatedCheckpoints: {
			loadActivatedCheckpointsFromStream();
			m_fields->m_loadingState = LoadingState::ReadExtraData;
			// falls through
		}
		case LoadingState::ReadExtraData: {
			m_fields->m_stream >> m_effectManager->m_persistentItemCountMap;

			m_fields->m_stream >> m_fields->m_loadedPersistentTimerItemSet;

			m_fields->m_stream >> m_fields->m_loadedAttempts;
			
			m_fields->m_updateExtraData = true;
			m_fields->m_loadingState = LoadingState::Ready;
			// falls through
		}
		case LoadingState::Ready: {
			endStream();
			if (m_fields->m_normalModeCheckpoints->count() > 0) {
				m_fields->m_lastSavedCheckpointTimestamp = static_cast<PSCheckpointObject*>(m_fields->m_normalModeCheckpoints->lastObject())->m_fields->m_timestamp;
			}
			registerCheckpointsAndActivatedCheckpoints();
			break;
		}
		case LoadingState::HandleFileError: {
			hideAndLockCursor(false);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			createQuickPopup("Error loading game",
				"The save file for this level <cr>could not be opened</c>.",
				"Ok",
				nullptr,
				[&](FLAlertLayer*, bool i_btn2) {
					m_fields->m_loadingState = LoadingState::CancelLevelLoad;
					hideAndLockCursor(true);
				}
			);
			break;
		}
		case LoadingState::HandleIncorrectVersion: {
			hideAndLockCursor(false);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			createQuickPopup("Error loading game",
				"Updating the save file <cr>failed</c>.",
				"Ok",
				nullptr,
				[&](FLAlertLayer*, bool i_btn2) {
					m_fields->m_loadingState = LoadingState::CancelLevelLoad;
					hideAndLockCursor(true);
				}
			);
			break;
		}
		case LoadingState::HandleDidNotFinishSaving: {
			hideAndLockCursor(false);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			createQuickPopup("Error loading game",
				"The save file for this level appears to be <cr>corrupted</c>.",
				"Ok",
				nullptr,
				[&](FLAlertLayer*, bool i_btn2) {
					m_fields->m_loadingState = LoadingState::CancelLevelLoad;
					hideAndLockCursor(true);
				}
			);
			break;
		}
		case LoadingState::ShowPlatformError: {
			hideAndLockCursor(false);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			createQuickPopup("Error loading game",
				"This save file version is <cr>not supported</c> on this platform.",
				"Ok",
				nullptr,
				[&](FLAlertLayer*, bool i_btn2) {
					m_fields->m_loadingState = LoadingState::CancelLevelLoad;
					hideAndLockCursor(true);
				}
			);
			break;
		}
		case LoadingState::ShowPlatformWarning: {
			hideAndLockCursor(false);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			createQuickPopup("Warning",
				"This save file is from a different platform. <cy>Load it anyways</c>? (<cr>this might be unstable or crash the game</c>).",
				"Cancel",
				"Ok",
				[&](FLAlertLayer*, bool i_btn2) {
					if (i_btn2) {
						if (m_fields->m_updatedFromPreviousLevelVersion && Mod::get()->getSettingValue<bool>("level-version-warning")) {
							m_fields->m_loadingState = LoadingState::ShowLevelVersionWarning;
							return;
						}
						if (m_fields->m_originalPSFVersion != 0 && m_fields->m_originalPSFVersion != m_fields->m_readPSFVersion && Mod::get()->getSettingValue<bool>("psf-version-warning")) {
							m_fields->m_loadingState = LoadingState::ShowPSFVersionWarning;
							return;
						}
						m_fields->m_loadingState = LoadingState::ReadLowDetailMode;
					} else {
						m_fields->m_loadingState = LoadingState::CancelLevelLoad;
					}
					hideAndLockCursor(true);
				}
			);
			break;
		}
		case LoadingState::ShowLevelVersionWarning: {
			hideAndLockCursor(false);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			createQuickPopup("Warning",
				"The level in the save file was updated from a previous version of the level, <cr>this might be unstable or crash the game</c>. This warning can be disabled in the PlatformerSaves <cy>mod settings</c> page.",
				"Ok",
				nullptr,
				[&](FLAlertLayer*, bool i_btn2) {
					if (m_fields->m_originalPSFVersion != 0 && m_fields->m_originalPSFVersion != m_fields->m_readPSFVersion && Mod::get()->getSettingValue<bool>("psf-version-warning")) {
						m_fields->m_loadingState = LoadingState::ShowPSFVersionWarning;
						return;
					}
					m_fields->m_loadingState = LoadingState::ReadLowDetailMode;
					hideAndLockCursor(true);
				}
			);
			break;
		}
		case LoadingState::ShowPSFVersionWarning: {
			hideAndLockCursor(false);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			createQuickPopup("Warning",
				"This save file was updated from a previous version of PlatformerSaves, <cr>this might be unstable or crash the game</c>. This warning can be disabled in the PlatformerSaves <cy>mod settings</c> page.",
				"Ok",
				nullptr,
				[&](FLAlertLayer*, bool i_btn2) {
					m_fields->m_loadingState = LoadingState::ReadLowDetailMode;
					hideAndLockCursor(true);
				}
			);
			break;
		}
		case LoadingState::HandleIncorrectLowDetailMode: {
			std::string l_message;
			if (m_level->m_lowDetailModeToggled) {
				l_message = "This save file was created with low detail mode <cr>disabled</c>, but it is currently <cg>enabled</c>. If you want to load it you should <cr>disable</c> low detail mode in the level settings.";
			} else {
				l_message = "This save file was created with low detail mode <cg>enabled</c>, but it is currently <cr>disabled</c>. If you want to load it you should <cg>enable</c> low detail mode in the level settings.";
			}
			hideAndLockCursor(false);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			createQuickPopup("Error loading game",
				l_message,
				"Ok",
				nullptr,
				[&](FLAlertLayer*, bool i_btn2) {
					m_fields->m_loadingState = LoadingState::CancelLevelLoad;
					hideAndLockCursor(true);
				}
			);
			break;
		}
		case LoadingState::HandleIncorrectHash: {
			hideAndLockCursor(false);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			if (m_level->m_levelType == GJLevelType::Editor) {
				createQuickPopup("Error loading game",
					"The version of the level in the save file does not match the current one. <cy>A new game will be started</c>.",
					"Cancel",
					"Ok",
					[&](FLAlertLayer*, bool i_btn2) {
						if (i_btn2) {
							removeSaveFile();
							m_fields->m_loadingState = LoadingState::Ready;
						} else {
							m_fields->m_loadingState = LoadingState::CancelLevelLoad;
						}
						hideAndLockCursor(true);
					}
				);
			} else {
				createQuickPopup("Error loading game",
					"The version of the level in the save file does not match the current one. <cy>Try to load it anyways</c>? (<cr>this might be unstable or crash the game</c>).",
					"Cancel",
					"Ok",
					[&](FLAlertLayer*, bool i_btn2) {
						if (i_btn2) {
							m_fields->m_loadingState = LoadingState::ReadCheckpointCount;
							m_fields->m_updatedFromPreviousLevelVersion = true;
						} else {
							m_fields->m_loadingState = LoadingState::CancelLevelLoad;
						}
						hideAndLockCursor(true);
					}
				);
			}
			break;
		}
		case LoadingState::WaitingForPopup: {
			break;
		}
		case LoadingState::CancelLevelLoad: {
			m_fields->m_cancelLevelLoad = true;
			LevelInfoLayer* l_levelInfoLayer = static_cast<LevelInfoLayer*>(CCScene::get()->getChildByID("LevelInfoLayer"));
			if (l_levelInfoLayer) {
				l_levelInfoLayer->m_progressTimer->setVisible(false);
			}
			m_fields->m_loadingState = LoadingState::Ready;
			endStream();
			break;
		}
	}
}

void PSPlayLayer::loadActivatedCheckpointsFromStream() {
	unsigned int l_size;
	m_fields->m_stream.read(reinterpret_cast<char*>(&l_size), 4);
	if (l_size != 0) {
		m_fields->m_activatedCheckpoints.resize(l_size);
		for (int i = 0; i < l_size; i++) {
			m_fields->m_activatedCheckpoints[i].load(m_fields->m_stream);
		}
	}
}

void PSPlayLayer::loadCheckpointFromStream() {
	PSCheckpointObject* l_checkpoint = reinterpret_cast<PSCheckpointObject*>(CheckpointObject::create());
	l_checkpoint->load(m_fields->m_stream); 

	GameObject* l_newPhysicalCPO = GameObject::createWithFrame("square_01_001.png");
	CC_SAFE_RETAIN(l_newPhysicalCPO);
	l_newPhysicalCPO->m_objectID = 0x2c;
	l_newPhysicalCPO->m_objectType = GameObjectType::Decoration;
	l_newPhysicalCPO->m_glowSprite = nullptr;
	l_newPhysicalCPO->m_isDisabled2 = true; // who knows
	l_newPhysicalCPO->m_isInvisible = true; // who knows
	l_newPhysicalCPO->setOpacity(0);

	// This never seemed to matter
	//int* l_unkField1 = reinterpret_cast<int*>(reinterpret_cast<size_t>(l_newPhysicalCPO)+0x3d4);
	//*l_unkField1 = 3;

	CC_SAFE_RELEASE(l_checkpoint->m_physicalCheckpointObject);
	l_checkpoint->m_physicalCheckpointObject = l_newPhysicalCPO;
	
	l_checkpoint->m_physicalCheckpointObject->setStartPos(l_checkpoint->m_fields->m_position);

#if defined(PS_DEBUG) && defined(PS_DESCRIBE)
	l_checkpoint->describe();
#endif

	m_fields->m_normalModeCheckpoints->addObject(l_checkpoint);
}

void PSPlayLayer::updateAsyncProcessCreateObjectsFromSetup() {
	cocos2d::SEL_CallFunc l_sel = callfunc_selector(PSPlayLayer::updateAsyncProcessCreateObjectsFromSetup);
	PSPlayLayer::processCreateObjectsFromSetup();
	if (m_loadingProgress >= 1.0) {
		l_sel = callfunc_selector(PSPlayLayer::endAsyncProcessCreateObjectsFromSetup);
	}
	 
	CCScene* l_currentScene = CCScene::get();
	if (l_currentScene) {
		l_currentScene->runAction(
			CCSequence::create(
				CCDelayTime::create(0.0f),
				CCCallFunc::create(
					this,
					l_sel
				),
				nullptr
			)
		);
	}
}

void PSPlayLayer::endAsyncProcessCreateObjectsFromSetup() {
	if (m_fields->m_transitionFadeScene) {
		CCDirector::get()->replaceScene(m_fields->m_transitionFadeScene);
		CC_SAFE_RELEASE(m_fields->m_transitionFadeScene);
		m_fields->m_transitionFadeScene = nullptr;
	}
}