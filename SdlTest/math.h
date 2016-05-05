#pragma once

//多边形和面的状态
#define POLY4DV1_STATE_ACTIVE			0x0001
#define POLY4DV1_STATE_CLIPPED			0x0002
#define POLY4DV1_STATE_BACKFACE			0x0004


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
}VECTOR4D,POINT4D,*VECTOR4D_PTR,*POINT4D_PTR;

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