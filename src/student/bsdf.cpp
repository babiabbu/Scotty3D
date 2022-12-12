
#include "../rays/bsdf.h"
#include "../util/rand.h"

namespace PT {

static Vec3 reflect(Vec3 dir) {

    // TODO (PathTracer): Task 5
    // Return reflection of dir about the surface normal (0,1,0).
    return Vec3{};
}

static Vec3 refract(Vec3 out_dir, float index_of_refraction, bool& was_internal) {

    // TODO (PathTracer): Task 5
    // Use Snell's Law to refract out_dir through the surface.
    // Return the refracted direction. Set was_internal to true if
    // refraction does not occur due to total internal reflection,
    // and false otherwise.

    // When dot(out_dir,normal=(0,1,0)) is positive, then out_dir corresponds to a
    // ray exiting the surface into vaccum (ior = 1). However, note that
    // you should actually treat this case as _entering_ the surface, because
    // you want to compute the 'input' direction that would cause this output,
    // and to do so you can simply find the direction that out_dir would refract
    // _to_, as refraction is symmetric.

    return Vec3{};
}

Scatter BSDF_Lambertian::scatter(Vec3 out_dir) const {
    //根据光线的出射方向，对光线的入射方向进行采样
    // TODO (PathTracer): Task 4

    // Sample the BSDF distribution using the cosine-weighted hemisphere sampler.
    // You can use BSDF_Lambertian::evaluate() to compute attenuation.
    //中文：使用余弦加权半球采样器对BSDF分布进行采样。可以使用BSDF_Ambertian:：evaluate（）计算衰减。

    //所有函数是在射线与物体交点处的局部坐标系下进行计算的，
    //交点为局部坐标系下的原点，交点处面积微元的法向为n = (0, 1, 0)。
    Scatter ret;
    ret.direction = sampler.sample();
    ret.attenuation = evaluate(out_dir,ret.direction);
    //ret.direction = Vec3{};
    //ret.attenuation = Spectrum{};
    return ret;
}

Spectrum BSDF_Lambertian::evaluate(Vec3 out_dir, Vec3 in_dir) const {

    // TODO (PathTracer): Task 4

    // Compute the ratio of reflected/incoming radiance when light from in_dir
    // is reflected through out_dir: albedo * cos(theta).
    //计算来自in_dir的光线时反射/入射辐射的比率,通过out_dir反射：反照率* cos（θ）。
    //theta是入射/反射光与法向量的夹角
    return 0 < in_dir.y ? albedo* in_dir.y : Spectrum{};
}

float BSDF_Lambertian::pdf(Vec3 out_dir, Vec3 in_dir) const {

    // TODO (PathTracer): Task 4

    // Compute the PDF for sampling in_dir from the cosine-weighted hemisphere distribution.
    //返回一个pdf值,pdf是经过采样的出射光线的概率密度,当L为恒定值，概率密度即为cos/pi
    float cos_theta = in_dir.y;
    //float cos = dot(out_dir, in_dir) / out_dir.norm() ;
    float pdf=cos_theta / PI_F;
    return pdf;
}

Scatter BSDF_Mirror::scatter(Vec3 out_dir) const {

    // TODO (PathTracer): Task 5

    Scatter ret;
    ret.direction = Vec3();
    ret.attenuation = Spectrum{};
    return ret;
}

Scatter BSDF_Glass::scatter(Vec3 out_dir) const {

    // TODO (PathTracer): Task 5

    // (1) Compute Fresnel coefficient. Tip: Schlick's approximation.
    // (2) Reflect or refract probabilistically based on Fresnel coefficient. Tip: RNG::coin_flip
    // (3) Compute attenuation based on reflectance or transmittance

    // Be wary of your eta1/eta2 ratio - are you entering or leaving the surface?
    // What happens upon total internal reflection?

    Scatter ret;
    ret.direction = Vec3();
    ret.attenuation = Spectrum{};
    return ret;
}

Scatter BSDF_Refract::scatter(Vec3 out_dir) const {

    // OPTIONAL (PathTracer): Task 5

    // When debugging BSDF_Glass, it may be useful to compare to a pure-refraction BSDF

    Scatter ret;
    ret.direction = Vec3();
    ret.attenuation = Spectrum{};
    return ret;
}

Spectrum BSDF_Diffuse::emissive() const {
    return radiance;
}

} // namespace PT
