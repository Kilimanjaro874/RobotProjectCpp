#include "gm_assemble_repository.h"

/// <summary>
/// Create AssemRepo classes.
/// Å¶ Generates Assem classes here and stores in assem_st_;
/// </summary>
 tol::AssemRepo* tol::AssemRepo::Create() {
	
	// ---- default params ---- //
	const std::string  normal_tex = "graphics/test.jpg";

	const tnl::Vector3 zero_vec = { 0, 0, 0 };
	const tnl::Quaternion zero_rot = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	// ---- get instance ---- //
	AssemRepo* asr = new AssemRepo();
	// ---- make Assembles ---- //
	

	// --- Shoulder: 2XX --- //
	// 200:
	assem_st* sh200 = new assem_st({ 200, "sh200" });
	sh200->assem_ = new Assemble();
	{
		dxe::Mesh* sp = dxe::Mesh::CreateSphere(0.3);
		sp->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		sp->pos_ = zero_vec;
		sp->rot_q_ = zero_rot;
		sh200->assem_->setMesh(sp);
		dxe::Mesh* jo = dxe::Mesh::CreateCylinder(0.2, 2.5);
		jo->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo->pos_ = { 0, -1.0, 0 };
		jo->rot_q_ = zero_rot;
		sh200->assem_->setMesh(jo);
	}
	// registrate 
	asr->assem_st_.push_back(sh200);

	return asr;
	
}

/// <summary>
/// get Assemble class pointer
/// Å¶ Excecute Create() in advance.
/// </summary>
/// <param name="id"> Assemble class id </param>
/// <param name="name"> Assemble class name </param>
/// <param name="is_resize"> Assemble class resize flag </param>
/// <param name="size"> Assemble class resize param : size of (x, y, z) </param>
/// <returns></returns>
tol::Assemble* tol::AssemRepo::getAssemble(const int& id, const std::string name, bool is_resize, float size) {
	for (auto a : assem_st_) {
		if (a->id_ == id || a->name_ == name) {
			if (is_resize) {
				a->assem_->setScale(size);
			}
			return a->assem_;
		}
	}
	return nullptr;
}