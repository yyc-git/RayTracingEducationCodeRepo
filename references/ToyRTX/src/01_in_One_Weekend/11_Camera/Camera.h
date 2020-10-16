#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <CppUtil/Basic/HeapObj.h>
#include <glm/glm.hpp>
#include "Ray.h"
#include "Defines.h"
namespace RTX {
    using namespace glm;
    using namespace Define;
    class Camera : public CppUtil::Basic::HeapObj {
        HEAP_OBJ_SETUP(Camera)
    public:
        // fov is top to bottom in degrees
        Camera(vec3 lookfrom, vec3 lookat, vec3 vup,
            float vfov, float aspect_ratio)
        {
            float theta = degrees_to_radians(vfov);
            float h = tan(theta / 2.0f);
            float viewport_height = 2.0f * h;
            float viewport_width = aspect_ratio * viewport_height;

            auto w = normalize(lookfrom - lookat);
            auto u = normalize(cross(vup, w));
            auto v = cross(w, u);

            origin = lookfrom;
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - w;
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
