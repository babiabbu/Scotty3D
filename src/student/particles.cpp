
#include "../scene/particles.h"
#include "../rays/pathtracer.h"

bool Scene_Particles::Particle::update(const PT::Object& scene, float dt, float radius) {

    auto ProjectLength = [](Vec3 normal, Vec3 dir) { return dot(normal, -dir); };

    auto AdjustNormal = [](Vec3& normal, Vec3 dir) {
        if(dot(normal, -dir) < 0.f) normal = -1 * normal;
    };

    float tLeft = dt; // time left for collision looping
    float eps = 1e-3; // minimum time to continue loop

    while(tLeft > 0) {
        Ray ray(pos, velocity.unit());

        auto intersection = scene.hit(ray);
        //AdjustNormal(intersection.normal, ray.dir);

        if(intersection.hit) {
            float v = velocity.norm();
            if(eps * v + radius > intersection.distance) {//不在这里更新eps也行，因为发生碰撞后eps后不会发生碰撞了
                velocity = velocity +
                           2 * ProjectLength(intersection.normal, velocity) * intersection.normal;
            } else {
                pos = pos + velocity * eps;
                velocity = velocity + acceleration * eps;
            }
        } else {
            pos = pos + velocity * tLeft;
            velocity = velocity + acceleration * tLeft;
            break;
        }
        // TODO: ray from particle origin; velocity is always unit
        // TODO: how far the particle will travel
        // TODO: hit something?
        // TODO: if hit something?
        // TODO: calculate new pos and velocity, and new simulation time.
        // TODO: if not hit
        // use Forward Euler to calculate new pos and velocity and break loop
        tLeft -= eps;
    }

    age -= dt;

    return age > 0.f; // dead particle?
}
/*
bool Scene_Particles::Particle::update(const PT::Object& scene, float dt, float radius) {

    float tLeft = dt; // time left for collision looping
    float eps = 1e-3; // minimum time to continue loop
    Vec3 posmul = pos+velocity*dt;//将要到这个位置
    while((tLeft - eps) > 0) {
        // TODO: ray from particle origin; velocity is always unit
        Ray ray = Ray(pos, velocity.unit());
        // TODO: how far the particle will travel                        
        if(scene.hit(ray).hit)
        {
            float timeNeed = (scene.hit(ray).distance-radius) / velocity.norm_squared(); //碰撞发生剩余的时间
            if(scene.hit(ray).distance<=radius||timeNeed<tLeft) {
                velocity =
                    (ray.dir - 2.0f * dot(scene.hit(ray).normal, ray.dir) * scene.hit(ray).normal)
                        .normalize() *
                    velocity.norm(); //加上一点衰弱系数
                pos = ray.at(scene.hit(ray).distance-radius);
                tLeft -= timeNeed;
                posmul = pos + tLeft * velocity;
                //  TODO: calculate new pos and velocity, and new simulation time.
                //不断检验
            } 
            else {
                pos = posmul;
                velocity += acceleration * tLeft;
                break;
            }
        }
        // TODO: if not hit
        // use Forward Euler to calculate new pos and velocity and break loop
        else {
            pos =posmul;
            velocity += acceleration * tLeft;
            break;
        }
    }
    age -= dt;
    return age > 0; // dead particle?
    return false;
}*/
