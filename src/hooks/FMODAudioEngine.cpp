#include "FMODAudioEngine.hpp"
#include "Geode/binding/FMODAudioEngine.hpp"

using namespace geode::prelude;
using namespace persistenceAPI;

size_t s_FMODAudioEngineDelegate1 = 0;
size_t s_FMODAudioEngineDelegate2 = 0;

// overrides

void PSFMODAudioEngine::fadeOutMusic(float i_time, int i_channel) {
	if (m_fields->m_disableFadeOutMusic) {
		return;
	}
	FMODAudioEngine::fadeOutMusic(i_time, i_channel);
}

void PSFMODAudioEngine::loadMusic(gd::string i_path, float i_speed, float i_p2, float i_volume, bool i_shouldLoop, int i_p5, int i_p6, bool i_p7) {
	if (m_fields->m_disableLoadMusic) {
		return;
	}
	FMODAudioEngine::loadMusic(i_path, i_speed, i_p2, i_volume, i_shouldLoop, i_p5, i_p6, i_p7);
}

// custom methods

void PSFMODAudioEngine::backupMusic() {
	m_fields->m_backgroundMusicChannelBackup = m_backgroundMusicChannel;
	m_backgroundMusicChannel = nullptr;
	for (int i = 0; i < m_audioState.m_unkMapIntFloat1.size(); i++) {
		m_fields->m_unkMapIntFloat1Backup[i] = m_audioState.m_unkMapIntFloat1[i];
	}
	for (int i = 0; i < m_audioState.m_unkMapIntFloat2.size(); i++) {
		m_fields->m_unkMapIntFloat2Backup[i] = m_audioState.m_unkMapIntFloat2[i];
	}
	for (int i = 0; i < m_audioState.m_unkMapIntFloat3.size(); i++) {
		m_fields->m_unkMapIntFloat3Backup[i] = m_audioState.m_unkMapIntFloat3[i];
	}
}

void PSFMODAudioEngine::restoreMusic() {
	m_backgroundMusicChannel = m_fields->m_backgroundMusicChannelBackup;
	m_fields->m_backgroundMusicChannelBackup = nullptr;
	for (int i = 0; i < m_fields->m_unkMapIntFloat1Backup.size(); i++) {
		m_audioState.m_unkMapIntFloat1[i] = m_fields->m_unkMapIntFloat1Backup[i];
	}
	for (int i = 0; i < m_fields->m_unkMapIntFloat2Backup.size(); i++) {
		m_audioState.m_unkMapIntFloat2[i] = m_fields->m_unkMapIntFloat2Backup[i];
	}
	for (int i = 0; i < m_fields->m_unkMapIntFloat3Backup.size(); i++) {
		m_audioState.m_unkMapIntFloat3[i] = m_fields->m_unkMapIntFloat3Backup[i];
	}
	m_fields->m_unkMapIntFloat1Backup.clear();
	m_fields->m_unkMapIntFloat2Backup.clear();
	m_fields->m_unkMapIntFloat3Backup.clear();
}