#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/CheckpointObject.hpp>
#include <sabe.persistenceapi/include/PersistenceAPI.hpp>

class $modify(PSCheckpointObject, CheckpointObject) {
protected:
	friend void operator>>(persistenceAPI::InputStream& i_stream, PSCheckpointObject& o_value);
	friend void operator<<(persistenceAPI::OutputStream& o_stream, PSCheckpointObject& i_value);

public:
	struct Fields {
		bool m_wasLoaded = false;
		cocos2d::CCPoint m_position;
		double m_timePlayed;
		long long m_timestamp;
	};
	
	void load(persistenceAPI::InputStream& i_stream);
	void save(persistenceAPI::OutputStream& o_stream);

	void clean();

#if defined(PS_DEBUG) && defined(PS_DESCRIBE)
	void describe();
#endif
};