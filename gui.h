#include <Windows.h>
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "Dwmapi.lib")
#include <d3dx9.h>
#include <d3d9.h>
#include <dwmapi.h>

typedef void render_func();

namespace gui {

	extern int Width;
	extern int Height;
	extern void* render_hack;

	LRESULT CALLBACK Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	BOOL RegisterDLLWindowClass(wchar_t szClassName[]);

	int DirectXxInit(HWND hWnd);
	void RenderLoop(const char* windowtitle);
	int Render();

	void DrawFilledBox(float x, float y, float w, float h, int r, int g, int b, int a);
	void DrawBox(float x, float y, float width, float height, float px, int r, int g, int b, int a);
	int text(const char* String, int x, int y, int r, int g, int b);
	int DrawShadowText(const char* String, int x, int y, int r, int g, int b);
}