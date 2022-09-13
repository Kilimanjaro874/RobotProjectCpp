#pragma once
#include <functional>

namespace tnl {

	/*

//
//  使用法サンプル
//


class Test {
public :
	uint32_t count = 0;
	tnl::TimerCallback<Test> tc = tnl::TimerCallback<Test>(this, &Test::call, 1.0f);

	void call(const float time);
};

void Test::call(const float time) {
	count++;
}

Test test;

void gameMain(float delta_time) {

	test.tc.update(delta_time);

	DrawStringEx(100, 100, -1, "%d", test.count );

}

	*/

	//------------------------------------------------------------------------------------------------------------
	//
	// 時間経過でのメソッド呼び出し
	// 
	//

	template <class T>
	class TimerCallback final {
	private:
		T* object_;
		std::function<void(T*, const float)> function_;
		float sum_time_ = 0;
		float regulation_time_ = 0;
		TimerCallback() {}
	public:

		//===================================================================================
		// コンストラクタ
		// arg1... 包含クラスの this ポインタを指定
		// arg2... コールバックで呼び出したい包含クラスのメンバメソッドまたはラムダ式指定
		// arg3... コールバック呼び出しの規定時間( 秒数指定 1.0f で1秒 )
		// arg4... 初回のコールバックのみ即呼び出したければ true を指定  省略可
		//===================================================================================
		TimerCallback( T* obj, const std::function<void(T*, const float)> func, const float regulation_time, const bool is_callback_start = false )
			: regulation_time_(regulation_time)
			, object_(obj)
			, function_(func)
			, sum_time_((is_callback_start) ? regulation_time : 0)
		{}
		~TimerCallback(){}

		//===================================================================================
		// 規定時間経過でコールバックを呼び出すアップデート( 毎フレーム呼び出せばOK )
		// arg1... フレーム間の経過時間( ミリ秒指定 )
		// ret.... コールバックを呼び出したら true が帰る
		//===================================================================================
		inline bool update( const float delta_time ) {
			sum_time_ += delta_time;
			if ( sum_time_ < regulation_time_ ) return false;
			function_(object_, delta_time);
			sum_time_ -= regulation_time_;
			return true;
		}
	};

}

