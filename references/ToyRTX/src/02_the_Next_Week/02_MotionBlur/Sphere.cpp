#include "Sphere.h"
#include "Ray.h"
#include <CppUtil/Basic/Math.h>

using namespace RTX;
using namespace CppUtil::Basic;
using namespace glm;

Sphere::Sphere(const vec3 & center, float radius, CppUtil::Basic::Ptr<Material> material)
	: center(center), radius(radius), material(material){}

bool Sphere::hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, Hit_record& rec) const
{
	vec3 oc = ray->origin() - center;
	auto dir = ray->direction();
	auto a = dot(dir, dir);
	auto half_b = dot(oc, dir);
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = half_b * half_b - a * c;

	if (discriminant > 0) {
		auto root = sqrt(discriminant);

		auto temp = (-half_b - root) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = ray->at(rec.t);
			rec.set_face_normal(ray, (rec.p - center) / radius);
			rec.material = material;
			return true;
		}

		temp = (-half_b + root) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = ray->at(rec.t);
			rec.set_face_normal(ray, (rec.p - center) / radius);
			rec.material = material;
			return true;
		}
	}
	return false;
}
