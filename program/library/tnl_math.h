#pragma once

#include <vector>
#include "tnl_vector.h"

namespace tnl {

	//----------------------------------------------------------------------------------------------
	// 円周率
	const float PI = 3.14159265358979f;

	//----------------------------------------------------------------------------------------------
	// ラジアンからデグリーへの変換
	inline float ToDegree(float radian) { return (radian / PI * 180.0f); }

	//----------------------------------------------------------------------------------------------
	// デグリーからラジアンへの変換
	inline float ToRadian(float degree) { return (PI / 180.0f * degree); }

	//----------------------------------------------------------------------------------------------
	// メルセンヌツイスター乱数シード設定
	void SetSeedMersenneTwister32(int seed);	
	//----------------------------------------------------------------------------------------------
	// 浮動小数乱数の取得
	// tips... この関数を使用する場合 SetSeedMersenneTwister32 で乱数シードを設定してください
	float GetRandomDistributionFloat(float min, float max);

	//=============================================================================================
	// name... GetSidesPointAndPlane
	// work... 指定座標が平面の表裏どちらに存在するか判定する
	// arg1... 指定座標
	// arg2... 平面の法線
	// arg3... 平面上に存在するどこかの座標
	// ret.... 表...1 	裏...-1		平面上...0
	// tips... none
	//=============================================================================================
	int GetSidesPointAndPlane(const Vector3& v, const Vector3& pn, const Vector3& pv);

	//=============================================================================================
	// name... GetRegionPointAndRect
	// work... 矩形の中心から２本の無限に伸びる対角線を引いた時
	//         指定した座標が対角線から上下左右どこに存在するか取得する
	// arg1... 指定座標
	// arg2... 矩形の中心座標
	// arg3... 矩形の幅
	// arg4... 矩形の高さ
	// ret... [上 0] [右 1] [下 2] [左 3]
	//=============================================================================================
	int GetRegionPointAndRect(const Vector3& p, const Vector3& rp, const int rect_w, const int rect_h);


	//=============================================================================================
	// name... GetXzRegionPointAndOBB
	// work... OBB のローカル空間に対してY軸に垂直な平面を OBB の対角に２枚 クロスした時
	//         指定した座標が対角面から前後左右どこに存在するか取得する
	// arg1... 指定座標
	// arg2... OBB の中心座標
	// arg3... OBB の幅, 高さ, 奥行
	// arg4... OBB の姿勢
	// ret... [前 0] [右 1] [後 2] [左 3]
	//=============================================================================================
	int GetXzRegionPointAndOBB(const Vector3& p, const Vector3& op, const Vector3& size, const Quaternion& q);


	//=============================================================================================
	// name... GetNearestRectPoint
	// work... 点と矩形の矩形内に存在する点への最近接点を取得
	// arg1... 矩形座標
	// arg2... 矩形の幅
	// arg3... 矩形の高さ
	// arg4... 点座標
	// ret... 最近接点
	//=============================================================================================
	tnl::Vector3 GetNearestRectPoint(const tnl::Vector3& rect_pos, float w, float h, const tnl::Vector3 &point);


	//=============================================================================================
	// work... 点と平面の平面上に存在する最近点
	// arg2... 点座標
	// arg3... 平面の法線
	// arg4... 平面上の座標
	//=============================================================================================
	tnl::Vector3 GetNearestPointPlane(const tnl::Vector3& v, const tnl::Vector3& pn, const tnl::Vector3& pv);


	//----------------------------------------------------------------------------------------------
	// ベジェスプライン補間
	// arg1... 補間始点
	// arg2... anker
	// arg3... anker
	// arg4... 補間終点
	// arg5... 0.0f ～ 1.0f
	// ret.... 補間座標
	Vector3 BezierSpline(const Vector3& _a1, const Vector3& _a2, const Vector3& _a3, const Vector3& _a4, float t);


	//----------------------------------------------------------------------------------------------
	//
	// 三次スプライン補間
	//
	class CubicSpline final {
	private:
		std::vector<tnl::Vector3> a_;
		std::vector<tnl::Vector3> b_;
		std::vector<tnl::Vector3> c_;
		std::vector<tnl::Vector3> d_;
		std::vector<tnl::Vector3> w_;

	public:
		CubicSpline() {};
		CubicSpline(const std::vector<tnl::Vector3>& v);

		//---------------------------------------------------------------------------
		// 補間座標の取得
		// arg1... 0.0f ～ 1.0f
		// ret.... 補間座標
		inline tnl::Vector3 getPosition(float t) {
			float rt = float(a_.size() - 1) * t;
			int p = int(floor(rt));
			float dt = rt - p;
			return a_[p] + (b_[p] + (c_[p] + d_[p] * dt) * dt) * dt;
		}

	};

	//----------------------------------------------------------------------------------------------
	//
	// 座標群線形補間
	//
	class PointsLerp final {
	private:
		float all_length_ = 0;
		std::vector< tnl::Vector3 > points_;
		std::vector< float > section_lengths_;
		float getLengthRate(float t) const noexcept;
		float getLengthPoint(int pn) const noexcept;
		int getPrevPoint(float t) const noexcept;

	public:

		PointsLerp(const std::vector< tnl::Vector3 >& points);

		//---------------------------------------------------------------------------
		// 補間座標の取得
		// arg1... 0.0f ～ 1.0f
		// ret.... 補間座標
		inline tnl::Vector3 getPosition(const float t) {
			int n = getPrevPoint(t);
			float len = getLengthRate(t) - getLengthPoint(n);
			tnl::Vector3 v = tnl::Vector3::Normalize((points_[n + 1] - points_[n]));
			return points_[n] + (v * len);
		}

	};

}