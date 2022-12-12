
#include "../rays/pathtracer.h"
#include "../rays/samplers.h"
#include "../util/rand.h"
#include "debug.h"

#define TASK_4 1//执行
#define TASK_6 0

namespace PT {

Spectrum Pathtracer::trace_pixel(size_t x, size_t y) {

    
    Vec2 xy((float)x, (float)y);
    Vec2 wh((float)out_w, (float)out_h);

    // xy是方形的左下角，需要在1*1的像素内进行超采样
    Vec2 Samplexy = Samplers::Rect().sample(); //取随机点
    Vec2 debug(0.0f, 0.0f);

    Ray ray = camera.generate_ray((Samplexy + xy) / wh);
    ray.depth = max_depth;

    if(RNG::coin_flip(0.0002f)) {
        log_ray(ray, 10.0f); //我喜欢白色
    }
    // Pathtracer::trace() returns the incoming light split into emissive and reflected components.
    auto [emissive, reflected] = trace(ray);
    return emissive + reflected;
}

Spectrum Pathtracer::sample_indirect_lighting(const Shading_Info& hit) {
    
    // TODO (PathTrace): Task 4

    // This function computes a single-sample Monte Carlo estimate of the _indirect_
    // lighting at our ray intersection point.
    Vec3 out_dir_inobj = hit.out_dir; // object坐标系下的out_dir，原本的就是object下的
    //Vec3 out_dir = hit.object_to_world.rotate(out_dir_inobj).unit();
    // (1) Randomly sample a new ray direction from the BSDF distribution using BSDF::scatter().
    Scatter scatter = hit.bsdf.scatter(out_dir_inobj);
    Vec3 in_dir_inobj = scatter.direction;
    Vec3 in_dir = hit.object_to_world.rotate(in_dir_inobj).unit();
    // (2) Create a new world-space ray and call Pathtracer::trace() to get incoming light. You
    // should modify time_bounds so that the ray does not intersect at time = 0. Remember to
    // set the new depth value.
    Ray ray =
        Ray(hit.pos, in_dir, Vec2{EPS_F, std::numeric_limits<float>::infinity()}, hit.depth - 1);
    //ray.depth = //射线的递归深度
    // Remember to set the new depth value.
        Spectrum radiance = Pathtracer::trace(ray).second;//递归

    // (3) Add contribution due to incoming light scaled by BSDF attenuation. Whether you
    // compute the BSDF scattering PDF should depend on if the BSDF is a discrete distribution
    // (see BSDF::is_discrete()).
    radiance *= hit.bsdf.evaluate(out_dir_inobj, in_dir_inobj);
    
    if(!hit.bsdf.is_discrete()) {
        radiance = radiance / hit.bsdf.pdf(out_dir_inobj, in_dir_inobj);
    }
    // You should only use the indirect component of incoming light (the second value returned
    // by Pathtracer::trace()), as the direct component will be computed in
    // Pathtracer::sample_direct_lighting().
    return radiance;
}

Spectrum Pathtracer::sample_direct_lighting(const Shading_Info& hit) {
   
    // This function computes a Monte Carlo estimate of the _direct_ lighting at our ray
    // intersection point by sampling both the BSDF and area lights.

    // Point lights are handled separately, as they cannot be intersected by tracing rays
    // into the scene.
    Spectrum radiance = point_lighting(hit);
    
    
    
    

    // For task 4, this function should perform almost the same sampling procedure as
    // Pathtracer::sample_indirect_lighting(), but instead accumulates the emissive component of
    // incoming light (the first value returned by Pathtracer::trace()). Note that since we only
    // want emissive, we can trace a ray with depth = 0.
   
#if TASK_4 == 1

    // TODO (PathTrace): Task 4
    Vec3 out_dir_inobj = hit.out_dir; // object坐标系下的out_dir，原本的就是object下的
    //Vec3 out_dir = hit.object_to_world.rotate(out_dir_inobj).unit();
    Scatter scatter = hit.bsdf.scatter(out_dir_inobj);
    Vec3 in_dir_inobj = scatter.direction;
    Vec3 in_dir = hit.object_to_world.rotate(in_dir_inobj).unit();

    Ray ray =
        Ray(hit.pos, in_dir, Vec2{EPS_F, std::numeric_limits<float>::infinity()}, hit.depth - 1);
    // ray.depth = //射线的递归深度

    Spectrum radiancetmp =
        Pathtracer::trace(ray).first * hit.bsdf.evaluate(out_dir_inobj, in_dir_inobj);
    if(!hit.bsdf.is_discrete()) {
        radiancetmp = radiancetmp / hit.bsdf.pdf(out_dir_inobj, in_dir_inobj);
    }
    radiance+=radiancetmp;
#endif

    // TODO (PathTrace): Task 6

    // For task 6, we want to upgrade our direct light sampling procedure to also
    // sample area lights using mixture sampling.

    // (1) If the BSDF is discrete, we don't need to bother sampling lights: the behavior
    // should be the same as task 4.

    // (2) Otherwise, we should randomly choose whether we get our sample from `BSDF::scatter`
    // or `Pathtracer::sample_area_lights`. Note that `Pathtracer::sample_area_lights` returns
    // a world-space direction pointing toward an area light. Choose between the strategies
    // with equal probability. Pay attention to the inputs and outputs of the area light functions -
    // they are in world space, while BSDF::scatter() is in local space. Use the attributes of the
    // Pathtracer::Shading_Info object to make sure spaces are consistent when you create a ray to
    // trace.

    // (3) Create a new world-space ray and call Pathtracer::trace() to get incoming light. You
    // should modify time_bounds so that the ray does not intersect at time = 0. We are again
    // only interested in the emissive component, so the ray depth can be zero.

    // (4) Add estimate of incoming light scaled by BSDF attenuation. Given a sample,
    // we don't know whether it came from the BSDF or the light, so you should use BSDF::evaluate(),
    // BSDF::pdf(), and Pathtracer::area_lights_pdf() to compute the proper weighting.
    // What is the PDF of our sample, given it could have been produced from either source?
#if TASK_6 == 1
        return radiance
#endif

        return radiance;
}

std::pair<Spectrum, Spectrum> Pathtracer::trace(const Ray& ray) {
    /*
     * 我们需要仔细阅读该函数并对其进行修改，去掉返回物体表面法向颜色的代码。
     */

    // This function orchestrates the path tracing process. For convenience, it
    // returns the incoming light along a ray in two components: emitted from the
    // surface the ray hits, and reflected through that point from other sources.

    // Trace ray into scene.
    Trace result = scene.hit(ray);
    if(!result.hit) {

        // If no surfaces were hit, sample the environemnt map.
        if(env_light.has_value()) {
            return {env_light.value().evaluate(ray.dir), {}};
        }
        return {};
    }

    // If we're using a two-sided material, treat back-faces the same as front-faces
    const BSDF& bsdf = materials[result.material];
    if(!bsdf.is_sided() && dot(result.normal, ray.dir) > 0.0f) {
        result.normal = -result.normal;
    }

    // TODO (PathTracer): Task 4
    // You will want to change the default normal_colors in debug.h, or delete this early out.
    //我们需要仔细阅读该函数并对其进行修改，去掉返回物体表面法向颜色的代码。
     //if(debug_data.normal_colors) return {Spectrum::direction(result.normal), {}};

    // If the BSDF is emissive, stop tracing and return the emitted light
    //是发光源，直接返回光源到相机视线内
    Spectrum emissive = bsdf.emissive();
    if(emissive.luma() > 0.0f) return {emissive, {}};

    // If the ray has reached maximum depth, stop tracing
    if(ray.depth == 0) return {};

    // Set up shading information
    Mat4 object_to_world = Mat4::rotate_to(result.normal);
    Mat4 world_to_object = object_to_world.T();
    Vec3 out_dir = world_to_object.rotate(ray.point - result.position).unit();

    Shading_Info hit = {bsdf,    world_to_object, object_to_world, result.position,
                        out_dir, result.normal,   ray.depth};

    // Sample and return light reflected through the intersection
    return {emissive, sample_direct_lighting(hit) + sample_indirect_lighting(hit)};
}

} // namespace PT
