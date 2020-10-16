#ifndef AABB_H
#define AABB_H

#include <CppUtil/Basic/HeapObj.h>
#include <glm/glm.hpp>
#include "Ray.h"

#define Optimized 1
namespace RTX 
{
	using namespace glm;
    using namespace glm;
    class AABB{
    public:
        AABB() :_min(vec3(0.0f)), _max(vec3(0.0f)) {}
        AABB(const vec3& a, const vec3& b) :_min(a), _max(b) {}

        vec3 min() const { return _min; }
        vec3 max() const { return _max; }

        bool hit(CppUtil::Basic::Ptr<Ray> ray, float tmin, float tmax) const;
        static AABB surroundingBox(const AABB& box0, const AABB& box1);
    private:
        bool isvalid = false;
        vec3 _min;
        vec3 _max;
    };
}
#endif //!_RAY_H_
