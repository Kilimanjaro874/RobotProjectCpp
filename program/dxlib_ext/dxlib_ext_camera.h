#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace dxe {

	class Camera {
	public :

		Camera(){}
		Camera(int screen_w, int screen_h) {
			screen_w_ = screen_w;
			screen_h_ = screen_h;
			aspect_ = (float)screen_w_ / (float)screen_h_;
		}

		int screen_w_ = 0 ;
		int screen_h_ = 0 ;

		// �J�����̂R�������W
		tnl::Vector3 pos_ = tnl::Vector3(0, 0, -100.0f);
		// �J�������R�����̂ǂ�����ʂ̒����ɂƂ炦�Ă��邩
		tnl::Vector3 target_ = tnl::Vector3(0, 0, 0);
		// �J�����́w��x���`����A�b�p�[�x�N�g��
		tnl::Vector3 up_ = tnl::Vector3(0, 1, 0);

		// �J�����̉�p ( �x )
		float angle_ = tnl::ToRadian(60.0f);
		// �J�����̃A�X�y�N�g�� ( �c���� )
		float aspect_ = 1.0f;
		// �J�����ɉf��͈͂̍ŋߋ���
		float near_ = 1.0f;
		// �J�����ɉf��͈͂̍ŉ�����
		float far_ = 5000.0f;

		tnl::Matrix view_;
		tnl::Matrix proj_;

		void update() {
			view_ = tnl::Matrix::LookAtLH(pos_, target_, up_);
			proj_ = tnl::Matrix::PerspectiveFovLH(angle_, aspect_, near_, far_);
		}

		inline tnl::Vector3 front() { return tnl::Vector3::Normalize(target_ - pos_); }
		inline tnl::Vector3 left() { return tnl::Vector3::Cross(front(), { 0, 1, 0 }); }
		inline tnl::Vector3 right() { return tnl::Vector3::Cross({ 0, 1, 0 }, front()); }
		inline tnl::Vector3 back() { return tnl::Vector3::Normalize(pos_ - target_); }

	};

}
