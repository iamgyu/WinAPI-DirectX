#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// 함수 프로토타입
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// 전역 변수
HINSTANCE hinst;
// 상수
const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "Hello World";	// 제목 표시줄의 텍스트
const int WINDOW_WIDTH = 400;			// 윈도우의 폭
const int WINDOW_HEIGHT = 400;			// 윈도우의 높이

// =================================================================
// 윈도우 애플리케이션의 시작 위치
// 매개변수는 다음과 같다
// hInstance. 애플리케이션의 현재 인스턴스에 대한 핸들
// hPrevInstance. 더 이상 사용하지 않는 매개변수이며, 항상 NULL이다
// lpCmdLine. 커맨드라인 인수의 NULL로 끝나는 문자열을 가리키는 포인터
// nCmdShow. 윈도우를 어떻게 보여줄 것인지 지정한다
// =================================================================

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	MSG msg;
	// 윈도우 생성
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;
	// 메인 메시지 루프
	int done = 0;
	while (!done)
	{
		// PeekMessage는 윈도우 메시지를 확인하는 논블로킹 메소드다
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// 종료 메시지를 찾는다.
			if (msg.message == WM_QUIT)
				done = -1;
			// 해석한 뒤 메시지를 WinProc에 전달한다
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

// =================================================================
// 윈도우 이벤트 콜백(Callback)함수
// =================================================================
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		// 윈도우에게 이 프로그램을 종료하라고 알린다
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

// =================================================================
// 윈도우 생성
// 에러가 발생할 경우 false를 반환한다
// =================================================================
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;

	// Window 클래스 구조체를 메인 윈도우에 대한 매개변수로 채운다

	wcx.cbSize = sizeof(wcx);					// 구조체의 크기
	wcx.style = CS_HREDRAW | CS_VREDRAW;		// 크기가 변하면 다시 그린다
	wcx.lpfnWndProc = WinProc;					// 윈도우 프로시저를 가리킨다
	wcx.cbClsExtra = 0;							// 여분의 클래스 메모리는 필요 없다
	wcx.cbWndExtra = 0;							// 여분의 윈도우 메모리는 필요 없다
	wcx.hInstance = hInstance;					// 인스턴스의 핸들
	wcx.hIcon = NULL;					
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);	// 미리 정의된 화살표

	// 배경 브러시
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;					// 메뉴 리소스의 이름
	wcx.lpszClassName = CLASS_NAME;				// 윈도우 클래스의 이름
	wcx.hIconSm = NULL;							// 작은 클래스 아이콘

	// Window 클래스를 등록한다
	// RegisterClassEx 함수는 에러가 발생할 경우 0을 반환한다
	if (RegisterClassEx(&wcx) == 0)				// 만약 에러가 발생한다면
		return false;

	//윈도우 생성
	hwnd = CreateWindow(
		CLASS_NAME,				// 윈도우 클래스의 이름
		APP_TITLE,				// 제목 표시줄의 텍스트
		WS_OVERLAPPEDWINDOW,	// 윈도우 스타일
		CW_USEDEFAULT,			// 윈도우의 기본 수평 위치
		CW_USEDEFAULT,			// 윈도우의 기본 수직 위치
		WINDOW_WIDTH,			// 윈도우의 폭
		WINDOW_HEIGHT,			// 윈도우의 높이
		(HWND)NULL,				// 부모 윈도우 없음
		(HMENU)NULL,			// 메뉴 없음
		hInstance,				// 애플리케이션 인스턴스의 핸들
		(LPVOID)NULL			// 윈도우 매개변수 없음
	);

	// 윈도우를 생성하는 동안 에러가 발생한다면
	if (!hwnd)
		return false;			// 더 이상 진행할 수 없으므로 종료한다

	// 윈도우를 표시한다
	ShowWindow(hwnd, nCmdShow);
	
	// 윈도우 프로시저에게 WM_PAINT 메시지를 보낸다
	UpdateWindow(hwnd);
	return true;
}