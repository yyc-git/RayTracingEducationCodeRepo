#ifndef _BVH_H
#define _BVH_H

#include "Hitable_list.h"
#include "Ray.h"

namespace RTX {
    class BVH : public Hitable {
    public:
        BVH() {}
        BVH(Hittable_list& objects, float t0, float t1) : BVH(objects, 0, objects.objects.size(), t0, t1) {}

        BVH(Hittable_list& objects, size_t start, size_t end, float time0, float time1);

        virtual bool boundingBox(float t0, float t1, AABB& output_box) const override;

        virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, Hit_record& rec) const override;
    public:
        Hitable::CPtr left;
        Hitable::CPtr right;
        AABB box;
    };
}
#endif