#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth,bool is_exiting) const
{
    vec3 color;
    // TODO: determine the color
    //need to calculate ambient colors first?
    color = world.ambient_intensity * world.ambient_color * color_ambient;
    //color = ambient;
    uint i;
    vec3 l, n, light_color;
    for (i = 0; i < world.lights.size(); ++i)
    {
      l = world.lights.at(i)->position - intersection_point;
      n = same_side_normal;
      //normalize l for shadow_obj parameter
      vec3 lnorm = l.normalized();

      double distance = (intersection_point - world.lights.at(i)->position).magnitude_squared();


      Ray shadow_obj;
      //
      shadow_obj.endpoint = world.lights[i]->position;
      shadow_obj.direction = lnorm;


      //Hit hit1;

      //Object *object1 = world.Closest_Intersection(shadow_obj, hit1);


      if(world.enable_shadows) {
          Hit hit2;
          Ray shadow_obj2;
          shadow_obj2.direction = lnorm;
          shadow_obj2.endpoint = intersection_point;

          if (world.Closest_Intersection(shadow_obj2, hit2)){
            if (sqrt(distance) > hit2.t) continue;
          }
        }

          //light color from lab
          light_color = (world.lights.at(i)->Emitted_Light(shadow_obj)) / distance;

          //l*n dot product
          double ln_DP = dot(n, l.normalized());

          //implement diffuse radiancy
          color += std::max(0.0, ln_DP) * color_diffuse * light_color;

          vec3 reflectionMod, cameraMod;


          cameraMod = -1.0 * ray.direction.normalized();

          reflectionMod = 2 * dot(l.normalized(),n)*n - l.normalized();


          //dot product of reflection and Camera
          double refCam_DP = dot(reflectionMod, cameraMod.normalized());

          //hopefully correct specular implementation
          double specular_intensity = pow(std::max(0.0,refCam_DP),specular_power);
          color += specular_intensity * color_specular * light_color;


        }


    return color;
}
