#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <CppUtil/Basic/HeapObj.h>
#include <glm/glm.hpp>
#include "Ray.h"
namespace RTX {
	using namespace glm;
	class Camera : public CppUtil::Basic::HeapObj {
		HEAP_OBJ_SETUP(Camera)
	public:
		// fov is top to bottom in degrees
		Camera()
		{
			auto aspect_ratio = 16.0 / 9.0;
			auto viewport_height = 2.0;
			auto viewport_width = aspect_ratio * viewport_height;
			auto focal_length = 1.0;

			origin = vec3(0.0f);
			horizontal = vec3(viewport_width, 0.0f, 0.0f);
			vertical = vec3(0.0f, viewport_height, 0.0f);
			lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - vec3(0.0f, 0.0f, focal_length);
		}

		virtual CppUtil::Basic::Ptr<Ray> GenRay(float u, float v) const
		{
			return ToPtr(new Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin));
		}

	private:
		vec3 origin;
		vec3 lower_left_corner;
		vec3 horizontal;
		vec3 vertical;
	};
}

#endif // !_RAY_CAMERA_H_
