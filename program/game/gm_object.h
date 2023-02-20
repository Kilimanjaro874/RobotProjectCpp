#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_coordinate.h"
#include "gm_assemble.h"
#include "gm_kinematics.h"


namespace tol {
	class Coordinate;
	class Assemble;
	class Kinematics;
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
		// ---- owned Classes ---- //
		std::shared_ptr<Coordinate> coordinate_ = nullptr;
		std::shared_ptr<Assemble> assemble_ = nullptr;
		std::shared_ptr<Kinematics> kinematics_ = nullptr;
		// ---- option Classes ---- //

		// ---- Tree structuer ---- //
		std::weak_ptr<Object> parent_;
		std::list<std::shared_ptr<Object>> children_;
		//// ---- Member functions ---- ////
	public:
		virtual void init();
		virtual void init(
			std::shared_ptr<Coordinate> cod,
			std::shared_ptr<Assemble> assem,
			std::shared_ptr<Kinematics> kin);
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
		// ---- getter ---- //
		std::weak_ptr<Object> getParent() { return parent_; }
		std::list<std::shared_ptr<Object>> getChildren() { return children_; }
		std::shared_ptr<Coordinate> getCoordinate() { return coordinate_; }
		std::shared_ptr<Assemble> getAssemble() { return assemble_; }
		std::shared_ptr<Kinematics> getKinematics() { return kinematics_; }
	};
}