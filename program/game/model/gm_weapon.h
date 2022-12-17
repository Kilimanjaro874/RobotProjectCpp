#pragma once
#include "gm_module.h"

class Weapon : public Module {
public:
	Weapon() {};
	~Weapon() {
		for (auto bs : _bullets) { delete bs; }
		for (auto bm : _bulletParts) { delete bm; }
	}
	float _reload_time;
	int _bullet_num;
	float _reload_count = 0;

	class Bullet {
	public:
		Bullet(tnl::Vector3 dir, tnl::Vector3 pos, float bs, float rd, dxe::Mesh* ms) {
			_dir = dir;
			_pos = pos;
			_init_pos = pos;
			_bullet_speed = bs;
			_range_distance = rd;
			_bullet = ms;
		};
		~Bullet() {
			delete _bullet;
		}
		tnl::Vector3 _dir;
		tnl::Vector3 _pos;
		tnl::Vector3 _init_pos;
		float _bullet_speed;
		float _range_distance;
		bool _is_hit = false;
		dxe::Mesh* _bullet = nullptr;
	};
	enum bullet_type {
		normal, rifle, missile, bomb, end
	};
	bullet_type _bullet_type = normal;
	std::vector<Bullet*> _bullets;
	std::vector<dxe::Mesh*> _bulletParts;
	void partsUpdate(float delta_time) override;
	void partsRender(dxe::Camera* camera) override;
	static Weapon* createWeaponMod(int id, std::string name, 
		float reload_time, int bullet_num,
		tnl::Vector3 pos, tnl::Vector3 rot_axis,
		tnl::Quaternion rot = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0),
		tnl::Vector3 dir_z = { 0, 0, 1 }, tnl::Vector3 dir_x = { 1, 0, 0 });
	void init_bulletParts();
	void genBullet(float delta_time, bool fire_trigger);
};