
#include "../util/camera.h"
#include "../rays/samplers.h"
#include "debug.h"

Ray Camera::generate_ray(Vec2 screen_coord) const {

    // TODO (PathTracer): Task 1
    // compute the position of the input sensor sample coordinate on the
    // canonical sensor plane one unit away from the pinhole.
    // Tip: Compute the ray direction in camera space and use
    // the camera transform to transform it back into world space.
    //�������ϵ����������

    //�������ϵ�µķ�������
    //float hFov =Radians(get_h_fov());//�������
    //float vFov =Radians(vert_fov);//�������
    Vec2 point = screen_coord*2.0-1.0f;//��0,1ת����-1,1
    Vec3 pointNDS = Vec3(point.x, point.y, -1.0f);
    Vec4 pointNDSH = Vec4(pointNDS, 1.0);//����w�������������һ��Ҫ��ס
    Vec4 dirEye = get_proj().inverse() * pointNDSH;
    dirEye.w = 0.0;
    Vec3 dirWorld = (iview * dirEye).unit().xyz();

    /* float xCamera = float(-tan(hFov * 0.5) * (1 - x) + tan(hFov * 0.5) * (x));
    float yCamera = float(-tan(vFov * 0.5) * (1 - y) + tan(vFov * 0.5) * (y));
    Vec3 d = Vec3(xCamera,yCamera, -1.0);//���ձ����ȽϺ���⣬��������½ǵ�x���ұ������Ͻǵ�x
    //������������ϵ�µ�����
    //view��iview�ĸ���ת������
    Vec3 dir = ( d).unit();*/
    return Ray(position,dirWorld);
}
