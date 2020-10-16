#include "Defines.h"
#include "Ray.h"
#include "Hitable_list.h"
#include "Sphere.h"
#include "MoveSphere.h"
#include "Camera.h"
#include "MoveCamera.h"
#include "Metal.h"
#include "Lambertian.h"
#include "Dielectrics.h"
#include "BVH_list.h"
#include "DiffuseLight.h"
#include "Rectangle.h"
#include "Box.h"
#include "Transform.h"
#include "ConstantMedium.h"

using namespace CppUtil::Basic;
using namespace RTX;
using namespace Define;
using namespace glm;
using namespace std;
const rgb background(0.004f);
rgb ray_color(CppUtil::Basic::Ptr<Ray> ray, Hittable_list& world, int depth)
{
	if (depth <= 0)
		return rgb(0.004f);

	Hit_record rec;
	if (!world.hit(ray, 0.0001f, infinity, rec))
	{
		//auto t = 0.5f * (normalize(ray->direction()).y + 1.0f);
		//rgb white = rgb(1.0f, 1.0f, 1.0f);
		//rgb blue = rgb(0.4f, 0.63f, 1.0f);
		//return 0.75f *((1.0f - t) * white + t * blue);
		return background;
	}

	CppUtil::Basic::Ptr<Ray> scattered;
	rgb attenuation;
	rgb emitted = rec.material->emitted(rec.uv, rec.p);

	if (!rec.material->scatter(ray, rec, attenuation, scattered))
		return emitted;

	return emitted + attenuation * ray_color(scattered, world, depth - 1);
}

Hittable_list creatWorld(float t0, float t1)
{
	const int maxCount = 11;
	Hittable_list world;
	for (int a = -maxCount; a < maxCount; a++) {
		for (int b = -maxCount; b < maxCount; b++) {
			auto choose_mat = random_float();
			vec3 center(a + 0.9f * random_float(), 0.2f, b + 0.9f * random_float());

			if ((center - vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
			{
				if (choose_mat < 0.55f) {
					// diffuse
					auto diffuse = ToPtr(new Lambertian(random(0.0f, 1.0f) * random(0.0f, 1.0f)));
					auto center2 = center + vec3(0, random_float(0.0f, 0.5f), 0.0f);
					world.add(ToPtr(new MoveSphere(center, center2, 0.0f, 1.0f, 0.2f, diffuse)));
				}
				else if (choose_mat < 0.85f) {
					// metal
					auto metal = ToPtr(new Metal((vec3(1.0f) + random_in_unit_sphere()) * 0.5f, random_float() * 0.5f));
					world.add(ToPtr(new Sphere(center, 0.2f, metal)));
				}
				else if (choose_mat < 0.97f)
				{
					// glass
					auto  glass = ToPtr(new Refraction(1.5f + random_float(-1.0f, 1.0f), (vec3(1.0f) + random_in_unit_sphere()) * 0.5f));
					world.add(ToPtr(new Sphere(center, 0.2f, glass)));
				}
				else
				{
					auto ligth = ToPtr(new DiffuseLight((vec3(1.0f) + random()) * 0.5f));
					world.add(ToPtr(new Sphere(center, 0.2f, ligth)));
				}
			}
		}
	}

	world.add(ToPtr(new Sphere(vec3(6.0f, 1.0f, 0.0f), 1.0f, ToPtr(new Metal(0.7f, 0.6f, 0.5f)))));
	world.add(ToPtr(new Sphere(vec3(2.0f, 1.0f, 0.0f), 1.0f, ToPtr(new Refraction(1.5f, 0.98f, 0.98f, 0.98f)))));
	world.add(ToPtr(new Sphere(vec3(2.0f, 1.0f, 0.0f), -0.96f, ToPtr(new Refraction(1.5f, 0.98f, 0.98f, 0.98f, 0.02f)))));
	world.add(ToPtr(new Sphere(vec3(-2.0f, 1.0f, 0.0f), 1.0f, ToPtr(new Lambertian(vec3(0.4f, 0.2f, 0.1f))))));
	world.add(ToPtr(new Sphere(vec3(-6.0f, 1.0f, 0.0f), 1.0f, ToPtr(new Refraction(2.5f, 0.0f, 0.98f, 0.98f)))));
	auto checker = ToPtr(new CheckerTexture(rgb(0.2, 0.3, 0.1), rgb(0.9, 0.9, 0.9)));
	world.add(ToPtr(new Sphere(vec3(0.0f, -300.5f, 0.0f), 300.0f, ToPtr(new Lambertian(checker)))));

	auto BVHNode = ToPtr(new BVH(world, t0, t1));
	world.clear();
	world.add(BVHNode);

	return world;
}

Hittable_list twoSpheres(float t0, float t1)
{
	Hittable_list world;
	auto checker = ToPtr(new CheckerTexture(rgb(0.2, 0.3, 0.1), rgb(0.9, 0.9, 0.9)));
	world.add(ToPtr(new Sphere(vec3(0.0f, 3.0f, 0.0f), 3.0f, ToPtr(new Lambertian(checker)))));
	auto pertext = ToPtr(new NoiseTexture(4.0f));
	world.add(ToPtr(new Sphere(vec3(0.0f, -300.5f, 0.0f), 300.0f, ToPtr(new Lambertian(pertext)))));

	auto BVHNode = ToPtr(new BVH(world, t0, t1));
	world.clear();
	world.add(BVHNode);

	return world;
}

Hittable_list earth() {
	Hittable_list world;
	auto image = ToPtr(new ImageTexture("E://data/earthmap.jpg"));
	world.add(ToPtr(new Sphere(vec3(0.0f, 1.0f, 0.0f), 1.0f, ToPtr(new Lambertian(image)))));
	auto pertext = ToPtr(new NoiseTexture(4.0f));
	world.add(ToPtr(new Sphere(vec3(0.0f, -300.5f, 0.0f), 300.0f, ToPtr(new Lambertian(pertext)))));

	auto difflight = ToPtr(new DiffuseLight(vec3(1.0f)));
	world.add(ToPtr(new RectangleXY(vec2(-0.5f, 0.5f), vec2(0.5f, 1.5f), 2.5f, difflight)));
	world.add(ToPtr(new RectangleYZ(vec2(0.5f, 1.5f), vec2(-0.5f, 0.5f), 2.5f, difflight)));
	world.add(ToPtr(new RectangleXZ(vec2(-0.5f, 0.5f), vec2(-0.5f, 0.5f), 2.5f, difflight)));

	//auto ligth = ToPtr(new DiffuseLight(vec3(2.0f)));
	//world.add(ToPtr(new Sphere(vec3(0.0f, 3.5f, 0.0f), 0.75f, ligth)));

	auto BVHNode = ToPtr(new BVH(world, 0.0f, 0.01f));
	world.clear();
	world.add(BVHNode);

	return world;
}

Hittable_list FinalScene() {
	Hittable_list world;

	auto ground = ToPtr(new Lambertian(vec3(0.48, 0.83, 0.53)));
	const int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = random_float(0.0f, 50.0f);
			auto z1 = z0 + w;

			world.add(ToPtr(new Box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground)));
		}
	}
	auto white = ToPtr(new Lambertian(vec3(0.73f)));
	int ns = 1000;
	for (int j = 0; j < ns; j++)
		world.add(ToPtr(new Sphere(Define::random(400.0f, 550.0f), 10.0f, white)));


	auto difflight = ToPtr(new DiffuseLight(vec3(7.0f)));
	world.add(ToPtr(new RectangleXZ(vec2(123.0f, 423.0f), vec2(147.0f, 412.0f), 600.0f, difflight)));

	auto  glass = ToPtr(new Refraction(1.5f, vec3(0.95f)));
	world.add(ToPtr(new Sphere(vec3(430.0f, 150.0f, 210.0f), 80.0f, glass)));

	auto dielectric = ToPtr(new DiffuseLight(vec3(1.5f)));
	world.add(ToPtr(new Sphere(vec3(220.0f, 280.0f, 300.0f), 50.0f, ToPtr(new Metal(vec3(0.8f))))));

	auto boundary = ToPtr(new Sphere(vec3(160.0f, 130.0f, 230.0f), 60.0f, dielectric));
	world.add(make_shared<ConstantMedium>(boundary, 0.003f, rgb(0.2f, 0.4f, 0.9f)));

	//auto allWorld = ToPtr(new Sphere(vec3(0.0f), 2000.0f, dielectric));
	//world.add(make_shared<ConstantMedium>(allWorld, 0.0001f, rgb(0.95f)));
	//world.add(make_shared<ConstantMedium>(allWorld, 0.0001f, rgb(0.725490196f, 0.47843137f, 0.34117647f)));

	auto image = ToPtr(new ImageTexture("E://data/earthmap.jpg"));
	world.add(ToPtr(new Sphere(vec3(400.0f, 200.0f, 400.0f), 100.0f, ToPtr(new Lambertian(image)))));

	auto pertext = ToPtr(new NoiseTexture(0.4f));
	world.add(ToPtr(new Sphere(vec3(0.0f, 350.0f, 150.0f), 80.0f, ToPtr(new Lambertian(pertext)))));

	auto BVHNode = ToPtr(new BVH(world, 0.0f, 0.01f));
	world.clear();
	world.add(BVHNode);

	return world;
}

//Hittable_list testground() {
//    Hittable_list world;
//
//	auto difflight = ToPtr(new DiffuseLight(vec3(7.0f)));
//	world.add(ToPtr(new RectangleXZ(vec2(123.0f, 423.0f), vec2(147.0f, 412.0f), 600.0f, difflight)));
//
//	auto ground = ToPtr(new Lambertian(vec3(0.48, 0.83, 0.53)));
//    const int boxes_per_side = 20;
//    for (int i = 0; i < boxes_per_side; i++) {
//        for (int j = 0; j < boxes_per_side; j++) {
//            auto w = 80.0;
//            auto x0 = -800.0 + i * w;
//            auto z0 = -800.0 + j * w;
//            auto y0 = 0.0;
//            auto x1 = x0 + w;
//            auto y1 = random_float(0.0f, 50.0f);
//            auto z1 = z0 + w;
//
//            world.add(ToPtr(new Box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground)));
//        }
//    }
//
//    auto BVHNode = ToPtr(new BVH(world, 0.0f, 0.01f));
//    world.clear();
//    world.add(BVHNode);
//
//    return world;
//}

Hittable_list CornellBox() {
	Hittable_list world;

	auto red = ToPtr(new Lambertian(vec3(0.65f, 0.05f, 0.05f)));
	auto white = ToPtr(new Lambertian(vec3(0.73f)));
	auto green = ToPtr(new Lambertian(vec3(0.12f, 0.45f, 0.15f)));
	auto light = ToPtr(new DiffuseLight(vec3(15.0f)));

	world.add(ToPtr(new RectangleXZ(vec2(213.0f, 343.0f), vec2(227.0f, 332.0f), 554.0f, light)));
	world.add(ToPtr(new RectangleYZ(vec2(0.0f, 555.0f), vec2(0.0f, 555.0f), 555.0f, green)));
	world.add(ToPtr(new RectangleYZ(vec2(0.0f, 555.0f), vec2(0.0f, 555.0f), 0.0f, red)));
	world.add(ToPtr(new RectangleXZ(vec2(0.0f, 555.0f), vec2(0.0f, 555.0f), 0.0f, white)));
	world.add(ToPtr(new RectangleXZ(vec2(0.0f, 555.0f), vec2(0.0f, 555.0f), 555.0f, white)));
	world.add(ToPtr(new RectangleXY(vec2(0.0f, 555.0f), vec2(0.0f, 555.0f), 555.0f, white)));

	auto box1 = ToPtr(new Box(vec3(0.0f), vec3(165.0f, 330.0f, 165.0f), white));
	auto rot1 = ToPtr(new RotateY(box1, 15.0f));
	auto trans1 = ToPtr(new Translate(rot1, vec3(265.0f, 0.0f, 295.0f)));
	world.add(trans1);

	auto box2 = ToPtr(new Box(vec3(0.0f), vec3(165.0f), white));
	auto rot2 = ToPtr(new RotateY(box2, -18.0f));
	auto trans2 = ToPtr(new Translate(rot2, vec3(130.0f, 0.5f, 65.0f)));
	world.add(trans2);

	auto BVHNode = ToPtr(new BVH(world, 0.0f, 0.01f));
	world.clear();
	world.add(BVHNode);

	return world;
}

int main(int argc, char** argv) {
	ImgWindow imgWindow(str_WindowTitle);
	if (!imgWindow.IsValid()) {
		printf("ERROR: Image Window Create Fail.\n");
		return 1;
	}

	Image& img = imgWindow.GetImg();
	const int val_ImgWidth = img.GetWidth();
	const int val_ImgHeight = img.GetHeight();
	const int val_ImgChannel = img.GetChannel();
	//World
	//Hittable_list world(std::move(creatWorld(0.0f, 0.1f)));
	//Hittable_list world(std::move(twoSpheres(0.0f, 0.1f)));
	Hittable_list world(std::move(FinalScene()));


	vec3 lookfrom(278.0f, 278.0f, -600.0f);
	vec3 lookat(278.0f, 278.0f, 0.0f);
	vec3 vup(0.0f, 1.0f, 0.0f);
	float aspect_ratio = 1.0f * img.GetWidth() / img.GetHeight();
	auto dist_to_focus = 10.0f;
	auto aperture = 0.1f;

	auto camera = ToPtr(new MoveCamera(lookfrom, lookat, vup, 40.0f,
		aspect_ratio, dist_to_focus, aperture, 0.0f, 1.0f));

	vec2 part = 1.0f / vec2(val_ImgWidth, val_ImgHeight);
#if 0
	size_t maxSumLoop = 50;
	size_t curSumLoop = 0;
	const int maxDepth = 50;
	CppUtil::Basic::Timer timer;
	timer.Start();
	Ptr<Operation> imgUpdate = ToPtr(new LambdaOp([&]()
	{
#pragma omp parallel for schedule(dynamic, 4)
		for (int i = 0; i < val_ImgWidth; i++)
		{
			for (int j = 0; j < val_ImgHeight; j++)
			{
				float u = (i + random_float()) * part.x;
				float v = (j + random_float()) * part.y;
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
		double wholeTime = timer.GetWholeTime();
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
					float u = (i + random_float()) * part.x;
					float v = (j + random_float()) * part.y;
					auto ray = camera->GenRay(u, v);
					pixelColor += ray_color(ray, world, maxDepth);
				}
				pixelColor /= samples_per_pixel;
				img.SetPixel(val_ImgWidth - 1 - i, j, Image::Pixel<float>(pixelColor.r, pixelColor.g, pixelColor.b));
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
