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
		int _id;
		std::string _name;
		tnl::Vector3 _dir;
		float _length;
	};
	std::vector<dk_st> _dk_st;
	// ----- 座標系定義用 ----- //
	tnl::Vector3 _pos;					// モジュールiの位置(ワールド座標：Σo)
	tnl::Vector3 _rot_axis;				// モジュールi回転軸(Σo)
	tnl::Vector3 _dir_z;				// z軸単位ベクトル定義(Σo)
	tnl::Vector3 _dir_x;				// x軸単位ベクトル定義(Σo)
	tnl::Quaternion _rot;				// クォータニオン(Σo)

	// ----- 逆運動学計算用 ----- //


	enum _ik_type {
		// ---- 制御モジュール - 制御目標モジュール同士をどの種類のIKで実施するか指定用 ---- //
		pos_to_pos,						// 位置同士を近づける
		dirz_look_pos,					// z軸を目標位置に向ける
		dirx_look_pos,					
		dirz_as_dirz,					// z軸を目標姿勢z軸と同じ向きに近づける
		dirx_as_dirx,
		dirz_as_dirx,
		dirx_as_dirx,
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
	void attachModule(Module* parent, Module* child, _attach_type type);

};