#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

class ray {
    public:
        ray() {}
        ray(const vec3&a, const vec3& b) {A=a; B=b;}

        vec3 origin() const {return A;}
        vec3 direction() const {return B;}
        vec3 point_at_parameter (double t) const {return A +B*t;}
        
        vec3 A,B;
};

#endif