#pragma once

#include <math.h>
#include <memory.h>

// pi ����
#define PI         ((float)3.141592654f)

//��ѧ��
#define DEG_TO_RAD(ang)((ang)*PI/180.0)
#define RAD_TO_DEG(rads)((rads)*180/PI)

//����κ����״̬
#define POLY4DV1_STATE_ACTIVE			0x0001
#define POLY4DV1_STATE_CLIPPED			0x0002
#define POLY4DV1_STATE_BACKFACE			0x0004

//�������
#define CAM_MODEL_EULER            0x0008
#define CAM_MODEL_UVN              0x0010

//����w������3D�����͵�///////////
typedef struct VECTOR3D_TYP
{
	union
	{
		float M[3];//����洢��ʽ

		struct
		{
			float x, y, z;
		};
	};
}VECTOR3D, POINT3D, *VECTOR3D_PTR, *POINT3D_PTR;

//����w������4D��������͵�
typedef struct VECTOR4D_TYP
{
	union
	{
		float M[4];//����洢��ʽ

		struct
		{
			float x, y, z, w;
		};
	};
}VECTOR4D, POINT4D, *VECTOR4D_PTR, *POINT4D_PTR;

//3dƽ��
typedef struct PLANE3D_TYP
{
	POINT3D p0;	//ƽ���ϵĵ�
	VECTOR3D n;	//ƽ��ķ��ߣ��������ǵ�λ������
}PLANE3D,*PLANE3D_PTR;

//4x4����
typedef struct MATRIX4X4_TYP
{
	union
	{
		float M[4][4];//����洢��ʽ
		
		//�����к��е�˳���Զ��������ķ�ʽ�洢
		struct
		{
			float M00, M01, M02, M03;
			float M10, M11, M12, M13;
			float M20, M21, M22, M23;
			float M30, M31, M32, M33;
		};
	};
}MATRIX4X4,*MATRIX4X4_PTR;

//��λ����

//4x4 ��λ����
const MATRIX4X4 IMAT_4x4 = {
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1 };

#define MAT_IDENTITY_4x4(m) { memcpy((void*)(m),(void*)&IMAT_4x4,sizeof(MATRIX4X4));}

//��������ݽṹ
typedef struct POLYF4DV1_TYP
{
	int state;		//״̬��Ϣ
	int attr;		//�����ε���������
	unsigned int color;		//����ε���ɫ

	POINT4D vlist[3];	//�����εĶ���
	POINT4D tvlist[3];	//�任�Ķ���

	POLYF4DV1_TYP *next;//ָ���б���һ������ε�ָ��
	POLYF4DV1_TYP *prev;//ָ���б�ǰһ������ε�ָ��

}POLYF4DV1,*POLYF4DV1_PTR;

//������ݽṹ
typedef struct CAM4DV1_TYP
{
	int state;			//���״̬
	int attr;			//�������

	POINT4D pos;		//�������������ϵ�е�λ��

	VECTOR4D dir;		//ŷ���ǶȻ�UVN���ģ�͵�ע�ӷ���

	VECTOR4D u;			//UVN���ģ�͵ĳ�������
	VECTOR4D v;		
	VECTOR4D n;		

	POINT4D target;		//UVNģ�͵�Ŀ¼λ��

	float view_dist;	//ˮƽ�Ӿ�ʹ�ֱ�Ӿ�

	float fov;			//ˮƽ����ʹ�ֱ�������Ұ

	//3d�ü���
	//�����Ұ����90�ȣ�3d�ü��淽�̽�Ϊһ����ƽ�淽��
	float near_clip_z;	//���ü���
	float far_clip_z;	//Զ�ü���

	PLANE3D rt_clip_plane;	//�Ҳü���
	PLANE3D lt_clip_plane;	//��ü���
	PLANE3D tp_clip_plane;	//�ϲü���
	PLANE3D bt_clip_plane;	//�²ü���

	float viewplane_width;	//��ƽ��Ŀ�Ⱥ͸߶�
	float viewplane_height;	//���ڹ�һ��ͶӰ��Ϊ2X2 �����С���ӿڻ����δ�����ͬ

	//��Ļ���ӿ���ͬ���
	float viewport_width;	//��Ļ/�ӿڵĴ�С
	float viewport_height;
	float viewport_center_x;//�ӿ�����	
	float viewport_center_y;

	//��߱�
	float aspect_ratio;	//��Ļ�Ŀ�߱�

	//�Ƿ���Ҫ��������ȡ���ڱ任����
	//���磬���ֹ���ʽ����͸�ӱ任����Ļ�任ʱ������Ҫ��Щ����
	//Ȼ���ṩ��Щ��������������

	MATRIX4X4 mcam;		//���ڴ洢�������굽�������任����
	MATRIX4X4 mper;		//���ڴ洢������굽͸������任����
	MATRIX4X4 mscr;		//���ڴ洢͸�����굽��������任����

}CAM4DV1,*CAM4DV1_PTR;

void VECTOR4D_ZERO(VECTOR4D_PTR v)
{
	v->x = v->y = v->z = 0;
	v->w = 1.0;
}

void VECTOR4D_INITXYZ(VECTOR4D_PTR v,float x,float y,float z)
{
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = 1.0f;
}

void VECTOR4D_COPY(VECTOR4D_PTR vdst,VECTOR4D_PTR vsrc)
{
	vdst->x = vsrc->x;
	vdst->y = vsrc->y;
	vdst->z = vsrc->z;
	vdst->w = vsrc->w;
}

void Init_CAM4DV1(
	CAM4DV1_PTR cam,			//�������
	int cam_attr,				//�������
	POINT4D_PTR	cam_pos,		//����ĳ�ʼλ��
	VECTOR4D_PTR cam_dir,			//����ĳ�ʼ�Ƕ�
	POINT4D_PTR cam_target,		//UVN����ĳ�ʼĿ��λ��
	float near_clip_z,			//���ü����Զ������
	float far_clip_z,		
	float fov,					//��Ұ����λΪ��
	float viewport_width,		//��Ļ/�ӿڴ�С
	float viewport_height)
{
	//���������ʼ���������cam
	cam->attr = cam_attr;		//�������

	VECTOR4D_COPY(&cam->pos, cam_pos);  //λ��
	VECTOR4D_COPY(&cam->dir, cam_dir);	//ŷ������ͷ���������Ƕ�

	//���� UVN���
	VECTOR4D_INITXYZ(&cam->u, 1, 0, 0);//����Ϊ+x�᷽��
	VECTOR4D_INITXYZ(&cam->v, 0, 1, 0);//����Ϊ+y�᷽��
	VECTOR4D_INITXYZ(&cam->n, 0, 0, 1);//����Ϊ+z�᷽��

	if (cam_target)
		VECTOR4D_COPY(&cam->target, cam_target); //UVNĿ��λ��
	else
		VECTOR4D_ZERO(&cam->target);

	cam->near_clip_z = near_clip_z;				//���ü���
	cam->far_clip_z = far_clip_z;				//Զ�ü���

	cam->viewport_width = viewport_width;		//�ӿڴ�С
	cam->viewport_height = viewport_height;

	cam->viewport_center_x = (viewport_width - 1) / 2;	//�ӿڵ�����
	cam->viewport_center_y = (viewport_height - 1) / 2;	//�ӿڵ�����

	cam->aspect_ratio = (float)viewport_width / (float)viewport_height;

	//�����еľ�������Ϊ��λ����
	MAT_IDENTITY_4x4(&cam->mcam);
	MAT_IDENTITY_4x4(&cam->mper);
	MAT_IDENTITY_4x4(&cam->mscr);

	//������ر���
	cam->fov = fov;

	//����ƽ���С����Ϊ2x(2/ar)
	cam->viewplane_width = 2.0;
	cam->viewplane_width = 2.0/cam->aspect_ratio;
	//����fov����ƽ���С�����Ӿ�
	float tan_fov_div2 = tan((DEG_TO_RAD(fov / 2)));

	cam->view_dist = (0.5)*(cam->viewplane_width)*tan_fov_div2;

	//�ж�fov�Ƿ�Ϊ90��
}
