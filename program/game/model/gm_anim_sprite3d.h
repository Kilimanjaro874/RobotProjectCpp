#pragma once
#include "gm_model.h"

//
// �A�j���[�V�����X�v���C�g ( 3D )
//

class AnimUnit {
public :
	Shared<tnl::SeekUnit> seek_;
	Shared<dxe::Texture> texture_;
	uint32_t parts_start_index_ = 0;
};

class AnimSprite3D : public Model {
private :
	float alpha_ = 1.0f;
	int dxlib_blend_mode_ = DX_BLENDMODE_ALPHA;
	tnl::Vector3 mtrl_emmisive_ = { 1, 1, 1 };
	std::unordered_map<std::string, Shared<AnimUnit>> anims_;
	Shared<AnimUnit> anim_current_ = nullptr;
	dxe::Camera* billboard_target_ = nullptr ;
public :
	AnimSprite3D(dxe::Camera* camera) { billboard_target_ = camera; }

	void update(float delta_time) override;

	//------------------------------------------------------------------------------------------
	// �A�j���[�V�����̐؂�ւ�
	// arg1... �؂�ւ������A�j���[�V�����̖��O
	// tips... regist �֐��œo�^����Ă���A�j���[�V�����ɐ؂�ւ���
	void setCurrentAnim(const std::string& anim_name);

	//------------------------------------------------------------------------------------------
	// �A�j���[�V�����̃R���g���[�����擾
	const Shared<tnl::SeekUnit> getCurrentAnimSeekUnit() { return anim_current_->seek_; }

	//------------------------------------------------------------------------------------------
	// �P��ނ̃A�j���[�V�������̓o�^
	// arg1... �쐬�����|���̉���
	// arg2... �쐬�����|���̏c��
	// arg3... �o�^����A�j���[�V�����̔C�ӂ̖��O
	// arg4... �A�j���[�V�����Ɏg�p�����e�N�X�`���p�X
	// arg5... �Đ����[�h ( tnl::SeekUnit::ePlayMode::REPEAT �� )
	// arg6... �S�̂̍Đ�����
	// arg7... �A�j���[�V�����̃t���[����
	// arg8... 1�t���[��������̉摜�̍���
	// arg9... �ォ�牽�R�}�ڂ���ǂݍ��ނ�
	void regist(
		const float plane_width
		, const float plane_height
		, const std::string& anim_name
		, const std::string& tex_path
		, const tnl::SeekUnit::ePlayMode play_mode
		, const float play_time
		, const uint32_t frame_num
		, const uint32_t frame_size_h
		, const uint32_t frame_start_num_h);

};
