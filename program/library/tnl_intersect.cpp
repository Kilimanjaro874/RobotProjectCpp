#include <memory>
#include "tnl_vector.h"
#include "tnl_intersect.h"
#include "tnl_math.h"
#include "tnl_matrix.h"

namespace tnl {

	// 点と矩形の衝突判定
	bool IsIntersectPointRect(const int point_x, const int point_y, const int rect_x, const int rect_y, const int rect_size) {
		int sx = rect_x - (rect_size >> 1);
		int sy = rect_y - (rect_size >> 1);
		int ex = sx + rect_size;
		int ey = sy + rect_size;
		if (point_x < sx) return false;
		if (point_x > ex) return false;
		if (point_y < sy) return false;
		if (point_y > ey) return false;
		return true;
	}

	// 矩形と矩形
	bool IsIntersectRectPrimitive(const int a_left, const int a_right, const int a_top, const int a_bottom,
		const int b_left, const int b_right, const int b_top, const int b_bottom) {
		if (a_right < b_left) return false;
		if (a_left > b_right) return false;
		if (a_top > b_bottom) return false;
		if (a_bottom < b_top) return false;
		return true;
	}

	// 矩形と矩形
	bool IsIntersectRect(const int a_rect_x, const int a_rect_y, const int a_rect_size_w, const int a_rect_size_h,
		const int b_rect_x, const int b_rect_y, const int b_rect_size_w, const int b_rect_size_h) {

		int a_right = a_rect_x + (a_rect_size_w >> 1);
		int a_left = a_rect_x - (a_rect_size_w >> 1);
		int a_top = a_rect_y - (a_rect_size_h >> 1);
		int a_bottom = a_rect_y + (a_rect_size_h >> 1);

		int b_right = b_rect_x + (b_rect_size_w >> 1);
		int b_left = b_rect_x - (b_rect_size_w >> 1);
		int b_top = b_rect_y - (b_rect_size_h >> 1);
		int b_bottom = b_rect_y + (b_rect_size_h >> 1);

		return IsIntersectRectPrimitive(a_left, a_right, a_top, a_bottom, b_left, b_right, b_top, b_bottom);
	}

	// 矩形と矩形の衝突検知 & 座標補正
	int IsIntersectRectToCorrectPosition(tnl::Vector3& a_now, const tnl::Vector3 &a_prev, const int a_rect_size_w, const int a_rect_size_h,
		const tnl::Vector3& b, const int b_rect_size_w, const int b_rect_size_h, const float correct_space) {

		const int NO_HIT = 0;
		const int CORRECT_LEFT = 1;
		const int CORRECT_RIGHT = 2;
		const int CORRECT_UP = 3;
		const int CORRECT_DOWN = 4;

		if (!tnl::IsIntersectRect(a_now, a_rect_size_w, a_rect_size_h, b, b_rect_size_w, b_rect_size_h)) return NO_HIT;

		tnl::Vector3 a_near = tnl::GetNearestRectPoint(a_prev, (float)a_rect_size_w, (float)a_rect_size_h, b);
		int n = tnl::GetRegionPointAndRect(a_near, b, b_rect_size_w, b_rect_size_h);

		if (0 == n) {
			float y = (a_now - a_prev).y;
			// 下向きに移動している場合は上に補正
			if (y >= 0) {
				a_now.y = b.y - (b_rect_size_h / 2) - (a_rect_size_h / 2) - correct_space;
				return CORRECT_UP;
			}
			// 上向きに移動している場合は左右どちらかに補正
			else {
				// 右に補正
				if (a_now.x > b.x) {
					a_now.x = b.x + (b_rect_size_w / 2) + (a_rect_size_w / 2) + correct_space;
					return CORRECT_RIGHT;
				}
				// 左に補正
				else {
					a_now.x = b.x - (b_rect_size_w / 2) - (a_rect_size_w / 2) - correct_space;
					return CORRECT_LEFT;
				}
			}
		}
		else if (2 == n) {
			float y = (a_now - a_prev).y;
			// 上向きに移動している場合は下に補正
			if (y <= 0) {
				a_now.y = b.y + (b_rect_size_h / 2) + (a_rect_size_h / 2) + correct_space;
				return CORRECT_DOWN;
			}
			// 上向きに移動している場合は左右どちらかに補正
			else {
				// 右に補正
				if (a_now.x > b.x) {
					a_now.x = b.x + (b_rect_size_w / 2) + (a_rect_size_w / 2) + correct_space;
					return CORRECT_RIGHT;
				}
				// 左に補正
				else {
					a_now.x = b.x - (b_rect_size_w / 2) - (a_rect_size_w / 2) - correct_space;
					return CORRECT_LEFT;
				}
			}
		}
		else if (1 == n) {
			float x = (a_now - a_prev).x;
			// 左向きに移動している場合は右に補正
			if (x <= 0) {
				a_now.x = b.x + (b_rect_size_w / 2) + (a_rect_size_w / 2) + correct_space;
				return CORRECT_RIGHT;
			}
			// 右向きに移動している場合は上下どちらかに補正
			else {
				// 上に補正
				if (a_now.y < b.y) {
					a_now.y = b.y - (b_rect_size_h / 2) - (a_rect_size_h / 2) - correct_space;
					return CORRECT_UP;
				}
				// 下に補正
				else {
					a_now.y = b.y + (b_rect_size_h / 2) + (a_rect_size_h / 2) + correct_space;
					return CORRECT_DOWN;
				}
			}
		}
		else if (3 == n) {
			float x = (a_now - a_prev).x;
			// 右向きに移動している場合は左に補正
			if (x >= 0) {
				a_now.x = b.x - (b_rect_size_w / 2) - (a_rect_size_w / 2) - correct_space;
				return CORRECT_LEFT;
			}
			// 左向きに移動している場合は上下どちらかに補正
			else {
				// 上に補正
				if (a_now.y < b.y) {
					a_now.y = b.y - (b_rect_size_h / 2) - (a_rect_size_h / 2) - correct_space;
					return CORRECT_UP;
				}
				// 下に補正
				else {
					a_now.y = b.y + (b_rect_size_h / 2) + (a_rect_size_h / 2) + correct_space;
					return CORRECT_DOWN;
				}
			}
		}
		return NO_HIT;
	}


	//----------------------------------------------------------------------------------------------
	bool IsIntersectSphere(const tnl::Vector3& a, float a_radius, const tnl::Vector3& b, float b_radius) {
		float l = (a - b).length();
		return l < (a_radius + b_radius) ? true : false;
	}

	//----------------------------------------------------------------------------------------------
	bool IsIntersectLinePlane(const tnl::Vector3 &line_start, const tnl::Vector3 &line_end, const tnl::Vector3 &plane_pos, const tnl::Vector3 &plane_normal, tnl::Vector3 *intersect_position) {

		if (1 == GetSidesPointAndPlane(line_start, plane_normal, plane_pos) && 1 == GetSidesPointAndPlane(line_end, plane_normal, plane_pos)) return false;
		if (-1 == GetSidesPointAndPlane(line_start, plane_normal, plane_pos) && -1 == GetSidesPointAndPlane(line_end, plane_normal, plane_pos)) return false;

		tnl::Vector3 ln = line_end - line_start;
		tnl::Vector3 pa = line_start - plane_pos;
		tnl::Vector3 pb = line_end - plane_pos;
		float na = fabsf(pa.dot(plane_normal));
		float nb = fabsf(pb.dot(plane_normal));

		if(intersect_position) *intersect_position = line_start + ln * (na / (na + nb));

		return true;
	}

	//----------------------------------------------------------------------------------------------
	bool IsIntersectLine2D(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4)
	{

		// x 座標によるチェック
		if (p1.x >= p2.x) {
			if ((p1.x < p3.x && p1.x < p4.x) || (p2.x > p3.x && p2.x > p4.x)) {
				return false;
			}
		}
		else {
			if ((p2.x < p3.x && p2.x < p4.x) || (p1.x > p3.x && p1.x > p4.x)) {
				return false;
			}
		}

		// y 座標によるチェック
		if (p1.y >= p2.y) {
			if ((p1.y < p3.y && p1.y < p4.y) || (p2.y > p3.y && p2.y > p4.y)) {
				return false;
			}
		}
		else {
			if ((p2.y < p3.y && p2.y < p4.y) || (p1.y > p3.y && p1.y > p4.y)) {
				return false;
			}
		}

		if (((p1.x - p2.x) * (p3.y - p1.y) + (p1.y - p2.y) * (p1.x - p3.x)) * ((p1.x - p2.x) * (p4.y - p1.y) + (p1.y - p2.y) * (p1.x - p4.x)) > 0) return false;
		if (((p3.x - p4.x) * (p1.y - p3.y) + (p3.y - p4.y) * (p3.x - p1.x)) * ((p3.x - p4.x) * (p2.y - p3.y) + (p3.y - p4.y) * (p3.x - p2.x)) > 0) return false;

		return true;
	}


	//----------------------------------------------------------------------------------------------
	bool IsIntersectLineTriangle2D(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& s, const Vector3& e)
	{
		if (IsIntersectLine2D(v1, v2, s, e)) return true;
		if (IsIntersectLine2D(v2, v3, s, e)) return true;
		if (IsIntersectLine2D(v3, v1, s, e)) return true;
		if (IsIntersectTrianglePoint(v1, v2, v3, s)) return true;
		if (IsIntersectTrianglePoint(v1, v2, v3, e)) return true;
		return false;
	}


	//----------------------------------------------------------------------------------------------
	bool IsIntersectTrianglePoint(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& p)
	{
		Vector3 a = v1 - p;
		Vector3 b = v2 - p;
		Vector3 c = v3 - p;

		float ab = Vector3::Dot(a, b);
		float ac = Vector3::Dot(a, c);
		float bc = Vector3::Dot(b, c);
		float cc = Vector3::Dot(c, c);

		if (0.0f > ((bc * ac) - (cc * ab))) return false;
		float bb = Vector3::Dot(b, b);
		if (0.0f > ((ab * bc) - (ac * bb))) return false;

		return true;
	}


	//----------------------------------------------------------------------------------------------
	bool IsIntersectLineTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& s, const Vector3& e)
	{

		Vector3 ab = v2 - v1;
		Vector3 ac = v3 - v1;
		Vector3 pq = s - e;

		Vector3 n = Vector3::Cross(ab, ac);

		float d = Vector3::Dot(pq, n);
		if (d <= 0.0f) return false;

		Vector3 ap = s - v1;
		float t = Vector3::Dot(ap, n);
		if (t < 0.0f) return false;
		if (t > d) return false;

		Vector3 e_ = Vector3::Cross(pq, ap);
		float v = Vector3::Dot(ac, e_);
		if (v < 0.0f || v > d) return false;
		float w = -Vector3::Dot(ab, e_);
		if (w < 0.0f || v + w > d) return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------
	bool IsIntersectRayOBB(const Vector3& pos, const Vector3& dir, const Vector3& aabb_max, const Vector3& aabb_min, const Matrix& obb_rot, Vector3& intersect_pos ) {

		// 光線を境界ボックスの空間へ移動
		Matrix invMat = Matrix::Inverse(obb_rot);

		Vector3 p_l, dir_l;
		p_l = Vector3::TransformCoord(pos, invMat);
		invMat._41 = 0.0f;
		invMat._42 = 0.0f;
		invMat._43 = 0.0f;
		dir_l = Vector3::TransformCoord(dir, invMat);

		// 交差判定
		float p[3], d[3], min[3], max[3];
		memcpy(p, &p_l, sizeof(Vector3));
		memcpy(d, &dir_l, sizeof(Vector3));
		memcpy(min, &aabb_min, sizeof(Vector3));
		memcpy(max, &aabb_max, sizeof(Vector3));

		float t = -FLT_MAX;
		float t_max = FLT_MAX;

		for (int i = 0; i < 3; ++i) {
			if (fabs(d[i]) < FLT_EPSILON) {
				if (p[i] < min[i] || p[i] > max[i])
					return false; // 交差していない
			}
			else {
				// スラブとの距離を算出
				// t1が近スラブ、t2が遠スラブとの距離
				float odd = 1.0f / d[i];
				float t1 = (min[i] - p[i]) * odd;
				float t2 = (max[i] - p[i]) * odd;
				if (t1 > t2) {
					float tmp = t1; t1 = t2; t2 = tmp;
				}

				if (t1 > t) t = t1;
				if (t2 < t_max) t_max = t2;

				// スラブ交差チェック
				if (t >= t_max)
					return false;
			}
		}

		// 交点
		intersect_pos = pos + ( dir * t ) ;

		return true;
	}

	//----------------------------------------------------------------------------------------------
	bool IsIntersectLineOBB(const Vector3& s, const Vector3& e, const Vector3& aabb_max, const Vector3& aabb_min, const Matrix& obb_rot, Vector3& intersect_pos) {
		if (!IsIntersectRayOBB(s, Vector3::Normalize(e - s), aabb_max, aabb_min, obb_rot, intersect_pos)) {
			return false;
		}
		if ((intersect_pos - s).length() > (e - s).length()) return false;
		return true;
	}

}