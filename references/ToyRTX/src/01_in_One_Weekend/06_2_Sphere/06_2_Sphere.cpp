#include <CppUtil/RTX/ImgWindow.h>
#include <CppUtil/Basic/Image.h>
#include <CppUtil/Basic/LambdaOp.h>
#include <CppUtil/Basic/Timer.h>

#include "Defines.h"
#include "Ray.h"

using namespace CppUtil::Basic;
using namespace RTX;
using namespace Define;
using namespace glm;
using namespace std;

float hit_sphere(const vec3& center, float radius, CppUtil::Basic::Ptr<Ray> ray)
{
	vec3 oc = ray->origin() - center;
	auto dir = ray->direction();
	auto a = dot(dir, dir);
	auto half_b = dot(oc, dir);
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = half_b * half_b - a * c;
	return discriminant < 0 ? -1.0f : (-half_b - sqrt(discriminant)) / a;
}

rgb ray_color(CppUtil::Basic::Ptr<Ray> ray)
{
	auto t = hit_sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, ray);
	if (t > 0.0f)
		return 0.5f * (ray->at(t) - vec3(0.0f, 0.0f, -1.0f) + 1.0f);

	t = 0.5f * (normalize(ray->direction()).y + 1.0f);
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
//	auto imgUpdate = ToPtr(new LambdaOp([&]()
//	{
//#pragma omp parallel for schedule(dynamic, 8)
//		for (int i = 0; i < val_ImgWidth; i++)
//		{
//			for (int j = 0; j < val_ImgHeight; j++)
//			{
//				float u = i * part.x;
//				float v = j * part.y;
//				CppUtil::Basic::Ptr<Ray> ray = ToPtr(new Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin));
//				rgb pixel = ray_color(ray);
//				img.SetPixel(val_ImgWidth - 1 - i, j, Image::Pixel<float>(pixel.r, pixel.g, pixel.b));
//			}
//		}
//	}, false));

    Timer timer;
    timer.Start( );
    size_t maxSumLoop = 500;
    size_t curSumLoop = 0;
    Ptr<Operation> imgUpdate = ToPtr(new LambdaOp([&]( ) {
        //size_t curLoop = static_cast<size_t>(glm::max(imgWindow.GetScale(), 1.0));
        int imgSize = val_ImgWidth * val_ImgHeight;
#pragma omp parallel for schedule(dynamic, 4)
        for (int pixelIdx = 0; pixelIdx < imgSize; pixelIdx++) {
            const uvec2 pixel(pixelIdx % val_ImgWidth, pixelIdx / val_ImgWidth);
            float u = pixel.x * part.x;
            float v = pixel.y * part.y;
            CppUtil::Basic::Ptr<Ray> ray = ToPtr(new Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin));
            rgb currentColor = ray_color(ray);
            //vec3 rst = rayTracer.TraceX(scene->obj, scene->camera->GenRay(u, v));

            auto _color = img.GetPixel_F(pixel.x, pixel.y);
            vec3 color(_color.r, _color.g, _color.b);
            vec3 newColor = (color*(float)curSumLoop + currentColor) / ((float)curSumLoop + 1);
            img.SetPixel(pixel.x, pixel.y, newColor);
        }
        curSumLoop++;
        double curStep = curSumLoop / (double)maxSumLoop * 100;
        double wholeTime = timer.GetWholeTime( );
        double speed = curSumLoop / wholeTime;
        double needTime = (maxSumLoop - curSumLoop) / speed;
        double sumTime = wholeTime + needTime;
        printf("\rINFO: %.2f%%, %.2f loop / s, use %.2f s, need %.2f s, sum %.2f s     ",
            curStep, speed, wholeTime, needTime, sumTime);

        if (curSumLoop == maxSumLoop) {
            printf("\n");
            imgUpdate->SetIsHold(false);
        }
    }));

	imgWindow.Run(imgUpdate);
	return 0;
}
