#pragma once
#include "tnl_sequence.h"

namespace tnl {

/*
//
//  �g�p�@�T���v��
//

class Test {
public:
	Test() {
		pos_ = point[check_point_now];
		tnl::Vector3 goal = point[getNextPoint()];
		tnl::Vector3 v = goal - pos_;
		tf_move.setMethodBrake(&pos_, goal, tnl::Vector3::Normalize(v), v.length(), 2.0f);
	}

	static constexpr int POINT_MAX = 3;
	tnl::Vector3 point[POINT_MAX] = {
		{100, 100, 0},
		{500, 500, 0},
		{800, 100, 0}
	};
	int check_point_now = 0;
	tnl::Vector3 pos_;
	tnl::TimerFluct<tnl::Vector3> tf_move;

	int getNextPoint() { return (check_point_now + 1) % POINT_MAX; }
	int updatePoint() { return ++check_point_now %= POINT_MAX; }

	void update(float deltatime) {
		if (tf_move.isCompleteUpdate(deltatime)) {
			updatePoint();
			pos_ = point[check_point_now];
			tnl::Vector3 goal = point[getNextPoint()];
			tnl::Vector3 v = goal - pos_;
			tf_move.setMethodBrake(&pos_, goal, tnl::Vector3::Normalize(v), v.length(), 2.0f);
		}
	}
	void render() {
		DrawCircle(pos_.x, pos_.y, 20, -1);
	}
};

bool init = false;
Test t;

void gameMain(float deltatime) {

	if (!init) {
		init = true;
	}

	t.update(deltatime);
	t.render();

}



*/


	//------------------------------------------------------------------------------------------------------------
	//
	// ���Ԍo�߂ł̐��l�ω�
	// tips... ���̃N���X���܂����N���X�̓|�C���^�Ő������邱��
	//

    template< class T >
    class TimerFluct final {
	private:
		T start_ = T();
		T complete_ = T();
		T to_complete_normalize_ = T();
		T* origin_ = nullptr;
		float distance_ = 0;
		float fluctuation_ = 0;
		float complete_time_ = 0;
		float trigonometric_power_ = 1.0f;
		Sequence<TimerFluct<T>> sequence_ = Sequence<TimerFluct<T>>( this, &TimerFluct<T>::seqIdle ) ;


		// ���ׂĂ̏����ŋ��ʂ�������Z�b�g
		inline void setCommonArgument(T* origin, const T& complete, const float complete_time) {
			origin_ = origin;
			complete_ = complete;
			complete_time_ = complete_time;
		}

		// ���ׂĂ̏����ŋ��ʂ���L�q
		inline bool seqCommonDescription() {
			if (sequence_.isStart()) { start_ = *origin_; }
			if (sequence_.getProgressTime() >= complete_time_) {
				*origin_ = complete_;
				return true;
			}
			return false;
		}

		// Accel �� Brake �̋��ʋL�q
		inline void setAccelBrakeDescription(const T& to_complete_normalize, const float distance) {
			distance_ = distance;
			to_complete_normalize_ = to_complete_normalize;
			// ���R�����̌����𗘗p���ĉ����x���v�Z 
			// ���� t = sqrt( 2h / g )
			// �ό`���� g = 2h / t^2
			// [ t:����  h:����  g:�d�͉����x ]
			fluctuation_ = (2.0f * distance_) / powf(complete_time_, 2.0f);
		}

		// �ҋ@
		inline bool seqIdle(const float deltatime) { return true; }

		// ���������^��
		inline bool seqLinear(const float deltatime) {
			if (seqCommonDescription()) {
				sequence_.change(&TimerFluct::seqIdle);
				return true;
			}
			T v = static_cast<T>( (complete_ - start_) / complete_time_ * sequence_.getProgressTime() ) ;
			*origin_ = start_ + v ;
			return false;
		}

		// �����������^��
		inline bool seqAccel(const float deltatime) {
			if (seqCommonDescription()) {
				sequence_.change(&TimerFluct::seqIdle);
				return true;
			}
			// �����������^���̌��� x = x0 + ( v0 * t ) + ( 1/2 * a * t^2 )
			// ( v0 * t ) �͏�����������͎g��Ȃ��̂ŏȗ�
			// [ x:���݈ʒu  x0:�����ʒu  t:����  a:�����x ]
			float a = fluctuation_;
			float t = sequence_.getProgressTime();
			T vn = to_complete_normalize_;
			*origin_ = start_ + vn * ( ( a * powf( t, 2 ) ) * 0.5f ) ;
			return false;
		}

		// �����������^��
		inline bool seqBrake(const float deltatime) {
			if (seqCommonDescription()) {
				sequence_.change( &TimerFluct::seqIdle );
				return true;
			}
			float a = fluctuation_;
			float t = sequence_.getProgressTime();
			T vn = to_complete_normalize_;
			*origin_ = complete_ - vn * ((a * powf(complete_time_ - t, 2)) * 0.5f);
			return false;
		}

		// sin�g 0 �` �΂̉^��
		inline bool seqSin(const float deltatime) {
			seqCommonDescription();
			T v = static_cast<T>(complete_ - start_) ;
			float t = sequence_.getProgressTime() / complete_time_ ;
			float s = sin( toRadian(t * 180.0f) ) ;
			float trigonometric = powf(s,trigonometric_power_) ;
			*origin_ = start_ + v * trigonometric ;
			return false;
		}

		// cos�g 0 �` �΂̉^��
		inline bool seqCos(const float deltatime) {
			seqCommonDescription();
			T v = static_cast<T>(complete_ - start_);
			float t = sequence_.getProgressTime() / complete_time_;
			float c = cos(toRadian(t * 180.0f));
			float trigonometric = powf(c, trigonometric_power_);
			*origin_ = start_ + v * trigonometric;
			return false;
		}

	public :
		TimerFluct<T>(){}
		~TimerFluct<T>(){}

		//==============================================================================================================
		// ���������^����ݒ�
		// arg1... �ϓ�������C���X�^���X�ւ̃|�C���^
		// arg2... �����l
		// arg3... �������� (�b)
		//==============================================================================================================
		inline void setMethodLinear(T* origin, const T& complete, const float complete_time){
			setCommonArgument(origin, complete, complete_time);
			sequence_.immediatelyChange(&TimerFluct<T>::seqLinear);
		}

		//==============================================================================================================
		// �����������^����ݒ�
		// arg1... �ϓ�������C���X�^���X�ւ̃|�C���^
		// arg2... �����l
		// arg3... arg1 -> arg2 �ւ̐��K���l
		// arg4... arg1 -> arg2 �ւ̋���
		// arg5... �������� (�b)
		//==============================================================================================================
		inline void setMethodAccel(T* origin, const T& complete, const T& to_complete_normalize, const float distance, const float complete_time) {
			setCommonArgument(origin, complete, complete_time);
			setAccelBrakeDescription(to_complete_normalize, distance);
			sequence_.immediatelyChange(&TimerFluct<T>::seqAccel);
		}

		//==============================================================================================================
		// �����������^����ݒ�
		// arg1... �ϓ�������C���X�^���X�ւ̃|�C���^
		// arg2... �����l
		// arg3... arg1 -> arg2 �ւ̐��K���l
		// arg4... arg1 -> arg2 �̋���
		// arg5... �������� (�b)
		//==============================================================================================================
		inline void setMethodBrake(T* origin, const T& complete, const T& to_complete_normalize, const float distance, const float complete_time) {
			setCommonArgument(origin, complete, complete_time);
			setAccelBrakeDescription(to_complete_normalize, distance);
			sequence_.immediatelyChange(&TimerFluct<T>::seqBrake);
		}

		//==============================================================================================================
		// sin�g 0 �` �΂̉^����ݒ�
		// arg1... �ϓ�������C���X�^���X�ւ̃|�C���^
		// arg2... �����l
		// arg3... �������� (�b)
		// arg4... �O�p��̏搔 �ȗ���
		//==============================================================================================================
		inline void setMethodSin(T* origin, const T& complete, const float complete_time, const float trigonometric_power = 1.0f) {
			trigonometric_power_ = trigonometric_power;
			setCommonArgument(origin, complete, complete_time);
			sequence_.immediatelyChange(&TimerFluct<T>::seqSin);
		}

		//==============================================================================================================
		// cos�g 0 �` �΂̉^����ݒ�
		// arg1... �ϓ�������C���X�^���X�ւ̃|�C���^
		// arg2... �����l
		// arg3... �������� (�b)
		// arg4... �O�p��̏搔 �ȗ���
		//==============================================================================================================
		inline void setMethodCos(T* origin, const T& complete, const float complete_time, const float trigonometric_power = 1.0f) {
			trigonometric_power_ = trigonometric_power;
			setCommonArgument(origin, complete, complete_time);
			sequence_.immediatelyChange(&TimerFluct<T>::seqCos);
		}

		//==============================================================================================================
		// ���l�ϓ��̍X�V
		// arg1... �t���[���Ԃ̃f���^�^�C��
		// ret.... [ true : �ϓ����� ] [ false : �ϓ��� ]
		//==============================================================================================================
		inline bool isCompleteUpdate( const float deltatime ) {
			return sequence_.update(deltatime);
		}

	};

}

