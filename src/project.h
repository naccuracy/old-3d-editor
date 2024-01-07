//  ☀️
#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <vector>
#include "geom.h"

// Project
class Project {
 public:
  // data
  std::vector<std::string> RingNames;
  Ring_array Rings;
  std::vector<std::string> MeshNames;
  std::vector<Mesh> Meshes;
  int SelectedR = -1;
  int SelectedM = -1;
  int flagsch = 0;
  Schema2d schema;
  // members
  void AddMesh(Mesh m, std::string& name);
  void AddRing(Ring r, const std::string& name);
  int Save(const std::string& filename);
  int Open(const std::string& filename);
  void DeleteMesh(int i);
  void DeleteRing(int i);
  void DeleteCurrentMesh();
  void DeleteCurrentRing();
  void Clear();
  Project();
  ~Project();
};

#endif //PROJECT_H
