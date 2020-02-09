//v 1.1 ����� �������� �. �.
//..........................
#ifndef GEOM23
#define GEOM23

#define _USE_MATH_DEFINES
#define _EPSILON 1e-3
#define EQUAL_ERROR_GEOM 1e-3
#define MIN3(A,B,C) ((A)<(B)?((A)<(C)?(A):(C)):((B)<(C)?(B):(C)))
#define MAX3(A,B,C) ((A)>(B)?((A)>(C)?(A):(C)):((B)>(C)?(B):(C)))
#define MIN2(A,B) ((A)<(B)?(A):(B))
#define MAX2(A,B) ((A)>(B)?(A):(B))

#include<math.h>
#include<vector>

using namespace std;
//global variables

//enums
enum v2fClassify{LEFT,			//�����
				RIGHT,			//������
				ORIGIN,			//������
				DESTINATION,	//�����
				BEYOND,			//����� �������
				BEHIND,			//����� �����
				BETWEEN};		//����� ������� � ������

enum l2dIntersect {COLLINEAR,	//�����������
				   PARALLEL,	//�����������
				   SKEW,		//��� ��������
				   SKEW_CROSS,	//��� �������� � ������������
				   SKEW_NO_CROSS};// ��� �������� �� �� ������������

enum v3fClassify {NEGATIVE,		//� ������������� ����������������
				  POSITIVE,		//� ������������� ����������������
				  ON};			//����� �� ���������

enum TbyTpos {NO_INTRSCT,		//������������ �� ������������
			  SAME_PLANE_INTRSCT,//������������ � ����� � ����� ���������
			  SMPL_INTRSCT};	//�������������� ������������ �� ����������� ����� ���������
enum EdgeT{CROSSING,			//������������
		   TOUCHING,			//�����������
		   INESSENTIAL};		//������������

enum InTri{OUTSIDE,				//�������
			_INSIDE,			//������
			BOUNDARY};			//�� �������
enum InBoard{NONE,				//�� ����� �������
			TOUCH,				//1-2
			BOARD};				//3
enum OperationType{UNION,
					INTERSECTION,
					SUBSTRUCTION};


//����� vector2f ��� �������� ���������� �������� � ������� � {0,0} 
// � ������ �{x,y} ��� ��������� ����� {x,y}
class vector2f
{
public:
	float x,y;
	vector2f(float _x = 0.0, float _y = 0.0):x(_x), y(_y){};
	vector2f operator+(vector2f &v);//�������� � ������� ��������
	vector2f operator-(vector2f &v);//��������� �� �������� �������
	vector2f operator*(float c);		//��������� ������� �� �����
	float DotProduct(vector2f &v);		//��������� ������������
	float operator[](int i);			//������������� ������ ��� ������� �� 2 ���������
	bool operator<(vector2f &v);		//������������������ �������
	bool operator>(vector2f &v);
	bool operator==(vector2f &v);
	bool operator!=(vector2f &v);
	float Length(void);				//������ �������
	void Normalize(void);			//������������ �������
	void Flip(void);				//��������� �����������
	float PolarAngle(void);			//���� � �������� �����������
	int Classify(vector2f &v0, vector2f &v1);//��������� ����� ������������ ������� [v0,v1]
};
//����� ��� �������� ������� � ������������ {x,y,z}
class vector3f
{
public:
	float x, y, z;
	vector3f(float _x = 0.0, float _y = 0.0, float _z = 0.0):x(_x),y(_y),z(_z){};
	vector3f(vector2f &v):x(v.x),y(v.y),z(0.0){};
	vector3f operator+(vector3f &v);
	vector3f operator-(vector3f &v);
	vector3f operator*(float c);
	float DotProduct(vector3f &v);
	vector3f CrossProduct(vector3f &v);//��������� ������������
	float operator[](int i);
	bool operator<(vector3f &v);		//������������������ �������
	bool operator>(vector3f &v);
	bool operator==(vector3f &v);
	bool operator!=(vector3f &v);
	float Length(void);
	void Normalize(void);
	void Flip(void);
	vector2f PrjX0Y(void);//�������� �� ���������
	vector2f PrjX0Z(void);
	vector2f PrjY0Z(void);
	int Classify(vector3f &v0, vector3f &v1, vector3f &v2);//��������� ����� ������������ ������������ [v0,v1,v2]
};
//����� ��� �������� ����� �� ��������� �0Y
class line2d
{
public:
	vector2f a,b;
	line2d():a(vector2f()), b(vector2f()){};
	line2d(vector2f &_a, vector2f _b):a(_a),b(_b){};
	int Intersect(line2d &l, float &t);
	int Cross(line2d &l, float &t);
	void Rotate(void);
	void Flip(void);
	vector2f Point(float t);
	bool IsVertical(void);
	bool IsHorizontal(void);
	int ClassifyPoint(vector2f p);
};
class line3d
{
public:
	vector3f a, b;
	line3d():a(vector3f()),b(vector3f()){};
	line3d(vector3f &_a, vector3f &_b):a(_a),b(_b){};
	void Flip(void);
	vector3f Point(float t);
	int IntersectWithBox(line3d &bb);
	int PointInBox(vector3f &p);
};
//������� �����
typedef vector<vector3f> vector3f_array;
typedef vector<vector2f> vector2f_array;
typedef vector<int> int_array;
class triangle3d
{
private:
	vector3f _v[3];
	line3d _box;
	vector3f _n;
public:
	triangle3d(vector3f p0, vector3f p1, vector3f p2);
	triangle3d();
	vector3f operator[](int i);
	line3d Box(void);
	vector3f N(void);
	int IntersectL(line3d &l, float &t);		//����������� � ������
	int IntersectT(triangle3d &T, vector3f_array &rez);//����������� � �������������
	int HasPoint(vector3f p);//����� �� ����������� �������
	int IsBounding(vector3f p);//�������� �� ����������� �����
	double SGeron();			//������� � �������������� ������� ������
};
//������ ��������
//����� �������� �����
class iEdge
{
public:
	int start, end;
	iEdge(int s = -1, int e = -1):start(s),end(e){};
	bool operator ==(iEdge &E);
	bool HasVertex(int i);
	line2d Line(vector2f_array p);
};
struct Line
{
	line2d l;
	iEdge i;
};
//����� �������� �����
class iFace
{
public:
	int a, b, c;
	iFace(int _a, int _b, int _c):a(_a), b(_b), c(_c){};
	iFace():a(-1),b(-1),c(-1){};
	void ChangeObhod();
	int Vertex(int i);
	bool HasEdge(iEdge n);
	bool operator ==(iFace f);
	iEdge Edge(int i);	
};
//
typedef vector<iEdge> iEdge_array;
typedef vector<iFace> iFace_array;
//����� �����

class Mesh
{
public:
	vector3f_array points;
	vector3f_array normals;
	iFace_array faces;
	line3d box;

	int Save(char* file_name);
	int Load(char* file_name);
	int Append(FILE *f);
	int Extract(FILE *f);
	Mesh();
	Mesh(const Mesh & m);
	void operator =(const Mesh &m);
	int set(vector3f_array p, iFace_array f);
	int set(Mesh m);
	int CalcNormals(void);
	void Intersect(Mesh B, Mesh &rez, iEdge_array &board);
	
	
	~Mesh();
};
//����� ��� ���������� ��������� �������� ��� ���������
class BooleanMesh
{
public:
	Mesh _A, _B, Rez;
	iFace_array Link_A, Link_B;//������ ���������
	iEdge_array intrsct_edges_A, intrsct_edges_B;//������ ����� �����������
	int_array faces_group_list_A, faces_group_list_B, 
			  in_out_A,in_out_B;
	int OperationType;

	int TouchBoard(iFace f, iEdge_array board);
	int CreateGroupList(int_array &list, Mesh A, iEdge_array intrsct, iFace_array Link);
	int GetNeiborsInfo(Mesh m, iFace_array &neibors);
	int InOutInfo1(Mesh mA, Mesh mB, int_array A_group_flags, int_array &in_out_status , iEdge_array A_board, iEdge_array B_board);
	Mesh GetA();
	Mesh GetB();

	BooleanMesh();
	int Set(Mesh A, Mesh B);//��� ����� �� ��������� ����������
	Mesh GetMesh(int OpTy);//��������� � ����������� �������������� �����
	~BooleanMesh();
};
//...........................
//����� �������� ������ ������������� ��������
class Frame
{
public:
	Frame();
	~Frame();
	Frame(vector3f _pos, vector3f _tag, vector3f _up);
	Frame(Frame &f);
	void GetCameraMatrix(float m[16]);
	
	void GetActorMatrix(float m[16]);
	void operator =(Frame &f);
	void set(vector3f _pos, vector3f _tag, vector3f _up);
	void RotateU(float angle);
	void RotateV(float angle);
	void RotateN(float angle);
	void GoU(float step);
	void GoV(float step);
	void GoN(float step);
	vector3f U();
	vector3f V();
	vector3f N();
	vector3f Position();
private:
	void CalcUVN(vector3f tag, vector3f pos, vector3f up);
	vector3f position;
	vector3f u, v, n;
};
//...........................
//functions

//math

//Iscinab
//��������� ���������� ����� �� ����� � ����� ������� � � b
int Iscinab(float a, float b, float c);

//det � detv ��������� ������������ ������� 3*3 ������� �������� ��������� � ������ ������
float det(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
float detv(float *x);//��������� ���������� ������� ���������� ��� ������ �� 9 �����

//geom

//IsInTriangle2ds
//��������� ���������� ����� �� ����� ������ ������������
int IsInTriangle2ds(vector2f *v, vector2f &p);

//GetO
//��������� ��������� ���������� ������ p0p1p2
//��������� �,� - ���������� ������ ����������
//z - ������ ����������
vector3f GetO(vector2f p1,vector2f p2,vector2f p3);

//TriangulateRing
//��������� ��������� ������ �� ������������(����)
int TriangulateRing(iEdge_array ring, iFace_array &rez);

//GetBoundRing
//��������� �������� ���������� ��������(�������) ��������� ������ �����
int GetBoundRing(vector2f_array data, iEdge_array &rez);

//CreateTriangulation
//������� ������������ ��������� ������ �����
//������� �������� ��������
//� ���������� �������� ������ ���������� �� ������� ����������� ���������� �����������
int CreateTriangulationDelone1(vector2f_array &data, iFace_array &faces);
int SimpleTriangulation(vector2f_array &data2d, iFace_array &faces);

//...........................
int Cube(Mesh* M, float w, float l, float h, int nw, int nl, int nh);
int Plane(Mesh *m, float w, float h, int nw, int nh);
int Sphere(Mesh* M, float R, int msegm, int rsegm);
int Cone(Mesh* m, float R, float H, int nr, int nH);
int Cilindr(Mesh* m, float R, float h, int nr, int nh);
int Piramid(Mesh* m, float h, float w, float l, int nw, int nl, int nh);
//...........................
int ExtrudeRing(Mesh* m, iEdge_array Ring, vector3f_array points, vector3f direct);
//...........................
int Prj3dTo2d(vector3f_array src, vector2f_array &dst);
int CreateBoundingRingTriangulation(vector2f_array d2d, iFace_array &faces);
int CmpEdgeLength(const void * a, const void *b);
//matrix math
int TranspoceMatrix44(float m[4][4],float mt[4][4]);
int Matrix16ToMatrix44(float m16[16], float m44[4][4]);
int Matrix44ToMatrix16(float m44[4][4], float m16[16]);
int LoadRotateMatrix44(float m44[4][4], float angle, float x, float y, float z);
int LoadScaleMatrix44(float m44[4][4], float sx, float sy, float sz);
int LoadTranslateMatrix44(float m44[4][4], float dx, float dy, float dz);
int LoadIdentityMatrix44(float m44[4][4]);
int Matrix44XVector3f(vector3f v, float m[4][4], vector3f &rez);
int Matrix44_X_Vector4(float v4[4], float m44[4][4], float rez4[4]);
int Vector4_X_Matrix44(float v4[4], float m44[4][4], float rez4[4]);
int Matrix44_X_Matrix44(float a[4][4], float b[4][4], float c[4][4]);
int Obhod(vector3f a, vector3f b, vector3f c, int lr);//lr=0- left =1 -right //return 0 �� ������� 1 - ������
int IsInOneLine(vector2f p, vector2f la, vector2f lb);
int IsInOneLine1(vector2f p, vector2f la, vector2f lb);
int NormalizePoints(vector2f_array src, vector2f_array &dst);

int TriangulationC(vector2f_array p, iEdge_array e, iFace_array &frez);
int TriangulatePolygon1(vector2f_array p, iEdge_array Poly, iFace_array &frez);

class Schema2d
{
public:
	vector2f_array pTop, pLeft, pFront;
	iEdge_array eTop, eLeft, eFront;
	float size_of_n_edge;
	int n;
	int n_view;
	vector3f minxyz, maxxyz;

	int Read(char *fn);
	int Save(char *fn);
	int Read(FILE *f);
	int Append(FILE *f);
	int FindAndApplyO(void);
	int GetBoundRect(line2d &r);
	Schema2d();
	~Schema2d();
};
class Ring
{
public:
	int flag;
	iEdge_array edges;
	vector2f_array points;
	Ring(const Ring& r)
	{
		flag=r.flag;
		edges=r.edges;
		points=r.points;
	};
	void operator =(const Ring& r)
	{
		flag=r.flag;
		edges=r.edges;
		points=r.points;
		return;
	};
	void clear()
	{
		edges.clear();
		points.clear();
		flag=0;
		return;
	};
	int addedge(vector2f a, vector2f b);
	Ring(){};
	~Ring(){};
};
typedef vector<Ring> Ring_array;
#endif