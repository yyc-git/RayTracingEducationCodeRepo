#include "AABB.h"

using namespace RTX;
using namespace glm;

bool AABB::hit(CppUtil::Basic::Ptr<Ray> ray, float tmin, float tmax) const 
{
    for (int a = 0; a < 3; a++) {
#if 1
        auto invD = 1.0f / ray->direction()[a];
        auto t0 = (min()[a] - ray->origin()[a]) * invD;
        auto t1 = (max()[a] - ray->origin()[a]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if (tmax <= tmin)
            return false;
#else
        auto t0 = fmin((_min[a] - ray->origin()[a]) / ray->direction()[a],
            (_max[a] - ray->origin()[a]) / ray->direction()[a]);
        auto t1 = fmax((_min[a] - ray->origin()[a]) / ray->direction()[a],
            (_max[a] - ray->origin()[a]) / ray->direction()[a]);
        tmin = fmax(t0, tmin);
        tmax = fmin(t1, tmax);
        if (tmax <= tmin)
            return false;
#endif
    }
    auto ptr = const_cast<AABB*>(this);
    return ptr->isvalid = true;
}

AABB AABB::surroundingBox(const AABB& box0, const AABB& box1)
{
    vec3 small(fmin(box0.min().x, box1.min().x),
        fmin(box0.min().y, box1.min().y),
        fmin(box0.min().z, box1.min().z));

    vec3 big(fmax(box0.max().x, box1.max().x),
        fmax(box0.max().y, box1.max().y),
        fmax(box0.max().z, box1.max().z));

    return AABB(small, big);
}