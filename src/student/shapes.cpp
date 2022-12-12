
#include "../rays/shapes.h"
#include "debug.h"

namespace PT {

const char* Shape_Type_Names[(int)Shape_Type::count] = {"None", "Sphere"};

BBox Sphere::bbox() const {

    BBox box;
    box.enclose(Vec3(-radius));
    box.enclose(Vec3(radius));
    return box;
}

Trace Sphere::hit(const Ray& ray) const {

    // TODO (PathTracer): Task 2
    // Intersect this ray with a sphere of radius Sphere::radius centered at the origin.

    // If the ray intersects the sphere twice, ret should
    // represent the first intersection, but remember to respect
    // ray.dist_bounds! For example, if there are two intersections,
    // but only the _later_ one is within ray.dist_bounds, you should
    // return that one!
    Trace ret;
    ret.origin = ray.point;
    ret.hit = false;       // was there an intersection?
    ret.distance = 0.0f;   // at what distance did the intersection occur?
    ret.position = Vec3{}; // where was the intersection?
    ret.normal = Vec3{};   // what was the surface normal at the intersection?
    /*
    float t0, t1;
    //ѡ�ü��νⷨ,��ʼ�������£�
    Vec3 C = bbox().center();
    Vec3 O = ray.point;
    Vec3 dir = ray.dir.unit();
    Vec3 L = C - O;
    float radius2 = radius * radius;
    // ray�ľ���
    float min = ray.dist_bounds.x;
    float max = ray.dist_bounds.y;

    float tca = dot(L, dir);
    // tca<0,����������ڣ�Ӧ�ñ���
    float d2 = dot(L, L) - tca * tca;
    if(d2 > radius2) //û����ײ
    {
        ret.hit = false;
        ret.distance = 0.0f;
    } else {
        float thc = sqrt(radius2 - d2);
        t0 = tca - thc;
        t1 = tca + thc;
        ret.hit = true;
        if(min <= t1 && t1 <= max)
            ret.distance = t1; //����ѡ�����ĵ�
        else
            ret.distance = t0;
    }
    ret.origin = ray.point;
    ret.position = ray.at(ret.distance);
    //�������ĺ�������Ƿ�����
    ret.normal = (ret.position - C).unit();
    */
    
    //�����㷨��
    
    auto b = dot(ray.point - bbox().center(), ray.dir);
    auto c = (ray.point - bbox().center()).norm_squared() - radius * radius;
    auto D = b * b - c;//Լȥ�˷�ĸ��2

    if(D >= 0.0f) {
        auto t0 = (-b - std::sqrt(D));//d^2=1
        auto t1 = (-b + std::sqrt(D));
        if(ray.dist_bounds[0] <= t0 && t0 <= ray.dist_bounds[1]) {
            ret.hit = true;
            ret.distance = t0;
            ret.position = ray.at(t0);
            ret.normal = ret.position.unit();
        } else if(ray.dist_bounds[0] <= t1 && t1 <= ray.dist_bounds[1]) {
            ret.hit = true;
            ret.distance = t1;
            ret.position = ray.at(t1);
            ret.normal = ret.position.unit();
        }
    }

    return ret;
}
} // namespace PT
