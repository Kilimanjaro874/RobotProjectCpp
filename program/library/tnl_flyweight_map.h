#pragma once
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include "tnl_util.h"

namespace tnl {

	//------------------------------------------------------------------------------------------------------------
	//
	// �t���C�E�F�C�g�ȃ}�b�v 
	// tips... 
	//
	template< class Key, class Class >
	class FlyWeightMap {
	private:
		std::unordered_map< Key, Class > management_map_;
	public:
		//-------------------------------------------------------------------------------
		// ���݂̗L��
		// ret... [ true : �o�^�ς� ] [ false : ���o�^ ] 
		inline bool isExist(Key key) {
			return (management_map_.end() != management_map_.find(key));
		}

		//-------------------------------------------------------------------------------
		// �o�^
		inline void regist(Key key, Class data) {
			management_map_.try_emplace(key, data);
		}

		//-------------------------------------------------------------------------------
		// �J��
		inline void erase(Key key) {
			auto it = management_map_.find(key);
			if (management_map_.end() == it) return;
			management_map_.erase(it);
		}

		//-------------------------------------------------------------------------------
		// �S�J��
		inline void clear() { management_map_.clear(); }

		//-------------------------------------------------------------------------------
		// ���̃}�b�v�݂̂��Q�Ƃ��Ă���R���e�i���J��
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
		// �擾
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
		// ���[�h�Ɠo�^
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
		// �����Ɠo�^
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
