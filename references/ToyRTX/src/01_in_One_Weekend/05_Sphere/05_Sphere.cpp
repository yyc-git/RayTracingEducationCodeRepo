#include <CppUtil/RTX/ImgWindow.h>
#include <CppUtil/Basic/Image.h>
#include <CppUtil/Basic/LambdaOp.h>

#include "Defines.h"
#include "Ray.h"

using namespace CppUtil::Basic;
using namespace RTX;
using namespace Define;
using namespace glm;
using namespace std;

bool hit_sphere(const vec3& center, float radius, CppUtil::Basic::Ptr<Ray> ray)
{
	vec3 oc = ray->origin() - center;
	auto dir = ray->direction();
	auto a = dot(dir, dir);
	auto b = 2.0f * dot(oc, dir);
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = b * b - 4.0f * a*c;
	return (discriminant > 0);
}

rgb ray_color(CppUtil::Basic::Ptr<Ray> ray) 
{
	if (hit_sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, ray))
		return rgb(1.0f, 0.0f, 0.0f);

	float t = 0.5f * (normalize(ray->direction()).y + 1.0f);
	rgb white = rgb(1.0f, 1.0f, 1.0f);
	rgb blue = rgb(0.4f, 0.63f, 1.0f);
	return (1.0f - t)*white + t * blue;
}

int main(int argc, char ** argv){
	ImgWindow imgWindow(str_WindowTitle);
	if (!imgWindow.IsValid()) {
		printf("ERROR: Image Window Create Fail.\n");
		return 1;
	}

	Image & img = imgWindow.GetImg();
	const int val_ImgWidth = img.GetWidth();
	const int val_ImgHeight = img.GetHeight();
	const int val_ImgChannel = img.GetChannel();
	// Camera
	const auto aspect_ratio = 1.0f * val_ImgWidth / val_ImgHeight;
	auto viewport_height = 2.0f;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0f;

	auto origin = vec3(0.0f, 0.0f, 0.0f);
	auto horizontal = vec3(viewport_width, 0.0f, 0.0f);
	auto vertical = vec3(0, viewport_height, 0.0f);
	auto lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - vec3(0.0f, 0.0f, focal_length);

	vec2 part = 1.0f / vec2(val_ImgWidth, val_ImgHeight);
	auto imgUpdate = ToPtr(new LambdaOp([&]()
	{
#pragma omp parallel for schedule(dynamic, 8)
		for (int i = 0; i < val_ImgWidth; i++) 
		{
			for (int j = 0; j < val_ImgHeight; j++)
			{
				float u = i * part.x;
				float v = j * part.y;
				CppUtil::Basic::Ptr<Ray> ray = ToPtr(new Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin));
				rgb BGColor = ray_color(ray);
				img.SetPixel(val_ImgWidth - 1 - i, j, Image::Pixel<float>(BGColor.r, BGColor.g, BGColor.b));
			}
		}
	}, false));

	imgWindow.Run(imgUpdate);
	return 0;
}
