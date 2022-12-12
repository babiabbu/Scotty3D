
#include "../util/camera.h"
#include "../rays/samplers.h"
#include "debug.h"

Ray Camera::generate_ray(Vec2 screen_coord) const {

    // TODO (PathTracer): Task 1
    // compute the position of the input sensor sample coordinate on the
    // canonical sensor plane one unit away from the pinhole.
    // Tip: Compute the ray direction in camera space and use
    // the camera transform to transform it back into world space.
    //相机坐标系下射线生成

    //相机坐标系下的方向向量
    //float hFov =Radians(get_h_fov());//横向比例
    //float vFov =Radians(vert_fov);//纵向比例
    Vec2 point = screen_coord*2.0-1.0f;//从0,1转换到-1,1
    Vec3 pointNDS = Vec3(point.x, point.y, -1.0f);
    Vec4 pointNDSH = Vec4(pointNDS, 1.0);//丢了w这个修正变量，一定要记住
    Vec4 dirEye = get_proj().inverse() * pointNDSH;
    dirEye.w = 0.0;
    Vec3 dirWorld = (iview * dirEye).unit().xyz();

    /* float xCamera = float(-tan(hFov * 0.5) * (1 - x) + tan(hFov * 0.5) * (x));
    float yCamera = float(-tan(vFov * 0.5) * (1 - y) + tan(vFov * 0.5) * (y));
    Vec3 d = Vec3(xCamera,yCamera, -1.0);//按照比例比较好理解，左边是左下角的x，右边是右上角的x
    //发射世界坐标系下的射线
    //view和iview哪个是转换矩阵
    Vec3 dir = ( d).unit();*/
    return Ray(position,dirWorld);
}
