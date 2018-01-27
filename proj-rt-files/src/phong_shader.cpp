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

      Ray shadow_obj(intersection_point, lnorm);
      //
      //shadow_obj.endpoint = intersection_point;


      Hit hit1;

      Object *object1 = world.Closest_Intersection(shadow_obj, hit1);


      if(!world.enable_shadows
          || (object1 == NULL && world.enable_shadows)
          || ((l.normalized() - intersection_point).magnitude() <= (shadow_obj.Point(hit1.t) - intersection_point).magnitude() && world.enable_shadows))
        {
          double distance = (intersection_point - world.lights.at(i)->position).magnitude_squared();
          //dist = distance.magnitude_squared();

          //light color from lab
          light_color = (world.lights.at(i)->Emitted_Light(ray)) / distance;

          //l*n dot product
          double ln_DP = dot(n, l.normalized());

          //implement diffuse radiancy
          color += std::max(0.0, ln_DP) * color_diffuse * light_color;

          vec3 reflectionMod, cameraMod;


          cameraMod = world.camera.position - intersection_point;

          reflectionMod = 2 * dot(l.normalized(),n)*n - l.normalized();


          //dot product of reflection and Camera
          double refCam_DP = dot(reflectionMod, cameraMod.normalized());

          //hopefully correct specular implementation
          double specular_intensity = pow(std::max(0.0,refCam_DP),specular_power);
          color += specular_intensity * color_specular * light_color;


        }
      }

    return color;
}
