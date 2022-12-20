#include "gm_module.h"

void Module::partsUpdate(float delta_time) {
	// ---- ���i�̈ʒu�E�p�������W���[�����W�n�ɉ����čX�V ---- //

	if (_id == 930) {
		printf("deb");
	}

	for (auto pts : _parts) {
		pts->mesh_->pos_ = _pos + tnl::Vector3::TransformCoord(pts->ofs_pos_, _rot);
		pts->mesh_->rot_q_ = pts->ofs_rot_ * _rot;
	}
}
/// <summary>
///  ----- ���i�̃V�F�[�_�[��`�悷�� ----- //
/// 
/// </summary>
/// <param name="camera"></param>
void Module::partsRender(dxe::Camera* camera) {
	// ----- ���i�̃V�F�[�_�[��`�悷�� ----- //

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
	// ---- partsUpdate���q���W���[���ɍċA�I�Ɏ��{ : preorder ---- //
	partsUpdate(delta_time);
	if (_children.size() == 0) return;		// �q�������F�e���֖߂�
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->partsUpdateTree(_children[i], delta_time);	// �q�̊֐����{
	}
}

void Module::partsRenderTree(const Module* mod, dxe::Camera* camera) {
	// ---- partsRender���q���W���[���ɍċA�I�Ɏ��{�Fpreorder ---- //
	partsRender(camera);
	if (_children.size() == 0) return;		// �q���Ȃ��F�e���֖߂�
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->partsRenderTree(_children[i], camera);		// �q�̊֐����{
	}
}

Module* Module::createModule(int id, std::string name, 
	tnl::Vector3 pos, tnl::Vector3 rot_axis,
	tnl::Quaternion rot , tnl::Vector3 dir_z, tnl::Vector3 dir_x) {
	// ----- ���W���[���𐶐����� ----- //
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
	// ----- ���W���[���c���[�\���\�z�̂��߁A�e�q�֌W�ݒ� ----- //
	parent->_children.push_back(child);
	child->_parent = parent;
	// ---- �΍��x�N�g����e�Ɋi�[ ---- //
	tnl::Vector3 delta_pos;
	if (_attach_type::absolute == type) { delta_pos = child->_pos - parent->_pos; }
	else if (_attach_type::relative == type)	{ delta_pos = child->_pos; }
	float delta_length = delta_pos.length();
	delta_pos.normalize();
	// ---- _dk_st�\���̂Ɋi�[ ---- //
	parent->_dk_st.push_back({ child->_id, child->_name, 
		delta_pos, delta_length, tnl::Quaternion::RotationAxis({0, 1, 0}, 0) });
	// ---- ����_dk_st�\���̂ɃR�s�[ ---- //
	_dk_st_tmp = _dk_st;
	_dk_st_next = _dk_st;
}

void Module::attachModuleTree(int id, std::string name, Module* child, _attach_type type) {
	// ----- ���W���[���c���[�\���́A����̐e(id, ���O�Q��)�Ɏq���W���[�����ċA�I�ɃA�^�b�`�Fpreorder ---- //
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
	// ----- ���W���[���Ƀp�[�c���A�^�b�`�F�ċA�����Fpreorder ----- //
	if (_id == id || _name == name) {
		_parts.push_back(parts);
		return;
	}
	if (_children.size() == 0) { return; }
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->attachPartsTree(id, name, parts);	// �q�̊֐����{
	}
}

void Module::attachIKstTree(std::vector<ik_st>& ik) {
	// ----- ���W���[����IK�ݒ���A�b�^�b�`�F�ċA�����Fpreorder ----- //
	for (int i = 0; i < ik.size(); i++) {
		// vector�^�ɂ���Y������ik_st�͑S�Ď擾���� : �S�T���Freturn ����
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
	// ----- ����id, name�ɉ��������W���[����؍\������ċA�I�Ɏ擾�Fpreorder ----- //
	if (this->_id == id || this->_name == name) {
		mod = this;	// �ċA�I��
		return mod;
	}
	for (int i = 0; i < _children.size(); i++) {
		mod = _children[i]->getModulePointerTree(mod, id, name);
		if (mod == nullptr) { continue; }
		if (mod->_id == id || mod->_name == name) { return mod; }	// �ċA�I��
	}
	return nullptr;

}

void Module::removeModuleTree(int id, std::string name, bool is_erase, _attach_type at) {
	// ----- �{���W���[�����q�ɂ�������id�A�u���́v���O�̃��W���[�����^���w�v�Z�̃����o����O�� ----- //
	// �܂��Ais_erase == true �ł���΁A���W���[�����̂���������
		// ---- ���W���[��i�̎q���A���W���[��i�̐e�ɋ��n�� ---- //
		for (int i = 0; i < _children.size(); i++) {
			if (_children[i]->_id == id || _children[i]->_name == name) {
				
				for (int j = 0; j < _children[i]->_children.size(); j++) {
					// ---- ���W���[��i�̎q��{���W���[���Ɉڊ� ---- //
					this->attachModule(this, _children[i]->_children[j], at);
				}
				// ---- �s�v�ƂȂ郂�W���[��[i]��dk_st���폜���� ---- //
				for (int n = 0; n < _dk_st.size(); n++) {
					if (_dk_st[n]._id == id || _dk_st[n]._name == name) {
						_dk_st.erase(std::cbegin(_dk_st) + n);
						_dk_st_tmp.erase(std::cbegin(_dk_st_tmp) + n);
						_dk_st_next.erase(std::cbegin(_dk_st_next) + n);
					}
				}
				// ---- _children[i]��is_erase�ɉ����č폜 ---- //
				if (is_erase) {
					_children[i]->~Module();
					_children.erase(std::cbegin(_children) + i);
				}
				return;
			}
		}
		if (_children.size() == 0) return;
		for (int i = 0; i < _children.size(); i++) {
			_children[i]->removeModuleTree(id, name, is_erase);	// �q�̊֐����{
		}
	}

void Module::directKinematics(const std::vector<dk_st>&dk) {
	// ----- ��ɐe���W���[����dk_st���Q�Ƃ��A���g�̍��W�n�E�p���X�V����֐� ----- //
	debugShowState();
	if (_id == 930 || _id == 4) {
		printf("deb#)");
	}
	// ---- ���ΓI�ȉ�]�ʏ����� ---- //
	for (auto d : _dk_st) {
		d._rot_sum = tnl::Quaternion::RotationAxis(_rot_axis, 0);
	}
	for (auto d : dk) {
		if (_id != d._id) { continue; }
		// ---- id����v�������̏����F���W�n�X�V ---- //
		_pos = d._dir * d._length;
		_rot *= d._rot_sum;
		_rot_tmp = d._rot_sum;
		_rot_axis_tmp = tnl::Vector3::TransformCoord(_rot_axis, _rot);
		_dir_z_tmp = tnl::Vector3::TransformCoord(_dir_z, _rot);
		_dir_x_tmp = tnl::Vector3::TransformCoord(_dir_x, _rot);
	}
	// ---- �q���W���[���܂ł̑��΍��W���X�V ---- //
	_dk_st_tmp = _dk_st;
	for (int i = 0; i < _dk_st.size(); i++) {
		_dk_st_tmp[i]._dir = tnl::Vector3::TransformCoord(_dk_st[i]._dir, _rot);
		_dk_st_tmp[i]._rot_sum = _rot_tmp;
	}
	// ---- �q���W���[���܂ł̐�΍��W���X�V�F�q���W���[���ɓn���̂͂����� ---- //
	_dk_st_next = _dk_st_tmp;
	for (int i = 0; i < _dk_st_next.size(); i++) {
		_dk_st_next[i]._dir = _pos + _dk_st_tmp[i]._dir * _dk_st[i]._length;
		_dk_st_next[i]._length = _dk_st_next[i]._dir.length();
		_dk_st_next[i]._dir.normalize();
	}
	
}

void Module::directKinematicsAnkIK(const std::vector<dk_st>& dk, float delta_time) {
	// ----- �^���w���t�^���w�v�Z�𓯎��Ɏ��s�������ꍇ�A�{�֐����g�p ----- //
	debugShowState();
	if (_id == 307 || _id == 4) {
		printf("deb#)");
	}
	

	// --- test end --- //

	// ---- ���ΓI�ȉ�]�ʏ����� ---- //
	for (auto d : _dk_st) {
		d._rot_sum = tnl::Quaternion::RotationAxis(_rot_axis, 0);
	}
	for (auto d : dk) {
		if (_id != d._id) { continue; }
		// ---- id����v�������̏����F���W�n�X�V ---- //
		_pos = d._dir * d._length;
		_rot *= d._rot_sum;
		_rot_tmp = d._rot_sum;
		_rot_axis_tmp = tnl::Vector3::TransformCoord(_rot_axis, _rot);	// IK���{�O�ɍ��W�n���X�V���Ă���
		_dir_z_tmp = tnl::Vector3::TransformCoord(_dir_z, _rot);		// �V 
		_dir_x_tmp = tnl::Vector3::TransformCoord(_dir_x, _rot);		// �V
		// --- test --- //



		// --- test End --- //
	}
	// ---- �t�^���w�v�Z���{ ---- //
	tnl::Quaternion rot_ik = inverseKinematics(delta_time);
	_rot *= rot_ik;									// IK�̉e���𔽉f
	_rot_tmp *= rot_ik;								// 1�t���[���̉�]�ʑ��ʂ���Z	
	_rot_axis_tmp = tnl::Vector3::TransformCoord(_rot_axis, _rot);		// IK���{���ʂ����W�n�ɔ��f
	_dir_z_tmp = tnl::Vector3::TransformCoord(_dir_z, _rot);			// �V
	_dir_x_tmp = tnl::Vector3::TransformCoord(_dir_x, _rot);			// �V
	// ---- �q���W���[���܂ł̑��΍��W���X�V ---- //
	_dk_st_tmp = _dk_st;
	for (int i = 0; i < _dk_st.size(); i++) {
		if (_dk_st[i]._is_effect_ik) {
			_dk_st_tmp[i]._dir = tnl::Vector3::TransformCoord(_dk_st[i]._dir, _rot);
			_dk_st_tmp[i]._rot_sum = _rot_tmp;
		}
	}
	// ---- �q���W���[���܂ł̐�΍��W���X�V�F�q���W���[���ɓn���̂͂����� ---- //
	_dk_st_next = _dk_st_tmp;
	for (int i = 0; i < _dk_st_next.size(); i++) {
		_dk_st_next[i]._dir = _pos + _dk_st_tmp[i]._dir * _dk_st[i]._length;
		_dk_st_next[i]._length = _dk_st_next[i]._dir.length();
		_dk_st_next[i]._dir.normalize();
	}

}

void Module::directKinematicsTree(const Module* mod, std::vector<dk_st>& dk) {
	// ----- directKinematics���q���W���[���ɍċA�I�Ɏ��{�Fpreorder ----- //
	directKinematics(dk);
	if (_children.size() == 0) return;		// �q���Ȃ��F�e���֖߂�
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->directKinematicsTree(_children[i], this->_dk_st_next);		// �q�̊֐����{
	}
}

void Module::directKinematicsAndIKTree(const Module* mod, std::vector<dk_st>& dk, float delta_time) {
	// ----- directKinematicsAndIK���q���W���[���ɍċA�I�Ɏ��{�Fpreorder ----- //
	directKinematicsAnkIK(dk, delta_time);
	if (_children.size() == 0) return;		// �q���Ȃ��F�e���֖߂�
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->directKinematicsAndIKTree(_children[i], this->_dk_st_next, delta_time);
	}
}

void Module::Tranlate(tnl::Vector3& move, _attach_type type) {
	// ----- ���W���[���̈ʒu��ω������� ----- //
	// absolute : move�ʒu�ֈړ�
	// relative : �����W���[�����W����̈ړ���
	if (_id == 930) {
		printf("deb");
	}

	// ---- �ʒu�ύX ---- //
	if (absolute == type) { _pos = move; }
	if (relative == type) { _pos += move; }
	// ---- �e��dk_st�ύX ---- //
	if (_parent == nullptr) { return; }		// �e�����Ȃ���ΏI��
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
	// ----- Translaate�����̎q���W���[���ɍċA�G�Ɏ��{: preorder ----- //
	// absolute : move�ʒu�ֈړ�
	// relative : �����W���[�����W����̈ړ���
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
	// ----- _ik_st�̒�`�ʂ�ɋt�^���w�v�Z�����{���� ----- //
	if (_ik_st.size() == 0) {
		// ��`��������΁A��]�����̃N�H�[�^�j�I����Ԃ�
		return tnl::Quaternion::RotationAxis(_rot_axis_tmp, 0);
	}		
	if (_id == 303) {
		printf("deb");
	}
	// ---- �t�^���w�v�Z ---- //
	float dth = 0;				// �Ίp�i�[
	float dth_sum = 0;			// �Ίp���a
	tnl::Vector3 pe;			// ����Ώۈʒu or �p���x�N�g��
	tnl::Vector3 pr;			// ����ڕW�ʒu or �p���x�N�g��
	tnl::Vector3 x;				// �v�Z���ʈꎞ�i�[�p
	tnl::Vector3 y;				//  �V
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
			printf("error: ik_type�̐ݒ���������Ă�������");
			break;
		}
		x = tnl::Vector3::Cross(pe, _rot_axis_tmp);
		y = tnl::Vector3::Cross(pr, _rot_axis_tmp);
		dth = delta_time * 60.0 * _ik_st[i]._kp * std::acosf(std::clamp(
			x.dot(y) / x.length() / y.length(),
			(float)-1, (float)1
		));
		if (!isfinite(dth)) { dth = 0; }	// ��]�����target or object : ���ٓ_->dth = 0�ŃG���[���
		if (dth > tnl::PI / 24) {			
			dth = tnl::PI / 24;
		}
		tnl::Vector3 axis = x.cross(y) / x.length() / y.length();	// ��]��������
		dth *= _rot_axis_tmp.dot(axis) >= 0 ? 1 : -1;
		dth_sum += dth;
	}
	tnl::Quaternion rot = tnl::Quaternion::RotationAxis(_rot_axis_tmp, dth_sum);
	
	return rot;
}

void Module::setEffectIKTree(int id, std::string name, bool is_ik) {
	// ----- setEffectIK�����̎q���W���[���������܂ōċA�I�Ɏ��{ : preorder ----- //
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
	// ----- ���W���[�����W�n���������邽�߂̊֐� ----- //
	// ���W���[��������A�����Ɏ��ɌĂяo�����B
	// ���A���A������\������
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
// �o�C�i���`���ۑ�
// arg1... �ۑ���t�@�C���p�X
// tips... �����̃p�[�c�ō\���������f���f�[�^�Ƃ��ăo�C�i���`���ŕۑ�����
// tips... ���̊֐��ŕۑ�����ɂ͊e�p�[�c�̃��b�V���� �v���O���}�u���`���Ő������Ă��鎖
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
// �o�C�i���`���ۑ����ꂽ���f���f�[�^�̓ǂݍ���
// arg1... �t�@�C���p�X
// tips... exportForFileBinary �ŕۑ����ꂽ�t�@�C����ǂݍ��݂܂�
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
	
	// ----- �ʒu�E�p�����\�� ----- //
	DrawStringEx(10, _id * 20, -1, "pos = (%f, %f, %f)", _pos.x, _pos.y, _pos.z);
	
}