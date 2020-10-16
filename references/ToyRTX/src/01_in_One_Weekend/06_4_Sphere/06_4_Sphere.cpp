#include <CppUtil/RTX/ImgWindow.h>
#include <CppUtil/Basic/Image.h>
#include <CppUtil/Basic/LambdaOp.h>
#include <CppUtil/Basic/Timer.h>

#include "Defines.h"
#include "Ray.h"
#include "Hitable_list.h"
#include "Sphere.h"

using namespace CppUtil::Basic;
using namespace RTX;
using namespace Define;
using namespace glm;
using namespace std;

rgb ray_color(CppUtil::Basic::Ptr<Ray> ray, Hittable_list& world)
{
	Hit_record rec;
	if (world.hit(ray, 0.0f, infinity, rec))
		return 0.5f * (rec.normal + rgb(1.0f));

	auto t = 0.5f * (normalize(ray->direction()).y + 1.0f);
	rgb white = rgb(1.0f, 1.0f, 1.0f);
	rgb blue = rgb(0.4f, 0.63f, 1.0f);
	return (1.0f - t)*white + t * blue;
}

int main(int argc, char ** argv) {
	ImgWindow imgWindow(str_WindowTitle);
	if (!imgWindow.IsValid()) {
		printf("ERROR: Image Window Create Fail.\n");
		return 1;
	}

	Image & img = imgWindow.GetImg();
	const int val_ImgWidth = img.GetWidth();
	const int val_ImgHeight = img.GetHeight();
	const int val_ImgChannel = img.GetChannel();

	//World
	Hittable_list world;
	world.add(ToPtr(new Sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f)));
	world.add(ToPtr(new Sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f)));
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
	float schedule = 0.0f;
	auto imgUpdate = ToPtr(new LambdaOp([&]()
	{
		Timer timer;
		timer.Start();
#pragma omp parallel for schedule(dynamic, 8)
		for (int i = 0; i < val_ImgWidth; i++)
		{
			for (int j = 0; j < val_ImgHeight; j++)
			{
				float u = i * part.x;
				float v = j * part.y;
				CppUtil::Basic::Ptr<Ray> ray = ToPtr(new Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin));
				rgb pixel = ray_color(ray, world);
				img.SetPixel(val_ImgWidth - 1 - i, j, Image::Pixel<float>(pixel.r, pixel.g, pixel.b));
			}
			float tmp = (i + 1) * 100.0f / val_ImgWidth;
			if (tmp > schedule)
			{
				schedule = tmp;
				float wholeTime = timer.GetWholeTime();
				printf("\rINFO: %.2f%%     wholeTime: %.2fs        ", tmp, wholeTime);
			}
		}
	}, false));

	imgWindow.Run(imgUpdate);
	return 0;
}
