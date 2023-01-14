#pragma once
#include <memory>
#include "gm_coordinate.h"
#include "gm_assemble.h"
#include "gm_kinematics.h"

namespace tol {
	class Coordinate;
	class assemble;
	class Kinematics;

	class Object : public std::enable_shared_from_this<Object> {
	/// <summary>
	/// Handle Coordinate, Mesh class.
	/// <reration>
	/// the base of teh Actor class.
	/// Actor : Owns the those classes(by tree structure).
	/// And management of Direct Kinematics, InverseKinematics,
	/// Render etc...
	/// </summary>
	public:
		Object(int id, std::string name) {
			id_ = id;
			name_ = name;
		}
		virtual ~Object() { }
		// ---- General ---- //
		int id_;
		std::string name_;
		// ---- class (required) ---- //
		Coordinate coordinate_;						// have a
		std::shared_ptr<Assemble> assemble_;		// have a (get from AssemRepo)
		// ---- class (option) ---- //
		bool is_active_kinematics_ = true;					
		std::shared_ptr<Kinematics> kinematics_;	// have a
		
	protected:
		// ---- Tree struct ---- //
		std::weak_ptr<Object> parent_;
		std::list<std::shared_ptr<Object>> children_;


	public:
		virtual void update(float delta_time);
		virtual void render(dxe::Camera* camera);
		virtual void updateTree(float delta_time);
		virtual void renderTree(dxe::Camera* camera);
		std::shared_ptr<Object> getObjectTree(int& id, std::string& name);
		// ---- setter ----- //
		void setParent(std::weak_ptr<Object> parent) { parent_ = parent; }
		void setChild(std::shared_ptr<Object> child) { children_.push_back(child); }
		// ---- getter ----- //
		std::weak_ptr<Object> getParent() { return parent_; }
		std::list<std::shared_ptr<Object>> getChildren() { return children_; }
	private:
	};
}