#include "MoveSphere.h"
#include <CppUtil/Basic/Math.h>

using namespace RTX;
using namespace CppUtil::Basic;
using namespace glm;

MoveSphere::MoveSphere(const vec3& center0, const vec3& center1, float t0, float t1, float radius, CppUtil::Basic::Ptr<Material> material):
    center0(center0), center1(center1), time0(t0), time1(t1), radius(radius), material(material){}

bool MoveSphere::hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, Hit_record& rec) const
{
	vec3 oc = ray->origin() - center(ray->time());
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
			rec.set_face_normal(ray, (rec.p - center(ray->time())) / radius);
			rec.material = material;
			return true;
		}

		temp = (-half_b + root) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = ray->at(rec.t);
			rec.set_face_normal(ray, (rec.p - center(ray->time())) / radius);
			rec.material = material;
			return true;
		}
	}
	return false;
}

vec3 MoveSphere::center(float time) const
{
    return center0 + (center1 - center0) * ((time - time0) / (time1 - time0));
}

bool MoveSphere::boundingBox(float t0, float t1, AABB& output_box) const 
{
    AABB box0(center(t0) - vec3(radius), center(t0) + vec3(radius));
    AABB box1(center(t1) - vec3(radius), center(t1) + vec3(radius));
    output_box = AABB::surroundingBox(box0, box1);
    return true;
}