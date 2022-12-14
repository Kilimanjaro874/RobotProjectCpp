#pragma once
#include "gm_parts.h"

class Module {
public:
	Module() {};
	virtual ~Module() {
		for (auto pts : _parts) delete pts;
	}
	//// ------ メンバ変数 ------ ////
	// ----- Render ----- //
	std::vector<Parts*> _parts;			// パーツクラス
	bool _is_render = true;				// 描画判定
	bool _is_ik = true;					// IK実施判定

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
		bool _is_effect_ik = true;		// IKの影響を次モジュールに与えるかどうか
	};
	std::vector<dk_st> _dk_st;			// 子モジュールまでの相対座標を格納
	std::vector<dk_st> _dk_st_tmp;		// Update実行中の子モジュールまでの相対座標を格納：初期値(_dk_st)からの回転量で(_rot)で計算
	std::vector<dk_st> _dk_st_next;		//　子モジュールのΣoに対する座標を格納

	// ----- 逆運動学計算用(IK) ----- //
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

	struct ik_st {
		// ---- 本モジュールでIK実施のためのパラメータを構造体で定義。 ---- //
		int _id;
		std::string _name;
		int _type;						// IKの種類を格納
		float _kp;						// IK係数
		Module* _target;				// 制御目標モジュール
		Module* _object;				// 制御対象モジュール
	};
	std::vector<ik_st> _ik_st;			// IK構造体

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
	void attachModuleTree(int id, std::string name, Module* child, _attach_type type = absolute);
	void attachPartsTree(int id, std::string name,  Parts* parts);
	void attachIKstTree(std::vector<ik_st>& ik);
	Module* getModulePointerTree(Module* mod, int id, std::string name);
	void removeModuleTree(int id, std::string name, bool is_erase = false, _attach_type type = absolute);
	void directKinematics(const std::vector<dk_st>& dk);
	void directKinematicsAnkIK(const std::vector<dk_st>& dk, float delta_time);
	void directKinematicsTree(const Module* mod, std::vector<dk_st>& dk);
	void directKinematicsAndIKTree(const Module* mod, std::vector<dk_st>& dk, float delta_time);
	void Tranlate(tnl::Vector3& move, _attach_type type = relative);
	void TranlateTree(int id, std::string name, tnl::Vector3& move, _attach_type type = relative);
	tnl::Quaternion inverseKinematics(float delta_time);
	void setEffectIKTree(int id, std::string name, bool is_ik);
	void setAxisView(float size = 0.1, float length = 1.0);
	// --- ファイル操作系 --- //
	void exportForFileBinary(const std::string& file_path);
	void loadFromFileBinary(const std::string& file_path);
	// --- test --- //
	void debugShowState();
};