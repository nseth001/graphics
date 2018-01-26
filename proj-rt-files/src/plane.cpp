#include "plane.h"
#include "ray.h"
#include <cfloat>


// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
bool Plane::
Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    // TODO
    //implementing  plane intersection
    //declaration
    vec3 x1, v, u, normal;
    double intersectVal, t;
    x1 = this->x1;
    v = ray.direction;
    u = ray.endpoint;
    normal = this->normal;

    intersectVal = dot(normal, v);
    t = -1*(dot(normal,(u-x1)) / intersectVal);

    Hit hitObj = {this, t, true};

    if (t >= 0)
    {
      hits.push_back(hitObj);
      return true;
    }



    return false;
}

vec3 Plane::
Normal(const vec3& point) const
{
    return normal;
}
