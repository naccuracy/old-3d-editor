//  ☀️
#include "geom.h"

vector2f vector2f::operator+(const vector2f& v) const {
  return vector2f(x + v.x, y + v.y);
}
vector2f vector2f::operator-(const vector2f& v) const {
  return vector2f(x - v.x, y - v.y);
}

vector2f vector2f::operator*(float c) const {
  return vector2f(x * c, y * c);
}

float vector2f::DotProduct(const vector2f& v) const {
  return x * v.x + y * v.y;
}

float vector2f::operator[](int i) const {
  if (i < 2) {
    if (i == 0)
      return x;
    if (i == 1)
      return y;
  }
  return 0.0;
}

bool vector2f::operator<(const vector2f& v) const {
  return ((x < v.x) || ((x == v.x) && (y < v.y)));
}
bool vector2f::operator>(const vector2f& v) const {
  return ((x > v.x) || ((x == v.x) && (y > v.y)));
}
bool vector2f::operator==(const vector2f& v) const {
  if ((fabs(x - v.x) <= EQUAL_ERROR_GEOM) &&
      (fabs(y - v.y) <= EQUAL_ERROR_GEOM))
    return true;
  else
    return false;
}
bool vector2f::operator!=(const vector2f& v) const {
  return !(*this == v);
}
float vector2f::Length() {
  return sqrt(x * x + y * y);
}
void vector2f::Normalize() {
  float L = Length();
  if (L < 1e-6)
    return;
  x = x / L;
  y = y / L;
  return;
}
void vector2f::Flip() {
  x = -x;
  y = -y;
  return;
}
float vector2f::PolarAngle() {
  if ((x == 0.0) || (y == 0.0))
    return -1.0;
  if (x == 0.0)
    return ((y > 0.0) ? (float)90.0 : (float)270.0);
  float teta = atan(y / x);
  teta = (float)(teta * 180.0 / M_PI);
  if (x > 0.0)
    return (float)((y >= 0.0) ? teta : teta + 360.0);
  else
    return (float)(teta + 180.0);
}
int vector2f::Classify(const vector2f& v0, const vector2f& v1) const {
  vector2f v2 = *this;
  vector2f a = v1 - v0;
  vector2f b = v2 - v0;
  double sa = a.x * b.y - a.y * b.x;
  if (sa > 0.01)
    return LEFT;
  if (sa < -0.01)
    return RIGHT;
  if (v0 == v2)
    return ORIGIN;
  if (v1 == v2)
    return DESTINATION;
  if ((a.x * b.x < 0) || (a.y * b.y < 0))
    return BEHIND;
  if (a.Length() < b.Length())
    return BEYOND;
  return BETWEEN;
}
//...............................................
vector3f vector3f::operator+(const vector3f& v) const {
  return vector3f(x + v.x, y + v.y, z + v.z);
}
vector3f vector3f::operator-(const vector3f& v) const {
  return vector3f(x - v.x, y - v.y, z - v.z);
}
vector3f vector3f::operator*(float c) const {
  return vector3f(x * c, y * c, z * c);
}
float vector3f::DotProduct(const vector3f& v) const {
  return x * v.x + y * v.y + z * v.z;
}
float vector3f::operator[](int i) const {
  if (i < 3) {
    if (i == 0)
      return x;
    if (i == 1)
      return y;
    if (i == 2)
      return z;
  }
  return 0.0;
}
bool vector3f::operator<(const vector3f& v) const {
  return ((x < v.x) || ((x == v.x) && (y < v.y)) ||
          ((x == v.x) && (y == v.y) && (z < v.z)));
}
bool vector3f::operator>(const vector3f& v) const {
  return ((x > v.x) || ((x == v.x) && (y > v.y)) ||
          ((x == v.x) && (y == v.y) && (z > v.z)));
}
bool vector3f::operator==(const vector3f& v) const {
  if ((fabs(x - v.x) < EQUAL_ERROR_GEOM) &&
      (fabs(y - v.y) < EQUAL_ERROR_GEOM) && (fabs(z - v.z) < EQUAL_ERROR_GEOM))
    return true;
  else
    return false;
}
bool vector3f::operator!=(const vector3f& v) const {
  return !(*this == v);
}
float vector3f::Length() {
  return sqrt(x * x + y * y + z * z);
}
void vector3f::Normalize() {
  float l = Length();
  if (l < 1e-6)
    return;
  x = x / l;
  y = y / l;
  z = z / l;
  return;
}
void vector3f::Flip() {
  x = -x;
  y = -y;
  z = -z;
  return;
}
vector3f vector3f::CrossProduct(const vector3f& v) const {
  return vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}
vector2f vector3f::PrjX0Y() {
  return vector2f(x, y);
}
vector2f vector3f::PrjX0Z() {
  return vector2f(x, z);
}
vector2f vector3f::PrjY0Z() {
  return vector2f(y, z);
}
int vector3f::Classify(const vector3f& v0,
                       const vector3f& v1,
                       const vector3f& v2) const {
  vector3f v = *this - v0;
  float l = v.Length();
  if (l == 0.0)
    return ON;
  vector3f n = vector3f(v1 - v0).CrossProduct(v2 - v0);
  n.Normalize();
  v.Normalize();
  float d = v.DotProduct(n);
  if (d > _EPSILON) {
    return POSITIVE;
  } else if (d < -_EPSILON) {
    return NEGATIVE;
  } else {
    return ON;
  }
}
// line2d members
void line2d::Rotate() {
  vector2f m = (a + b) * 0.5;
  vector2f v = (b - a);
  vector2f n(v.y, -v.x);
  a = m - n * 0.5;
  b = m + n * 0.5;
  return;
}

void line2d::Flip() {
  Rotate();
  Rotate();
  return;
}

int line2d::Intersect(const line2d& l, float& t) const {
  vector2f A = a;
  vector2f B = b;
  vector2f C = l.a;
  vector2f D = l.b;
  vector2f n = vector2f((D - C).y, (C - D).x);
  float denom = n.DotProduct(B - A);
  if (denom == 0.0) {
    int aclass = A.Classify(C, D);
    if ((aclass == LEFT) || (aclass == RIGHT))
      return PARALLEL;
    else
      return COLLINEAR;
  }
  float num = n.DotProduct(A - C);
  t = -num / denom;
  return SKEW;
}
int line2d::Cross(const line2d& l, float& t) const {
  float s = 0.0;
  int cross_type = l.Intersect(*this, s);
  if ((cross_type == PARALLEL) || (cross_type == COLLINEAR))
    return cross_type;
  if ((s < 0.001) || (s > 1.001))
    return SKEW_NO_CROSS;
  Intersect(l, t);
  if ((t >= 0.0) && (t <= 1.0))
    return SKEW_CROSS;
  else
    return SKEW_NO_CROSS;
}
vector2f line2d::Point(float t) {
  return (a + (b - a) * t);
}
bool line2d::IsHorizontal() {
  return (a.y == b.y);
}
bool line2d::IsVertical() {
  return (a.x == b.x);
}
int line2d::ClassifyPoint(const vector2f& p) const {
  switch (p.Classify(a, b)) {
    case LEFT:
      return ((a.y < p.y) && (p.y <= b.y) ? CROSSING : INESSENTIAL);
    case RIGHT:
      return ((b.y < p.y) && (p.y <= a.y) ? CROSSING : INESSENTIAL);
    case BETWEEN:
    case ORIGIN:
    case DESTINATION:
      return TOUCHING;
    default:
      return INESSENTIAL;
  }
}
// class line3d
void line3d::Flip() {
  vector3f v = a;
  a = b;
  b = v;
  return;
}
vector3f line3d::Point(float t) {
  return (a + (b - a) * t);
}
int line3d::PointInBox(const vector3f& p) {
  int rez = 0;
  if (Iscinab(a.x, b.x, p.x))
    rez++;
  if (Iscinab(a.y, b.y, p.y))
    rez++;
  if (Iscinab(a.z, b.z, p.z))
    rez++;
  if (rez == 3)
    return 1;  // warning
  else
    return 0;
}
int line3d::IntersectWithBox(const line3d& bb) {
  vector3f p;
  p = vector3f(bb.a.x, bb.a.y, bb.a.z);
  if (PointInBox(p))
    return 1;
  p = vector3f(bb.a.x, bb.a.y, bb.b.z);
  if (PointInBox(p))
    return 1;
  p = vector3f(bb.a.x, bb.b.y, bb.a.z);
  if (PointInBox(p))
    return 1;
  p = vector3f(bb.a.x, bb.b.y, bb.b.z);
  if (PointInBox(p))
    return 1;
  p = vector3f(bb.b.x, bb.a.y, bb.a.z);
  if (PointInBox(p))
    return 1;
  p = vector3f(bb.b.x, bb.a.y, bb.b.z);
  if (PointInBox(p))
    return 1;
  p = vector3f(bb.b.x, bb.b.y, bb.a.z);
  if (PointInBox(p))
    return 1;
  p = vector3f(bb.b.x, bb.b.y, bb.b.z);
  if (PointInBox(p))
    return 1;
  return 0;
}
// triangle3d
triangle3d::triangle3d(vector3f p0, vector3f p1, vector3f p2) {
  _v[0] = p0;
  _v[1] = p1;
  _v[2] = p2;
  _box.a.x = MIN3(p0.x, p1.x, p2.x);
  _box.a.y = MIN3(p0.y, p1.y, p2.y);
  _box.a.z = MIN3(p0.z, p1.z, p2.z);
  _box.b.x = MAX3(p0.x, p1.x, p2.x);
  _box.b.y = MAX3(p0.y, p1.y, p2.y);
  _box.b.z = MAX3(p0.z, p1.z, p2.z);
  _n = vector3f(p1 - p0).CrossProduct(p2 - p0);
  _n.Normalize();
}
triangle3d::triangle3d() {
  _v[0] = _v[1] = _v[2] = _box.a = _box.b = _n = vector3f(0, 0, 0);
}
vector3f triangle3d::operator[](int i) {
  return _v[i];
}
line3d triangle3d::Box() {
  return _box;
}
vector3f triangle3d::N() {
  return _n;
}
int triangle3d::IntersectL(line3d& l, float& t) {
  vector3f a = l.a;
  vector3f b = l.b;
  vector3f c = _v[0];
  float denom = _n.DotProduct(b - a);
  if (denom == 0.0) {
    int aclass = a.Classify(_v[0], _v[1], _v[2]);
    if (aclass != ON)
      return PARALLEL;
    else
      return COLLINEAR;
  }
  float num = _n.DotProduct(a - c);
  t = -num / denom;
  return SKEW;
}
int triangle3d::IntersectT(triangle3d& T, vector3f_array& rez) {
  int ca, cb, cc, rflag;
  line2d lines2_0[3], lines2_1[3];
  line3d lines3_0[3], lines3_1[3];
  vector2f points2_0[3], points2_1[3];
  vector3f p;
  unsigned int i, j;
  float dx, dy, dz, d, param;

  ca = _v[0].Classify(T[0], T[1], T[2]);
  cb = _v[1].Classify(T[0], T[1], T[2]);
  cc = _v[2].Classify(T[0], T[1], T[2]);

  lines3_0[0] = line3d(_v[0], _v[1]);
  lines3_0[1] = line3d(_v[1], _v[2]);
  lines3_0[2] = line3d(_v[2], _v[0]);
  lines3_1[0] = line3d(T[0], T[1]);
  lines3_1[1] = line3d(T[1], T[2]);
  lines3_1[2] = line3d(T[2], T[0]);

  if ((ca == cb) && (ca == cc)) {
    if (ca != ON)
      return NO_INTRSCT;
    else {
      dx = MIN2((Box().b.x - Box().a.x), (T.Box().b.x - T.Box().a.x));
      dy = MIN2((Box().b.y - Box().a.y), (T.Box().b.y - T.Box().a.y));
      dz = MIN2((Box().b.z - Box().a.z), (T.Box().b.z - T.Box().a.z));
      d = MIN3(dx, dy, dz);
      if (d == dx) {
        for (i = 0; i < 3; i++) {
          points2_0[i] = _v[i].PrjY0Z();
          points2_1[i] = T[i].PrjY0Z();
        }
      } else if (d == dy) {
        for (i = 0; i < 3; i++) {
          points2_0[i] = _v[i].PrjX0Z();
          points2_1[i] = T[i].PrjX0Z();
        }
      } else if (d == dz) {
        for (i = 0; i < 3; i++) {
          points2_0[i] = _v[i].PrjX0Y();
          points2_1[i] = T[i].PrjX0Y();
        }
      }
      // ��������� ������ ��������������� �������������
      lines2_0[0] = line2d(points2_0[0], points2_0[1]);
      lines2_0[1] = line2d(points2_0[1], points2_0[2]);
      lines2_0[2] = line2d(points2_0[2], points2_0[0]);
      lines2_1[0] = line2d(points2_1[0], points2_1[1]);
      lines2_1[1] = line2d(points2_1[1], points2_1[2]);
      lines2_1[2] = line2d(points2_1[2], points2_1[0]);
      for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
          if (lines2_0[i].Cross(lines2_1[j], param) == SKEW_CROSS) {
            rez.push_back(lines3_0[i].Point(param));
          }
        }
      }
      // �������� �� ��������� ������
      for (i = 0; i < 3; i++) {
        if (IsInTriangle2ds(points2_0, points2_1[i]) != OUTSIDE)
          rez.push_back(T[i]);
        if (IsInTriangle2ds(points2_1, points2_0[i]) != OUTSIDE)
          rez.push_back(_v[i]);
      }
      rflag = SAME_PLANE_INTRSCT;
    }
  } else {
    for (i = 0; i < 3; i++) {
      if (IntersectL(lines3_1[i], param) == SKEW) {
        if ((param >= 0.0) && (param <= 1.0))

        {
          p = lines3_1[i].Point(param);
          if (IsBounding(p) != OUTSIDE) {
            rez.push_back(p);
          }
        }
      }
    }
    for (i = 0; i < 3; i++) {
      if (T.IntersectL(lines3_0[i], param) == SKEW) {
        if ((param >= 0.0) && (param <= 1.0))

        {
          p = lines3_0[i].Point(param);
          if (T.IsBounding(p) != OUTSIDE) {
            rez.push_back(p);
          }
        }
      }
    }
    rflag = SMPL_INTRSCT;
  }
  if (rez.size() == 0)
    return NO_INTRSCT;
  // �������� ������������� �����
  for (i = 0; i < rez.size(); i++) {
    for (j = i + 1; j < rez.size(); j++) {
      if (rez[i] == rez[j]) {
        rez.erase(rez.begin() + j);
        j--;
      }
    }
  }
  return rflag;
}
int triangle3d::IsBounding(vector3f p) {
  vector2f vt1[3], ppp;
  float dx, dy, dz, dmin;
  int i;
  dx = Box().b.x - Box().a.x;
  dy = Box().b.y - Box().a.y;
  dz = Box().b.z - Box().a.z;
  dmin = MIN3(dx, dy, dz);
  if (dmin == dx) {
    ppp = p.PrjY0Z();
    for (i = 0; i < 3; i++) {
      vt1[i] = _v[i].PrjY0Z();
    }
  } else if (dmin == dy) {
    ppp = p.PrjX0Z();
    for (i = 0; i < 3; i++) {
      vt1[i] = _v[i].PrjX0Z();
    }
  } else if (dmin == dz) {
    ppp = p.PrjX0Y();
    for (i = 0; i < 3; i++) {
      vt1[i] = _v[i].PrjX0Y();
    }
  }
  return IsInTriangle2ds(vt1, ppp);
}
int triangle3d::HasPoint(vector3f p) {
  vector3f pa, ab, pn;
  pa = p - _v[0];
  ab = _v[1] - _v[0];
  pn = pa.CrossProduct(ab);
  if (pn.CrossProduct(N()).Length() == 0.0)
    return 1;
  else
    return 0;
}
double triangle3d::SGeron() {
  double a, b, c, p;
  vector3f v;
  v = _v[0] - _v[1];
  a = v.Length();
  v = _v[2] - _v[1];
  b = v.Length();
  v = _v[0] - _v[2];
  c = v.Length();
  p = (a + b + c) / 2;
  return sqrt(p * (p - a) * (p - b) * (p - c));
}
// iEdge
bool iEdge::operator==(const iEdge& E) const {
  return (((start == E.start) && (end == E.end)) ||
          ((start == E.end) && (end == E.start)));
}
bool iEdge::HasVertex(int i) {
  return (i == start || i == end);
}
line2d iEdge::Line(vector2f_array p) {
  return line2d(p[start], p[end]);
}
// iFace
iEdge iFace::Edge(int i) {
  iEdge e;
  if (i == 0)
    e = iEdge(a, b);
  if (i == 1)
    e = iEdge(b, c);
  if (i == 2)
    e = iEdge(c, a);
  return e;
}
//
void iFace::ChangeObhod() {
  int i;
  i = a;
  a = b;
  b = i;
  return;
}
bool iFace::HasEdge(iEdge n) {
  return ((n.end == a || n.end == b || n.end == c) &&
          (n.start == a || n.start == b || n.start == c));
}
bool iFace::operator==(iFace f) {
  return (HasEdge(f.Edge(0)) && HasEdge(f.Edge(1)) && HasEdge(f.Edge(2)));
}
//
int iFace::Vertex(int i) {
  if (i == 0)
    return a;
  if (i == 1)
    return b;
  if (i == 2)
    return c;
  return -1;
}
//...................................................................................
int Iscinab(float a, float b, float c) {
  if (a <= b) {
    if ((a <= c) && (c <= b))
      return 1;
  } else if ((b <= c) && (c <= a))
    return 1;
  return 0;
}
//
int IsInTriangle2ds(vector2f* v, vector2f& p) {
  int rez = 0, rez1 = 0;
  int j, k;
  line2d e, ee;
  for (int i = 0; i < 3; i++) {
    if (i < 2) {
      j = i;
      k = i + 1;
    } else {
      j = 2;
      k = 0;
    }
    e = line2d(v[j], v[k]);
    ee = line2d(v[k], v[j]);
    switch (e.ClassifyPoint(p)) {
      case TOUCHING:
        return BOUNDARY;
      case CROSSING:
        rez = 1 - rez;
        break;
    }
    switch (ee.ClassifyPoint(p)) {
      case TOUCHING:
        return BOUNDARY;
      case CROSSING:
        rez1 = 1 - rez1;
        break;
    }
  }
  return rez + rez1;
}
// ������������ ������� 3*3
float det(float x1,
          float y1,
          float z1,
          float x2,
          float y2,
          float z2,
          float x3,
          float y3,
          float z3) {
  return (x1 * y2 * z3 + x3 * y1 * z2 + x2 * y3 * z1 - x3 * y2 * z1 -
          x1 * y3 * z2 - x2 * y1 * z3);
}
float detv(float* x) {
  return det(x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8]);
}
// Bounding Circle
vector3f GetO(vector2f p1, vector2f p2, vector2f p3) {
  float xy1, xy2, xy3;
  xy1 = p1.x * p1.x + p1.y * p1.y;
  xy2 = p2.x * p2.x + p2.y * p2.y;
  xy3 = p3.x * p3.x + p3.y * p3.y;
  vector2f r;
  float a, b, c, d;
  a = det(p1.x, p1.y, 1, p2.x, p2.y, 1, p3.x, p3.y, 1);
  b = det(xy1, p1.y, 1, xy2, p2.y, 1, xy3, p3.y, 1);
  c = det(xy1, p1.x, 1, xy2, p2.x, 1, xy3, p3.x, 1);
  d = det(xy1, p1.x, p1.y, xy2, p2.x, p2.y, xy3, p3.x, p3.y);
  float x, y, r2;
  x = b / (2 * a);
  y = -c / (2 * a);
  r = p1 - vector2f(x, y);
  r2 = r.Length();
  return vector3f(x, y, r2);
}
// ��������� ��������� ������ �� ������������(����)
int TriangulateRing(iEdge_array ring, iFace_array& rez) {
  if (ring.size() == 0)
    return 0;
  int i, n = ring[0].start;
  iFace f;
  for (i = 1; i < (int)ring.size(); i++) {
    if (!ring[i].HasVertex(n)) {
      f = iFace(n, ring[i].start, ring[i].end);
      rez.push_back(f);
    }
  }
  return 1;
}
int TriangulatePolygon1(vector2f_array p, iEdge_array Poly, iFace_array& frez) {
  // ��������� �������������� ������ ����� ��������
  int i, j, k, n, notfound, i1, i2;
  vector3f a, b;
  int_array pn;
  float Z, zz, zz1;
  int obhod = 0;
  pn.push_back(Poly[0].start);
  pn.push_back(Poly[0].end);
  j = 1;
  while (pn.size() < Poly.size()) {
    for (i = 1; i < Poly.size(); i++) {
      if (Poly[i].HasVertex(pn[j]) && !Poly[i].HasVertex(pn[j - 1])) {
        if (Poly[i].start == pn[j])
          pn.push_back(Poly[i].end);
        if (Poly[i].end == pn[j])
          pn.push_back(Poly[i].start);
        j++;
        break;
      }
    }
  }
  // ���������� ��� ��������
  iEdge_array BoundRing;
  GetBoundRing(p, BoundRing);
  notfound = 0;  // ������� ����������
  if (BoundRing.size() == Poly.size()) {
    notfound = 1;
    for (i = 0; i < pn.size(); i++) {
      k = i - 1;
      j = i + 1;
      if (i == 0)
        k = pn.size() - 1;
      if (i == pn.size() - 1)
        j = 0;
      a = vector3f(p[pn[k]] - p[pn[i]]);
      b = vector3f(p[pn[j]] - p[pn[i]]);
      a.Normalize();
      b.Normalize();
      Z = a.CrossProduct(b).z;
      if (Z > -0.001 && Z < 0.001) {
        notfound = 0;
        break;
      }
    }
  }  // ������� ��������

  // ������������
  if (notfound)
    return TriangulateRing(BoundRing, frez);
  else {
    // ����� ����������� ������
    for (i = 0; i < pn.size(); i++) {
      notfound = 1;
      for (j = 0; j < BoundRing.size(); j++)
        if (pn[i] == BoundRing[j].start || pn[i] == BoundRing[j].end) {
          n = i;
          k = n - 1;
          j = n + 1;
          if (n == 0)
            k = pn.size() - 1;
          if (n == pn.size() - 1)
            j = 0;
          a = vector3f(p[pn[k]] - p[pn[n]]);
          b = vector3f(p[pn[j]] - p[pn[n]]);
          a.Normalize();
          b.Normalize();
          Z = a.CrossProduct(b).z;
          if (Z > 0.001 || Z < -0.001) {
            notfound = 0;
            break;
          }
        }
      if (notfound == 0) {
        n = i;
        break;
      }
    }

    i = n - 1;
    j = n + 1;
    if (n == 0)
      i = pn.size() - 1;
    if (n == pn.size() - 1)
      j = 0;
    a = vector3f(p[pn[i]] - p[pn[n]]);
    b = vector3f(p[pn[j]] - p[pn[n]]);
    a.Normalize();
    b.Normalize();
    Z = a.CrossProduct(b).z;
    if (Z < -0.001) {
      obhod = 0;
    }
    if (Z > 0.001) {
      obhod = 1;
    }

    do {
      notfound = 0;
      for (i = 0; i < pn.size(); i++) {
        k = i - 1;
        j = i + 1;
        if (i == 0)
          k = pn.size() - 1;
        if (i == pn.size() - 1)
          j = 0;
        a = vector3f(p[pn[k]] - p[pn[i]]);
        b = vector3f(p[pn[j]] - p[pn[i]]);
        a.Normalize();
        b.Normalize();
        Z = a.CrossProduct(b).z;

        if ((obhod == 1) && (Z < 0.001)) {
          i1 = k;
          i2 = j;

          i = i1;
          k = i - 1;
          j = i + 1;
          if (i == 0)
            k = pn.size() - 1;
          if (i == pn.size() - 1)
            j = 0;
          a = vector3f(p[pn[k]] - p[pn[i]]);
          b = vector3f(p[pn[j]] - p[pn[i]]);
          a.Normalize();
          b.Normalize();
          zz = a.CrossProduct(b).z;
          if (zz > 0.001) {
            frez.push_back(iFace(pn[i], pn[j], pn[k]));
          }

          i = i2;
          k = i - 1;
          j = i + 1;
          if (i == 0)
            k = pn.size() - 1;
          if (i == pn.size() - 1)
            j = 0;
          a = vector3f(p[pn[k]] - p[pn[i]]);
          b = vector3f(p[pn[j]] - p[pn[i]]);
          a.Normalize();
          b.Normalize();
          zz1 = a.CrossProduct(b).z;
          if (zz1 > 0.001) {
            frez.push_back(iFace(pn[i], pn[j], pn[k]));
          }
          if (zz > 0.001)
            pn.erase(pn.begin() + i1);
          if (i1 < i2)
            i2--;
          if (zz1 > 0.001)
            pn.erase(pn.begin() + i2);
          notfound = 1;
          break;
        }
        if ((obhod == 0) && (Z > -0.001)) {
          i1 = k;
          i2 = j;

          i = i1;
          k = i - 1;
          j = i + 1;
          if (i == 0)
            k = pn.size() - 1;
          if (i == pn.size() - 1)
            j = 0;
          a = vector3f(p[pn[k]] - p[pn[i]]);
          b = vector3f(p[pn[j]] - p[pn[i]]);
          a.Normalize();
          b.Normalize();
          zz = a.CrossProduct(b).z;
          if (zz < -0.001) {
            frez.push_back(iFace(pn[i], pn[j], pn[k]));
          }

          i = i2;
          k = i - 1;
          j = i + 1;
          if (i == 0)
            k = pn.size() - 1;
          if (i == pn.size() - 1)
            j = 0;
          a = vector3f(p[pn[k]] - p[pn[i]]);
          b = vector3f(p[pn[j]] - p[pn[i]]);
          a.Normalize();
          b.Normalize();
          zz1 = a.CrossProduct(b).z;
          if (zz1 < -0.001) {
            frez.push_back(iFace(pn[i], pn[j], pn[k]));
          }
          if (zz < -0.001)
            pn.erase(pn.begin() + i1);
          if (i1 < i2)
            i2--;
          if (zz1 < -0.001)
            pn.erase(pn.begin() + i2);
          notfound = 1;
          break;
        }
      }
      if (pn.size() <= 3)
        break;
    } while (notfound);

    if (pn.size() >= 3) {
      for (i = 1; i < pn.size() - 1; i++) {
        frez.push_back(iFace(pn[0], pn[i], pn[i + 1]));
      }
    }
  }

  return 0;
}
// ��������� �������� ���������� ��������(�������) ��������� ������ �����
int GetBoundRing(vector2f_array data, iEdge_array& rez) {
  // ���������� ������ ������������ ����� �� ������ ������ �����
  int i, j, n, k, aclass;
  n = (int)data.size();
  //(n*n-n)/2
  for (i = 0; i < n - 1; i++)
    for (j = i + 1; j < n; j++)
      rez.push_back(iEdge(i, j));
  // ��������� � ����� ������� �� �������� ����� ����� ��� ����� ������
  // ���� ��� � ����� �� ��� ����� �������� � ������ ����� ��� ���������
  for (j = 0; j < (int)rez.size(); j++) {
    k = -1;
    for (i = 0; i < n; i++) {
      aclass = data[i].Classify(data[rez[j].start], data[rez[j].end]);
      if (aclass < 2)  // ����� �� �������� �������� �����
      {
        if (k == -1)
          k = aclass;
        else {
          if (k != aclass) {
            rez.erase(rez.begin() + j);
            j--;
            break;
          }
        }
      }
    }
  }
  for (i = 0; i < n; i++) {
    for (j = 0; j < rez.size(); j++) {
      if (data[i].Classify(data[rez[j].start], data[rez[j].end]) == BETWEEN) {
        rez.erase(rez.begin() + j);
        /// break;
        j--;
      }
    }
  }

  vector2f p;
  p = data[0];
  for (i = 1; i < data.size(); i++)
    if (data[i] < p)
      p = data[i];
  iEdge t;
  for (i = 0; i < rez.size(); i++) {
    if (data[rez[i].start] == p) {
      t = rez[i];
      rez[i] = rez[0];
      rez[0] = t;
      break;
    }
    if (data[rez[i].end] == p) {
      j = rez[i].start;
      rez[i].start = rez[i].end;
      rez[i].end = j;
      t = rez[i];
      rez[i] = rez[0];
      rez[0] = t;
      break;
    }
  }
  return 1;
}
// ������� ������������ ��������� ������ �����
// ������� �������� ��������
// � ���������� �������� ������ ���������� �� ������� ����������� ����������
// �����������
int CreateTriangulationDelone1(vector2f_array& data2d, iFace_array& faces) {
  line2d bBox2d;

  float dx, dy;
  int i, j, k;
  iEdge_array SuperRing;
  iFace_array SRTri;
  vector2f O;
  vector3f_array Circle;

  GetBoundRing(data2d, SuperRing);
  int_array tn;
  for (i = 0; i < data2d.size(); i++) {
    k = 0;
    for (j = 0; j < SuperRing.size(); j++) {
      if (SuperRing[j].HasVertex(i)) {
        k = 1;
        break;
      }
    }
    tn.push_back(k);
  }
  TriangulatePolygon1(data2d, SuperRing, SRTri);
  faces = SRTri;
  for (i = 0; i < faces.size(); i++) {
    Circle.push_back(
        GetO(data2d[faces[i].a], data2d[faces[i].b], data2d[faces[i].c]));
  }
  // ���������� �����
  for (i = 0; i < data2d.size(); i++) {
    if (tn[i] == 0) {
      O = data2d[i];
      SuperRing.clear();
      for (j = 0; j < faces.size(); j++) {
        dx = O.x - Circle[j].x;
        dy = O.y - Circle[j].y;
        if (sqrt(dx * dx + dy * dy) <
            Circle[j].z)  // ����� ������ � ��������� ����������
        {
          for (k = 0; k < 3; k++)
            SuperRing.push_back(faces[j].Edge(k));
          faces.erase(faces.begin() + j);
          Circle.erase(Circle.begin() + j);
          j--;
        }
      }
      // �������� ������������� ����� �� ������ ��� ��������� ������
      for (j = 0; j < SuperRing.size(); j++) {
        for (k = j + 1; k < SuperRing.size(); k++) {
          if (SuperRing[j] == SuperRing[k]) {
            SuperRing.erase(SuperRing.begin() + j);
            j--;
            SuperRing.erase(SuperRing.begin() + k - 1);
            break;
          }
        }
      }
      // ������� ������ �� ������������
      for (j = 0; j < SuperRing.size(); j++) {
        faces.push_back(iFace(SuperRing[j].start, SuperRing[j].end, i));
        Circle.push_back(GetO(data2d[SuperRing[j].start],
                              data2d[SuperRing[j].end], data2d[i]));
      }
    }
  }
  return 1;
}
Mesh::Mesh() {}

Mesh::Mesh(const Mesh& m) {
  if (points.size())
    points.clear();
  if (faces.size())
    faces.clear();
  if (normals.size())
    normals.clear();
  points = m.points;
  faces = m.faces;
  normals = m.normals;
}

Mesh::~Mesh() {
}

int Mesh::set(Mesh m) {
  if (points.size())
    points.clear();
  if (faces.size())
    faces.clear();
  if (normals.size())
    normals.clear();
  points = m.points;
  faces = m.faces;
  normals = m.normals;
  int i;
  box.a = vector3f(100000.0, 100000.0, 100000.0);
  box.b = vector3f(-100000.0, -100000.0, -100000.0);
  for (i = 0; i < points.size(); i++) {
    box.a.x = MIN2(box.a.x, points[i].x);
    box.a.y = MIN2(box.a.y, points[i].y);
    box.a.z = MIN2(box.a.z, points[i].z);
    box.b.x = MAX2(box.b.x, points[i].x);
    box.b.y = MAX2(box.b.y, points[i].y);
    box.b.z = MAX2(box.b.z, points[i].z);
  }
  return 1;
}
int Mesh::set(vector3f_array p, iFace_array f) {
  if (points.size())
    points.clear();
  if (faces.size())
    faces.clear();
  if (normals.size())
    normals.clear();
  points = p;
  faces = f;
  CalcNormals();
  int i;
  box.a = vector3f(100000.0, 100000.0, 100000.0);
  box.b = vector3f(-100000.0, -100000.0, -100000.0);
  for (i = 0; i < p.size(); i++) {
    box.a.x = MIN2(box.a.x, p[i].x);
    box.a.y = MIN2(box.a.y, p[i].y);
    box.a.z = MIN2(box.a.z, p[i].z);
    box.b.x = MAX2(box.b.x, p[i].x);
    box.b.y = MAX2(box.b.y, p[i].y);
    box.b.z = MAX2(box.b.z, p[i].z);
  }
  return 0;
}
int Mesh::CalcNormals() {
  unsigned int i, j;
  vector3f a, b, n;
  normals.clear();
  for (i = 0; i < faces.size(); i++) {
    a = points[faces[i].b] - points[faces[i].a];
    b = points[faces[i].c] - points[faces[i].a];
    n = a.CrossProduct(b);
    n.Normalize();
    normals.push_back(n);
  }
  return 0;
}
int Mesh::Append(std::ofstream& f) {
  int i, pn, fn;
  pn = points.size();
  fn = faces.size();
  // points

  f << pn << std::endl;
  for (i = 0; i < pn; ++i) {
    f << points[i].x << " " << points[i].y << " " << points[i].z << std::endl;
  }
  // normals
  f << fn << std::endl;
  for (i = 0; i < fn; ++i) {
    f << normals[i].x << " " << normals[i].y << " " << normals[i].z
      << std::endl;
  }
  // faces
  f << fn << std::endl;
  for (i = 0; i < fn; i++)
    f << faces[i].a << " " << faces[i].b << " " << faces[i].c << std::endl;
  return 1;
}
int Mesh::Save(const std::string& filename) {
  std::ofstream ostrm(filename);
  Append(ostrm);
  return 1;
}
int Mesh::Extract(std::ifstream& f) {
  int i, pn, fn;
  // points
  f >> pn;
  points.reserve(pn);
  for (i = 0; i < pn; ++i) {
    vector3f p;
    f >> p.x >> p.y >> p.z;
    points.emplace_back(p);
  }
  // normals
  f >> fn;
  normals.reserve(fn);
  for (i = 0; i < fn; ++i) {
    vector3f p;
    f >> p.x >> p.y >> p.z;
    normals.emplace_back(p);
  }
  // faces
  f >> fn;
  faces.reserve(fn);
  for (i = 0; i < fn; ++i) {
    iFace face;
    f >> face.a >> face.b >> face.c;
    faces.emplace_back(face);
  }
  return 1;
}
int Mesh::Load(const std::string& filename) {
  std::ifstream istrm(filename);
  Extract(istrm);
  return 1;
}
// BooleanMesh
BooleanMesh::BooleanMesh() {}

Mesh BooleanMesh::GetA() {
  return _A;
}

Mesh BooleanMesh::GetB() {
  return _B;
}

BooleanMesh::~BooleanMesh() {
}
void Mesh::Intersect(Mesh B, Mesh& rez, iEdge_array& board)  // �������� �����
{
  unsigned int i, j, k, l;

  int same, simple, vertex_not_in_ring;
  int triangulate_type;
  int not_found;
  int flag, k1;
  vector3f norm;
  board.clear();
  vector3f_array rez_p, all_points, intrsct_points;
  vector2f_array temp_points;
  iEdge_array board_edges, all_edges, intrsct_ring;
  iEdge intrsct_edge;
  std::vector<iEdge_array> all_rings;
  iFace_array all_faces, ring_faces, board_faces, rez_f;
  triangle3d a, b;
  int_array numbers;
  rez_p = points;
  for (i = 0; i < faces.size(); i++) {  // main
    all_points.clear();
    all_faces.clear();
    all_rings.clear();
    all_edges.clear();

    vertex_not_in_ring = same = simple = 0;
    triangulate_type = 0;

    a = triangle3d(points[faces[i].a], points[faces[i].b], points[faces[i].c]);
    for (j = 0; j < B.faces.size(); j++) {  // j
      intrsct_points.clear();
      b = triangle3d(B.points[B.faces[j].a], B.points[B.faces[j].b],
                     B.points[B.faces[j].c]);
      flag = a.IntersectT(b, intrsct_points);
      // ���������� ����� � ������ ������
      if (flag != NO_INTRSCT) {
        numbers.clear();
        for (k = 0; k < intrsct_points.size(); k++) {
          not_found = 1;
          for (l = 0; l < all_points.size(); l++) {
            if (intrsct_points[k] == all_points[l]) {
              not_found = 0;
              numbers.push_back(l);
              break;
            }
          }
          if (not_found) {
            numbers.push_back(all_points.size());
            all_points.push_back(intrsct_points[k]);
          }
        }
        if (flag == SMPL_INTRSCT) {  // SMPL_INTRSCT
          simple = 1;
          if (intrsct_points.size() > 1) {  // if
            intrsct_edge = iEdge(numbers[0], numbers[1]);
            // ���������� ����� � ������ ������ �����
            not_found = 1;
            for (k = 0; k < all_edges.size(); k++) {
              if (intrsct_edge == all_edges[k]) {
                not_found = 0;
                break;
              }
            }
            if (not_found) {
              all_edges.push_back(intrsct_edge);
            }
          }                                // if
        }                                  // SMPL_INTRSCT
        if (flag == SAME_PLANE_INTRSCT) {  // SAME_PLANE_INTRSCT
          same = 1;
          if (intrsct_points.size() > 2) {  // intrsct_points.size()>2
            temp_points.clear();
            intrsct_ring.clear();
            Prj3dTo2d(intrsct_points, temp_points);
            GetBoundRing(temp_points, intrsct_ring);
            // ��������� ������� ������ � ������
            for (k = 0; k < intrsct_ring.size(); k++) {
              intrsct_ring[k].start = numbers[intrsct_ring[k].start];
              intrsct_ring[k].end = numbers[intrsct_ring[k].end];
            }
            all_rings.push_back(intrsct_ring);
          }  // intrsct_points.size()>2
        }    // SAME_PLANE_INTRSCT
      }
    }  // j
    if (all_points.size() == 0) {
      // ��� �����������
      all_faces.push_back(faces[i]);
    } else {  // ����� ������������
      // ���������� ������ i ������������ � ������ ����� ��� ������������
      vertex_not_in_ring = 0;
      for (j = 0; j < 3; j++) {
        not_found = 1;
        for (k = 0; k < all_points.size(); k++) {
          if (a[j] == all_points[k]) {
            not_found = 0;
            break;
          }
        }
        if (not_found) {
          all_points.push_back(a[j]);
          vertex_not_in_ring = 1;
        }
      }
      // ����������� ���� ������������
      triangulate_type = 0;  // ��� ����
      if (same == 1 && all_rings.size() != 0) {
        if (vertex_not_in_ring == 0)
          triangulate_type = 1;  // ������
        else
          triangulate_type = 2;  // ������� � ���������� �� ������ �����
      } else {
        triangulate_type = 3;  // ������� � ���������� �� ������ �����
      }
      // ������������
      switch (triangulate_type) {
        case 1:
          // ������
          all_faces.clear();
          for (j = 0; j < all_rings.size(); j++) {
            ring_faces.clear();
            TriangulateRing(all_rings[j], ring_faces);
            // ���������� ������������� � ������ ������
            for (k = 0; k < ring_faces.size(); k++) {
              not_found = 1;
              for (l = 0; l < all_faces.size(); l++) {
                if (ring_faces[k] == all_faces[l]) {
                  not_found = 0;
                  break;
                }
              }
              if (not_found) {
                all_faces.push_back(ring_faces[k]);
              }
            }
          }
          // ������ ����� �������
          board_edges.clear();
          for (j = 0; j < all_faces.size(); j++) {
            for (l = 0; l < 3; l++) {
              not_found = 1;
              for (k = 0; k < board_edges.size(); k++) {
                if (all_faces[j].Edge(l) == board_edges[k]) {
                  not_found = 0;
                  break;
                }
              }
              if (not_found) {
                board_edges.push_back(all_faces[j].Edge(l));
              }
            }
          }
          // ������
          break;
        case 2:
          // ������� � ���������� �� ������ �����
          // ���������� ������ �����������
          board_edges.clear();
          all_edges.clear();
          for (j = 0; j < all_rings.size(); j++) {
            for (k = 0; k < all_rings[j].size(); k++) {
              not_found = 1;
              for (l = 0; l < all_edges.size(); l++) {
                if (all_edges[l] == all_rings[j][k]) {
                  not_found = 0;
                  break;
                }
              }
              if (not_found) {
                all_edges.push_back(all_rings[j][k]);
              }
            }
          }
          // ������������
          temp_points.clear();
          Prj3dTo2d(all_points, temp_points);
          ring_faces.clear();
          TriangulationC(temp_points, all_edges, ring_faces);
          // �������� �� ��� ������������� ������� ������ ������ ����
          // ������
          all_faces.clear();
          for (j = 0; j < ring_faces.size(); j++) {
            not_found = 1;
            for (k = 0; k < all_rings.size(); k++) {
              flag = 0;
              for (l = 0; l < all_rings[k].size(); l++) {
                if (all_rings[k][l].HasVertex(ring_faces[j].a))
                  flag++;
                if (all_rings[k][l].HasVertex(ring_faces[j].b))
                  flag++;
                if (all_rings[k][l].HasVertex(ring_faces[j].c))
                  flag++;
              }
              if (flag == 6) {
                not_found = 0;
                break;
              }
            }
            if (not_found) {
              all_faces.push_back(ring_faces[j]);
            }
          }
          // ���������� ������������ �����
          for (j = 0; j < all_rings.size(); j++) {
            ring_faces.clear();
            TriangulateRing(all_rings[j], ring_faces);
            // ���������� ������������� � ������ ������
            for (k = 0; k < ring_faces.size(); k++) {
              not_found = 1;
              for (l = 0; l < all_faces.size(); l++) {
                if (ring_faces[k] == all_faces[l]) {
                  not_found = 0;
                  break;
                }
              }
              if (not_found) {
                all_faces.push_back(ring_faces[k]);
                // ������ ����� �������

                for (l = 0; l < 3; l++) {
                  not_found = 1;
                  for (k1 = 0; k1 < board_edges.size(); k1++) {
                    if (ring_faces[k].Edge(l) == board_edges[k1]) {
                      not_found = 0;
                      break;
                    }
                  }
                  if (not_found) {
                    board_edges.push_back(ring_faces[k].Edge(l));
                  }
                }

                //
              }
            }
          }
          // ������� � ���������� �� ������ �����
          break;
        case 3:
          // ������� ������������
          temp_points.clear();
          Prj3dTo2d(all_points, temp_points);
          TriangulationC(temp_points, all_edges, all_faces);

          board_edges.clear();
          for (j = 0; j < all_edges.size(); j++)
            board_edges.push_back(all_edges[j]);
          // ������� ������������
          break;
        default:
          // ��� �����������
          all_faces.push_back(faces[i]);
      }

    }  // ����� ������������
    // ���������� ������ ����� � ����������
    numbers.clear();
    for (j = 0; j < all_points.size(); j++) {
      not_found = 1;
      for (k = 0; k < rez_p.size(); k++) {
        if (all_points[j] == rez_p[k]) {
          not_found = 0;
          numbers.push_back(k);
          break;
        }
      }
      if (not_found) {
        numbers.push_back(rez_p.size());
        rez_p.push_back(all_points[j]);
      }
    }
    // ����������� ������� ����� � �������������
    if (numbers.size() != 0) {
      // ����������� ������� ����� � ������ �������
      for (j = 0; j < board_edges.size(); j++) {
        board_edges[j].start = numbers[board_edges[j].start];
        board_edges[j].end = numbers[board_edges[j].end];
      }
      //
      for (j = 0; j < all_faces.size(); j++) {
        all_faces[j].a = numbers[all_faces[j].a];
        all_faces[j].b = numbers[all_faces[j].b];
        all_faces[j].c = numbers[all_faces[j].c];
      }
    }

    // ���������� ���� ����� ������������� � ����������
    for (j = 0; j < all_faces.size(); j++) {
      not_found = 1;
      for (k = 0; k < rez_f.size(); k++) {
        if (all_faces[j] == rez_f[k]) {
          not_found = 0;
          break;
        }
      }
      if (not_found) {
        norm = (rez_p[all_faces[j].a] - rez_p[all_faces[j].c])
                   .CrossProduct(rez_p[all_faces[j].b] - rez_p[all_faces[j].c]);
        norm.Normalize();
        if (norm != normals[i])
          all_faces[j].ChangeObhod();
        rez_f.push_back(all_faces[j]);
      }
    }
    // ���������� ����� ����� ����������� � �������
    for (j = 0; j < board_edges.size(); j++) {
      not_found = 1;
      for (k = 0; k < board.size(); k++) {
        if (board_edges[j] == board[k]) {
          not_found = 0;
          break;
        }
      }
      if (not_found) {
        board.push_back(board_edges[j]);
      }
    }
  }  // main
  rez.set(rez_p, rez_f);
  return;
}
int BooleanMesh::Set(Mesh A, Mesh B) {
  A.Intersect(B, _A, intrsct_edges_A);
  B.Intersect(A, _B, intrsct_edges_B);

  GetNeiborsInfo(_A, Link_A);
  GetNeiborsInfo(_B, Link_B);
  // MessageBox(NULL,NULL,NULL,0);
  CreateGroupList(faces_group_list_B, _B, intrsct_edges_B, Link_B);
  CreateGroupList(faces_group_list_A, _A, intrsct_edges_A, Link_A);

  InOutInfo1(_A, _B, faces_group_list_A, in_out_A, intrsct_edges_A,
             intrsct_edges_B);
  InOutInfo1(_B, _A, faces_group_list_B, in_out_B, intrsct_edges_B,
             intrsct_edges_A);
  return 1;
}
int BooleanMesh::TouchBoard(iFace f, iEdge_array board) {
  // ������������ � ���������� ���������� ����� ������������ �������
  // ����������� � �������
  int i, f0, f1, f2;
  f0 = f1 = f2 = 0;
  for (i = 0; i < board.size(); i++) {
    if (f.Edge(0) == board[i])
      f0 = 1;
    if (f.Edge(1) == board[i])
      f1 = 1;
    if (f.Edge(2) == board[i])
      f2 = 1;
  }
  return f1 + f0 + f2;
}

int BooleanMesh::CreateGroupList(int_array& list,
                                 Mesh A,
                                 iEdge_array intrsct,
                                 iFace_array Link) {
  int i, j, not_found, k, n, col;
  int_array stack;
  // ���������� ���� ������������� ���� -1
  list.clear();
  for (i = 0; i < A.faces.size(); i++) {
    list.push_back(-1);
  }
  // ����� ������������� ��������� ������� � ������� �����������
  // ����� ������������ ���������� ������� 0

  for (i = 0; i < A.faces.size(); i++) {
    col = TouchBoard(A.faces[i], intrsct);
    if (col == 3) {
      list[i] = 0;
    }
  }
  ///
  n = 1;
  for (i = 0; i < A.faces.size(); i++) {
    if (list[i] == -1) {
      list[i] = n;
      col = TouchBoard(A.faces[i], intrsct);
      if (col == 0) {
        if (Link[i].a != -1)
          if (list[Link[i].a] == -1)
            stack.push_back(Link[i].a);
        if (Link[i].b != -1)
          if (list[Link[i].b] == -1)
            stack.push_back(Link[i].b);
        if (Link[i].c != -1)
          if (list[Link[i].c] == -1)
            stack.push_back(Link[i].c);
      } else {  // ��������� ������ �� ��� �� �� �� ������� �� ������� �����
        for (k = 0; k < 3; k++) {
          not_found = 1;
          for (j = 0; j < intrsct.size(); j++) {
            if (A.faces[i].Edge(k) == intrsct[j]) {
              not_found = 0;
              break;
            }
          }
          if (not_found) {
            if (Link[i].Vertex(k) != -1)
              if (list[Link[i].Vertex(k)] == -1)
                stack.push_back(Link[i].Vertex(k));
          }
        }
      }
      while (stack.size() > 0) {
        list[stack[0]] = n;
        col = TouchBoard(A.faces[stack[0]], intrsct);
        if (col == 0) {
          if (Link[stack[0]].a != -1)
            if (list[Link[stack[0]].a] == -1)
              stack.push_back(Link[stack[0]].a);
          if (Link[stack[0]].b != -1)
            if (list[Link[stack[0]].b] == -1)
              stack.push_back(Link[stack[0]].b);
          if (Link[stack[0]].c != -1)
            if (list[Link[stack[0]].c] == -1)
              stack.push_back(Link[stack[0]].c);
        } else {  // ��������� ������ �� ��� �� �� �� ������� �� ������� ��
          // ������� �����
          for (k = 0; k < 3; k++) {
            not_found = 1;
            for (j = 0; j < intrsct.size(); j++) {
              if (A.faces[stack[0]].Edge(k) == intrsct[j]) {
                not_found = 0;
                break;
              }
            }
            if (not_found) {
              if (Link[stack[0]].Vertex(k) != -1)
                if (list[Link[stack[0]].Vertex(k)] == -1)
                  stack.push_back(Link[stack[0]].Vertex(k));
            }
          }
        }
        stack.erase(stack.begin());
      }
      n++;
    }
  }
  return 0;
}
// int BooleanMesh::CreateGroupList(int_array &list, Mesh A, iEdge_array
// intrsct, iFace_array Link)
//{
//	int i,j, not_found, k, n, col;
//	int_array stack;
//	//���������� ���� ������������� ���� -1
//	list.clear();
//	for(i=0;i<A.faces.size();i++)
//	{
//		list.push_back(-1);
//	}
//	//����� ������������� ��������� ������� � ������� �����������
//	//����� ������������ ���������� ������� 0
//
//	for(i=0;i<A.faces.size();i++)
//	{
//		col=TouchBoard(A.faces[i],intrsct);
//		if(col==3)
//		{
//			list[i]=0;
//		}
//	}
//	///
//	n=1;
//	for(i=0;i<A.faces.size();i++)
//	{
//		if(list[i]==-1)
//		{
//			list[i]=n;
//			col=TouchBoard(A.faces[i],intrsct);
//			if(col==0)
//			{
//				if(list[Link[i].a]==-1)stack.push_back(Link[i].a);
//				if(list[Link[i].b]==-1)stack.push_back(Link[i].b);
//				if(list[Link[i].c]==-1)stack.push_back(Link[i].c);
//			}
//			else
//			{//��������� ������ �� ��� �� �� �� ������� �� �������
// ����� 				for(k=0;k<3;k++)
//				{
//					not_found=1;
//					for(j=0;j<intrsct.size();j++)
//					{
//						if(A.faces[i].Edge(k)==intrsct[j])
//						{
//							not_found=0;
//							break;
//						}
//					}
//					if(not_found)
//					{
//						if(list[Link[i].Vertex(k)]==-1)stack.push_back(Link[i].Vertex(k));
//					}
//				}
//			}
//			while(stack.size()>0)
//			{
//				list[stack[0]]=n;
//				col=TouchBoard(A.faces[stack[0]],intrsct);
//				if(col==0)
//				{
//					if(list[Link[stack[0]].a]==-1)stack.push_back(Link[stack[0]].a);
//					if(list[Link[stack[0]].b]==-1)stack.push_back(Link[stack[0]].b);
//					if(list[Link[stack[0]].c]==-1)stack.push_back(Link[stack[0]].c);
//				}
//				else
//				{//��������� ������ �� ��� �� �� �� ������� ��
// ������� �� ������� ����� for(k=0;k<3;k++)
//					{
//						not_found=1;
//						for(j=0;j<intrsct.size();j++)
//						{
//							if(A.faces[stack[0]].Edge(k)==intrsct[j])
//							{
//								not_found=0;
//								break;
//							}
//						}
//						if(not_found)
//						{
//							if(list[Link[stack[0]].Vertex(k)]==-1)stack.push_back(Link[stack[0]].Vertex(k));
//						}
//					}
//				}
//				stack.erase(stack.begin());
//			}
//			n++;
//		}
//	}
//  return 0;
// }

Mesh BooleanMesh::GetMesh(int OpTy) {
  Mesh R;
  vector3f_array allp;
  iFace_array bfaces;
  iFace_array allf;
  int_array numbers;
  int i, j, find;
  for (i = 0; i < _A.points.size(); i++) {
    allp.push_back(_A.points[i]);
  }

  for (i = 0; i < _B.points.size(); i++) {
    find = 0;
    for (j = 0; j < allp.size(); j++) {
      if (_B.points[i] == allp[j]) {
        find = 1;
        numbers.push_back(j);
        break;
      }
    }
    if (find == 0) {
      numbers.push_back(allp.size());
      allp.push_back(_B.points[i]);
    }
  }
  // ��������� �������� ������ � ������ B
  bfaces = _B.faces;
  for (i = 0; i < bfaces.size(); i++) {
    bfaces[i].a = numbers[bfaces[i].a];
    bfaces[i].b = numbers[bfaces[i].b];
    bfaces[i].c = numbers[bfaces[i].c];
  }
  allf.clear();
  if (OpTy == UNION) {
    for (i = 0; i < in_out_A.size(); i++) {
      if (in_out_A[i] == 2 || in_out_A[i] == 1) {
        allf.push_back(_A.faces[i]);
      }
    }
    for (i = 0; i < in_out_B.size(); i++) {
      if (in_out_B[i] == 2) {
        allf.push_back(bfaces[i]);
      }
    }
  }
  if (OpTy == INTERSECTION) {
    for (i = 0; i < in_out_A.size(); i++) {
      if (in_out_A[i] == 3 || in_out_A[i] == 1) {
        allf.push_back(_A.faces[i]);
      }
    }
    for (i = 0; i < in_out_B.size(); i++) {
      if (in_out_B[i] == 3) {
        allf.push_back(bfaces[i]);
      }
    }
  }
  if (OpTy == SUBSTRUCTION) {
    for (i = 0; i < in_out_A.size(); i++) {
      // if(in_out_A[i]==2 || in_out_A[i]==4)
      if (in_out_A[i] == 2) {
        allf.push_back(_A.faces[i]);
      }
    }
    for (i = 0; i < in_out_B.size(); i++) {
      if (in_out_B[i] == 3) {
        bfaces[i].ChangeObhod();
        allf.push_back(bfaces[i]);
      }
    }
  }
  R.set(allp, allf);
  return R;
}
// primitives
int Plane(Mesh* m, float w, float h, int nw, int nh) {
  int np, nf, i, j, k, **ar;
  float dx, dy;
  dx = w / nw;
  dy = h / nh;
  np = (nw + 1) * (nh + 1);
  nf = nw * nh * 2;
  j = 0;
  k = 0;
  for (i = 0; i < np; i++) {
    m->points.push_back(vector3f(j * dx, k * dy, 0.0));
    j++;
    if (j > nw) {
      k++;
      j = 0;
      if (k > nh)
        break;
    }
  }
  ar = new int*[nw + 1];
  for (i = 0; i <= nw; i++)
    ar[i] = new int[nh + 1];
  k = 0;
  for (i = 0; i <= nw; i++) {
    for (j = 0; j <= nh; j++) {
      ar[i][j] = k;
      k++;
    }
  }
  for (i = 0; i < nw; i++) {
    for (j = 0; j < nh; j++) {
      m->faces.push_back(iFace(ar[i][j], ar[i + 1][j + 1], ar[i + 1][j]));
      m->faces.push_back(iFace(ar[i][j], ar[i][j + 1], ar[i + 1][j + 1]));
    }
  }
  for (i = 0; i < nf; i++)
    m->normals.push_back(vector3f(0, 0, -1));
  for (i = 0; i <= nw; i++) {
    delete ar[i];
  }
  delete ar;
  return 1;
}
///////////////////////////////////////////////////////////
int Cube(Mesh* M, float w, float l, float h, int nw, int nl, int nh) {
  int i, j, k, n, **num, *ar, np, nt, gr[6];
  float dx, dy, dz;
  vector3f p;
  iFace t;
  vector3f_array pv;
  iFace_array tv;
  np = (nw + 1) * (nh + 1) * 2 + (nw + 1) * (nl + 1) * 2 +
       (nh + 1) * (nl + 1) * 2;
  nt = 4 * nw * nh + 4 * nw * nl + 4 * nh * nl;
  dx = w / nw;
  dy = h / nh;
  dz = l / nl;
  k = 0;
  // XY
  num = new int*[nw + 1];
  for (i = 0; i <= nw; i++)
    num[i] = new int[nh + 1];
  for (n = 0; n < 2; n++) {
    for (i = 0; i <= nw; i++) {
      for (j = 0; j <= nh; j++) {
        p = vector3f(i * dx, j * dy, n * l);
        pv.push_back(p);
        num[i][j] = k;
        k++;
      }
    }
    for (i = 0; i < nw; i++) {
      for (j = 0; j < nh; j++) {
        if (n) {
          t = iFace(num[i][j], num[i + 1][j + 1], num[i][j + 1]);
          tv.push_back(t);
          t = iFace(num[i][j], num[i + 1][j], num[i + 1][j + 1]);
          tv.push_back(t);
        } else {
          t = iFace(num[i][j], num[i][j + 1], num[i + 1][j + 1]);
          tv.push_back(t);
          t = iFace(num[i][j], num[i + 1][j + 1], num[i + 1][j]);
          tv.push_back(t);
        }
      }
    }
  }
  for (i = 0; i <= nw; i++)
    delete num[i];
  delete num;
  // XZ
  num = new int*[nw + 1];
  for (i = 0; i <= nw; i++)
    num[i] = new int[nl + 1];
  for (n = 0; n < 2; n++) {
    for (i = 0; i <= nw; i++) {
      for (j = 0; j <= nl; j++) {
        p = vector3f(i * dx, n * h, j * dz);
        pv.push_back(p);
        num[i][j] = k;
        k++;
      }
    }
    for (i = 0; i < nw; i++) {
      for (j = 0; j < nl; j++) {
        if (n) {
          t = iFace(num[i][j], num[i][j + 1], num[i + 1][j + 1]);
          tv.push_back(t);
          t = iFace(num[i][j], num[i + 1][j + 1], num[i + 1][j]);
          tv.push_back(t);
        } else {
          t = iFace(num[i][j], num[i + 1][j + 1], num[i][j + 1]);
          tv.push_back(t);
          t = iFace(num[i][j], num[i + 1][j], num[i + 1][j + 1]);
          tv.push_back(t);
        }
      }
    }
  }
  for (i = 0; i <= nw; i++)
    delete num[i];
  delete num;
  // YZ
  num = new int*[nh + 1];
  for (i = 0; i <= nh; i++)
    num[i] = new int[nl + 1];
  for (n = 0; n < 2; n++) {
    for (i = 0; i <= nh; i++) {
      for (j = 0; j <= nl; j++) {
        p = vector3f(n * w, i * dy, j * dz);
        pv.push_back(p);
        num[i][j] = k;
        k++;
      }
    }
    for (i = 0; i < nh; i++) {
      for (j = 0; j < nl; j++) {
        if (n) {
          t = iFace(num[i][j], num[i + 1][j + 1], num[i][j + 1]);
          tv.push_back(t);
          t = iFace(num[i][j], num[i + 1][j], num[i + 1][j + 1]);
          tv.push_back(t);
        } else {
          t = iFace(num[i][j], num[i][j + 1], num[i + 1][j + 1]);
          tv.push_back(t);
          t = iFace(num[i][j], num[i + 1][j + 1], num[i + 1][j]);
          tv.push_back(t);
        }
      }
    }
  }
  if (k != np)
    return 0;
  ar = new int[np];
  for (i = 0; i < np; i++)
    ar[i] = -1;
  // ����� ������������� �����
  for (i = 0; i < np; i++) {
    for (j = i + 1; j < np; j++) {
      if ((pv[i] == pv[j]) && (ar[j] == -1))
        ar[j] = i;
    }
  }
  // ���������� �����
  for (i = 0; i < np; i++) {
    if (ar[i] != -1) {
      for (j = 0; j < nt; j++) {
        if (tv[j].a == i)
          tv[j].a = ar[i];
        if (tv[j].b == i)
          tv[j].b = ar[i];
        if (tv[j].c == i)
          tv[j].c = ar[i];
      }
    }
  }
  // �������
  gr[0] = nw * nh * 2;
  gr[1] = nw * nh * 4;
  gr[2] = nw * nh * 4 + nw * nl * 2;
  gr[3] = nw * nh * 4 + nw * nl * 4;
  gr[4] = nw * nh * 4 + nw * nl * 4 + nh * nl * 2;
  gr[5] = nw * nh * 4 + nw * nl * 4 + nh * nl * 4;
  for (i = 0; i < nt; i++) {
    if (i < gr[0])
      M->normals.push_back(vector3f(0, 0, -1));
    if ((i >= gr[0]) && (i < gr[1]))
      M->normals.push_back(vector3f(0, 0, 1));
    if ((i >= gr[1]) && (i < gr[2]))
      M->normals.push_back(vector3f(0, -1, 0));
    if ((i >= gr[2]) && (i < gr[3]))
      M->normals.push_back(vector3f(0, 1, 0));
    if ((i >= gr[3]) && (i < gr[4]))
      M->normals.push_back(vector3f(-1, 0, 0));
    if ((i >= gr[4]) && (i < gr[5]))
      M->normals.push_back(vector3f(1, 0, 0));
  }
  j = 0;
  for (i = 0; i < np; i++)
    if (ar[i] == -1)
      j++;
  j = 0;
  for (i = 0; i < np; i++) {
    if (ar[i] == -1) {
      M->points.push_back(pv[i]);
      // ��������� ����� ������ ����� ����� �������� �������������
      if (i != j) {
        for (k = 0; k < nt; k++) {
          if (tv[k].a == i)
            tv[k].a = j;
          if (tv[k].b == i)
            tv[k].b = j;
          if (tv[k].c == i)
            tv[k].c = j;
        }
      }
      j++;
    }
  }
  // �����
  for (i = 0; i < nt; i++)
    M->faces.push_back(tv[i]);
  // ������� �����
  delete ar;
  for (i = 0; i <= nh; i++)
    delete num[i];
  delete num;
  pv.clear();
  tv.clear();
  return 1;
}

int Sphere(Mesh* M, float R, int msegm, int rsegm) {
  int i, j, k, np, nt, **ar, k1;
  float Ry, x, y, z, da, dr;
  np = (msegm - 1) * rsegm + 2;
  nt = (msegm - 1) * rsegm * 2;
  M->points.push_back(vector3f(0, R, 0));
  ar = new int*[msegm - 1];
  for (i = 0; i < msegm - 1; i++)
    ar[i] = new int[rsegm + 1];
  dr = 2 * R / msegm;
  da = 360.0 / (float)rsegm;
  da = da * M_PI / 180;
  k = 1;
  for (i = 0; i < msegm - 1; i++) {
    y = R - (i + 1) * dr;
    Ry = sqrt(R * R - y * y);
    k1 = k;
    for (j = 0; j <= rsegm; j++) {
      if (j < rsegm) {
        x = Ry * cos(j * da);
        z = Ry * sin(j * da);
        M->points.push_back(vector3f(x, y, z));
        ar[i][j] = k;
        k++;
      } else {
        ar[i][j] = k1;
      }
    }
  }
  M->points.push_back(vector3f(0, -R, 0));
  // �����
  for (i = 0; i < msegm - 2; i++) {
    for (j = 0; j < rsegm; j++) {
      M->faces.push_back(iFace(ar[i][j], ar[i + 1][j + 1], ar[i + 1][j]));
      M->faces.push_back(iFace(ar[i][j], ar[i][j + 1], ar[i + 1][j + 1]));
    }
  }
  for (j = 0; j < rsegm; j++) {
    M->faces.push_back(iFace(ar[0][j], 0, ar[0][j + 1]));
    M->faces.push_back(iFace(ar[msegm - 2][j], ar[msegm - 2][j + 1], np - 1));
  }
  // �������
  vector3f_array na;
  vector3f V;
  int a, b, c;
  for (i = 0; i < np; i++) {
    na.push_back(M->points[i]);
    na[i].Normalize();
  }
  for (i = 0; i < nt; i++) {
    a = M->faces[i].a;
    b = M->faces[i].b;
    c = M->faces[i].c;
    V = vector3f(na[a].x + na[b].x + na[c].x, na[a].y + na[b].y + na[c].y,
                 na[a].z + na[b].z + na[c].z);
    V.Normalize();
    M->normals.push_back(V);
  }
  for (i = 0; i < msegm - 1; i++)
    delete ar[i];
  delete ar;
  na.clear();
  return 1;
}
int Cone(Mesh* m, float R, float H, int nr, int nH) {
  int i, j, k, np, nt, **ar, nh;
  float dh, da, x, y, z, h, Ry;
  vector3f a, b;
  int A, B, C;
  nh = nH;
  dh = H / nh;
  h = H - dh;
  nh = nH - 1;
  np = nr * (nh + 1) + 2;
  nt = nr * (nh + 1) * 2;
  da = 2 * M_PI / nr;
  ar = new int*[nh + 1];
  for (i = 0; i <= nh; i++)
    ar[i] = new int[nr + 1];
  // vertices
  k = 0;
  for (i = 0; i <= nh; i++) {
    y = dh * i;
    Ry = R * (H - y) / H;
    for (j = 0; j <= nr; j++) {
      if (j < nr) {
        x = Ry * cos(j * da);
        z = Ry * sin(j * da);
        m->points.push_back(vector3f(x, y, z));
        ar[i][j] = k;
        k++;
      } else
        ar[i][j] = ar[i][0];
    }
  }
  m->points.push_back(vector3f(0, 0, 0));
  k++;
  m->points.push_back(vector3f(0, H, 0));
  // faces
  for (i = 0; i < nh; i++) {
    for (j = 0; j < nr; j++) {
      m->faces.push_back(iFace(ar[i][j], ar[i + 1][j], ar[i + 1][j + 1]));
      m->faces.push_back(iFace(ar[i][j], ar[i + 1][j + 1], ar[i][j + 1]));
    }
  }
  for (j = 0; j < nr; j++) {
    m->faces.push_back(iFace(ar[0][j], ar[0][j + 1], np - 2));
    m->faces.push_back(iFace(ar[nh][j], np - 1, ar[nh][j + 1]));
  }
  for (i = 0; i <= nh; i++)
    delete ar[i];
  delete ar;
  // normals
  for (i = 0; i < nt; i++) {
    A = m->faces[i].a;
    B = m->faces[i].b;
    C = m->faces[i].c;
    a = (m->points[A] - m->points[C]);  // CA
    b = (m->points[B] - m->points[C]);  // CB
    m->normals.push_back(a.CrossProduct(b));
    m->normals[i].Normalize();
    if (i >= nt - 2 * nr)
      m->normals[i].Flip();
  }
  return 1;
}
int Cilindr(Mesh* m, float R, float h, int nr, int nh) {
  int i, j, k, np, nt, **ar;
  float dh, da, x, y, z;
  vector3f a, b;
  int A, B, C;
  np = nr * (nh + 1) + 2;
  nt = nr * (nh + 1) * 2;
  dh = h / nh;
  da = 2 * M_PI / nr;
  ar = new int*[nh + 1];
  for (i = 0; i <= nh; i++)
    ar[i] = new int[nr + 1];
  // vertices
  k = 0;
  for (i = 0; i <= nh; i++) {
    y = dh * i;
    for (j = 0; j <= nr; j++) {
      if (j < nr) {
        x = R * cos(j * da);
        z = R * sin(j * da);
        m->points.push_back(vector3f(x, y, z));
        ar[i][j] = k;
        k++;
      } else
        ar[i][j] = ar[i][0];
    }
  }
  m->points.push_back(vector3f(0, 0, 0));
  m->points.push_back(vector3f(0, h, 0));
  // faces
  k = 0;
  for (i = 0; i < nh; i++) {
    for (j = 0; j < nr; j++) {
      m->faces.push_back(iFace(ar[i][j], ar[i + 1][j], ar[i + 1][j + 1]));
      m->faces.push_back(iFace(ar[i][j], ar[i + 1][j + 1], ar[i][j + 1]));
    }
  }
  for (j = 0; j < nr; j++) {
    m->faces.push_back(iFace(ar[0][j], ar[0][j + 1], np - 2));
    m->faces.push_back(iFace(ar[nh][j], np - 1, ar[nh][j + 1]));
  }
  for (i = 0; i <= nh; i++)
    delete ar[i];
  delete ar;
  // normals
  for (i = 0; i < nt; i++) {
    A = m->faces[i].a;
    B = m->faces[i].b;
    C = m->faces[i].c;
    a = (m->points[A] - m->points[C]);  // CA
    b = (m->points[B] - m->points[C]);  // CB
    m->normals.push_back(a.CrossProduct(b));
    m->normals[i].Normalize();
    if (i >= nt - 2 * nr)
      m->normals[i].Flip();
  }
  return 1;
}

int Piramid(Mesh* m, float h, float w, float l, int nw, int nl, int nh) {
  int i, j, k, **ar, **sloy, np, nt, x, kx, k2, I;
  float dh, dx, dz;
  vector3f a, b, *vec;
  int A, B, C;
  x = (nw + nl) * 2;  // ���������� ����� ��������� �� �������
  np = (nh - 1) * x + 1 + (nw + 1) * (nl + 1);
  nt = 2 * nw * nl + 2 * nw + 2 * nl + (nh - 1) * (4 * nw + 4 * nl);
  dh = h / nh;
  dx = w / nw;
  dz = l / nl;
  sloy = new int*[2];  // ��� �������� ������� ����� ������� ����
  for (i = 0; i < 2; i++)
    sloy[i] = new int[x + 1];
  ar = new int*[nw + 1];  // ��� �������� ������� ����� "���"
  for (i = 0; i <= nw; i++)
    ar[i] = new int[nl + 1];
  vec = new vector3f[x];  // ��� �������� ������������ ��������
  m->points.push_back(vector3f(0, h, 0));
  k = 1;  // �� ������ ����� ��� ������� � ������
  // ��������� "���"
  for (i = 0; i <= nw; i++) {
    for (j = 0; j <= nl; j++) {
      m->points.push_back(vector3f(-w / 2 + dx * i, 0, -l / 2 + dz * j));
      ar[i][j] = k;
      k++;
    }
  }
  kx = 0;
  for (i = 0; i <= nw; i++) {
    a = (m->points[0] - m->points[ar[i][0]]);
    vec[kx] = a;
    sloy[0][kx] = ar[i][0];
    kx++;
  }
  for (i = 1; i < nl; i++) {
    a = (m->points[0] - m->points[ar[nw][i]]);
    vec[kx] = a;
    sloy[0][kx] = ar[nw][i];
    kx++;
  }
  for (i = nw; i >= 0; i--) {
    a = (m->points[0] - m->points[ar[i][nl]]);
    vec[kx] = a;
    sloy[0][kx] = ar[i][nl];
    kx++;
  }
  for (i = nl - 1; i >= 1; i--) {
    a = (m->points[0] - m->points[ar[0][i]]);
    vec[kx] = a;
    sloy[0][kx] = ar[0][i];
    kx++;
  }
  sloy[0][kx] = sloy[0][0];
  for (i = 0; i <= x; i++) {
    sloy[1][i] = sloy[0][i];
  }
  // �������� ����� �������-����
  for (i = 0; i < x; i++) {
    vec[i].x /= nh;
    vec[i].y /= nh;
    vec[i].z /= nh;
  }
  // ����� ���
  k2 = 0;
  for (i = 0; i < nw; i++) {
    for (j = 0; j < nl; j++) {
      m->faces.push_back(iFace(ar[i][j], ar[i + 1][j], ar[i + 1][j + 1]));
      k2++;
      m->faces.push_back(iFace(ar[i][j], ar[i + 1][j + 1], ar[i][j + 1]));
      k2++;
    }
  }
  I = 0;  // ���� 0..nh-1
  while (I < nh - 1) {
    for (j = 0; j < x; j++) {
      sloy[1][j] = k;
      a = m->points[sloy[0][j]];
      m->points.push_back(
          vector3f(a.x + vec[j].x, a.y + vec[j].y, a.z + vec[j].z));
      k++;
    }
    sloy[1][x] = sloy[1][0];
    // ����� ����
    for (j = 0; j < x; j++) {
      m->faces.push_back(iFace(sloy[0][j], sloy[1][j], sloy[1][j + 1]));
      k2++;
      m->faces.push_back(iFace(sloy[0][j], sloy[1][j + 1], sloy[0][j + 1]));
      k2++;
    }
    // ������� ��� ������ ����
    for (i = 0; i <= x; i++) {
      sloy[0][i] = sloy[1][i];
    }
    I++;
  };
  // ��������� ����
  for (i = 0; i < x; i++) {
    m->faces.push_back(iFace(sloy[1][i], 0, sloy[0][i + 1]));
    k2++;
  }
  // �������
  for (i = 0; i < nt; i++) {
    A = m->faces[i].a;
    B = m->faces[i].b;
    C = m->faces[i].c;
    a = (m->points[A] - m->points[C]);  // CA
    b = (m->points[B] - m->points[C]);  // CB
    m->normals.push_back(a.CrossProduct(b));
    m->normals[i].Normalize();
  }
  delete vec;
  for (i = 0; i <= nw; i++)
    delete ar[i];
  delete ar;
  for (i = 0; i < 2; i++)
    delete sloy[i];
  delete sloy;
  return 1;
}
int SimpleTriangulation(vector2f_array& data2d, iFace_array& faces) {
  vector2f ar[3];
  int_array numbers, type;
  unsigned int i, j, n, t;
  int flag;
  // ��� ������ ����� �� ������� ������
  for (i = 0; i < data2d.size(); i++) {
    flag = 1;  // ��������� �������� �� ��� � ����� ������ ���� ������������
    for (j = 0; j < faces.size(); j++) {
      if (data2d[i] == data2d[faces[j].a] || data2d[i] == data2d[faces[j].b] ||
          data2d[i] == data2d[faces[j].c]) {
        flag = 0;
        break;
      }
    }
    if (flag == 1)  // ����� �� �������� ����� ������������
    {
      for (j = 0; j < faces.size(); j++) {
        ar[0] = data2d[faces[j].a];
        ar[1] = data2d[faces[j].b];
        ar[2] = data2d[faces[j].c];
        t = IsInTriangle2ds(ar, data2d[i]);
        if (t != OUTSIDE) {
          numbers.push_back(
              j);  // ������ ������� ������������� � ������� ����� �����
          type.push_back(t);  // ��� ���������
        }
      }
      for (j = 0; j < numbers.size(); j++) {
        n = numbers[j];
        if (type[j] == _INSIDE) {
          faces.push_back(iFace(i, faces[n].a, faces[n].b));
          faces.push_back(iFace(i, faces[n].b, faces[n].c));
          faces.push_back(iFace(i, faces[n].c, faces[n].a));
        }
        if (type[j] == BOUNDARY) {
          if (data2d[i].Classify(data2d[faces[n].a], data2d[faces[n].b]) ==
              BETWEEN) {
            faces.push_back(iFace(i, faces[n].c, faces[n].a));
            faces.push_back(iFace(i, faces[n].b, faces[n].c));
          }
          if (data2d[i].Classify(data2d[faces[n].a], data2d[faces[n].c]) ==
              BETWEEN) {
            faces.push_back(iFace(i, faces[n].a, faces[n].b));
            faces.push_back(iFace(i, faces[n].b, faces[n].c));
          }
          if (data2d[i].Classify(data2d[faces[n].b], data2d[faces[n].c]) ==
              BETWEEN) {
            faces.push_back(iFace(i, faces[n].a, faces[n].b));
            faces.push_back(iFace(i, faces[n].c, faces[n].a));
          }
        }
        faces.erase(faces.begin() + n);
      }
      numbers.clear();
      type.clear();
    }
  }
  return 1;
}

int Prj3dTo2d(vector3f_array src, vector2f_array& dst) {
  line3d bBox;
  unsigned int i;
  float dx, dy, dz, delta;
  bBox.a = src[0];
  bBox.b = src[0];
  for (i = 1; i < src.size(); i++) {
    if (bBox.a.x > src[i].x)
      bBox.a.x = src[i].x;
    if (bBox.a.y > src[i].y)
      bBox.a.y = src[i].y;
    if (bBox.a.z > src[i].z)
      bBox.a.z = src[i].z;

    if (bBox.b.x < src[i].x)
      bBox.b.x = src[i].x;
    if (bBox.b.y < src[i].y)
      bBox.b.y = src[i].y;
    if (bBox.b.z < src[i].z)
      bBox.b.z = src[i].z;
  }
  dx = bBox.b.x - bBox.a.x;
  dy = bBox.b.y - bBox.a.y;
  dz = bBox.b.z - bBox.a.z;
  delta = MIN3(dx, dy, dz);
  if (delta == dx) {
    for (i = 0; i < src.size(); i++)
      dst.push_back(src[i].PrjY0Z());
  } else if (delta == dy) {
    for (i = 0; i < src.size(); i++)
      dst.push_back(src[i].PrjX0Z());
  } else if (delta == dz) {
    for (i = 0; i < src.size(); i++)
      dst.push_back(src[i].PrjX0Y());
  }
  return 1;
}
int CreateBoundingRingTriangulation(vector2f_array d2d, iFace_array& faces) {
  iEdge_array ring;
  GetBoundRing(d2d, ring);
  TriangulateRing(ring, faces);
  return 1;
}
int CmpEdgeLength(const void* a, const void* b) {
  Line A, B;
  A = *(Line*)a;
  B = *(Line*)b;
  float la, lb;
  la = (A.l.b - A.l.a).Length();
  lb = (B.l.b - B.l.a).Length();
  if (la >= lb)
    return 1;
  else
    return -1;
}

//.............................................................
void Frame::CalcUVN(vector3f pos, vector3f tag, vector3f up) {
  n = tag - pos;
  n.Normalize();
  v = up;
  v.Normalize();
  u = n.CrossProduct(v);
  u.Normalize();
  v = u.CrossProduct(n);
  v.Normalize();
  return;
}
Frame::Frame() {
  vector3f pos, tag, up;
  pos = vector3f(0, 0, 0);
  tag = vector3f(0, 0, 1);
  up = vector3f(0, 1, 0);
  position = pos;
  CalcUVN(pos, tag, up);
}
Frame::Frame(vector3f _pos, vector3f _tag, vector3f _up) {
  position = _pos;
  CalcUVN(_pos, _tag, _up);
}
void Frame::set(vector3f _pos, vector3f _tag, vector3f _up) {
  position = _pos;
  CalcUVN(_pos, _tag, _up);
}
Frame::Frame(Frame& f) {
  v = f.V();
  n = f.N();
  u = f.U();
  position = f.Position();
}
void Frame::operator=(Frame& f) {
  v = f.V();
  n = f.N();
  u = f.U();
  position = f.Position();
}
void Frame::GetActorMatrix(float m[]) {
  m[0] = u.x;
  m[4] = v.x;
  m[8] = n.x;
  m[12] = position.x;
  m[1] = u.y;
  m[5] = v.y;
  m[9] = n.y;
  m[13] = position.y;
  m[2] = u.z;
  m[6] = v.z;
  m[10] = n.z;
  m[14] = position.z;
  m[3] = 0;
  m[7] = 0;
  m[11] = 0;
  m[15] = 1;
  return;
}
void Frame::GetCameraMatrix(float m[]) {
  vector3f nn = (n * (-1.0));
  m[0] = u.x;
  m[4] = u.y;
  m[8] = u.z;
  m[12] = -(position.DotProduct(u));
  m[1] = v.x;
  m[5] = v.y;
  m[9] = v.z;
  m[13] = -(position.DotProduct(v));
  m[2] = nn.x;
  m[6] = nn.y;
  m[10] = nn.z;
  m[14] = -(position.DotProduct(nn));
  m[3] = 0;
  m[7] = 0;
  m[11] = 0;
  m[15] = 1;
  return;
}

void Frame::RotateN(float angle) {
  float m[4][4];
  LoadRotateMatrix44(m, angle, n.x, n.y, n.z);
  vector3f v1, u1;
  Matrix44XVector3f(v, m, v1);
  v = v1;
  Matrix44XVector3f(u, m, u1);
  u = u1;
  return;
}

void Frame::RotateU(float angle) {
  float m[4][4];
  LoadRotateMatrix44(m, angle, u.x, u.y, u.z);
  vector3f v1, n1;
  Matrix44XVector3f(v, m, v1);
  v = v1;
  Matrix44XVector3f(n, m, n1);
  n = n1;
  return;
}

void Frame::RotateV(float angle) {
  float m[4][4];
  LoadRotateMatrix44(m, angle, v.x, v.y, v.z);
  vector3f u1, n1;
  Matrix44XVector3f(u, m, u1);
  u = u1;
  Matrix44XVector3f(n, m, n1);
  n = n1;
  return;
}

void Frame::GoU(float step) {
  position = position + (u * step);
  return;
}

void Frame::GoV(float step) {
  position = position + (v * step);
  return;
}

void Frame::GoN(float step) {
  position = position + (n * step);
  return;
}
vector3f Frame::N() {
  return n;
}

vector3f Frame::U() {
  return u;
}

vector3f Frame::V() {
  return v;
}

vector3f Frame::Position() {
  return position;
}
Frame::~Frame(){};
//............................................
// ��������� 2� ���������� ������ 4�4 c=a*b
int Matrix44_X_Matrix44(float a[4][4], float b[4][4], float c[4][4]) {
  int i, j, k;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      c[i][j] = 0.0;
      for (k = 0; k < 4; k++) {
        c[i][j] = c[i][j] + a[i][k] * b[k][j];
      }
    }
  }
  return 0;
}
int Vector4_X_Matrix44(float v4[4], float m44[4][4], float rez4[4]) {
  // ������� 4�4 ���������� �� ������ ������! ��������� ���� ������ ������
  int i, j;
  for (i = 0; i < 4; i++) {
    rez4[i] = 0.0;
    for (j = 0; j < 4; j++) {
      rez4[i] += m44[j][i] * v4[j];
    }
  }
  return 0;
}
int Matrix44_X_Vector4(float v4[4], float m44[4][4], float rez4[4]) {
  // ������� 4�4 ���������� �� ������ �������! ��������� ���� ������ �������
  int i, j;
  for (i = 0; i < 4; i++) {
    rez4[i] = 0.0;
    for (j = 0; j < 4; j++) {
      rez4[i] += m44[i][j] * v4[j];
    }
  }
  return 0;
}

int Matrix44XVector3f(vector3f v, float m[4][4], vector3f& rez) {
  float v4[4], rez4[4];
  v4[0] = v.x;
  v4[1] = v.y;
  v4[2] = v.z;
  v4[3] = 1;
  Matrix44_X_Vector4(v4, m, rez4);
  rez = vector3f(rez4[0], rez4[1], rez4[2]);
  return 0;
}

int LoadIdentityMatrix44(float m44[4][4]) {
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      if (i == j)
        m44[i][j] = 1.0;
      else
        m44[i][j] = 0.0;
    }
  }
  return 0;
}

int LoadTranslateMatrix44(float m44[4][4], float dx, float dy, float dz) {
  LoadIdentityMatrix44(m44);
  m44[0][3] = dx;
  m44[1][3] = dy;
  m44[2][3] = dz;
  return 0;
}

int LoadScaleMatrix44(float m44[4][4], float sx, float sy, float sz) {
  LoadIdentityMatrix44(m44);
  m44[0][0] = sx;
  m44[1][1] = sy;
  m44[2][2] = sz;
  return 0;
}

int LoadRotateMatrix44(float m44[4][4],
                       float angle,
                       float x,
                       float y,
                       float z) {
  // ������� �������� �� ���� angle � �������� ������ ������� {x,y,z}
  float a, c, s;
  a = angle * (float)(M_PI / 180.0);
  c = cos(a);
  s = sin(a);
  LoadIdentityMatrix44(m44);
  m44[0][0] = c + (1 - c) * x * x;
  m44[0][1] = (1 - c) * y * x - s * z;
  m44[0][2] = (1 - c) * z * x + s * y;
  m44[1][0] = (1 - c) * x * y + s * z;
  m44[1][1] = c + (1 - c) * y * y;
  m44[1][2] = (1 - c) * z * y - s * x;
  m44[2][0] = (1 - c) * x * z - s * y;
  m44[2][1] = (1 - c) * y * z + s * x;
  m44[2][2] = c + (1 - c) * z * z;

  return 0;
}
int Matrix44ToMatrix16(float m44[4][4], float m16[16]) {
  int i, j, k = 0;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      m16[k] = m44[j][i];
      k++;
    }
  }
  return 0;
}

int Matrix16ToMatrix44(float m16[16], float m44[4][4]) {
  int i, j, k;
  for (i = 0; i < 16; i++) {
    k = i / 4;
    j = i % 4;
    m44[j][k] = m16[i];
  }
  return 0;
}
int TranspoceMatrix44(float m[4][4], float mt[4][4]) {
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      mt[j][i] = m[i][j];
    }
  }
  return 0;
}
//
int Obhod(vector3f a, vector3f b, vector3f c, int lr)
// lr=0- left =1 -right //return 0 �� ������� 1 - ������
{
  vector3f_array src;
  vector2f_array dst;
  src.push_back(a);
  src.push_back(b);
  src.push_back(c);
  Prj3dTo2d(src, dst);
  float rez;
  rez = (dst[0].x - dst[2].x) * (dst[1].y - dst[2].y) -
        (dst[1].x - dst[2].x) * (dst[0].y - dst[2].y);
  if (lr == 0) {
    if (rez < 0)
      return 0;
    else
      return 1;
  } else {
    if (rez < 0)
      return 1;
    else
      return 0;
  }
}

// ���������� ��������� �� ��� ����� �� ����� �����
int IsInOneLine(vector2f p, vector2f la, vector2f lb) {
  float rez;
  rez = (la.x - p.x) * (lb.y - p.y) - (lb.x - p.x) * (la.y - p.y);
  if (fabs(rez) <= 0.01)
    return 1;
  else
    return 0;
}

int IsInOneLine1(vector2f p, vector2f la, vector2f lb) {
  float rez;
  vector3f a(la - p), b(lb - p);

  rez = fabs(a.CrossProduct(b).z) / 2.0;
  if (rez <= 0.01)
    return 1;
  else
    return 0;
}

int NormalizePoints(vector2f_array src, vector2f_array& dst) {
  int i, min = 0;
  vector2f n;
  float max;
  for (i = 0; i < src.size(); i++)
    if (src[min].Length() > src[i].Length())
      min = i;
  for (i = 0; i < src.size(); i++) {
    n = src[i] - src[min];
    dst.push_back(n);
  }
  max = dst[0].x;
  for (i = 0; i < dst.size(); i++) {
    if (max < dst[i].x)
      max = dst[i].x;
    if (max < dst[i].y)
      max = dst[i].y;
  }
  for (i = 0; i < dst.size(); i++) {
    dst[i].x = dst[i].x * 100 / max;
    dst[i].y = dst[i].y * 100 / max;
  }
  return 1;
}

int TriangulationC(vector2f_array pp, iEdge_array e, iFace_array& frez) {
  iEdge_array all_edges, edges;
  Line* lines;
  unsigned int i, j, k, l, notfound, n, g;
  iEdge edge;
  float t;
  // ������ ������������ �����
  for (i = 0; i < pp.size() - 1; i++) {
    for (j = i + 1; j < pp.size(); j++) {
      notfound = 1;
      for (k = 0; k < e.size(); k++)
        if (e[k] == iEdge(i, j)) {
          notfound = 0;
          break;
        }
      if (notfound)
        all_edges.push_back(iEdge(i, j));
    }
  }
  n = all_edges.size();

  lines = new Line[n];

  for (i = 0; i < n; i++) {
    lines[i].l = all_edges[i].Line(pp);
    lines[i].i = all_edges[i];
  }
  qsort(lines, n, sizeof(Line), CmpEdgeLength);
  // �������� ������ ���� �����
  for (i = 0; i < n; i++) {
    edges.push_back(lines[i].i);
  }
  // ������� ����� ���� �� ��� �������� ��������� �����
  for (i = 0; i < pp.size(); i++) {
    for (j = 0; j < edges.size(); j++) {
      if (edges[j].HasVertex(i) == 0) {
        if (pp[i].Classify(pp[edges[j].start], pp[edges[j].end]) == BETWEEN) {
          edges.erase(edges.begin() + j);
          j--;
        }
      }
    }
  }
  all_edges.clear();
  all_edges = e;
  // ������� �������������� �����
  for (i = 0; i < edges.size(); i++) {
    notfound = 1;
    for (j = 0; j < all_edges.size(); j++) {
      if (all_edges[j].HasVertex(edges[i].start) == 0 &&
          all_edges[j].HasVertex(edges[i].end) == 0) {
        g = edges[i].Line(pp).Cross(all_edges[j].Line(pp), t);
        if (g == SKEW_CROSS) {
          notfound = 0;
          break;
        }
      }
    }
    if (notfound) {
      all_edges.push_back(edges[i]);
    }
  }

  for (i = 0; i < all_edges.size(); i++) {
    for (j = 0; j < pp.size(); j++) {
      if (!all_edges[i].HasVertex(j)) {
        notfound = 0;
        for (k = 0; k < all_edges.size(); k++) {
          if (k != i) {
            if (all_edges[k] == iEdge(j, all_edges[i].start))
              notfound++;
            if (all_edges[k] == iEdge(j, all_edges[i].end))
              notfound++;
          }
        }
        if (notfound == 2) {
          notfound = 0;
          for (l = 0; l < frez.size(); l++) {
            if (frez[l] == iFace(all_edges[i].start, all_edges[i].end, j)) {
              notfound = 1;
              break;
            }
          }
          if (notfound == 0) {
            frez.push_back(iFace(all_edges[i].start, all_edges[i].end, j));
          }
        }
      }
    }
  }
  vector2f ar[3];
  // �������� ������������� � ������� �������� �����
  for (i = 0; i < frez.size(); i++) {
    notfound = 1;
    ar[0] = pp[frez[i].a];
    ar[1] = pp[frez[i].b];
    ar[2] = pp[frez[i].c];
    for (j = 0; j < pp.size(); j++) {
      if ((j != frez[i].a) && (j != frez[i].b) && (j != frez[i].c)) {
        if (IsInTriangle2ds(ar, pp[j])) {
          notfound = 0;
          break;
        }
      }
    }
    if (notfound == 0) {
      frez.erase(frez.begin() + i);
      i--;
    }
  }
  delete lines;
  return 1;
}

int BooleanMesh::GetNeiborsInfo(Mesh m, iFace_array& neibors) {
  unsigned int i, j;
  neibors.clear();
  iFace f;
  for (i = 0; i < m.faces.size(); i++) {
    f.a = f.b = f.c = -1;
    for (j = 0; j < m.faces.size(); j++) {
      if (i != j) {
        if (m.faces[j].HasEdge(m.faces[i].Edge(0)))
          f.a = j;
        if (m.faces[j].HasEdge(m.faces[i].Edge(1)))
          f.b = j;
        if (m.faces[j].HasEdge(m.faces[i].Edge(2)))
          f.c = j;
      }
    }
    neibors.push_back(f);
  }
  return 1;
}

int BooleanMesh::InOutInfo1(Mesh mA,
                            Mesh mB,
                            int_array A_group_flags,
                            int_array& in_out_status,
                            iEdge_array A_board,
                            iEdge_array B_board) {
  int_array CurGrList;
  iEdge_array GrStatusList;
  vector3f na, nb, t, nat;
  iEdge ae, be;
  int i, j, k, l, find;
  float rez;
  GrStatusList.push_back(iEdge(A_group_flags[0], -1));
  for (i = 1; i < A_group_flags.size(); i++) {
    find = 0;
    for (j = 0; j < GrStatusList.size(); j++) {
      if (GrStatusList[j].start == A_group_flags[i]) {
        find = 1;
        break;
      }
    }
    if (find == 0) {
      GrStatusList.push_back(iEdge(A_group_flags[i], -1));
    }
  }

  for (i = 0; i < GrStatusList.size(); i++) {  // main
    // ����������� ������ ������ ������� ������
    CurGrList.clear();
    for (j = 0; j < A_group_flags.size(); j++) {
      if (A_group_flags[j] == GrStatusList[i].start)
        CurGrList.push_back(j);
    }
    // ����� ����� ������� � ��������
    for (j = 0; j < CurGrList.size(); j++) {
      find = 0;
      for (k = 0; k < A_board.size(); k++) {
        for (l = 0; l < 3; l++)
          if (mA.faces[CurGrList[j]].Edge(l) == A_board[k]) {
            find = 1;
            ae = mA.faces[CurGrList[j]].Edge(l);
            break;
          }
        if (find == 1)
          break;
      }
      if (find == 1) {
        na = mA.normals[CurGrList[j]];
        break;
      }
    }
    // ����� ����� ������� �� 2 ������
    for (j = 0; j < B_board.size(); j++) {
      if (((mA.points[ae.start] == mB.points[B_board[j].start]) &&
           (mA.points[ae.end] == mB.points[B_board[j].end])) ||
          ((mA.points[ae.start] == mB.points[B_board[j].end]) &&
           (mA.points[ae.end] == mB.points[B_board[j].start]))) {
        be = B_board[j];
        break;
      }
    }
    // ����� ����� �� 2 ������ ������� � ��������� ������
    for (j = 0; j < mB.faces.size(); j++) {
      if (mB.faces[j].HasEdge(be)) {
        nb = mB.normals[j];
        break;
      }
    }

    t = mA.points[ae.end] - mA.points[ae.start];
    t.Normalize();
    nat = na.CrossProduct(t);
    rez = nat.DotProduct(nb);
    nat.Normalize();
    if (nat == nb)
      GrStatusList[i].end = 1;  // same
    else
      GrStatusList[i].end = 4;  // opposite
    if (rez > 0.001)
      GrStatusList[i].end = 2;  // out
    if (rez < -0.001)
      GrStatusList[i].end = 3;  // in
  }                             // main
  in_out_status.clear();
  for (i = 0; i < A_group_flags.size(); i++)
    in_out_status.push_back(0);

  for (i = 0; i < GrStatusList.size(); i++) {
    for (j = 0; j < A_group_flags.size(); j++) {
      if (GrStatusList[i].start == A_group_flags[j]) {
        in_out_status[j] = GrStatusList[i].end;
      }
    }
  }
  return 1;
}
// ������ � ��������
Schema2d::Schema2d() {}

Schema2d::~Schema2d() {}

void Schema2d::Clear() {
  pTop.clear();
  pFront.clear();
  pLeft.clear();

  eTop.clear();
  eFront.clear();
  eLeft.clear();
}

int Schema2d::Append(std::ofstream& ostrm) {
  ostrm << n_view << " " << n << " " << size_of_n_edge << std::endl;
  auto view_printer = [&ostrm](const std::string& name, const iEdge_array& E,
                               const vector2f_array& P) {
    ostrm << name << E.size();
    for (int i = 0; i < E.size(); ++i) {
      ostrm << std::endl
            << P[E[i].start].x << " " << P[E[i].start].y << " " << P[E[i].end].x
            << " " << P[E[i].end].y;
    }
  };
  view_printer("Front ", eFront, pFront);
  view_printer("Left ", eLeft, pLeft);
  view_printer("Top ", eTop, pTop);
  return 1;
};
int Schema2d::Save(const std::string& filename) {
  std::ofstream ostrm(filename);
  Append(ostrm);
  return 1;
}
int Schema2d::Read(std::ifstream& istrm) {
  Clear();
  istrm >> n_view >> n >> size_of_n_edge;
  auto view_reader = [&istrm](iEdge_array& e, vector2f_array& p) {
    int N, j, k, not_found_e, not_found_s;
    std::string str;
    std::vector<line2d> temp;
    iEdge te;
    if (!temp.empty()) {
      temp.clear();
    }
    istrm >> str >> N;
    temp.reserve(N);
    for (j = 0; j < N; j++) {
      line2d t{};
      istrm >> t.a.x >> t.a.y >> t.b.x >> t.b.y;
      temp.emplace_back(t);
    }
    for (j = 0; j < N; j++) {
      not_found_s = 1;
      not_found_e = 1;
      for (k = 0; k < p.size(); k++) {
        if (p[k] == temp[j].a) {
          not_found_s = 0;
          te.start = k;
        }
        if (p[k] == temp[j].b) {
          not_found_e = 0;
          te.end = k;
        }
      }
      if (not_found_s) {
        te.start = p.size();
        p.push_back(temp[j].a);
      }
      if (not_found_e) {
        te.end = p.size();
        p.push_back(temp[j].b);
      }
      e.push_back(te);
    }
  };
  view_reader(eFront, pFront);
  view_reader(eTop, pTop);
  view_reader(eLeft, pLeft);
  return 1;
}
int Schema2d::Read(const std::string& filename) {
  std::ifstream istrm(filename);
  Read(istrm);
  return 1;
}

int Schema2d::FindAndApplyO() {
  int i;
  vector2f O;
  float minx, miny, maxx, maxy;
  miny = pFront[0].y;
  maxx = pFront[0].x;
  for (i = 1; i < pFront.size(); i++) {
    if (maxx < pFront[i].x)
      maxx = pFront[i].x;
    if (miny > pFront[i].y)
      miny = pFront[i].y;
  }
  maxy = pTop[0].y;
  for (i = 1; i < pTop.size(); i++)
    if (maxy < pTop[i].y)
      maxy = pTop[i].y;
  minx = pLeft[0].x;
  for (i = 1; i < pLeft.size(); i++)
    if (minx > pLeft[i].x)
      minx = pLeft[i].x;

  O = vector2f((minx + maxx) / 2, (miny + maxy) / 2);

  for (i = 0; i < pFront.size(); i++) {
    pFront[i].x -= O.x;
    pFront[i].x = -pFront[i].x;
    pFront[i].y -= O.y;
  }
  for (i = 0; i < pTop.size(); i++) {
    pTop[i].x -= O.x;
    pTop[i].x = -pTop[i].x;
    pTop[i].y -= O.y;
    pTop[i].y = -pTop[i].y;
  }
  for (i = 0; i < pLeft.size(); i++) {
    pLeft[i].x -= O.x;
    pLeft[i].y -= O.y;
  }
  // ������� ���������� ���
  minxyz = vector3f(pFront[0].x, pFront[0].y, pLeft[0].x);
  maxxyz = minxyz;
  for (i = 1; i < pFront.size(); i++) {
    if (minxyz.x > pFront[i].x)
      minxyz.x = pFront[i].x;
    if (minxyz.y > pFront[i].y)
      minxyz.y = pFront[i].y;

    if (maxxyz.x < pFront[i].x)
      maxxyz.x = pFront[i].x;
    if (maxxyz.y < pFront[i].y)
      maxxyz.y = pFront[i].y;
  }
  for (i = 1; i < pLeft.size(); i++) {
    if (minxyz.z > pLeft[i].x)
      minxyz.z = pLeft[i].x;
    if (maxxyz.z < pLeft[i].x)
      maxxyz.z = pLeft[i].x;
  }
  return 1;
}

int Schema2d::GetBoundRect(line2d& r) {
  vector2f pf, pt, pl;
  int i;
  pf = pFront[0];
  for (i = 1; i < pFront.size(); i++) {
    if (pf.x < pFront[i].x)
      pf.x = pFront[i].x;
    if (pf.y < pFront[i].y)
      pf.y = pFront[i].y;
  }

  pt = pTop[0];
  for (i = 1; i < pTop.size(); i++) {
    if (pf.x < pTop[i].x)
      pf.x = pTop[i].x;
    if (pf.y < pTop[i].y)
      pf.y = pTop[i].y;
  }

  pl = pLeft[0];
  for (i = 1; i < pLeft.size(); i++) {
    if (pf.x < pLeft[i].x)
      pf.x = pLeft[i].x;
    if (pf.y < pLeft[i].y)
      pf.y = pLeft[i].y;
  }
  r.a = vector2f(0, 0);
  r.b = vector2f(MAX2(pf.x, pl.x) * 2, MAX2(pf.y, pt.y) * 2);
  return 1;
}
int Ring::addedge(vector2f a, vector2f b) {
  int i, j, k;
  j = k = -1;
  for (i = 0; i < points.size(); i++) {
    if (points[i] == a)
      j = i;
    if (points[i] == b)
      k = i;
  }
  if (j == -1) {
    j = points.size();
    points.push_back(a);
  }
  if (k == -1) {
    k = points.size();
    points.push_back(b);
  }
  edges.push_back(iEdge(j, k));
  return 1;
}
int ExtrudeRing(Mesh* m,
                iEdge_array Ring,
                vector3f_array points,
                vector3f direct) {
  vector2f_array temp;
  vector3f_array p;
  iFace_array frez;
  iFace f1, f2;
  int i, n, n1;
  vector3f nd, nt;

  Prj3dTo2d(points, temp);
  TriangulatePolygon1(temp, Ring, frez);
  // �������������� ����������� ������ ������
  nd = direct;
  nd.Normalize();

  for (i = 0; i < frez.size(); i++) {
    nt = (points[frez[i].b] - points[frez[i].a])
             .CrossProduct(points[frez[i].c] - points[frez[i].a]);
    nt.Normalize();
    if (nd.DotProduct(nt) > 0.0)
      frez[i].ChangeObhod();
  }

  // ���������� ����� ������ ������
  n = points.size();
  p = points;
  for (i = 0; i < n; i++) {
    p.push_back(points[i] + direct);
  }
  // ����������� ������������ ��������� ������
  n1 = frez.size();
  for (i = 0; i < n1; i++) {
    f1.a = frez[i].a + n;
    f1.b = frez[i].b + n;
    f1.c = frez[i].c + n;
    f1.ChangeObhod();
    frez.push_back(f1);
  }
  int j;
  for (i = 0; i < Ring.size(); i++) {
    for (j = 0; j < frez.size(); j++) {
      if (frez[j].Edge(0) == Ring[i]) {
        if (Ring[i].start != frez[j].Edge(0).start) {
          j = Ring[i].start;
          Ring[i].start = Ring[i].end;
          Ring[i].end = j;
          break;
        }
      }
      if (frez[j].Edge(1) == Ring[i]) {
        if (Ring[i].start != frez[j].Edge(1).start) {
          j = Ring[i].start;
          Ring[i].start = Ring[i].end;
          Ring[i].end = j;
          break;
        }
      }
      if (frez[j].Edge(2) == Ring[i]) {
        if (Ring[i].start != frez[j].Edge(2).start) {
          j = Ring[i].start;
          Ring[i].start = Ring[i].end;
          Ring[i].end = j;
          break;
        }
      }
    }
  }
  // ���������� ����� ���������
  for (i = 0; i < Ring.size(); i++) {
    f1 = iFace(Ring[i].end, Ring[i].start, Ring[i].start + n);
    f2 = iFace(Ring[i].start + n, Ring[i].end + n, Ring[i].end);
    frez.push_back(f1);
    frez.push_back(f2);
  }
  m->set(p, frez);
  return 1;
}

void Mesh::operator=(const Mesh& m) {
  set(m);
  return;
}
