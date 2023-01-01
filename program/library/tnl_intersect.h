#pragma once
#include "tnl_matrix.h"

namespace tnl {

	//-----------------------------------------------------------------------------------------------------
	// �_�Ƌ�`(�����`)�̂̏Փˌ��m
	// arg1...�_�̂����W
	// arg2...�_�̂����W
	// arg3...��`�̒��S�����W
	// arg4...��`�̒��S�����W
	// arg5...��`�̃T�C�Y
	// ret....[ �Փ˂��Ă��� : true ]   [ �Փ˂��Ă��Ȃ� : false ]
	bool IsIntersectPointRect(const int point_x, const int point_y, const int rect_x, const int rect_y, const int rect_size);

	//-----------------------------------------------------------------------------------------------------
	// ��`�Ƌ�`�̏Փˌ��m ����`�̍���E�E���̒��_���W�w��
	// arg1...��`A�̍��̂����W
	// arg2...��`A�̉E�̂����W
	// arg3...��`A�̏�̂����W
	// arg4...��`A�̉��̂����W
	// arg5...��`B�̍��̂����W
	// arg6...��`B�̉E�̂����W
	// arg7...��`B�̏�̂����W
	// arg8...��`B�̉��̂����W
	// ret....[ �Փ˂��Ă��� : true ]   [ �Փ˂��Ă��Ȃ� : false ]
	bool IsIntersectRectPrimitive(const int a_left, const int a_right, const int a_top, const int a_bottom,
		const int b_left, const int b_right, const int b_top, const int b_bottom);

	//-----------------------------------------------------------------------------------------------------
	// ��`�Ƌ�`�̏Փˌ��m�@����`�̒��S�ƕ������w��
	// arg1...��`A�̂����W
	// arg2...��`A�̂����W
	// arg3...��`A�̕�
	// arg4...��`A�̍���
	// arg5...��`B�̂����W
	// arg6...��`B�̂����W
	// arg7...��`B�̕�
	// arg8...��`B�̍���
	// ret....[ �Փ˂��Ă��� : true ]   [ �Փ˂��Ă��Ȃ� : false ]
	bool IsIntersectRect(const int a_rect_x, const int a_rect_y, const int a_rect_size_w, const int a_rect_size_h,
		const int b_rect_x, const int b_rect_y, const int b_rect_size_w, const int b_rect_size_h);

	//-----------------------------------------------------------------------------------------------------
	// ��`�Ƌ�`�̏Փˌ��m�@����`�̒��S�ƕ������w��
	// arg1...��`A�̍��W
	// arg2...��`A�̕�
	// arg3...��`A�̍���
	// arg4...��`B�̍��W
	// arg5...��`B�̕�
	// arg6...��`B�̍���
	// ret....[ �Փ˂��Ă��� : true ]   [ �Փ˂��Ă��Ȃ� : false ]
	inline bool IsIntersectRect(const tnl::Vector3& a, const int a_rect_size_w, const int a_rect_size_h,
		const tnl::Vector3& b, const int b_rect_size_w, const int b_rect_size_h) {
		return IsIntersectRect((int)a.x, (int)a.y, a_rect_size_w, a_rect_size_h, (int)b.x, (int)b.y, b_rect_size_w, b_rect_size_h);
	}
 
	//-----------------------------------------------------------------------------------------------------
	// ��`�Ƌ�`�̏Փˌ��m & ���W�␳
	// arg1...��`A�̌��ݍ��W( �␳�Ώۂ̍��W�ϐ� )
	// arg2...��`A�̈ړ��O�̍��W
	// arg3...��`A�̕�
	// arg4...��`A�̍���
	// arg5...��`B�̍��W
	// arg6...��`B�̕�
	// arg7...��`B�̍���
	// arg8...�␳�̒ǉ��X�y�[�X( �ȗ��� �f�t�H���g 1.0f )
	// ret....[ �Փ˂��Ă��Ȃ� : 0 ] 
	//        [ �Ώۂ����ɕ␳ : 1 ] 
	//        [ �Ώۂ��E�ɕ␳ : 2 ]
	//        [ �Ώۂ���ɕ␳ : 3 ]
	//        [ �Ώۂ����ɕ␳ : 4 ]
	// tips...��`A���␳�Ώۂ̃I�u�W�F�N�g
	//        ��`B�͕Ǔ��̈ړ����Ȃ��I�u�W�F�N�g��z�肵�Ă��܂�
	int IsIntersectRectToCorrectPosition( tnl::Vector3& a_now, const tnl::Vector3 &a_prev, const int a_rect_size_w, const int a_rect_size_h,
		const tnl::Vector3& b, const int b_rect_size_w, const int b_rect_size_h, const float correct_space = 1.0f);


	//----------------------------------------------------------------------------------------------
	// work... 2D��̐����̔���
	// arg1... ����A�̎n�_
	// arg2... ����A�̏I�_
	// arg3... ����B�̎n�_
	// arg4... ����B�̏I�_
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	// tips... none
	bool IsIntersectLine2D(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4);

	//----------------------------------------------------------------------------------------------
	// work... �O�p�`�Ɛ����̏Փ˔���
	// arg1-3. �O�p�`���\�����钸�_
	// arg5-5. �n�_�ƏI�_ 
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	// tips... 2D �p
	bool IsIntersectLineTriangle2D(const Vector3& _v1, const Vector3& _v2, const Vector3& _v3, const Vector3& _s, const Vector3& _e);


	//-----------------------------------------------------------------------------------------------------
	// ���Ƌ��̏Փˌ��m
	// arg1...�~A�̒��S���W
	// arg2...�~A�̔��a
	// arg3...�~B�̒��S���W
	// arg4...�~B�̔��a
	// ret....[ �Փ˂��Ă��� : true ]   [ �Փ˂��Ă��Ȃ� : false ]
	bool IsIntersectSphere(const tnl::Vector3& a, float a_radius, const tnl::Vector3& b, float b_radius);

	//----------------------------------------------------------------------------------------------
	// work... �����ƕ��ʂ̏Փ˔���
	// arg1... �����̎n�_
	// arg2... �����̏I�_
	// arg3... ���ʏ�̍��W ( 0, 0, 0 �ȊO )
	// arg4... ���ʂ̖@��
	// arg5... �Փ˓_�̎󂯎��p( �K�v�Ȃ���Ώȗ��� )
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	// tips... �Փ˂��Ă��Ȃ��ꍇ arg5 �œn���������͕ω��Ȃ�
	bool IsIntersectLinePlane(const tnl::Vector3 &line_start, const tnl::Vector3 &line_end,
		const tnl::Vector3 &plane_pos, const tnl::Vector3 &plane_normal,
		tnl::Vector3* intersect_position = nullptr);

	//----------------------------------------------------------------------------------------------
	// work... �O�p�`�Ɠ_�̔���
	// arg1-3. �O�p�`�̒��_���W
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	// tips... none
	bool IsIntersectTrianglePoint(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& p);

	//----------------------------------------------------------------------------------------------
	// work... �O�p�`�Ɛ����̏Փ˔���
	// arg1-3. �O�p�`���\�����钸�_
	// arg2-4. �n�_�ƏI�_ 
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	// tips... 3D �p
	bool IsIntersectLineTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& s, const Vector3& e);

	//----------------------------------------------------------------------------------------------
	// work... ���C��OBB �̏Փ˔���
	// arg1... ���C��̍��W ( �����Ȃ�n�_ )
	// arg2... ���C�̕����x�N�g��
	// arg3... �{�b�N�X��AABB �Ƃ������� ������̍��W
	// arg4... �{�b�N�X��AABB �Ƃ������� �E�O���̍��W
	// arg5... OBB �̉�]�s��
	// arg7... ���Z���ʂ̌�_( �ȗ��� )
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	bool IsIntersectRayOBB( const Vector3& pos, const Vector3& dir, const Vector3& aabb_max, const Vector3& aabb_min, const Matrix& obb_rot, Vector3& intersect_pos );

	//----------------------------------------------------------------------------------------------
	// work... ������OBB �̏Փ˔���
	bool IsIntersectLineOBB(const Vector3& s, const Vector3& e, const Vector3& aabb_max, const Vector3& aabb_min, const Matrix& obb_rot, Vector3& intersect_pos );

}
