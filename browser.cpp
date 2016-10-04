#include "browser.h"

#define ID_WL				102
#define ID_OPEN				101

#define ID_LIST				124
#define ID_EDIT				125

#define ID_TB_PAGEUP		5000
#define ID_TB_BACK			5001
#define ID_TB_REFRESH		5002
#define ID_TB_CUT			5003
#define ID_TB_COPY			5004
#define ID_TB_PASTE			5005
#define ID_TB_TOSVG			5006
#define ID_TB_TOCSS			5007
#define ID_TB_TOPNG			5008

#define PS					1024
#define WIDTH				300
#define PATH_EDIT_HEIGHT	20

#define	ID_COPY				1
#define ID_CUT				2

HMENU hMenu;
extern HINSTANCE	hInst;
extern HFONT		textFont;
extern HPEN			pnGray200;
extern HBRUSH		brBlue2;
extern HBRUSH		brWhite;

HICON hIcon = NULL;

SCROLLINFO si;
RECT size;

HWND tree;
HWND toolbar;
HWND grlist;
HWND pathEdit;
bool serchpath = true;

HIMAGELIST	imTree;
HIMAGELIST	imList;
int			licznik_el;
HTREEITEM	mc;

class fileDirAndName{
public:
	std::string path;
	std::string name;

	fileDirAndName(std::string path, std::string name);

	std::string GetPathAndName();
	std::string GetNameForCopy();
};

std::string fileDirAndName::GetNameForCopy(){
	std::string n = name;
	n.erase(n.begin() + n.size() - 4,n.end());
	if(n.size() > 250)
		n.erase(n.begin() + 247,n.end());
	n += " (c).ggr";
	return n;
}

fileDirAndName::fileDirAndName(std::string path,std::string name){
	std::basic_string <char>::size_type i = path.find_last_of('\\', -1);

	if(i != std::basic_string<char>::npos){
		path.erase(path.begin() + i + 1, path.end());
	}

	this->path = path;
	this->name = name;
}

std::string fileDirAndName::GetPathAndName(){
	std::string fp = path;
	fp += name;
	return fp;
}

std::vector<fileDirAndName> cFiles;
int id_cfiles = 0;

class back{
public:
	back *prev;
	char *path;
	back(char* Path,back *Prev);
	~back();
};

back::~back(){
   if(path) delete path;
}

back::back(char *Path,back *Prev){
	path=new char[strlen(Path)+1];
	strcpy(path,Path);
	prev=Prev;
}

void SetEdit(bool sp, LPCSTR path){
	serchpath = sp;
	SetWindowText(pathEdit, path);
	serchpath = true;
}

back* lastPath=0;

void SavePath(char* p){
	back* n=new back(p,lastPath);
	lastPath=n;
	SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)5000,(LPARAM)MAKELONG(1,0));
	SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)5001,(LPARAM)MAKELONG(1,0));
}

class FolderData{
public:
	char* p;
	FolderData(_finddata_t* f);
	FolderData(char* _p);
	~FolderData();
};

FolderData::FolderData(_finddata_t* f){
	p=new char[strlen(f->name)+1];
	strcpy(p, f->name);
	licznik_el++;
}
FolderData::FolderData(char* _p){
	p=new char[strlen(_p)+1];
	strcpy(p, _p);
	licznik_el++;
}
FolderData::~FolderData(){
	delete p;
	licznik_el--;
}

int CheckSubFolders(char *path){
	char buffer[PS];
	wsprintf(buffer,"%s*.*",path);
	_finddata_t f;
	intptr_t r;
	if((r=_findfirst(buffer,&f))<=0)
		;
	else{
		if(f.attrib & 16/*sprawdzanie, czy dany obiekt jest folderem*/ && strcmp(f.name,".") && strcmp(f.name,"..")/*eliminacja znaków: folderu bie¿¹cego (.) i nadrzêdnego (..)*/)
			return 1;
		while(!_findnext(r,&f)){
			if(f.attrib & 16/*sprawdzanie, czy dany obiekt jest folderem*/ && strcmp(f.name,".") && strcmp(f.name,"..")/*eliminacja znaków: folderu bie¿¹cego (.) i nadrzêdnego (..)*/)
				return 1;
		}
	}
	return 0;
}

#define DY 80

GrItem::GrItem(std::string grpath, std::string file_name, std::string insert_name, cdib pr):pr(pr){
	this->insert_name = insert_name;
	this->file_name=file_name;
	path = grpath;
	selected = false;
}

void GrItem::GrDraw(HDC hdc,int x,int &dy, RECT &size){
	if(selected){
		SelectObject(hdc, brBlue2);
	}else{
		SelectObject(hdc, brWhite);
	}
	RoundRect(hdc,x,dy + 1,size.right - 2,DY + dy,5,5);

	std::string temp = "Nazwa pliku (file name): ";
	temp+= file_name;
	dy += 5;
	float k = 32;
	DrawIconEx(hdc, x + 5,dy + 7,hIcon,k,71 * k / 96,0,0,DI_NORMAL);
	TextOut(hdc,x + 40 + 5, dy, temp.c_str(),temp.size());
	dy += 20;
	temp = "Wewnêtrzna nazwa (insert name): ";
	temp += insert_name;
	
	TextOut(hdc,x + 40 + 5, dy, temp.c_str(),temp.size());
	dy += 20;
	pr.StretchBltB(hdc,x + 4,dy,size.right - 2 - 8,HEIGHT);
	dy += HEIGHT + 5;
}

void GrItem::operator = (const GrItem &gi){
	pr = gi.pr;
	path = gi.path;
	selected = gi.selected;
	insert_name = gi.insert_name;
	file_name = gi.file_name;	
}

std::vector<GrItem> GrItemFile;
std::vector<UINT> GrItemSelIndex;

void UnselectAll(){
	if(GrItemSelIndex.size()){
		for(std::vector<UINT>::iterator i = GrItemSelIndex.begin(); i < GrItemSelIndex.end(); i++){
			GrItemFile[*i].selected = false;
		}
		GrItemSelIndex.clear();
	}
}

void Select(UINT index){
	if(index < GrItemFile.size()){
		UnselectAll();
		GrItemFile[index].selected = true;
		GrItemSelIndex.push_back(index);
	}
}

void ColectFileDir(int id){
	id_cfiles = id;
	if(GrItemSelIndex.size()){
		cFiles.clear();
		for(std::vector<UINT>::iterator i = GrItemSelIndex.begin(); i < GrItemSelIndex.end(); i++){
			cFiles.push_back(fileDirAndName(GrItemFile[*i].path,GrItemFile[*i].file_name));
		}
		SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_PASTE,(LPARAM)MAKELONG(1,0));
	}
}

void WmLButtonDown(HWND hWnd, POINT cp, WPARAM wParam){
	UINT index = si.nPos + cp.y / DY;
	if(index >= GrItemFile.size()){
		return ;
	}
	if(wParam & MK_SHIFT){
		if(GrItemSelIndex.size()){
			if(GrItemSelIndex.size() > 1u){
				for(std::vector<UINT>::iterator i = GrItemSelIndex.begin() + 1; i < GrItemSelIndex.end(); i++){
					GrItemFile[*i].selected = false;
				}
				GrItemSelIndex.erase(GrItemSelIndex.begin() + 1, GrItemSelIndex.end());
			}
			if(index < GrItemSelIndex[0]){
				for(UINT i = index;  i < GrItemSelIndex[0]; i++){
					GrItemFile[i].selected = true;
					GrItemSelIndex.push_back(i);
				}
			}else{
				for(UINT i = GrItemSelIndex[0] + 1;  i <= index; i++){
					GrItemFile[i].selected = true;
					GrItemSelIndex.push_back(i);
				}
			}
		}else{
			Select(index);
		}
	}else if(wParam & MK_CONTROL){
		for(std::vector<UINT>::iterator i = GrItemSelIndex.begin(); i < GrItemSelIndex.end(); i++){
			if(*i == index){
				GrItemFile[index].selected = false;
				GrItemSelIndex.erase(i);
				InvalidateRect(hWnd, NULL, false);
				return ;
			}
		}
		GrItemFile[index].selected = true;
		GrItemSelIndex.push_back(index);
	}else{
		Select(index);
	}
	if(GrItemSelIndex.size()){
		SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_CUT,(LPARAM)MAKELONG(1,0));
		SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_COPY,(LPARAM)MAKELONG(1,0));
		SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_TOSVG,(LPARAM)MAKELONG(1,0));
		SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_TOCSS,(LPARAM)MAKELONG(1,0));
		SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_TOPNG,(LPARAM)MAKELONG(1,0));
	}else{
		SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_CUT,(LPARAM)MAKELONG(0,0));
		SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_COPY,(LPARAM)MAKELONG(0,0));
		SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_TOSVG,(LPARAM)MAKELONG(0,0));
		SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_TOCSS,(LPARAM)MAKELONG(0,0));
		SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_TOPNG,(LPARAM)MAKELONG(0,0));
	}
	InvalidateRect(hWnd,NULL,false);
}

void InsertFiles(std::string path){

	_finddata_t f;
	intptr_t r;
	GrItemFile.clear();
	GrItemSelIndex.clear();
	SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_CUT,(LPARAM)MAKELONG(0,0));
	SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_COPY,(LPARAM)MAKELONG(0,0));
	SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_TOSVG,(LPARAM)MAKELONG(0,0));
	SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_TOCSS,(LPARAM)MAKELONG(0,0));
	SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_TOPNG,(LPARAM)MAKELONG(0,0));

	std::string path2 = path;
	path2 += "*.ggr";
	std::string path3;
	gradient gr;

	if((r=_findfirst(path2.c_str(),&f))>0){
		path3 = path;
		path3 += f.name;
		gr.LoadFile(path3);
		GrItem gi(path3,f.name,gr.gname,gr.gr);
		GrItemFile.push_back(gi);

		while(!_findnext(r,&f)){
			path3 = path;
			path3 += f.name;
			gr.LoadFile(path3);
			GrItem gi(path3,f.name,gr.gname,gr.gr);
			GrItemFile.push_back(gi);
		}
	}
	_findclose(r);
	si.cbSize		= sizeof(SCROLLINFO);
	si.fMask		= SIF_PAGE|SIF_POS|SIF_RANGE;
	si.nMax			= size.bottom < GrItemFile.size() * DY ? GrItemFile.size() - floor((double)size.bottom / DY) : 0;
	si.nMin			= 0;
	si.nPage		= si.nMax > 10 ? size.bottom / DY : 1;
	si.nPos			= 0;
	SetScrollInfo(grlist,SB_VERT,&si,true);
	InvalidateRect(grlist,NULL,true);
}

void Paste(){

	HTREEITEM item=TreeView_GetSelection(tree);
	if(item){
		TV_ITEM i;
		i.hItem=item;
		i.mask=TVIF_PARAM;
		TreeView_GetItem(tree,&i);
		FolderData *d=(FolderData*)i.lParam;
		
		if(d){
			switch(id_cfiles){
				case ID_CUT:
					{
						std::string path;
						for(std::vector<fileDirAndName>::iterator i = cFiles.begin(); i < cFiles.end(); i++){
							path = d->p;
							path += i->name;
							MoveFile(i->GetPathAndName().c_str(),path.c_str());
						}
						cFiles.clear();
						SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_PASTE,(LPARAM)MAKELONG(0,0));
					}
					break;
				case ID_COPY:
					{
						std::string path;
						for(std::vector<fileDirAndName>::iterator i = cFiles.begin(); i < cFiles.end(); i++){
							path = d->p;
							path += i->name;
							if(!CopyFile(i->GetPathAndName().c_str(),path.c_str(), true)){
								path = d->p;
								path += i->GetNameForCopy();
								CopyFile(i->GetPathAndName().c_str(), path.c_str(), true);
							}
						}
					}
					break;
			}

			if(item != mc){
				InsertFiles(d->p);
			}
		}
	}
}

void InsertDir(HWND tree, HTREEITEM parent, char* path){
	HTREEITEM i=TreeView_GetNextItem(tree, parent, TVGN_CHILD);
	while(i){
		HTREEITEM del=i;
		i=TreeView_GetNextSibling(tree, i);

		TV_ITEM item;
		item.mask=TVIF_PARAM;
		item.hItem=del;
		TreeView_GetItem(tree, &item);
		FolderData* d=(FolderData*)item.lParam;
		if(d)
			delete d;

		TreeView_DeleteItem(tree, del);
	}
	char buffer[PS];
	strcpy(buffer, path);
	/*char* r1=strrchr(buffer, '\\');
	if(r1){
		r1[0]=0;
		strcat(buffer, "\\");
		ltoa(licznik_el, buffer+strlen(buffer), 10);
		SetWindowText(GetParent(tree), buffer);
	}*/
	_finddata_t f;
	intptr_t r;
	if((r=_findfirst(buffer,&f)) > 0){
		if((f.attrib & _A_SUBDIR) && strcmp(f.name, ".") && strcmp(f.name, "..")){
			TV_INSERTSTRUCT tvins;
			tvins.hParent=parent;
			tvins.hInsertAfter=TVI_LAST;
//				SHFILEINFO shfi;
//				SHGetFileInfo(path, 0, &shfi, sizeof(shfi), SHGFI_ICON|SHGFI_SMALLICON|SHGFI_DISPLAYNAME);
//				int index=ImageList_AddIcon(imTree, shfi.hIcon);
			tvins.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM|TVIF_CHILDREN;
			tvins.item.hItem;
			tvins.item.state;
			tvins.item.stateMask;
			tvins.item.pszText=f.name;
			tvins.item.cchTextMax;
			tvins.item.iImage=I_IMAGECALLBACK;
			tvins.item.iSelectedImage=I_IMAGECALLBACK;
			strcpy(buffer, path);
			char* r1=strrchr(buffer, '\\');
			if(r1){
				r1[1]=0;
				strcat(buffer, f.name);
				strcat(buffer, "\\");
				tvins.item.lParam=(LPARAM)new FolderData(buffer);
				tvins.item.cChildren=CheckSubFolders(buffer);
			}
			HTREEITEM item=TreeView_InsertItem(tree, &tvins);
		}
		while(!_findnext(r,&f)){
			if((f.attrib & _A_SUBDIR) && strcmp(f.name, ".") && strcmp(f.name, "..")){
				TV_INSERTSTRUCT tvins;
				tvins.hParent=parent;
				tvins.hInsertAfter=TVI_LAST;
//				SHFILEINFO shfi;
//				SHGetFileInfo(path, 0, &shfi, sizeof(shfi), SHGFI_ICON|SHGFI_SMALLICON|SHGFI_DISPLAYNAME);
//				int index=ImageList_AddIcon(imTree, shfi.hIcon);
				tvins.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM|TVIF_CHILDREN;
				tvins.item.hItem;
				tvins.item.state;
				tvins.item.stateMask;
				tvins.item.pszText=f.name;
				tvins.item.cchTextMax;
				tvins.item.iImage=I_IMAGECALLBACK;
				tvins.item.iSelectedImage=I_IMAGECALLBACK;
				strcpy(buffer, path);
				char* r1=strrchr(buffer, '\\');
				if(r1){
					r1[1]=0;
					strcat(buffer, f.name);
					strcat(buffer, "\\");
					tvins.item.lParam=(LPARAM)new FolderData(buffer);
					tvins.item.cChildren=CheckSubFolders(buffer);
				}
				HTREEITEM item=TreeView_InsertItem(tree, &tvins);
			}
		}
	 }
	_findclose(r);
}

void InsertDrivers(HWND tree, HTREEITEM parent){

	TV_INSERTSTRUCT tvins;
	tvins.hParent=parent;
	tvins.hInsertAfter=TVI_LAST;

		for(int i='A'; i<='Z'; i++){
			char path[10];
			wsprintf(path, "%c:\\", (char)i);
			UINT type=GetDriveType(path);
				if(type!=0 && type!=1){
				SHFILEINFO shfi;
				SHGetFileInfo(path, 0, &shfi, sizeof(shfi), SHGFI_ICON|SHGFI_SMALLICON|SHGFI_DISPLAYNAME);
				int index=ImageList_AddIcon(imTree, shfi.hIcon);
				tvins.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM|TVIF_CHILDREN;
				tvins.item.hItem;
				tvins.item.state;
				tvins.item.stateMask;
				tvins.item.pszText=shfi.szDisplayName;
				tvins.item.cchTextMax;
				tvins.item.iImage=index;
				tvins.item.iSelectedImage=index;
				tvins.item.cChildren=1;
				tvins.item.lParam=(LPARAM)new FolderData(path);
				HTREEITEM item=TreeView_InsertItem(tree, &tvins);
				DestroyIcon(shfi.hIcon);
			}
		}
}

void InsertDriversToList(HWND list){
		LV_ITEM item;
		item.mask=LVIF_TEXT|LVIF_IMAGE;

		for(int i='A'; i<='Z'; i++){
			char path[10];
			wsprintf(path, "%c:\\", (char)i);
			UINT type=GetDriveType(path);
				if(type!=0 && type!=1){
					SHFILEINFO shfi;
					SHGetFileInfo(path, 0, &shfi, sizeof(shfi), SHGFI_ICON|SHGFI_SMALLICON|SHGFI_DISPLAYNAME);
					int index=ImageList_AddIcon(imList, shfi.hIcon);
					item.mask=LVIF_TEXT|LVIF_IMAGE;
					item.iItem=0;
					item.iSubItem=0;
					item.state;
					item.stateMask;
					item.pszText=shfi.szDisplayName;
					item.cchTextMax;
					item.iImage=index;
					item.lParam=(LPARAM)new FolderData(path);
					ListView_InsertItem(list, &item);
					DestroyIcon(shfi.hIcon);
				}
		}
}
//Wstaw katalog do listy
void InsertDirToList(HWND list, char* path,int dodaj){
	if(dodaj)
		SavePath(path);
	SendMessage(list, WM_SETREDRAW, FALSE, 0);
	LV_ITEM item;
	int i=ListView_GetNextItem(list, -1, LVNI_ALL);
	while(i!= -1){
		item.mask=LVIF_PARAM;
		item.iItem=i;
		ListView_GetItem(list, &item);
		FolderData* d=(FolderData*)item.lParam;
		if(d){
			delete d;
		}
		ListView_DeleteItem(list, i);
		i=ListView_GetNextItem(list, -1, LVNI_ALL);
	}
	char buffer[PS];
	strcpy(buffer, path);
	/*char* r1=strrchr(buffer, '\\');
	if(r1){
		r1[0]=0;
		strcat(buffer, "\\");
		ltoa(licznik_el, buffer+strlen(buffer), 10);
		SetWindowText(GetParent(list), buffer);
	}*/
	i=0;
	_finddata_t f;
	intptr_t r;
	if((r=_findfirst(buffer,&f))>0){
		while(!_findnext(r,&f)){
			if(f.attrib && strcmp(f.name, ".") && strcmp(f.name, "..")){
				LV_ITEM lvitem;
				lvitem.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
				lvitem.iItem=i;
				lvitem.iSubItem=0;
				lvitem.pszText=f.name;
				lvitem.iImage=I_IMAGECALLBACK;
				strcpy(buffer, path);
				char* r1=strrchr(buffer, '\\');
				if(r1){
					r1[1]=0;
					strcat(buffer, f.name);
					strcat(buffer, "\\");
					lvitem.lParam=(LPARAM)new FolderData(buffer);
				}
				ListView_InsertItem(list, &lvitem);
				lvitem.iSubItem=3;
				char d[PS];
				lvitem.mask=LVIF_TEXT;                                                                                                                                                         //NM_DBLCLK
				tm *newtime;
				newtime = localtime(&f.time_create);
				strftime(buffer,PS,"%Y-%B-%d %H:%M:%d",newtime);       //ListView_HitTest
				lvitem.pszText=buffer;
				ListView_SetItem(list, &lvitem);
				lvitem.iSubItem=1;
				path[strlen(path)-3]=NULL;
				wsprintf(d,"%s\\%s",path,f.name);
				path[strlen(path)]= '*';
				lvitem.pszText=((f.attrib & _A_SUBDIR)?"folder":"plik");
				ListView_SetItem(list,&lvitem);
				lvitem.iSubItem=2;
				if(f.size<1000)
					wsprintf(d,"%i b",f.size);
				else if(f.size<1000000)
					wsprintf(d,"%i Kb",f.size/1000);
				else
					wsprintf(d,"%i Mb",f.size/1000000);
				lvitem.pszText= d;
				ListView_SetItem(list,&lvitem);
				i++;
			}
		}
	 }
	SendMessage(list, WM_SETREDRAW, TRUE, 0);
}

//ZnajdŸ folder w drzewie
void FindDirInTree(HWND tree, std::string p, int &n,HTREEITEM item){
	std::string copy = p;
	if(*(copy.end()-1) != '\\')
		copy += "\\";
	
	std::basic_string<char>::size_type s = 0u;

	for(int i = 0; i < n; i++){
		s=copy.find_first_of('\\',s?s+1:0);
	}
	copy.erase(s+1,copy.size());

	n++;
	if(copy.size()){

		while(item){
			TV_ITEM i;
			i.mask=TVIF_HANDLE|TVIF_PARAM;
			i.hItem=item;
			TreeView_GetItem(tree, &i);
			FolderData* d=(FolderData*)i.lParam;
			if(d){
				if(copy.compare(d->p)==0){
					TreeView_Expand(tree, item, TVE_EXPAND);
					TreeView_SelectItem(tree ,item);
					FindDirInTree(tree, p, n,TreeView_GetNextItem(tree, item, TVGN_CHILD));
					break;
				}
			}
			item=TreeView_GetNextSibling(tree, item);
		}
	}
}

LRESULT CALLBACK BrowserProc(HWND hWnd, UINT uMsg, WPARAM wParam,LPARAM lParam){
	switch(uMsg){
		case WM_CREATE:
			{
				//hMenu=LoadMenu(((LPCREATESTRUCT)lParam)->hInstance,MAKEINTRESOURCE(IDR_MENU1));
				//hMenu=GetSubMenu(hMenu,0);
				if(!hIcon)
					hIcon = LoadIcon(hInst, (LPCTSTR)IDI_GIMP);

				TBBUTTON tb[11];					//tablica struktur potrzebna do funkcji CreateToolbarEx
	
				int i = 0;

				tb[i].iBitmap	= i;
				tb[i].idCommand	= ID_TB_PAGEUP;
				tb[i].fsState	= TBSTATE_ELLIPSES;
				tb[i].fsStyle	= TBSTYLE_BUTTON;
				tb[i].dwData	= NULL;
				tb[i].iString	= -1;

				i++;
				
				tb[i].iBitmap=i;
				tb[i].idCommand=ID_TB_BACK;
				tb[i].fsState=TBSTATE_ELLIPSES;
				tb[i].fsStyle=TBSTYLE_BUTTON;
				tb[i].dwData=NULL;
				tb[i].iString=-1;

				i++;

				tb[i].iBitmap=i;
				tb[i].idCommand=ID_TB_REFRESH;
				tb[i].fsState=TBSTATE_ELLIPSES|TBSTATE_ENABLED;
				tb[i].fsStyle=TBSTYLE_BUTTON;
				tb[i].dwData=NULL;
				tb[i].iString=-1;

				i++;

				tb[i].iBitmap=0;
				tb[i].idCommand=0;
				tb[i].fsState=TBSTATE_ELLIPSES|TBSTATE_ENABLED;
				tb[i].fsStyle=TBSTYLE_SEP;
				tb[i].dwData=NULL;
				tb[i].iString=-1;

				i++;

				tb[i].iBitmap=i - 1;
				tb[i].idCommand=ID_TB_CUT;
				tb[i].fsState=TBSTATE_ELLIPSES;
				tb[i].fsStyle=TBSTYLE_BUTTON;
				tb[i].dwData=NULL;
				tb[i].iString=-1;

				i++;

				tb[i].iBitmap=i - 1;
				tb[i].idCommand=ID_TB_COPY;
				tb[i].fsState=TBSTATE_ELLIPSES;
				tb[i].fsStyle=TBSTYLE_BUTTON;
				tb[i].dwData=NULL;
				tb[i].iString=-1;

				i++;

				tb[i].iBitmap=i - 1;
				tb[i].idCommand=ID_TB_PASTE;
				tb[i].fsState=TBSTATE_ELLIPSES;
				tb[i].fsStyle=TBSTYLE_BUTTON;
				tb[i].dwData=NULL;
				tb[i].iString=-1;

				i++;

				tb[i].iBitmap=0;
				tb[i].idCommand=0;
				tb[i].fsState=TBSTATE_ELLIPSES;
				tb[i].fsStyle=TBSTYLE_SEP;
				tb[i].dwData=NULL;
				tb[i].iString=-1;

				i++;

				tb[i].iBitmap=i - 2;
				tb[i].idCommand=ID_TB_TOSVG;
				tb[i].fsState=TBSTATE_ELLIPSES;
				tb[i].fsStyle=TBSTYLE_BUTTON;
				tb[i].dwData=NULL;
				tb[i].iString=-1;

				i++;

				tb[i].iBitmap=i - 2;
				tb[i].idCommand=ID_TB_TOCSS;
				tb[i].fsState=TBSTATE_ELLIPSES;
				tb[i].fsStyle=TBSTYLE_BUTTON;
				tb[i].dwData=NULL;
				tb[i].iString=-1;

				i++;

				tb[i].iBitmap=i - 2;
				tb[i].idCommand=ID_TB_TOPNG;
				tb[i].fsState=TBSTATE_ELLIPSES;
				tb[i].fsStyle=TBSTYLE_BUTTON;
				tb[i].dwData=NULL;
				tb[i].iString=-1;

				toolbar=CreateToolbarEx(hWnd,WS_CHILD|WS_VISIBLE,111,sizeof(tb)/sizeof(TBBUTTON),hInst,IDB_BROWSER,tb,sizeof(tb)/sizeof(TBBUTTON),22,22,22,22,sizeof(TBBUTTON));
				SendMessage(toolbar,TB_AUTOSIZE,0,0);

				ITEMIDLIST* pidl;
				SHGetSpecialFolderLocation(0, CSIDL_DRIVES, &pidl);
				char buffer[PS];

				//SHGetPathFromIDList(pidl, buffer);

				SHFILEINFO shfi;
				SHGetFileInfo((char*)pidl, 0, &shfi, sizeof(shfi),
											 SHGFI_PIDL|SHGFI_DISPLAYNAME|SHGFI_ICON|SHGFI_SMALLICON);

				//	MessageBox(0, shfi.szDisplayName, 0, MB_OK);

				tree = CreateWindowEx(WS_EX_CLIENTEDGE, "SysTreeView32","",TVS_SHOWSELALWAYS|WS_CHILD|WS_VISIBLE|TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT, 10, 10, 100, 100, hWnd, (HMENU)123 ,hInst, 0);
				imTree=ImageList_Create(16, 16, ILC_COLOR24|ILC_MASK, 10, 10);
				ImageList_SetBkColor(imTree, GetSysColor(COLOR_WINDOW));
				TreeView_SetImageList(tree, imTree, TVSIL_NORMAL);
				ImageList_AddIcon(imTree, LoadIcon(0, IDI_APPLICATION));	//0
				ImageList_AddIcon(imTree, LoadIcon(0, IDI_HAND));			//1
				ImageList_AddIcon(imTree, shfi.hIcon);						//2

				
				TV_INSERTSTRUCT tvins;
				tvins.hParent=TVI_ROOT;
				tvins.hInsertAfter=TVI_LAST;
				tvins.item;
				tvins.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM;
				tvins.item.hItem;
				tvins.item.state;
				tvins.item.stateMask;
				tvins.item.pszText=shfi.szDisplayName;
				tvins.item.cchTextMax;
				tvins.item.iImage=2;
				tvins.item.iSelectedImage=2;
				tvins.item.cChildren;
				tvins.item.lParam=0;
				mc=TreeView_InsertItem(tree, &tvins);
				InsertDrivers(tree, mc);	//Dodaj napêdy do drzewa
				TreeView_Expand(tree, mc, TVE_EXPAND);

				grlist = CreateWindow(GRADIENT_BROWSER_LIST, GRADIENT_BROWSER_LIST,WS_CHILD|WS_VISIBLE|WS_VSCROLL,10,10,100,100,hWnd,(HMENU)ID_LIST, hInst, NULL);

				pathEdit = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL,10,10,100,PATH_EDIT_HEIGHT,hWnd,(HMENU)ID_EDIT,hInst,NULL);

				NONCLIENTMETRICS ncm;
				ncm.cbSize = sizeof(ncm);
				SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
				SendMessage(pathEdit,WM_SETFONT,(WPARAM)CreateFontIndirect(&ncm.lfStatusFont),NULL);

				/*SetWindowText(pathEdit,"D:\\Program Files (x86)\\GIMP-2.0\\share\\gimp\\2.0\\gradients");*/
			}
		break;
		case WM_COMMAND:{
			switch(LOWORD(wParam)){
				case ID_TB_PAGEUP:
				{
					HTREEITEM item=TreeView_GetSelection(tree);
					TreeView_Expand(tree, item, TVE_COLLAPSE);
					if(item){
						TV_ITEM i;
						i.hItem=item;
						i.mask=TVIF_PARAM;
						TreeView_GetItem(tree,&i);
						FolderData *d=(FolderData*)i.lParam;
						if(d){
							char bufor[PS];
							strcpy(bufor,d->p);
							if(strlen(bufor)>3){
								char *p=strrchr(bufor,'\\');
								*p=NULL;
								p=strrchr(bufor,'\\');
								*(p+1)=NULL;
								char bufor2[PS];
								wsprintf(bufor2,"%s*.*",bufor);
								HTREEITEM item=TreeView_GetNextItem(tree, mc, TVGN_CHILD);
								int n=1;
								FindDirInTree(tree, bufor,n,item);

								HTREEITEM hti = TreeView_GetSelection(tree);

								TV_ITEM i;
								i.mask	= TVIF_HANDLE|TVIF_PARAM;
								i.hItem	= hti;

								if(TreeView_GetItem(tree, &i)){
									FolderData* fd = (FolderData*)i.lParam;
									if(fd){
										InsertFiles(fd->p);
										SetEdit(false, fd->p);
									}
								}
							}

						}
					}
				}
				break;
				case ID_TB_BACK:
					if(lastPath){
						back* prev=lastPath->prev;
						if(prev){
							delete lastPath;
							lastPath=prev;
							int n=1;
							char buffer[PS];
							strcpy(buffer,prev->path);
							char *p=strrchr(buffer,'\\');
							if(p)
								*p=NULL;
							FindDirInTree(tree,buffer,n,TreeView_GetNextItem(tree,mc,TVGN_CHILD));

							HTREEITEM hti = TreeView_GetSelection(tree);
									
							TV_ITEM i;
							i.mask	= TVIF_HANDLE|TVIF_PARAM;
							i.hItem	= hti;

							if(TreeView_GetItem(tree, &i)){
								FolderData* fd = (FolderData*)i.lParam;
								if(fd){
									InsertFiles(fd->p);
									SetEdit(false, fd->p);
								}
							}
						}else{
							TreeView_SelectItem(tree,mc);
							SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)5000,(LPARAM)MAKELONG(0,0));
							SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)5001,(LPARAM)MAKELONG(0,0));
							GrItemFile.clear();
							GrItemSelIndex.clear();
							InvalidateRect(grlist, NULL, false);
						}
					}
				break;
				case ID_TB_REFRESH:
					{
						HTREEITEM hti = TreeView_GetSelection(tree);

						if(hti != mc){
									
							TV_ITEM i;
							i.mask	= TVIF_HANDLE|TVIF_PARAM;
							i.hItem	= hti;

							if(TreeView_GetItem(tree, &i)){
								FolderData* fd = (FolderData*)i.lParam;
								if(fd){
									InsertFiles(fd->p);
									SetEdit(false, fd->p);
								}
							}
						}
					}
					break;
				case ID_TB_CUT:
					{
						ColectFileDir(ID_CUT);
					}
					break;
				case ID_TB_COPY:
					{
						ColectFileDir(ID_COPY);
					}
					break;
				case ID_TB_PASTE:
					{
						Paste();
					}
					break;
				case ID_TB_TOSVG:
					{
						std::vector<std::string> tPath;

						for(std::vector<UINT>::iterator i = GrItemSelIndex.begin(); i < GrItemSelIndex.end(); i++){
							tPath.push_back(GrItemFile[*i].path);
						}
						if(tPath.size())
							Document::CreateGradientSvgFile(GetParent(hWnd), tPath);
					}
					break;
				case ID_TB_TOCSS:
					{
						std::vector<std::string> tPath;

						for(std::vector<UINT>::iterator i = GrItemSelIndex.begin(); i < GrItemSelIndex.end(); i++){
							tPath.push_back(GrItemFile[*i].path);
						}
						if(tPath.size())
							Document::ExportGradientFromGgrToCss(GetParent(hWnd), tPath);
					}
					break;
				case ID_TB_TOPNG:
					{
						std::vector<std::string> tPath;

						for(std::vector<UINT>::iterator i = GrItemSelIndex.begin(); i < GrItemSelIndex.end(); i++){
							tPath.push_back(GrItemFile[*i].path);
						}
						if(tPath.size())
							Document::CreateGradientPngFile(GetParent(hWnd), tPath);
					}
					break;
				case ID_EDIT:
					{
						switch(HIWORD(wParam)){
							case EN_CHANGE:
								{
									if(!serchpath)
										break;
									UINT tl = GetWindowTextLength(pathEdit);
									if(!tl)
										break;
									tl += 1;
									int n = 1;
									std::string p = "D:\\Program Files (x86)\\GIMP-2.0\\share\\gimp\\2.0\\gradients";
									char* path = new char[tl];
									GetWindowText(pathEdit, path,tl);
									p = path;
									delete[] path;
									FindDirInTree(tree,p.c_str(),n,TreeView_GetNextItem(tree,mc,TVGN_CHILD));
									HTREEITEM hti = TreeView_GetSelection(tree);
									
									TV_ITEM i;
									i.mask	= TVIF_HANDLE|TVIF_PARAM;
									i.hItem	= hti;

									if(TreeView_GetItem(tree, &i)){
										FolderData* fd = (FolderData*)i.lParam;
										if(fd){
											p+= "\\";
											if(p.compare(fd->p)){
												//MessageBox(hWnd, "Coœ nie tak", "Coœ nie tak",MB_OK);
											}
											InsertFiles(fd->p);
										}
									}
								}
								break;
						}
					}
					break;
			}
			break;
		}
		case WM_NOTIFY:{
			switch(((NMHDR*)lParam)->code){
				case NM_RCLICK:
				{
					if(((NMHDR*)lParam)->idFrom==123){
						POINT point;
						GetCursorPos(&point);
						ScreenToClient(tree,&point);
						LV_HITTESTINFO info;
						info.pt=point;
						HTREEITEM item=TreeView_HitTest(tree,&info);
						TV_ITEM tvitem;
						tvitem.hItem=item;
						tvitem.mask=TVIF_PARAM;
						TreeView_GetItem(tree,&tvitem);
						FolderData *d=(FolderData*)(tvitem.lParam);
						if(d){
							char b[PS];
							strcpy(b, d->p);
							char* p=strrchr(b, '\\');
							if(p)
								p[0]=0;

							SHELLEXECUTEINFO sei;
							sei.cbSize=sizeof(sei);
							sei.fMask=SEE_MASK_INVOKEIDLIST;
							sei.hwnd=hWnd;
							sei.lpVerb="properties";
							sei.lpFile=b;
							sei.lpParameters=0;
							sei.lpDirectory=0;
							sei.nShow=SW_NORMAL;
							sei.hInstApp=0;
							sei.lpIDList=0;
							sei.lpClass=0;
							sei.hkeyClass=0;
							sei.dwHotKey=0;
							sei.hIcon=0;
							sei.hProcess=0;
							ShellExecuteEx(&sei);
						}
					}
				}
				break;
				case NM_CLICK:{
					if(((NMHDR*)lParam)->idFrom==123){

					}
				}
				break;
				case NM_DBLCLK:{
/*					if(((NMHDR*)lParam)->hwndFrom==tree){
						MessageBox(0, "drzewo", "", MB_OK);
					}
					else{
						MessageBox(0, "lista", "", MB_OK);
					}*/
					if(((NMHDR*)lParam)->idFrom==123){

					}
				}
					break;
				case TVN_GETDISPINFO:{
					TV_DISPINFO* di=(TV_DISPINFO*)lParam;
					FolderData* d=(FolderData*)di->item.lParam;
					if(d){
						char buffer[PS];
						strcpy(buffer, d->p);
						char* r=strrchr(buffer, '\\');
						if(r){
							r[0]=0;
							SHFILEINFO shfi;
							SHGetFileInfo(buffer, 0, &shfi, sizeof(shfi), SHGFI_ICON|SHGFI_SMALLICON|SHGFI_DISPLAYNAME);
							ImageList_ReplaceIcon(imTree, 0, shfi.hIcon);
							DestroyIcon(shfi.hIcon);
							SHGetFileInfo(buffer, 0, &shfi, sizeof(shfi), SHGFI_OPENICON|SHGFI_SMALLICON|SHGFI_ICON|SHGFI_DISPLAYNAME);
							ImageList_ReplaceIcon(imTree, 1, shfi.hIcon);
							DestroyIcon(shfi.hIcon);
							di->item.iImage=0;
							di->item.iSelectedImage=1;
						}
					}
				}
				break;

				case LVN_GETDISPINFO:{
					LV_DISPINFO* di=(LV_DISPINFO*)lParam;
					FolderData* d=(FolderData*)di->item.lParam;
					if(d){
						char buffer[PS];
						strcpy(buffer, d->p);
						char* r=strrchr(buffer, '\\');
						if(r){
							if(strlen(buffer)>3)
								r[0]=0;
							SHFILEINFO shfi;

							SHGetFileInfo(buffer, 0, &shfi, sizeof(shfi), SHGFI_ICON|SHGFI_SMALLICON|SHGFI_DISPLAYNAME);
							ImageList_ReplaceIcon(imList, 0, shfi.hIcon);
							DestroyIcon(shfi.hIcon);
							di->item.iImage=0;
							DestroyIcon(shfi.hIcon);
						}
					}
				}
				break;

				case TVN_SELCHANGED:
				{
						NM_TREEVIEW* tv = (NM_TREEVIEW*)lParam;
						if(tv->action!=TVC_UNKNOWN){

						FolderData* d1=(FolderData*)tv->itemNew.lParam;
						if(d1){
							 char p[PS];
							 wsprintf(p, "%s", d1->p);
							 SavePath(p);
							 InsertFiles(p);
							 SetEdit(false, p);
						}
					}
				}
				break;

				case TVN_ITEMEXPANDING:
					NM_TREEVIEW* tv = (NM_TREEVIEW*)lParam;
					FolderData* d1=(FolderData*)tv->itemNew.lParam;
					if(d1){
						 char p[PS];
						 wsprintf(p, "%s*.*", d1->p);
						 InsertDir(tree, tv->itemNew.hItem, p);
					}
			}
		}
		break;
		case WM_SIZE:{
			RECT rect;
			GetClientRect(hWnd, &rect);
			RECT br;
			GetWindowRect(toolbar, &br);
			SendMessage(toolbar,TB_AUTOSIZE,0,0);
			SetWindowPos(pathEdit, 0, 0, br.bottom - br.top, rect.right - rect.left, PATH_EDIT_HEIGHT,SWP_NOZORDER); 
			SetWindowPos(tree, 0, 0, br.bottom-br.top + PATH_EDIT_HEIGHT, WIDTH,
							 rect.bottom-(br.bottom-br.top) - PATH_EDIT_HEIGHT, SWP_NOZORDER);
			SetWindowPos(grlist, 0, WIDTH, br.bottom-br.top + PATH_EDIT_HEIGHT,
				rect.right-WIDTH>300?rect.right-WIDTH:2*WIDTH, rect.bottom-(br.bottom-br.top)-PATH_EDIT_HEIGHT, SWP_NOZORDER);
		}
		break;
		case WM_DESTROY:
			DestroyWindow(hWnd);
			GrItemFile.clear();
			GrItemSelIndex.clear();
			cFiles.clear();
		break;
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

LRESULT CALLBACK GradientBrowserProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

	switch(msg){
		case WM_CREATE:
			{
				si.nPos = 0;
			}
			break;
		case WM_SIZE:
			{
				GetClientRect(hWnd,&size);
				si.cbSize		= sizeof(SCROLLINFO);
				si.fMask		= SIF_PAGE|SIF_POS|SIF_RANGE;
				si.nMax			= size.bottom < GrItemFile.size() * DY ? GrItemFile.size() - floor((double)size.bottom / DY) : 0;
				si.nMin			= 0;
				si.nPage		= si.nMax > 10 ? size.bottom / DY  : 1;
				si.nPos			= min(si.nMax,max(0,si.nPos));
				SetScrollInfo(hWnd,SB_VERT,&si,true);
			}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);

				SelectObject(hdc, pnGray200);
				Rectangle(hdc, size.left, size.top, size.right, size.bottom);
				SetStretchBltMode(hdc, COLORONCOLOR);
				SetBkMode(hdc, TRANSPARENT);

				SelectObject(hdc, textFont);
				int dy = 0;
				for(int i = si.nPos; i < GrItemFile.size(); i++){
					GrItemFile[i].GrDraw(hdc, 1, dy, size);
					if(dy > size.bottom)
						break;
				}

				EndPaint(hWnd, &ps);
			}
			break;
		case WM_KEYDOWN:
			{
				switch(wParam){
					case VK_DELETE:
						{
							if(GrItemSelIndex.size()){
								std::string s = "Czy na pewno chcesz usun¹æ ";

								char dest[100];
								_ltoa(GrItemSelIndex.size(), dest, 10);
								s += dest;
								if(GrItemSelIndex.size() > 1)
									s += "pliki (are you sure you want to delete ";
								else
									s += "plik (are you sure you want to delete ";
								s += dest;
								if(GrItemSelIndex.size() > 1)
									s += " files?";
								else
									s += " file?";

								if(MessageBox(hWnd, s.c_str(), "Ostrze¿enie (warning)", MB_YESNO|MB_ICONWARNING) == IDYES){
									sort(GrItemSelIndex.begin(), GrItemSelIndex.end());
									for(int i = GrItemSelIndex.size() - 1; i > -1; i--){
										DeleteFile(GrItemFile[GrItemSelIndex[i]].path.c_str());
										GrItemFile.erase(GrItemFile.begin() + GrItemSelIndex[i]);
									}
									GrItemSelIndex.clear();
									si.fMask		= SIF_PAGE|SIF_POS|SIF_RANGE;
									si.nMax			= size.bottom < GrItemFile.size() * DY ? GrItemFile.size() - floor((double)size.bottom / DY) : 0;
									si.nMin			= 0;
									si.nPage		= si.nMax > 10 ? size.bottom / DY  : 1;
									si.nPos			= min(si.nMax,si.nPos);
									SetScrollInfo(hWnd,SB_VERT,&si,true);
									InvalidateRect(hWnd, NULL, false);
								}
								if(!GrItemFile.size())
								{
									SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_CUT,(LPARAM)MAKELONG(0,0));
									SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_COPY,(LPARAM)MAKELONG(0,0));
									SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_TOSVG,(LPARAM)MAKELONG(0,0));
									SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_TOCSS,(LPARAM)MAKELONG(0,0));
									SendMessage(toolbar,TB_ENABLEBUTTON,(WPARAM)ID_TB_TOPNG,(LPARAM)MAKELONG(0,0));
								}

							}
						}
						break;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				SetFocus(hWnd);
				POINT cp = {0,0};

				cp.x = LOWORD(lParam);
				cp.y = HIWORD(lParam);

				WmLButtonDown(hWnd, cp, wParam);
			}
			break;
		case WM_MOUSEWHEEL:
			{
				int iAccumDelta = (SHORT)HIWORD(wParam);	// warzne jest rzutowanie na SHORT bo innaczej gubi znak (-)
				
				si.nPos = max(min(si.nMax, si.nPos - iAccumDelta / WHEEL_DELTA),0);

				SetScrollInfo(hWnd,SB_VERT,&si,true);

				InvalidateRect(hWnd,NULL,false);
			}
			return 0;
		case WM_VSCROLL:
			{
				si.cbSize		= sizeof(SCROLLINFO);
				si.fMask		= SIF_ALL;

				//GetScrollInfo(hWnd,SB_VERT,&si);

				si.fMask		= SIF_POS;

				int k = si.nMax > 40 ? 4 : 1;
				
				switch(LOWORD(wParam)){
					case SB_ENDSCROLL:
						break;
					case SB_PAGEUP:
							if(si.nPos - (int)si.nPage < 0){
								si.nPos	= 0;
							}else{
								si.nPos	-= si.nPage;
							}
						break;
					case SB_PAGEDOWN:
							if((int)si.nPos + si.nPage > (int)si.nMax){
								si.nPos	= si.nMax;
							}else{
								si.nPos	+= si.nPage;
							}
						break;
					case SB_LINEUP:
						{
							if((int)si.nPos - k < 0){
								si.nPos	= 0;
							}else{
								si.nPos	-= k;
							}
						}
						break;
					case SB_LINEDOWN:
						{
							if((int)si.nPos + k > (int)si.nMax){
								si.nPos	= si.nMax;
							}else{
								si.nPos	+= k;
							}
						}
						break;
					case SB_THUMBTRACK:
						{
							si.nPos		= si.nMax - HIWORD(wParam) < si.nPage ? si.nMax :HIWORD(wParam) ;
						}
						break;
				}

				//si.nPos;

				InvalidateRect(hWnd,NULL,false);

				SetScrollInfo(hWnd,SB_VERT,&si,true);
			}
			break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}