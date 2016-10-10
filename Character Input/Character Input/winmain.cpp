#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// �Լ� ������Ÿ��
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// ���� ����
HINSTANCE hinst;
HDC hdc;									// ����̽� ���ؽ�Ʈ�� �ڵ�
TCHAR ch = ' ';								// �Է��� ����
RECT rect;									// �簢�� ����ü
PAINTSTRUCT ps;								// WM_PAINT���� ���

// ���
const char CLASS_NAME[] = "WinMain";		
const char APP_TITLE[] = "HEllo World";		// ���� ǥ������ �ؽ�Ʈ
const int WINDOW_WIDTH = 400;				// �������� ��
const int WINDOW_HEIGHT = 300;				// �������� ����

// ====================================================
// ������ ���ø����̼��� ���� ��ġ
// ====================================================

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	MSG msg;
	// ������ ����
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;

	// ���� �޽��� ����
	int done = 0;
	while (!done)
	{
		// PeekMessage�� ������ �޽����� Ȯ���ϴ� ����ŷ �޼ҵ��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ���� �޽����� ã�´�
			if (msg.message == WM_QUIT)
				done = -1;
			// �ؼ��� �� �޽����� WinProc�� �����Ѵ�
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
// ====================================================
// ������ �̺�Ʈ �ݹ�(Callback) �Լ�
// ====================================================
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY:
			// �����쿡�� �� ���α׷��� �����϶�� ���Ѵ�
			PostQuitMessage(0);
			return 0;
		case WM_CHAR:		// Ű���忡�� ���ڸ� �Է��ߴ�
		{
			switch (wParam)
			{
				case 0x08:		// �齺���̽�(Backspace)
				case 0x09:		// ��(Tab)
				case 0x0A:		// �����ǵ�(Linefeed)
				case 0x0D:		// ĳ���� ����(Carriage Return)
				case 0X1B:		// �̽�������(Escape)
					MessageBeep((UINT)-1);		// �������� �鸮���� ǥ�õ����� �ʴ´�
				default:		// ǥ�� �� �� �ִ� ����
					ch = (TCHAR)wParam;	//���ڸ� �����´�
					InvalidateRect(hwnd, NULL, TRUE);	// WM_PAINT�� �����Ѵ�
					return 0;
			}
		}
		case WM_PAINT:		// �����츦 �ٽ� �׷��� �ϴ� ���
			hdc = BeginPaint(hwnd, &ps); // ����̽� ���ؽ�Ʈ�� �ڵ��� �����´�
			GetClientRect(hwnd, &rect);	 // �������� �簢���� �����´�
			// ���ڸ� ǥ���Ѵ�
			TextOut(hdc, rect.right / 2, rect.bottom / 2, &ch, 1);
			EndPaint(hwnd, &ps);
			return 0;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

// ====================================================
// ������ ����
// ������ �߻��� ��� false�� ��ȯ�Ѵ�
// ====================================================

bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;

	// Window Ŭ���� ����ü�� ���� �����쿡 ���� �Ű������� ä���
	wcx.cbSize = sizeof(wcx);		// ����ü�� ũ��
	wcx.style = CS_HREDRAW | CS_VREDRAW; // ũ�Ⱑ ���ϸ� �ٽ� �׸���
	wcx.lpfnWndProc = WinProc;		// ������ ���ν����� ����Ų��
	wcx.cbClsExtra = 0;				// ������ Ŭ���� �޸𸮴� �ʿ� ����
	wcx.cbWndExtra = 0;				// ������ ������ �޸𸮴� �ʿ� ����
	wcx.hInstance = hInstance;		// �ν��Ͻ��� �ڵ�
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);	// �̸� ���ǵ� ȭ��ǥ

	// ��� �귯��
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;		// �޴� ���ҽ��� �̸�
	wcx.lpszClassName = CLASS_NAME; // ������ Ŭ������ �̸�
	wcx.hIconSm = NULL;				// ���� Ŭ���� ������

	// Window Ŭ������ ����Ѵ�
	// RegisterClassEx �Լ��� ������ �߻��� ��� 0�� ��ȯ�Ѵ�
	if (RegisterClassEx(&wcx) == 0)	// ������ �߻��Ѵٸ�
		return false;
	
	// ������ ����
	hwnd = CreateWindow(
		CLASS_NAME,					// ������ Ŭ������ �̸�
		APP_TITLE,					// ���� ǥ������ �ؽ�Ʈ
		WS_OVERLAPPEDWINDOW,		// ������ ��Ÿ��
		CW_USEDEFAULT,				// �������� �⺻ ���� ��ġ
		CW_USEDEFAULT,				// �������� �⺻ ���� ��ġ
		WINDOW_WIDTH,				// �������� ��
		WINDOW_HEIGHT,				// �������� ����
		(HWND)NULL,					// �θ� ������ ����
		(HMENU)NULL,				// �޴� ����
		hInstance,					// ���ø����̼� �ν��Ͻ��� �ڵ�
		(LPVOID)NULL				// ������ �Ű����� ����
		);

	// �����츦 �����ϴ� ���� ������ �߻��Ѵٸ�
	if (!hwnd)
		return false;				// �� �̻� ������ �� �����Ƿ� �����Ѵ�

	// �����츦 ǥ���Ѵ�
	ShowWindow(hwnd, nCmdShow);

	// ������ ���ν������� WM_PAINT�޽����� ������
	UpdateWindow(hwnd);
	return true;
}