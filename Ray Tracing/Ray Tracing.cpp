#include "./utility/rtweekend.h"

#include "./utility/color.h"
#include "./utility/hittable_list.h"
#include "./utility/sphere.h"
#include "./utility/camera.h"

#include <iostream>

// t^2 b⋅b+2tb⋅(A−C)+(A−C)⋅(A−C)−r^2=0
// 判断光线与球是否相交
// 球体中心坐标 | 半径 | 射线
double hit_sphere(const point3& center, double radius, const ray& r) {
    //获取球心到交点的正交向量
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0) { //没相交
        return -1.0;
    }else {                 //相切或相交
        return (-half_b - sqrt(discriminant)) / a ;
    }
}

//计算交点值
color ray_color(const ray& r, const hittable& world) {
    
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    //线性插值
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;

    //World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    //world.add(make_shared<sphere>(point3(1, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));


    //Camera, 视角在原点
    camera cam;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {

        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i) {
            // 对像素点周围随机采样samples_per_pixel次,最后将采样到的颜色累加
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            //将累加的颜色除以采样数,还原颜色
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone. \n";
}
