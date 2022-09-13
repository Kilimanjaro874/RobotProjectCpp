#include "gm_anim_sprite3d.h"

//--------------------------------------------------------------------------------------------------------
void AnimSprite3D::setCurrentAnim(const std::string& anim_name) {
	int idx = 0;
	// ����܂ŕ\�����Ă����A�j���[�V�������\���ɂ���
	if (anim_current_){
		anim_current_->seek_->stop();
		idx = anim_current_->parts_start_index_ + anim_current_->seek_->getSeekFrame(tnl::SeekUnit::eFrameType::CURRENT);
		parts_[idx]->is_render_ = false;
	}
	// ���݂̃A�j���[�V�������X�V���čĐ�
	anim_current_ = anims_[anim_name];
	idx = anim_current_->parts_start_index_ ;
	for (int i = 0; i < anim_current_->seek_->getTotalFrameNum(); ++i) {
		parts_[idx + i]->mesh_->alpha_ = alpha_;
		parts_[idx + i]->mesh_->setBlendMode(dxlib_blend_mode_);
		parts_[idx + i]->mesh_->setMtrlEmissive(mtrl_emmisive_);
	}
	idx += anim_current_->seek_->getSeekFrame(tnl::SeekUnit::eFrameType::CURRENT);
	parts_[idx]->is_render_ = true;
	anim_current_->seek_->play();
}


//--------------------------------------------------------------------------------------------------------
void AnimSprite3D::update(float delta_time) {

	// �X�V�O�̃t���[�����\���ɂ���
	int idx = anim_current_->parts_start_index_ + anim_current_->seek_->getSeekFrame(tnl::SeekUnit::eFrameType::CURRENT);
	parts_[idx]->is_render_ = false;

	// �A�j���[�V�����̃t���[�����X�V
	anim_current_->seek_->update(delta_time);

	// �V�����t���[����\����Ԃɂ���
	idx = anim_current_->parts_start_index_ + anim_current_->seek_->getSeekFrame(tnl::SeekUnit::eFrameType::CURRENT);
	parts_[idx]->is_render_ = true;

	// �p�[�c���W�̍X�V
	parts_[idx]->mesh_->pos_ = pos_;
	parts_[idx]->mesh_->rot_q_ = tnl::Quaternion::LookAtAxisY(billboard_target_->pos_, billboard_target_->target_);
}

//--------------------------------------------------------------------------------------------------------
void AnimSprite3D::regist(
	const float plane_width
	, const float plane_height
	, const std::string& anim_name
	, const std::string& tex_path
	, const tnl::SeekUnit::ePlayMode play_mode
	, const float play_time
	, const uint32_t frame_num
	, const uint32_t frame_size_h
	, const uint32_t frame_start_num_h)
{
	// �P��ނ̃A�j���[�V���������쐬
	Shared<AnimUnit> unit = std::make_shared<AnimUnit>();
	unit->seek_ = tnl::SeekUnit::Create(60, play_time, frame_num, play_mode, tnl::SeekUnit::ePlayDir::FORWARD);
	unit->texture_ = dxe::Texture::CreateFromFile(tex_path);
	unit->parts_start_index_ = parts_.size();
	anims_[anim_name] = unit;

	int img_w, img_h;
	GetGraphSize(unit->texture_->getGraphHandle(), &img_w, &img_h);

	// �t���[�����Ƀp�[�c���쐬
	float sv = (float)frame_start_num_h / (float)img_h;
	float ev = (float)(frame_start_num_h + frame_size_h) / (float)img_h;
	for (int i = 0; i < frame_num; ++i) {
		Parts* parts = new Parts();
		parts->mesh_ = dxe::Mesh::CreatePlaneMV(
			{ plane_width, plane_height, 0 }, 1, 1, true, 
			{ (float)i / (float)frame_num, sv, 0 }, { float(i+1) / (float)frame_num, ev, 0 });
		parts->mesh_->setTexture(unit->texture_);
		parts->mesh_->setDefaultLightEnable(false);
		parts_.emplace_back(parts);
		parts_[unit->parts_start_index_ + i]->is_render_ = false;
	}


}

