#include <CppUtil/RTX/ImgWindow.h>
#include <CppUtil/Basic/Image.h>
#include <CppUtil/Basic/LambdaOp.h>
#include <CppUtil/Basic/Timer.h>

#include "Defines.h"
#include "Ray.h"
#include "Hitable_list.h"
#include "Sphere.h"
#include "Camera.h"
#include "Metal.h"
#include "Lambertian.h"
using namespace CppUtil::Basic;
using namespace RTX;
using namespace Define;
using namespace glm;
using namespace std;

rgb ray_color(CppUtil::Basic::Ptr<Ray> ray, Hittable_list& world, int depth)
{
	if (depth <= 0)
		return rgb(0.004f);
	Hit_record rec;
	if (world.hit(ray, 0.0001f, infinity, rec))
	{
		CppUtil::Basic::Ptr<Ray> scattered;
		rgb attenuation;
		if (rec.material->scatter(ray, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return rgb(0, 0, 0);
	}

	auto t = 0.5f * (normalize(ray->direction()).y + 1.0f);
	rgb white = rgb(1.0f, 1.0f, 1.0f);
	rgb blue = rgb(0.4f, 0.63f, 1.0f);
	return (1.0f - t) * white + t * blue;
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
	//不知道什么原因官方给的金色渲染的结果很奇怪，因此查了一些资料设置了这个金色，看起来似乎更接近一些
	world.add(ToPtr(new Sphere(vec3(-1.1f, 0.0f, -1.0f), 0.5f, ToPtr(new Metal(0.98f, 0.839f, 0.35f,0.05f)))));
	world.add(ToPtr(new Sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, ToPtr(new Lambertian(0.7f, 0.3f, 0.3f)))));
	world.add(ToPtr(new Sphere(vec3(1.1f, 0.0f, -1.0f), 0.5f,ToPtr(new Metal(0.8f,0.8f,0.8f)))));
	world.add(ToPtr(new Sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f, ToPtr(new Lambertian(0.8f, 0.8f, 0.0f)))));
	// Camera
	Camera::Ptr camera = ToPtr(new Camera());

    vec2 part = 1.0f / vec2(val_ImgWidth, val_ImgHeight);
#if 0
    size_t maxSumLoop = 50;
    size_t curSumLoop = 0;
    const int maxDepth = 50;
    CppUtil::Basic::Timer timer;
    timer.Start( );
    Ptr<Operation> imgUpdate = ToPtr(new LambdaOp([&]( )
    {
#pragma omp parallel for schedule(dynamic, 4)
        for (int i = 0; i < val_ImgWidth; i++)
        {
            for (int j = 0; j < val_ImgHeight; j++)
            {
                float u = (i + random_float( ))* part.x;
                float v = (j + random_float( ))* part.y;
                auto ray = camera->GenRay(u, v);
                auto currentColor = ray_color(ray, world, maxDepth);

                auto imgColor = img.GetPixel_F(val_ImgWidth - 1 - i, j);
                vec3 color(imgColor.r, imgColor.g, imgColor.b);
                vec3 newColor = (color * (float)curSumLoop + currentColor) / ((float)curSumLoop + 1.0f);
                img.SetPixel(val_ImgWidth - 1 - i, j, Image::Pixel<float>(newColor.r, newColor.g, newColor.b));
            }
        }
        ++curSumLoop;
        double curStep = curSumLoop / (double)maxSumLoop * 100;
        double wholeTime = timer.GetWholeTime( );
        double speed = curSumLoop / wholeTime;
        double needTime = (maxSumLoop - curSumLoop) / speed;
        double sumTime = wholeTime + needTime;
        printf("\rINFO: %.2f%%, %.2f loop / s, use %.2f s, need %.2f s, sum %.2f s     ",
            curStep, speed, wholeTime, needTime, sumTime);
        if (curSumLoop == maxSumLoop)
            imgUpdate->SetIsHold(false);
    }));
#else
    const int samples_per_pixel = 50;
    const int maxDepth = 50;
    float schedule = 0.0f;
    auto imgUpdate = ToPtr(new LambdaOp([&]()
    {
        Timer timer;
        timer.Start();
#pragma omp parallel for schedule(dynamic, 4)
        for (int i = 0; i < val_ImgWidth; i++)
        {
            for (int j = 0; j < val_ImgHeight; j++)
            {
                rgb pixelColor(0.0f);
                for (int s = 0; s < samples_per_pixel; ++s)
                {
                    float u = (i + random_float( ))* part.x;
                    float v = (j + random_float( ))* part.y;
                    auto ray = camera->GenRay(u, v);
                    pixelColor += ray_color(ray, world, maxDepth);
                }
                pixelColor /= samples_per_pixel;
                img.SetPixel(val_ImgWidth - 1 - i, j, Image::Pixel<float>(sqrt(pixelColor.r), sqrt(pixelColor.g), sqrt(pixelColor.b)));
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
#endif

	imgWindow.Run(imgUpdate);
	return 0;
}
