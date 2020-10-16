#include <CppUtil/Basic/Math.h>

#include "Rectangle.h"
#include "Ray.h"
#include "Defines.h"

using namespace RTX;
using namespace CppUtil::Basic;
using namespace glm;

RectangleXY::RectangleXY(const vec2& x, const vec2& y, float k, CppUtil::Basic::Ptr<Material> material)
	:x(x), y(y), k(k), material(material){}

bool RectangleXY::hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, Hit_record& rec) const
{
	auto temp = (k - ray->origin().z) / ray->direction().z;
	if (temp > t_max || temp < t_min)
		return false;
	auto xt = ray->origin().x + temp * ray->direction().x;
	auto yt = ray->origin().y + temp * ray->direction().y;
	if (xt < x.x || xt > x.y || yt < y.x || yt > y.y)
		return false;

	rec.uv = vec2((xt - x.x) / (x.y - x.x), (yt - y.x) / (y.y - y.x));
	rec.t = temp;
	auto outward_normal = vec3(0, 0, 1);
	rec.set_face_normal(ray, outward_normal);
	rec.material = this->material;
	rec.p = ray->at(temp);
	return true;
}

bool RectangleXY::boundingBox(float t0, float t1, AABB& output_box) const
{
	output_box = AABB(vec3(x.x, y.x, k - 0.001f), vec3(x.y, y.y, k + 0.001f));
	return true;
}

RectangleYZ::RectangleYZ(const vec2& y, const vec2& z, float k, CppUtil::Basic::Ptr<Material> material)
	:y(y), z(z), k(k), material(material) {}

bool RectangleYZ::hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, Hit_record& rec) const
{
	auto temp = (k - ray->origin().x) / ray->direction().x;
	if (temp > t_max || temp < t_min)
		return false;
	//auto xt = ray->origin().x + temp * ray->direction().x;
	auto yt = ray->origin().y + temp * ray->direction().y;
	auto zt = ray->origin().z + temp * ray->direction().z;
	if (yt < y.x || yt > y.y || zt < z.x || zt > z.y)
		return false;

	rec.uv = vec2((yt - y.x) / (y.y - y.x), (zt - z.x) / (z.y - z.x));
	rec.t = temp;
	auto outward_normal = vec3(1.0f, 0.0f, 0.0f);
	rec.set_face_normal(ray, outward_normal);
	rec.material = this->material;
	rec.p = ray->at(temp);
	return true;
}

bool RectangleYZ::boundingBox(float t0, float t1, AABB& output_box) const
{
	output_box = AABB(vec3(k - 0.001f, y.x, z.x), vec3(k + 0.001f, y.y, z.y));
	return true;
}

RectangleXZ::RectangleXZ(const vec2& x, const vec2& z, float k, CppUtil::Basic::Ptr<Material> material)
	:x(x), z(z), k(k), material(material) {}

bool RectangleXZ::hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, Hit_record& rec) const
{
	auto temp = (k - ray->origin().y) / ray->direction().y;
	if (temp > t_max || temp < t_min)
		return false;
	auto xt = ray->origin().x + temp * ray->direction().x;
	//auto yt = ray->origin().y + temp * ray->direction().y;
	auto zt = ray->origin().z + temp * ray->direction().z;
	if (xt < x.x || xt > x.y || zt < z.x || zt > z.y)
		return false;

	rec.uv = vec2((xt - x.x) / (x.y - x.x), (zt - z.x) / (z.y - z.x));
	rec.t = temp;
	auto outward_normal = vec3(0.0f, 1.0f, 0.0f);
	rec.set_face_normal(ray, outward_normal);
	rec.material = this->material;
	rec.p = ray->at(temp);
	return true;
}

bool RectangleXZ::boundingBox(float t0, float t1, AABB& output_box) const
{
	output_box = AABB(vec3(x.x, k - 0.001f, z.x), vec3(x.y, k + 0.001f, z.y));
	return true;
}