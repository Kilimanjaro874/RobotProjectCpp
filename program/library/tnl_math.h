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
	// ���W�ƃT�C�Y���� AABB �� max min ���擾
	inline tnl::Vector3 ToMaxAABB(const tnl::Vector3& pos, const tnl::Vector3& size) { return pos + (size * 0.5f); }
	inline tnl::Vector3 ToMinAABB(const tnl::Vector3& pos, const tnl::Vector3& size) { return pos - (size * 0.5f); }

	//----------------------------------------------------------------------------------------------
	// �����Z���k�c�C�X�^�[�����V�[�h�ݒ�
	void SetSeedMersenneTwister32(int seed);	
	//----------------------------------------------------------------------------------------------
	// �������������̎擾
	// tips... ���̊֐����g�p����ꍇ SetSeedMersenneTwister32 �ŗ����V�[�h��ݒ肵�Ă�������
	float GetRandomDistributionFloat(float min, float max);

	//=============================================================================================
	// work... �w����W�����ʂ̕\���ǂ���ɑ��݂��邩���肷��
	// arg1... �w����W
	// arg2... ���ʂ̖@��
	// arg3... ���ʏ�ɑ��݂���ǂ����̍��W
	// ret.... �\...1 	��...-1		���ʏ�...0
	// tips... none
	//=============================================================================================
	int GetSidesPointAndPlane(const Vector3& v, const Vector3& pn, const Vector3& pv);

	//=============================================================================================
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
	// work... AABB�̒��S����S�Ă̑Ίp�ӂ֖����ɐL�т镽�ʂ��l�����Ƃ�
	//         �w�肵�����W���Ίp�ӗ̈�̏㉺���E�O��ǂ��ɑ��݂��邩�擾����
	// arg1... �w����W
	// arg2... AABB max
	// arg3... AABB min
	// ret... [�� 0] [�E 1] [�� 2] [�� 3] [ �O 4 ] [ �� 5 ]
	//=============================================================================================
	int GetRegionPointAndAABB( const Vector3& p, const Vector3& max, const Vector3& min );

	//=============================================================================================
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
	// work... �_��AABB��AABB���ɑ��݂���_�ւ̍ŋߐړ_���擾
	// arg1... �_���W
	// arg2... AABB max
	// arg3... AABB min
	// ret... �ŋߐړ_
	//=============================================================================================
	tnl::Vector3 GetNearestPointAABB(const tnl::Vector3& point, const tnl::Vector3& aabb_max, const tnl::Vector3& aabb_min );

	//=============================================================================================
	// name... GetNearestRectPoint
	// work... �_�Ƌ�`�̋�`���ɑ��݂���_�ւ̍ŋߐړ_���擾
	// arg1... ��`���W
	// arg2... ��`�̕�
	// arg3... ��`�̍���
	// arg4... �_���W
	// ret... �ŋߐړ_( 2D )
	//=============================================================================================
	tnl::Vector3 GetNearestRectPoint(const tnl::Vector3& rect_pos, float w, float h, const tnl::Vector3 &point);

	//=============================================================================================
	// name... GetNearestRectPoint
	// work... �_�ƒ����`�̒����`���ɑ��݂���_�ւ̍ŋߐړ_���擾
	// arg1... �����`�̒��S
	// arg2... �����`��X���̒P�ʃx�N�g��
	// arg3... �����`��Y���̒P�ʃx�N�g��
	// arg4... �����`�̕ӂ̒����̔���
	// arg5... �_���W
	// ret... �ŋߐړ_( 3D )
	//=============================================================================================
	tnl::Vector3 GetNearestRectPoint(const tnl::Vector3& rect_pos, const tnl::Vector3& axis_x, const tnl::Vector3& axis_y, const tnl::Vector3& rect_side_length, const tnl::Vector3& point);

	//=============================================================================================
	// work... �_�ƎO�p�`�̎O�p�`��ɑ��݂���ŋߓ_
	// arg2... �_���W
	// arg345. �O�p�`�̒��_���W
	// arg4... �O�p�`��̍��W
	//=============================================================================================
	tnl::Vector3 GetNearestPointTriangle(const tnl::Vector3& p, const tnl::Vector3& a, const tnl::Vector3& b, const tnl::Vector3& c);

	//=============================================================================================
	// work... �_�ƕ��ʂ̕��ʏ�ɑ��݂���ŋߓ_
	// arg2... �_���W
	// arg3... ���ʂ̖@��
	// arg4... ���ʏ�̍��W
	//=============================================================================================
	tnl::Vector3 GetNearestPointPlane(const tnl::Vector3& v, const tnl::Vector3& pn, const tnl::Vector3& pv);


	//=============================================================================================
	// work... ���� AABB (A) �Ɠ����Ȃ� AABB (B) ���Փ˂������� A �̕␳���W���擾����
	// arg1... �ړ��O�� A ���W
	// arg2... A �̃T�C�Y
	// arg3... B �̍��W
	// arg4... B �̃T�C�Y
	// arg5... �␳���W
	// arg6... �␳�X�y�[�X ( �f�t�H���g 0.1f )
	// ret... [���ɕ␳ 0] [�E�ɕ␳ 1] [��ɕ␳ 2] [���ɕ␳ 3] [ �O�ɕ␳ 4 ] [ ��ɕ␳ 5 ]
	//=============================================================================================
	int GetCorrectPositionIntersectAABB( const tnl::Vector3& a_prev, const tnl::Vector3& a_size, const tnl::Vector3& b, const tnl::Vector3& b_size, tnl::Vector3& out, const float correct_space = 0.1f);

	//=============================================================================================
	// �x�W�F�X�v���C�����
	// arg1... ��Ԏn�_
	// arg2... anker
	// arg3... anker
	// arg4... ��ԏI�_
	// arg5... 0.0f �` 1.0f
	// ret.... ��ԍ��W
	//=============================================================================================
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