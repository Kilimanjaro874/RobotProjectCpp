#pragma once
#include <stack>
#include <functional>

namespace tnl {

/**************************************************************************************************************
* 
*  �g�p�@�T���v��
* 

class Test {
public :
	tnl::Sequence<Test> seq_ = tnl::Sequence<Test>( this, &Test::seq1 );
	bool seq1(const float delta_time);
	bool seq2(const float delta_time);
	bool seq3(const float delta_time);
};

bool Test::seq1(const float delta_time) {
	DrawStringEx(100, 100, -1, "seq1");
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_Z)) seq_.change(&Test::seq2);
	return true;
}
bool Test::seq2(const float delta_time) {
	DrawStringEx(100, 100, -1, "seq2");
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_Z)) seq_.change(&Test::seq3);
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_X)) seq_.back();
	return true;
}
bool Test::seq3(const float delta_time) {
	DrawStringEx(100, 100, -1, "seq3");
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_X)) seq_.back();
	return true;
}

Test* inst = nullptr ;

void gameMain(float delta_time) {

	if (!init) {
		inst = new Test();
		init = true;
	}

	inst->seq_.update(delta_time);

}

*****************************************************************************************************/

	template <class T>
	class Sequence final {
	private:
		T* object_;
		std::function<bool(T*, const float)> now_;
		std::function<bool(T*, const float)> next_;
		std::stack<std::function<bool(T*, const float)>> prevs_;
		bool (T::*p_now_)(const float);
		bool (T::*p_next_)(const float);
		typedef bool (T::*p_prev_)(const float) ;
		std::stack<p_prev_> p_prevs_;
		bool is_start_ = true;
		bool is_change_ = false;
		float sum_time_ = 0;
		Sequence() {}
	public:

		//===================================================================================
		// �R���X�g���N�^
		// arg1... ��܃N���X�� this �|�C���^���w��
		// arg2... �R�[���o�b�N�ŌĂяo��������܃N���X�̃����o���\�b�h�܂��̓����_���w��
		//===================================================================================
		Sequence(T* obj, bool (T::*func)(const float))
			: object_(obj)
			, next_(func)
			, now_(func)
			, p_now_(func)
			, p_next_(func)
		{}

		~Sequence() {}

		//===================================================================================
		// �V�[�P���X�̃A�b�v�f�[�g ( ���t���[���Ăяo����OK )
		// arg1... �t���[���Ԃ̌o�ߎ���( �b�̃f���^�^�C�� )
		//===================================================================================
		inline bool update(const float deltatime) {
			sum_time_ += deltatime;
			bool ret = now_(object_, deltatime);
			if (!is_change_) {
				is_start_ = false;
				return ret;
			}
			now_ = next_;
			p_now_ = p_next_;
			is_start_ = true;
			sum_time_ = 0;
			is_change_ = false;
			return ret;
		}

		//===================================================================================
		// ��r
		//===================================================================================
		inline bool isComparable(bool (T::*func)(const float)) const { return p_now_ == func; }

		//===================================================================================
		// �������p�@�V�[�P���X�̍ŏ��̂P�t���[������ true ���A��
		//===================================================================================
		inline bool isStart() const { return is_start_; }

		//===================================================================================
		// �V�[�P���X�̌o�ߎ��Ԃ��擾 ( �b )
		//===================================================================================
		inline float getProgressTime() const { return sum_time_; }

		//===================================================================================
		// �V�[�P���X�̕ύX
		// arg1... ���̃t���[��������s�������܃N���X�̃��\�b�h���w��
		//===================================================================================
		inline void change(bool (T::*func)(const float)) {
			prevs_.push(now_);
			p_prevs_.push(p_now_);
			next_ = func;
			p_next_ = func;
			is_change_ = true;
		}

		//===================================================================================
		// 1�O�̃V�[�P���X�ɖ߂�
		// tips... �O�̃V�[�P���X�����݂��Ȃ���Ή������Ȃ�
		//===================================================================================
		inline void back() {
			if (prevs_.empty()) return;			
			next_ = prevs_.top();
			p_next_ = p_prevs_.top();
			prevs_.pop();
			p_prevs_.pop();
			is_change_ = true;
		}


		//===================================================================================
		// �V�[�P���X�𑦍��ɕύX
		// arg1... ���s�������܃N���X�̃��\�b�h���w��
		// tisp... ���t���[����҂��������ɃV�[�P���X��ύX����
		//===================================================================================
		inline void immediatelyChange(bool (T::*func)(const float)) {
			prevs_.push(now_);
			p_prevs_.push(p_now_);
			now_ = func;
			p_now_ = func;
			is_start_ = true;
			sum_time_ = 0;
			is_change_ = false;
		}

	};

}

