#pragma once
#include <memory>
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
		virtual ~AssemRepo() {}

	private:
		struct assem_st {
			int id_;
			std::string name_;
			std::shared_ptr<Assemble> assem_;

		};
		std::vector<std::shared_ptr<assem_st>> assem_st_;
	public:
		static std::shared_ptr<AssemRepo> Create();
		// ---- setter ---- //
		// ---- getter ---- //
		std::shared_ptr<Assemble> getAssemble(const int& id, const std::string name = "", bool is_resize = false, float size = 1.0);
		std::shared_ptr<Assemble> CopyRepoAssemble(const int& id, const std::string name = "", bool is_resize = false, float size = 1.0);
	private:

	};
}