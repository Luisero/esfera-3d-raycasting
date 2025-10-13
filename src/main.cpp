#include "../include/Sphere.hpp"
#include "../include/Vector3.hpp"
#include "../include/Point3.hpp"
#include "../include/Plain.hpp"
#include "../include/Cilinder.hpp"
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>

const float wJanela = 2.f, hJanela = 1.5f;
const int nCol = 2.f*400, nLin = 1.5*400;
float Dx = wJanela / nCol;
float Dy = hJanela / nLin;
float dJanela = 10;
int backgroundR = 100;
int backgroundG = 100;
int backgroundB = 100;

Point3 posicaoLuz(2, 3, 0);
// Luz ambiente
Point3 luz_amb(.3, .3, .3);

Point3 posicaoObservador(0, 0, 0);

std::vector<std::unique_ptr<Object>> world;

// ------------------ CRIAÇÃO DE OBJETOS ------------------
float rEsfera1 = .18f;
Point3 c_esf1(0, -0.02, -dJanela-rEsfera1);
Point3 cor_esf1(1, 1, 1);
Point3 dif_esf1(.9, .9, .9);
Point3 esp_esf1(.3, .3, .3);

float rEsfera2 = .15f;
Point3 c_esf2(0,0.25, -dJanela-rEsfera2-.03f);
Point3 cor_esf2(1, 1, 1);
Point3 dif_esf2(.9, .9, .9);
Point3 esp_esf2(.3, .3, .3);

float rEsfera3 = .1f;
Point3 c_esf3(0, 0.44, -dJanela-rEsfera3-.08f);
Point3 cor_esf3(1, 1, 1);
Point3 dif_esf3(.9, .9, .9);
Point3 esp_esf3(.3, .3, .3);

// encontrando o z do olho para que ele fique apenas parcialmente fora da cabeça (esfera maior)
float eye_x = 0.04;
float eye_y = 0.46;
float eye_radius = .015f;
Point3 head = c_esf3;
float eye_z = head.z + std::sqrt(pow(rEsfera3, 2) - pow((eye_x - head.x), 2) - pow((eye_y - head.y), 2));

float rEsfera4 = eye_radius;
Point3 c_esf4(-eye_x, eye_y, eye_z);
Point3 cor_esf4(0, 0, 0);
Point3 dif_esf4(.1, .1, .1);
Point3 esp_esf4(.1, .1, .1);

float rEsfera5 = eye_radius;
Point3 c_esf5(eye_x, eye_y, eye_z);
Point3 cor_esf5(0, 0, 0);
Point3 dif_esf5(.1, .1, .1);
Point3 esp_esf5(.1, .1, .1);

Point3 p0(-3, -0.2, -8);
Point3 p1( 3, -0.2, -8);
Point3 p2(-3, -0.2, -20);
Point3 cor_plain1(0, 1, 1);
Point3 dif_plain1(0, 1, 1);
Point3 esp_plain1(.7, .7, .7);

float rCilindro1 = .02f;
Point3 c_cilindro1(-0.13, 0.25, -dJanela-rCilindro1-0.15f);
float h_cilindro1 = 0.2f;
Vector3 dc_cilindro1(-1, 0.2, 0);
bool bottom_cilindro1 = true;
bool upper_cilindro1 = true;
Point3 cor_cilindro1(0.3, 0.18, 0.02);
Point3 dif_cilindro1(0.3, .18, 0.02);
Point3 esp_cilindro1(.3, .3, .3);

float rCilindro2 = 0.02f;
Point3 c_cilindro2(0.13, 0.25, -dJanela-rCilindro2-0.15f);
float h_cilindro2 = 0.2f;
Vector3 dc_cilindro2(1, .2, 0);
bool bottom_cilindro2 = true;
bool upper_cilindro2 = true;
Point3 cor_cilindro2(0.3, 0.18, 0.02);
Point3 dif_cilindro2(0.3, .18, 0.02);
Point3 esp_cilindro2(.3, .3, .3);

float rCilindro3 = 0.12f;
Point3 c_cilindro3(0.04, 0.53, -dJanela-rCilindro3-0.16f);
float h_cilindro3 = 0.01f;
Vector3 dc_cilindro3(1, 2, 0);
bool bottom_cilindro3 = true;
bool upper_cilindro3 = true;
Point3 cor_cilindro3(0.3, 0.18, 0.02);
Point3 dif_cilindro3(0.3, .18, 0.02);
Point3 esp_cilindro3(.3, .3, .3);

float rCilindro4 = 0.06f;
Point3 c_cilindro4(0.04, 0.53, -dJanela-rCilindro4-0.22f);
float h_cilindro4 = 0.2f;
Vector3 dc_cilindro4(1, 2, 0);
bool bottom_cilindro4 = true;
bool upper_cilindro4 = true;
Point3 cor_cilindro4(0.3, 0.18, 0.02);
Point3 dif_cilindro4(0.3, .18, 0.02);
Point3 esp_cilindro4(.3, .3, .3);

void convertDisplayParaJanela(int display_x, int display_y, float &ndc_x,
                         float &ndc_y) {
  // Adiciona 0.5 para obter o centro do pixel
  ndc_x = -wJanela/2.0f + Dx/2.0f + display_x*Dx;
  ndc_y = hJanela / 2.0f - Dy/2.0f - display_y*Dy;
}

Point3 setColor(const Vector3 &d, HitRecord rec, const Point3 &posicaoLuz) {

  Point3 obj_color = rec.obj_ptr->getColor();
  Point3 mat_dif = rec.obj_ptr->getDiffuse();
  Point3 mat_esp = rec.obj_ptr->getSpecular();
  //Vector3 surface_normal = object.getSurfaceNormal(p_int);

  Point3 cor_amb(obj_color.x*luz_amb.x, obj_color.y*luz_amb.y, obj_color.z*luz_amb.z);
  //surface_normal.normalize();
  Vector3 dir_luz(rec.p_int, posicaoLuz);
  float dist_to_light = dir_luz.length();
  dir_luz.normalize();
  Vector3 d_inv(rec.p_int, posicaoObservador);
  d_inv.normalize();

  bool on_shadow = false;
  for(const auto& other : world) {
    if(other.get() == rec.obj_ptr) continue; // se ignora
    HitRecord temp_rec;
    if(other->Intersect(rec.p_int, dir_luz, 0.001f, dist_to_light, temp_rec)){
      on_shadow = true;
      break;
    }
  }
  if (on_shadow){
    return cor_amb;
  }
  // Iluminação difusa
  Point3 luz_dif(1, 1, 1);
  float i_dif = std::max(0.f, dot(rec.normal, dir_luz));
  Point3 cor_dif(
    luz_dif.x*mat_dif.x*i_dif,
    luz_dif.y*mat_dif.y*i_dif,
    luz_dif.z*mat_dif.z*i_dif
  );

  // Intensidade especular
  Point3 luz_esp(1, 1, 1);
  int brightness = 50; // nível de espalhamento da luz
  Vector3 reflection = reflect(rec.normal, dir_luz);
  float i_esp = pow(std::max(0.f, dot(reflection, d_inv)), brightness);
  Point3 cor_esp(
    luz_esp.x*mat_esp.x*i_esp,
    luz_esp.y*mat_esp.y*i_esp,
    luz_esp.z*mat_esp.z*i_esp
  );

  Point3 cor_final(cor_amb+cor_esp+cor_dif);
  cor_final.clamp();
  return cor_final;
}

std::mutex g_image_mutex;

void raycast(std::ofstream &image, int lin_start, int col_start, int width, int height) {
  for (int l = lin_start; l < lin_start+height; l++) {
    for (int c = col_start; c < col_start+width; c++) {
      float x, y;
      convertDisplayParaJanela(c, l, x, y);

      Vector3 d(x - posicaoObservador.x, y - posicaoObservador.y, -dJanela);
      d.normalize();
      // P(t)= PO + t*d

      float closest_so_far = 99999;
      const Object* closest_object = nullptr;
      HitRecord rec;
      bool hit_anything = false;
      for (const auto& object : world) {
        //float temp = object->Intersect(posicaoObservador, d);
        HitRecord temp_rec;
        if(object->Intersect(posicaoObservador, d, 0.001f, closest_so_far, temp_rec)){
          hit_anything = true;
          closest_so_far = temp_rec.t;
          rec = temp_rec;
          closest_object = object.get();
        }
      }
      //Point3 p_int = posicaoObservador+d*closest_disc;
      //Vector3 hitPoint(d.x * disc, d.y * disc, d.z * disc);

      if (hit_anything) {
        Point3 cor_final = setColor(d, rec, posicaoLuz);
        image << (int)(cor_final.x * 255) << " "
              << (int)(cor_final.y * 255) << " "
              << (int)(cor_final.z * 255) << " ";
      } else {
        image << backgroundR << " " << backgroundG << " " << backgroundB << " ";
      }
    }
    image << "\n";
  }
}

int main() {
  std::ofstream image("image.ppm");
  
  if (image.is_open()) {
    image << "P3\n";
    image << nCol << " " << nLin << "\n";
    image << 255 << "\n";

    world.push_back(std::make_unique<Plain>(p0, p1, p2, cor_plain1, dif_plain1, esp_plain1));
    world.push_back(std::make_unique<Sphere>(c_esf3, rEsfera3, cor_esf3, dif_esf3, esp_esf3));
    world.push_back(std::make_unique<Sphere>(c_esf2, rEsfera2, cor_esf2, dif_esf2, esp_esf2));
    world.push_back(std::make_unique<Sphere>(c_esf1, rEsfera1, cor_esf1, dif_esf1, esp_esf1));
    world.push_back(std::make_unique<Sphere>(c_esf4, rEsfera4, cor_esf4, dif_esf4, esp_esf4));
    world.push_back(std::make_unique<Sphere>(c_esf5, rEsfera5, cor_esf5, dif_esf5, esp_esf5));
    world.push_back(std::make_unique<Cilinder>(c_cilindro1, h_cilindro1, rCilindro1, dc_cilindro1, bottom_cilindro1, upper_cilindro1, cor_cilindro1, dif_cilindro1, esp_cilindro1));
    world.push_back(std::make_unique<Cilinder>(c_cilindro2, h_cilindro2, rCilindro2, dc_cilindro2, bottom_cilindro2, upper_cilindro2, cor_cilindro2, dif_cilindro2, esp_cilindro2));
    world.push_back(std::make_unique<Cilinder>(c_cilindro3, h_cilindro3, rCilindro3, dc_cilindro3, bottom_cilindro3, upper_cilindro3, cor_cilindro3, dif_cilindro3, esp_cilindro3));
    world.push_back(std::make_unique<Cilinder>(c_cilindro4, h_cilindro4, rCilindro4, dc_cilindro4, bottom_cilindro4, upper_cilindro4, cor_cilindro4, dif_cilindro4, esp_cilindro4));

    raycast(image, 0, 0, nCol, nLin);

    image.close();
  }
}
