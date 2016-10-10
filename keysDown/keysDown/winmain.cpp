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
bool vkKeys[256];

// ���
const char CLASS_NAME[] = "Keyboard";
const char APP_TITLE[] = "Keys Down";	// ���� ǥ������ �ؽ�Ʈ
const int WINDOW_WIDTH = 400;			// �������� ��
const int WINDOW_HEIGHT = 400;			// �������� ����

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	MSG msg;
	// ������ ����
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;

	for (int i = 0; i < 256; i++)	// ���� Ű �迭 ����
		vkKeys[i] = false;

	// ���� �޽��� ����
	int done = 0;
	while (!done)
	{
		// PeekMessage�� ������ �޽����� Ȯ���ϴ� ����ŷ �޼ҵ��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ���� �޽����� ã�´�.
			if (msg.message == WM_QUIT)
				done = -1;
			// �ؼ��� �� �޽����� WinProc�� �����Ѵ�
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

// =================================================================
// ������ �̺�Ʈ �ݹ�(Callback)�Լ�
// =================================================================
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	short nVirtKey;
	const short SHIFTED = (short)0x8000;
	TEXTMETRIC tm;
	DWORD chWidth = 20;
	DWORD chHeight = 20;

	switch (msg)
	{
		case WM_CREATE:
			// ���� metrics�� �����´�
			hdc = GetDC(hwnd);
			GetTextMetrics(hdc, &tm);
			ReleaseDC(hwnd, hdc);
			chWidth = tm.tmAveCharWidth;		// ��� ������ �ʺ�
			chHeight = tm.tmHeight;				// ������ ����
			return 0;

		case WM_DESTROY:
			// �����쿡�� �� ���α׷��� �����϶�� �˸���
			PostQuitMessage(0);
			return 0;
			
		case WM_KEYDOWN:								// Ű�� ���� ���
			vkKeys[wParam] = true;
			switch (wParam)
			{
				case VK_SHIFT:							// ����Ʈ(shift)Ű
					nVirtKey = GetKeyState(VK_LSHIFT);	// ���� ����Ʈ�� ���¸� �����´�
					if (nVirtKey & SHIFTED)				// ���� ����Ʈ�� �����ٸ�
						vkKeys[VK_LSHIFT] = true;
					nVirtKey = GetKeyState(VK_RSHIFT);	// ������ ����Ʈ�� ���¸� �����´�
					if (nVirtKey & SHIFTED)				// ������ ����Ʈ�� �����ٸ�
						vkKeys[VK_RSHIFT] = true;
					break;
				case VK_CONTROL:						// ��Ʈ��(control)Ű
					nVirtKey = GetKeyState(VK_LCONTROL);
					if (nVirtKey & SHIFTED)				// ���� ��Ʈ���� �����ٸ�
						vkKeys[VK_LCONTROL] = true;
					nVirtKey = GetKeyState(VK_RCONTROL);
					if (nVirtKey & SHIFTED)				// ������ ��Ʈ���� �����ٸ�
						vkKeys[VK_RCONTROL] = true;
					break;
			}
			InvalidateRect(hwnd, NULL, TRUE);			// WM_PAINT�� �����Ѵ�
			return 0;
			break;
		case WM_KEYUP:									// Ű�� �� ���
			vkKeys[wParam] = false;
			switch (wParam)
			{
				case VK_SHIFT:							// ����Ʈ(shift)Ű
					nVirtKey = GetKeyState(VK_LSHIFT);
					if ((nVirtKey & SHIFTED) == 0)		// ���� ����Ʈ�� �����ٸ�
						vkKeys[VK_LSHIFT] = false;
					nVirtKey = GetKeyState(VK_RSHIFT);
					if ((nVirtKey & SHIFTED) == 0)		// ������ ����Ʈ�� �����ٸ�
						vkKeys[VK_RSHIFT] = false;
					break;
				case VK_CONTROL:						// ��Ʈ��(control)Ű
					nVirtKey = GetKeyState(VK_LCONTROL);
					if ((nVirtKey & SHIFTED) == 0)		// ���� ��Ʈ���� �����ٸ�
						vkKeys[VK_LCONTROL] = false;
					nVirtKey = GetKeyState(VK_RCONTROL);
					if ((nVirtKey & SHIFTED) == 0)		// ������ ��Ʈ���� �����ٸ�
						vkKeys[VK_RCONTROL] = false;
					break;
			}
			InvalidateRect(hwnd, NULL, TRUE);			// WM_PAINT�� �����Ѵ�
			return 0;
			break;

		case WM_CHAR:		//Ű���忡�� ���ڸ� �Է��ߴ�
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

		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			TextOut(hdc, 0, 0, &ch, 1);

			for (int r = 0; r < 16; r++)
			{
				for (int c = 0; c < 16; c++)
				{
					if (vkKeys[r * 16 + c])
					{
						SetBkMode(hdc, OPAQUE);				// ���� �޹���� �������ϰ� �Ѵ�
						TextOut(hdc, c*chWidth + chWidth * 2, r*chHeight + chHeight * 2, "T ", 2);
					}
					else
					{
						SetBkMode(hdc, TRANSPARENT);		// ���� �޹���� �����ϰ� �Ѵ�
						TextOut(hdc, c*chWidth + chWidth * 2, r*chHeight + chHeight * 2, "F ", 2);
					}
				}
			}

			EndPaint(hwnd, &ps);
			return 0;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

// =================================================================
// ������ ����
// ������ �߻��� ��� false�� ��ȯ�Ѵ�
// =================================================================
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;

	// Window Ŭ���� ����ü�� ���� �����쿡 ���� �Ű������� ä���

	wcx.cbSize = sizeof(wcx);					// ����ü�� ũ��
	wcx.style = CS_HREDRAW | CS_VREDRAW;		// ũ�Ⱑ ���ϸ� �ٽ� �׸���
	wcx.lpfnWndProc = WinProc;					// ������ ���ν����� ����Ų��
	wcx.cbClsExtra = 0;							// ������ Ŭ���� �޸𸮴� �ʿ� ����
	wcx.cbWndExtra = 0;							// ������ ������ �޸𸮴� �ʿ� ����
	wcx.hInstance = hInstance;					// �ν��Ͻ��� �ڵ�
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);	// �̸� ���ǵ� ȭ��ǥ

	// ��� �귯��
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcx.lpszMenuName = NULL;					// �޴� ���ҽ��� �̸�
	wcx.lpszClassName = CLASS_NAME;				// ������ Ŭ������ �̸�
	wcx.hIconSm = NULL;							// ���� Ŭ���� ������

												// Window Ŭ������ ����Ѵ�
												// RegisterClassEx �Լ��� ������ �߻��� ��� 0�� ��ȯ�Ѵ�
	if (RegisterClassEx(&wcx) == 0)				// ���� ������ �߻��Ѵٸ�
		return false;

	//������ ����
	hwnd = CreateWindow(
		CLASS_NAME,				// ������ Ŭ������ �̸�
		APP_TITLE,				// ���� ǥ������ �ؽ�Ʈ
		WS_OVERLAPPEDWINDOW,	// ������ ��Ÿ��
		CW_USEDEFAULT,			// �������� �⺻ ���� ��ġ
		CW_USEDEFAULT,			// �������� �⺻ ���� ��ġ
		WINDOW_WIDTH,			// �������� ��
		WINDOW_HEIGHT,			// �������� ����
		(HWND)NULL,				// �θ� ������ ����
		(HMENU)NULL,			// �޴� ����
		hInstance,				// ���ø����̼� �ν��Ͻ��� �ڵ�
		(LPVOID)NULL			// ������ �Ű����� ����
		);

	// �����츦 �����ϴ� ���� ������ �߻��Ѵٸ�
	if (!hwnd)
		return false;			// �� �̻� ������ �� �����Ƿ� �����Ѵ�

								// �����츦 ǥ���Ѵ�
	ShowWindow(hwnd, nCmdShow);

	// ������ ���ν������� WM_PAINT �޽����� ������
	UpdateWindow(hwnd);
	return true;
}