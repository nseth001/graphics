#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth,bool is_exiting) const
{
    vec3 color;
    vec3 reflectorColor;
    Ray reflector;
    vec3 shaderColor;




    //phong shader reflective calculations
    vec3 rVal = (ray.direction - 2 * dot(ray.direction, same_side_normal) * same_side_normal);


    reflector.endpoint = intersection_point;
    reflector.direction = rVal;

    //modify cast ray so we can perform the Cast_Ray computation with iterative
    //recursive depth.

    reflectorColor = world.Cast_Ray(reflector, recursion_depth+=1);

    shaderColor = shader->Shade_Surface(ray, intersection_point, same_side_normal, recursion_depth,is_exiting);

    color = (reflectivity * reflectorColor + (1 - reflectivity)* shaderColor);

    return color;
}
