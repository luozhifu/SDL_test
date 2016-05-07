#pragma once

#include <math.h>
#include <memory.h>
#include <cstdio>

// pi 定义
#define PI         ((float)3.141592654f)

//数学宏
#define DEG_TO_RAD(ang)((ang)*PI/180.0)
#define RAD_TO_DEG(rads)((rads)*180/PI)

// bit manipulation macros
#define SET_BIT(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define RESET_BIT(word,bit_flag) ((word)=((word) & (~bit_flag)))

//多边形和面的状态
#define POLY4DV1_STATE_ACTIVE			0x0001
#define POLY4DV1_STATE_CLIPPED			0x0002
#define POLY4DV1_STATE_BACKFACE			0x0004

//多边形和面的状态
#define OBJECT4DV1_STATE_ACTIVE			0x0001
#define OBJECT4DV1_STATE_VISIBLE		0x0002
#define OBJECT4DV1_STATE_CULLED			0x0004

//物体顶点
#define OBJECT4DV1_MAX_VERTICES			1024
#define OBJECT4DV1_MAX_POLYS			1024

//渲染列表定义
#define RENDERLIST4DV1_MAX_POLYS		32768   //最大多边形数量

//转换控制标志
#define TRANSFORM_LOCAL_ONLY		0		//在本地或世界顶点列表执行转换

#define TRANSFORM_LOCAL_TO_TRANS	2		//执行转换到本地顶点列表，但存储结果在转换中

//相机类型
#define CAM_MODEL_EULER            0x0008
#define CAM_MODEL_UVN              0x0010

//定义相机旋转顺序
#define CAM_ROT_SEQ_ZYX				4

#define EPSILON_E5 (float)(1E-5)


// double sided flag
#define PLX_2SIDED_FLAG              0x1000   // this poly is double sided
#define PLX_1SIDED_FLAG              0x0000   // this poly is single sided

//存储查询表
float cos_look[361];					//
float sin_look[361];					//

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

//设置单位矩阵
#define MAT_IDENTITY_4X4(m) { memcpy((void*)(m),(void*)&IMAT_4x4,sizeof(MATRIX4X4));}

//矩阵拷贝
#define MAT_COPY_4X4(src_mat, dest_mat) {memcpy((void *)(dest_mat), (void *)(src_mat), sizeof(MATRIX4X4) ); }

//多边形数据结构
typedef struct POLY4DV1_TYP
{
	int state;					//状态信息
	int attr;					//多物形的物理属性
	int color;					//多边形的颜色

	POINT4D_PTR vlist;			//指向自己的顶点列表
	int vert[3];				//顶点列表索引

}POLY4DV1, *POLY4DV1_PTR;

//自包含多边形数据结构
typedef struct POLYF4DV1_TYP
{
	int state;					//状态信息
	int attr;					//多物形的物理属性
	UINT color;					//多边形的颜色

	POINT4D vlist[3];			//三角形的顶点
	POINT4D tvlist[3];			//变换的顶点

	POLYF4DV1_TYP *next;		//指向列表下一个多边形的指针
	POLYF4DV1_TYP *prev;		//指向列表前一个多边形的指针

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

//存储渲染列表的对象，这样可以同时有多个渲染列表
typedef struct RENDERLIST4DV1_TYP
{
	int state;			//渲染列表的状态
	int attr;			//渲染列表的属性

	//渲染列表是一个指针数组
	//其中每个指针指向一个自包含的、可渲染的多边形面(POLYF4DV1);
	POLYF4DV1_PTR poly_ptrs[RENDERLIST4DV1_MAX_POLYS];

	//为避免每帧都为多边形分配和释放存储空间
	//多边形存储在下述数组中
	POLYF4DV1 poly_data[RENDERLIST4DV1_MAX_POLYS];

	int num_polys;		//渲染列表中包含的多边形数目
}RENDERLIST4DV1, *RENDERLIST4DV1_PTR;

//基于顶点列表和多边形列表的物体 
typedef struct OBJECT4DV1_TYP
{
	int id;
	char name[64];
	int state;			//渲染列表的状态
	int attr;			//渲染列表的属性

	float avg_radius;
	float max_radius;

	POINT4D world_pos;

	VECTOR4D dir;

	VECTOR4D ux, uy, uz;

	int num_vertices;	//物体顶点数

	POINT4D vlist_local[OBJECT4DV1_MAX_VERTICES];
	POINT4D vlist_trans[OBJECT4DV1_MAX_VERTICES];

	int num_polys;		//物体风格的多边形数
	POLY4DV1 plist[OBJECT4DV1_MAX_POLYS];		//多边形数组
}OBJECT4DV1,*OBJECT4DV1_PTR;

//三角函数
// trig functions
inline float Fast_Sin(float theta)
{
	// convert angle to 0-359
	theta = fmodf(theta, 360);

	// make angle positive
	if (theta < 0) theta += 360.0;

	// compute floor of theta and fractional part to interpolate
	int theta_int = (int)theta;
	float theta_frac = theta - theta_int;

	// now compute the value of sin(angle) using the lookup tables
	// and interpolating the fractional part, note that if theta_int
	// is equal to 359 then theta_int+1=360, but this is fine since the
	// table was made with the entries 0-360 inclusive
	return(sin_look[theta_int] +
		theta_frac*(sin_look[theta_int + 1] - sin_look[theta_int]));
}

inline float Fast_Cos(float theta)
{
	// convert angle to 0-359
	theta = fmodf(theta, 360);

	// make angle positive
	if (theta < 0) theta += 360.0;

	// compute floor of theta and fractional part to interpolate
	int theta_int = (int)theta;
	float theta_frac = theta - theta_int;

	// now compute the value of sin(angle) using the lookup tables
	// and interpolating the fractional part, note that if theta_int
	// is equal to 359 then theta_int+1=360, but this is fine since the
	// table was made with the entries 0-360 inclusive
	return(cos_look[theta_int] +
		theta_frac*(cos_look[theta_int + 1] - cos_look[theta_int]));
}

inline void VECTOR4D_ZERO(VECTOR4D_PTR v)
{
	v->x = v->y = v->z = 0;
	v->w = 1.0;
}

inline void VECTOR4D_INITXYZ(VECTOR4D_PTR v,float x,float y,float z)
{
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = 1.0f;
}

inline void VECTOR4D_COPY(VECTOR4D_PTR vdst,VECTOR4D_PTR vsrc)
{
	vdst->x = vsrc->x;
	vdst->y = vsrc->y;
	vdst->z = vsrc->z;
	vdst->w = vsrc->w;
}

inline void Init_CAM4DV1(
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
	MAT_IDENTITY_4X4(&cam->mcam);
	MAT_IDENTITY_4X4(&cam->mper);
	MAT_IDENTITY_4X4(&cam->mscr);

	//设置相关变量
	cam->fov = fov;

	//将视平面大小设置为2x(2/ar)
	cam->viewplane_width = 2.0;
	cam->viewplane_width = static_cast<float>(2.0/cam->aspect_ratio);
	//根据fov和视平面大小计算视距
	float tan_fov_div2 = static_cast<float>(tan((DEG_TO_RAD(fov / 2))));

	cam->view_dist = static_cast<float>((0.5)*(cam->viewplane_width)*tan_fov_div2);

	//判断fov是否为90度
}

inline void Reset_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list)
{
	//这个函数初始化和重围传递进来的渲染列表
	//为将多边形插入到其中做好准备
	//这个版本的渲染列表由一个FACE4DV1指针数组组成
	//每一帧都需要调用这个函数

	//这里使用num_polys来跟踪渲染列表中包含的多边形数目
	//因此将其设置为0
	//如果需要使渲染列表更通用，需要采用更健壮的方案
	//并将其与多边形指针列表的关联切断
	rend_list->num_polys = 0;
}

inline void Reset_OBJECT4DV1(OBJECT4DV1_PTR obj)
{
	RESET_BIT(obj->state, OBJECT4DV1_STATE_CULLED);

	// now the clipped and backface flags for the polygons 
	for (int poly = 0; poly < obj->num_polys; poly++)
	{
		// acquire polygon
		POLY4DV1_PTR curr_poly = &obj->plist[poly];

		// first is this polygon even visible?
		if (!(curr_poly->state & POLY4DV1_STATE_ACTIVE))
			continue; // move onto next poly

					  // reset clipped and backface flags
		RESET_BIT(curr_poly->state, POLY4DV1_STATE_CLIPPED);
		RESET_BIT(curr_poly->state, POLY4DV1_STATE_BACKFACE);

	} // end for poly	
}

//设置像素
inline void setPixels(int x, int y, UINT color, DWORD*	imageData, int imageWidth) {
	int index = (int)(imageWidth * y + x);

	imageData[index] = 0xff0000;
}

//绘制线条
inline void drawLine(int x1, int y1, int x2, int y2, UINT color, DWORD* imageData, int imageWidth)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int ux = ((dx > 0) << 1) - 1;//x的增量方向,取1或-1
	int uy = ((dy > 0) << 1) - 1;//y的增量方向,取1或-1

	dx = abs(dx);
	dy = abs(dy);

	int x = x1, y = y1;
	int eps = 0;

	//线段靠近x轴
	if (dx > dy)
	{
		for (x = x1; x != x2 + ux; x += ux)
		{
			setPixels(x, y, color, imageData, imageWidth);
			eps += dy;
			if ((eps << 1) >= dx)
			{
				y += uy;
				eps -= dx;
			}
		}
	}
	else
	{
		for (y = y1; y != y2 + uy; y += uy)
		{
			setPixels(x, y, color, imageData, imageWidth);
			eps += dx;
			if ((eps << 1) >= dy)
			{
				x += ux;
				eps -= dy;
			}
		}
	}
}


inline int Insert_POLYF4DV1_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,POLYF4DV1_PTR poly)
{
	//判断是否已满
	if (rend_list->num_polys >= RENDERLIST4DV1_MAX_POLYS)
		return 0;

	int index = rend_list->num_polys;

	//顶点指针
	rend_list->poly_ptrs[index] = &rend_list->poly_data[index];

	//拷贝字段
	rend_list->poly_data[index].state = poly->state;
	rend_list->poly_data[index].attr = poly->attr;
	rend_list->poly_data[index].color = poly->color;

	//拷贝顶点，
	memcpy((void*)&rend_list->poly_data[index], (void*)poly, sizeof(POLYF4DV1));

	//建立链接
	if(index == 0)
	{
		rend_list->poly_data[0].next = NULL;
		rend_list->poly_data[0].prev = NULL;
	}
	else
	{
		rend_list->poly_data[index].next = NULL;
		rend_list->poly_data[index].prev = &rend_list->poly_data[index-1];

		rend_list->poly_data[index-1].next = &rend_list->poly_data[index];
	}

	//增加列表中多边形数量
	rend_list->num_polys++;

	return 1;
}

//绘制渲染列表
inline void Draw_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list, DWORD* imageData, int imageWidth)
{
	//这个函数“执行”渲染列表
	//在线框模型下，无需对多边形进行排序，但后面需要这样做，以削除隐藏面
	//另外，这里让函数去判断位深，并调用相应的光栅化器

	//现在，我们只有一个多边形列表，可以绘制它
	//但在线框引擎中，“背面”概念无关紧要
	for (int poly=0;poly < rend_list->num_polys;poly++)
	{
		POLYF4DV1_PTR polyPtr = rend_list->poly_ptrs[poly];

		if (!(polyPtr->state & POLY4DV1_STATE_ACTIVE) ||
			(polyPtr->state & POLY4DV1_STATE_CLIPPED) ||
			(polyPtr->state & POLY4DV1_STATE_BACKFACE))
			continue;//进入下一个多边形



		//绘制三角形的边
		//2d初始化过程中已经设置好裁剪，位于2d屏幕/窗口外的多边形都将被裁剪掉
		drawLine(polyPtr->tvlist[0].x,
			polyPtr->tvlist[0].y,
			polyPtr->tvlist[1].x,
			polyPtr->tvlist[1].y,
			polyPtr->color, imageData, imageWidth);

		drawLine(polyPtr->tvlist[1].x,
			polyPtr->tvlist[1].y,
			polyPtr->tvlist[2].x,
			polyPtr->tvlist[2].y,
			polyPtr->color, imageData, imageWidth);

		drawLine(polyPtr->tvlist[2].x,
			polyPtr->tvlist[2].y,
			polyPtr->tvlist[0].x,
			polyPtr->tvlist[0].y,
			polyPtr->color, imageData, imageWidth);
	}

}

//绘制渲染列表
inline void Draw_OBJECT4DV1_Wire16(OBJECT4DV1_PTR obj, DWORD* imageData, int imageWidth)
{
	//这个函数“执行”渲染列表
	//在线框模型下，无需对多边形进行排序，但后面需要这样做，以削除隐藏面
	//另外，这里让函数去判断位深，并调用相应的光栅化器

	//现在，我们只有一个多边形列表，可以绘制它
	//但在线框引擎中，“背面”概念无关紧要
	for (int poly = 0; poly < obj->num_polys; poly++)
	{
		POLYF4DV1_PTR polyPtr = (POLYF4DV1_PTR)&obj->plist[poly];

		if (!(polyPtr->state & POLY4DV1_STATE_ACTIVE) ||
			(polyPtr->state & POLY4DV1_STATE_CLIPPED) ||
			(polyPtr->state & POLY4DV1_STATE_BACKFACE))
			continue;//进入下一个多边形

		int vindex_0 = obj->plist[poly].vert[0];
		int vindex_1 = obj->plist[poly].vert[1];
		int vindex_2 = obj->plist[poly].vert[2];

		//绘制三角形的边
		//2d初始化过程中已经设置好裁剪，位于2d屏幕/窗口外的多边形都将被裁剪掉
		drawLine(obj->vlist_trans[vindex_0].x, obj->vlist_trans[vindex_0].y,
			obj->vlist_trans[vindex_1].x, obj->vlist_trans[vindex_1].y,
			polyPtr->color, imageData, imageWidth);

		drawLine(obj->vlist_trans[vindex_1].x, obj->vlist_trans[vindex_1].y,
			obj->vlist_trans[vindex_2].x, obj->vlist_trans[vindex_2].y,
			polyPtr->color, imageData, imageWidth);

		drawLine(obj->vlist_trans[vindex_2].x, obj->vlist_trans[vindex_2].y,
			obj->vlist_trans[vindex_0].x, obj->vlist_trans[vindex_0].y,
			polyPtr->color, imageData, imageWidth);
	}

}
inline void Mat_Init_4X4(MATRIX4X4_PTR ma,
						float m00, float m01, float m02, float m03,
						float m10, float m11, float m12, float m13,
						float m20, float m21, float m22, float m23,
						float m30, float m31, float m32, float m33)
{
	// row major form
	ma->M00 = m00; ma->M01 = m01; ma->M02 = m02; ma->M03 = m03;
	ma->M10 = m10; ma->M11 = m11; ma->M12 = m12; ma->M13 = m13;
	ma->M20 = m20; ma->M21 = m21; ma->M22 = m22; ma->M23 = m23;
	ma->M30 = m30; ma->M31 = m31; ma->M32 = m32; ma->M33 = m33;
}


inline void Mat_Mul_4X4(MATRIX4X4_PTR ma,
	MATRIX4X4_PTR mb,
	MATRIX4X4_PTR mprod)
{
	// this function multiplies two 4x4 matrices together and 
	// and stores the result in mprod
	// note later we will take advantage of the fact that we know
	// that w=1 always, and that the last column of a 4x4 is
	// always 0

	for (int row = 0; row<4; row++)
	{
		for (int col = 0; col<4; col++)
		{
			// compute dot product from row of ma 
			// and column of mb

			float sum = 0; // used to hold result

			for (int index = 0; index<4; index++)
			{
				// add in next product pair
				sum += (ma->M[row][index] * mb->M[index][col]);
			} // end for index

			  // insert resulting row,col element
			mprod->M[row][col] = sum;

		} // end for col

	} // end for row

} // end Mat_Mul_4X4

//矩阵旋转
inline void Build_XYZ_Rotation_MATRIX4X4(float theta_x,float theta_y,float theta_z,MATRIX4X4_PTR mrot)
{
	MATRIX4X4 mx, my, mz, mtmp;       // working matrices
	float sin_theta = 0, cos_theta = 0;   // used to initialize matrices
	int rot_seq = 0;                  // 1 for x, 2 for y, 4 for z

									  // step 0: fill in with identity matrix
	MAT_IDENTITY_4X4(mrot);

	// step 1: based on zero and non-zero rotation angles, determine
	// rotation sequence
	if (fabs(theta_x) > EPSILON_E5) // x
		rot_seq = rot_seq | 1;

	if (fabs(theta_y) > EPSILON_E5) // y
		rot_seq = rot_seq | 2;

	if (fabs(theta_z) > EPSILON_E5) // z
		rot_seq = rot_seq | 4;

	// now case on sequence
	switch (rot_seq)
	{
	case 0: // no rotation
	{
		// what a waste!
		return;
	} break;

	case 1: // x rotation
	{
		// compute the sine and cosine of the angle
		cos_theta = Fast_Cos(theta_x);
		sin_theta = Fast_Sin(theta_x);

		// set the matrix up 
		Mat_Init_4X4(&mx, 1, 0, 0, 0,
			0, cos_theta, sin_theta, 0,
			0, -sin_theta, cos_theta, 0,
			0, 0, 0, 1);

		// that's it, copy to output matrix
		MAT_COPY_4X4(&mx, mrot);
		return;

	} break;

	case 2: // y rotation
	{
		// compute the sine and cosine of the angle
		cos_theta = Fast_Cos(theta_y);
		sin_theta = Fast_Sin(theta_y);

		// set the matrix up 
		Mat_Init_4X4(&my, cos_theta, 0, -sin_theta, 0,
			0, 1, 0, 0,
			sin_theta, 0, cos_theta, 0,
			0, 0, 0, 1);


		// that's it, copy to output matrix
		MAT_COPY_4X4(&my, mrot);
		return;

	} break;

	case 3: // xy rotation
	{
		// compute the sine and cosine of the angle for x
		cos_theta = Fast_Cos(theta_x);
		sin_theta = Fast_Sin(theta_x);

		// set the matrix up 
		Mat_Init_4X4(&mx, 1, 0, 0, 0,
			0, cos_theta, sin_theta, 0,
			0, -sin_theta, cos_theta, 0,
			0, 0, 0, 1);

		// compute the sine and cosine of the angle for y
		cos_theta = Fast_Cos(theta_y);
		sin_theta = Fast_Sin(theta_y);

		// set the matrix up 
		Mat_Init_4X4(&my, cos_theta, 0, -sin_theta, 0,
			0, 1, 0, 0,
			sin_theta, 0, cos_theta, 0,
			0, 0, 0, 1);

		// concatenate matrices 
		Mat_Mul_4X4(&mx, &my, mrot);
		return;

	} break;

	case 4: // z rotation
	{
		// compute the sine and cosine of the angle
		cos_theta = Fast_Cos(theta_z);
		sin_theta = Fast_Sin(theta_z);

		// set the matrix up 
		Mat_Init_4X4(&mz, cos_theta, sin_theta, 0, 0,
			-sin_theta, cos_theta, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);


		// that's it, copy to output matrix
		MAT_COPY_4X4(&mz, mrot);
		return;

	} break;

	case 5: // xz rotation
	{
		// compute the sine and cosine of the angle x
		cos_theta = Fast_Cos(theta_x);
		sin_theta = Fast_Sin(theta_x);

		// set the matrix up 
		Mat_Init_4X4(&mx, 1, 0, 0, 0,
			0, cos_theta, sin_theta, 0,
			0, -sin_theta, cos_theta, 0,
			0, 0, 0, 1);

		// compute the sine and cosine of the angle z
		cos_theta = Fast_Cos(theta_z);
		sin_theta = Fast_Sin(theta_z);

		// set the matrix up 
		Mat_Init_4X4(&mz, cos_theta, sin_theta, 0, 0,
			-sin_theta, cos_theta, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		// concatenate matrices 
		Mat_Mul_4X4(&mx, &mz, mrot);
		return;

	} break;

	case 6: // yz rotation
	{
		// compute the sine and cosine of the angle y
		cos_theta = Fast_Cos(theta_y);
		sin_theta = Fast_Sin(theta_y);

		// set the matrix up 
		Mat_Init_4X4(&my, cos_theta, 0, -sin_theta, 0,
			0, 1, 0, 0,
			sin_theta, 0, cos_theta, 0,
			0, 0, 0, 1);

		// compute the sine and cosine of the angle z
		cos_theta = Fast_Cos(theta_z);
		sin_theta = Fast_Sin(theta_z);

		// set the matrix up 
		Mat_Init_4X4(&mz, cos_theta, sin_theta, 0, 0,
			-sin_theta, cos_theta, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		// concatenate matrices 
		Mat_Mul_4X4(&my, &mz, mrot);
		return;

	} break;

	case 7: // xyz rotation
	{
		// compute the sine and cosine of the angle x
		cos_theta = Fast_Cos(theta_x);
		sin_theta = Fast_Sin(theta_x);

		// set the matrix up 
		Mat_Init_4X4(&mx, 1, 0, 0, 0,
			0, cos_theta, sin_theta, 0,
			0, -sin_theta, cos_theta, 0,
			0, 0, 0, 1);

		// compute the sine and cosine of the angle y
		cos_theta = Fast_Cos(theta_y);
		sin_theta = Fast_Sin(theta_y);

		// set the matrix up 
		Mat_Init_4X4(&my, cos_theta, 0, -sin_theta, 0,
			0, 1, 0, 0,
			sin_theta, 0, cos_theta, 0,
			0, 0, 0, 1);

		// compute the sine and cosine of the angle z
		cos_theta = Fast_Cos(theta_z);
		sin_theta = Fast_Sin(theta_z);

		// set the matrix up 
		Mat_Init_4X4(&mz, cos_theta, sin_theta, 0, 0,
			-sin_theta, cos_theta, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		// concatenate matrices, watch order!
		Mat_Mul_4X4(&mx, &my, &mtmp);
		Mat_Mul_4X4(&mtmp, &mz, mrot);

	} break;

	default: break;

	} // end switch
}

inline void Build_Sin_Cos_Tables()
{
	// generate the tables 0 - 360 inclusive
	for (int ang = 0; ang <= 360; ang++)
	{
		// convert ang to radians
		float theta = (float)ang*PI / (float)180;

		// insert next entry into table
		cos_look[ang] = static_cast<float>(cos(theta));
		sin_look[ang] = static_cast<float>(sin(theta));

	} // end for ang
}

void Mat_Mul_VECTOR4D_4X4(VECTOR4D_PTR  va,
	MATRIX4X4_PTR mb,
	VECTOR4D_PTR  vprod)
{
	// this function multiplies a VECTOR4D against a 
	// 4x4 matrix - ma*mb and stores the result in mprod
	// the function makes no assumptions

	for (int col = 0; col < 4; col++)
	{
		// compute dot product from row of ma 
		// and column of mb
		float sum = 0; // used to hold result

		for (int row = 0; row<4; row++)
		{
			// add in next product pair
			sum += (va->M[row] * mb->M[row][col]);
		} // end for index

		  // insert resulting col element
		vprod->M[col] = sum;

	} // end for col

} // end Mat_Mul_VECTOR4D_4X4

inline void Transform_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,	//转换的渲染列表
									MATRIX4X4_PTR mt,				//转换矩阵
									int coord_select)				//转换的坐标
{
	// this function simply transforms all of the polygons vertices in the local or trans
	// array of the render list by the sent matrix

	// what coordinates should be transformed?
	switch (coord_select)
	{
	case TRANSFORM_LOCAL_ONLY:
	{
		for (int poly = 0; poly < rend_list->num_polys; poly++)
		{
			// acquire current polygon
			POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];

			// is this polygon valid?
			// transform this polygon if and only if it's not clipped, not culled,
			// active, and visible, note however the concept of "backface" is 
			// irrelevant in a wire frame engine though
			if ((curr_poly == NULL) || !(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
				(curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
				(curr_poly->state & POLY4DV1_STATE_BACKFACE))
				continue; // move onto next poly

						  // all good, let's transform 
			for (int vertex = 0; vertex < 3; vertex++)
			{
				// transform the vertex by mt
				POINT4D presult; // hold result of each transformation

								 // transform point
				Mat_Mul_VECTOR4D_4X4(&curr_poly->vlist[vertex], mt, &presult);

				// store result back
				VECTOR4D_COPY(&curr_poly->vlist[vertex], &presult);
			} // end for vertex

		} // end for poly

	} break;

	case TRANSFORM_LOCAL_TO_TRANS:
	{
		// transform each local/model vertex of the render list and store result
		// in "transformed" vertex list
		for (int poly = 0; poly < rend_list->num_polys; poly++)
		{
			// acquire current polygon
			POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];

			// is this polygon valid?
			// transform this polygon if and only if it's not clipped, not culled,
			// active, and visible, note however the concept of "backface" is 
			// irrelevant in a wire frame engine though
			if ((curr_poly == NULL) || !(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
				(curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
				(curr_poly->state & POLY4DV1_STATE_BACKFACE))
				continue; // move onto next poly

						  // all good, let's transform 
			for (int vertex = 0; vertex < 3; vertex++)
			{
				// transform the vertex by mt
				Mat_Mul_VECTOR4D_4X4(&curr_poly->vlist[vertex], mt, &curr_poly->tvlist[vertex]);
			} // end for vertex

		} // end for poly

	} break;

	default: break;

	} // end switch
}

inline void Transform_OBJECT4DV1(OBJECT4DV1_PTR obj, // object to transform
	MATRIX4X4_PTR mt,   // transformation matrix
	int coord_select,   // selects coords to transform
	int transform_basis) // flags if vector orientation
						 // should be transformed too
{
	// this function simply transforms all of the vertices in the local or trans
	// array by the sent matrix

	// what coordinates should be transformed?
	switch (coord_select)
	{
		case TRANSFORM_LOCAL_ONLY:
		{
			// transform each local/model vertex of the object mesh in place
			for (int vertex = 0; vertex < obj->num_vertices; vertex++)
			{
				POINT4D presult; // hold result of each transformation

								 // transform point
				Mat_Mul_VECTOR4D_4X4(&obj->vlist_local[vertex], mt, &presult);

				// store result back
				VECTOR4D_COPY(&obj->vlist_local[vertex], &presult);
			} // end for index
		} break;
	}
}

inline void VECTOR4D_Add(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vsum)
{
	// this function adds va+vb and return it in vsum
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
	vsum->z = va->z + vb->z;
	vsum->w = 1;

} // end VECTOR4D_Add

inline void Model_To_World_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,POINT4D_PTR world_pos,int coord_select=TRANSFORM_LOCAL_TO_TRANS)
{
	for (int poly=0;poly<rend_list->num_polys;poly++)
	{
		//获取当前多边形
		POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];

		if (curr_poly == NULL || !(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
			(curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
			(curr_poly->state & POLY4DV1_STATE_BACKFACE))
			continue;

		for (int vertex = 0;vertex<3;vertex++)
		{
			VECTOR4D_Add(&curr_poly->vlist[vertex], world_pos, &curr_poly->tvlist[vertex]);
		}
	}
}

inline void Model_To_World_OBJECT4DV1(OBJECT4DV1_PTR obj, int coord_select = TRANSFORM_LOCAL_TO_TRANS)
{
	if (coord_select == TRANSFORM_LOCAL_TO_TRANS)
	{
		for (int vertex = 0; vertex < obj->num_vertices; vertex++)
		{
			// translate vertex
			VECTOR4D_Add(&obj->vlist_local[vertex], &obj->world_pos, &obj->vlist_trans[vertex]);
		} // end for vertex
	} // end if local
	else
	{ // TRANSFORM_TRANS_ONLY
		for (int vertex = 0; vertex < obj->num_vertices; vertex++)
		{
			// translate vertex
			VECTOR4D_Add(&obj->vlist_trans[vertex], &obj->world_pos, &obj->vlist_trans[vertex]);
		} // end for vertex
	} // end else trans
}

inline void Build_CAM4DV1_Matrix_Euler(CAM4DV1_PTR cam,int cam_rot_seq)
{
	//这个函数根据欧拉角度计算相机变换矩阵，并将其存储到传入的相机对象中
	//第6章介绍过，要创建相机变换矩阵，需要这样做：Mcam = mt(-1) * my(-1) * mx(-1) * mz(-1)
	//即相机平衡矩阵乘以相机绕y,x,z轴的旋转矩阵的逆矩阵
	//采用什么样的旋转顺序完全取决于用户，因此这里没有强制采用某种顺序,而是根据参数cam_rot_seq的值来决定采用哪种顺序
	//该参数的取值为CAM_ROT_SEQ_XYZ,其中XYZ可以按任何顺序排列，即YXZ,ZXY等

	MATRIX4X4 mt_inv,	//相机平移矩阵的逆矩阵
		mx_inv,			//相机绕x轴的旋转矩阵的逆矩阵
		my_inv,			//相机绕y轴的旋转矩阵的逆矩阵
		mz_inv,			//相机绕z轴的旋转矩阵的逆矩阵
		mrot,			//所有逆旋转矩阵的积
		mtmp;			//用于存储临时矩阵

	//第一步：根据相机位置计算相机平移矩阵的逆矩阵
	Mat_Init_4X4(&mt_inv, 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-cam->pos.x, -cam->pos.y, -cam->pos.z, 1);

	//第二步：创建旋转矩阵的逆矩阵
	//要创建旋转矩阵的逆矩阵，可以将其转置，也可以将每个旋转角度取负

	//提取欧拉角度
	float theta_x = cam->dir.x;
	float theta_y = cam->dir.y;
	float theta_z = cam->dir.z;

	float cos_theta = Fast_Cos(theta_x);
	float sin_theta = -Fast_Sin(theta_x);

	Mat_Init_4X4(&mx_inv,	1, 0,			0,			0,
							0, cos_theta,	sin_theta,	0,
							0, -sin_theta,	cos_theta,	0,
							0, 0,			0,			1);

	cos_theta = Fast_Cos(theta_y);
	sin_theta = -Fast_Sin(theta_y);

	Mat_Init_4X4(&my_inv,	cos_theta,	0, -sin_theta,	0,
							0,			1, 0,			0,
							sin_theta,	0, cos_theta,	0,
							0,			0, 0,			1);

	cos_theta = Fast_Cos(theta_z);
	sin_theta = -Fast_Sin(theta_z);

	Mat_Init_4X4(&mz_inv,	cos_theta,	sin_theta,	0, 0,
							-sin_theta, cos_theta,	0, 0,
							0,			0,			1, 0,
							0,			0,			0, 1);

	switch (cam_rot_seq)
	{
	case CAM_ROT_SEQ_ZYX:
		Mat_Mul_4X4(&mz_inv, &mx_inv, &mtmp);
		Mat_Mul_4X4(&mtmp, &my_inv, &mrot);
		break;
	}

	Mat_Mul_4X4(&mt_inv, &mrot, &cam->mcam);
}

inline void World_To_Camera_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,CAM4DV1_PTR cam)
{
	for (int poly = 0; poly < rend_list->num_polys; ++poly)
	{
		POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];

		if ((curr_poly == NULL) || !(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
			(curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
			(curr_poly->state & POLY4DV1_STATE_BACKFACE))
			continue;

		for (int vertex = 0; vertex < 3; ++vertex)
		{
			POINT4D presult;

			Mat_Mul_VECTOR4D_4X4(&curr_poly->tvlist[vertex], &cam->mcam, &presult);

			VECTOR4D_COPY(&curr_poly->tvlist[vertex], &presult);
		}
	}
}

inline void World_To_Camera_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam)
{
	for (int vertex = 0; vertex < obj->num_vertices; vertex++)
	{
		// transform the vertex by the mcam matrix within the camera
		// it better be valid!
		POINT4D presult; // hold result of each transformation

						 // transform point
		Mat_Mul_VECTOR4D_4X4(&obj->vlist_trans[vertex], &cam->mcam, &presult);

		// store result back
		VECTOR4D_COPY(&obj->vlist_trans[vertex], &presult);
	} // end for vertex
}

inline void Camera_To_Perspective_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list, CAM4DV1_PTR cam)
{
	for (int poly = 0; poly < rend_list->num_polys; ++poly)
	{
		POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[poly];

		if ((curr_poly == NULL) || !(curr_poly->state & POLY4DV1_STATE_ACTIVE) ||
			(curr_poly->state & POLY4DV1_STATE_CLIPPED) ||
			(curr_poly->state & POLY4DV1_STATE_BACKFACE))
			continue;

		for (int vertex = 0; vertex < 3; ++vertex)
		{
			float z = curr_poly->tvlist[vertex].z;

			POINT4D_PTR p = &curr_poly->tvlist[vertex];

			p->x = cam->view_dist * p->x / z;
			p->y = cam->view_dist * p->y * cam->aspect_ratio / z;
		}
	}
}

inline void Camera_To_Perspective_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam)
{
	for (int vertex = 0; vertex < obj->num_vertices; vertex++)
	{
		float z = obj->vlist_trans[vertex].z;

		// transform the vertex by the view parameters in the camera
		obj->vlist_trans[vertex].x = cam->view_dist*obj->vlist_trans[vertex].x / z;
		obj->vlist_trans[vertex].y = cam->view_dist*obj->vlist_trans[vertex].y*cam->aspect_ratio / z;
		// z = z, so no change

		// not that we are NOT dividing by the homogenous w coordinate since
		// we are not using a matrix operation for this version of the function 

	} // end for vertex
}

inline void Perspective_To_Screen_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam)
{
	float alpha = (0.5*cam->viewport_width - 0.5);
	float beta = (0.5*cam->viewport_height - 0.5);

	for (int vertex = 0; vertex < obj->num_vertices; vertex++)
	{
		// assumes the vertex is in perspective normalized coords from -1 to 1
		// on each axis, simple scale them to viewport and invert y axis and project
		// to screen

		// transform the vertex by the view parameters in the camera
		obj->vlist_trans[vertex].x = alpha + alpha*obj->vlist_trans[vertex].x;
		obj->vlist_trans[vertex].y = beta - beta *obj->vlist_trans[vertex].y;

	} // end for vertex
}

inline char *Get_Line_PLG(char *buffer,int maxlength, FILE *fp)
{
	int index = 0;
	int length = 0;

	while(1)
	{
		if (!fgets(buffer, maxlength, fp))
			return NULL;

		//计算空格数
		for (length = strlen(buffer), index = 0; isspace(buffer[index]); index++);

		if (index >= length || buffer[index] == '#')
			continue;
				
			
		//此时得到了一个数据行
		return &buffer[index];
	}
}

inline int Load_OBJECT4DV1_PLG(OBJECT4DV1* obj, const char* filename, VECTOR4D* vscale, VECTOR4D* vpos, VECTOR4D* rot)
{
	
	FILE *fp;
	char buffer[256];

	char *token_string;

	memset(obj, 0, sizeof(OBJECT4DV1));

	obj->state = OBJECT4DV1_STATE_ACTIVE | OBJECT4DV1_STATE_VISIBLE;

	obj->world_pos.x = vpos->x;
	obj->world_pos.y = vpos->y;
	obj->world_pos.z = vpos->z;
	obj->world_pos.w = vpos->w;

	if(!(fp = fopen(filename,"r")))
	{
		printf("Couldn't open PLG file %s", filename);
		return 0;
	}

	//读取物体描述符
	if(!(token_string = Get_Line_PLG(buffer,255,fp)))
	{
		printf("PLG file error with file %s(object descriptor invalid).", filename);
		return 0;
	}

	//对物体描述符进行分析
	sscanf(token_string, "%s %d %d", obj->name, &obj->num_vertices, &obj->num_polys);

	//加载顶点列表
	for (int vertex = 0; vertex < obj->num_vertices; ++vertex)
	{
		//读取物体描述符
		if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
		{
			printf("PLG file error with file %s(vertex list invalid).", filename);
			return 0;
		}

		//分析顶点
		sscanf(token_string, "%f %f %f", &obj->vlist_local[vertex].x, &obj->vlist_local[vertex].y, &obj->vlist_local[vertex].z);
		obj->vlist_local[vertex].w = 1;

		obj->vlist_local[vertex].x *= vscale->x;
		obj->vlist_local[vertex].y *= vscale->y;
		obj->vlist_local[vertex].z *= vscale->z;
	}

	int poly_surface_desc = 0;	//PLG/PLX多边形描述符
	int poly_num_verts = 0;		//当前多边形的顶点数
	char temp_string[8];

	//第五步：加载多边形列表
	for (int poly = 0; poly < obj->num_polys; ++poly)
	{
		//读取多边形描述符
		if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
		{
			printf("PLG file error with file %s(polygon descriptor invalid).", filename);
			return 0;
		}

		//每个多边形有三个顶点

		sscanf(token_string, "%s %d %d %d %d", temp_string,&poly_num_verts,&obj->plist[poly].vert[0], &obj->plist[poly].vert[1], &obj->plist[poly].vert[2]);


		obj->plist[poly].state = POLY4DV1_STATE_ACTIVE;
	}

	fclose(fp);

	return 1;
}
