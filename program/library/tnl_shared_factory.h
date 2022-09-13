#pragma once
#include <any>
#include <memory>

namespace tnl {

	//------------------------------------------------------------------------------------------
	//
	// std::shared_ptr での動的生成を強制するファクトリクラス
	//  
	// tips: shared_from_this はコンストラクタで呼び出してはいけないので
	//       代わりとして shared_from_this_constructor を提供
	//       派生クラスではこれらの仮想関数を shared_from_this が使えるコンストラクタとして使用
	//

	template< class T >
	class SharedFactory : public std::enable_shared_from_this<T> {
	private :

		// new の使用を禁止
		static void* operator new(size_t i) { return _mm_malloc(i, 16); }

	protected : 
		SharedFactory() {}

		//-----------------------------------------------------------------------------------
		// 生成された後に付随して実行される仮想関数 (引数なし)
		virtual void shared_from_this_constructor() {}

		//-----------------------------------------------------------------------------------
		// 生成された後に付随して実行される仮想関数
		// tips.. 引数は create で指定された引数が渡される
		virtual void shared_from_this_constructor(std::any d) {}

	public :
		static void operator delete(void* p) { _mm_free(p); }
		virtual ~SharedFactory() {}

		//-----------------------------------------------------------------------------------
		// 引数なしの生成関数
		template< class U >
		static std::shared_ptr<U> Create() {
			std::shared_ptr<U> ptr = std::make_shared<U>();
			ptr->shared_from_this_constructor();
			return ptr;
		}

		//-----------------------------------------------------------------------------------
		// 引数付きの生成関数
		template< class U, class Desc >
		static std::shared_ptr<U> Create( Desc desc ) {
			std::shared_ptr<U> ptr = std::shared_ptr<U>( new U() );
			ptr->shared_from_this_constructor(desc);
			return ptr;
		}

	};

}
