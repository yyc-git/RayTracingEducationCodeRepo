#ifndef _HITABLE_H_
#define _HITABLE_H_ 

#include <CppUtil/Basic/HeapObj.h>
#include <glm/glm.hpp>
#include "Ray.h"

namespace RTX 
{
	using namespace glm;
	class Material;
	struct Hit_record {
		float t;
		bool front_face;
		vec3 p;
		vec3 normal;
		CppUtil::Basic::Ptr<Material> material;

		inline void set_face_normal(CppUtil::Basic::Ptr<Ray> ray, const vec3& outward_normal) 
		{
			front_face = dot(ray->direction(), outward_normal) < 0;
			normal = front_face ? outward_normal : -outward_normal;
		}
	};
	class Hitable : public CppUtil::Basic::HeapObj {
		HEAP_OBJ_SETUP(Hitable)
	public:
		virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, Hit_record& rec) const = 0;
	};
}
#endif