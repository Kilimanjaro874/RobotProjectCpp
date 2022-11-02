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
	// 座標とサイズから AABB の max min を取得
	inline tnl::Vector3 ToMaxAABB(const tnl::Vector3& pos, const tnl::Vector3& size) { return pos + (size * 0.5f); }
	inline tnl::Vector3 ToMinAABB(const tnl::Vector3& pos, const tnl::Vector3& size) { return pos - (size * 0.5f); }

	//----------------------------------------------------------------------------------------------
	// メルセンヌツイスター乱数シード設定
	void SetSeedMersenneTwister32(int seed);	
	//----------------------------------------------------------------------------------------------
	// 浮動小数乱数の取得
	// tips... この関数を使用する場合 SetSeedMersenneTwister32 で乱数シードを設定してください
	float GetRandomDistributionFloat(float min, float max);

	//=============================================================================================
	// work... 指定座標が平面の表裏どちらに存在するか判定する
	// arg1... 指定座標
	// arg2... 平面の法線
	// arg3... 平面上に存在するどこかの座標
	// ret.... 表...1 	裏...-1		平面上...0
	// tips... none
	//=============================================================================================
	int GetSidesPointAndPlane(const Vector3& v, const Vector3& pn, const Vector3& pv);

	//=============================================================================================
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
	// work... AABBの中心から全ての対角辺へ無限に伸びる平面を考えたとき
	//         指定した座標が対角辺領域の上下左右前後どこに存在するか取得する
	// arg1... 指定座標
	// arg2... AABB max
	// arg3... AABB min
	// ret... [下 0] [右 1] [上 2] [左 3] [ 前 4 ] [ 後 5 ]
	//=============================================================================================
	int GetRegionPointAndAABB( const Vector3& p, const Vector3& max, const Vector3& min );

	//=============================================================================================
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
	// work... 点とAABBのAABB内に存在する点への最近接点を取得
	// arg1... 点座標
	// arg2... AABB max
	// arg3... AABB min
	// ret... 最近接点
	//=============================================================================================
	tnl::Vector3 GetNearestPointAABB(const tnl::Vector3& point, const tnl::Vector3& aabb_max, const tnl::Vector3& aabb_min );

	//=============================================================================================
	// name... GetNearestRectPoint
	// work... 点と矩形の矩形内に存在する点への最近接点を取得
	// arg1... 矩形座標
	// arg2... 矩形の幅
	// arg3... 矩形の高さ
	// arg4... 点座標
	// ret... 最近接点( 2D )
	//=============================================================================================
	tnl::Vector3 GetNearestRectPoint(const tnl::Vector3& rect_pos, float w, float h, const tnl::Vector3 &point);

	//=============================================================================================
	// name... GetNearestRectPoint
	// work... 点と長方形の長方形内に存在する点への最近接点を取得
	// arg1... 長方形の中心
	// arg2... 長方形のX軸の単位ベクトル
	// arg3... 長方形のY軸の単位ベクトル
	// arg4... 長方形の辺の長さの半分
	// arg5... 点座標
	// ret... 最近接点( 3D )
	//=============================================================================================
	tnl::Vector3 GetNearestRectPoint(const tnl::Vector3& rect_pos, const tnl::Vector3& axis_x, const tnl::Vector3& axis_y, const tnl::Vector3& rect_side_length, const tnl::Vector3& point);

	//=============================================================================================
	// work... 点と三角形の三角形上に存在する最近点
	// arg2... 点座標
	// arg345. 三角形の頂点座標
	// arg4... 三角形上の座標
	//=============================================================================================
	tnl::Vector3 GetNearestPointTriangle(const tnl::Vector3& p, const tnl::Vector3& a, const tnl::Vector3& b, const tnl::Vector3& c);

	//=============================================================================================
	// work... 点と平面の平面上に存在する最近点
	// arg2... 点座標
	// arg3... 平面の法線
	// arg4... 平面上の座標
	//=============================================================================================
	tnl::Vector3 GetNearestPointPlane(const tnl::Vector3& v, const tnl::Vector3& pn, const tnl::Vector3& pv);


	//=============================================================================================
	// work... 動く AABB (A) と動かない AABB (B) が衝突した時の A の補正座標を取得する
	// arg1... 移動前の A 座標
	// arg2... A のサイズ
	// arg3... B の座標
	// arg4... B のサイズ
	// arg5... 補正座標
	// arg6... 補正スペース ( デフォルト 0.1f )
	// ret... [下に補正 0] [右に補正 1] [上に補正 2] [左に補正 3] [ 前に補正 4 ] [ 後に補正 5 ]
	//=============================================================================================
	int GetCorrectPositionIntersectAABB( const tnl::Vector3& a_prev, const tnl::Vector3& a_size, const tnl::Vector3& b, const tnl::Vector3& b_size, tnl::Vector3& out, const float correct_space = 0.1f);

	//=============================================================================================
	// ベジェスプライン補間
	// arg1... 補間始点
	// arg2... anker
	// arg3... anker
	// arg4... 補間終点
	// arg5... 0.0f ～ 1.0f
	// ret.... 補間座標
	//=============================================================================================
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