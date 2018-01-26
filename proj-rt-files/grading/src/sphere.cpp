#include "sphere.h"
#include "ray.h"


// Determine if the ray intersects with the sphere
bool Sphere::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    // TODO
    vec3 u;
    vec3 v;
    vec3 w;
    vec3 center;
    double a, b, c, radius;
    //finish defs
    center = this->center;
    u = ray.direction;
    w = ray.endpoint;
    v = w - center;

    radius = this->radius;

    a = dot(u,u);
    b = dot(u,v);
    c = dot(v,v) - radius*radius;

    double discr = (b*b) - c;

    if (discr > 0) {
        double t1 = -b - pow(discr, 1/2);
        double t2 = -b + pow(discr, 1/2);

        if (t1 > 0) {
            Hit hit1 = {this, t1, true};
            hits.push_back(hit1);
        }

        if (t2 > 0) {
            Hit hit2 = {this, t2, true};
            hits.push_back(hit2);
        }
    }
    return false;
}

vec3 Sphere::Normal(const vec3& point) const
{
    vec3 normal;
    // TODO: set the normal
    normal = point - center;
    //normal.normalized()
    normal = normal.normalized();
    return normal;
}
