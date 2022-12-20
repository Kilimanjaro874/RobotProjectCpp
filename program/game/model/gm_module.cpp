#include "gm_module.h"

void Module::partsUpdate(float delta_time) {
	// ---- 部品の位置・姿勢をモジュール座標系に応じて更新 ---- //

	if (_id == 930) {
		printf("deb");
	}

	for (auto pts : _parts) {
		pts->mesh_->pos_ = _pos + tnl::Vector3::TransformCoord(pts->ofs_pos_, _rot);
		pts->mesh_->rot_q_ = pts->ofs_rot_ * _rot;
	}
}
/// <summary>
///  ----- 部品のシェーダーを描画する ----- //
/// 
/// </summary>
/// <param name="camera"></param>
void Module::partsRender(dxe::Camera* camera) {
	// ----- 部品のシェーダーを描画する ----- //

	if (_id == 930) {
		printf("deb");
	}

	if (!_is_render) { return; }
	for (auto pts : _parts) {
		if (!pts->is_render_) continue;
		pts->mesh_->render(camera);
	}
}

void Module::partsUpdateTree(const Module* mod, float delta_time) {
	// ---- partsUpdateを子モジュールに再帰的に実施 : preorder ---- //
	partsUpdate(delta_time);
	if (_children.size() == 0) return;		// 子が無い：親側へ戻る
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->partsUpdateTree(_children[i], delta_time);	// 子の関数実施
	}
}

void Module::partsRenderTree(const Module* mod, dxe::Camera* camera) {
	// ---- partsRenderを子モジュールに再帰的に実施：preorder ---- //
	partsRender(camera);
	if (_children.size() == 0) return;		// 子がない：親側へ戻る
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->partsRenderTree(_children[i], camera);		// 子の関数実施
	}
}

Module* Module::createModule(int id, std::string name, 
	tnl::Vector3 pos, tnl::Vector3 rot_axis,
	tnl::Quaternion rot , tnl::Vector3 dir_z, tnl::Vector3 dir_x) {
	// ----- モジュールを生成する ----- //
	Module* mod = new Module();
	mod->_id = id;
	mod->_name = name;
	mod->_pos = pos;
	mod->_rot_axis = rot_axis;
	mod->_rot = rot;
	mod->_dir_z = dir_z;
	mod->_dir_x = dir_x;

	return mod;
}

void Module::attachModule(Module* parent, Module* child, _attach_type type) {
	// ----- モジュールツリー構造構築のため、親子関係設定 ----- //
	parent->_children.push_back(child);
	child->_parent = parent;
	// ---- 偏差ベクトルを親に格納 ---- //
	tnl::Vector3 delta_pos;
	if (_attach_type::absolute == type) { delta_pos = child->_pos - parent->_pos; }
	else if (_attach_type::relative == type)	{ delta_pos = child->_pos; }
	float delta_length = delta_pos.length();
	delta_pos.normalize();
	// ---- _dk_st構造体に格納 ---- //
	parent->_dk_st.push_back({ child->_id, child->_name, 
		delta_pos, delta_length, tnl::Quaternion::RotationAxis({0, 1, 0}, 0) });
	// ---- 他の_dk_st構造体にコピー ---- //
	_dk_st_tmp = _dk_st;
	_dk_st_next = _dk_st;
}

void Module::attachModuleTree(int id, std::string name, Module* child, _attach_type type) {
	// ----- モジュールツリー構造の、特定の親(id, 名前参照)に子モジュールを再帰的にアタッチ：preorder ---- //
	if (this->_id == id || this->_name == name) {
		attachModule(this, child, type);
		return;
	}
	if (this->_children.size() == 0) { return; }
	for (int i = 0; i < this->_children.size(); i++) {
		this->_children[i]->attachModuleTree(id, name, child, type);
	}
}

void Module::attachPartsTree(int id, std::string name, Parts* parts) {
	// ----- モジュールにパーツをアタッチ：再帰処理：preorder ----- //
	if (_id == id || _name == name) {
		_parts.push_back(parts);
		return;
	}
	if (_children.size() == 0) { return; }
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->attachPartsTree(id, name, parts);	// 子の関数実施
	}
}

void Module::attachIKstTree(std::vector<ik_st>& ik) {
	// ----- モジュールにIK設定をアッタッチ：再帰処理：preorder ----- //
	for (int i = 0; i < ik.size(); i++) {
		// vector型にある該当するik_stは全て取得する : 全探査：return 無し
		if (_id == ik[i]._id || _name == ik[i]._name) {
			_ik_st.push_back(ik[i]);
		}
	}
	if (_children.size() == 0) { return; }
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->attachIKstTree(ik);
	}
}

Module* Module::getModulePointerTree(Module* mod, int id, std::string name) {
	// ----- あるid, nameに応じたモジュールを木構造から再帰的に取得：preorder ----- //
	if (this->_id == id || this->_name == name) {
		mod = this;	// 再帰終了
		return mod;
	}
	for (int i = 0; i < _children.size(); i++) {
		mod = _children[i]->getModulePointerTree(mod, id, name);
		if (mod == nullptr) { continue; }
		if (mod->_id == id || mod->_name == name) { return mod; }	// 再帰終了
	}
	return nullptr;

}

void Module::removeModuleTree(int id, std::string name, bool is_erase, _attach_type at) {
	// ----- 本モジュールより子にある特定のid、「又は」名前のモジュールを運動学計算のメンバから外す ----- //
	// また、is_erase == true であれば、モジュール自体を消去する
		// ---- モジュールiの子を、モジュールiの親に橋渡し ---- //
		for (int i = 0; i < _children.size(); i++) {
			if (_children[i]->_id == id || _children[i]->_name == name) {
				
				for (int j = 0; j < _children[i]->_children.size(); j++) {
					// ---- モジュールiの子を本モジュールに移管 ---- //
					this->attachModule(this, _children[i]->_children[j], at);
				}
				// ---- 不要となるモジュール[i]のdk_stを削除する ---- //
				for (int n = 0; n < _dk_st.size(); n++) {
					if (_dk_st[n]._id == id || _dk_st[n]._name == name) {
						_dk_st.erase(std::cbegin(_dk_st) + n);
						_dk_st_tmp.erase(std::cbegin(_dk_st_tmp) + n);
						_dk_st_next.erase(std::cbegin(_dk_st_next) + n);
					}
				}
				// ---- _children[i]をis_eraseに応じて削除 ---- //
				if (is_erase) {
					_children[i]->~Module();
					_children.erase(std::cbegin(_children) + i);
				}
				return;
			}
		}
		if (_children.size() == 0) return;
		for (int i = 0; i < _children.size(); i++) {
			_children[i]->removeModuleTree(id, name, is_erase);	// 子の関数実施
		}
	}

void Module::directKinematics(const std::vector<dk_st>&dk) {
	// ----- 主に親モジュールのdk_stを参照し、自身の座標系・姿勢更新する関数 ----- //
	debugShowState();
	if (_id == 930 || _id == 4) {
		printf("deb#)");
	}
	// ---- 相対的な回転量初期化 ---- //
	for (auto d : _dk_st) {
		d._rot_sum = tnl::Quaternion::RotationAxis(_rot_axis, 0);
	}
	for (auto d : dk) {
		if (_id != d._id) { continue; }
		// ---- idが一致した時の処理：座標系更新 ---- //
		_pos = d._dir * d._length;
		_rot *= d._rot_sum;
		_rot_tmp = d._rot_sum;
		_rot_axis_tmp = tnl::Vector3::TransformCoord(_rot_axis, _rot);
		_dir_z_tmp = tnl::Vector3::TransformCoord(_dir_z, _rot);
		_dir_x_tmp = tnl::Vector3::TransformCoord(_dir_x, _rot);
	}
	// ---- 子モジュールまでの相対座標を更新 ---- //
	_dk_st_tmp = _dk_st;
	for (int i = 0; i < _dk_st.size(); i++) {
		_dk_st_tmp[i]._dir = tnl::Vector3::TransformCoord(_dk_st[i]._dir, _rot);
		_dk_st_tmp[i]._rot_sum = _rot_tmp;
	}
	// ---- 子モジュールまでの絶対座標を更新：子モジュールに渡すのはこちら ---- //
	_dk_st_next = _dk_st_tmp;
	for (int i = 0; i < _dk_st_next.size(); i++) {
		_dk_st_next[i]._dir = _pos + _dk_st_tmp[i]._dir * _dk_st[i]._length;
		_dk_st_next[i]._length = _dk_st_next[i]._dir.length();
		_dk_st_next[i]._dir.normalize();
	}
	
}

void Module::directKinematicsAnkIK(const std::vector<dk_st>& dk, float delta_time) {
	// ----- 運動学＆逆運動学計算を同時に実行したい場合、本関数を使用 ----- //
	debugShowState();
	if (_id == 307 || _id == 4) {
		printf("deb#)");
	}
	

	// --- test end --- //

	// ---- 相対的な回転量初期化 ---- //
	for (auto d : _dk_st) {
		d._rot_sum = tnl::Quaternion::RotationAxis(_rot_axis, 0);
	}
	for (auto d : dk) {
		if (_id != d._id) { continue; }
		// ---- idが一致した時の処理：座標系更新 ---- //
		_pos = d._dir * d._length;
		_rot *= d._rot_sum;
		_rot_tmp = d._rot_sum;
		_rot_axis_tmp = tnl::Vector3::TransformCoord(_rot_axis, _rot);	// IK実施前に座標系を更新しておく
		_dir_z_tmp = tnl::Vector3::TransformCoord(_dir_z, _rot);		// 〃 
		_dir_x_tmp = tnl::Vector3::TransformCoord(_dir_x, _rot);		// 〃
		// --- test --- //



		// --- test End --- //
	}
	// ---- 逆運動学計算実施 ---- //
	tnl::Quaternion rot_ik = inverseKinematics(delta_time);
	_rot *= rot_ik;									// IKの影響を反映
	_rot_tmp *= rot_ik;								// 1フレームの回転量総量を乗算	
	_rot_axis_tmp = tnl::Vector3::TransformCoord(_rot_axis, _rot);		// IK実施結果を座標系に反映
	_dir_z_tmp = tnl::Vector3::TransformCoord(_dir_z, _rot);			// 〃
	_dir_x_tmp = tnl::Vector3::TransformCoord(_dir_x, _rot);			// 〃
	// ---- 子モジュールまでの相対座標を更新 ---- //
	_dk_st_tmp = _dk_st;
	for (int i = 0; i < _dk_st.size(); i++) {
		if (_dk_st[i]._is_effect_ik) {
			_dk_st_tmp[i]._dir = tnl::Vector3::TransformCoord(_dk_st[i]._dir, _rot);
			_dk_st_tmp[i]._rot_sum = _rot_tmp;
		}
	}
	// ---- 子モジュールまでの絶対座標を更新：子モジュールに渡すのはこちら ---- //
	_dk_st_next = _dk_st_tmp;
	for (int i = 0; i < _dk_st_next.size(); i++) {
		_dk_st_next[i]._dir = _pos + _dk_st_tmp[i]._dir * _dk_st[i]._length;
		_dk_st_next[i]._length = _dk_st_next[i]._dir.length();
		_dk_st_next[i]._dir.normalize();
	}

}

void Module::directKinematicsTree(const Module* mod, std::vector<dk_st>& dk) {
	// ----- directKinematicsを子モジュールに再帰的に実施：preorder ----- //
	directKinematics(dk);
	if (_children.size() == 0) return;		// 子がない：親側へ戻る
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->directKinematicsTree(_children[i], this->_dk_st_next);		// 子の関数実施
	}
}

void Module::directKinematicsAndIKTree(const Module* mod, std::vector<dk_st>& dk, float delta_time) {
	// ----- directKinematicsAndIKを子モジュールに再帰的に実施：preorder ----- //
	directKinematicsAnkIK(dk, delta_time);
	if (_children.size() == 0) return;		// 子がない：親側へ戻る
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->directKinematicsAndIKTree(_children[i], this->_dk_st_next, delta_time);
	}
}

void Module::Tranlate(tnl::Vector3& move, _attach_type type) {
	// ----- モジュールの位置を変化させる ----- //
	// absolute : move位置へ移動
	// relative : 現モジュール座標からの移動量
	if (_id == 930) {
		printf("deb");
	}

	// ---- 位置変更 ---- //
	if (absolute == type) { _pos = move; }
	if (relative == type) { _pos += move; }
	// ---- 親のdk_st変更 ---- //
	if (_parent == nullptr) { return; }		// 親がいなければ終了
	for (int i = 0; i < _parent->_dk_st.size(); i++) {
		if (_parent->_dk_st[i]._id != _id) { continue; }
		tnl::Vector3 delta_pos = _pos - _parent->_pos;
		float delta_length = delta_pos.length();
		delta_pos.normalize();
		_parent->_dk_st[i]._dir = delta_pos;
		_parent->_dk_st[i]._length = delta_length;
	}
}

void Module::TranlateTree(int id, std::string name, tnl::Vector3& move, _attach_type type) {
	// ----- Translaateを特定の子モジュールに再帰敵に実施: preorder ----- //
	// absolute : move位置へ移動
	// relative : 現モジュール座標からの移動量
	if (_id == 200) {
		printf("deb");
	}
	if (_id == id || _name == name) {
		Tranlate(move, type);
		return;
	}
	if (_children.size() == 0) return;
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->TranlateTree(id, name, move, type);
	}
}



tnl::Quaternion Module::inverseKinematics(float delta_time) {
	// ----- _ik_stの定義通りに逆運動学計算を実施する ----- //
	if (_ik_st.size() == 0) {
		// 定義が無ければ、回転無しのクォータニオンを返す
		return tnl::Quaternion::RotationAxis(_rot_axis_tmp, 0);
	}		
	if (_id == 303) {
		printf("deb");
	}
	// ---- 逆運動学計算 ---- //
	float dth = 0;				// 偏角格納
	float dth_sum = 0;			// 偏角総和
	tnl::Vector3 pe;			// 制御対象位置 or 姿勢ベクトル
	tnl::Vector3 pr;			// 制御目標位置 or 姿勢ベクトル
	tnl::Vector3 x;				// 計算結果一時格納用
	tnl::Vector3 y;				//  〃
	for (int i = 0; i < _ik_st.size(); i++) {
		switch (_ik_st[i]._type)
		{
		case(pos_to_pos):
			pe = _ik_st[i]._object->_pos - this->_pos;
			pr = _ik_st[i]._target->_pos - this->_pos;
			break;

		case(dirz_look_pos):
			pe = _ik_st[i]._object->_dir_z_tmp;
			pr = _ik_st[i]._target->_pos - this->_pos;
			break;

		case(dirx_look_pos):
			pe = _ik_st[i]._object->_dir_x_tmp;
			pr = _ik_st[i]._target->_pos - this->_pos;
			break;

		case(dirz_as_dirz):
			pe = _ik_st[i]._object->_dir_z_tmp;
			pr = _ik_st[i]._target->_dir_z_tmp;
			break;

		case(dirx_as_dirx):
			pe = _ik_st[i]._object->_dir_x_tmp;
			pr = _ik_st[i]._target->_dir_x_tmp;
			break;

		case(dirz_as_dirx):
			pe = _ik_st[i]._object->_dir_z_tmp;
			pr = _ik_st[i]._target->_dir_x_tmp;
			break;

		case(dirx_as_dirz):
			pe = _ik_st[i]._object->_dir_x_tmp;
			pr = _ik_st[i]._target->_dir_z_tmp;
			break;

		default:
			printf("error: ik_typeの設定を見直してください");
			break;
		}
		x = tnl::Vector3::Cross(pe, _rot_axis_tmp);
		y = tnl::Vector3::Cross(pr, _rot_axis_tmp);
		dth = delta_time * 60.0 * _ik_st[i]._kp * std::acosf(std::clamp(
			x.dot(y) / x.length() / y.length(),
			(float)-1, (float)1
		));
		if (!isfinite(dth)) { dth = 0; }	// 回転軸上にtarget or object : 特異点->dth = 0でエラー回避
		if (dth > tnl::PI / 24) {			
			dth = tnl::PI / 24;
		}
		tnl::Vector3 axis = x.cross(y) / x.length() / y.length();	// 回転方向決定
		dth *= _rot_axis_tmp.dot(axis) >= 0 ? 1 : -1;
		dth_sum += dth;
	}
	tnl::Quaternion rot = tnl::Quaternion::RotationAxis(_rot_axis_tmp, dth_sum);
	
	return rot;
}

void Module::setEffectIKTree(int id, std::string name, bool is_ik) {
	// ----- setEffectIKを特定の子モジュールが現れるまで再帰的に実施 : preorder ----- //
	for (int i = 0; i < _dk_st.size(); i++) {
		if (_dk_st[i]._id == id || _dk_st[i]._name == name) {
			_dk_st[i]._is_effect_ik = is_ik;
		}

	}
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->setEffectIKTree(id, name, is_ik);
	}
	return;
}

void Module::setAxisView(float size, float length) {
	// ----- モジュール座標系を可視化するための関数 ----- //
	// モジュール生成後、すぐに時に呼び出す事。
	// 球、ｘ、ｚ軸を表示する
	Parts* sp = new Parts();
	sp->mesh_ = dxe::Mesh::CreateSphere(size);
	sp->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/gray.bmp"));
	this->_parts.push_back(sp);
	/*Parts* x = new Parts();
	x->mesh_ = dxe::Mesh::CreateCylinder(size/2, length);
	x->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	x->ofs_pos_ = this->_dir_x * length / 2;
	x->ofs_rot_ = this->_rot;
	x->ofs_rot_ *= tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	this->_parts.push_back(x);*/
	Parts* z = new Parts();
	z->mesh_ = dxe::Mesh::CreateCylinder(size/2, length);
	z->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	z->ofs_pos_ = this->_dir_z * length / 2;
	z->ofs_rot_ = this->_rot;
	//z->ofs_rot_ *= tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(-90));
	this->_parts.push_back(z);
}

//----------------------------------------------------------------------------------------------------------
// バイナリ形式保存
// arg1... 保存先ファイルパス
// tips... 複数のパーツで構成したモデルデータとしてバイナリ形式で保存する
// tips... この関数で保存するには各パーツのメッシュを プログラマブル形式で生成している事
void Module::exportForFileBinary(const std::string& file_path) {
	auto path = tnl::DetachmentFilePath(file_path);
	std::string parts_path;
	for (int i = 0; i < _parts.size(); ++i) {
		parts_path = std::get<0>(path) + "/" + "parts_" + std::to_string(i);
		_parts[i]->mesh_->exportForFileFormatMV(parts_path);
	}

	FILE* fp = nullptr;
	fopen_s(&fp, file_path.c_str(), "wb");
	if (fp) {
		int parts_num = _parts.size();
		fwrite(&parts_num, sizeof(int), 1, fp);

		char tex_path_buff[128] = { 0 };
		for (int i = 0; i < _parts.size(); ++i) {
			memset(tex_path_buff, 0, sizeof(tex_path_buff));
			sprintf_s(tex_path_buff, "%s", _parts[i]->mesh_->getTexture()->getFilePath().c_str());
			fwrite(tex_path_buff, sizeof(tex_path_buff), 1, fp);

			fwrite(&_parts[i]->mesh_->pos_, sizeof(tnl::Vector3), 1, fp);
			fwrite(&_parts[i]->mesh_->scl_, sizeof(tnl::Vector3), 1, fp);
			fwrite(&_parts[i]->mesh_->rot_q_, sizeof(tnl::Quaternion), 1, fp);

			fwrite(&_parts[i]->ofs_pos_, sizeof(tnl::Vector3), 1, fp);
			fwrite(&_parts[i]->ofs_rot_, sizeof(tnl::Quaternion), 1, fp);
		}
		fclose(fp);
	}

}

//----------------------------------------------------------------------------------------------------------
// バイナリ形式保存されたモデルデータの読み込み
// arg1... ファイルパス
// tips... exportForFileBinary で保存されたファイルを読み込みます
void Module::loadFromFileBinary(const std::string& file_path) {

	auto path = tnl::DetachmentFilePath(file_path);

	FILE* fp = nullptr;
	fopen_s(&fp, file_path.c_str(), "rb");
	if (fp) {
		int parts_num = 0;
		fread(&parts_num, sizeof(int), 1, fp);

		char tex_path_buff[128] = { 0 };
		for (int i = 0; i < parts_num; ++i) {
			fread(tex_path_buff, sizeof(tex_path_buff), 1, fp);

			Parts* parts = new Parts();
			parts->mesh_ = dxe::Mesh::CreateFromFileMV(std::get<0>(path) + "/" + "parts_" + std::to_string(i));
			fread(&parts->mesh_->pos_, sizeof(tnl::Vector3), 1, fp);
			fread(&parts->mesh_->scl_, sizeof(tnl::Vector3), 1, fp);
			fread(&parts->mesh_->rot_q_, sizeof(tnl::Quaternion), 1, fp);

			fread(&parts->ofs_pos_, sizeof(tnl::Vector3), 1, fp);
			fread(&parts->ofs_rot_, sizeof(tnl::Quaternion), 1, fp);

			parts->mesh_->setTexture(dxe::Texture::CreateFromFile(tex_path_buff));

			_parts.emplace_back(parts);
		}

		fclose(fp);
	}
}

void Module::debugShowState() {
	
	// ----- 位置・姿勢等表示 ----- //
	DrawStringEx(10, _id * 20, -1, "pos = (%f, %f, %f)", _pos.x, _pos.y, _pos.z);
	
}