#ifndef _SPHERE_H_
#define _SPHERE_H_
#include "Hitable.h"

namespace RTX {
	using namespace glm;
	using namespace RTX;
	class Sphere : public Hitable 
	{
	public:
		Sphere(const vec3 & center = vec3(0.0f), float radius = 1.0f,CppUtil::Basic::Ptr<Material> material = nullptr);
		virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, Hit_record& rec) const override;

		vec3& Center()  { return center; }
		const vec3 & Center() const { return center; }
		float Radius() { return radius; }
	protected:
		glm::vec3 center;
		float radius;
		CppUtil::Basic::Ptr<Material> material;
	};
}
#endif 
