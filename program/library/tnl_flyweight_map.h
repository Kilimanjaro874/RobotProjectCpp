#pragma once
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include "tnl_util.h"

namespace tnl {

	//------------------------------------------------------------------------------------------------------------
	//
	// フライウェイトなマップ 
	// tips... 
	//
	template< class Key, class Class >
	class FlyWeightMap {
	private:
		std::unordered_map< Key, Class > management_map_;
	public:
		//-------------------------------------------------------------------------------
		// 存在の有無
		// ret... [ true : 登録済み ] [ false : 未登録 ] 
		inline bool isExist(Key key) {
			return (management_map_.end() != management_map_.find(key));
		}

		//-------------------------------------------------------------------------------
		// 登録
		inline void regist(Key key, Class data) {
			management_map_.try_emplace(key, data);
		}

		//-------------------------------------------------------------------------------
		// 開放
		inline void erase(Key key) {
			auto it = management_map_.find(key);
			if (management_map_.end() == it) return;
			management_map_.erase(it);
		}

		//-------------------------------------------------------------------------------
		// 全開放
		inline void clear() { management_map_.clear(); }

		//-------------------------------------------------------------------------------
		// このマップのみが参照しているコンテナを開放
		inline void eraseUnique() {
			auto it = management_map_.begin();
			while (it != management_map_.end()) {
				if (1 < it->second.use_count()) {
					it++;
					continue;
				}
				it = management_map_.erase( it );
			}
		}

		//-------------------------------------------------------------------------------
		// 取得
		inline Class get(Key key) {
			Class ptr;
			try {
				ptr = management_map_.at(key);
			}
			catch (std::out_of_range&) {
				//TNL_DEBUG_ASSERTE("fly weight map get error");
			}
			return ptr;
		}


		//-------------------------------------------------------------------------------
		// ロードと登録
		template< class ConstKey, class Desc >
		Class create(ConstKey key, Desc desc, Class(*creator)(Desc) ) {
			Class data;
			if (isExist(key)) {
				data = get(key);
				return data;
			}
			data = creator(desc);
			regist(key, data);
			return data;
		}

		//-------------------------------------------------------------------------------
		// 生成と登録
		template< class ConstKey >
		Class create(ConstKey key, Class(*creator)()) {
			Class data;
			if (isExist(key)) {
				data = get(key);
				return data;
			}
			data = creator();
			regist(key, data);
			return data;
		}

	};

}
