#pragma once
#include "gm_parts.h"

class Module {
public:
	Module() {};
	virtual ~Module() {
		for (auto pts : _parts) delete pts;
		delete _parent;
		for (auto ch : _children) delete ch;
	}
	//// ------ メンバ変数 ------ ////
	// ----- Render ----- //
	std::vector<Parts*> _parts;			// パーツクラス
	bool _is_render = true;				// 描画判定

	// ----- Parameter ----- //
	int _id;							// ID: ツリー構造の検索等に用いる
	std::string _name;					// 名前：ツリー構造の検索当に用いる
	Module* _parent;					// 親モジュール: ツリー構造走査用
	std::vector<Module*> _children;		// 子モジュール：ツリー構造走査用
											
	// ----- 運動学計算用(DK) ----- //
	struct dk_st {
		// ---- 親から子モジュールへ運動学計算を実施するためのパラメータを構造体で定義しておく ---- //
		int _id;						// 子モジュールid
		std::string _name;				// 子モジュールname
		tnl::Vector3 _dir;				// 子モジュールの方向を単位ベクトルで格納(Σo)
		float _length;					// 子モジュールの距離をfloatで格納(Σo)
		tnl::Quaternion _rot_sum;		// 親モジュールからの回転量総量を格納する(Σo)
	};
	std::vector<dk_st> _dk_st;			// 子モジュールまでの相対座標を格納
	std::vector<dk_st> _dk_st_tmp;		// Update実行中の子モジュールまでの相対座標を格納：初期値(_dk_st)からの回転量で(_rot)で計算
	std::vector<dk_st> _dk_st_next;		//　子モジュールのΣoに対する座標を格納

	// ----- 座標系定義用 ----- //
	tnl::Vector3 _pos;					// モジュールiの位置(ワールド座標：Σo)
	tnl::Vector3 _rot_axis;				// モジュールi回転軸(Σo)
	tnl::Vector3 _rot_axis_tmp;			// Update実行中の回転軸ベクトル：初期値(_rot_axis)からの回転量(_rot)で計算：
										//  => 数値計算累計誤差の影響を避けるため
	tnl::Vector3 _dir_z;				// z軸単位ベクトル定義(Σo)
	tnl::Vector3 _dir_z_tmp;			// Update実行中のz軸単位ベクトル：初期値(_dir_z)からの回転量(_rot)で計算：
										//  => 数値計算累計誤差の影響を避けるため
	tnl::Vector3 _dir_x;				// x軸単位ベクトル定義(Σo)
	tnl::Vector3 _dir_x_tmp;			// Update実行中のz軸単位ベクトル：初期値(_dir_z)からの回転量(_rot)で計算：
										//  => 数値計算累計誤差の影響を避けるため
	tnl::Quaternion _rot;				// モジュールの回転量(Σo)：
	tnl::Quaternion _rot_tmp;			// モジュールiまでの回転量総量を一時格納(Σo ~ Σi-1)
	// ----- 逆運動学計算用 ----- //


	enum _ik_type {
		// ---- 制御モジュール - 制御目標モジュール同士をどの種類のIKで実施するか指定用 ---- //
		pos_to_pos,						// 位置同士を近づける
		dirz_look_pos,					// z軸を目標位置に向ける
		dirx_look_pos,					
		dirz_as_dirz,					// z軸を目標姿勢z軸と同じ向きに近づける
		dirx_as_dirx,
		dirz_as_dirx,
		dirx_as_dirz,
	};

	enum _attach_type {
		// ---- モジュールを親にアタッチする時、位置：絶対座標系参照 or 相対座標参照か
		absolute,
		relative,
	};

	//// ----- メンバ関数 ------ ////
	virtual void partsUpdate(float delta_time);
	virtual void partsRender(dxe::Camera* camera);
	void partsUpdateTree(const Module* mod, float delta_time);
	void partsRenderTree(const Module* mod, dxe::Camera* camera);
	static Module* createModule(int id, std::string name, tnl::Vector3 pos, tnl::Vector3 rot_axis,
		tnl::Quaternion rot = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0), 
		tnl::Vector3 dir_z = { 0, 0, 1 }, tnl::Vector3 dir_x = { 1, 0, 0 });
	void attachModule(Module* parent, Module* child, _attach_type type = absolute);
	void removeModuleTree(Module* mod, int id, std::string name, bool is_erase = false);
	void directKinematics(const std::vector<dk_st>& dk);
	void directKinematicsTree(const Module* mod, std::vector<dk_st>& dk);
	
};