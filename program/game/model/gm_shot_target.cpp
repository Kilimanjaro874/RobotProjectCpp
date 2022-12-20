#include "gm_shot_taget.h"

ShotTarget* ShotTarget::init(std::vector<tnl::Vector3> points, float move_time) {
	// ---- ターゲット初期化 ---- //
	ShotTarget* st = new ShotTarget();
	Parts* target = new Parts();
	target->mesh_ = dxe::Mesh::CreateSphere(st->_circle_size);
	target->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	
	st->_parts.push_back(target);
	st->_move_points = points;
	st->_move_time = move_time;

	return st;
}

void ShotTarget::move(float delta_time) {
	
	// 
	tnl::Vector3 Ps = _move_points[_move_point_num];
	tnl::Vector3 Pe;
	if (_move_point_num >= _move_points.size() - 1) {
		Pe = _move_points[0];
	}
	else {
		Pe = _move_points[_move_point_num + 1];
	}
	// 初期化
	if (_move_time_count == 0) {
		_dir_z = (Pe - Ps);
		_distance = _dir_z.length();
		_dir_z.normalize();
		
	}

	// 時間管理
	_move_time_count += delta_time / _move_time;
	
	tnl::Vector3 tmp_pos = Ps + _dir_z * _move_time_count * _move_time_count * _distance;
	_pos = tmp_pos;
	// 到達確認
	if (_move_time_count >= 1.0) {
		_move_time_count = 0;
		_move_point_num++;
		if (_move_point_num >= _move_points.size()) {
			_move_point_num = 0;
		}

	}
	
}