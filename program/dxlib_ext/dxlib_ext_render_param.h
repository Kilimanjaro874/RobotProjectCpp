#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace dxe {

	class RenderParam {
	public :
		RenderParam() {
			dxlib_mtrl_.Ambient = { 0.1f, 0.1f, 0.1f, 1 };
			dxlib_mtrl_.Diffuse = { 1, 1, 1, 1 };
			dxlib_mtrl_.Emissive = { 0.4f, 0.4f, 0.4f, 1 };
			dxlib_mtrl_.Power = 5;
			dxlib_mtrl_.Specular = { 0.25f, 0.25f, 0.25f, 1 };
		}

		// �J�����O���[�h�ݒ�
		// arg1... DX_CULLING_***
		inline void setCullingMode(const int mode) noexcept { dxlib_culling_mode_ = mode; }

		// �u�����h���[�h�ݒ�
		// arg1... DX_BLENDMODE_***
		// tips... MV ���b�V���̂ݑΉ� ( dxlib �̎d�l )
		inline void setBlendMode(const int mode) noexcept { dxlib_blend_mode_ = mode; }

		// �e�N�X�`���T���v���t�B���^�ݒ�
		// arg1... DX_DRAWMODE_***
		// tips... MV ���b�V���̂ݑΉ� ( dxlib �̎d�l )
		inline void setSampleFileterMode(const int mode) noexcept { dxlib_sample_filter_mode_ = mode; }

		// �f�t�H���g���C�g�̗L����
		inline void setDefaultLightEnable(const bool f) noexcept { is_default_light_enable_ = f; }

		// �[�x�e�X�g�̗L����
		inline void setDepthTestEnable(const bool f) noexcept { is_depth_test_enable_ = f; }

		// �[�x�o�b�t�@�ւ̏������ݗL����
		inline void setWriteDepthBufferEnable(const bool f) noexcept { is_write_depth_buffer_enable_ = f; }

		// �f�B�t���[�Y�J���[�ݒ�
		inline void setMtrlDiffuse(const tnl::Vector3& col) noexcept {
			dxlib_mtrl_.Diffuse = { col.x, col.y, col.z, 1.0f };
		}
		// �X�y�L�����J���[�ݒ�
		inline void setMtrlSpecular(const tnl::Vector3& col) noexcept {
			dxlib_mtrl_.Specular = { col.x, col.y, col.z, 1.0f };
		}
		// �G�~�b�V���J���[�ݒ�
		inline void setMtrlEmissive(const tnl::Vector3& col) noexcept {
			dxlib_mtrl_.Emissive = { col.x, col.y, col.z, 1.0f };
		}
		// �X�y�L�����p���[�ݒ�
		inline void setMtrlSpecPower(const float pow) noexcept {
			dxlib_mtrl_.Power = pow;
		}

		inline RenderParam& operator = (const RenderParam& other) noexcept {
			alpha_ = other.alpha_;
			is_default_light_enable_ = other.is_default_light_enable_;
			is_depth_test_enable_ = other.is_depth_test_enable_;
			is_write_depth_buffer_enable_ = other.is_write_depth_buffer_enable_;
			dxlib_blend_mode_ = other.dxlib_blend_mode_;
			dxlib_culling_mode_ = other.dxlib_culling_mode_;
			dxlib_sample_filter_mode_ = other.dxlib_sample_filter_mode_;
			memcpy(&dxlib_mtrl_, &other.dxlib_mtrl_, sizeof(MATERIALPARAM));
			return *this;
		}

		float			alpha_ = 1.0f;
		bool			is_default_light_enable_ = true;
		bool			is_depth_test_enable_ = true;
		bool			is_write_depth_buffer_enable_ = true;
		int				dxlib_blend_mode_ = DX_BLENDMODE_NOBLEND;
		int				dxlib_culling_mode_ = DX_CULLING_NONE;
		int				dxlib_sample_filter_mode_ = DX_DRAWMODE_BILINEAR;
		MATERIALPARAM	dxlib_mtrl_;

	};

}
