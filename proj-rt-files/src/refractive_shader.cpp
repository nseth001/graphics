#include "refractive_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Refractive_Shader::
Shade_Surface(const Ray& ray, const vec3& intersection_point,
        const vec3& same_side_normal, int recursion_depth,bool is_exiting) const
{
    //Hints: Use REFRACTIVE_INDICES::AIR for air refractive_index
    //       Use is_exiting to decide the refractive indices on the ray and transmission sides
    vec3 reflection_color;
    vec3 refraction_color;
    double reflectance_ratio=-1;
    if(!world.disable_fresnel_refraction)
    {
        //TODO (Test 27+): Compute the refraction_color:
        // - Check if it is total internal reflection.
        //      If so update the reflectance_ratio for total internal refraction
        //
        //      else, follow the instructions below
        //
        //        (Test 28+): Update the reflectance_ratio
        //
        //        (Test 27+): Cast the refraction ray and compute the refraction_color
        //
        //declare ni, nr
        double ni;
        double nr;
        vec3 n;
        if (is_exiting) {
          n = -1.0 * same_side_normal;
          ni = refractive_index;
          nr = REFRACTIVE_INDICES::AIR;
        }
        else {
          n = same_side_normal;
          ni = REFRACTIVE_INDICES::AIR;
          nr = refractive_index;
        }
        double nDiv = ni/nr;
        double cosI = dot(-1.0*ray.direction,n) / ray.direction.magnitude();
        double cosR = pow(1.0-nDiv*nDiv*(1.0 - pow(cosI,2.0)),0.5);

        if ((1 - pow(nDiv,2)*(1-pow(cosI,2)))  < 0)
        {
          reflectance_ratio = 1;
          refraction_color = vec3(0,0,0);
        }
        else{

          vec3 tVal = nDiv * (ray.direction.normalized()
                      - dot(ray.direction.normalized(),n)*n)
                      - (cosR*n);

          //now need to cast the computed ray
          Ray refractRay(intersection_point, tVal);
          //not sure if i increment my recursion depth
          refraction_color = shader->world.Cast_Ray(refractRay, recursion_depth+=1);

          //compute refract vars
          double refractParallel = pow( (nr * cosI - ni*cosR)/
                                    (nr*cosI + ni*cosR),2);
          double refractPerpendicular = pow( (ni * cosI - nr*cosR)/
                                    (ni*cosI + nr*cosR),2);

          double k = ((refractPerpendicular + refractParallel) / 2);
          reflectance_ratio = k;


        }
    }

    if(!world.disable_fresnel_reflection){
        //TODO:(Test 26+): Compute reflection_color:
        // - Cast Reflection Ray andd get color
        //
        Ray reflect;
        reflect.endpoint = intersection_point;
        reflect.direction = (ray.direction - 2 * dot(ray.direction, same_side_normal) * same_side_normal);
        reflection_color = world.Cast_Ray(reflect, recursion_depth+=1);
    }

    Enforce_Refractance_Ratio(reflectance_ratio);
    vec3 color;
    // TODO: (Test 26+) Compute final 'color' by blending reflection_color and refraction_color using
    //                  reflectance_ratio
    //
    color = reflectance_ratio * reflection_color + (1 - reflectance_ratio) * refraction_color;
    return color;
}

void Refractive_Shader::
Enforce_Refractance_Ratio(double& reflectance_ratio) const
{
    if(world.disable_fresnel_reflection) reflectance_ratio=0;
    else if(world.disable_fresnel_refraction) reflectance_ratio=1;
}
