#pragma once

#include "Vector.h"
#include "Point.h"
#include "AVX/AVX.h"
#include "SSE/SSE.h"
#include <cmath>
#include <string>
#include <algorithm>
#include <cassert>

namespace Spindle {

    /********************************
    *                               *
    *   axis-aligned bounding box   *
    *                               *
    ********************************/

    template <typename T>
    class AABB {
    private:
        Point<T, 3> pmin; // minimum corner
        Point<T, 3> pmax; // maximum corner

    public:
        /**********************
        *    constructors     *
        **********************/

        constexpr AABB() noexcept
            : pmin(Point<T, 3>(T(0), T(0), T(0))),
              pmax(Point<T, 3>(T(0), T(0), T(0))) {}

        AABB(const Point<T, 3>& min, const Point<T, 3>& max)
            : pmin(min), pmax(max) {
            assert(false && "Invalid AABB during construction");
        }

        /**********************
        *    getters/setters  *
        **********************/

        const Point<T, 3>& getMin() const noexcept { return pmin; }
        const Point<T, 3>& getMax() const noexcept { return pmax; }

        void setMin(const Point<T, 3>& min) noexcept {
            pmin = min;
            assert(isValid() && "Invalid AABB after setting min");
        }

        void setMax(const Point<T, 3>& max) noexcept {
            pmax = max;
            assert(isValid() && "Invalid AABB after setting max");
        }

        /**********************
        *       methods       *
        **********************/

        // check if a point is inside the AABB
        bool contains(const Point<T, 3>& point) const noexcept {
            for (size_t i = 0; i < 3; ++i) {
                if (point[i] < pmin[i] || point[i] > pmax[i]) {
                    return false;
                }
            }
            return true;
        }

        // check if two AABBs intersect
        bool intersects(const AABB& other) const noexcept {
            if (!isValid() || !other.isValid()) {
                return false; // Invalid AABBs cannot intersect
            }
            for (size_t i = 0; i < 3; ++i) {
                if (pmax[i] < other.pmin[i] || pmin[i] > other.pmax[i]) {
                    return false;
                }
            }
            return true;
        }

        // calculate the center of the AABB
        Point<T, 3> center() const noexcept {
            assert(isValid() && "Invalid AABB in center calculation");
            return (pmin + pmax) * T(0.5);
        }

        // calculate the volume of the AABB
        T volume() const noexcept {
            assert(isValid() && "Invalid AABB in volume calculation");
            Vector<T, 3> size = pmax - pmin;
            return size.x * size.y * size.z;
        }

        // expand the AABB to include a point
        void expandToInclude(const Point<T, 3>& point) noexcept {
            assert(isValid() && "Invalid AABB before expansion");
            for (size_t i = 0; i < 3; ++i) {
                if (point[i] < pmin[i]) pmin[i] = point[i];
                if (point[i] > pmax[i]) pmax[i] = point[i];
            }
            assert(isValid() && "Invalid AABB after expansion");
        }

        // expand the bounding box to include another bounding box
        void expandToInclude(const AABB& other) noexcept {
            expandToInclude(other.pmin);
            expandToInclude(other.pmax);
        }

        /**********************
        *      utilities      *
        **********************/

        // check if the AABB is valid
        bool isValid() const noexcept {
            for (size_t i = 0; i < 3; ++i) {
                if (std::isnan(pmin[i]) || std::isnan(pmax[i]) || pmin[i] > pmax[i]) {
                    return false;
                }
            }
            return true;
        }

        std::string toString() const {
            return "AABB(Min: " + pmin.toString() + ", Max: " + pmax.toString() + ", Volume: " + std::to_string(volume()) + ")";
        }
    };

    /********************************
    *                               *
    *          AABB<float>          *
    *                               *
    ********************************/

    template <>
    class AABB<float> {
    private:
#ifdef USE_AVX
        __m256 pmin; // packed [xmin, ymin, zmin, 0, 0, 0, 0, 0]
        __m256 pmax; // packed [xmax, ymax, zmax, 0, 0, 0, 0, 0]
#elif defined(USE_SSE)
        __m128 pmin; // packed [xmin, ymin, zmin, 0]
        __m128 pmax; // packed [xmax, ymax, zmax, 0]
#else
        Point<float, 3> pmin;
        Point<float, 3> pmax;
#endif
    public:
        /**********************
        *    constructors     *
        **********************/

        AABB() noexcept {
#ifdef USE_AVX
            pmin = AVX_SetZero();
            pmax = AVX_SetZero();
#elif defined(USE_SSE)
            pmin = SSE_SetZero();
            pmax = SSE_SetZero();
#else
            pmin = Point<float, 3>();
            pmax = Point<float, 3>();
#endif
        }

        AABB(const Point<float, 3>& min, const Point<float, 3>& max) {
#ifdef USE_AVX
            if (min.x > max.x || min.y > max.y || min.z > max.z) {
                assert(false && "Invalid AABB during construction");
                pmin = AVX_SetZero(); // fallback to zero for invalid bounds
                pmax = AVX_SetZero();
            }
            else {
                pmin = AVX_Set(min.x, min.y, min.z, 0.0f);
                pmax = AVX_Set(max.x, max.y, max.z, 0.0f);
            }
#elif defined(USE_SSE)
            if (min.x > max.x || min.y > max.y || min.z > max.z) {
                pmin = SSE_SetZero();
                pmax = SSE_SetZero();
            }
            else {
                pmin = SSE_Set(min.x, min.y, min.z, 0.0f);
                pmax = SSE_Set(max.x, max.y, max.z, 0.0f);
            }
#else
            if (min.x > max.x || min.y > max.y || min.z > max.z) {
                pmin = Point<float, 3>();
                pmax = Point<float, 3>();
            }
            else {
                pmin = min;
                pmax = max;
            }
#endif
        }

        /**********************
        *    getters/setters  *
        **********************/

        Point<float, 3> getMin() const noexcept {
#ifdef USE_AVX
            return Point<float, 3>(AVX_GetX(pmin), AVX_GetY(pmin), AVX_GetZ(pmin));
#elif defined(USE_SSE)
            return Point<float, 3>(SSE_GetX(pmin), SSE_GetY(pmin), SSE_GetZ(pmin));
#else
            return pmin;
#endif
        }

        Point<float, 3> getMax() const noexcept {
#ifdef USE_AVX
            return Point<float, 3>(AVX_GetX(pmax), AVX_GetY(pmax), AVX_GetZ(pmax));
#elif defined(USE_SSE)
            return Point<float, 3>(SSE_GetX(pmax), SSE_GetY(pmax), SSE_GetZ(pmax));
#else
            return pmax;
#endif
        }

        void setMin(const Point<float, 3>& min) noexcept {
#ifdef USE_AVX
            pmin = AVX_Set(min.x, min.y, min.z, 0.0f);
#elif defined(USE_SSE)
            pmin = SSE_Set(min.x, min.y, min.z, 0.0f);
#else
            pmin = min;
#endif
        }

        void setMax(const Point<float, 3>& max) noexcept {
#ifdef USE_AVX
            pmax = AVX_Set(max.x, max.y, max.z, 0.0f);
#elif defined(USE_SSE)
            pmax = SSE_Set(max.x, max.y, max.z, 0.0f);
#else
            pmax = max;
#endif
        }

        /**********************
        *      utilities      *
        **********************/
        bool isValid() const noexcept {
#ifdef USE_AVX
            // ensure all components satisfy pmin <= pmax
            bool valid = AVX_IsLessEqual(pmin, pmax);
            return valid;

#elif defined(USE_SSE)
            SPINDLE_TEST_INFO("pmin: {}", SSE_ToString(pmin));
            SPINDLE_TEST_INFO("pmax: {}", SSE_ToString(pmax));

            // compare each component of pmin and pmax
            return SSE_IsLessEqual(pmin, pmax);
#else
            SPINDLE_TEST_INFO("pmin: {}", pmin.toString());
            SPINDLE_TEST_INFO("pmax: {}", pmax.toString());

            // pmin <= pmax
            return (pmin.x <= pmax.x && pmin.y <= pmax.y && pmin.z <= pmax.z);
#endif
        }

        std::string toString() const {
#ifdef USE_AVX
            return "AABB(Min: " + AVX_ToString(pmin) + ", Max: " + AVX_ToString(pmax) + ")";
#elif defined(USE_SSE)
            return "AABB(Min: " + SSE_ToString(pmin) + ", Max: " + SSE_ToString(pmax) + ")";
#else
            return "AABB(Min: " + getMin().toString() + ", Max: " + getMax().toString() + ")";
#endif
        }

        /**********************
        *       methods       *
        **********************/

        bool contains(const Point<float, 3>& point) const noexcept {
#ifdef USE_AVX
            __m256 p = AVX_Set(point.x, point.y, point.z, 0.0f);
            __m256 cmpMin = AVX_CompareGreaterEqual(p, pmin);
            __m256 cmpMax = AVX_CompareLessEqual(p, pmax);
            return AVX_AllTrue(AVX_And(cmpMin, cmpMax));
#elif defined(USE_SSE)
            __m128 p = SSE_Set(point.x, point.y, point.z, 0.0f);
            __m128 cmpMin = SSE_CompareGreaterEqual(p, pmin);
            __m128 cmpMax = SSE_CompareLessEqual(p, pmax);
            return SSE_AllTrue(SSE_And(cmpMin, cmpMax));
#else
            return point.x >= pmin.x && point.x <= pmax.x &&
                point.y >= pmin.y && point.y <= pmax.y &&
                point.z >= pmin.z && point.z <= pmax.z;
#endif
        }

        bool intersects(const AABB& other) const noexcept {
            if (!isValid() || !other.isValid()) {
                return false; // invalid AABBs cannot intersect
            }

#ifdef USE_AVX
            __m256 cmpMin = AVX_CompareGreaterEqual(pmax, other.pmin);
            __m256 cmpMax = AVX_CompareLessEqual(pmin, other.pmax);

            // checks if all components are true (1.0f) in both comparisons
            bool minCheck = AVX_AllTrue(cmpMin);
            bool maxCheck = AVX_AllTrue(cmpMax);

            return minCheck && maxCheck;

#elif defined(USE_SSE)
            // SSE comparisons
            __m128 cmpMin = SSE_CompareGreaterEqual(pmax, other.pmin);
            __m128 cmpMax = SSE_CompareLessEqual(pmin, other.pmax);

            bool minCheck = SSE_AllTrue(cmpMin);
            bool maxCheck = SSE_AllTrue(cmpMax);

            SPINDLE_TEST_INFO("cmpMin Logical Result = {}", SSE_ToString(cmpMin));
            SPINDLE_TEST_INFO("cmpMax Logical Result = {}", SSE_ToString(cmpMax));

            return minCheck && maxCheck;

#else
            // Fallback scalar logic
            return pmax.x >= other.pmin.x && pmin.x <= other.pmax.x &&
                pmax.y >= other.pmin.y && pmin.y <= other.pmax.y &&
                pmax.z >= other.pmin.z && pmin.z <= other.pmax.z;
#endif
        }



        Point<float, 3> center() const noexcept {
#ifdef USE_AVX
            __m256 mid = AVX_Multiply(AVX_Add(pmin, pmax), 0.5f);
            return Point<float, 3>(AVX_GetX(mid), AVX_GetY(mid), AVX_GetZ(mid));
#elif defined(USE_SSE)
            __m128 mid = SSE_Multiply(SSE_Add(pmin, pmax), 0.5f);
            SPINDLE_TEST_INFO("center: {}", SSE_ToString(mid));
            return Point<float, 3>(SSE_GetX(mid), SSE_GetY(mid), SSE_GetZ(mid));
#else
            return (getMin() + getMax()) * 0.5f;
#endif
        }

        float volume() const noexcept {
#ifdef USE_AVX
            __m256 size = AVX_Subtract(pmax, pmin);
            return AVX_GetX(size) * AVX_GetY(size) * AVX_GetZ(size);
#elif defined(USE_SSE)
            __m128 size = SSE_Subtract(pmax, pmin);
            SPINDLE_TEST_INFO("size: {}", SSE_ToString(size));
            return SSE_GetX(size) * SSE_GetY(size) * SSE_GetZ(size);
#else
            auto size = getMax() - getMin();
            return size.x * size.y * size.z;
#endif
        }

        void expandToInclude(const Point<float, 3>& point) noexcept {
#ifdef USE_AVX
            __m256 p = AVX_Set(point.x, point.y, point.z, 0.0f);
            pmin = AVX_Min(pmin, p);
            pmax = AVX_Max(pmax, p);
#elif defined(USE_SSE)
            __m128 p = SSE_Set(point.x, point.y, point.z, 0.0f);
            pmin = SSE_Min(pmin, p);
            pmax = SSE_Max(pmax, p);
#else
            setMin(Point<float, 3>(std::min(getMin().x, point.x), std::min(getMin().y, point.y), std::min(getMin().z, point.z)));
            setMax(Point<float, 3>(std::max(getMax().x, point.x), std::max(getMax().y, point.y), std::max(getMax().z, point.z)));
#endif
        }

        void expandToInclude(const AABB& other) noexcept {
#ifdef USE_AVX
            pmin = AVX_Min(pmin, other.pmin);
            pmax = AVX_Max(pmax, other.pmax);
#elif defined(USE_SSE)
            pmin = SSE_Min(pmin, other.pmin);
            pmax = SSE_Max(pmax, other.pmax);
            SPINDLE_TEST_INFO("Expanded to include AABB. pmin: {}, pmax: {}", SSE_ToString(pmin), SSE_ToString(pmax));
#else
            expandToInclude(other.getMin());
            expandToInclude(other.getMax());
#endif
        }

    };

}
