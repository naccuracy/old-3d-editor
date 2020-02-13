//Defines the entry point for the application.
//

#include "stdafx.h"
#include "Diplom.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include "geom.h"
#include <string>
//
int flagpoint=0;
int nnn;
Schema2d schema;
int flagsch=0;
int GetStringFromFile(FILE *f, char str[256])
{
	char c=0;
	int i=0;
	while (1)
	{
		fread(&c,1,1,f);
		if(c!='\n'){str[i]=c;i++;}
		else{if(i!=0){str[i]='\0';break;}}
	}
	return 1;
}
//
//Project
class Project
{

public:
	//data
	vector<string> RingNames;
	Ring_array Rings;
	vector<string> MeshNames;
	vector<Mesh> Meshes;
	int SelectedR, SelectedM;
	//members
	void AddMesh(Mesh m, char* name)
	{
		Meshes.push_back(m);
		MeshNames.push_back(name);
		return;
	};

	void AddRing(Ring r, char* name)
	{
		Rings.push_back(r);
		RingNames.push_back(name);
		return;
	};
	int Save(char *file)
	{
		if(schema.eTop.size()==0 && Meshes.size()==0 && Rings.size()==0)
		{
		 MessageBox(NULL,_T("Íåò íåîáõîäèìîñòè ñîõðàíÿòü ïóñòîé ïðîåêò"),_T("Îøèáêà"),0);return 0;
		}
		FILE *f=NULL;
		int i,j;
		fopen_s(&f,file,"wt");
		if(f==NULL){MessageBox(NULL,_T("Îøèáêà ñîçäàíèÿ ôàéëà"),_T("Îøèáêà"),0);return 0;}
		if(schema.eTop.size()!=0)
		{
			fprintf(f,"schema\n");
			schema.Append(f);
		}
		if(Meshes.size()!=0)
		{
			fprintf(f,"\nmeshes\n");
			fprintf(f,"%d\n",Meshes.size());//êîëè÷åñòâî ñåòîê
			for(i=0;i<Meshes.size();i++)
			{
				fprintf(f,"%s\n",MeshNames[i].str);
				Meshes[i].Append(f);
			}
			fprintf(f,"%d\n",SelectedM);
		}
		if(Rings.size()!=0)
		{
			fprintf(f,"rings\n");
			fprintf(f,"%d\n",Rings.size());
			for(i=0;i<Rings.size();i++)
			{
				fprintf(f,"%s\n",RingNames[i].str);
				fprintf(f,"%d\n",Rings[i].flag);
				fprintf(f,"%d\n",Rings[i].points.size());
				for(j=0;j<Rings[i].points.size();j++)
					fprintf(f,"%f %f\n",Rings[i].points[j].x,Rings[i].points[j].y);
				fprintf(f,"%d\n",Rings[i].edges.size());
				for(j=0;j<Rings[i].edges.size();j++)
					fprintf(f,"%d %d\n",Rings[i].edges[j].start,Rings[i].edges[j].end);
			}
		}
		fclose(f);
		return 1;
	};
	int Open(char *file)
	{
		Clear();
		Ring ri;
		Mesh me;
		int i, j, n,m;
		char str[256];
		FILE *f=NULL;
		vector2f v;
		iEdge ie;
		fopen_s(&f,file,"rt");
		if(f==NULL)
		{
			MessageBox(NULL,_T("Ôàéë íå íàéäåí!"),_T("Îøèáêà"),0);return 0;
		}
		
			fscanf(f,"%s",&str);
		
			if(strcmp(str,"schema")==0)
			{
				schema.Read(f);
				flagsch=1;
				fscanf(f,"%s",&str);
		
			}
			if(strcmp(str,"meshes")==0)
			{
		
				fscanf(f,"%d",&n);
				for(i=0;i<n;i++)
				{
					//fscanf(f,"%s",&str);
					GetStringFromFile(f,str);
					//MessageBoxA(NULL,str,"123",0);
					me.~Mesh();
					me.Extract(f);
					AddMesh(me,str);
				}
				fscanf(f,"%d",&SelectedM);
				fscanf(f,"%s",&str);
			}
			if(strcmp(str,"rings")==0)
			{
				fscanf(f,"%d",&n);
				for(i=0;i<n;i++)
				{
					ri.clear();
					GetStringFromFile(f,str);
					
					fscanf_s(f,"%d",&m);
					ri.flag=m;
					
					fscanf_s(f,"%d",&m);
					for(j=0;j<m;j++)
					{
						fscanf_s(f,"%f %f",&v.x,&v.y);
						ri.points.push_back(v);
					}
					fscanf_s(f,"%d",&m);
					for(j=0;j<m;j++)
					{
						fscanf(f,"%d %d",&ie.start,&ie.end);
						ri.edges.push_back(ie);
					}
					AddRing(ri,str);
				}

			}
		
		fclose(f);
		return 1;
	};
	void DeleteMesh(int i)
	{
		Meshes.erase(Meshes.begin()+i);
		MeshNames.erase(MeshNames.begin()+i);
		return ;
	};
	void DeleteRing(int i)
	{
		Rings.erase(Rings.begin()+i);
		RingNames.erase(RingNames.begin()+i);
		return ;
	};
	void DeleteCurrentMesh()
	{
		if(SelectedM>=0)
		{
			DeleteMesh(SelectedM);
			SelectedM=-1;
		}
		return;
	};

	void DeleteCurrentRing()
	{
		if(SelectedR>=0)
		{
			DeleteRing(SelectedR);
			SelectedR=-1;
		}
		return;
	};
	void Clear()
	{
		if(Rings.size()>0)
		{
			Rings.clear();
			RingNames.clear();
		}
		if(Meshes.size()>0)
		{
			Meshes.clear();
			MeshNames.clear();
		}
		SelectedR=SelectedM=-1;
		return;
	};
	Project()
	{
		Clear();
	};
	~Project(){Clear();};
};
//
#define MAX_LOADSTRING 100
//////////////////////////////////////////////////////////////////////////////////////////////////
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
#define IDB1 39001
#define IDB2 39002
#define IDCB1 39003
#define IDCB2 39004
HWND b1, cb1;
HWND b2, cb2;
vector3f minsch, maxsch;
//////////////////////////////////////////////////
POINT MainFormSize;
int m3D_flag=0;
int mSchema_flag=0;
int tryselect=0;
//DATA
Project Prjct;

char Text[256];
HWND MainWnd;
///////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
ATOM Register3DViewerClass(HINSTANCE hInstance);
LRESULT CALLBACK	m3DViewerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int SetPFD(HDC hdc);
void RenderMesh(Mesh m, int light, vector3f clr);
ATOM RegisterSchemaViewerClass(HINSTANCE hInstance);
LRESULT CALLBACK SchemeViewerProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int IsPixelInEdge(float x, float y, int viewflag, vector2f a, vector2f b, vector2f _0pos, float scale);

INT_PTR CALLBACK GetText(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK CreateBool(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK CreateSphera(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK CreateCube(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK CreateCone(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK SelectRing(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK CreateObjFromSchema(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ChangePosition(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ChangeXYZ(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
int Redraw3D(void);
int RedrawSc(void);
///////////////////////////////////////////////////////////////////////////////////////////////////
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DIPLOM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	Register3DViewerClass(hInstance);
	RegisterSchemaViewerClass(hInstance);
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIPLOM));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIPLOM));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_DIPLOM);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   MainFormSize.x=GetSystemMetrics(SM_CXSCREEN);
   MainFormSize.y=GetSystemMetrics(SM_CYSCREEN);
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
      0, 0, MainFormSize.x, MainFormSize.y, NULL, NULL, hInstance, NULL);
   CreateWindow(_T("static"),_T("Ñïèñîê îáúåêòîâ"), WS_CHILD|WS_VISIBLE,
	   10,20,MainFormSize.x/3-40,20,hWnd,(HMENU)200000,hInstance,NULL);
   cb1=CreateWindow(_T("combobox"),_T("Objects"),WS_CHILD|WS_VISIBLE|WS_BORDER|CBS_DROPDOWN|CBS_AUTOHSCROLL,
	   10,40,MainFormSize.x/3-40,100,hWnd,(HMENU)IDCB1,hInstance,NULL);
   b1=CreateWindow(_T("button"),_T("Óäàëèòü"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_BORDER,
	   10,70,MainFormSize.x/3-40,20,hWnd,(HMENU)IDB1,hInstance,NULL);

   CreateWindow(_T("static"),_T("Ñïèñîê êîíòóðîâ"), WS_CHILD|WS_VISIBLE,
	   10,MainFormSize.y/2,MainFormSize.x/3-40,20,hWnd,(HMENU)200000,hInstance,NULL);
   cb2=CreateWindow(_T("combobox"),_T("Rings"),WS_CHILD|WS_VISIBLE|WS_BORDER|CBS_DROPDOWN|CBS_AUTOHSCROLL,
	   10,MainFormSize.y/2+20,MainFormSize.x/3-40,100,hWnd,(HMENU)IDCB2,hInstance,NULL);
   b2=CreateWindow(_T("button"),_T("Óäàëèòü"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_BORDER,
	   10,MainFormSize.y/2+50,MainFormSize.x/3-40,20,hWnd,(HMENU)IDB2,hInstance,NULL);
   if (!hWnd)
   {
      return FALSE;
   }
   MainWnd=hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, n,i;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_MESH:
			if(m3D_flag==0)
			CreateWindow(_T("3D_VIEWER"), _T("Ðåäàêòîð ñåòîê"), WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			MainFormSize.x/3, 40, MainFormSize.x*2/3, MainFormSize.x*5/9,hWnd,NULL, hInst, NULL);
			break;
		case IDM_CONTUR:
			if(mSchema_flag==0)
			CreateWindow(_T("SCHEMA_VIEWER"), _T("Ðåäàêòîð êîíòóðîâ"), WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			MainFormSize.x/3-20, 60, MainFormSize.x*2/3, MainFormSize.x*5/9,hWnd,NULL, hInst, NULL);
			break;
		case IDM_LOAD:
			if(MessageBoxA(hWnd,"Ñîõðàíèòü òåêóùèé ïðîåêò?","Ñîîáùåíèå",MB_YESNO)==IDYES)
			{
			 strcpy_s(Text,256,"Ñîõðàíèòü");
			 if(DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPENSAVE), hWnd, GetText)==IDOK)
			 
			 Prjct.Save(Text);
			
			}
			strcpy_s(Text,256,"Îòêðûòü");
			if(DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPENSAVE), hWnd, GetText)==IDOK)
			
			Prjct.Open(Text);
			
			n=SendMessage(cb1, CB_GETCOUNT, 0, 0);
		for(i=n-1;i>=0;i--)
			 SendMessage(cb1, CB_DELETESTRING, i, 0);
		n=SendMessage(cb2, CB_GETCOUNT, 0, 0);
		for(i=n-1;i>=0;i--)
			 SendMessage(cb2, CB_DELETESTRING, i, 0);
	 
		for(i=0;i<Prjct.MeshNames.size();i++)
			SendMessageA(cb1,CB_ADDSTRING, 0, (LPARAM)Prjct.MeshNames[i].str);
		SendMessage(cb1,CB_SETCURSEL,(WPARAM)Prjct.SelectedM,0);

		for(i=0;i<Prjct.RingNames.size();i++)
			SendMessageA(cb2,CB_ADDSTRING, 0, (LPARAM)Prjct.RingNames[i].str);
		SendMessage(cb2,CB_SETCURSEL,(WPARAM)Prjct.SelectedR,0);
		InvalidateRect(hWnd,NULL,true);
			break;
		case IDM_SAVE:
			strcpy_s(Text,256,"Ñîõðàíèòü");
			if(DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPENSAVE), hWnd, GetText)==IDOK)
			
			Prjct.Save(Text);
			
			break;
		case IDM_NEW:
			if(MessageBoxA(hWnd,"Ñîõðàíèòü òåêóùèé ïðîåêò?","Ñîîáùåíèå",MB_YESNO)==IDYES)
			{
			 strcpy_s(Text,256,"Ñîõðàíèòü");
			 if(DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPENSAVE), hWnd, GetText)==IDOK)
			 
			 Prjct.Save(Text);
			
			}
			schema.~Schema2d();
			Prjct.Clear();
			n=SendMessage(cb1, CB_GETCOUNT, 0, 0);
		for(i=n-1;i>=0;i--)
			 SendMessage(cb1, CB_DELETESTRING, i, 0);
		n=SendMessage(cb2, CB_GETCOUNT, 0, 0);
		for(i=n-1;i>=0;i--)
			 SendMessage(cb2, CB_DELETESTRING, i, 0);
	 
		for(i=0;i<Prjct.MeshNames.size();i++)
			SendMessageA(cb1,CB_ADDSTRING, 0, (LPARAM)Prjct.MeshNames[i].str);
		SendMessage(cb1,CB_SETCURSEL,(WPARAM)Prjct.SelectedM,0);

		for(i=0;i<Prjct.RingNames.size();i++)
			SendMessageA(cb2,CB_ADDSTRING, 0, (LPARAM)Prjct.RingNames[i].str);
		SendMessage(cb2,CB_SETCURSEL,(WPARAM)Prjct.SelectedR,0);
		InvalidateRect(hWnd,NULL,true);
			break;
		case IDB1://óäàëèòü
			
			n=SendMessageA(cb1,CB_GETCURSEL,0,0);
			GetWindowTextA(cb1,Text,256);
			if(n>-1)
			{
				Prjct.DeleteCurrentMesh();
			
				n=SendMessage(cb1, CB_GETCOUNT, 0, 0);
				for(i=n-1;i>=0;i--)
					SendMessage(cb1, CB_DELETESTRING, i, 0);
	 
				for(i=0;i<Prjct.MeshNames.size();i++)
					SendMessageA(cb1,CB_ADDSTRING, 0, (LPARAM)Prjct.MeshNames[i].str);
				SendMessage(cb1,CB_SETCURSEL,(WPARAM)Prjct.SelectedM,0);
				Redraw3D();
			}
			break;
		case IDB2://óäàëèòü
			
			n=SendMessageA(cb2,CB_GETCURSEL,0,0);
			GetWindowTextA(cb2,Text,256);
			if(n>-1)
			{
				Prjct.DeleteCurrentRing();
				n=SendMessage(cb2, CB_GETCOUNT, 0, 0);
				for(i=n-1;i>=0;i--)
					SendMessage(cb2, CB_DELETESTRING, i, 0);
	 
				for(i=0;i<Prjct.RingNames.size();i++)
					SendMessageA(cb2,CB_ADDSTRING, 0, (LPARAM)Prjct.RingNames[i].str);
				SendMessage(cb2,CB_SETCURSEL,(WPARAM)Prjct.SelectedR,0);
				RedrawSc();
			}
			break;
		case IDCB1:
			if(HIWORD(wParam)==CBN_SELCHANGE)
			{
				
				Prjct.SelectedM=SendMessageA(cb1,CB_GETCURSEL,0,0);
				Redraw3D();
			}
			break;
		case IDCB2:
			if(HIWORD(wParam)==CBN_SELCHANGE)
			{
				Prjct.SelectedR=SendMessageA(cb2,CB_GETCURSEL,0,0);
				RedrawSc();
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		InvalidateRect(hWnd,NULL,true);
		break;
	case WM_USER+1:
		n=SendMessage(cb1, CB_GETCOUNT, 0, 0);
		for(i=n-1;i>=0;i--)
			 SendMessage(cb1, CB_DELETESTRING, i, 0);
		n=SendMessage(cb2, CB_GETCOUNT, 0, 0);
		for(i=n-1;i>=0;i--)
			 SendMessage(cb2, CB_DELETESTRING, i, 0);
	 
		for(i=0;i<Prjct.MeshNames.size();i++)
			SendMessageA(cb1,CB_ADDSTRING, 0, (LPARAM)Prjct.MeshNames[i].str);
		SendMessage(cb1,CB_SETCURSEL,(WPARAM)Prjct.SelectedM,0);

		for(i=0;i<Prjct.RingNames.size();i++)
			SendMessageA(cb2,CB_ADDSTRING, 0, (LPARAM)Prjct.RingNames[i].str);
		SendMessage(cb2,CB_SETCURSEL,(WPARAM)Prjct.SelectedR,0);
		InvalidateRect(hWnd,NULL,true);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Rectangle(hdc,15,95,MainFormSize.x/3-30,MainFormSize.y/2-20);
		Rectangle(hdc,15,MainFormSize.y/2+75,MainFormSize.x/3-30,MainFormSize.y-60);
		//èíôî îá îáúåêòå
		if(Prjct.SelectedM>=0)
		{
			n=sprintf_s(Text,256,"Êîëè÷åñòâî òî÷åê: %d",Prjct.Meshes[Prjct.SelectedM].points.size());
			TextOutA(hdc,20, 110,Text,n);
			n=sprintf_s(Text,256,"Êîëè÷åñòâî ãðàíåé: %d",Prjct.Meshes[Prjct.SelectedM].faces.size());
			TextOutA(hdc,20, 130,Text,n);
		}
		else
		{
			n=sprintf_s(Text,256,"Âûäåëèòå îáúåêò...");
			TextOutA(hdc,20, 110,Text,n);
		}

		if(Prjct.SelectedR>=0)
		{
			if(Prjct.Rings[Prjct.SelectedR].flag==0)n=sprintf_s(Text,256,"Âèä: Ãëàâíûé");
			if(Prjct.Rings[Prjct.SelectedR].flag==1)n=sprintf_s(Text,256,"Âèä: Ñâåðõó");
			if(Prjct.Rings[Prjct.SelectedR].flag==2)n=sprintf_s(Text,256,"Âèä: Ñëåâà");
			TextOutA(hdc,20, MainFormSize.y/2+80,Text,n);
			n=sprintf_s(Text,256,"Êîëè÷åñòâî òî÷åê: %d",Prjct.Rings[Prjct.SelectedR].points.size());
			TextOutA(hdc,20,  MainFormSize.y/2+110,Text,n);
			n=sprintf_s(Text,256,"Êîëè÷åñòâî ðåáåð: %d",Prjct.Rings[Prjct.SelectedR].edges.size());
			TextOutA(hdc,20,  MainFormSize.y/2+140,Text,n);
		}
		else
		{
			n=sprintf_s(Text,256,"Âûäåëèòå îáúåêò...");
			TextOutA(hdc,20,  MainFormSize.y/2+80,Text,n);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_CLOSE:
		if(MessageBoxA(hWnd,"Ñîõðàíèòü òåêóùèé ïðîåêò?","Ñîîáùåíèå",MB_YESNO)==IDYES)
		{
			 strcpy_s(Text,256,"Ñîõðàíèòü");
			 if(DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPENSAVE), hWnd, GetText)==IDOK)
			 Prjct.Save(Text);
		}
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		Prjct.Clear();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

ATOM Register3DViewerClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= m3DViewerProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIPLOM));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName	= _T("3D_VIEWER");
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
LRESULT CALLBACK m3DViewerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	static float width, height;
	static Frame Camera=Frame(vector3f(2,10,200),vector3f(2,10,0),vector3f(0,1,0));
	static float step=10.0, angle=2.0;
	static int ffill=0;
	static int light=0;
	static GLfloat l[4]={1.0,1.0,1.0,1.0};
	static GLfloat l1[4]={10.0,100.0,200.0,1.0};
	static GLfloat ls[4]={1.0,1.0,1.0,1.0};

	int wmId, wmEvent;
	static HDC hdc;
	static HGLRC hrc;
	float AspectRatio;
	int i,n;
	Mesh tempm;
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDHELP:
			DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_BOOL:
			DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_BOOL), hWnd, CreateBool);
		break;
		case ID_SPHERE:
			DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SPHERE), hWnd,CreateSphera);
			break;
		case ID_CONE:
			strcpy_s(Text,256,"Êîíóñ");
			DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONE), hWnd, CreateCone);
			break;
		case ID_CILINDR:
			strcpy_s(Text,256,"Öèëèíäð");
			DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONE), hWnd, CreateCone);
			break;
		case ID_CUBE:
			strcpy_s(Text,256,"Ïàðàëëåëåïèïåä");
			DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CUBE), hWnd, CreateCube);
			break;
		case ID_PIRAMID:
			strcpy_s(Text,256,"Ïèðàìèäà");
			DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CUBE), hWnd, CreateCube);
			break;
		case ID_PLANE:
			strcpy_s(Text,256,"Ïëîñêîñòü");
			DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONE), hWnd, CreateCone);
			break;
		case ID_POS:
			CreateDialog((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_Pos), hWnd,(DLGPROC) ChangePosition);
			break;
		case ID_SAVEOBJECT:
			strcpy_s(Text,256,"Ñîõðàíèòü");
			if(DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPENSAVE), hWnd, GetText)==IDOK)
				if(Prjct.SelectedM>=0)Prjct.Meshes[Prjct.SelectedM].Save(Text);
				else MessageBox(hWnd,_T("Íè îäèí èç îáüåêòîâ íå âûäåëåí"),_T("Îøèáêà ñîõðàíåíèÿ"),0);
			break;
		case ID_LOADOBJECT:
			strcpy_s(Text,256,"Çàãðóçèòü");
			if(DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPENSAVE), hWnd, GetText)==IDOK)
			tempm.Load(Text);
			Prjct.AddMesh(tempm,Text);
			tempm.~Mesh();
			SendMessage(MainWnd,WM_USER+1,(WPARAM)hWnd,0);
			break;
		case ID_DELOBJ:
			if(Prjct.SelectedM==-1)MessageBox(hWnd,_T("Íè îäèí îáúåêò íå âûäåëåí!"),_T("Óäàëåíèå"),0);
			else 
			{
				Prjct.DeleteCurrentMesh();
				InvalidateRect(hWnd,NULL,false);
				SendMessage(MainWnd,WM_USER+1,(WPARAM)hWnd,0);
			}
			break;
		case ID_COORD:
			if(Prjct.SelectedM<0)MessageBox(hWnd,_T("Íè îäèí îáúåêò íå âûäåëåí!"),_T("Îøèáêà"),0);
			else 
			{
				CreateDialog((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hWnd,(DLGPROC) ChangeXYZ);	
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		InvalidateRect(hWnd,NULL,true);
		break;
	case WM_CREATE:
		m3D_flag=1;
		hdc=GetDC(hWnd);
		SetPFD(hdc);
		hrc=wglCreateContext(hdc);
		wglMakeCurrent(hdc,hrc);
		
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glLightfv(GL_LIGHT0,GL_AMBIENT_AND_DIFFUSE,l);
		glLightfv(GL_LIGHT0,GL_SPECULAR,ls);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
		
		
		break;
	case WM_SIZE:
		height=HIWORD(lParam);
		width=LOWORD(lParam);
		if(height==0.0)height=1.0;
		glViewport(0,0,(GLsizei)width,(GLsizei)height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		AspectRatio=width/height;
		gluPerspective(60.0,AspectRatio,0.5,9000.0); 
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		InvalidateRect(hWnd,NULL,false);
		break;
	case WM_CHAR:
		//
		//if((TCHAR)wParam==(TCHAR)'l')light=1-light;
		if((TCHAR)wParam==(TCHAR)'f')ffill=1-ffill;
		if((TCHAR)wParam==(TCHAR)'a')Camera.GoU(-step);
		if((TCHAR)wParam==(TCHAR)'d')Camera.GoU(step);
		if((TCHAR)wParam==(TCHAR)'s')Camera.GoN(-step);
		if((TCHAR)wParam==(TCHAR)'w')Camera.GoN(step);
		if((TCHAR)wParam==(TCHAR)'7')Camera.GoV(-step);
		if((TCHAR)wParam==(TCHAR)'9')Camera.GoV(step);
		if((TCHAR)wParam==(TCHAR)'q')Camera.RotateV(angle);
		if((TCHAR)wParam==(TCHAR)'e')Camera.RotateV(-angle);

		if((TCHAR)wParam==(TCHAR)'8')Camera.RotateU(angle);
		if((TCHAR)wParam==(TCHAR)'5')Camera.RotateU(-angle);

		if((TCHAR)wParam==(TCHAR)'4')Camera.RotateN(angle);
		if((TCHAR)wParam==(TCHAR)'6')Camera.RotateN(-angle);
		//
		InvalidateRect(hWnd,NULL,false);
		break;
	case WM_PAINT:
		ValidateRect(hWnd,NULL);
		////////////////////////
		float m[16];
		if(ffill)
		{
				glEnable(GL_LIGHTING);
		}
		else
			glDisable(GL_LIGHTING);
		
		glClearColor(1.0,1.0,1.0,1.0);
		glShadeModel(GL_FLAT);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
	
		Camera.GetCameraMatrix(m);
		l1[0]=Camera.Position().x;
		l1[1]=Camera.Position().y;
		l1[2]=Camera.Position().z;
		glLightfv(GL_LIGHT0,GL_POSITION,l1);
		glLoadMatrixf(m);
		if(Prjct.SelectedM>=0)RenderMesh(Prjct.Meshes[Prjct.SelectedM],ffill,vector3f(0.2,0.75,0.2));
		if(ffill)
		{
			//glDisable(GL_LIGHTING);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		}
		for(i=0;i<Prjct.Meshes.size();i++)
		{
			if(i!=Prjct.SelectedM)
				RenderMesh(Prjct.Meshes[i],ffill,vector3f(0.2,0.2,0.4));
		}
		if(ffill)
		{
			glDisable(GL_BLEND);
			//glEnable(GL_LIGHTING);
		}
		//îñè
		glBegin(GL_LINES);
		glColor3f(0.5,0.5,0.5);
		glVertex3f(0,0,0);
		glVertex3f(1000,0,0);
		glVertex3f(0,0,0);
		glVertex3f(0,1000,0);
		glVertex3f(0,0,0);
		glVertex3f(0,0,1000);
		glEnd();
		SwapBuffers(hdc);
		break;
	case WM_DESTROY:
		m3D_flag=0;
		wglMakeCurrent(hdc,NULL);
		wglDeleteContext(hrc);
		ReleaseDC(hWnd,hdc);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int SetPFD(HDC hdc)
{
	int nPF;								//pixel format number
	static PIXELFORMATDESCRIPTOR pfd={
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	32,
	0,0,0,0,0,0,
	0,0,
	0,0,0,0,0,
	16,
	0,0,0,0,0,0,0};
	nPF=ChoosePixelFormat(hdc,&pfd);
	SetPixelFormat(hdc,nPF,&pfd);
	
	return 1;
}

void RenderMesh(Mesh m, int light, vector3f clr)
{
	int i;
	if(light==1)
	{
		glPolygonMode(GL_FRONT,GL_FILL);
		glPolygonMode(GL_BACK,GL_FILL);
	}
	if(light==0)
	{
		glPolygonMode(GL_FRONT,GL_LINE);
		glPolygonMode(GL_BACK,GL_LINE);
	}
	glBegin(GL_TRIANGLES);
	if(light==1)glColor4f(clr.x,clr.y,clr.z,0.5);
	else
	glColor3f(clr.x,clr.y,clr.z);
	for(i=0;i<m.faces.size();i++)
	{
		if(light)glNormal3f(m.normals[i].x,m.normals[i].y,m.normals[i].z);
		glVertex3f( m.points[m.faces[i].a].x,
					m.points[m.faces[i].a].y,
					m.points[m.faces[i].a].z);
		glVertex3f( m.points[m.faces[i].b].x,
					m.points[m.faces[i].b].y,
					m.points[m.faces[i].b].z);
		glVertex3f( m.points[m.faces[i].c].x,
					m.points[m.faces[i].c].y,
					m.points[m.faces[i].c].z);
	}
	glEnd();
	
	return;
}
ATOM RegisterSchemaViewerClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= SchemeViewerProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIPLOM));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU2);
	wcex.lpszClassName	= _T("SCHEMA_VIEWER");
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
LRESULT CALLBACK SchemeViewerProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static COLORREF gp;
	static Mesh m;
	char str[256];
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	static RECT r;
	static vector2f nullpos, oldpos, downpos, p, a, b;
	
	static float scale=1;
	static int x=0, y=0,i ,j ,k;
	static Ring ring;
	static line2d l;
	static int SELECT_MODE=0;
	static vector3f_array r3d;
	static vector3f norm; 
	
	HDC hdc;
	
	static HPEN normalp, selectp;
	switch (msg)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDS_CLOSE:
			DestroyWindow(hWnd);
			break;
		case IDS_OPEN:
			strcpy_s(Text,256,"Îòêðûòü");
			if(DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPENSAVE), hWnd, GetText)==IDOK)
			{
				schema.Read(Text);
				schema.FindAndApplyO();
				schema.GetBoundRect(l);
				mSchema_flag=1;
				minsch=schema.minxyz;
				maxsch=schema.maxxyz;
				GetClientRect(hWnd,&r);
				nullpos=vector2f(r.right/2,r.bottom/2);
				scale=(float)MAX2(r.bottom,r.right)/(float)MAX2(l.b.x+10,l.b.y+10);
			}
			break;
		case IDS_EXTRUDE:
			if(DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_EXTRUDE), hWnd, CreateObjFromSchema)==IDOK)
			{
				ring.clear();
				ring=Prjct.Rings[Prjct.SelectedR];
				r3d.clear();
				m.~Mesh();
				for(i=0;i<ring.points.size();i++)
				{
					if(ring.flag==0)
						r3d.push_back(vector3f(ring.points[i].x,ring.points[i].y,minsch.z));
					if(ring.flag==1)
						r3d.push_back(vector3f(ring.points[i].x,minsch.y,ring.points[i].y));
					if(ring.flag==2)
						r3d.push_back(vector3f(minsch.x,ring.points[i].y,ring.points[i].x));
				}
				if(ring.flag==0)norm=vector3f(0,0,maxsch.z-minsch.z);
				if(ring.flag==1)norm=vector3f(0,maxsch.y-minsch.y,0);
				if(ring.flag==2)norm=vector3f(maxsch.x-minsch.x,0,0);

				ExtrudeRing(&m,ring.edges,r3d,norm);
				Prjct.AddMesh(m,Text);			
				SendMessage(MainWnd,WM_USER+1,(WPARAM)hWnd,0);
			}
			break;
		case IDS_SELECT:
			strcpy_s(Text,256,"Âûáåðèòå êîíòóð");
			if(DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SELECT), hWnd, SelectRing)==IDOK)
			{
				ring.clear();
				ring=Prjct.Rings[Prjct.SelectedR];
			}
			
			break;
		case IDS_DELETE:
			strcpy_s(Text,256,"Óäàëåíèå êîíòóðà");
			if(DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SELECT), hWnd, SelectRing)==IDOK)
			{
				Prjct.DeleteCurrentRing();
				ring.clear();
			}
			
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		InvalidateRect(hWnd,NULL,true);
		break;
	case WM_CREATE:
		if(flagsch==1)
		{
			flagsch=0;
			schema.GetBoundRect(l);
			mSchema_flag=1;
			minsch=schema.minxyz;
			maxsch=schema.maxxyz;
			
		}
		normalp=CreatePen(PS_SOLID,3,RGB(0,0,0));
		selectp=CreatePen(PS_SOLID,3,RGB(100,200,100));
		
	case WM_SIZE:
		GetClientRect(hWnd,&r);
		nullpos=vector2f(r.right/2,r.bottom/2);
		scale=(float)MAX2(r.bottom,r.right)/(float)MAX2(l.b.x+10,l.b.y+10);
		break;
	case WM_LBUTTONDOWN:
		oldpos=nullpos;
		downpos=vector2f((float)LOWORD(lParam),(float)HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		x=LOWORD(lParam);
		y=HIWORD(lParam);
		InvalidateRect(hWnd,NULL,true);
		tryselect=1;
		////
		break;
	case WM_KEYDOWN:
		SELECT_MODE=1-SELECT_MODE;
		if(SELECT_MODE)
		{
			ring.clear();
			Prjct.SelectedR=-1;
			sprintf(str,"Ðåäàêòîð êîíòóðîâ - âûäåëåíèå êîíòóðà");
			SetWindowTextA(hWnd,str);
			InvalidateRect(hWnd,NULL,true);
		}
		else
		{
			sprintf(str,"Ðåäàêòîð êîíòóðîâ");
			SetWindowTextA(hWnd,str);
			if(ring.edges.size()>0)
			{
				Prjct.SelectedR=Prjct.Rings.size();
				if(ring.flag==0)sprintf_s(Text,256,"¹%d ðåáåð: %d ãëàâíûé âèä",Prjct.SelectedR,ring.edges.size());
				if(ring.flag==1)sprintf_s(Text,256,"¹%d ðåáåð: %d âèä ñâåðõó",Prjct.SelectedR,ring.edges.size());
				if(ring.flag==2)sprintf_s(Text,256,"¹%d ðåáåð: %d âèä ñëåâà",Prjct.SelectedR,ring.edges.size());
				Prjct.AddRing(ring,Text);
				SendMessage(MainWnd,WM_USER+1,(WPARAM)hWnd,0);
				InvalidateRect(hWnd,NULL,true);
			}
		}
		break;
	case WM_MOUSEMOVE:
		if(wParam==MK_LBUTTON)
		{
			
				nullpos.x=(oldpos.x+((float)LOWORD(lParam)-downpos.x));
				nullpos.y=(oldpos.y+((float)HIWORD(lParam)-downpos.y));
				InvalidateRect(hWnd,NULL,true);
		}
		break;
		
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SelectObject(hdc,(HGDIOBJ)normalp);
		for(i=0;i<schema.eFront.size();i++)
		{
			p=schema.pFront[schema.eFront[i].start];
			MoveToEx(hdc,nullpos.x-p.x*scale,nullpos.y-p.y*scale,NULL);
			p=schema.pFront[schema.eFront[i].end];
			LineTo(hdc,nullpos.x-p.x*scale,nullpos.y-p.y*scale);
		}
		for(i=0;i<schema.eTop.size();i++)
		{
			p=schema.pTop[schema.eTop[i].start];
			MoveToEx(hdc,nullpos.x-p.x*scale,p.y*scale+nullpos.y,NULL);
			p=schema.pTop[schema.eTop[i].end];
			LineTo(hdc,nullpos.x-p.x*scale,p.y*scale+nullpos.y);
		}
		for(i=0;i<schema.eLeft.size();i++)
		{
			p=schema.pLeft[schema.eLeft[i].start];
			MoveToEx(hdc,nullpos.x+p.x*scale,nullpos.y-p.y*scale,NULL);
			p=schema.pLeft[schema.eLeft[i].end];
			LineTo(hdc,nullpos.x+p.x*scale,nullpos.y-p.y*scale);
		}
		SelectObject(hdc,(HGDIOBJ)selectp);
		if(SELECT_MODE==0)
		{
			ring.clear();if(Prjct.SelectedR>=0)ring=Prjct.Rings[Prjct.SelectedR];
		}
		for(i=0;i<ring.edges.size();i++)
		{
			if(ring.flag==0)//FRONT
			{
				a=vector2f(nullpos.x-ring.points[ring.edges[i].start].x*scale,nullpos.y-ring.points[ring.edges[i].start].y*scale);
				b=vector2f(nullpos.x-ring.points[ring.edges[i].end].x*scale,nullpos.y-ring.points[ring.edges[i].end].y*scale);
			}
			if(ring.flag==1)//TOP
			{
				a=vector2f(nullpos.x-ring.points[ring.edges[i].start].x*scale,ring.points[ring.edges[i].start].y*scale+nullpos.y);
				b=vector2f(nullpos.x-ring.points[ring.edges[i].end].x*scale,ring.points[ring.edges[i].end].y*scale+nullpos.y);
			}
			if(ring.flag==2)//LEFT
			{
				a=vector2f(nullpos.x+ring.points[ring.edges[i].start].x*scale,nullpos.y-ring.points[ring.edges[i].start].y*scale);
				b=vector2f(nullpos.x+ring.points[ring.edges[i].end].x*scale,nullpos.y-ring.points[ring.edges[i].end].y*scale);
			}
			MoveToEx(hdc,a.x,a.y,NULL);
			LineTo(hdc,b.x,b.y);
		}
		//îïðåäåëåíèå âûäåëåííîãî ðåáðà
		if(tryselect && SELECT_MODE)
		{
			gp=GetPixel(hdc,x,y);
			if(gp==RGB(0,0,0))
			{
				
				for(i=0;i<schema.eFront.size();i++)
				{
					a=schema.pFront[schema.eFront[i].start];
					b=schema.pFront[schema.eFront[i].end];
					if(IsPixelInEdge(x,y,0,a,b,nullpos,scale))
					{
						
						if(ring.edges.size()==0)
							ring.flag=0;
						if(ring.flag!=0)MessageBox(NULL,NULL,NULL,0);
						else ring.addedge(a,b);
						break;
					}
				}
				for(i=0;i<schema.eTop.size();i++)
				{
					a=schema.pTop[schema.eTop[i].start];
					b=schema.pTop[schema.eTop[i].end];
					if(IsPixelInEdge(x,y,1,a,b,nullpos,scale))
					{
						
						if(ring.edges.size()==0)
							ring.flag=1;
						if(ring.flag!=1)MessageBox(NULL,NULL,NULL,0);
						else ring.addedge(a,b);
						break;
					}
				}
				for(i=0;i<schema.eLeft.size();i++)
				{
					a=schema.pLeft[schema.eLeft[i].start];
					b=schema.pLeft[schema.eLeft[i].end];
					if(IsPixelInEdge(x,y,2,a,b,nullpos,scale))
					{
						
						if(ring.edges.size()==0)
							ring.flag=2;
						if(ring.flag!=2)MessageBox(NULL,NULL,NULL,0);
						else ring.addedge(a,b);
						break;
					}
				}
			}
			tryselect=0;
			InvalidateRect(hWnd,NULL,true);
		}
		EndPaint(hWnd, &ps);
		x=y=-1;
		break;
	case WM_MOUSEWHEEL:
		if(GET_WHEEL_DELTA_WPARAM(wParam)>0.0)
			scale+=0.5;
		else
			scale-=0.5;
		InvalidateRect(hWnd,NULL,true);
		break;
	//case WM_CLOSE:
	//	/*i=MessageBoxA(hWnd,"Âû óâåðåíû?","Çàêðûòü",MB_YESNO);
	//	if(i==IDYES)DestroyWindow(hWnd);*/
	//	break;
	case WM_DESTROY:
		mSchema_flag=0;
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int IsPixelInEdge(float x, float y, int viewflag, vector2f a, vector2f b, vector2f _0pos, float scale)
{
	vector2f A, B, P, ab,ap,pb;
	int rez;
	P=vector2f(x,y);
	//////
	if(viewflag==0)//FRONT
	{
		A=vector2f(_0pos.x-a.x*scale,_0pos.y-a.y*scale);
		B=vector2f(_0pos.x-b.x*scale,_0pos.y-b.y*scale);
	}
	
	if(viewflag==1)//TOP
	{
		A=vector2f(_0pos.x-a.x*scale,a.y*scale+_0pos.y);
		B=vector2f(_0pos.x-b.x*scale,b.y*scale+_0pos.y);
	}
	
	if(viewflag==2)//LEFT
	{
		A=vector2f(_0pos.x+a.x*scale,_0pos.y-a.y*scale);
		B=vector2f(_0pos.x+b.x*scale,_0pos.y-b.y*scale);
	}

	ab=B-A;
	ap=P-A;
	pb=P-B;
	rez=0;
	if(fabs(ab.Length()-ap.Length()-pb.Length())<0.1)rez=1;
	return rez;
}

INT_PTR CALLBACK GetText(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowTextA(hDlg,Text);
		return (INT_PTR)TRUE;
		
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			if(LOWORD(wParam) == IDOK)GetDlgItemTextA(hDlg,IDC_EDIT1,Text,256);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)FALSE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK CreateBool(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i; char str[32];
	Mesh a, b, c;
	BooleanMesh bm;
	int an, bn, on;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		for(i=0;i<Prjct.MeshNames.size();i++)
		{
			SendMessageA(GetDlgItem(hDlg,IDC_COMBO1),CB_ADDSTRING, 0, (LPARAM)Prjct.MeshNames[i].str);
			SendMessageA(GetDlgItem(hDlg,IDC_COMBO2),CB_ADDSTRING, 0, (LPARAM)Prjct.MeshNames[i].str);
		}
		SendMessage(GetDlgItem(hDlg,IDC_COMBO1),CB_SETCURSEL,0,0);
		SendMessage(GetDlgItem(hDlg,IDC_COMBO2),CB_SETCURSEL,0,0);
		
		sprintf_s(str,32,"Îáúåäèíåíèå");
		SendMessageA(GetDlgItem(hDlg,IDC_COMBO3),CB_ADDSTRING, 0, (LPARAM)str);
		sprintf_s(str,32,"Ïåðåñå÷åíèå");
		SendMessageA(GetDlgItem(hDlg,IDC_COMBO3),CB_ADDSTRING, 0, (LPARAM)str);
		sprintf_s(str,32,"Âû÷èòàíèå");
		SendMessageA(GetDlgItem(hDlg,IDC_COMBO3),CB_ADDSTRING, 0, (LPARAM)str);
		SendMessage(GetDlgItem(hDlg,IDC_COMBO3),CB_SETCURSEL,0,0);
		
		return (INT_PTR)TRUE;
		
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			
			if(LOWORD(wParam) == IDOK)
			{
				an=SendMessage(GetDlgItem(hDlg,IDC_COMBO1),CB_GETCURSEL,0,0);
				bn=SendMessage(GetDlgItem(hDlg,IDC_COMBO2),CB_GETCURSEL,0,0);
				on=SendMessage(GetDlgItem(hDlg,IDC_COMBO3),CB_GETCURSEL,0,0);
				if(an==bn)
				{
					MessageBox(hDlg,_T("Íåëüçÿ ñîçäàòü áóëåâñêèé îáúåêò èç îäèíàêîâûõ îïåðàíäîâ!"),_T("Âíèìàíèå"),0);
					break;
				}
				else
				{
					a=Prjct.Meshes[an];
					b=Prjct.Meshes[bn];
					a.CalcNormals();
					b.CalcNormals();
					
					bm.Set(a,b);
					
					if(on==0)c=bm.GetMesh(UNION);
					if(on==1)c=bm.GetMesh(INTERSECTION);
					if(on==2)c=bm.GetMesh(SUBSTRUCTION);
				
					c.CalcNormals();
					GetDlgItemTextA(hDlg,IDC_EDIT1,str,32);
					SendMessage(MainWnd,WM_USER+1,(WPARAM)hDlg,0);
					Prjct.AddMesh(c,str);
					Prjct.SelectedM=Prjct.Meshes.size()-1;
					SendMessage(MainWnd,WM_USER+1,(WPARAM)hDlg,0);
				}
			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK CreateSphera(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int nr,nh;
	char str[32];
	Mesh a;
	float r;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemTextA(hDlg,IDC_EDIT1,"50");		
		SetDlgItemTextA(hDlg,IDC_EDIT2,"6");		
		SetDlgItemTextA(hDlg,IDC_EDIT3,"6");
		SetDlgItemTextA(hDlg,IDC_EDIT4,"Ñôåðà");		
		return (INT_PTR)TRUE;
		
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			
			if(LOWORD(wParam) == IDOK)
			{
				GetDlgItemTextA(hDlg,IDC_EDIT1,str,32);
				sscanf_s(str,"%f",&r);
				GetDlgItemTextA(hDlg,IDC_EDIT2,str,32);	
				sscanf_s(str,"%d",&nr);
				GetDlgItemTextA(hDlg,IDC_EDIT3,str,32);
				sscanf_s(str,"%d",&nh);
				GetDlgItemTextA(hDlg,IDC_EDIT4,str,32);
				Sphere(&a,r,nh,nr);
				Prjct.AddMesh(a,str);
				Prjct.SelectedM=Prjct.Meshes.size()-1;
				SendMessage(MainWnd,WM_USER+1,(WPARAM)hDlg,0);
				
			}
			EndDialog(hDlg, LOWORD(wParam));
			
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK CreateCube(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int nw,nl,nh;
	char str[32];
	Mesh a;
	float w,h,l;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowTextA(hDlg,Text);
		SetDlgItemTextA(hDlg,IDC_EDIT1,"40");		
		SetDlgItemTextA(hDlg,IDC_EDIT2,"40");		
		SetDlgItemTextA(hDlg,IDC_EDIT3,"40");
		SetDlgItemTextA(hDlg,IDC_EDIT4,"1");		
		SetDlgItemTextA(hDlg,IDC_EDIT5,"1");		
		SetDlgItemTextA(hDlg,IDC_EDIT6,"1");
		SetDlgItemTextA(hDlg,IDC_EDIT7,Text);		
		return (INT_PTR)TRUE;
		
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			
			if(LOWORD(wParam) == IDOK)
			{
				GetDlgItemTextA(hDlg,IDC_EDIT1,str,32);
				sscanf_s(str,"%f",&l);
				GetDlgItemTextA(hDlg,IDC_EDIT2,str,32);	
				sscanf_s(str,"%f",&w);
				GetDlgItemTextA(hDlg,IDC_EDIT3,str,32);
				sscanf_s(str,"%f",&h);
				GetDlgItemTextA(hDlg,IDC_EDIT4,str,32);
				sscanf_s(str,"%d",&nl);
				GetDlgItemTextA(hDlg,IDC_EDIT5,str,32);	
				sscanf_s(str,"%d",&nw);
				GetDlgItemTextA(hDlg,IDC_EDIT6,str,32);
				sscanf_s(str,"%d",&nh);
				GetDlgItemTextA(hDlg,IDC_EDIT7,str,32);
				if(strcmp(Text,"Ïàðàëëåëåïèïåä")==0)Cube(&a,w,l,h,nw,nl,nh);
				if(strcmp(Text,"Ïèðàìèäà")==0)Piramid(&a,h,w,l,nw,nl,nh);
				Prjct.AddMesh(a,str);
				Prjct.SelectedM=Prjct.Meshes.size()-1;
				SendMessage(MainWnd,WM_USER+1,(WPARAM)hDlg,0);
				
			}
			EndDialog(hDlg, LOWORD(wParam));
			
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK CreateCone(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int nr,nh;
	char str[32];
	Mesh a;
	float r,h;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowTextA(hDlg,Text);
		SetDlgItemTextA(hDlg,IDC_EDIT1,"40");		
		SetDlgItemTextA(hDlg,IDC_EDIT2,"40");		
		
		SetDlgItemTextA(hDlg,IDC_EDIT3,"6");		
		SetDlgItemTextA(hDlg,IDC_EDIT4,"1");		
		SetDlgItemTextA(hDlg,IDC_EDIT5,Text);		
		return (INT_PTR)TRUE;
		
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			
			if(LOWORD(wParam) == IDOK)
			{
				GetDlgItemTextA(hDlg,IDC_EDIT1,str,32);
				sscanf_s(str,"%f",&r);
				GetDlgItemTextA(hDlg,IDC_EDIT2,str,32);	
				sscanf_s(str,"%f",&h);
				GetDlgItemTextA(hDlg,IDC_EDIT3,str,32);
				sscanf_s(str,"%d",&nr);
				GetDlgItemTextA(hDlg,IDC_EDIT4,str,32);	
				sscanf_s(str,"%d",&nh);
				GetDlgItemTextA(hDlg,IDC_EDIT5,str,32);

				if(strcmp(Text,"Êîíóñ")==0)Cone(&a,r,h,nr,nh);
				if(strcmp(Text,"Öèëèíäð")==0)Cilindr(&a,r,h,nr,nh);
				if(strcmp(Text,"Ïëîñêîñòü")==0)Plane(&a,r,h,nr,nh);
				Prjct.AddMesh(a,str);
				SendMessage(MainWnd,WM_USER+1,(WPARAM)hDlg,0);
				Prjct.SelectedM=Prjct.Meshes.size()-1;
				
			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK CreateObjFromSchema(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	char str[256];
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		for(i=0;i<Prjct.Rings.size();i++)
		{
			SendMessageA(GetDlgItem(hDlg,IDC_COMBO1),CB_ADDSTRING, 0, (LPARAM)Prjct.RingNames[i].str);
		}
		SendMessageA(GetDlgItem(hDlg,IDC_COMBO1),CB_SETCURSEL,0,0);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			Prjct.SelectedR=SendMessageA(GetDlgItem(hDlg,IDC_COMBO1),CB_GETCURSEL,0,0);
			GetDlgItemTextA(hDlg,IDC_EDIT1,Text,256);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDC_COMBO1)
		{
			if(HIWORD(wParam)==CBN_SELCHANGE)
			{
				Prjct.SelectedR=SendMessageA(GetDlgItem(hDlg,IDC_COMBO1),CB_GETCURSEL,0,0);
				SetWindowTextA(GetDlgItem(hDlg,IDC_EDIT1),Prjct.RingNames[Prjct.SelectedR].str);
			}
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}



INT_PTR CALLBACK SelectRing(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	char str[256];
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowTextA(hDlg,Text);
		for(i=0;i<Prjct.Rings.size();i++)
		{
			SendMessageA(GetDlgItem(hDlg,IDC_COMBO1),CB_ADDSTRING, 0, (LPARAM)Prjct.RingNames[i].str);
		}
		SendMessageA(GetDlgItem(hDlg,IDC_COMBO1),CB_SETCURSEL,0,0);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			Prjct.SelectedR=SendMessageA(GetDlgItem(hDlg,IDC_COMBO1),CB_GETCURSEL,0,0);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK ChangePosition(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	vector3f O;
	vector3f D,R,M, tmp;
	float alpha;
	int i;
	char str[256];
	
	float Mtrx[4][4];
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		{
			
			SetDlgItemTextA(hDlg,IDC_EDIT1,"0");
			SetDlgItemTextA(hDlg,IDC_EDIT2,"0");
			SetDlgItemTextA(hDlg,IDC_EDIT3,"0");

			SetDlgItemTextA(hDlg,IDC_EDIT4,"0");
			SetDlgItemTextA(hDlg,IDC_EDIT5,"0");
			SetDlgItemTextA(hDlg,IDC_EDIT6,"0");

			SetDlgItemTextA(hDlg,IDC_EDIT7,"0");
			SetDlgItemTextA(hDlg,IDC_EDIT8,"0");
			SetDlgItemTextA(hDlg,IDC_EDIT9,"0");
			SetDlgItemTextA(hDlg,IDC_EDIT10,"0");

			SetDlgItemTextA(hDlg,IDC_EDIT11,"1");
			SetDlgItemTextA(hDlg,IDC_EDIT12,"1");
			SetDlgItemTextA(hDlg,IDC_EDIT13,"1");
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1)
		{
			GetDlgItemTextA(hDlg,IDC_EDIT1,str,256);sscanf(str,"%f",&O.x);
			GetDlgItemTextA(hDlg,IDC_EDIT2,str,256);sscanf(str,"%f",&O.y);
			GetDlgItemTextA(hDlg,IDC_EDIT3,str,256);sscanf(str,"%f",&O.z);

			GetDlgItemTextA(hDlg,IDC_EDIT4,str,256);sscanf(str,"%f",&D.x);
			GetDlgItemTextA(hDlg,IDC_EDIT5,str,256);sscanf(str,"%f",&D.y);
			GetDlgItemTextA(hDlg,IDC_EDIT6,str,256);sscanf(str,"%f",&D.z);
			if(Prjct.SelectedM>=0)
			{
				for(i=0;i<Prjct.Meshes[Prjct.SelectedM].points.size();i++)
					Prjct.Meshes[Prjct.SelectedM].points[i]=Prjct.Meshes[Prjct.SelectedM].points[i]-O+D+O;
				InvalidateRect(GetParent(hDlg),NULL,false);
			}
			else
				MessageBoxA(hDlg,"Âûäåëèòå êàêîé íèþóäü îáúåêò","Ñîîáùåíèå",0);
			return (INT_PTR)TRUE;
		}
		if( LOWORD(wParam) == IDC_BUTTON2)
		{
			GetDlgItemTextA(hDlg,IDC_EDIT1,str,256);sscanf(str,"%f",&O.x);
			GetDlgItemTextA(hDlg,IDC_EDIT2,str,256);sscanf(str,"%f",&O.y);
			GetDlgItemTextA(hDlg,IDC_EDIT3,str,256);sscanf(str,"%f",&O.z);

			GetDlgItemTextA(hDlg,IDC_EDIT7,str,256);sscanf(str,"%f",&R.x);
			GetDlgItemTextA(hDlg,IDC_EDIT8,str,256);sscanf(str,"%f",&R.y);
			GetDlgItemTextA(hDlg,IDC_EDIT9,str,256);sscanf(str,"%f",&R.z);
			GetDlgItemTextA(hDlg,IDC_EDIT10,str,256);sscanf(str,"%f",&alpha);
			R.Normalize();
			LoadRotateMatrix44(Mtrx,alpha,R.x,R.y,R.z);
			if(Prjct.SelectedM>=0)
			{
				for(i=0;i<Prjct.Meshes[Prjct.SelectedM].points.size();i++)
				{
					Prjct.Meshes[Prjct.SelectedM].points[i]=Prjct.Meshes[Prjct.SelectedM].points[i]-O;
					Matrix44XVector3f(Prjct.Meshes[Prjct.SelectedM].points[i],Mtrx,tmp);
					Prjct.Meshes[Prjct.SelectedM].points[i]=tmp+O;
				}
				Prjct.Meshes[Prjct.SelectedM].normals.clear();
				Prjct.Meshes[Prjct.SelectedM].CalcNormals();
				InvalidateRect(GetParent(hDlg),NULL,false);
			}
			else
				MessageBoxA(hDlg,"Âûäåëèòå êàêîé íèþóäü îáúåêò","Ñîîáùåíèå",0);
			return (INT_PTR)TRUE;
		}
		if( LOWORD(wParam) == IDC_BUTTON3)
		{
			GetDlgItemTextA(hDlg,IDC_EDIT1,str,256);sscanf(str,"%f",&O.x);
			GetDlgItemTextA(hDlg,IDC_EDIT2,str,256);sscanf(str,"%f",&O.y);
			GetDlgItemTextA(hDlg,IDC_EDIT3,str,256);sscanf(str,"%f",&O.z);

			GetDlgItemTextA(hDlg,IDC_EDIT11,str,256);sscanf(str,"%f",&M.x);
			GetDlgItemTextA(hDlg,IDC_EDIT12,str,256);sscanf(str,"%f",&M.y);
			GetDlgItemTextA(hDlg,IDC_EDIT13,str,256);sscanf(str,"%f",&M.z);
			
			LoadScaleMatrix44(Mtrx,M.x,M.y,M.z);
			if(Prjct.SelectedM>=0)
			{
				for(i=0;i<Prjct.Meshes[Prjct.SelectedM].points.size();i++)
				{
					Prjct.Meshes[Prjct.SelectedM].points[i]=Prjct.Meshes[Prjct.SelectedM].points[i]-O;
					Matrix44XVector3f(Prjct.Meshes[Prjct.SelectedM].points[i],Mtrx,tmp);
					Prjct.Meshes[Prjct.SelectedM].points[i]=tmp+O;
				}
				InvalidateRect(GetParent(hDlg),NULL,false);
			}
			else
				MessageBoxA(hDlg,"Âûäåëèòå êàêîé íèþóäü îáúåêò","Ñîîáùåíèå",0);
			return (INT_PTR)TRUE;
		}
		if(LOWORD(wParam)==IDCANCEL)
		{
			EndDialog(hDlg,true);
			return true;
		}
		break;
	}
	return (INT_PTR)FALSE;
};

INT_PTR CALLBACK ChangeXYZ(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	vector3f XYZ;
	
	int i;
	char str[256];
	
	float Mtrx[4][4];
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		{
			nnn=0;
			SetDlgItemTextA(hDlg,IDC_EDIT1,"0");
			SetDlgItemTextA(hDlg,IDC_EDIT2,"0");
			SetDlgItemTextA(hDlg,IDC_EDIT3,"0");

			SetDlgItemTextA(hDlg,IDC_EDIT4,"0");
			flagpoint=1;
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1)
		{
			if(nnn<Prjct.Meshes[Prjct.SelectedM].points.size()-1)
			{
				nnn++;
				sprintf_s(str,256,"%d",nnn);
				SetDlgItemTextA(hDlg,IDC_EDIT4,str);

				sprintf_s(str,256,"%f",Prjct.Meshes[Prjct.SelectedM].points[nnn].x);
				SetDlgItemTextA(hDlg,IDC_EDIT1,str);

				sprintf_s(str,256,"%f",Prjct.Meshes[Prjct.SelectedM].points[nnn].y);
				SetDlgItemTextA(hDlg,IDC_EDIT2,str);

				sprintf_s(str,256,"%f",Prjct.Meshes[Prjct.SelectedM].points[nnn].z);
				SetDlgItemTextA(hDlg,IDC_EDIT3,str);


			}
			return (INT_PTR)TRUE;
		}
		if( LOWORD(wParam) == IDC_BUTTON2)
		{
			if(nnn>0)
			{
				nnn--;
				sprintf_s(str,256,"%d",nnn);
				SetDlgItemTextA(hDlg,IDC_EDIT4,str);

				sprintf_s(str,256,"%f",Prjct.Meshes[Prjct.SelectedM].points[nnn].x);
				SetDlgItemTextA(hDlg,IDC_EDIT1,str);

				sprintf_s(str,256,"%f",Prjct.Meshes[Prjct.SelectedM].points[nnn].y);
				SetDlgItemTextA(hDlg,IDC_EDIT2,str);

				sprintf_s(str,256,"%f",Prjct.Meshes[Prjct.SelectedM].points[nnn].z);
				SetDlgItemTextA(hDlg,IDC_EDIT3,str);
				InvalidateRect(GetParent(hDlg),NULL,false);
			}
			return (INT_PTR)TRUE;
		}
		if( LOWORD(wParam) == IDC_BUTTON3)
		{
			GetDlgItemTextA(hDlg,IDC_EDIT4,str,256);
			sscanf_s(str,"%d",&nnn);
			if(nnn>Prjct.Meshes[Prjct.SelectedM].points.size()-1 ||nnn <0)
			{
				nnn=0;
				sprintf_s(str,256,"%d",nnn);
				SetDlgItemTextA(hDlg,IDC_EDIT4,str);
			}
			sprintf_s(str,256,"%f",Prjct.Meshes[Prjct.SelectedM].points[nnn].x);
			SetDlgItemTextA(hDlg,IDC_EDIT1,str);

			sprintf_s(str,256,"%f",Prjct.Meshes[Prjct.SelectedM].points[nnn].y);
			SetDlgItemTextA(hDlg,IDC_EDIT2,str);

			sprintf_s(str,256,"%f",Prjct.Meshes[Prjct.SelectedM].points[nnn].z);
			SetDlgItemTextA(hDlg,IDC_EDIT3,str);
			InvalidateRect(GetParent(hDlg),NULL,false);
			return (INT_PTR)TRUE;
		}
		if(LOWORD(wParam)==IDOK)
		{
			GetDlgItemTextA(hDlg,IDC_EDIT4,str,256);
			sscanf_s(str,"%d",&nnn);
			GetDlgItemTextA(hDlg,IDC_EDIT1,str,256);
			sscanf_s(str,"%f",&XYZ.x);
			GetDlgItemTextA(hDlg,IDC_EDIT2,str,256);
			sscanf_s(str,"%f",&XYZ.y);
			GetDlgItemTextA(hDlg,IDC_EDIT3,str,256);
			sscanf_s(str,"%f",&XYZ.z);
			Prjct.Meshes[Prjct.SelectedM].points[nnn]=XYZ;
			InvalidateRect(GetParent(hDlg),NULL,false);
			return true;
		}
		if(LOWORD(wParam)==IDCANCEL)
		{
			flagpoint=0;
			EndDialog(hDlg,true);
			return true;
		}
		break;
	}
	return (INT_PTR)FALSE;
};

int Redraw3D()
{
	HWND h=FindWindow(_T("3D_VIEWER"),NULL);
	if(h!=NULL)
		InvalidateRect(h,NULL,true);
	return 1;
}

int RedrawSc()
{
	HWND h=FindWindow(_T("SCHEMA_VIEWER"),NULL);
	if(h!=NULL)
		InvalidateRect(h,NULL,true);
	return 1;
}
