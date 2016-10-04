#include "myControls.h"

extern Drawing *RYS;

#pragma region InitMyControls - inicjalizacja kontrolek
	void InitMyControls(HINSTANCE hInstance){
		WNDCLASS wnd;

		wnd.cbClsExtra		= NULL;
		wnd.hbrBackground	= NULL;
		wnd.hInstance		= hInstance;
		wnd.lpszMenuName	= NULL;
		wnd.style			= CS_VREDRAW|CS_HREDRAW;
		wnd.cbWndExtra		= sizeof(NumericWndData);
		wnd.hCursor			= LoadCursor(NULL,IDC_HAND);
		wnd.hIcon			= LoadIcon(NULL,IDI_APPLICATION);
		wnd.lpfnWndProc		= NumericProc;
		wnd.lpszClassName	= "NumericWindow";

		if(!RegisterClass(&wnd)) MessageBox(NULL,"Error: NumericWindow","Error message",MB_OK);
	}
#pragma endregion

#pragma region Procedury kontrolek
	LRESULT CALLBACK NumericProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
		switch(msg){
			case WM_CREATE:
				{
					NumericWndData *numWnd = (NumericWndData*) GetWindowLongPtr(hWnd,0);
					numWnd = new NumericWndData(hWnd,RGB(228,226,213),RGB(0,0,0),0,255,0,255);
					SetWindowLong(hWnd,0,(LONG)numWnd);
				}
				break;
			case WM_ENABLE:
				{
					NumericWndData *numWnd = (NumericWndData*) GetWindowLongPtr(hWnd,0);
					numWnd->WmEnable(hWnd);
				}
				break;
			case WM_TIMER:
				{
					NumericWndData *numWnd = (NumericWndData*) GetWindowLongPtr(hWnd, 0);
					numWnd->WmTimer(hWnd);
				}
				break;
			case WM_KEYDOWN:
				{
					NumericWndData *numWnd = (NumericWndData*) GetWindowLongPtr(hWnd, 0);
					if(numWnd->WmKeyDown(hWnd,wParam)){
						InvalidateRect(hWnd,NULL,false);
					}
				}
				break;
			case WM_SIZE:
				{
					NumericWndData *numWnd = (NumericWndData*) GetWindowLongPtr(hWnd,0);
						numWnd->SetWndSize(hWnd);
				}
				break;
			case WM_MOUSEMOVE:
				{
					NumericWndData *numWnd = (NumericWndData*) GetWindowLongPtr(hWnd, 0);
					numWnd->WmMouseMove(hWnd);
				}
				break;
			case WM_LBUTTONDOWN:
				{
					NumericWndData *numWnd = (NumericWndData*) GetWindowLongPtr(hWnd, 0);
					numWnd->WmLButtonDown(hWnd);
				}
				break;
			case WM_LBUTTONUP:
				{
					NumericWndData *numWnd = (NumericWndData*) GetWindowLongPtr(hWnd, 0);
					numWnd->WmLButtonUp(hWnd);
				}
				break;
			case WM_PAINT:
				{
					PAINTSTRUCT ps;
					HDC hdc = BeginPaint(hWnd,&ps);
					NumericWndData *numWnd = (NumericWndData*) GetWindowLongPtr(hWnd, 0);
					if(numWnd)
						numWnd->WmPaint(hdc,RYS);
					EndPaint(hWnd,&ps);
				}
				break;
			case WM_DESTROY:
				{
					NumericWndData *numWnd = (NumericWndData*) GetWindowLongPtr(hWnd, 0);
					if(numWnd)
						delete numWnd;
				}
				break;
			case GET_VALUE:
				{
					NumericWndData *numWnd = (NumericWndData*) GetWindowLongPtr(hWnd, 0);
					if(numWnd)
						return (LRESULT)numWnd->Value();
					return 0;
				}
		}
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
#pragma endregion