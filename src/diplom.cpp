// diplom.cpp : Defines the entry point for the application.
//  ☀️
#ifdef WINDOWS_PLATFORM
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include "imgui_impl_opengl2.h"
#include "imgui_impl_sdl2.h"
#include "renderTarget.h"
#include "dialogs.h"
#include "geom.h"
#include "project.h"

//----------------------------------------------------------------------
void RenderMesh(Mesh m, int light_, vector3f clr);
// DATA
int nnn = 0;  // selected point index in selected mesh
Project Prjct;
// GUI
ImGuiMessageBox messageBox;
ImGuiInputBox inputBox;
int tryselect = 0;
char Text[256];

//cone cilindr plane
struct Obj1Info {
    char name[256];
    int w = 40;
    int h = 40;
    int ws = 18;
    int hs = 18;
} Obj1InfoHolder;

//cube piramid plane
struct Obj2Info {
    char name[256];
    int l = 40;
    int w = 40;
    int h = 40;
    int ls = 18;
    int ws = 18;
    int hs = 18;
} Obj2InfoHolder;

struct SphereInfo {
    char name[256];
    int r = 40;
    int ws = 18;
    int hs = 18;
} SphereInfoHolder;

struct BoolMeshInfo {
    ImGuiComboFlags flags = ImGuiComboFlags_PopupAlignLeft;
    std::vector<std::string> operations = {"union", "intersection", "substraction"};
    char name[256];
    int an = 0; // object A index
    int bn = 1; // object B index
    int on = 0; // operation number
} BoolMeshInfoHolder;

struct CoordsInfo {
    int n = 0;
    vector3f XYZ {0.f, 0.f, 0.f};
} CoordsInfoHolder;

bool m3D_flag = false;      // 3dmesh editor was opened
bool mSchema_flag = false;  // contur editor was opened
bool bool_flag = false;
bool sphere_flag = false;
bool cone_flag = false;
bool cilindr_flag = false;
bool cube_flag = false;
bool piramid_flag = false;
bool plane_flag = false;
bool pos_flag = false;
bool saveobj_flag = false;
bool loadobj_flag = false;
bool delobj_flag = false;
bool coords_flag = false;
RenderTarget* rt3d = nullptr;
///////////////////////////////////////

struct m3DViewer_t {
  float width = 768, height = 1024;
  Frame Camera =
      Frame(vector3f(2, 10, 200), vector3f(2, 10, 0), vector3f(0, 1, 0));
  float step = 10.0, angle = 2.0;
  int ffill = 0;
  GLfloat l[4] = {1.0, 1.0, 1.0, 1.0};
  GLfloat l1[4] = {10.0, 100.0, 200.0, 1.0};
  GLfloat ls[4] = {1.0, 1.0, 1.0, 1.0};
  float AspectRatio = 1.f;
  int i = 0, n = 0;
  Mesh tempm;

  void init() {
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, l);
    glLightfv(GL_LIGHT0, GL_SPECULAR, ls);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  }

  void onSize(int w, int h) {
    height = h;
    width = w;
    if (height == 0)
      height = 1;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    AspectRatio = width / height;
    gluPerspective(60.0, AspectRatio, 0.5, 9000.0);
  }
  
  void onKeyDown(int /*wparam*/) {
  }

  void onKeyUp(int key) {
	  switch (key) {
			  case SDLK_f: ffill = 1-ffill; break;
			  case SDLK_a: Camera.GoU(-step); break;
			  case SDLK_d: Camera.GoU(step); break;
			  case SDLK_s: Camera.GoN(-step); break;
			  case SDLK_w: Camera.GoN(step); break;
			  case SDLK_7: Camera.GoV(-step); break;
			  case SDLK_9: Camera.GoV(step); break;
			  case SDLK_q: Camera.RotateV(angle); break;
			  case SDLK_e: Camera.RotateV(-angle); break;
			  case SDLK_8: Camera.RotateU(angle); break;
			  case SDLK_5: Camera.RotateU(-angle); break;
			  case SDLK_4: Camera.RotateN(angle); break;
			  case SDLK_6: Camera.RotateN(-angle); break;
			  default: return;
		  }
  }

  void DrawScene() {
    float m[16];
    if (ffill) {
      glEnable(GL_LIGHTING);
    } else {
      glDisable(GL_LIGHTING);
    }

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    Camera.GetCameraMatrix(m);
    l1[0] = Camera.Position().x;
    l1[1] = Camera.Position().y;
    l1[2] = Camera.Position().z;
    glLightfv(GL_LIGHT0, GL_POSITION, l1);
    glLoadMatrixf(m);

    if (Prjct.SelectedM >= 0) {
      RenderMesh(Prjct.Meshes[Prjct.SelectedM], ffill,
                 vector3f(0.2, 0.75, 0.2));
    }
    if (ffill) {
      // glDisable(GL_LIGHTING);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    for (i = 0; i < Prjct.Meshes.size(); i++) {
      if (i != Prjct.SelectedM)
        RenderMesh(Prjct.Meshes[i], ffill, vector3f(0.2, 0.2, 0.4));
    }
    if (ffill) {
      glDisable(GL_BLEND);
      //glEnable(GL_LIGHTING);
    }
    // оси
    glBegin(GL_LINES);
    glColor3f(0.5, 0.5, 0.5);
    glVertex3f(0, 0, 0);
    glVertex3f(1000, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1000, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1000);
    glEnd();
	glFinish();
  }
} m3DViewer;

void DeleteCurrentSelectedMesh() {
    if (Prjct.SelectedM != -1) {
        Prjct.DeleteCurrentMesh();
        std::cout << "Selected mesh object was deleted!" << std::endl;
    } else {
        std::cout << "Warning: No selected mesh object!"<< std::endl;
    }
}

void DeleteCurrentSelectedRing() {
    if (Prjct.SelectedR != -1) {
        Prjct.DeleteCurrentRing();
        std::cout << "Selected ring was deleted!" << std::endl;
    } else {
        std::cout << "Warning: No selected rings!"<< std::endl;
    }
}

int IsPixelInEdge(float x,
                  float y,
                  int viewflag,
                  vector2f a,
                  vector2f b,
                  vector2f _0pos,
                  float scale);

void ShowAppMainMenuBar();

void ShowAppMainMenuBar() {
  bool needShowAboutPopup = false;
  bool tryToSaveProject = false;
  static bool openProject = false;
  std::function<void()> afterSaveFunc = {};
  std::string save_title = "Do you want to save project first";
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Project")) {
      if (ImGui::MenuItem("New", "CTRL+N")) {
        tryToSaveProject = true;
        afterSaveFunc = []() {
          std::cout << "afterSaveFunc" << std::endl;
          Prjct.Clear();
        };
      }
      if (ImGui::MenuItem("Open", "CTRL+O")) {
        tryToSaveProject = true;
        afterSaveFunc = []() {
          openProject = true;
          std::cout << "afterSaveFunc set openProject true" << std::endl;
        };
      }
      if (ImGui::MenuItem("Save", "CTRL+S")) {
        save_title = "Saving project";
        tryToSaveProject = true;
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Tools")) {
      if (ImGui::MenuItem("3D Mesh", "CTRL+M", &m3D_flag)) {
        std::cout << "3D Mesh menu item selected!" << std::endl;
      }
      if (ImGui::MenuItem("2D Scheme", "CTRL+C", &mSchema_flag)) {
        std::cout << "2D Scheme menu item selected!" << std::endl;
      }
      if (ImGui::MenuItem("Bool", "CTRL+C", &bool_flag)) {
        std::cout << "Bool tool selected!" << std::endl;
      }
      if (ImGui::MenuItem("Sphere", "CTRL+C", &sphere_flag)) {
        std::cout << "Sphere tool selected!" << std::endl;
      }
      if (ImGui::MenuItem("Cone", "CTRL+C", &cone_flag)) {
        std::cout << "Cone tool selected!" << std::endl;
      }
      if (ImGui::MenuItem("Cilindr", "CTRL+C", &cilindr_flag)) {
        std::cout << "Cilindr tool selected!" << std::endl;
      }
      if (ImGui::MenuItem("Cube", "CTRL+C", &cube_flag)) {
        std::cout << "Cube tool selected!" << std::endl;
      }
      if (ImGui::MenuItem("piramid", "CTRL+C", &piramid_flag)) {
        std::cout << "piramid tool selected!" << std::endl;
      }
      if (ImGui::MenuItem("plane", "CTRL+C", &plane_flag)) {
        std::cout << "plane tool selected!" << std::endl;
      }
      if (ImGui::MenuItem("pos", "CTRL+C", &pos_flag)) {
        std::cout << "pos tool selected!" << std::endl;
      }
      if (ImGui::MenuItem("saveobj", "CTRL+C", &saveobj_flag)) {
        std::cout << "saveobj tool selected!" << std::endl;
                          //if(Prjct.SelectedM>=0)Prjct.Meshes[Prjct.SelectedM].Save(Text);
                          //else MessageBox(hWnd,_T("Ни один из обьектов не выделен"),_T("Ошибка сохранения"),0); break; 
      }
      if (ImGui::MenuItem("loadobj", "CTRL+C", &loadobj_flag)) {
        std::cout << "loadobj tool selected!" << std::endl;
                  /*strcpy_s(Text,256,"Загрузить");
                  if(DialogBox((HINSTANCE)GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_OPENSAVE), hWnd, GetText)==IDOK) tempm.Load(Text);
                  Prjct.AddMesh(tempm,Text);
                  tempm.~Mesh();*/
      }
      if (ImGui::MenuItem("delobj", "CTRL+C", &delobj_flag)) {
          DeleteCurrentSelectedMesh();
      }
      if (ImGui::MenuItem("coords", "CTRL+C", &coords_flag)) {
        std::cout << "coords tool selected!" << std::endl;
        CoordsInfoHolder.n = 0;
	  }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help")) {
      if (ImGui::MenuItem("About", "CTRL+H")) {
        std::cout << "About selected, should show mbox" << std::endl;
        needShowAboutPopup = true;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
  // imgui popups
  if (needShowAboutPopup) {
    needShowAboutPopup = false;
    messageBox.show("Help",
                    "Create 3D objects using extrude and boolean tools");
  }
  if (tryToSaveProject) {
    tryToSaveProject = false;
    inputBox.show(
        save_title.c_str(), "Path to save project as",
        [afterSaveFunc]() {
          std::cout << "Save project to " << inputBox.getText() << std::endl;
          Prjct.Save(inputBox.getText());
          if (afterSaveFunc) {
            afterSaveFunc();
          }
        },
        afterSaveFunc);
  }
  if (openProject) {
    openProject = false;
    inputBox.show("Open Project", "Path to project file", []() {
      std::cout << "Open File " << inputBox.getText() << std::endl;
      Prjct.Open(inputBox.getText());
      std::cout << "project opened: Meshes:" << std::endl;
      for (size_t i = 0; i < Prjct.MeshNames.size(); ++i) {
        std::cout << "    " << Prjct.MeshNames[i] << std::endl;
      }
    });
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
        case WM_PAINT:
                hdc = BeginPaint(hWnd, &ps);
                Rectangle(hdc,15,95,MainFormSize.x/3-30,MainFormSize.y/2-20);
                Rectangle(hdc,15,MainFormSize.y/2+75,MainFormSize.x/3-30,MainFormSize.y-60);
                //инфо об объекте
                if(Prjct.SelectedM>=0)
                {
                        n=sprintf_s(Text,256,"Количество точек:
%d",Prjct.Meshes[Prjct.SelectedM].points.size()); TextOutA(hdc,20, 110,Text,n);
                        n=sprintf_s(Text,256,"Количество граней:
%d",Prjct.Meshes[Prjct.SelectedM].faces.size()); TextOutA(hdc,20, 130,Text,n);
                }
                else
                {
                        n=sprintf_s(Text,256,"Выделите объект...");
                        TextOutA(hdc,20, 110,Text,n);
                }

                if(Prjct.SelectedR>=0)
                {
                        if(Prjct.Rings[Prjct.SelectedR].flag==0)n=sprintf_s(Text,256,"Вид:Главный");
                        if(Prjct.Rings[Prjct.SelectedR].flag==1)n=sprintf_s(Text,256,"Вид:Сверху"); 
                        if(Prjct.Rings[Prjct.SelectedR].flag==2)n=sprintf_s(Text,256,"Вид:Слева");
                        TextOutA(hdc,20, MainFormSize.y/2+80,Text,n);
                        n=sprintf_s(Text,256,"Количество точек:%d",Prjct.Rings[Prjct.SelectedR].points.size());
                        TextOutA(hdc,20,MainFormSize.y/2+110,Text,n);
                        n=sprintf_s(Text,256,"Количество ребер:%d",Prjct.Rings[Prjct.SelectedR].edges.size());
                        TextOutA(hdc,20,MainFormSize.y/2+140,Text,n);
                }
                else
                {
                        n=sprintf_s(Text,256,"Выделите объект...");
                        TextOutA(hdc,20,  MainFormSize.y/2+80,Text,n);
                }
                EndPaint(hWnd, &ps);
                break;
}
}*/
void RenderMesh(Mesh m, int light_, vector3f clr) {
	auto fillMode = light_ == 1? GL_FILL: GL_LINE;
    glPolygonMode(GL_FRONT, fillMode);
    glPolygonMode(GL_BACK, fillMode);

  glBegin(GL_TRIANGLES);
  if (light_ == 1)
    glColor4f(clr.x, clr.y, clr.z, 0.5);
  else
    glColor3f(clr.x, clr.y, clr.z);
  for (int i = 0; i < m.faces.size(); ++i) {
    if (light_) {
      glNormal3f(m.normals[i].x, m.normals[i].y, m.normals[i].z);
	}
    glVertex3f(m.points[m.faces[i].a].x, m.points[m.faces[i].a].y,
               m.points[m.faces[i].a].z);
    glVertex3f(m.points[m.faces[i].b].x, m.points[m.faces[i].b].y,
               m.points[m.faces[i].b].z);
    glVertex3f(m.points[m.faces[i].c].x, m.points[m.faces[i].c].y,
               m.points[m.faces[i].c].z);
  }
  glEnd();

  return;
}
/*
LRESULT CALLBACK SchemeViewerProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM
lParam)
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
                        strcpy_s(Text,256,"Открыть");
                        if(DialogBox((HINSTANCE)GetModuleHandle(NULL),
MAKEINTRESOURCE(IDD_OPENSAVE), hWnd, GetText)==IDOK)
                        {
                                schema.Read(Text);
                                schema.FindAndApplyO();
                                schema.GetBoundRect(l);
                                mSchema_flag=1;
                                GetClientRect(hWnd,&r);
                                nullpos=vector2f(r.right/2,r.bottom/2);
                                scale=(float)MAX2(r.bottom,r.right)/(float)MAX2(l.b.x+10,l.b.y+10);
                        }
                        break;
                case IDS_EXTRUDE:
                        if(DialogBox((HINSTANCE)GetModuleHandle(NULL),
MAKEINTRESOURCE(IDD_EXTRUDE), hWnd, CreateObjFromSchema)==IDOK)
                        {
                                ring.clear();
                                ring=Prjct.Rings[Prjct.SelectedR];
                                r3d.clear();
                                m.~Mesh();
                                for(i=0;i<ring.points.size();i++)
                                {
                                        if(ring.flag==0)
                                                r3d.push_back(vector3f(ring.points[i].x,ring.points[i].y,schema.minxyz.z));
                                        if(ring.flag==1)
                                                r3d.push_back(vector3f(ring.points[i].x,schema.minxyz.y,ring.points[i].y));
                                        if(ring.flag==2)
                                                r3d.push_back(vector3f(schema.minxyz.x,ring.points[i].y,ring.points[i].x));
                                }
                                if(ring.flag==0)norm=vector3f(0,0,schema.maxxyz.z-schema.minxyz.z);
                                if(ring.flag==1)norm=vector3f(0,schema.maxxyz.y-schema.minxyz.y,0);
                                if(ring.flag==2)norm=vector3f(schema.maxxyz.x-schema.minxyz.x,0,0);

                                ExtrudeRing(&m,ring.edges,r3d,norm);
                                Prjct.AddMesh(m,Text);
                                SendMessage(MainWnd,WM_USER+1,(WPARAM)hWnd,0);
                        }
                        break;
                case IDS_SELECT:
                        strcpy_s(Text,256,"Выберите контур");
                        if(DialogBox((HINSTANCE)GetModuleHandle(NULL),
MAKEINTRESOURCE(IDD_SELECT), hWnd, SelectRing)==IDOK)
                        {
                                ring.clear();
                                ring=Prjct.Rings[Prjct.SelectedR];
                        }

                        break;
                case IDS_DELETE:
                        strcpy_s(Text,256,"Удаление контура");
                        if(DialogBox((HINSTANCE)GetModuleHandle(NULL),
MAKEINTRESOURCE(IDD_SELECT), hWnd, SelectRing)==IDOK)
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
                        sprintf(str,"Редактор контуров - выделение контура");
                        SetWindowTextA(hWnd,str);
                        InvalidateRect(hWnd,NULL,true);
                }
                else
                {
                        sprintf(str,"Редактор контуров");
                        SetWindowTextA(hWnd,str);
                        if(ring.edges.size()>0)
                        {
                                Prjct.SelectedR=Prjct.Rings.size();
                                if(ring.flag==0)sprintf_s(Text,256,"№%d ребер: %d главный вид",Prjct.SelectedR,ring.edges.size());
                                if(ring.flag==1)sprintf_s(Text,256,"№%d ребер: %d вид сверху",Prjct.SelectedR,ring.edges.size());
                                if(ring.flag==2)sprintf_s(Text,256,"№%d ребер: %d вид слева",Prjct.SelectedR,ring.edges.size()); Prjct.AddRing(ring,Text);
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
                //определение выделенного ребра
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
        //	i=MessageBoxA(hWnd,"Вы уверены?","Закрыть",MB_YESNO);
        //	if(i==IDYES)DestroyWindow(hWnd);
        //	break;
        case WM_DESTROY:
                mSchema_flag=0;
                break;
        default:
                return DefWindowProc(hWnd, msg, wParam, lParam);
        }
        return 0;
}
*/
int IsPixelInEdge(float x,
                  float y,
                  int viewflag,
                  vector2f a,
                  vector2f b,
                  vector2f _0pos,
                  float scale) {
  vector2f A, B, P, ab, ap, pb;
  int rez;
  P = vector2f(x, y);
  //////
  if (viewflag == 0)  // FRONT
  {
    A = vector2f(_0pos.x - a.x * scale, _0pos.y - a.y * scale);
    B = vector2f(_0pos.x - b.x * scale, _0pos.y - b.y * scale);
  }

  if (viewflag == 1)  // TOP
  {
    A = vector2f(_0pos.x - a.x * scale, a.y * scale + _0pos.y);
    B = vector2f(_0pos.x - b.x * scale, b.y * scale + _0pos.y);
  }

  if (viewflag == 2)  // LEFT
  {
    A = vector2f(_0pos.x + a.x * scale, _0pos.y - a.y * scale);
    B = vector2f(_0pos.x + b.x * scale, _0pos.y - b.y * scale);
  }

  ab = B - A;
  ap = P - A;
  pb = P - B;
  rez = 0;
  if (fabs(ab.Length() - ap.Length() - pb.Length()) < 0.1)
    rez = 1;
  return rez;
}

/*
INT_PTR CALLBACK CreateObjFromSchema(HWND hDlg, UINT message, WPARAM wParam,
LPARAM lParam)
{
        int i;
        char str[256];
        UNREFERENCED_PARAMETER(lParam);
        switch (message)
        {
        case WM_INITDIALOG:
                for(i=0;i<Prjct.Rings.size();i++)
                {
                        SendMessageA(GetDlgItem(hDlg,IDC_COMBO1),CB_ADDSTRING,
0, (LPARAM)Prjct.RingNames[i].str);
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
                        SendMessageA(GetDlgItem(hDlg,IDC_COMBO1),CB_ADDSTRING,
0, (LPARAM)Prjct.RingNames[i].str);
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
INT_PTR CALLBACK ChangePosition(HWND hDlg, UINT message, WPARAM wParam, LPARAM
lParam)
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
                                MessageBoxA(hDlg,"Выделите какой ниюудь объект","Сообщение",0); return (INT_PTR)TRUE;
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
                                MessageBoxA(hDlg,"Выделите какой ниюудь объект","Сообщение",0); return (INT_PTR)TRUE;
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
                                MessageBoxA(hDlg,"Выделите какой ниюудь объект","Сообщение",0); return (INT_PTR)TRUE;
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
*/
////////////////////////////////////////////////////////////
// MAIN

// The window we'll be rendering to
SDL_Window* gWindow = NULL;
// OpenGL context
SDL_GLContext gContext;

// Starts up SDL, creates window, and initializes OpenGL
bool init();
// Initializes matrices and clear color
bool initGL();
// Per frame update
void update();
// Renders quad to the screen
void render();
// Frees media and shuts down SDL
void close();
void reshape(int width, int height);

bool init() {
  // Initialization flag
  bool success = true;
  std::string winCaption = "2d3d";
  int height = 768;
  int width = 1024;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError()
              << std::endl;
    success = false;
  } else {
    // Use OpenGL 2.1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    // const char* gl_ver = "#version 230";
    // Create window
    gWindow = SDL_CreateWindow("2d3d", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, width, height,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
      std::cout << "Window could not be created! SDL Error: " << SDL_GetError()
                << std::endl;
      success = false;
    } else {
      // Create context
      gContext = SDL_GL_CreateContext(gWindow);
      if (gContext == NULL) {
        std::cout << "OpenGL context could not be created! SDL Error: "
                  << SDL_GetError() << std::endl;
        success = false;
      } else {
        // Use Vsync
        if (SDL_GL_SetSwapInterval(1) < 0) {
          std::cout << "Warning: Unable to set VSync! SDL Error: "
                    << SDL_GetError() << std::endl;
        }
        // Initialize OpenGL
        if (!initGL()) {
          std::cout << "Unable to initialize OpenGL!" << std::endl;
          success = false;
        }

        // hide mouse cursor and use relative mouse move mode
        // if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0) {
        //  std::cout << "Warning: Unable to set RelativeMouseMode! SDL
        //  Error: "
        //            << SDL_GetError() << std::endl;
        //}
        // imgui setup
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForOpenGL(gWindow, gContext);
        ImGui_ImplOpenGL2_Init();
        // Setup font
        ImFont* font =
            io.Fonts->AddFontFromFileTTF("rc/XO_Tahion_Nu.ttf", 16.0f, nullptr,
                                         io.Fonts->GetGlyphRangesCyrillic());
        if (font == nullptr) {
          std::cout << "WARRNING: font is nullptr!";
        }
      }
    }
  }
  return success;
}

bool initGL() {
  bool success = true;
  glClearColor(0.5, 0.6, 0.8, 1.0);
  glClearDepth(1.0f);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glShadeModel(GL_SMOOTH);
  reshape(768, 1024);
  rt3d = new RenderTarget(768, 1024);
  return success;
}

void update() {
}

void render() {
  //draw to texture
  if (rt3d && m3D_flag) {
    rt3d->Bind();
    m3DViewer.init();
    m3DViewer.onSize(rt3d->GetWidth(), rt3d->GetHeight());
    m3DViewer.DrawScene();
    rt3d->Unbind();
  }
  //imgui layout
  ImGui_ImplOpenGL2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
  ///--------------------------------------FORM1 {
  {
    // project panel
    ImGui::Begin("Project");
    ShowAppMainMenuBar();

    static ImGuiComboFlags flags = ImGuiComboFlags_PopupAlignLeft;
    std::string preview_value =
        (Prjct.MeshNames.empty() || Prjct.SelectedM == -1)
            ? "empty"
            : Prjct.MeshNames[Prjct.SelectedM];
    if (ImGui::BeginCombo("3d objects", preview_value.c_str(), flags)) {
      for (int n = 0; n < Prjct.Meshes.size(); n++) {
        const bool is_selected = (Prjct.SelectedM == n);
        if (ImGui::Selectable(Prjct.MeshNames[n].c_str(), is_selected))
          Prjct.SelectedM = n;
        // Set the initial focus when opening the combo (scrolling +
        // keyboard navigation focus)
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }

    if (ImGui::Button("Delete object")) {
      messageBox.show("Are you shure about that?", "Delete mesh!", &DeleteCurrentSelectedMesh);
    }

    preview_value = (Prjct.RingNames.empty() || Prjct.SelectedR == -1)
                        ? "empty"
                        : Prjct.RingNames[Prjct.SelectedR];
    if (ImGui::BeginCombo("2d conturs", preview_value.c_str(), flags)) {
      for (int n = 0; n < Prjct.Rings.size(); n++) {
        const bool is_selected = (Prjct.SelectedR == n);
        if (ImGui::Selectable(Prjct.RingNames[n].c_str(), is_selected))
          Prjct.SelectedR = n;
        // Set the initial focus when opening the combo (scrolling +
        // keyboard navigation focus)
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }

    if (ImGui::Button("Delete contur")) {
      messageBox.show("Are you shure about that?", "Delete contur!", &DeleteCurrentSelectedRing);
    }
    messageBox.process();
    inputBox.process();
    ImGui::End();
    // 3d editor
    if (m3D_flag) {
      ImGui::Begin("3D Editor", &m3D_flag);
      ImGui::InputText("input text", Text, IM_ARRAYSIZE(Text));
      ImGui::SameLine();
      if (ImGui::Button("Кнопка 1")) {
        std::cout << "button1 from 3d Editor clicked text: " << Text
                  << std::endl;
        m3D_flag = false;
      }
      ///////////////////set rendertarget
      if (rt3d) {
        // we access the ImGui window size
        const float window_width = ImGui::GetContentRegionAvail().x;
        const float window_height = ImGui::GetContentRegionAvail().y;
        // we rescale the framebuffer to the actual window size here and
        // reset the glViewport
        rt3d->Rescale(window_width, window_height);
        glViewport(0, 0, window_width, window_height);
        // we get the screen position of the window
        ImVec2 pos = ImGui::GetCursorScreenPos();
        // and here we can add our created texture as image to ImGui
        ImGui::GetWindowDrawList()->AddImage(
            (ImTextureID)rt3d->getFrameTexture(), ImVec2(pos.x, pos.y),
            ImVec2(pos.x + window_width, pos.y + window_height), ImVec2(0, 1),
            ImVec2(1, 0));
      }
      ///////////////////////////////////
      ImGui::End();
    }
    // 2d editor
    if (mSchema_flag) {
      ImGui::Begin("2D Editor", &mSchema_flag);
      if (ImGui::Button("Кнопка 1")) {
        std::cout << "button1 from 2d Editor clicked" << std::endl;
        mSchema_flag = false;
      }
      ImGui::End();
    }
    // create new sphere
    if (coords_flag && Prjct.SelectedM >= 0) {
      ImGui::Begin("Координаты точки", &coords_flag);
      ImGui::InputInt("точка №", &CoordsInfoHolder.n);
      ImGui::InputFloat("X", &CoordsInfoHolder.XYZ.x);
      ImGui::InputFloat("Y", &CoordsInfoHolder.XYZ.y);
      ImGui::InputFloat("Z", &CoordsInfoHolder.XYZ.z);
      if (ImGui::Button("<<")) {
          CoordsInfoHolder.n--;
          if (CoordsInfoHolder.n < 0) {
              CoordsInfoHolder.n = Prjct.Meshes[Prjct.SelectedM].points.size() - 1;
          }
          CoordsInfoHolder.XYZ = Prjct.Meshes[Prjct.SelectedM].points[CoordsInfoHolder.n];
      }
      if (ImGui::Button("Set")) {
        Prjct.Meshes[Prjct.SelectedM].points[CoordsInfoHolder.n] = CoordsInfoHolder.XYZ;
      }
      if (ImGui::Button(">>")) {
          CoordsInfoHolder.n++;
          if (CoordsInfoHolder.n > Prjct.Meshes[Prjct.SelectedM].points.size() - 1) {
              CoordsInfoHolder.n = 0;
          }
          CoordsInfoHolder.XYZ = Prjct.Meshes[Prjct.SelectedM].points[CoordsInfoHolder.n];
      }
      ImGui::End();
    }
    // create new sphere
    if (sphere_flag) {
      ImGui::Begin("Create Sphere", &sphere_flag);
      ImGui::InputInt("radius", &SphereInfoHolder.r);
      ImGui::InputInt("w segments", &SphereInfoHolder.ws);
      ImGui::InputInt("h segments", &SphereInfoHolder.hs);
      ImGui::InputText("name", SphereInfoHolder.name, IM_ARRAYSIZE(SphereInfoHolder.name));
      if (ImGui::Button("Кнопка 1")) {
        std::cout << "button1 from 2d Editor clicked" << std::endl;
        Mesh a;
        Sphere(&a, SphereInfoHolder.r, SphereInfoHolder.ws, SphereInfoHolder.hs);
        std::string s = std::string(SphereInfoHolder.name);
        Prjct.AddMesh(a, s);
        Prjct.SelectedM = Prjct.Meshes.size()-1;
        sphere_flag = false;
      }
      ImGui::End();
    }
    // create new cone cilindr or plane
    bool tool_flag = cone_flag | cilindr_flag | plane_flag;
    if (tool_flag) {
      std::string title = "Create ";
      auto* funcPtr = &Cone;
      if (cone_flag) {
        title += "Cone";
        funcPtr = &Cone;
      }
      if (cilindr_flag) {
        title += "Cilindr";
        funcPtr = &Cilindr;
      }
      if (plane_flag) {
        title += "Plane";
        funcPtr = &Plane;
      }
      ImGui::Begin(title.data(), &tool_flag);
      ImGui::InputInt("width", &Obj1InfoHolder.w);
      ImGui::InputInt("height", &Obj1InfoHolder.h);
      ImGui::InputInt("w segments", &Obj1InfoHolder.ws);
      ImGui::InputInt("h segments", &Obj1InfoHolder.hs);
      ImGui::InputText("name", Obj1InfoHolder.name, IM_ARRAYSIZE(Obj1InfoHolder.name));
      if (ImGui::Button("Кнопка 1")) {
        Mesh a;
        funcPtr(&a, Obj1InfoHolder.w, Obj1InfoHolder.h, Obj1InfoHolder.ws, Obj1InfoHolder.hs);
        std::string s = std::string(Obj1InfoHolder.name);
        Prjct.AddMesh(a, s);
        Prjct.SelectedM = Prjct.Meshes.size()-1;

        cone_flag = cilindr_flag = plane_flag = false;
      }
      ImGui::End();
    }
    // create new cube or piramid
    tool_flag = cube_flag | piramid_flag;
    if (tool_flag) {
      std::string title = "Create ";
      auto* funcPtr = &Cube;
      if (cube_flag) {
        title += "Cube";
        funcPtr = &Cube;
      }
      if (piramid_flag) {
        title += "Piramid";
        funcPtr = &Piramid;
      }
      ImGui::Begin(title.data(), &tool_flag);
      ImGui::InputInt("length", &Obj2InfoHolder.l);
      ImGui::InputInt("width", &Obj2InfoHolder.w);
      ImGui::InputInt("height", &Obj2InfoHolder.h);
      ImGui::InputInt("l segments", &Obj2InfoHolder.ls);
      ImGui::InputInt("w segments", &Obj2InfoHolder.ws);
      ImGui::InputInt("h segments", &Obj2InfoHolder.hs);
      ImGui::InputText("name", Obj2InfoHolder.name, IM_ARRAYSIZE(Obj2InfoHolder.name));
      if (ImGui::Button("Кнопка 1")) {
        Mesh a;
        funcPtr(&a, Obj2InfoHolder.l,Obj2InfoHolder.w, Obj2InfoHolder.h, Obj2InfoHolder.ls, Obj2InfoHolder.ws, Obj2InfoHolder.hs);
        std::string s = std::string(Obj2InfoHolder.name);
        Prjct.AddMesh(a, s);
        Prjct.SelectedM = Prjct.Meshes.size()-1;

        cube_flag = piramid_flag = false;
      }
      ImGui::End();
    }
    //////////// bool mesh creation gui
    if (bool_flag && Prjct.MeshNames.size() >= 2) {
      ImGui::Begin("Create boolean meshes", &bool_flag);
      ImGui::InputText("name", BoolMeshInfoHolder.name, IM_ARRAYSIZE(BoolMeshInfoHolder.name));
    //A combobox
      std::string preview_value = Prjct.MeshNames[BoolMeshInfoHolder.an];
      if (ImGui::BeginCombo("Operand A:", preview_value.c_str(), BoolMeshInfoHolder.flags)) {
            for (int n = 0; n < Prjct.Meshes.size(); n++) {
                const bool is_selected = (BoolMeshInfoHolder.an == n);
                if (ImGui::Selectable(Prjct.MeshNames[n].c_str(), is_selected))
                    BoolMeshInfoHolder.an = n;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
      }
    //B combobox
      preview_value = Prjct.MeshNames[BoolMeshInfoHolder.bn];
      if (ImGui::BeginCombo("Operand B:", preview_value.c_str(), BoolMeshInfoHolder.flags)) {
            for (int n = 0; n < Prjct.Meshes.size(); n++) {
                const bool is_selected = (BoolMeshInfoHolder.bn == n);
                if (ImGui::Selectable(Prjct.MeshNames[n].c_str(), is_selected))
                    BoolMeshInfoHolder.bn = n;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
      }
    //operation combobox
      preview_value = BoolMeshInfoHolder.operations[BoolMeshInfoHolder.on];
      if (ImGui::BeginCombo("Operation:", preview_value.c_str(), BoolMeshInfoHolder.flags)) {
            for (int n = 0; n < BoolMeshInfoHolder.operations.size(); n++) {
                const bool is_selected = (BoolMeshInfoHolder.on == n);
                if (ImGui::Selectable(BoolMeshInfoHolder.operations[n].c_str(), is_selected))
                    BoolMeshInfoHolder.on = n;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
      }
    //button Calculate
      if (ImGui::Button("Создать объект")) {
        if (BoolMeshInfoHolder.an != BoolMeshInfoHolder.bn) {
            Mesh a, b, c;
            BooleanMesh bm;
            std::cout << "DEBUG: an = " << BoolMeshInfoHolder.an << std::endl;
            std::cout << "DEBUG: bn = " << BoolMeshInfoHolder.bn << std::endl;
            std::cout << "DEBUG: operation = " << BoolMeshInfoHolder.operations[BoolMeshInfoHolder.on] << std::endl;
            a=Prjct.Meshes[BoolMeshInfoHolder.an];
            b=Prjct.Meshes[BoolMeshInfoHolder.bn];
            a.CalcNormals();
            b.CalcNormals();

            std::cout << "DEBUG: prepare bool mesh" << std::endl;
            bm.Set(a,b);

            std::cout << "DEBUG: create bool mesh" << std::endl;
            if (BoolMeshInfoHolder.on==0) c=bm.GetMesh(UNION);
            if (BoolMeshInfoHolder.on==1) c=bm.GetMesh(INTERSECTION);
            if (BoolMeshInfoHolder.on==2) c=bm.GetMesh(SUBSTRUCTION);

            std::cout << "DEBUG: calc normals" << std::endl;
            c.CalcNormals();
            std::cout << "DEBUG: add new mesh to project" << std::endl;
            std::string s = std::string(BoolMeshInfoHolder.name);
            Prjct.AddMesh(c, s);
            Prjct.SelectedM=Prjct.Meshes.size()-1;
            std::cout << "DEBUG: finish creation" << std::endl;

            bool_flag = false;
        } else {
            std::cout << "Warning: a = " << BoolMeshInfoHolder.an
                << " and b = " << BoolMeshInfoHolder.bn << ", but its should be different"
                << std::endl;
        }
      }
      ImGui::End();
    }
    /////////////
  }
  ///--------------------------------------FORM1 }
  // Rendering
  ImGui::Render();
  glClearColor(0.5, 0.6, 0.8, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void reshape(int width, int height) {
  glViewport(0, 0, (GLint)width, (GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLfloat AspectRatio = (GLfloat)width / (GLfloat)height;
  gluPerspective(60.0, AspectRatio, 1.0, 2000.0);
}

void close() {
  if (rt3d) {
    delete rt3d;
  }
  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  // Destroy window
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  // Quit SDL subsystems
  SDL_Quit();
}

int main(int argc, char* args[]) {
  // Start up SDL and create window
  std::cout << "Debug " << __FUNCTION__ << " in " << __FILE__ << ": "
            << __LINE__ << std::endl;
  if (!init()) {
    printf("Failed to initialize!\n");
  } else {
    // Main loop flag
    bool quit = false;
    // Event handler
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();
    int frames = 0;
    double all_time = 0;
    std::chrono::duration<double> delta_time;
    auto start_time = std::chrono::system_clock::now();
    auto end_time = std::chrono::system_clock::now();
    // While application is running
    while (!quit) {
      end_time = std::chrono::system_clock::now();
      delta_time = end_time - start_time;
      double dt = delta_time.count();
      // Handle events on queue
      while (SDL_PollEvent(&e) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&e);
        // User requests quit
        if (e.type == SDL_QUIT) {
          quit = true;
        } else if (e.type == SDL_KEYDOWN) {
			m3DViewer.onKeyDown(e.key.keysym.sym);
        } else if (e.type == SDL_KEYUP) {
			m3DViewer.onKeyUp(e.key.keysym.sym);
        } else if (e.type == SDL_MOUSEMOTION) {
          int x = 0, y = 0;
          SDL_GetMouseState(&x, &y);
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
          int x = 0, y = 0;
          SDL_GetMouseState(&x, &y);
        } else if (e.type == SDL_MOUSEBUTTONUP) {
          int x = 0, y = 0;
          SDL_GetMouseState(&x, &y);
        } else if (e.type == SDL_WINDOWEVENT_RESIZED ||
                   e.type == SDL_WINDOWEVENT_SIZE_CHANGED) {
          // event->window.windowID
          reshape(e.window.data1, e.window.data2);
        }
      }
      update();
      // Render quad
      render();
      // Update screen
      SDL_GL_SwapWindow(gWindow);
      ++frames;
      all_time += dt;
      start_time = end_time;
    }
    // Disable text input
    SDL_StopTextInput();
    std::cout << "FPS: " << frames / all_time << std::endl;
    std::cout << "time: " << all_time << " sec" << std::endl;
    std::cout << "released resources, exit" << std::endl;
  }
  // Free resources and close SDL
  close();
  return 0;
}
