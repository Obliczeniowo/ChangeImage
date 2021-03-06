/* Program made by Krzysztof Zajączkowski, contact ProgrammingMalyszKZ@gmail.com, Licence: GPL*/

#define WINVER 0x0500
#define _WIN32_WINNT 0x0500   // for Mouse Wheel support

#include <windows.h>
#include <io.h>

#include "Drawing.h"

#include "browser.h"

Drawing *RYS			= new Drawing;

LPSTR BelkaTyt			= "Gradient from image";
LPSTR NazwaKlasy		= "Gradient from image";
LPSTR ImageWindowName	= "Image Window";

HWND browserWnd;

#include "myDialogBox.h"

std::vector<std::string> tPath;

HBRUSH	brGray150	= CreateSolidBrush(RGB(150,150,150));
HBRUSH	brGray230	= CreateSolidBrush(RGB(230, 230, 230));
HBRUSH	brBlue		= CreateSolidBrush(RGB(0,150,255));
HBRUSH	brRed		= CreateSolidBrush(RGB(255,0,0));
HBRUSH	brBlue2		= CreateSolidBrush(RGB(150,200,255));
HBRUSH	brWhite		= CreateSolidBrush(RGB(255,255,255));

HPEN	pnGray200	= CreatePen(PS_SOLID, 2, RGB(200,200,200));
HPEN	pnRed		= CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

HFONT textFont;

void DeleteGraphicsObjects(){
	DeleteObject(brGray150);
	DeleteObject(brGray230);
	DeleteObject(brBlue);
	DeleteObject(brRed);
	DeleteObject(brWhite);

	DeleteObject(pnGray200);
	DeleteObject(pnRed);
	DeleteObject(textFont);
}

HMENU FindImage(std::string str, int &id){
	HMENU hMenu = CreateMenu();
	_finddata64i32_t fd;
	intptr_t hFile;
	std::string copy;
	copy = str;
	str += "*.png";
	hFile = _findfirst(str.c_str(),&fd);
	if(hFile == -1L){
	}else{
		do
		{
			std::string copy2;
			copy2 = copy + fd.name;
			AppendMenu(hMenu,MF_STRING, id, fd.name);
			id++;
			tPath.push_back(copy2);
		}while(_findnext(hFile,&fd) == 0);
		_findclose( hFile );
	}
	return hMenu;
}

HMENU Folders(std::string path,int &id)
{
	HMENU hp = FindImage(path, id);
	_finddata64i32_t fd;
	intptr_t hFile;
	std::string copy = path + "*";
	hFile = _findfirst(copy.c_str(),&fd);
	if(hFile == -1L){
	}else{
		do{
			if(fd.attrib & _A_SUBDIR){
				//MessageBox(NULL,fd.name,"info",MB_OK);
				if(strcmp(fd.name,".") && strcmp(fd.name,"..")){
					AppendMenu(hp,MF_POPUP,(UINT_PTR)FindImage(path + fd.name + "\\", id),fd.name);
					//Folders(path + fd.name + "\\");
				}
			}
		}while(_findnext(hFile,&fd) == 0);
		_findclose(hFile);
	}
	//AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hp, "Standardowe pliki (standard file)");
	return hp;
}

//Zmienne globalne
HWND okno;
HWND mdiClient;
HINSTANCE hInst;

#define CL_SETFRAMECOLOR	5000
#define CL_SETBKCOLOR       5001
#define CL_SETPATHCOLOR		5003
#define SET_IMAGE			5002
#define SET_SHOWPARENT		5004

#define CL_SETTEXTCOLOR		5005
#define SET_MODALONOFF		5006
#define SET_ROPMODE			5007

LRESULT CALLBACK StartupWindowProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
	static RECT WndSize;
	static char startupPath[MAX_PATH];
	static COLORREF FrameColor=RGB(0,0,0);
	static COLORREF BkColor=RGB(255,255,225);
	static COLORREF PathColor=RGB(255,0,0);
	static COLORREF TextColor = RGB(255,255,255);
	static int RopMode = 0;
	static HBITMAP hBmp;
	static HBITMAP bgBmp;
	static BOOL showParent=FALSE;
	static int swStyle=SW_SHOWMAXIMIZED;
	static LPSTR WindowText=NULL;
	static RECT email = {13,55,255,72};
	static RECT licence = {13,69,87,86};
	static RECT source = {17, 270,215,287};
	static RECT gimpuj = {17,150,82,167};
	static RECT homePage = {13,85, 130, 103};
	switch(msg){
		case WM_CREATE:
			{
				EnableWindow(GetParent(hWnd),FALSE);
				NONCLIENTMETRICS ncm;
				ncm.cbSize=sizeof(ncm);
				SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
				textFont=CreateFontIndirect(&ncm.lfStatusFont);
				LPSTR path=GetCommandLine();
				if(path[0]=='"')
					path++;
				char *p=strrchr(path,'"');
				if(p)
					*p=NULL;
				strcpy(startupPath,path);
				SetWindowPos(hWnd,NULL,(GetSystemMetrics(SM_CXSCREEN)-640)/2,(GetSystemMetrics(SM_CYSCREEN)-400)/2,640,400,SWP_NOZORDER);
				WindowText=new char[GetWindowTextLength(hWnd)+1];
				GetWindowText(hWnd,WindowText,GetWindowTextLength(hWnd)+1);
				HDC hdc = GetDC(NULL);
				RopMode = GetROP2(hdc);
				DeleteDC(hdc);
			}
			break;
		case WM_SIZE:
			{
				GetClientRect(hWnd,&WndSize);
				HDC hdc = GetDC(hWnd);
				
				DeleteObject(bgBmp);
				bgBmp = CreateCompatibleBitmap(hdc, WndSize.right - WndSize.left, WndSize.bottom - WndSize.top);

				DeleteDC(hdc);
			}
			break;
		case WM_DESTROY:
			{
				DeleteObject(hBmp);
				EnableWindow(GetParent(hWnd),TRUE);
				if(showParent && GetParent(hWnd)){
					ShowWindow(GetParent(hWnd),swStyle);
					UpdateWindow(GetParent(hWnd));
				}
				if(WindowText)
					delete[] WindowText;
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			{
				POINT mp;
				GetCursorPos(&mp);
				ScreenToClient(hWnd, &mp);
				if(PtInRect(&email, mp)){
					ShellExecute(NULL,"open","mailto:ProgrammingMalyszKZ@gmail.com?subject=Pogram Create Gradient from bitmap",NULL,NULL,SW_SHOWNORMAL);
				}else if(PtInRect(&licence, mp)){
					ShellExecute(NULL,"open","http://pl.wikipedia.org/wiki/GNU_General_Public_License",NULL,NULL,SW_SHOWNORMAL);
					ShellExecute(NULL,"open","http://en.wikipedia.org/wiki/GNU_General_Public_License",NULL,NULL,SW_SHOWNORMAL);
				}else if(PtInRect(&source, mp)){
					ShellExecute(NULL,"open","http://registry.gimp.org/node/25571",NULL,NULL,SW_SHOWNORMAL);
				}else if(PtInRect(&homePage, mp)){
					ShellExecute(NULL,"open","http://obliczeniowo.com.pl",NULL, NULL,SW_SHOWNORMAL);
				}else{
					DeleteObject(hBmp);
					DeleteObject(bgBmp);
					EnableWindow(GetParent(hWnd),TRUE);
					if(showParent && GetParent(hWnd)){
						ShowWindow(GetParent(hWnd),swStyle);
						UpdateWindow(GetParent(hWnd));
					}
					DestroyWindow(hWnd);
				}
			}
			break;
		case WM_KEYDOWN:
			DeleteObject(hBmp);
			DeleteObject(bgBmp);
			EnableWindow(GetParent(hWnd),TRUE);
			if(showParent && GetParent(hWnd)){
				ShowWindow(GetParent(hWnd),swStyle);
				UpdateWindow(GetParent(hWnd));
			}
			DestroyWindow(hWnd);
			break;
		case WM_MOUSEMOVE:
			{
				InvalidateRect(hWnd, NULL, false);
			}
			break;
		case WM_PAINT:
			{
				POINT mp;
				GetCursorPos(&mp);
				ScreenToClient(hWnd, &mp);
				PAINTSTRUCT ps;
				HDC hdc=BeginPaint(hWnd,&ps);
				HDC mem2 = CreateCompatibleDC(hdc);
				SetBkMode(mem2,TRANSPARENT);
				SelectObject(mem2,textFont);
				SetTextColor(mem2,RGB(255,255,255));
				SelectObject(mem2, bgBmp);
				if(hBmp){
					HDC mem=CreateCompatibleDC(hdc);
					SelectObject(mem,hBmp);
					BITMAP bmp;
					GetObject(hBmp,sizeof(bmp),&bmp);
					SetStretchBltMode(mem2, COLORONCOLOR);
					StretchBlt(mem2,0,0,WndSize.right,WndSize.bottom,mem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
					if(PtInRect(&email,mp))
						RYS->fillRectangle(mem2,email,RGB(185,100,0));
					else if(PtInRect(&licence, mp)){
						RYS->fillRectangle(mem2,licence, RGB(185, 100, 0));
					}else if(PtInRect(&source, mp)){
						RYS->fillRectangle(mem2,source, RGB(185, 100, 0));
					}else if(PtInRect(&homePage, mp)){
						RYS->fillRectangle(mem2,homePage, RGB(185, 100, 0));
					}

					DeleteDC(mem);
				}else{
					RYS->fillRectangle(mem2,WndSize,BkColor);
				}
				SetROP2(mem2,RopMode);
				char text[]="Autor programu: Krzysztof Zajączkowski";
				TextOut(mem2,15,10,text,(int)strlen(text));
				RECT r;
				SetRect(&r,15,25,WndSize.right-15,WndSize.bottom-40);
				if(WindowText){
					DrawText(mem2,WindowText,-1,&r,DT_LEFT|DT_WORDBREAK);
				}
				SetRect(&r,15,WndSize.bottom-25,WndSize.right-15,WndSize.bottom);
				SetTextColor(mem2,PathColor);
				DrawText(mem2,startupPath,-1,&r,DT_LEFT|DT_END_ELLIPSIS);
				SetRect(&r, 18, 120, 250,400);
				SetTextColor(mem2, RGB(255, 202, 121));
				//SetTextColor(mem2, RGB(134, 101, 60));
				char c[] = "Program made by\n\nKrzysztof Zajaczkowski\n\ncan by used for anybody\n\n\nSource code can be\ndownloaded from here:\n\nhttp://registry.gimp.org/node/25571";
				DrawText(mem2, c, -1, &r, DT_LEFT|DT_END_ELLIPSIS);
				BitBlt(hdc,0,0,WndSize.right - WndSize.left, WndSize.bottom - WndSize.top, mem2, 0, 0, SRCCOPY);
				DeleteDC(mem2);
				EndPaint(hWnd,&ps);
			}
		case CL_SETFRAMECOLOR:
			if(wParam)
				FrameColor=*((COLORREF*)wParam);
			break;
		case CL_SETBKCOLOR:
			if(wParam)
				BkColor=*((COLORREF*)wParam);
			break;
		case CL_SETPATHCOLOR:
			if(wParam)
				PathColor=*((COLORREF*)wParam);
			break;
		case CL_SETTEXTCOLOR:
			if(wParam)
				TextColor=*((COLORREF*)wParam);
			break;
		case SET_IMAGE:
			if(wParam)
				hBmp=*((HBITMAP*)wParam);
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		case SET_SHOWPARENT:
			showParent=(wParam?TRUE:FALSE);
			swStyle=(int)lParam;
			break;
		case SET_ROPMODE:
			RopMode = (int)wParam;
			InvalidateRect(hWnd,NULL,TRUE);
			break;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

void CALLBACK InitWindow(HINSTANCE hInstance){

	WNDCLASS wnd;

	wnd.style=CS_VREDRAW|CS_HREDRAW;
	wnd.hInstance=hInstance;
	wnd.lpszClassName="startup_window";
	wnd.lpfnWndProc=StartupWindowProc;
	wnd.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wnd.hCursor=LoadCursor(NULL, IDC_ARROW);
	wnd.lpszMenuName=NULL;
	wnd.cbClsExtra=0;
	wnd.cbWndExtra=0;
	wnd.hbrBackground=CreateSolidBrush(RGB(255,255,225));

	RegisterClass(&wnd);
}

HWND CALLBACK CreateStartupWindow(HWND hWndParent,HINSTANCE hInstance,LPSTR WindowText){
	return CreateWindow("startup_window",WindowText,WS_VISIBLE|WS_POPUP,0,0,100,100,hWndParent,NULL,hInstance,NULL);
}

void StartupFile(std::string &StartupFile,std::string startupFileName){
	char *path	= GetCommandLine();
	char *p		= strrchr(path, '\\');
	*p ++;
	char temp = *p;
	*p = NULL;
	if(path[0] == 34)
		path++;
	StartupFile = p;
	StartupFile += startupFileName;
	*p = temp;
}

void StartupPath(std::string &StartupPath){
	char *path	= GetCommandLine();
	char *p		= strrchr(path,'\\');
	p ++;
	char temp = *p;
	*p			= NULL;
	if(path[0]==34)
		path++;
	StartupPath = path;
	*p = temp;
}

void GetFileFromCmdLine(LPSTR CmdLine,std::vector<std::string> &fileTable){
	char *end;
	char *begin = strlen(CmdLine) ? CmdLine + 1 : NULL;
	while(begin){
		if(*(begin - 1) == '"'){
			end = strchr(begin,'"');
			if(end){
				*end = NULL;
				std::string s = begin;
				fileTable.push_back(s);
				*end = '"';
				if(strlen(end) > 1){
					begin = end + 3;
				}else{
					begin = NULL;
				}
			}else{
				begin = NULL;
			}
		}else{
			end = strchr(begin, ' ');
			if(end){
				*end = NULL;
				std::string s = begin - 1;
				fileTable.push_back(s);
				*end = ' ';
				if(strlen(end) > 1){
					begin = end + 2;
				}else{
					begin = NULL;
				}
			}else if(strlen(begin)){
				std::string s = begin - 1;
				fileTable.push_back(s);
				begin = NULL;
			}else{
				begin = NULL;
			}
		}
	}
}

void GetFileFromCmdLine(LPSTR CmdLine, std::vector<std::string> &fileTable, std::vector<std::string> &filter){
	strlwr(CmdLine);
	GetFileFromCmdLine(CmdLine,fileTable);

	if(fileTable.size()){
		for(int i = 0; i < (int) fileTable.size(); i++){
			bool w = false; // Jak false to usuń z listy
			for(std::vector<std::string>::iterator j = filter.begin(); j < filter.end(); j++){
				if(fileTable[i].size() > j->size()){
					std::string temp = fileTable[i].c_str() + fileTable[i].size() - j->size();
					if(temp.compare(*j) == 0){
						w = true;
						j = filter.end() - 1;
					}
				}
			}
			if(!w){
				fileTable.erase(fileTable.begin() + i);
				i --;
			}
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
	static RECT WndSize;
	switch(msg){
		case WM_CREATE:
			{

				DragAcceptFiles(hWnd, true);
			}
			break;
		case WM_DROPFILES:
			{
				HDROP hDrop = (HDROP)wParam;
				int n = DragQueryFile(hDrop,0xFFFFFFFF,0,0);
				char buffer[1024];
				for(int i=0;i<n;i++){
					DragQueryFile(hDrop,i,buffer,1024);
					
					std::string temp;
					temp = buffer;
					if(temp.find(".bmp")!=std::string::npos || temp.find(".jpg")!=std::string::npos ||
						temp.find(".png")!=std::string::npos || temp.find(".gif")!=std::string::npos||
						temp.find(".BMP")!=std::string::npos || temp.find(".JPG")!=std::string::npos ||
						temp.find(".PNG")!=std::string::npos || temp.find(".GIF")!=std::string::npos)
					{
						HWND hChild = CreateMDIWindow("myMidiChild", "wnd",
								MDIS_ALLCHILDSTYLES, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
								mdiClient, (HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE), 0);
						ShowWindow(hChild, SW_HIDE);
						Document *doc = new Document(hChild, buffer);
						SetWindowLong(hChild,GWL_USERDATA,(LONG)doc);
						SendMessage(doc->imWindow,WM_SETTINGCHANGE,wParam,lParam);
					}
				}
				DragFinish(hDrop);
			}
			break;
		case WM_MOUSEWHEEL:
			{
				HWND hwnd = (HWND)SendMessage(mdiClient,WM_MDIGETACTIVE,0,0);
				if(hwnd){
					SendMessage(hwnd,WM_MOUSEWHEEL,wParam,lParam);
				}
			}
			return 0; // musi zwrócić wartość rzeby nie buraczyć
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case ID_GRPRFILE:
					{
						Document::CreateGradientPngFile(hWnd);
					}
					break;
				case ID_EXPORT_SVG:
					{
						Document::ExportGradientFromSvgToGgr(hWnd);
					}
					break;
				case ID_TO_CSS:
					{
						Document::ExportGradientFromGgrToCss(hWnd);
					}
					break;
				case ID_SVGGR:
					{
						Document::CreateGradientSvgFile(hWnd);
					}
					break;
				case ID_OPEN:
					{
						HWND hChild = CreateMDIWindow("myMidiChild", "wnd",
							MDIS_ALLCHILDSTYLES, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
							mdiClient, (HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE), 0);
						ShowWindow(hChild, SW_HIDE);
						Document *doc = new Document(hChild);
						SetWindowLong(hChild,GWL_USERDATA,(LONG)doc);
						SendMessage(doc->imWindow,WM_SETTINGCHANGE,wParam,lParam);
					}
					break;
				case ID_CLOSEFILE:
					{
						HWND hwnd = (HWND)SendMessage(mdiClient,WM_MDIGETACTIVE,0,0);
						if(hwnd){
							SendMessage(mdiClient,WM_MDIDESTROY,(WPARAM)hwnd,0);
						}
					}
					break;
				case ID_CLOSEAPPLICATION:
					DestroyWindow(hWnd);
					break;
				case ID_KASKADOWO:
					SendMessage(mdiClient, WM_MDICASCADE, 0, 0);
					InvalidateRect(mdiClient,NULL,false);
					break;
				case ID_TILEH:
					SendMessage(mdiClient, WM_MDITILE, MDITILE_HORIZONTAL, 0);
					InvalidateRect(mdiClient,NULL,false);
					break;
				case ID_TILEV:
					SendMessage(mdiClient, WM_MDITILE, MDITILE_VERTICAL, 0);
					InvalidateRect(mdiClient,NULL,false);
					break;
				case ID_GRADIENT_BROWSER:
					{
						if(!IsWindow(browserWnd)){
							browserWnd=CreateWindowEx(WS_EX_DLGMODALFRAME, lpszAppName,lpszAppName,WS_OVERLAPPEDWINDOW|WS_VISIBLE,
										CW_USEDEFAULT,CW_USEDEFAULT,640,480,hWnd,0,hInst,0);
						}
					}
					break;
				default:
					{
						int k = LOWORD(wParam) - 6000;
						if(k < (int)tPath.size() && k > -1){
							HWND hChild = CreateMDIWindow("myMidiChild", "wnd",
								MDIS_ALLCHILDSTYLES, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
								mdiClient, (HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE), 0);
							ShowWindow(hChild, SW_HIDE);
							Document *doc = new Document(hChild,tPath[LOWORD(wParam) - 6000].c_str());
							SetWindowLong(hChild,GWL_USERDATA,(LONG)doc);
							SendMessage(doc->imWindow,WM_SETTINGCHANGE,wParam,lParam);
						}
					}
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefFrameProc(hWnd,mdiClient,msg,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE ,LPSTR cmdLine,int styl){

	setlocale(LC_CTYPE, "");
	InitMyControls(hInstance);

	InitViewImageClass(hInstance);

	hInst=hInstance;

	// Register toolbar control classes from the DLL for the common control.
	INITCOMMONCONTROLSEX iccex; // The INITCOMMONCONTROLSEX structure
	iccex.dwSize = sizeof (INITCOMMONCONTROLSEX);
	iccex.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&iccex);
	//IntCommonControls
	
	WNDCLASS wnd;

	wnd.cbClsExtra		= NULL;
	wnd.cbWndExtra		= NULL;
	wnd.hbrBackground	= NULL;
	wnd.hCursor			= LoadCursor(NULL,IDC_ARROW);
	wnd.hIcon			= LoadIcon(hInstance,MAKEINTRESOURCE(IDI_CHANGEIMAGE));
	wnd.hInstance		= hInstance;
	wnd.lpfnWndProc		= WndProc;
	wnd.lpszClassName	= NazwaKlasy;
	wnd.lpszMenuName	= MAKEINTRESOURCE(IDR_MDICHILDS);
	wnd.style			= CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS;

	if(!RegisterClass(&wnd)) return 0;

	wnd.style=CS_HREDRAW|CS_VREDRAW;
	wnd.lpfnWndProc=BrowserProc;
	wnd.cbClsExtra=0;
	wnd.cbWndExtra=0;
	wnd.hInstance=hInstance;
	wnd.hIcon=0;
	wnd.hCursor=LoadCursor(0, IDC_ARROW);
	wnd.hbrBackground=0;
	wnd.lpszMenuName=0;//MAKEINTRESOURCE(MENU_1);
	wnd.lpszClassName=lpszAppName;

	if(!RegisterClass(&wnd))
		return 0;

	wnd.hIcon			= LoadIcon(NULL,IDI_APPLICATION);
	wnd.hCursor			= LoadCursor(NULL,IDC_ARROW);
	wnd.lpfnWndProc		= GradientBrowserProc;
	wnd.lpszMenuName	= NULL;
	wnd.lpszClassName	= GRADIENT_BROWSER_LIST;
	wnd.style=CS_HREDRAW|CS_VREDRAW;

	if(!RegisterClass(&wnd)) return 0;

	wnd.hIcon			= LoadIcon(NULL,IDI_APPLICATION);
	wnd.hCursor			= LoadCursor(NULL,IDC_ARROW);
	wnd.lpfnWndProc		= ImageWindowProc;
	wnd.lpszMenuName	= NULL;
	wnd.lpszClassName	= ImageWindowName;

	if(!RegisterClass(&wnd)) return 0;

	wnd.hIcon			= LoadIcon(hInstance,MAKEINTRESOURCE(IDI_CHANGEIMAGE));
	wnd.hbrBackground	= NULL;
	wnd.hCursor			= LoadCursor(NULL,IDC_ARROW);
	wnd.lpfnWndProc		= mdiChildProc;
	wnd.lpszClassName	= "myMidiChild";

	if(!RegisterClass(&wnd)) return 0;

	wnd.hIcon			= LoadIcon(hInstance,MAKEINTRESOURCE(IDI_CHANGEIMAGE));
	wnd.hbrBackground	= NULL;
	wnd.hCursor			= LoadCursor(NULL,IDC_ARROW);
	wnd.lpfnWndProc		= WindowGrPoints;
	wnd.lpszClassName	= "WindowGrPoints";

	if(!RegisterClass(&wnd)) return 0;

	wnd.hIcon			= LoadIcon(hInstance,MAKEINTRESOURCE(IDI_CHANGEIMAGE));
	wnd.hbrBackground	= NULL;
	wnd.hCursor			= LoadCursor(NULL,IDC_ARROW);
	wnd.lpfnWndProc		= WndScProc;
	wnd.lpszClassName	= "setColorWindow";

	if(!RegisterClass(&wnd)) return 0;

	HMENU menu			= LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MDICHILDS));
	CLIENTCREATESTRUCT ccs;
	ccs.hWindowMenu		= GetSubMenu(menu, 1);
	ccs.idFirstChild=1000;

	okno=CreateWindow(NazwaKlasy, BelkaTyt, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, menu, hInstance, NULL);

	mdiClient  = CreateWindow("MDICLIENT","",WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL,0, 0, 100, 100, okno, 0, hInstance, (LPVOID)&ccs);

	InitWindow(hInstance);

	HWND window = CreateStartupWindow(okno,hInstance,"Wersja: 1.0.1.2\nData utworzenia: 19.05.2012\nKontakt: ProgrammingMalyszKZ@gmail.com\nLicencja: GPL\nMoja strona domowa");

	HBITMAP bmp = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_STARTUP_BITMAP));

	std::vector<std::string> fileTable;
	std::vector<std::string> filter;

	LPSTR sp;

	StartupPath(sp);
	
	std::string startuppath = sp;
	startuppath += "\\Standard graphics file\\";

	int id = 6000;

	InsertMenu(GetSubMenu(menu,0), 2, MF_BYPOSITION| MF_POPUP,(UINT_PTR)Folders(startuppath,id),"Standardowe pliki (standards file)");

	filter.push_back((std::string) ".bmp");
	filter.push_back((std::string) ".jpg");
	filter.push_back((std::string) ".png");
	filter.push_back((std::string) ".gif");

	GetFileFromCmdLine(cmdLine,fileTable,filter);

	for(std::vector<std::string>::iterator i = fileTable.begin(); i < fileTable.end(); i++){
		HWND hChild = CreateMDIWindow("myMidiChild", "wnd",
			MDIS_ALLCHILDSTYLES, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			mdiClient, hInstance, 0);
		ShowWindow(hChild, SW_NORMAL);
		Document *doc = new Document(hChild, i->c_str());
		SetWindowLong(hChild,GWL_USERDATA,(LONG)doc);
		SendMessage(doc->imWindow,WM_SETTINGCHANGE,0/*wParam*/,0/*lParam*/);
	}

	SendMessage(window,SET_IMAGE,(WPARAM)&bmp,0);
	SendMessage(window,SET_SHOWPARENT,(WPARAM)1,(LPARAM)SW_SHOWMAXIMIZED);
	COLORREF c = RGB(255,255,255);
	SendMessage(window,CL_SETFRAMECOLOR,(WPARAM)&c,NULL);
	SendMessage(window,CL_SETPATHCOLOR,(WPARAM)&c,NULL);
	SendMessage(window,CL_SETTEXTCOLOR,(WPARAM)&c,NULL);

	MSG msg;

	while(GetMessage(&msg,NULL,NULL,NULL)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DeleteObject(bmp);
	

	DeleteGraphicsObjects();

	return 0;
}