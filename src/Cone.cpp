#include "../include/Cone.hpp"
#include "../include/Point3.hpp"
#include <vector>
#include <cmath>

Cone::Cone(Point3 &c_base, float radius, bool has_bottom, Point3 &vertice, Point3 &color, Point3 &dif_color, Point3 &esp_color) {
    this->center = c_base;
    this->radius = radius;
    this->has_bottom = has_bottom;
    this->vertice = vertice;
    this->dc = normalize(Vector3(center, vertice));
    this->height = Vector3(center, vertice).length();
    this->color = color;
    this->diffuse_color = dif_color;
    this->specular_color = esp_color;

    // fazendo a matriz resultante do produto entre o vetor dc coluna e vetor dc transposto (M barra)
    std::vector<std::vector<float>> M_barra;
    for(int i = 0; i < 3; i++){
        std::vector<float> linha;
        for(int j = 0; j < 3; j++){
            const float val = this->dc[i]*this->dc[j];
            linha.push_back(val);
        }
        M_barra.push_back(std::vector<float>(linha));
    }
    // fazendo a subtração do vetor identidade pelo M barra (M)
    std::vector<std::vector<float>> M;
    for(int i = 0; i < 3; i++){
        std::vector<float> linha;
        for(int j = 0; j < 3; j++){
            float val = 0 - M_barra[i][j];
            if(i == j) val = 1 - M_barra[i][j];
            linha.push_back(val);
        }
        M.push_back(std::vector<float>(linha));
    }
    // fazendo o cálculo do M* (M_barra - (H/R)² * M)
    float relation = std::pow(height/radius, 2);
    // (H/R)² * M -> M_temp
    std::vector<std::vector<float>> M_temp;
    for(int i = 0; i < 3; i++){
      std::vector<float> linha;
      for(int j = 0; j < 3; j++){
        float val = M[i][j] * relation;
        linha.push_back(val);
      }
      M_temp.push_back(linha);
    }
    // M_barra - M_temp
    for(int i = 0; i < 3; i++){
      std::vector<float> linha;
      for(int j = 0; j < 3; j++){
        float val = M_barra[i][j] - M_temp[i][j];
        linha.push_back(val);
      }
      this->M_star.push_back(linha);
    }
}

bool Cone::Intersect(const Point3 &origin, const Vector3 &dir, float t_min, float t_max, HitRecord &hr) const {
  // calculando os coeficientes a, b e c da equação do cilindro
  // _trans = transposto
  // a = dir_trans*M*dir
  // a_vc = M_star*dir (vetor coluna do a)
  Vector3 a_vc(0, 0, 0);
  for(int i = 0; i < 3; i++){
      float val = 0;
      for(int j = 0; j < 3; j++){
          val += this->M_star[i][j]*dir[j];     
      }
      if(i == 0) a_vc.x = val;
      if(i == 1) a_vc.y = val;
      if(i == 2) a_vc.z = val;
  }
  double a = dot(dir, a_vc);
  Vector3 w(center, origin);
  // b = 2*(w_trans*M_star*dir) - 2*H*dir_trans*dc
  double b = 2*dot(w, a_vc) - 2*height*dot(dir, dc);
  // M_star*w
  Vector3 c_vc(0, 0, 0);
  for(int i = 0; i < 3; i++){
      float val = 0;
      for(int j = 0; j < 3; j++){
          val += this->M_star[i][j]*w[j];     
      }
      if(i == 0) c_vc.x = val;
      if(i == 1) c_vc.y = val;
      if(i == 2) c_vc.z = val;
  }
  double c = height*height - 2*height*dot(w, dc) + dot(w, c_vc);

  double disc = b*b - 4*a*c;
  if(disc < 0) return false;

  double sqrtD = std::sqrt(disc);
  double t1 = (-b + sqrtD)/(2*a);
  double t2 = (-b - sqrtD)/(2*a);

  bool hit_anything = false;

  if(t1 > t_min && t1 < t_max){
    Point3 p_int = origin + t1*dir;
    Vector3 s(center, p_int);
    double alpha = dot(s, dc);
    Vector3 Piv(p_int, vertice);
    Vector3 N = cross(cross(Piv, dc), Piv);
    Vector3 normal = normalize(N);
    if(alpha >= 0 && alpha <= height) {
      hr.t = t1;
      hr.p_int = p_int;
      hr.normal = normal;
      hr.obj_ptr = this;

      hit_anything = true;
    }
  }
  else if(t2 > t_min && t2 < t_max) {
    Point3 p_int = origin + t2*dir;
    Vector3 s(center, p_int);
    double alpha = dot(s, dc);
    Vector3 Piv(p_int, vertice);
    Vector3 N = cross(cross(Piv, dc), Piv);
    Vector3 normal = normalize(N);
    if(alpha >= 0 && alpha <= height) {
      hr.t = t2;
      hr.p_int = p_int;
      hr.normal = normal;
      hr.obj_ptr = this;

      hit_anything = true;
    }
  }

  if(has_bottom){
    double denominator = dot(dir, this->dc);
    if(denominator != 0){
      double t_i = dot(Vector3(origin, center), this->dc)/denominator;
      Point3 p_int_fundo = origin + t_i*dir;
      Vector3 center_to_int(center, p_int_fundo);
      if(center_to_int.length() <= radius){
        hr.t = t_i;
        hr.p_int = p_int_fundo;
        hr.normal = this->dc;
        hr.obj_ptr = this;
        hit_anything = true;
      }
    }
  }

  if(!hit_anything) return false;

  return true;
}