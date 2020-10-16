#ifndef _BOX_H_
#define _BOX_H_
#include "Hitable_list.h"
namespace RTX {
	using namespace glm;
	using namespace RTX;
	class Box : public Hitable
	{
	public:
		Box(const vec3& p0 = vec3(0.0f), const vec3& p1 = vec3(0.0f),CppUtil::Basic::Ptr<Material> material = nullptr);
		virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, Hit_record& rec) const override;
        virtual bool boundingBox(float t0, float t1, AABB& output_box) const override;
	protected:
		vec3 box_min;
		vec3 box_max;
		Hittable_list sides;
	};
}
#endif 
