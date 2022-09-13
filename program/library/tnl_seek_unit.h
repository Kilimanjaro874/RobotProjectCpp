#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include "tnl_using.h"
#include "tnl_shared_factory.h"

namespace tnl {

	class SeekUnit : public tnl::SharedFactory<SeekUnit> {
	public:
		SeekUnit(){}

		virtual ~SeekUnit() {}

		enum class ePlayMode {
			SINGLE,		// �P���Đ�
			REPEAT,		// ���s�[�g�Đ�
			REFLECTION	// ���]�Đ�( ���s�[�g )
		};
		enum class ePlayDir {
			FORWARD,	// �ʏ�
			REVERSE		// �t�Đ�
		};
		enum class eFrameType {
			PREV,		// �O�̃t���[��
			CURRENT,	// ���݂̃t���[��
			NEXT		// ���̃t���[��
		};

		SeekUnit(const double fix_fps
			, const double total_time
			, uint32_t total_frame_num
			, const ePlayMode& play_mode
			, const ePlayDir& play_dir) {
			fix_fps_ = fix_fps;
			setTotalTime(total_time);
			setTotalFrameNum(total_frame_num);
			setPlayState(play_mode, play_dir);
		}

		//-----------------------------------------------------------------------------------------------------------------
		// arg1... �Œ�t���[�����[�g
		// arg2... �S�̂̍Đ�����
		// arg3... ���t���[����
		// arg4... ePlayMode
		// arg5... ePlayDir
		// tips... ���t���[������ 0 ��ݒ肷��ƃt���[���Ǘ��ł͂Ȃ����Ԃ̊����ŊǗ��������̂Ƒz�肵�܂�
		//         ���̂��ߑ��t���[������ 0 ��ݒ肵���ꍇ ���\�b�h���� Frame �ƕt���Ă�����͔̂񐄏��ł�
		static Shared<SeekUnit> Create(
			const double fix_fps
			, const double total_time
			, uint32_t total_frame_num
			, const ePlayMode& play_mode
			, const ePlayDir& play_dir) {
			Shared<SeekUnit> unit = std::make_shared<SeekUnit>(
				fix_fps
				, total_time
				, total_frame_num
				, play_mode
				, play_dir );
			return unit;
		}

		// �Đ�( ��~�����ꏊ����Đ� )
		inline void play() { is_playing_ = true; }
		// ��~
		inline void stop() { is_playing_ = false; }

		// ���X�^�[�g( �n�߂���Đ� )
		void restart();

		// �V�[�N�ꏊ�� 0 �` 1.0 �̊����Ŏw�肵���ꏊ�֔�΂�
		void jumpSeekRate(const double seek_rate);

		// �V�[�N�ꏊ�� �t���[���ԍ� �Őݒ肵���ꏊ�֔�΂�
		void jumpSeekFrame(uint32_t frame);

		// �A�b�v�f�[�g����( ���t���[�����s )
		void update(const double delta_time);


		//-------------------------------------------------------------------------------------------------------
		//
		// setter
		//
		// 
		// �Đ��X�e�[�g�ݒ�
		void setPlayState(const ePlayMode& play_mode, const ePlayDir& play_dir);

		// �S�̂̃X�s�[�h��ݒ�
		inline void setTimeScale(const double time_scale) { time_scale_ = time_scale; }

		// �t���[���̍Đ��J�n���ɂP�x�������s�����R�[���o�b�N��ݒ�
		// arg1... �t���[���ԍ�
		// arg2... �R�[���o�b�N
		void setFrameTriggerFunction(const uint32_t& frame, std::function<void()> func);

		//-------------------------------------------------------------------------------------------------------
		//
		// getter
		//
		inline uint32_t getTotalFrameNum() { return total_frame_num_; }

		// ���݂̑S�̂̃V�[�N���Ԃ̊����� 0 �` 1.0 �ŕԂ�
		inline double getSeekRate() { return seek_position_ / total_time_; }

		// ���݂̃t���[���̃t���[��������̍Đ����Ԃ̊���
		double getSeekRateInFrame();

		// �t���[���ԍ����擾
		// arg1... eFrameType
		uint32_t getSeekFrame(const eFrameType& e_frame);

	private:
		bool is_playing_ = false;
		double fix_fps_ = 60.0f;
		double time_scale_ = 1.0f;
		double total_time_ = 0;
		double seek_position_ = 0;
		int32_t direction_ = 1;
		uint32_t total_frame_num_ = 0;
		std::function<void()> call_of_trigger_ = nullptr;
		std::vector<std::function<void()>> frame_functions_storage_;
		ePlayMode play_mode_ = ePlayMode::SINGLE;
		ePlayDir play_dir_ = ePlayDir::FORWARD;

		void seekProcess(float delta_time);

		// �S�̂̍Đ����Ԃ�ݒ�
		inline void setTotalTime(const double total_time) { total_time_ = total_time; }

		// ���t���[�����̐ݒ�
		void setTotalFrameNum(const uint32_t frame_num = 0);

	};


}

