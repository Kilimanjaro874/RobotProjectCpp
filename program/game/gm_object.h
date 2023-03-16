#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_coordinate.h"
#include "gm_assemble.h"
#include "gm_kinematics.h"
#include "gm_weapon.h"
#include "gm_circle_collider.h"

namespace tol {
	class Coordinate;
	class Assemble;
	class Kinematics;
	class Weapon;
	class CircleCollider;

	class Object : public std::enable_shared_from_this<Object> {

	public:
		/// <summary>
		/// Handle Coordinate, Assemble, Kinematics classes.
		/// <reration>
		/// base of the Actor class.
		/// (Actor has object classes by tree structure.)
		/// </summary>
		/// <param name="id"> unique id. </param>
		/// <param name="name"> unique name. </param>
		Object(int id, std::string name)
			:id_(id), name_(name) {}
		virtual ~Object() {}
		//// ---- Member variables ---- ////
	protected:
		// ---- General ---- //
		int id_;
		std::string name_;
		bool is_alive_ = true;				// survival flag
		// --- option nums --- //
		// for freely used from other classes.(0 : move distance, 1, add damage, 2. hp )
		float option_params[5] = { 0, 0, 0, 0, 0 };		
		bool is_positional_parentage_ = true;			// kinematics effective setting.
		// ---- owned Classes ---- //
		std::shared_ptr<Coordinate> coordinate_ = nullptr;
		std::shared_ptr<Assemble> assemble_ = nullptr;
		std::shared_ptr<Kinematics> kinematics_ = nullptr;
		// ---- option Classes ---- //
		std::shared_ptr<Weapon> weapon_ = nullptr;
		std::shared_ptr<CircleCollider> cir_collider_ = nullptr;
		// ---- Tree structuer ---- //
		std::weak_ptr<Object> parent_;
		std::list<std::shared_ptr<Object>> children_;
		//// ---- Member functions ---- ////
	public:
		virtual void init();
		virtual void init(
			std::shared_ptr<Coordinate> cod,
			std::shared_ptr<Assemble> assem,
			std::shared_ptr<Kinematics> kin
		);
		virtual void update(float delta_time);
		virtual void updateTree(float delta_time
		);
		virtual void render(dxe::Camera* camera);
		virtual void renderTree(dxe::Camera* camera);
		// ---- for user functions ---- //
		void Translate(tnl::Vector3 move, bool absolute_move = false);
		void Rotation(tnl::Quaternion rot, bool absolute_move = true);
		tnl::Vector3 getRight();
		tnl::Vector3 getUp();
		tnl::Vector3 getForward();
		void setRenderScale(float size);
		// ---- for Tree structure ---- //
		std::shared_ptr<Object> getObjectTree(const int id, const std::string& name);
		// ---- setter ---- //
		void setParent(std::weak_ptr<Object> parent) { parent_ = parent; }
		void setChild(std::shared_ptr<Object> child) { children_.push_back(child);}
		void setCoordinate(std::shared_ptr<Coordinate> cod) { coordinate_ = cod; }
		void setAssemble(std::shared_ptr<Assemble> assem) { assemble_ = assem; }
		void setKinematics(std::shared_ptr<Kinematics> kin) { kinematics_ = kin; }
		void setWeapon(std::shared_ptr<Weapon> weapon) { weapon_ = weapon; }
		void setCircleCollider(std::shared_ptr<CircleCollider> collider) { cir_collider_ = collider; }
		void setIsPositionalParentage(bool is_positional_parentage) { is_positional_parentage_ = is_positional_parentage; }
		void setOptionParams(int row, float num) { if (row > 4 || row < 0) { return; } option_params[row] = num; }
		void setIsAlive(bool alive) { is_alive_ = alive; }
		// ---- getter ---- //
		std::weak_ptr<Object> getParent() { return parent_; }
		std::list<std::shared_ptr<Object>> getChildren() { return children_; }
		std::shared_ptr<Coordinate> getCoordinate() { return coordinate_; }
		std::shared_ptr<Assemble> getAssemble() { return assemble_; }
		std::shared_ptr<Kinematics> getKinematics() { return kinematics_; }
		std::shared_ptr<Weapon> getWeapon() { return weapon_; }
		std::shared_ptr<CircleCollider> getCircleCollider() { return cir_collider_; }
		bool getIsPositionalParentage() { return is_positional_parentage_; }
		float getOptionParams(int row) { if (row > 4 || row < 0) { return NULL; } return option_params[row]; }
		bool getIsAlive() { return is_alive_; }
	};
}