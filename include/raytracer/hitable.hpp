#ifndef HITABLE_HPP
#define HITABLE_HPP

#include "ray.hpp"

class material;

struct hit_record {
    double t;
    vec3 p;
    vec3 normal;
    material *mat_ptr;
};

class hitable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif