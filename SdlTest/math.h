#pragma once

//����κ����״̬
#define POLY4DV1_STATE_ACTIVE			0x0001
#define POLY4DV1_STATE_CLIPPED			0x0002
#define POLY4DV1_STATE_BACKFACE			0x0004


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
}VECTOR4D,POINT4D,*VECTOR4D_PTR,*POINT4D_PTR;

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