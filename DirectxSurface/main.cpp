#include <windows.h>
#include <d3d9.h>

#include "myMath.h"

#pragma comment(lib, "d3d9.lib")


/*��������*/
#define WINDOW_WIDTH      640		 //���ڳߴ�
#define WINDOW_HEIGHT     480


/*��������*/
HWND				m_hwnd;

IDirect3DDevice9*	m_device;		//d3d�豸
IDirect3DSurface9*	m_surface;		//directx����
D3DSURFACE_DESC		m_surfacedesc;	//��������

DWORD*				m_imageData;	//��������


//��ʼ�������λ�úͷ���
POINT4D cam_pos = { 0,0,-100,1 };
VECTOR4D cam_dir = { 0,0,0,1 };


CAM4DV1				m_cam;			//���

RENDERLIST4DV1		m_rend_list;	//��Ⱦ�б�
POLYF4DV1			m_poly1;		//�����
POINT4D				m_poly1_pos;	//�������������

void Game_Init();

LRESULT CALLBACK sWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		//draw();
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void render()
{
	Sleep(100);

	static MATRIX4X4 mrot;	//��ͨ��ת����
	static float ang_y = 0;	//��ת�Ƕ�

	//��ʼ����Ⱦ�б�
	Reset_RENDERLIST4DV1(&m_rend_list);

	//�������ε���Ⱦ�б�
	Insert_POLYF4DV1_RENDERLIST4DV1(&m_rend_list, &m_poly1);

	//Y����ͨ��ת
	Build_XYZ_Rotation_MATRIX4X4(0, ang_y, 0, &mrot);

	//��ת�����
	if (++ang_y > 360.0) ang_y = 0;

	//��ת��Ⱦ�б��е�������εı�������
	Transform_RENDERLIST4DV1(&m_rend_list, &mrot, TRANSFORM_LOCAL_ONLY);
	
	//ִ�б���/ģ�͵�����ת��
	Model_To_World_RENDERLIST4DV1(&m_rend_list, &m_poly1_pos);

	//�����������
	Build_CAM4DV1_Matrix_Euler(&m_cam, CAM_ROT_SEQ_ZYX);

	World_To_Camera_RENDERLIST4DV1(&m_rend_list, &m_cam);

	Camera_To_Perspective_RENDERLIST4DV1(&m_rend_list, &m_cam);

	Perspective_To_Screen_RENDERLIST4DV1(&m_rend_list, &m_cam);

	Draw_RENDERLIST4DV1(&m_rend_list, m_imageData, WINDOW_WIDTH);
}


void draw()
{
	//��Ⱦ����������
	//ȡ��ָ���ڴ�����ָ��
	D3DLOCKED_RECT lockedRect;
	m_surface->LockRect(&lockedRect, 0, 0);

	m_imageData = (DWORD*)lockedRect.pBits;

	memset(m_imageData, 0, WINDOW_WIDTH* WINDOW_HEIGHT*4);

	render();


	m_surface->UnlockRect();
	m_imageData = nullptr;

	//��Ⱦ
	m_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	m_device->BeginScene();

	IDirect3DSurface9 * pBackBuffer = NULL;

	m_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_device->StretchRect(m_surface, NULL, pBackBuffer, NULL, D3DTEXF_LINEAR);

	m_device->EndScene();
	m_device->Present(0, 0, 0, 0);

	pBackBuffer->Release();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	WNDCLASSEX winClass;

	winClass.lpszClassName = "DirectSurface";
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = sWndProc;
	winClass.hInstance = hInstance;
	winClass.hIcon = NULL;
	winClass.hIconSm = NULL;
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = NULL;
	winClass.lpszMenuName = NULL;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;

	if (!RegisterClassEx(&winClass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), "error", MB_ICONERROR);
		return 1;
	}

	m_hwnd = CreateWindowEx(NULL,
		"DirectSurface",					// window class name
		"Draw Surface",			// window caption
		WS_OVERLAPPEDWINDOW, 		// window style
		CW_USEDEFAULT,				// initial x position
		CW_USEDEFAULT,				// initial y position
		WINDOW_WIDTH,						// initial x size
		WINDOW_HEIGHT,						// initial y size
		NULL,						// parent window handle
		NULL,						// window menu handle
		hInstance,					// program instance handle
		NULL);						// creation parameters

	//��ʼ��directx
	D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;

	HRESULT hr = 0;

	// Step 1: Create the IDirect3D9 object.

	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d9)
	{
		::MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
		return 0;
	}

	// Step 2: Check for hardware vp.

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = WINDOW_WIDTH;
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = m_hwnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 4: Create the device.

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		deviceType,         // device type
		m_hwnd,               // window associated with device
		vp,                 // vertex processing
		&d3dpp,             // present parameters
		&m_device);            // return created device

	if (FAILED(hr))
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			m_hwnd,
			vp,
			&d3dpp,
			&m_device);

		if (FAILED(hr))
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, "CreateDevice() - FAILED", 0, 0);
			return 0;
		}
	}

	d3d9->Release(); // done with d3d9 object

	m_device->CreateOffscreenPlainSurface(WINDOW_WIDTH, WINDOW_HEIGHT, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_surface, NULL);
	m_surface->GetDesc(&m_surfacedesc);

	//��ʼ����Ϸ
	Game_Init();


	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	MSG    msg;
	ZeroMemory(&msg, sizeof(msg));

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{

			if (msg.message == WM_QUIT)
				break;

			// translate any accelerator keys
			TranslateMessage(&msg);

			// send the message to the window proc
			DispatchMessage(&msg);
		} // end while

		draw();
	}

	//�ͷ���Դ
	m_device->Release();
	if (m_surface)
		m_surface->Release();

	return 0;
}

void Game_Init()
{
	//��ʼ����ѧ����
	Build_Sin_Cos_Tables();

	//��ʼ��һ�������
	m_poly1.state = POLY4DV1_STATE_ACTIVE;
	m_poly1.attr = 0;
	m_poly1.color = D3DCOLOR_XRGB(0, 0xFF, 0);

	m_poly1.vlist[0].x = 0;
	m_poly1.vlist[0].y = 50;
	m_poly1.vlist[0].z = 0;
	m_poly1.vlist[0].w = 1;

	m_poly1.vlist[1].x = 50;
	m_poly1.vlist[1].y = -50;
	m_poly1.vlist[1].z = 0;
	m_poly1.vlist[1].w = 1;

	m_poly1.vlist[2].x = -50;
	m_poly1.vlist[2].y = -50;
	m_poly1.vlist[2].z = 0;
	m_poly1.vlist[2].w = 1;

	m_poly1.next = m_poly1.prev = NULL;

	//ʹ��90FOV��ʼ�����
	Init_CAM4DV1(&m_cam,
		CAM_MODEL_EULER,		//ŷ�����ģ��
		&cam_pos,				//��ʼ���λ��
		&cam_dir,				//��ʼ����Ƕ�
		NULL,					//��Ŀ��
		50.0,					//Զ���ü���
		500.0,
		90.0,					//�Ӿ�Ƕ�
		WINDOW_WIDTH,			//������Ļ��ͼ
		WINDOW_HEIGHT);
}
