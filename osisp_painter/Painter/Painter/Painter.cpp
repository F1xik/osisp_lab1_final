#include "Painter.h"
#include <windows.h>


#include<string.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
char szProgName[] = "��� ���������"; 
bool isPainting = false;
POINT pt;
HWND hWndEditText;
int rValue, gValue, bValue;
HDC hdc;
HWND hWnd, hWndButton, hWndLine;
RECT r;
HPEN;


void PaintBrushLine(){
	hdc = GetDC(hWnd);
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);
	char brushWidth[3];
	GetWindowText(hWndEditText, brushWidth, 3);
	//MessageBox(NULL, (LPCSTR)brushWidth, (LPCSTR)L"Ok!", MB_OK);
	r.left = pt.x;
	r.top = pt.y;
	r.right = pt.x + atoi(brushWidth);
	r.bottom = pt.y + atoi(brushWidth);
	FillRect(hdc, &r, HBRUSH(CreateSolidBrush(RGB(rValue, gValue, bValue))));
	//ValidateRect(hWnd, NULL);
	ReleaseDC(hWnd, hdc);

}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	//HWND hWnd;
	//HWND hWndButton;
	//HWND hWndLine;
	MSG lpMsg;
	WNDCLASS w;

	w.lpszClassName = szProgName; //��� ��������� - ��������� ����
	w.hInstance = hInstance; //������������� �������� ����������
	w.lpfnWndProc = WndProc; //��������� �� ������� ����
	w.hCursor = LoadCursor(NULL, IDC_ARROW); //��������� ������
	w.hIcon = 0;
	w.lpszMenuName = 0;
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //���� ���� ����
	w.style = CS_HREDRAW | CS_VREDRAW;
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;

	//���� �� ������� ���������������� ����� ���� - �������
	if (!RegisterClass(&w))
		return 0;

	//�������� ���� � ������, �������� ��������� CreateWindow
	hWnd = CreateWindow(szProgName, //��� ���������
		"���������� ����������� Win32 API", //��������� ����
		WS_OVERLAPPEDWINDOW, //����� ���� - ���������������
		0, //��������� ���� �� ������ �� �
		0, //��������� �� �
		800, //������
		600, //������
		(HWND)NULL, //������������� ������������� ����
		(HMENU)NULL, //������������� ����
		(HINSTANCE)hInstance, //������������� ���������� ���������
		(HINSTANCE)NULL); //���������� �������������� ����������

	
	hWndButton = CreateWindow("button", "brush", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		10, 10, 80, 30, hWnd, (HMENU)10001, hInstance, NULL);
	//������� ���� �� ������ �� �����
	hWndEditText = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "", WS_CHILD | WS_VISIBLE | ES_RIGHT,
		70, 10, 100, 30, hWnd, (HMENU)10002, hInstance, NULL);
	hWndLine = CreateWindow("button", "line", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		170, 10, 180, 30, hWnd, (HMENU)10003, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	//������� ���������� ����
	

	//���� ��������� ���������

	while (GetMessage(&lpMsg, NULL, 0, 0)) {
		TranslateMessage(&lpMsg);
		DispatchMessage(&lpMsg);
	}
	return(lpMsg.wParam);
}

//������� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT messg,
	WPARAM wParam, LPARAM lParam)
{
	//HDC hdc; //������ �������� ����������
	PAINTSTRUCT ps; //������ ��������� ��������� ������������ ������
	LOGFONT lf;
	HFONT hFont;
	//RECT r;
	HBRUSH hBrush;
	HPEN hPen;
	bool brushChosen = false;

	//���� ��������� ���������
	switch (messg)
	{
		
	

		//��������� ������ - ���������� ����
	case WM_DESTROY:
		PostQuitMessage(0); //�������� ��������� ������ � ����� 0 - ���������� ����������
		//DeleteObject(hPen);
		//DeleteObject(hBrush);
		break;

	case WM_LBUTTONDOWN:		
		if (IsDlgButtonChecked(hWnd, 10001)){			
			isPainting = true;
		}
		else
		if (IsDlgButtonChecked(hWnd, 10003)){
			hdc = GetDC(hWnd);
			GetCursorPos(&pt);
			ScreenToClient(hWnd, &pt);
			MoveToEx(hdc, pt.x, pt.y, NULL);
		}
		
		break;
	case WM_LBUTTONUP:
		isPainting = false;
		if (IsDlgButtonChecked(hWnd, 10003)){
			//MessageBox(NULL, (LPCSTR)L"lol", (LPCSTR)L"Ok!", MB_OK);
			
			GetCursorPos(&pt);
			ScreenToClient(hWnd, &pt);
			char brushWidth[3];
			GetWindowText(hWndEditText, brushWidth, 3);
			hPen = CreatePen(PS_SOLID, atoi(brushWidth), RGB(rValue, gValue, bValue));
			SelectObject(hdc, hPen);
			LineTo(hdc, pt.x, pt.y);
			DeleteObject(hPen);
			GetStockObject(DC_PEN);
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_MOUSEMOVE:
		if (isPainting){			
			PaintBrushLine();
		}
		break;
	case WM_COMMAND:
		
		if (10001 == LOWORD(wParam) | 10003 == LOWORD(wParam)){
			if (IsDlgButtonChecked(hWnd, 10001) | IsDlgButtonChecked(hWnd,10003)){
				CHOOSECOLOR ch_color = { 0 };
				ch_color.lStructSize = sizeof(CHOOSECOLOR);
				ch_color.hwndOwner = hWnd;
				COLORREF cust_colors[16] = { 0 };
				ch_color.lpCustColors = cust_colors;
				if (ChooseColor(&ch_color))
				{
					rValue = GetRValue(ch_color.rgbResult);
					gValue = GetGValue(ch_color.rgbResult);
					bValue = GetBValue(ch_color.rgbResult);
				}
			}
		}
		/*if (10001 == LOWORD(wParam) && IsDlgButtonChecked(hWnd, 10003))
			CheckDlgButton(hWndButton, 10001, BST_UNCHECKED);
		else
		if (10003 == LOWORD(wParam) && IsDlgButtonChecked(hWnd, 10001))
			CheckDlgButton(hWndLine, 10003, BST_UNCHECKED);*/
		break;

	default:
		return(DefWindowProc(hWnd, messg, wParam, lParam)); //����������� ������� ���������� �� �������������
	}
	return 0;
}


Painter::Painter()
{
}


Painter::~Painter()
{
}
