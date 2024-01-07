//  ☀️
#include "project.h"

void Project::AddMesh(Mesh m, std::string& name) {
    Meshes.push_back(m);
    MeshNames.push_back(name);
    return;
}

void Project::AddRing(Ring r, const std::string& name) {
    Rings.push_back(r);
    RingNames.push_back(name);
    return;
}

int Project::Save(const std::string& filename) {
    if (schema.eTop.size() == 0 && Meshes.size() == 0 && Rings.size() == 0) {
      std::cout << "Нет необходимости сохранять пустой проект" << std::endl;
      return 0;
    }
    int i, j;
    std::ofstream ostrm(filename);
    /*if(f==NULL){
            //MessageBox(NULL,_T("Ошибка создания файла"),_T("Ошибка"),0);
            return 0;
    }*/
    if (schema.eTop.size() != 0) {
      ostrm << "schema" << std::endl;
      schema.Append(ostrm);
      ostrm << std::endl;
    }
    if (Meshes.size() != 0) {
      ostrm << "meshes" << std::endl;
      ostrm << Meshes.size() << std::endl;  // количество сеток
      for (i = 0; i < Meshes.size(); ++i) {
        ostrm << MeshNames[i] << std::endl;
        Meshes[i].Append(ostrm);
      }
      ostrm << SelectedM << std::endl;
    }
    if (Rings.size() != 0) {
      ostrm << "rings" << std::endl;
      ostrm << Rings.size() << std::endl;
      for (i = 0; i < Rings.size(); ++i) {
        ostrm << RingNames[i] << std::endl;
        ostrm << Rings[i].flag << std::endl;
        ostrm << Rings[i].points.size() << std::endl;
        for (j = 0; j < Rings[i].points.size(); ++j) {
          ostrm << Rings[i].points[j].x << " " << Rings[i].points[j].y
                << std::endl;
        }
        ostrm << Rings[i].edges.size() << std::endl;
        for (j = 0; j < Rings[i].edges.size(); j++) {
          ostrm << Rings[i].edges[j].start << " " << Rings[i].edges[j].end
                << std::endl;
        }
      }
    }
    return 1;
}

int Project::Open(const std::string& filename) {
    Clear();
    std::string str;
    std::ifstream istrm(filename);
    if (!istrm.is_open()) {
      std::cout << "ERROR: file " << filename << " not found!" << std::endl;
      return 0;
    }

    istrm >> str;
    if (str == "schema") {
      schema.Read(istrm);
      flagsch = 1;
      istrm >> str;
    }
    if (str == "meshes") {
      int n = 0;
      istrm >> n;
      for (int i = 0; i < n; ++i) {
        std::string name = {};
        istrm >> name;
        Mesh me;
        me.Extract(istrm);
        AddMesh(me, name);
      }
      istrm >> SelectedM;
      istrm >> str;
    }
    if (str == "rings") {
      int n = 0;
      istrm >> n;
      for (int i = 0; i < n; ++i) {
        Ring ri;
        std::string name = {};
        istrm >> name;
        istrm >> ri.flag;
        int m = 0;
        istrm >> m;
        ri.points.reserve(m);
        for (int j = 0; j < m; ++j) {
          vector2f v2f;
          istrm >> v2f.x >> v2f.y;
          ri.points.emplace_back(v2f);
        }
        istrm >> m;
        ri.edges.reserve(m);
        for (int j = 0; j < m; ++j) {
          iEdge ie;
          istrm >> ie.start >> ie.end;
          ri.edges.emplace_back(ie);
        }
        AddRing(ri, name);
      }
    }
    return 1;
}

void Project::DeleteMesh(int i) {
    Meshes.erase(Meshes.begin() + i);
    MeshNames.erase(MeshNames.begin() + i);
    return;
}

void Project::DeleteRing(int i) {
    Rings.erase(Rings.begin() + i);
    RingNames.erase(RingNames.begin() + i);
    return;
}

void Project::DeleteCurrentMesh() {
    if (SelectedM >= 0) {
      DeleteMesh(SelectedM);
      SelectedM = -1;
    }
    return;
}

void Project::DeleteCurrentRing() {
    if (SelectedR >= 0) {
      DeleteRing(SelectedR);
      SelectedR = -1;
    }
    return;
}

void Project::Clear() {
    if (Rings.size() > 0) {
      Rings.clear();
      RingNames.clear();
    }
    if (Meshes.size() > 0) {
      Meshes.clear();
      MeshNames.clear();
    }
    SelectedR = SelectedM = -1;
    schema.Clear();
    return;
}

Project::Project(){
}

Project::~Project(){
}

