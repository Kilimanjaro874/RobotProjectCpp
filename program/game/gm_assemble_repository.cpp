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
	auto non = std::make_shared<assem_st>();
	non->id_ = 0;
	non->name = "non";
	non->assem_ = std::make_shared<Assemble>(Assemble());
	asr->assem_st_.push_back(std::move(non));

	// --- Lower Back : 1XX ---- //
	// 100
	auto lb100 = std::make_shared<assem_st>();
	lb100->id_ = 100;
	lb100->name = "lb100";
	lb100->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto jo1 = std::make_shared<Parts>(Parts());
		jo1->mesh_ = dxe::Mesh::CreateCylinder(0.2, 2);
		jo1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		lb100->assem_->setParts(jo1);

		auto jo2 = std::make_shared<Parts>(Parts());
		jo2->mesh_ = dxe::Mesh::CreateCylinder(0.2, 2.5);
		jo2->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo2->ofs_pos_ += {0, -1, 0};
		jo2->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
		lb100->assem_->setParts(jo2);
	}
	asr->assem_st_.push_back(std::move(lb100));
	// 101

	// --- Body : 2XX --- //
	// 200
	auto bd200 = std::make_shared<assem_st>();
	bd200->id_ = 200;
	bd200->name = "bd200";
	bd200->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto sp1 = std::make_shared<Parts>(Parts());
		sp1->mesh_ = dxe::Mesh::CreateSphere(0.3);
		sp1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		bd200->assem_->setParts(sp1);

		auto jo1 = std::make_shared<Parts>(Parts());
		jo1->mesh_ = dxe::Mesh::CreateCylinder(0.2, 0.5);
		jo1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo1->ofs_pos_ += {0, 0.25, 0};
		bd200->assem_->setParts(jo1);

		auto jo2 = std::make_shared<Parts>(Parts());
		jo2->mesh_ = dxe::Mesh::CreateCylinder(0.2, 2.1213);
		jo2->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo2->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(45));
		jo2->ofs_pos_ += {-0.75, 1.25, 0};
		bd200->assem_->setParts(jo2);

		auto jo3 = std::make_shared<Parts>(Parts());
		jo3->mesh_ = dxe::Mesh::CreateCylinder(0.2, 2.1213);
		jo3->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo3->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(-45));
		jo3->ofs_pos_ += {0.75, 1.25, 0};
		bd200->assem_->setParts(jo3);

		auto jo4 = std::make_shared<Parts>(Parts());
		jo4->mesh_ = dxe::Mesh::CreateCylinder(0.2, 4.0);
		jo4->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo4->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
		jo4->ofs_pos_ += {0, 2.0, 0};
		bd200->assem_->setParts(jo4);

		auto sp2 = std::make_shared<Parts>(Parts());
		sp2->mesh_ = dxe::Mesh::CreateSphere(0.75);
		sp2->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		sp2->ofs_pos_ += {0, 1.2, 0};
		bd200->assem_->setParts(sp2);
	}
	asr->assem_st_.push_back(std::move(bd200));

	// --- Right Arm : 3XX --- //
	// 300
	auto ra300 = std::make_shared<assem_st>();
	ra300->id_ = 300;
	ra300->name = "ra300";
	ra300->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto sp1 = std::make_shared<Parts>(Parts());
		sp1->mesh_ = dxe::Mesh::CreateSphere(0.3);
		sp1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		ra300->assem_->setParts(sp1);

		auto jo1 = std::make_shared<Parts>(Parts());
		jo1->mesh_ = dxe::Mesh::CreateCylinder(0.2, 2.0);
		jo1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo1->ofs_pos_ += {0, -1.0, 0};
		ra300->assem_->setParts(jo1);
	}
	asr->assem_st_.push_back(std::move(ra300));
	// 301
	auto ra301 = std::make_shared<assem_st>();
	ra301->id_ = 301;
	ra301->name = "ra301";
	ra301->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto sp1 = std::make_shared<Parts>(Parts());
		sp1->mesh_ = dxe::Mesh::CreateSphere(0.3);
		sp1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		ra301->assem_->setParts(sp1);

		auto jo1 = std::make_shared<Parts>(Parts());
		jo1->mesh_ = dxe::Mesh::CreateCylinder(0.2, 0.5);
		jo1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo1->ofs_pos_ += {0, -0.25, 0};
		ra301->assem_->setParts(jo1);
	}
	asr->assem_st_.push_back(std::move(ra301));

	// --- Left Arm : 4XX --- //
	// 400
	auto la400 = std::make_shared<assem_st>();
	la400->id_ = 400;
	la400->name = "la400";
	la400->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto sp1 = std::make_shared<Parts>(Parts());
		sp1->mesh_ = dxe::Mesh::CreateSphere(0.3);
		sp1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		la400->assem_->setParts(sp1);

		auto jo1 = std::make_shared<Parts>(Parts());
		jo1->mesh_ = dxe::Mesh::CreateCylinder(0.2, 2.0);
		jo1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo1->ofs_pos_ += {0, -1.0, 0};
		la400->assem_->setParts(jo1);
	}
	asr->assem_st_.push_back(std::move(la400));
	// 401
	auto la401 = std::make_shared<assem_st>();
	la401->id_ = 401;
	la401->name = "la401";
	la401->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto sp1 = std::make_shared<Parts>(Parts());
		sp1->mesh_ = dxe::Mesh::CreateSphere(0.3);
		sp1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		la401->assem_->setParts(sp1);

		auto jo1 = std::make_shared<Parts>(Parts());
		jo1->mesh_ = dxe::Mesh::CreateCylinder(0.2, 0.5);
		jo1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo1->ofs_pos_ += {0, -0.25, 0};
		la401->assem_->setParts(jo1);
	}
	asr->assem_st_.push_back(std::move(la401));

	// --- Head : 5XX --- //
	// 500
	auto he500 = std::make_shared<assem_st>();
	he500->id_ = 500;
	he500->name = "he500";
	he500->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto jo1 = std::make_shared<Parts>(Parts());
		jo1->mesh_ = dxe::Mesh::CreateCylinder(0.2, 0.5);
		jo1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo1->ofs_pos_ += {0, 0.25, 0};
		he500->assem_->setParts(jo1);
	}
	asr->assem_st_.push_back(std::move(he500));
	// 501
	auto he501 = std::make_shared<assem_st>();
	he501->id_ = 501;
	he501->name = "he501";
	he501->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto bo1 = std::make_shared<Parts>(Parts());
		bo1->mesh_ = dxe::Mesh::CreateBox(0.5);
		bo1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		he501->assem_->setParts(bo1);
	}
	asr->assem_st_.push_back(std::move(he501));

	// --- Right Leg : 6XX --- //
	// 600
	auto rl600 = std::make_shared<assem_st>();
	rl600->id_ = 600;
	rl600->name = "rl600";
	rl600->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto sp1 = std::make_shared<Parts>(Parts());
		sp1->mesh_ = dxe::Mesh::CreateSphere(0.4);
		sp1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		rl600->assem_->setParts(sp1);

		auto jo1 = std::make_shared<Parts>(Parts());
		jo1->mesh_ = dxe::Mesh::CreateCylinder(0.3, 2.5);
		jo1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo1->ofs_pos_ += {0, -2.5 / 2, 0};
		rl600->assem_->setParts(jo1);
	}
	asr->assem_st_.push_back(std::move(rl600));
	// 601
	auto rl601 = std::make_shared<assem_st>();
	rl601->id_ = 601;
	rl601->name = "rl601";
	rl601->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto sp1 = std::make_shared<Parts>(Parts());
		sp1->mesh_ = dxe::Mesh::CreateSphere(0.4);
		sp1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		rl601->assem_->setParts(sp1);

		auto jo1 = std::make_shared<Parts>(Parts());
		jo1->mesh_ = dxe::Mesh::CreateCylinder(0.3, 1);
		jo1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo1->ofs_pos_ += {0, -1.0 / 2, 0};
		rl601->assem_->setParts(jo1);

		auto bo1 = std::make_shared<Parts>(Parts());
		bo1->mesh_ = dxe::Mesh::CreateBox(0.5);
		bo1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		bo1->ofs_pos_ += {0, -0.75, 0.5};
		rl601->assem_->setParts(bo1);
	}
	asr->assem_st_.push_back(std::move(rl601));

	// --- Light Leg : 7XX --- //
	// 700
	auto ll700 = std::make_shared<assem_st>();
	ll700->id_ = 700;
	ll700->name = "ll700";
	ll700->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto sp1 = std::make_shared<Parts>(Parts());
		sp1->mesh_ = dxe::Mesh::CreateSphere(0.4);
		sp1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		ll700->assem_->setParts(sp1);

		auto jo1 = std::make_shared<Parts>(Parts());
		jo1->mesh_ = dxe::Mesh::CreateCylinder(0.3, 2.5);
		jo1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo1->ofs_pos_ += {0, -2.5 / 2, 0};
		ll700->assem_->setParts(jo1);
	}
	asr->assem_st_.push_back(std::move(ll700));
	// 701
	auto ll701 = std::make_shared<assem_st>();
	ll701->id_ = 701;
	ll701->name = "ll701";
	ll701->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto sp1 = std::make_shared<Parts>(Parts());
		sp1->mesh_ = dxe::Mesh::CreateSphere(0.4);
		sp1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		ll701->assem_->setParts(sp1);

		auto jo1 = std::make_shared<Parts>(Parts());
		jo1->mesh_ = dxe::Mesh::CreateCylinder(0.3, 1);
		jo1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		jo1->ofs_pos_ += {0, -1.0 / 2, 0};
		ll701->assem_->setParts(jo1);

		auto bo1 = std::make_shared<Parts>(Parts());
		bo1->mesh_ = dxe::Mesh::CreateBox(0.5);
		bo1->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_tex));
		bo1->ofs_pos_ += {0, -0.75, 0.5};
		ll701->assem_->setParts(bo1);
	}
	asr->assem_st_.push_back(std::move(ll701));

	// --- Shoulder : 2XX --- //
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

	// --- Bullets : 9XX ---- //
	// 900 
	auto bu900 = std::make_shared<assem_st>();
	bu900->id_ = 900;
	bu900->name = "machine_gun";
	bu900->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto sp = std::make_shared<Parts>(Parts());
		sp->mesh_ = dxe::Mesh::CreateSphere(1.0);
		sp->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
		sp->ofs_pos_ = zero_v;
		sp->ofs_rot_ = zero_r;
		bu900->assem_->setParts(sp);
	}
	asr->assem_st_.push_back(std::move(bu900));
	// 901
	auto bu901 = std::make_shared<assem_st>();
	bu901->id_ = 901;
	bu901->name = "rifle";
	bu901->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto sp = std::make_shared<Parts>(Parts());
		sp->mesh_ = dxe::Mesh::CreateSphere(1.0);
		sp->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
		sp->ofs_pos_ = zero_v;
		sp->ofs_rot_ = zero_r;
		bu901->assem_->setParts(sp);
	}
	asr->assem_st_.push_back(std::move(bu901));

	// --- Enemy : 10XX --- //
	// --- 1000 --- //
	auto en1000 = std::make_shared<assem_st>();
	en1000->id_ = 1000;
	en1000->name = "BallEnemy";
	en1000->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto sp = std::make_shared<Parts>(Parts());
		sp->mesh_ = dxe::Mesh::CreateSphere(10.0);
		sp->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
		sp->ofs_pos_ = zero_v;
		sp->ofs_rot_ = zero_r;
		en1000->assem_->setParts(sp);
	}
	asr->assem_st_.push_back(std::move(en1000));

	// --- Target : 20XX --- //
	// 2000
	auto tar2000 = std::make_shared<assem_st>();
	tar2000->id_ = 2000;
	tar2000->name = "move_sight";
	tar2000->assem_ = std::make_shared<Assemble>(Assemble());
	{
		auto sp = std::make_shared<Parts>(Parts());
		sp->mesh_ = dxe::Mesh::CreatePlane(tnl::Vector3{30, 30, 0});
		sp->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/sight2.png"));
		sp->ofs_pos_ = zero_v;
		sp->ofs_rot_ = zero_r;
		tar2000->assem_->setParts(sp);
	}
	asr->assem_st_.push_back(std::move(tar2000));



	return asr;
}

/// <summary>
/// get new Assemble class pointer(retun :Assembled class (copy of) stacked in this class)
/// ¦ Excecute Create() in advance.
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