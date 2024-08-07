#include "FMODAudioEngine.hpp"
#include "Geode/binding/FMODAudioEngine.hpp"

using namespace geode::prelude;
using namespace persistenceAPI;

unsigned long long s_FMODAudioEngineDelegate1 = 0;
unsigned long long s_FMODAudioEngineDelegate2 = 0;

// overrides

void PSFMODAudioEngine::fadeOutMusic(float i_time, int i_channel) {
	if (m_fields->m_disableFadeOutMusic) {
		return;
	}
	FMODAudioEngine::fadeOutMusic(i_time, i_channel);
}

void PSFMODAudioEngine::loadMusic(gd::string i_path, float i_speed, float i_p2, float i_volume, bool i_shouldLoop, int i_p5, int i_p6) {
	if (m_fields->m_disableLoadMusic) {
		return;
	}
	FMODAudioEngine::loadMusic(i_path, i_speed, i_p2, i_volume, i_shouldLoop, i_p5, i_p6);
}

// custom methods

void PSFMODAudioEngine::backupMusic() {
	m_fields->m_backgroundMusicChannelBackup = m_backgroundMusicChannel;
	m_backgroundMusicChannel = nullptr;
	for (int i = 0; i < m_audioState.m_unkMapIntFloat7.size(); i++) {
		m_fields->m_unkMapIntFloat7Backup[i] = m_audioState.m_unkMapIntFloat7[i];
	}
	for (int i = 0; i < m_audioState.m_unkMapIntFloat8.size(); i++) {
		m_fields->m_unkMapIntFloat8Backup[i] = m_audioState.m_unkMapIntFloat8[i];
	}
	for (int i = 0; i < m_audioState.m_unkMapIntFloat9.size(); i++) {
		m_fields->m_unkMapIntFloat9Backup[i] = m_audioState.m_unkMapIntFloat9[i];
	}
}

void PSFMODAudioEngine::restoreMusic() {
	m_backgroundMusicChannel = m_fields->m_backgroundMusicChannelBackup;
	m_fields->m_backgroundMusicChannelBackup = nullptr;
	for (int i = 0; i < m_fields->m_unkMapIntFloat7Backup.size(); i++) {
		m_audioState.m_unkMapIntFloat7[i] = m_fields->m_unkMapIntFloat7Backup[i];
	}
	for (int i = 0; i < m_fields->m_unkMapIntFloat8Backup.size(); i++) {
		m_audioState.m_unkMapIntFloat8[i] = m_fields->m_unkMapIntFloat8Backup[i];
	}
	for (int i = 0; i < m_fields->m_unkMapIntFloat9Backup.size(); i++) {
		m_audioState.m_unkMapIntFloat9[i] = m_fields->m_unkMapIntFloat9Backup[i];
	}
	m_fields->m_unkMapIntFloat7Backup.clear();
	m_fields->m_unkMapIntFloat8Backup.clear();
	m_fields->m_unkMapIntFloat9Backup.clear();
}