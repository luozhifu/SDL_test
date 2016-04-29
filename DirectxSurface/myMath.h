#pragma once

#include <math.h>
#include <memory.h>

// pi 定义
#define PI         ((float)3.141592654f)

//数学宏
#define DEG_TO_RAD(ang)((ang)*PI/180.0)
#define RAD_TO_DEG(rads)((rads)*180/PI)

//多边形和面的状态
#define POLY4DV1_STATE_ACTIVE			0x0001
#define POLY4DV1_STATE_CLIPPED			0x0002
#define POLY4DV1_STATE_BACKFACE			0x0004

//相机类型
#define CAM_MODEL_EULER            0x0008
#define CAM_MODEL_UVN              0x0010

//不含w分量的3D向量和点///////////
typedef struct VECTOR3D_TYP
{
	union
	{
		float M[3];//数组存储方式

		struct
		{
			float x, y, z;
		};
	};
}VECTOR3D, POINT3D, *VECTOR3D_PTR, *POINT3D_PTR;

//包含w分量和4D齐次向量和点
typedef struct VECTOR4D_TYP
{
	union
	{
		float M[4];//数组存储方式

		struct
		{
			float x, y, z, w;
		};
	};
}VECTOR4D, POINT4D, *VECTOR4D_PTR, *POINT4D_PTR;

//3d平面
typedef struct PLANE3D_TYP
{
	POINT3D p0;	//平面上的点
	VECTOR3D n;	//平面的法线（不必是是单位向量）
}PLANE3D,*PLANE3D_PTR;

//4x4矩阵
typedef struct MATRIX4X4_TYP
{
	union
	{
		float M[4][4];//数组存储方式
		
		//按先行后列的顺序以独立变量的方式存储
		struct
		{
			float M00, M01, M02, M03;
			float M10, M11, M12, M13;
			float M20, M21, M22, M23;
			float M30, M31, M32, M33;
		};
	};
}MATRIX4X4,*MATRIX4X4_PTR;

//单位矩阵

//4x4 单位矩阵
const MATRIX4X4 IMAT_4x4 = {
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1 };

#define MAT_IDENTITY_4x4(m) { memcpy((void*)(m),(void*)&IMAT_4x4,sizeof(MATRIX4X4));}

//多边形数据结构
typedef struct POLYF4DV1_TYP
{
	int state;		//状态信息
	int attr;		//多物形的物理属性
	unsigned int color;		//多边形的颜色

	POINT4D vlist[3];	//三角形的顶点
	POINT4D tvlist[3];	//变换的顶点

	POLYF4DV1_TYP *next;//指向列表下一个多边形的指针
	POLYF4DV1_TYP *prev;//指向列表前一个多边形的指针

}POLYF4DV1,*POLYF4DV1_PTR;

//相机数据结构
typedef struct CAM4DV1_TYP
{
	int state;			//相机状态
	int attr;			//相机属性

	POINT4D pos;		//相机在世界坐标系中的位置

	VECTOR4D dir;		//欧拉角度或UVN相机模型的注视方向

	VECTOR4D u;			//UVN相机模型的朝向向量
	VECTOR4D v;		
	VECTOR4D n;		

	POINT4D target;		//UVN模型的目录位置

	float view_dist;	//水平视距和垂直视距

	float fov;			//水平方向和垂直方向的视野

	//3d裁剪面
	//如果视野不是90度，3d裁剪面方程将为一般性平面方程
	float near_clip_z;	//近裁剪面
	float far_clip_z;	//远裁剪面

	PLANE3D rt_clip_plane;	//右裁剪面
	PLANE3D lt_clip_plane;	//左裁剪面
	PLANE3D tp_clip_plane;	//上裁剪面
	PLANE3D bt_clip_plane;	//下裁剪面

	float viewplane_width;	//视平面的宽度和高度
	float viewplane_height;	//对于归一化投影，为2X2 否则大小与视口或屏蔽窗口相同

	//屏幕和视口是同义词
	float viewport_width;	//屏幕/视口的大小
	float viewport_height;
	float viewport_center_x;//视口中心	
	float viewport_center_y;

	//宽高比
	float aspect_ratio;	//屏幕的宽高比

	//是否需要下述矩阵取决于变换方法
	//例如，以手工方式进行透视变换、屏幕变换时、不需要这些矩阵
	//然而提供这些矩阵提高了灵活性

	MATRIX4X4 mcam;		//用于存储世界坐标到相机坐标变换矩阵
	MATRIX4X4 mper;		//用于存储相机坐标到透视坐标变换矩阵
	MATRIX4X4 mscr;		//用于存储透视坐标到屏蔽坐标变换矩阵

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
	CAM4DV1_PTR cam,			//相机对象
	int cam_attr,				//相机属性
	POINT4D_PTR	cam_pos,		//相机的初始位置
	VECTOR4D_PTR cam_dir,			//相机的初始角度
	POINT4D_PTR cam_target,		//UVN相机的初始目标位置
	float near_clip_z,			//近裁剪面和远剪裁面
	float far_clip_z,		
	float fov,					//视野，单位为度
	float viewport_width,		//屏幕/视口大小
	float viewport_height)
{
	//这个函数初始化相机对象cam
	cam->attr = cam_attr;		//相机属性

	VECTOR4D_COPY(&cam->pos, cam_pos);  //位置
	VECTOR4D_COPY(&cam->dir, cam_dir);	//欧拉相机和方向向量或角度

	//对于 UVN相机
	VECTOR4D_INITXYZ(&cam->u, 1, 0, 0);//设置为+x轴方向
	VECTOR4D_INITXYZ(&cam->v, 0, 1, 0);//设置为+y轴方向
	VECTOR4D_INITXYZ(&cam->n, 0, 0, 1);//设置为+z轴方向

	if (cam_target)
		VECTOR4D_COPY(&cam->target, cam_target); //UVN目标位置
	else
		VECTOR4D_ZERO(&cam->target);

	cam->near_clip_z = near_clip_z;				//近裁剪面
	cam->far_clip_z = far_clip_z;				//远裁剪面

	cam->viewport_width = viewport_width;		//视口大小
	cam->viewport_height = viewport_height;

	cam->viewport_center_x = (viewport_width - 1) / 2;	//视口的中心
	cam->viewport_center_y = (viewport_height - 1) / 2;	//视口的中心

	cam->aspect_ratio = (float)viewport_width / (float)viewport_height;

	//将所有的矩阵都设置为单位矩阵
	MAT_IDENTITY_4x4(&cam->mcam);
	MAT_IDENTITY_4x4(&cam->mper);
	MAT_IDENTITY_4x4(&cam->mscr);

	//设置相关变量
	cam->fov = fov;

	//将视平面大小设置为2x(2/ar)
	cam->viewplane_width = 2.0;
	cam->viewplane_width = 2.0/cam->aspect_ratio;
	//根据fov和视平面大小计算视距
	float tan_fov_div2 = tan((DEG_TO_RAD(fov / 2)));

	cam->view_dist = (0.5)*(cam->viewplane_width)*tan_fov_div2;

	//判断fov是否为90度
}
