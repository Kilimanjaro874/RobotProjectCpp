#include "gm_assemble_repository.h"

/// <summary>
/// Create AssemRepo class.
/// -> Generate & store Assmble classes in assem_st_;
/// </summary>
/// <returns></returns>
std::shared_ptr<tol::AssemRepo> tol::AssemRepo::Create() {
	// ---- default params ---- //
	const std::string normal_tex = "graphics/test.jpg";
	const tnl::Vector3 zero_v = { 0, 0, 0 };
	const tnl::Quaternion zero_r = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	
	auto asr = std::make_unique<AssemRepo>(AssemRepo());

	// ---- Generate & store Assemble cllasses ---- //


	// --- Shoulder : 2XX ---- //
	// 200
	auto sh200 = std::make_shared<assem_st>();
	sh200->id_ = 200;
	sh200->name = "";
	sh200->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto sp = std::make_shared<Parts>(Parts());
		sp->mesh_ = dxe::Mesh::CreateSphere(0.2);
		sp->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		sp->ofs_pos_ = zero_v;
		sp->ofs_rot_ = zero_r;
		sh200->assem_->setParts(sp);
		auto jo = std::make_shared<Parts>(Parts());
		jo->mesh_ = dxe::Mesh::CreateCylinder(0.1, 1.0);
		jo->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo->ofs_pos_ = { 0, 0.5, 0 };
		jo->ofs_rot_ = zero_r;
		sh200->assem_->setParts(jo);
	}
	asr->assem_st_.push_back(std::move(sh200));
	// 201
	auto sh201 = std::make_shared<assem_st>();
	sh201->id_ = 201;
	sh201->name = "";
	sh201->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto sp = std::make_shared<Parts>(Parts());
		sp->mesh_ = dxe::Mesh::CreateSphere(0.3);
		sp->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		sp->ofs_pos_ = zero_v;
		sp->ofs_rot_ = zero_r;
		sh201->assem_->setParts(sp);
		auto jo = std::make_shared<Parts>(Parts());
		jo->mesh_ = dxe::Mesh::CreateCylinder(0.2, 1.0);
		jo->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo->ofs_pos_ = { 0, 0.5, 0 };
		jo->ofs_rot_ = zero_r;
		sh201->assem_->setParts(jo);
	}
	asr->assem_st_.push_back(std::move(sh201));

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
std::shared_ptr<tol::Assemble> tol::AssemRepo::CopyAssemble(int id, std::string name, bool is_resize, float size) {
	for (auto itr = assem_st_.begin(); itr != assem_st_.end(); itr++) {
		auto assem = *itr;
		if (assem->id_ != id || assem->name != name) {
			continue;
		}
		auto target = *itr;
		auto tmp_assem = std::make_unique<Assemble>(Assemble());
		std::vector<std::shared_ptr<Parts>> parts = target->assem_->CopyParts();
		for (int i = 0; i < parts.size(); i++) {
			tmp_assem->setParts(parts[i]);
		}
		if (is_resize) {
			tmp_assem->setPartsScale(size);
		}
		return tmp_assem;
	}
	return nullptr;
}