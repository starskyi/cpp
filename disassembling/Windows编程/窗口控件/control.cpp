#include <Windows.h>
#include <commCtrl.h>
#include <iostream>
#include <string.h>

//输出消息到Debug控制台
void print(LPCWSTR format, ...)
{
	WCHAR buff[100];
	va_list ap;
	va_start(ap, format);
	wvsprintfW(buff, format, ap);
	va_end(ap);
	OutputDebugStringW(buff);
}


LRESULT WndProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		//添加控件
		CreateWindowW(WC_BUTTON, L"移动窗口", WS_CHILD | WS_VISIBLE, 10, 5, 120, 30, hWnd, (HMENU)0X001, NULL, NULL);
		CreateWindowW(WC_BUTTON, L"获取文本框内容", WS_CHILD | WS_VISIBLE, 10, 40, 120, 30, hWnd, (HMENU)0X002, NULL, NULL);
		CreateWindowW(WC_BUTTON, L"设置文本框内容", WS_CHILD | WS_VISIBLE, 10, 80, 120, 30, hWnd, (HMENU)0X003, NULL, NULL);
		CreateWindowW(WC_BUTTON, L"设置父窗口", WS_CHILD | WS_VISIBLE, 10, 120, 120, 30, hWnd, (HMENU)0X004, NULL, NULL);
		CreateWindow(WC_EDIT, L"请输入窗口标题", WS_CHILD | WS_VISIBLE|WS_BORDER, 10, 160, 200, 150, hWnd, (HMENU)0X005, NULL, NULL);

		//HWND textBox = GetDlgItem(hWnd, 0x005);
		//SetWindowTextW(textBox, L"myWindows");
		SetDlgItemTextW(hWnd, 0x005, L"myWindows");
		break;
	}
	case WM_CLOSE:
	{
		//MessageBoxW(hWnd, L"窗口关闭了", L"tip", MB_OK);
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		ExitProcess(0);
		break;
	}
	case WM_COMMAND:
	{
		

		WORD crlId = LOWORD(wParam);
		print(L"%03d\n", crlId);
		switch (crlId)
		{
		case 0x001:
		{
			

			RECT rect{ 0 };
			GetClientRect(hWnd, &rect);
			
			print(L"rect.right = %d\n", rect.right);
			//print(L"rand%right=%d\n", rand() % rect.right);
			int x = rand() % (rect.right - 120);
			
			int y = rand() % (rect.bottom - 30);
			
			print(L"x = %d, y = %d\n ", x, y);
			MoveWindow((HWND)lParam, x, y, 120, 30, true);
			break;
		}
		case 0x002:
		{
			HWND textBox = GetDlgItem(hWnd, 0x005);
			WCHAR buff[100];
			GetWindowTextW(textBox, buff, 100);
			MessageBoxW(hWnd, buff, L"消息", 0);

			break;
		}
		case 0x003:
		{
			//HWND textBox = GetDlgItem(hWnd, 0x005);
			//SetWindowTextW(textBox, L"hello");
			SetDlgItemTextW(hWnd, 0x005, L"hello");
			break;
		}
		case 0x004:
		{
			WCHAR buff[100];
			GetDlgItemTextW(hWnd, 0x005, buff, 100);
			HWND hwnd = FindWindowW(NULL, buff);
			SetParent((HWND)lParam, hwnd);
			break;
		}
		case 0x005:
		{
			break;
		}
		default:
			break;
		}

	}
	default:
		break;
	}
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd) 
{
	srand(time(NULL));
	HWND hWnd;
	MSG msg;
	LPCWSTR wndClassName = L"mywndclass";

	//1.创建窗口类
	WNDCLASS wndClass = { 0 };
	wndClass.lpszClassName = wndClassName;
	wndClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	wndClass.lpfnWndProc = WndProc;

	//2.注册窗口类
	RegisterClassW(&wndClass);

	//3.创建窗口
	hWnd = CreateWindowW(
		wndClassName, 
		L"myWindows", 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, 
		0, 
		CW_USEDEFAULT, 
		0, 
		NULL, 
		NULL, 
		hInstance, 
		0);

	//4.显示窗口
	if (hWnd) {
		ShowWindow(hWnd, SW_SHOWNORMAL);
	}
	else 
	{
		MessageBoxW(NULL, L"创建窗口失败", L"error", MB_OK);
	}
	
	//5.获取消息分发消息
	while (GetMessageW(&msg, hWnd, 0, 0)) {
		TranslateMessage(&msg);//将键盘消息发给文本框, 如果注释掉文本框将不可编辑
		DispatchMessageW(&msg);
	}

	return 0;
}