#include "gm_bgm_se_manager.h"
#include "../dxlib_ext/dxlib_ext.h"
#include <fstream>
#include <sstream>



void BGM_SE_Mgr::setBaseVolume(enum sound_type st, int volume) {
	// ---- Sound全般の音量基準を設定 ---- //
	if (bgm == st) {
		if (volume < 0) { _base_bgm_volume = 0; return; }
		if (volume > 255) { _base_bgm_volume = 255; return; }
		_base_bgm_volume = volume;
	}
	if (se == st) {
		if (volume < 0) { _base_se_volume = 0; return; }
		if (volume > 255) { _base_se_volume = 255; return; }
		_base_se_volume = volume;
	}
}

void BGM_SE_Mgr::playSound(enum sound_type st, int id, std::string name, play_type pt, bool stop) {
	
	if (st == bgm) {
		for (auto bgm : _bgm_st) {
			if (bgm._id == id || bgm._name == name) {
				int volume = bgm._volume + _base_bgm_volume;
				if (volume < 0) { volume = 0; }
				if (volume > 255) { volume = 255; }
				ChangeVolumeSoundMem(volume, bgm._handler_id);
				switch (pt)
				{
				case BGM_SE_Mgr::one_shot:
					if (stop) {
						if (CheckSoundMem(bgm._handler_id) == 1) { break; }
					}
					PlaySoundMem(bgm._handler_id, DX_PLAYTYPE_BACK);
					break;
				case BGM_SE_Mgr::loop:
					PlaySoundMem(bgm._handler_id, DX_PLAYTYPE_LOOP);
					break;
				case BGM_SE_Mgr::stop:
					break;
				default:
					break;
				}
			}
		}
	}
	if (st == se) {
		for (auto se : _se_st) {
			if (se._id == id || se._name == name) {
				int volume = se._volume + _base_bgm_volume;
				if (volume < 0) { volume = 0; }
				if (volume > 255) { volume = 255; }
				ChangeVolumeSoundMem(volume, se._handler_id);
				switch (pt)
				{
				case BGM_SE_Mgr::one_shot:
					if (stop) {
						if (CheckSoundMem(se._handler_id) == 1) { break; }
					}
					PlaySoundMem(se._handler_id, DX_PLAYTYPE_BACK);
					break;
				case BGM_SE_Mgr::loop:
					PlaySoundMem(se._handler_id, DX_PLAYTYPE_LOOP);
					break;
				case BGM_SE_Mgr::stop:
					break;
				default:
					break;
				}
			}
		}
	}
}

void BGM_SE_Mgr::stopSoundAll() {
	for (auto bgm :_bgm_st) {
		if (CheckSoundMem(bgm._handler_id)) { StopSoundMem(bgm._handler_id); }
	}
	for (auto se : _se_st) {
		if (CheckSoundMem(se._handler_id)) { StopSoundMem(se._handler_id); }
	}
}

void BGM_SE_Mgr::setSoundsBGMInit(std::string bgm_path) {
	// ---- Sound BGM ファイルパスを_bgm_stへ登録 ---- //
	std::string str_buf;
	std::string str_conma_buf;
	static std::string str[100][4];
	int i = 0;
	int j = 0;

	// 1. BGMのCSVデータを開く 
	std::ifstream ifs1(bgm_path);
	if (!ifs1) {
		printf("error! File can't opened");
	}
	while (std::getline(ifs1, str_buf)) {
		std::string tmp = "";
		std::istringstream stream(str_buf);

		while (std::getline(stream, tmp, ','))
		{
			str[i][j] = tmp;
			j++;
		}
		j = 0;
		i++;
	}

	for (int i = 1; i < 100; i++) {		// ヘッダ1行は無視
		if (str[i][0] == "") { continue; }		// 無記入：無視
		std::string tmp_file_path = str[i][3] + str[i][1];
		int handle = LoadSoundMem(tmp_file_path.c_str());
		if (handle == -1) {
			printf("error, File can't opened : %s\n", tmp_file_path.c_str());
			continue;
		}
		// 通常処理
		sound_st tmp_s_st;
		tmp_s_st._name = str[i][1];
		tmp_s_st._st = bgm;
		tmp_s_st._id = stoi(str[i][0]);
		tmp_s_st._handler_id = handle;
		tmp_s_st._volume = stoi(str[i][2]);
		// option
		tmp_s_st._play_type = one_shot;
		tmp_s_st._is_play = false;
		// 配列に格納
		_bgm_st.push_back(tmp_s_st);
	}
}

void BGM_SE_Mgr::setSoundsSEInit(std::string se_path) {
	// ---- Sound SE ファイルパスを_se_stへ登録 ---- //
	std::string str_buf;
	std::string str_conma_buf;
	static std::string str[100][4];
	int i = 0;
	int j = 0;

	// 1. SEのCSVデータを開く 
	std::ifstream ifs1(se_path);
	if (!ifs1) {
		printf("error! File can't opened");
	}
	while (std::getline(ifs1, str_buf)) {
		std::string tmp = "";
		std::istringstream stream(str_buf);

		while (std::getline(stream, tmp, ','))
		{
			str[i][j] = tmp;
			j++;
		}
		j = 0;
		i++;
	}

	for (int i = 1; i < 100; i++) {		// ヘッダ1行は無視
		if (str[i][0] == "") { continue; }		// 無記入：無視
		std::string tmp_file_path = str[i][3] + str[i][1];
		int handle = LoadSoundMem(tmp_file_path.c_str());
		if (handle == -1) {
			printf("error, File can't opened : %s\n", tmp_file_path.c_str());
			continue;
		}
		// 通常処理
		sound_st tmp_s_st;
		tmp_s_st._name = str[i][1];
		tmp_s_st._st = bgm;
		tmp_s_st._id = stoi(str[i][0]);
		tmp_s_st._handler_id = handle;
		tmp_s_st._volume = stoi(str[i][2]);
		// option
		tmp_s_st._play_type = one_shot;
		tmp_s_st._is_play = false;
		// 配列に格納
		_se_st.push_back(tmp_s_st);
	}
}

