#include "gui.h"

namespace gui {

	int Width = 1920;
	int Height = 1080;

	HWND hWnd;
	MSG Message;
	HWND TargetWnd;
	HINSTANCE hInstance;
	RECT TargetRect;
	RECT CheckWnd;
	HINSTANCE  inj_hModule;
	MARGINS Margin = { 0, 0, Width, Height };

	IDirect3D9Ex* dx_Object = NULL;
	IDirect3DDevice9Ex* dx_Device = NULL;
	D3DPRESENT_PARAMETERS dx_Param;
	ID3DXFont* dx_Font = 0;

	void* render_hack;

	int DirectXxInit(HWND _hWnd)
	{
		if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &dx_Object)))
			exit(1);

		hWnd = _hWnd;

		dx_Param.BackBufferFormat = D3DFMT_A8R8G8B8;
		dx_Param.hDeviceWindow = hWnd;
		dx_Param.MultiSampleQuality = DEFAULT_QUALITY;
		dx_Param.SwapEffect = D3DSWAPEFFECT_DISCARD;
		dx_Param.Windowed = true;

		if (FAILED(dx_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dx_Param, 0, &dx_Device)))
			exit(1);

		D3DXCreateFont(dx_Device, 8, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "System", &dx_Font);

		return 0;
	}

	void RenderLoop(const char* windowtitle) {
		if (PeekMessage(&Message, hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		TargetWnd = FindWindow(0, windowtitle);
		GetWindowRect(TargetWnd, &TargetRect);
		MoveWindow(hWnd, TargetRect.left, TargetRect.top, TargetRect.right - TargetRect.left, TargetRect.bottom - TargetRect.top, true);

		if (!TargetWnd)
		{
			exit(0);
		}

		gui::Render();
		Sleep(1);
	}

	LRESULT CALLBACK Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		switch (Message)
		{
		case WM_PAINT:
			DwmExtendFrameIntoClientArea(hWnd, &Margin);

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);
		}

		return 0;
	}

	BOOL RegisterDLLWindowClass(wchar_t szClassName[])
	{
		WNDCLASSEX OverlayWnd;

		OverlayWnd.cbClsExtra = 0;
		OverlayWnd.cbSize = sizeof(WNDCLASSEX);
		OverlayWnd.cbWndExtra = 0;
		OverlayWnd.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
		OverlayWnd.hCursor = LoadCursor(0, IDC_ARROW);
		OverlayWnd.hIcon = LoadIcon(0, IDI_APPLICATION);
		OverlayWnd.hIconSm = LoadIcon(0, IDI_APPLICATION);
		OverlayWnd.hInstance = hInstance;
		OverlayWnd.lpfnWndProc = Proc;
		OverlayWnd.lpszClassName = "overlay";
		OverlayWnd.lpszMenuName = "overlay";
		OverlayWnd.style = CS_HREDRAW | CS_VREDRAW;

		if (!RegisterClassEx(&OverlayWnd))
			return 0;
	}

	void DrawFilledBox(float x, float y, float w, float h, int r, int g, int b, int a)
	{
		static ID3DXLine* pLine;

		if (!pLine)
			D3DXCreateLine(dx_Device, &pLine);

		D3DXVECTOR2 vLine[2];

		pLine->SetWidth(w);
		pLine->SetAntialias(false);
		pLine->SetGLLines(true);

		vLine[0].x = x + w / 2;
		vLine[0].y = y;
		vLine[1].x = x + w / 2;
		vLine[1].y = y + h;

		pLine->Begin();
		pLine->Draw(vLine, 2, D3DCOLOR_RGBA(r, g, b, a));
		pLine->End();
	}

	void DrawBox(float x, float y, float width, float height, float px, int r, int g, int b, int a)
	{
		DrawFilledBox(x, y + height, width, px, r, g, b, a);
		DrawFilledBox(x - px, y, px, height, r, g, b, a);
		DrawFilledBox(x, y - px, width, px, r, g, b, a);
		DrawFilledBox(x + width, y, px, height, r, g, b, a);
	}

	int text(const char* String, int x, int y, int r, int g, int b)
	{
		RECT Font;
		Font.bottom = 0;
		Font.left = x;
		Font.top = y;
		Font.right = 0;

		dx_Font->DrawTextA(0, String, (int)strlen(String), &Font, DT_NOCLIP, D3DCOLOR_ARGB(255, r, g, b));

		return 0;
	}

	int DrawShadowText(const char* String, int x, int y, int r, int g, int b)
	{
		RECT Font;
		Font.bottom = 0;
		Font.left = x;
		Font.top = y;
		Font.right = 0;

		RECT Fonts;
		Fonts.bottom = 0;
		Fonts.left = x + 1;
		Fonts.top = y + 1;
		Fonts.right = 0;

		dx_Font->DrawTextA(0, String, (int)strlen(String), &Fonts, DT_NOCLIP, D3DCOLOR_ARGB(255, 1, 1, 1));
		dx_Font->DrawTextA(0, String, (int)strlen(String), &Font, DT_NOCLIP, D3DCOLOR_ARGB(255, r, g, b));

		return 0;
	}

	int Render()
	{
		dx_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
		dx_Device->BeginScene();

		if (TargetWnd == GetForegroundWindow())
		{
			((render_func*)render_hack)();
		}
		dx_Device->EndScene();
		dx_Device->PresentEx(0, 0, 0, 0, 0);
		return 0;
	}
}