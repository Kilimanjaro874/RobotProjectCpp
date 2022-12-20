#include "gm_weapon.h"

void Weapon::init_bulletParts() {
	_bulletParts.resize(end);
	// ---- normal Bullet ---- //
	_bulletParts[normal] = dxe::Mesh::CreateSphere(0.2);
	_bulletParts[normal]->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
}

void Weapon::partsUpdate(float delta_time) {
	Module::partsUpdate(delta_time);
	// ---- Bullet�N���X�̃A�b�v�f�[�g���� ---- //
	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i]->_pos += _bullets[i]->_dir * _bullets[i]->_bullet_speed;
		_bullets[i]->_bullet->pos_ = _bullets[i]->_pos;
		tnl::Vector3 norm = _bullets[i]->_pos - _bullets[i]->_init_pos;
		if (norm.length() > _bullets[i]->_range_distance) {
			delete _bullets[i];
			_bullets.erase(std::cbegin(_bullets) + i);
		}
	}
}

void Weapon::partsRender(dxe::Camera* camera) {
	Module::partsRender(camera);
	// ---- Bullet�N���X�̕`�揈�� ---- //
	for (auto bs : _bullets) {
		bs->_bullet->render(camera);
	}
}

Weapon* Weapon::createWeaponMod(int id, std::string name,
	float reload_time, int bullet_num,
	tnl::Vector3 pos, tnl::Vector3 rot_axis,
	tnl::Quaternion rot, tnl::Vector3 dir_z, tnl::Vector3 dir_x) {
	// ----- ���탂�W���[���𐶐����� ----- //
	Weapon* wea = new Weapon();
	wea->_id = id;
	wea->_name = name;
	wea->_reload_time = reload_time;
	wea->_bullet_num = bullet_num;
	wea->_pos = pos;
	wea->_rot_axis = rot_axis;
	wea->_rot = rot;
	wea->_dir_z = dir_z;
	wea->_dir_x = dir_x;
	// ������
	wea->init_bulletParts();

	return wea;
}

/// <summary>
///  �����[�h���ԁA�g���K�[�ɉ����Ēe�̐���������
///  Bullet�N���X�Q�Ɣz��Ɋi�[��AupdateBullets�ōX�V���Ǘ�
/// </summary>
/// <param name="delta_time"></param>
/// <param name="fire_trigger"></param>
bool Weapon::genBullet(float delta_time, bool fire_trigger) {
	_reload_count = _reload_count + delta_time;
	if (_reload_count >= _reload_time) { _reload_count = _reload_time; }
	if (_reload_count < _reload_time) { return false; }
	if (!fire_trigger) { return false; }
	_reload_count = 0;
	// ---- �e�̐������� ---- //
	if (_bullet_type == normal) {
		dxe::Mesh* bullet = _bulletParts[normal]->createClone();			// �V���Ȓe�ۃ��b�V���𐶐�
		_bullets.push_back(new Bullet(_dir_z_tmp, _pos, 10, 2000, bullet));	// �V���Ȓe�ۃN���X��o�^
	}
	return true;
}

