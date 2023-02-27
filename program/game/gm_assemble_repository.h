#pragma once
#include <memory>
#include "gm_assemble.h"

namespace tol {
	class Assemble;
	class AssemRepo {
	// <summary>
	/// Create & store Assembles.
	/// <relation>
	/// Object class: have a Assemble class.
	///  -> A specific Assemble class can be copied from the getAssemble function of this class.
	///		( specific Assemble class : Create it in advance with the init function.)
	/// </summary>
	public:
		AssemRepo() {}
		virtual ~AssemRepo() {}
		//// ----- Member variables ----- /////
	private:
		struct assem_st {
			int id_ = -1;
			std::string name = "";
			std::shared_ptr<Assemble> assem_ = nullptr;
		};
		std::list<std::shared_ptr<assem_st>> assem_st_;
		//// ----- Member functions ----- ////
	public:
		static std::shared_ptr<AssemRepo> Create();
		std::shared_ptr<Assemble> CopyAssemble(int id, std::string name = "", bool is_resize = false, float size = 1.0);
		// ---- setter ---- //
		
		// ---- getter ---- //
		
	};
}