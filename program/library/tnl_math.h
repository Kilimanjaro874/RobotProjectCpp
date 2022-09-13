#pragma once

#include <vector>
#include "tnl_vector.h"

namespace tnl {

	//----------------------------------------------------------------------------------------------
	// �~����
	const float PI = 3.14159265358979f;

	//----------------------------------------------------------------------------------------------
	// ���W�A������f�O���[�ւ̕ϊ�
	inline float ToDegree(float radian) { return (radian / PI * 180.0f); }

	//----------------------------------------------------------------------------------------------
	// �f�O���[���烉�W�A���ւ̕ϊ�
	inline float ToRadian(float degree) { return (PI / 180.0f * degree); }

	//----------------------------------------------------------------------------------------------
	// �����Z���k�c�C�X�^�[�����V�[�h�ݒ�
	void SetSeedMersenneTwister32(int seed);	
	//----------------------------------------------------------------------------------------------
	// �������������̎擾
	// tips... ���̊֐����g�p����ꍇ SetSeedMersenneTwister32 �ŗ����V�[�h��ݒ肵�Ă�������
	float GetRandomDistributionFloat(float min, float max);

	//=============================================================================================
	// name... GetSidesPointAndPlane
	// work... �w����W�����ʂ̕\���ǂ���ɑ��݂��邩���肷��
	// arg1... �w����W
	// arg2... ���ʂ̖@��
	// arg3... ���ʏ�ɑ��݂���ǂ����̍��W
	// ret.... �\...1 	��...-1		���ʏ�...0
	// tips... none
	//=============================================================================================
	int GetSidesPointAndPlane(const Vector3& v, const Vector3& pn, const Vector3& pv);

	//=============================================================================================
	// name... GetRegionPointAndRect
	// work... ��`�̒��S����Q�{�̖����ɐL�т�Ίp������������
	//         �w�肵�����W���Ίp������㉺���E�ǂ��ɑ��݂��邩�擾����
	// arg1... �w����W
	// arg2... ��`�̒��S���W
	// arg3... ��`�̕�
	// arg4... ��`�̍���
	// ret... [�� 0] [�E 1] [�� 2] [�� 3]
	//=============================================================================================
	int GetRegionPointAndRect(const Vector3& p, const Vector3& rp, const int rect_w, const int rect_h);


	//=============================================================================================
	// name... GetXzRegionPointAndOBB
	// work... OBB �̃��[�J����Ԃɑ΂���Y���ɐ����ȕ��ʂ� OBB �̑Ίp�ɂQ�� �N���X������
	//         �w�肵�����W���Ίp�ʂ���O�㍶�E�ǂ��ɑ��݂��邩�擾����
	// arg1... �w����W
	// arg2... OBB �̒��S���W
	// arg3... OBB �̕�, ����, ���s
	// arg4... OBB �̎p��
	// ret... [�O 0] [�E 1] [�� 2] [�� 3]
	//=============================================================================================
	int GetXzRegionPointAndOBB(const Vector3& p, const Vector3& op, const Vector3& size, const Quaternion& q);


	//=============================================================================================
	// name... GetNearestRectPoint
	// work... �_�Ƌ�`�̋�`���ɑ��݂���_�ւ̍ŋߐړ_���擾
	// arg1... ��`���W
	// arg2... ��`�̕�
	// arg3... ��`�̍���
	// arg4... �_���W
	// ret... �ŋߐړ_
	//=============================================================================================
	tnl::Vector3 GetNearestRectPoint(const tnl::Vector3& rect_pos, float w, float h, const tnl::Vector3 &point);


	//=============================================================================================
	// work... �_�ƕ��ʂ̕��ʏ�ɑ��݂���ŋߓ_
	// arg2... �_���W
	// arg3... ���ʂ̖@��
	// arg4... ���ʏ�̍��W
	//=============================================================================================
	tnl::Vector3 GetNearestPointPlane(const tnl::Vector3& v, const tnl::Vector3& pn, const tnl::Vector3& pv);


	//----------------------------------------------------------------------------------------------
	// �x�W�F�X�v���C�����
	// arg1... ��Ԏn�_
	// arg2... anker
	// arg3... anker
	// arg4... ��ԏI�_
	// arg5... 0.0f �` 1.0f
	// ret.... ��ԍ��W
	Vector3 BezierSpline(const Vector3& _a1, const Vector3& _a2, const Vector3& _a3, const Vector3& _a4, float t);


	//----------------------------------------------------------------------------------------------
	//
	// �O���X�v���C�����
	//
	class CubicSpline final {
	private:
		std::vector<tnl::Vector3> a_;
		std::vector<tnl::Vector3> b_;
		std::vector<tnl::Vector3> c_;
		std::vector<tnl::Vector3> d_;
		std::vector<tnl::Vector3> w_;

	public:
		CubicSpline() {};
		CubicSpline(const std::vector<tnl::Vector3>& v);

		//---------------------------------------------------------------------------
		// ��ԍ��W�̎擾
		// arg1... 0.0f �` 1.0f
		// ret.... ��ԍ��W
		inline tnl::Vector3 getPosition(float t) {
			float rt = float(a_.size() - 1) * t;
			int p = int(floor(rt));
			float dt = rt - p;
			return a_[p] + (b_[p] + (c_[p] + d_[p] * dt) * dt) * dt;
		}

	};

	//----------------------------------------------------------------------------------------------
	//
	// ���W�Q���`���
	//
	class PointsLerp final {
	private:
		float all_length_ = 0;
		std::vector< tnl::Vector3 > points_;
		std::vector< float > section_lengths_;
		float getLengthRate(float t) const noexcept;
		float getLengthPoint(int pn) const noexcept;
		int getPrevPoint(float t) const noexcept;

	public:

		PointsLerp(const std::vector< tnl::Vector3 >& points);

		//---------------------------------------------------------------------------
		// ��ԍ��W�̎擾
		// arg1... 0.0f �` 1.0f
		// ret.... ��ԍ��W
		inline tnl::Vector3 getPosition(const float t) {
			int n = getPrevPoint(t);
			float len = getLengthRate(t) - getLengthPoint(n);
			tnl::Vector3 v = tnl::Vector3::Normalize((points_[n + 1] - points_[n]));
			return points_[n] + (v * len);
		}

	};

}