#pragma once
#include "gm_assemble.h"

namespace tol {
	class AssemRepo {
	/// <summary>
	/// Create & store Assembles.
	/// <relation>
	/// Object class: have a Assemble class.
	///  -> A specific Assemble class can be copied from the getAssemble function of this class.
	///		( specific Assemble class : Create it in advance with the init function.)
	/// </summary>
	public:
		AssemRepo() {}
		virtual ~AssemRepo() {
			for (auto a : assem_st_) {
				delete a;
			}
		}

	private:
		struct assem_st {
			int id_;
			std::string name_;
			Assemble* assem_;
		};
		std::vector<assem_st*> assem_st_;
	public:
		static AssemRepo* Create();
		// ---- setter ---- //
		// ---- getter ---- //
		Assemble* getAssemble(const int& id, const std::string name = "", bool is_resize = false, float size = 1.0);
	private:

	};
}