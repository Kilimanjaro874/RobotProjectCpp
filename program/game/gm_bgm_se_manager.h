#pragma once
#include <string>
#include <vector>

class BGM_SE_Mgr {
public:
	BGM_SE_Mgr() {};
	~BGM_SE_Mgr() {};
	enum play_type {
		one_shot, loop, stop
	};
	enum sound_type {
		bgm, se
	};
private:
	int _base_bgm_volume;
	int _base_se_volume;
	
	struct sound_st {
		std::string _name;
		enum sound_type _st;
		int _id;
		int _handler_id;
		int _volume;
		// option
		int _play_type;
		bool _is_play;
	};
	std::vector<sound_st> _bgm_st;
	std::vector<sound_st> _se_st;

public:
	void setSoundsBGMInit(std::string bgm_path);
	void setSoundsSEInit(std::string se_path);
	void setBaseVolume(enum sound_type, int volume);
	void playSound(enum sound_type st, int id, std::string name, play_type pt);
	void stopSound(enum sound_type st, int id, std::string name);
	void setVolume(enum sound_type st, int id, std::string name);
	void serchSoundSt(enum sound_type st, int id, std::string name);
	void stopSoundAll();

};