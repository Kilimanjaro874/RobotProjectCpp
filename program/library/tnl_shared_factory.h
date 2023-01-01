#pragma once
#include <any>
#include <memory>

namespace tnl {

	//------------------------------------------------------------------------------------------
	//
	// std::shared_ptr �ł̓��I��������������t�@�N�g���N���X
	//  
	// tips: shared_from_this �̓R���X�g���N�^�ŌĂяo���Ă͂����Ȃ��̂�
	//       ����Ƃ��� shared_from_this_constructor ���
	//       �h���N���X�ł͂����̉��z�֐��� shared_from_this ���g����R���X�g���N�^�Ƃ��Ďg�p
	//

	template< class T >
	class SharedFactory : public std::enable_shared_from_this<T> {
	private :

		// new �̎g�p���֎~
		static void* operator new(size_t i) { return _mm_malloc(i, 16); }

	protected : 
		SharedFactory() {}

		//-----------------------------------------------------------------------------------
		// �������ꂽ��ɕt�����Ď��s����鉼�z�֐� (�����Ȃ�)
		virtual void shared_from_this_constructor() {}

		//-----------------------------------------------------------------------------------
		// �������ꂽ��ɕt�����Ď��s����鉼�z�֐�
		// tips.. ������ create �Ŏw�肳�ꂽ�������n�����
		virtual void shared_from_this_constructor(std::any d) {}

	public :
		static void operator delete(void* p) { _mm_free(p); }
		virtual ~SharedFactory() {}

		//-----------------------------------------------------------------------------------
		// �����Ȃ��̐����֐�
		template< class U >
		static std::shared_ptr<U> Create() {
			std::shared_ptr<U> ptr = std::make_shared<U>();
			ptr->shared_from_this_constructor();
			return ptr;
		}

		//-----------------------------------------------------------------------------------
		// �����t���̐����֐�
		template< class U, class Desc >
		static std::shared_ptr<U> Create( Desc desc ) {
			std::shared_ptr<U> ptr = std::shared_ptr<U>( new U() );
			ptr->shared_from_this_constructor(desc);
			return ptr;
		}

	};

}
