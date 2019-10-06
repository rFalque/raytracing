#include <iostream>
#include <limits>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <raytracer/renderingOptions.hpp>
#include <raytracer/vec3.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/hitable_list.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/camera.hpp>
#include <raytracer/materials.hpp>


hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i=1;
    for (int a=-11; a < 11 ; a++) {
        for (int b=-11; b<11; b++) {
            double choose_mat = drand48();
            vec3 center(a+0.9*drand48(), 0.2, b+0.9*drand48());

            if ((vec3(4, 0.2, 0) - center).length() > 0.9) {
                if (choose_mat <0.8) { // difuse
                    list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
                } else if (choose_mat < 0.95) {
                    list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1+drand48()), 0.5*(1+drand48()), 0.5*(1+drand48())), 0.5*drand48() ));
                } else {
                    list[i++] = new sphere(center, 0.2, new diaelectric(1.5) );
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0,1,0), 1.0, new diaelectric(1.5) );
    list[i++] = new sphere(vec3(4,1,0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0) );
    list[i++] = new sphere(vec3(-4,1,0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)) );
    
    return new hitable_list(list, i);
}


// generate background color + color of balls
vec3 color(const ray& r, hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, std::numeric_limits< double >::max(), rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth <50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth+1);
        } else {
            return vec3(0,0,0);
        }
    } else {
        vec3 unit_direction = unit_vector(r.direction());
        double t = 0.5*(unit_direction.y()+1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

int main() {
    // load options
    renderingOptions opts;
    opts.loadYAML("../config.yaml");

    // pre-assign array for RGB image of size opts.nx*opts.ny
    unsigned char img_rgb[opts.nx*opts.ny*3];

    /*
    hitable *list[5];
    list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    list[1] = new sphere(vec3(0, -100.5, 1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new diaelectric(1.5));
    list[4] = new sphere(vec3(-1, 0, -1), -0.45, new diaelectric(1.5));
    hitable *world = new hitable_list(list, 5);
    */

    hitable *world = random_scene();

    camera cam(opts.lookfrom, opts.lookat, vec3(0,1,0), opts.fov, double(opts.nx)/double(opts.ny), opts.aperture, opts.dist_to_focus);

    for (int j = opts.ny-1; j>=0; j--) {
        for (int i=0; i<opts.nx; i++) {
            vec3 col(0,0,0);
            for (int s=0; s<opts.ns; s++) {
                double u = double(i+drand48()) / double(opts.nx);
                double v = double(j+drand48()) / double(opts.ny);
                ray r = cam.get_ray(u,v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world, 0);
            }
            col /= double(opts.ns);
            col = vec3 (sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            int ir = int(255.99*col.r());
            int ig = int(255.99*col.g());
            int ib = int(255.99*col.b());

            img_rgb[ (i + (opts.ny-1-j)*opts.nx)*3 + 0] = ir;
            img_rgb[ (i + (opts.ny-1-j)*opts.nx)*3 + 1] = ig;
            img_rgb[ (i + (opts.ny-1-j)*opts.nx)*3 + 2] = ib;
        }
    }

    stbi_write_bmp(opts.output_path.c_str(), opts.nx, opts.ny, 3, img_rgb);

}
