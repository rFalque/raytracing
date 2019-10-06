
#ifndef SHPERE_HPP
#define SHPERE_HPP

#include "hitable.hpp"

class sphere : public hitable {
    public:
        sphere() {}
        sphere(vec3 cen, double r, material *mat) {center = cen; radius =r; mat_ptr = mat;}
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
        vec3 center;
        double radius;
        material *mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = dot(oc, r.direction());
    double c = dot(oc, oc) - radius*radius;
    double discriminant = b*b - a*c;
    if (discriminant > 0) {
        double temp = (-b - sqrt(b*b - a*c))/a;
        if (t_min < temp && temp < t_max)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center)/radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(b*b - a*c))/a;
        if (t_min < temp && temp < t_max)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center)/radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

#endif