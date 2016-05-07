#pragma once

#include <math.h>
#include <memory.h>
#include <cstdio>

// pi ����
#define PI         ((float)3.141592654f)

//��ѧ��
#define DEG_TO_RAD(ang)((ang)*PI/180.0)
#define RAD_TO_DEG(rads)((rads)*180/PI)

// bit manipulation macros
#define SET_BIT(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define RESET_BIT(word,bit_flag) ((word)=((word) & (~bit_flag)))

//����κ����״̬
#define POLY4DV1_STATE_ACTIVE			0x0001
#define POLY4DV1_STATE_CLIPPED			0x0002
#define POLY4DV1_STATE_BACKFACE			0x0004

//����κ����״̬
#define OBJECT4DV1_STATE_ACTIVE			0x0001
#define OBJECT4DV1_STATE_VISIBLE		0x0002
#define OBJECT4DV1_STATE_CULLED			0x0004

//���嶥��
#define OBJECT4DV1_MAX_VERTICES			1024
#define OBJECT4DV1_MAX_POLYS			1024

//��Ⱦ�б���
#define RENDERLIST4DV1_MAX_POLYS		32768   //�����������

//ת�����Ʊ�־
#define TRANSFORM_LOCAL_ONLY		0		//�ڱ��ػ����綥���б�ִ��ת��

#define TRANSFORM_LOCAL_TO_TRANS	2		//ִ��ת�������ض����б����洢�����ת����

//�������
#define CAM_MODEL_EULER            0x0008
#define CAM_MODEL_UVN              0x0010

//���������ת˳��
#define CAM_ROT_SEQ_ZYX				4

#define EPSILON_E5 (float)(1E-5)


// double sided flag
#define PLX_2SIDED_FLAG              0x1000   // this poly is double sided
#define PLX_1SIDED_FLAG              0x0000   // this poly is single sided

//�洢��ѯ��
float cos_look[361];					//
float sin_look[361];					//

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

//���õ�λ����
#define MAT_IDENTITY_4X4(m) { memcpy((void*)(m),(void*)&IMAT_4x4,sizeof(MATRIX4X4));}

//���󿽱�
#define MAT_COPY_4X4(src_mat, dest_mat) {memcpy((void *)(dest_mat), (void *)(src_mat), sizeof(MATRIX4X4) ); }

//��������ݽṹ
typedef struct POLY4DV1_TYP
{
	int state;					//״̬��Ϣ
	int attr;					//�����ε���������
	int color;					//����ε���ɫ

	POINT4D_PTR vlist;			//ָ���Լ��Ķ����б�
	int vert[3];				//�����б�����

}POLY4DV1, *POLY4DV1_PTR;

//�԰�����������ݽṹ
typedef struct POLYF4DV1_TYP
{
	int state;					//״̬��Ϣ
	int attr;					//�����ε���������
	UINT color;					//����ε���ɫ

	POINT4D vlist[3];			//�����εĶ���
	POINT4D tvlist[3];			//�任�Ķ���

	POLYF4DV1_TYP *next;		//ָ���б���һ������ε�ָ��
	POLYF4DV1_TYP *prev;		//ָ���б�ǰһ������ε�ָ��

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

//�洢��Ⱦ�б�Ķ�����������ͬʱ�ж����Ⱦ�б�
typedef struct RENDERLIST4DV1_TYP
{
	int state;			//��Ⱦ�б��״̬
	int attr;			//��Ⱦ�б������

	//��Ⱦ�б���һ��ָ������
	//����ÿ��ָ��ָ��һ���԰����ġ�����Ⱦ�Ķ������(POLYF4DV1);
	POLYF4DV1_PTR poly_ptrs[RENDERLIST4DV1_MAX_POLYS];

	//Ϊ����ÿ֡��Ϊ����η�����ͷŴ洢�ռ�
	//����δ洢������������
	POLYF4DV1 poly_data[RENDERLIST4DV1_MAX_POLYS];

	int num_polys;		//��Ⱦ�б��а����Ķ������Ŀ
}RENDERLIST4DV1, *RENDERLIST4DV1_PTR;

//���ڶ����б�Ͷ�����б������ 
typedef struct OBJECT4DV1_TYP
{
	int id;
	char name[64];
	int state;			//��Ⱦ�б��״̬
	int attr;			//��Ⱦ�б������

	float avg_radius;
	float max_radius;

	POINT4D world_pos;

	VECTOR4D dir;

	VECTOR4D ux, uy, uz;

	int num_vertices;	//���嶥����

	POINT4D vlist_local[OBJECT4DV1_MAX_VERTICES];
	POINT4D vlist_trans[OBJECT4DV1_MAX_VERTICES];

	int num_polys;		//������Ķ������
	POLY4DV1 plist[OBJECT4DV1_MAX_POLYS];		//���������
}OBJECT4DV1,*OBJECT4DV1_PTR;

//���Ǻ���
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
	MAT_IDENTITY_4X4(&cam->mcam);
	MAT_IDENTITY_4X4(&cam->mper);
	MAT_IDENTITY_4X4(&cam->mscr);

	//������ر���
	cam->fov = fov;

	//����ƽ���С����Ϊ2x(2/ar)
	cam->viewplane_width = 2.0;
	cam->viewplane_width = static_cast<float>(2.0/cam->aspect_ratio);
	//����fov����ƽ���С�����Ӿ�
	float tan_fov_div2 = static_cast<float>(tan((DEG_TO_RAD(fov / 2))));

	cam->view_dist = static_cast<float>((0.5)*(cam->viewplane_width)*tan_fov_div2);

	//�ж�fov�Ƿ�Ϊ90��
}

inline void Reset_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list)
{
	//���������ʼ������Χ���ݽ�������Ⱦ�б�
	//Ϊ������β��뵽��������׼��
	//����汾����Ⱦ�б���һ��FACE4DV1ָ���������
	//ÿһ֡����Ҫ�����������

	//����ʹ��num_polys��������Ⱦ�б��а����Ķ������Ŀ
	//��˽�������Ϊ0
	//�����Ҫʹ��Ⱦ�б��ͨ�ã���Ҫ���ø���׳�ķ���
	//������������ָ���б�Ĺ����ж�
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

//��������
inline void setPixels(int x, int y, UINT color, DWORD*	imageData, int imageWidth) {
	int index = (int)(imageWidth * y + x);

	imageData[index] = 0xff0000;
}

//��������
inline void drawLine(int x1, int y1, int x2, int y2, UINT color, DWORD* imageData, int imageWidth)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int ux = ((dx > 0) << 1) - 1;//x����������,ȡ1��-1
	int uy = ((dy > 0) << 1) - 1;//y����������,ȡ1��-1

	dx = abs(dx);
	dy = abs(dy);

	int x = x1, y = y1;
	int eps = 0;

	//�߶ο���x��
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
	//�ж��Ƿ�����
	if (rend_list->num_polys >= RENDERLIST4DV1_MAX_POLYS)
		return 0;

	int index = rend_list->num_polys;

	//����ָ��
	rend_list->poly_ptrs[index] = &rend_list->poly_data[index];

	//�����ֶ�
	rend_list->poly_data[index].state = poly->state;
	rend_list->poly_data[index].attr = poly->attr;
	rend_list->poly_data[index].color = poly->color;

	//�������㣬
	memcpy((void*)&rend_list->poly_data[index], (void*)poly, sizeof(POLYF4DV1));

	//��������
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

	//�����б��ж��������
	rend_list->num_polys++;

	return 1;
}

//������Ⱦ�б�
inline void Draw_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list, DWORD* imageData, int imageWidth)
{
	//���������ִ�С���Ⱦ�б�
	//���߿�ģ���£�����Զ���ν������򣬵�������Ҫ��������������������
	//���⣬�����ú���ȥ�ж�λ���������Ӧ�Ĺ�դ����

	//���ڣ�����ֻ��һ��������б����Ի�����
	//�����߿������У������桱�����޹ؽ�Ҫ
	for (int poly=0;poly < rend_list->num_polys;poly++)
	{
		POLYF4DV1_PTR polyPtr = rend_list->poly_ptrs[poly];

		if (!(polyPtr->state & POLY4DV1_STATE_ACTIVE) ||
			(polyPtr->state & POLY4DV1_STATE_CLIPPED) ||
			(polyPtr->state & POLY4DV1_STATE_BACKFACE))
			continue;//������һ�������



		//���������εı�
		//2d��ʼ���������Ѿ����úòü���λ��2d��Ļ/������Ķ���ζ������ü���
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

//������Ⱦ�б�
inline void Draw_OBJECT4DV1_Wire16(OBJECT4DV1_PTR obj, DWORD* imageData, int imageWidth)
{
	//���������ִ�С���Ⱦ�б�
	//���߿�ģ���£�����Զ���ν������򣬵�������Ҫ��������������������
	//���⣬�����ú���ȥ�ж�λ���������Ӧ�Ĺ�դ����

	//���ڣ�����ֻ��һ��������б����Ի�����
	//�����߿������У������桱�����޹ؽ�Ҫ
	for (int poly = 0; poly < obj->num_polys; poly++)
	{
		POLYF4DV1_PTR polyPtr = (POLYF4DV1_PTR)&obj->plist[poly];

		if (!(polyPtr->state & POLY4DV1_STATE_ACTIVE) ||
			(polyPtr->state & POLY4DV1_STATE_CLIPPED) ||
			(polyPtr->state & POLY4DV1_STATE_BACKFACE))
			continue;//������һ�������

		int vindex_0 = obj->plist[poly].vert[0];
		int vindex_1 = obj->plist[poly].vert[1];
		int vindex_2 = obj->plist[poly].vert[2];

		//���������εı�
		//2d��ʼ���������Ѿ����úòü���λ��2d��Ļ/������Ķ���ζ������ü���
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

//������ת
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

inline void Transform_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,	//ת������Ⱦ�б�
									MATRIX4X4_PTR mt,				//ת������
									int coord_select)				//ת��������
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
		//��ȡ��ǰ�����
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
	//�����������ŷ���Ƕȼ�������任���󣬲�����洢����������������
	//��6�½��ܹ���Ҫ��������任������Ҫ��������Mcam = mt(-1) * my(-1) * mx(-1) * mz(-1)
	//�����ƽ�������������y,x,z�����ת����������
	//����ʲô������ת˳����ȫȡ�����û����������û��ǿ�Ʋ���ĳ��˳��,���Ǹ��ݲ���cam_rot_seq��ֵ��������������˳��
	//�ò�����ȡֵΪCAM_ROT_SEQ_XYZ,����XYZ���԰��κ�˳�����У���YXZ,ZXY��

	MATRIX4X4 mt_inv,	//���ƽ�ƾ���������
		mx_inv,			//�����x�����ת����������
		my_inv,			//�����y�����ת����������
		mz_inv,			//�����z�����ת����������
		mrot,			//��������ת����Ļ�
		mtmp;			//���ڴ洢��ʱ����

	//��һ�����������λ�ü������ƽ�ƾ���������
	Mat_Init_4X4(&mt_inv, 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-cam->pos.x, -cam->pos.y, -cam->pos.z, 1);

	//�ڶ�����������ת����������
	//Ҫ������ת���������󣬿��Խ���ת�ã�Ҳ���Խ�ÿ����ת�Ƕ�ȡ��

	//��ȡŷ���Ƕ�
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

		//����ո���
		for (length = strlen(buffer), index = 0; isspace(buffer[index]); index++);

		if (index >= length || buffer[index] == '#')
			continue;
				
			
		//��ʱ�õ���һ��������
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

	//��ȡ����������
	if(!(token_string = Get_Line_PLG(buffer,255,fp)))
	{
		printf("PLG file error with file %s(object descriptor invalid).", filename);
		return 0;
	}

	//���������������з���
	sscanf(token_string, "%s %d %d", obj->name, &obj->num_vertices, &obj->num_polys);

	//���ض����б�
	for (int vertex = 0; vertex < obj->num_vertices; ++vertex)
	{
		//��ȡ����������
		if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
		{
			printf("PLG file error with file %s(vertex list invalid).", filename);
			return 0;
		}

		//��������
		sscanf(token_string, "%f %f %f", &obj->vlist_local[vertex].x, &obj->vlist_local[vertex].y, &obj->vlist_local[vertex].z);
		obj->vlist_local[vertex].w = 1;

		obj->vlist_local[vertex].x *= vscale->x;
		obj->vlist_local[vertex].y *= vscale->y;
		obj->vlist_local[vertex].z *= vscale->z;
	}

	int poly_surface_desc = 0;	//PLG/PLX�����������
	int poly_num_verts = 0;		//��ǰ����εĶ�����
	char temp_string[8];

	//���岽�����ض�����б�
	for (int poly = 0; poly < obj->num_polys; ++poly)
	{
		//��ȡ�����������
		if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
		{
			printf("PLG file error with file %s(polygon descriptor invalid).", filename);
			return 0;
		}

		//ÿ�����������������

		sscanf(token_string, "%s %d %d %d %d", temp_string,&poly_num_verts,&obj->plist[poly].vert[0], &obj->plist[poly].vert[1], &obj->plist[poly].vert[2]);


		obj->plist[poly].state = POLY4DV1_STATE_ACTIVE;
	}

	fclose(fp);

	return 1;
}
