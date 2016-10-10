#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// 함수 프로토타입
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// 전역 변수
HINSTANCE hinst;
HDC hdc;									// 디바이스 컨텍스트의 핸들
TCHAR ch = ' ';								// 입력한 문자
RECT rect;									// 사각형 구조체
PAINTSTRUCT ps;								// WM_PAINT에서 사용

// 상수
const char CLASS_NAME[] = "WinMain";		
const char APP_TITLE[] = "HEllo World";		// 제목 표시줄의 텍스트
const int WINDOW_WIDTH = 400;				// 윈도우의 폭
const int WINDOW_HEIGHT = 300;				// 윈도우의 높이

// ====================================================
// 윈도우 애플리케이션의 시작 위치
// ====================================================

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
			// 종료 메시지를 찾는다
			if (msg.message == WM_QUIT)
				done = -1;
			// 해석한 뒤 메시지를 WinProc에 전달한다
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
// ====================================================
// 윈도우 이벤트 콜백(Callback) 함수
// ====================================================
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY:
			// 윈도우에게 이 프로그램을 종료하라고 말한다
			PostQuitMessage(0);
			return 0;
		case WM_CHAR:		// 키보드에서 문자를 입력했다
		{
			switch (wParam)
			{
				case 0x08:		// 백스페이스(Backspace)
				case 0x09:		// 탭(Tab)
				case 0x0A:		// 라인피드(Linefeed)
				case 0x0D:		// 캐리지 리턴(Carriage Return)
				case 0X1B:		// 이스케이프(Escape)
					MessageBeep((UINT)-1);		// 비프음이 들리지만 표시되지는 않는다
				default:		// 표시 할 수 있는 문자
					ch = (TCHAR)wParam;	//문자를 가져온다
					InvalidateRect(hwnd, NULL, TRUE);	// WM_PAINT를 강제한다
					return 0;
			}
		}
		case WM_PAINT:		// 윈도우를 다시 그려야 하는 경우
			hdc = BeginPaint(hwnd, &ps); // 디바이스 컨텍스트의 핸들을 가져온다
			GetClientRect(hwnd, &rect);	 // 윈도우의 사각형을 가져온다
			// 문자를 표시한다
			TextOut(hdc, rect.right / 2, rect.bottom / 2, &ch, 1);
			EndPaint(hwnd, &ps);
			return 0;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

// ====================================================
// 윈도우 생성
// 에러가 발생할 경우 false를 반환한다
// ====================================================

bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;

	// Window 클래스 구조체를 메인 윈도우에 대한 매개변수로 채운다
	wcx.cbSize = sizeof(wcx);		// 구조체의 크기
	wcx.style = CS_HREDRAW | CS_VREDRAW; // 크기가 변하면 다시 그린다
	wcx.lpfnWndProc = WinProc;		// 윈도우 프로시저를 가리킨다
	wcx.cbClsExtra = 0;				// 여분의 클래스 메모리는 필요 없다
	wcx.cbWndExtra = 0;				// 여분의 윈도우 메모리는 필요 없다
	wcx.hInstance = hInstance;		// 인스턴스의 핸들
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);	// 미리 정의된 화살표

	// 배경 브러시
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;		// 메뉴 리소스의 이름
	wcx.lpszClassName = CLASS_NAME; // 윈도우 클래스의 이름
	wcx.hIconSm = NULL;				// 작은 클래스 아이콘

	// Window 클래스를 등록한다
	// RegisterClassEx 함수는 에러가 발생할 경우 0을 반환한다
	if (RegisterClassEx(&wcx) == 0)	// 에러가 발생한다면
		return false;
	
	// 윈도우 생성
	hwnd = CreateWindow(
		CLASS_NAME,					// 윈도우 클래스의 이름
		APP_TITLE,					// 제목 표시줄의 텍스트
		WS_OVERLAPPEDWINDOW,		// 윈도우 스타일
		CW_USEDEFAULT,				// 윈도우의 기본 수평 위치
		CW_USEDEFAULT,				// 윈도우의 기본 수직 위치
		WINDOW_WIDTH,				// 윈도우의 폭
		WINDOW_HEIGHT,				// 윈도우의 높이
		(HWND)NULL,					// 부모 윈도우 없음
		(HMENU)NULL,				// 메뉴 없음
		hInstance,					// 애플리케이션 인스턴스의 핸들
		(LPVOID)NULL				// 윈도우 매개변수 없음
		);

	// 윈도우를 생성하는 동안 에러가 발생한다면
	if (!hwnd)
		return false;				// 더 이상 진행할 수 없으므로 종료한다

	// 윈도우를 표시한다
	ShowWindow(hwnd, nCmdShow);

	// 윈도우 프로시저에게 WM_PAINT메시지를 보낸다
	UpdateWindow(hwnd);
	return true;
}