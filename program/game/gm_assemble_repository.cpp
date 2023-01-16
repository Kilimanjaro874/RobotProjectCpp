#include "gm_assemble_repository.h"
#include "gm_assemble.h"

/// <summary>
/// Create AssemRepo classes.
/// Å¶ Generates Assem classes here and stores in assem_st_;
/// </summary>
 std::shared_ptr<tol::AssemRepo> tol::AssemRepo::Create() {
	
	// ---- default params ---- //
	const std::string  normal_tex = "graphics/test.jpg";

	const tnl::Vector3 zero_vec = { 0, 0, 0 };
	const tnl::Quaternion zero_rot = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	// ---- get instance ---- //
	auto asr = std::make_shared<AssemRepo>(AssemRepo());
	// ---- make Assembles ---- //
	

	// --- Shoulder: 2XX --- //
	// 200:
	/*std::shared_ptr<assem_st> sh200 = std::make_shared<assem_st>({200, "", std::make_unique<Assemble>(Assemble())});
	*/
	auto sh200 = std::make_shared<assem_st>();
	sh200->id_ = 200;
	sh200->name_ = "";
	sh200->assem_ = std::make_unique<Assemble>(Assemble());
	{
		auto sp = std::make_shared<Parts>(Parts());
		sp->mesh_ = dxe::Mesh::CreateSphere(0.2);
		sp->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		sp->ofs_pos_ = zero_vec;
		sp->ofs_rot_ = zero_rot;
		sh200->assem_->setParts(sp);
		auto jo = std::make_shared<Parts>(Parts());
		jo->mesh_ = dxe::Mesh::CreateCylinder(0.1, 1.0);
		jo->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo->ofs_pos_ = { 0, 0.5, 0 };
		jo->ofs_rot_ = zero_rot;
		sh200->assem_->setParts(jo);
	}
	// registrate 
	asr->assem_st_.push_back(std::move(sh200));

	return asr;
	
}

/// <summary>
/// get new Assemble class pointer(retun :Assembled class (copy of) stacked in this class)
/// Å¶ Excecute Create() in advance.
/// </summary>
/// <param name="id"> Assemble class id </param>
/// <param name="name"> Assemble class name </param>
/// <param name="is_resize"> Assemble class resize flag </param>
/// <param name="size"> Assemble class resize param : size of (x, y, z) </param>
/// <returns></returns>
std::shared_ptr<tol::Assemble> tol::AssemRepo::getAssemble(const int& id, const std::string name, bool is_resize, float size) {
	for (int i = 0; i < assem_st_.size(); i++) {
		if (assem_st_[i]->id_ == id || assem_st_[i]->name_ == name) {
			auto target = assem_st_[i]->assem_;
			auto tmp = std::make_shared<tol::Assemble>(Assemble());
			tmp->setIsUpdate(target->getIsUpdate());
			tmp->setIsRender(target->getIsRender());
			std::vector<std::shared_ptr<Parts>> parts = target->CreateCopyParts();
			for (int i = 0; i < parts.size(); i++) {
				tmp->setParts(parts[i]);
			}
			if (is_resize) {
				tmp->setScale(size);
			}
			return tmp;
		}
	}
	return nullptr;
}