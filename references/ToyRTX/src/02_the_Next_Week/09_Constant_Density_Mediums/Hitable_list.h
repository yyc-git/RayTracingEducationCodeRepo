#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hitable.h"
#include "Ray.h"
#include <vector>

namespace RTX {
    class Hittable_list : public Hitable {
    public:
        Hittable_list() {}
        Hittable_list(Hitable::CPtr object) { add(object); }

        void clear() { objects.clear(); }
        void add(Hitable::CPtr object) { objects.push_back(object); }
        virtual bool boundingBox(float t0, float t1, AABB& output_box) const override
        {
            if (objects.empty()) return false;

            AABB temp_box;
            bool first_box = true;

            for (const auto& object : objects) {
                if (!object->boundingBox(t0, t1, temp_box)) return false;
                output_box = first_box ? temp_box : AABB::surroundingBox(output_box, temp_box);
                first_box = false;
            }

            return true;
        }

        virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, Hit_record& rec) const override
        {
            Hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = t_max;

            for (const auto& object : objects)
            {
                if (object->hit(ray, t_min, closest_so_far, temp_rec))
                {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return hit_anything;
        }
    public:
        std::vector<Hitable::CPtr> objects;
    };
}
#endif