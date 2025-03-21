#pragma once
#include "Geode/modify/Modify.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/FMODAudioEngine.hpp>
#include <sabe.persistenceapi/include/PersistenceAPI.hpp>

class $modify(PSFMODAudioEngine, FMODAudioEngine) {
public:
	struct Fields {
		bool m_disableFadeOutMusic;
		bool m_disableLoadMusic;
		FMOD::ChannelGroup* m_backgroundMusicChannelBackup;
		gd::unordered_map<int,float> m_unkMapIntFloat1Backup;
		gd::unordered_map<int,float> m_unkMapIntFloat2Backup;
		gd::unordered_map<int,float> m_unkMapIntFloat3Backup;
	};

	// overrides

	$override
	void fadeOutMusic(float i_time, int i_channel);

	$override
	void loadMusic(gd::string i_path, float i_speed, float i_p2, float i_volume, bool i_shouldLoop, int i_p5, int i_p6, bool i_p7);

	// custom methods

	void backupMusic();

	void restoreMusic();
};