#include <random>
#include "tnl_math.h"

namespace tnl {

    std::mt19937 g_mersenne ;
    void SetSeedMersenneTwister32(int seed) {
        g_mersenne.seed(seed);
    }

    float GetRandomDistributionFloat(float min, float max) {
        std::uniform_real_distribution<> param(static_cast<double>(min), static_cast<double>(max));
        return static_cast<float>(param(g_mersenne));
    }


	int GetSidesPointAndPlane(const Vector3& v, const Vector3& pn, const Vector3& pv)
	{
		Vector3 vec = v - pv;
		float a = vec.dot( pn );
		if (a > 0.0f) 		return 1;		// •\
		else if (a < 0.0f)	return -1;		// — 
		else 				return 0;		// •½–Êã
	}


	int GetRegionPointAndRect(const Vector3& p, const Vector3& rp, const int rect_w, const int rect_h) {
		tnl::Vector3 v1 = tnl::Vector3::Normalize({ float(rect_h), float(rect_w), 0 });
		tnl::Vector3 v2 = tnl::Vector3::Normalize({ float(rect_h), float(-rect_w), 0 });
//		tnl::Vector3 vc1 = v1.cross(tnl::Vector3(0, 0, 1));
//		tnl::Vector3 vc2 = v2.cross(tnl::Vector3(0, 0, 1));
		tnl::Vector3 vc1 = v1;
		tnl::Vector3 vc2 = v2;

		int s1 = tnl::GetSidesPointAndPlane(p, vc1, rp);
		int s2 = tnl::GetSidesPointAndPlane(p, vc2, rp);
		if (s1 >= 0 && s2 >= 0) {
			return 1;
		}
		else if (s1 >= 0 && s2 <= 0) {
			return 2;
		}
		else if (s1 <= 0 && s2 >= 0) {
			return 0;
		}
		else {
			return 3;
		}
	}


    int GetXzRegionPointAndOBB(const Vector3& p, const Vector3& op, const Vector3& size, const Quaternion& q) {
        tnl::Vector3 pv1 = tnl::Vector3::Normalize(size) * 0.5f ;
        tnl::Vector3 pv2 = {-pv1.x, pv1.y, pv1.z};
        tnl::Vector3 pn1 = tnl::Vector3::Normalize( tnl::Vector3::Cross(pv1, pv1.xz()) );
        tnl::Vector3 pn2 = tnl::Vector3::Normalize( tnl::Vector3::Cross(pv2, pv2.xz()) );
        pv1 = op + tnl::Vector3::TransformCoord(pv1, q);
        pv2 = op + tnl::Vector3::TransformCoord(pv2, q);
        pn1 = tnl::Vector3::TransformCoord(pn1, q);
        pn2 = tnl::Vector3::TransformCoord(pn2, q);

        int s1 = tnl::GetSidesPointAndPlane(p, pn1, pv1);
        int s2 = tnl::GetSidesPointAndPlane(p, pn2, pv2);
        if (s1 >= 0 && s2 >= 0) {
            return 1;
        }
        else if (s1 >= 0 && s2 <= 0) {
            return 2;
        }
        else if (s1 <= 0 && s2 >= 0) {
            return 0;
        }
        else {
            return 3;
        }
    }


	tnl::Vector3 GetNearestRectPoint(const tnl::Vector3& rect_pos, float w, float h, const tnl::Vector3 &point) {
		tnl::Vector3 nearest(0, 0, 0);
		float hw = w * 0.5f;
		float hh = h * 0.5f;
		float l = rect_pos.x - hw;
		float r = rect_pos.x + hw;
		float t = rect_pos.y - hh;
		float b = rect_pos.y + hh;
		nearest.x = (point.x > l && point.x < r) ? point.x : (point.x > l) ? r : l;
		nearest.y = (point.y > t && point.y < b) ? point.y : (point.y > t) ? b : t;
		return nearest;
	}


    tnl::Vector3 GetNearestPointPlane(const tnl::Vector3& v, const tnl::Vector3& pn, const tnl::Vector3& pv) {
        float d = tnl::Vector3::Dot(pn, pv);
        float t = tnl::Vector3::Dot(pn, v) - d;
        return v - (pn * t);
    }


    Vector3 BezierSpline(const Vector3& _a1, const Vector3& _a2, const Vector3& _a3, const Vector3& _a4, float t) {

        t = (1.0f < t) ? 1.0f : t;
        t = (0.0f > t) ? 0.0f : t;

        float wkT = (1.0f - t);
        float wkX, wkY, wkZ;
        wkX = wkY = wkZ = 0.0f;

        float T2 = (t * t);
        float T3 = (t * t * t);
        float wkT2 = (wkT * wkT);
        float wkT3 = (wkT * wkT * wkT);

        wkX = ((wkT3)*_a1.x);
        wkX = wkX + ((3 * (wkT2)) * (t * _a2.x));
        wkX = wkX + (((3 * wkT) * (T2)) * (_a3.x));
        wkX = wkX + ((T3)*_a4.x);

        wkY = ((wkT3)*_a1.y);
        wkY = wkY + ((3 * (wkT2)) * (t * _a2.y));
        wkY = wkY + (((3 * wkT) * (T2)) * (_a3.y));
        wkY = wkY + ((T3)*_a4.y);

        wkZ = ((wkT3)*_a1.z);
        wkZ = wkZ + ((3 * (wkT2)) * (t * _a2.z));
        wkZ = wkZ + (((3 * wkT) * (T2)) * (_a3.z));
        wkZ = wkZ + ((T3)*_a4.z);

        return Vector3( wkX, wkY, wkZ );
    }



    CubicSpline::CubicSpline(const std::vector<tnl::Vector3>& v) {

        int n = static_cast<int>(v.size()) - 1;

        for (int i = 0; i <= n; ++i) {
            a_.emplace_back(v[i]);
        }

        for (int i = 0; i <= n; ++i) {
            if (i == 0) {
                c_.emplace_back(tnl::Vector3{ 0, 0, 0 });
            }
            else if (i == n) {
                c_.emplace_back(tnl::Vector3{ 0, 0, 0 });
            }
            else {
                c_.emplace_back(tnl::Vector3{
                    3.0f * (a_[i - 1].x - 2.0f * a_[i].x + a_[i + 1].x),
                    3.0f * (a_[i - 1].y - 2.0f * a_[i].y + a_[i + 1].y),
                    3.0f * (a_[i - 1].z - 2.0f * a_[i].z + a_[i + 1].z)
                    });
            }
        }

        for (int i = 0; i < n; ++i) {
            if (i == 0) {
                w_.emplace_back(tnl::Vector3{ 0, 0, 0 });
            }
            else {
                float x = 4.0f - w_[i - 1].x;
                float y = 4.0f - w_[i - 1].y;
                float z = 4.0f - w_[i - 1].z;
                c_[i].x = (c_[i].x - c_[i - 1].x) / x;
                c_[i].y = (c_[i].y - c_[i - 1].y) / y;
                c_[i].z = (c_[i].z - c_[i - 1].z) / z;
                w_.emplace_back(tnl::Vector3{ 1.0f / x, 1.0f / y, 1.0f / z });
            }
        }

        for (int i = (n - 1); i > 0; --i) {
            c_[i] = c_[i] - c_[i + 1] * w_[i];
        }

        for (int i = 0; i <= n; ++i) {
            if (i == n) {
                d_.emplace_back(tnl::Vector3{ 0,0,0 });
                b_.emplace_back(tnl::Vector3{ 0,0,0 });
            }
            else {
                d_.emplace_back((c_[i + 1] - c_[i]) / 3.0f);
                b_.emplace_back(a_[i + 1] - a_[i] - c_[i] - d_[i]);
            }
        }

    }


    PointsLerp::PointsLerp(const std::vector< tnl::Vector3 >& points) {
        points_.resize(points.size());
        section_lengths_.resize(points.size());
        std::copy(points.begin(), points.end(), points_.begin());

        for (size_t i = 1; i < points_.size(); ++i) {
            section_lengths_[i] = (points_[i - 1] - points_[i]).length();
            all_length_ += section_lengths_[i];
            section_lengths_[i] += section_lengths_[i - 1];
        }
    }
    float PointsLerp::getLengthRate(float t) const noexcept {
        float n = 0;
        t = modff(t, &n);
        t = (t < 0.0f) ? 1.0f + t : t;
        return all_length_ * t;
    }

    float PointsLerp::getLengthPoint(int pn) const noexcept {
        if (pn < 0 || pn >= (int)section_lengths_.size()) return 0;
        return section_lengths_[pn];
    }

    int PointsLerp::getPrevPoint(float t) const noexcept {
        float d = getLengthRate(t);
        float c = 0.0f;
        for (size_t i = 0; i < section_lengths_.size(); ++i) {
            if (d >= section_lengths_[i]) continue;
            return i - 1;
        }
        return 0;
    }
}