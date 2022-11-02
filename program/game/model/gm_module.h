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
	//// ------ �����o�ϐ� ------ ////
	// ----- Render ----- //
	std::vector<Parts*> _parts;			// �p�[�c�N���X
	bool _is_render = true;				// �`�攻��

	// ----- Parameter ----- //
	int _id;							// ID: �c���[�\���̌������ɗp����
	std::string _name;					// ���O�F�c���[�\���̌������ɗp����
	Module* _parent;					// �e���W���[��: �c���[�\�������p
	std::vector<Module*> _children;		// �q���W���[���F�c���[�\�������p
											
	// ----- �^���w�v�Z�p(DK) ----- //
	struct dk_st {
		// ---- �e����q���W���[���։^���w�v�Z�����{���邽�߂̃p�����[�^���\���̂Œ�`���Ă��� ---- //
		int _id;
		std::string _name;
		tnl::Vector3 _dir;
		float _length;
	};
	std::vector<dk_st> _dk_st;
	// ----- ���W�n��`�p ----- //
	tnl::Vector3 _pos;					// ���W���[��i�̈ʒu(���[���h���W�F��o)
	tnl::Vector3 _rot_axis;				// ���W���[��i��]��(��o)
	tnl::Vector3 _dir_z;				// z���P�ʃx�N�g����`(��o)
	tnl::Vector3 _dir_x;				// x���P�ʃx�N�g����`(��o)
	tnl::Quaternion _rot;				// �N�H�[�^�j�I��(��o)

	// ----- �t�^���w�v�Z�p ----- //


	enum _ik_type {
		// ---- ���䃂�W���[�� - ����ڕW���W���[�����m���ǂ̎�ނ�IK�Ŏ��{���邩�w��p ---- //
		pos_to_pos,						// �ʒu���m���߂Â���
		dirz_look_pos,					// z����ڕW�ʒu�Ɍ�����
		dirx_look_pos,					
		dirz_as_dirz,					// z����ڕW�p��z���Ɠ��������ɋ߂Â���
		dirx_as_dirx,
		dirz_as_dirx,
		dirx_as_dirx,
	};

	enum _attach_type {
		// ---- ���W���[����e�ɃA�^�b�`���鎞�A�ʒu�F��΍��W�n�Q�� or ���΍��W�Q�Ƃ�
		absolute,
		relative,
	};

	//// ----- �����o�֐� ------ ////
	virtual void partsUpdate(float delta_time);
	virtual void partsRender(dxe::Camera* camera);
	void partsUpdateTree(const Module* mod, float delta_time);
	void partsRenderTree(const Module* mod, dxe::Camera* camera);
	static Module* createModule(int id, std::string name, tnl::Vector3 pos, tnl::Vector3 rot_axis,
		tnl::Quaternion rot = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0), 
		tnl::Vector3 dir_z = { 0, 0, 1 }, tnl::Vector3 dir_x = { 1, 0, 0 });
	void attachModule(Module* parent, Module* child, _attach_type type);

};