#include "../include/Sphere.hpp"
#include "../include/Vector3.hpp"
#include "../include/Point3.hpp"
#include "../include/Plain.hpp"
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

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
Point3 c_esf2(0,0.18, -dJanela-rEsfera2-.03f);
Point3 cor_esf2(1, 1, 1);
Point3 dif_esf2(.9, .9, .9);
Point3 esp_esf2(.3, .3, .3);

float rEsfera3 = .1f;
Point3 c_esf3(0, 0.35, -dJanela-rEsfera3-.08f);
Point3 cor_esf3(1, 1, 1);
Point3 dif_esf3(.9, .9, .9);
Point3 esp_esf3(.3, .3, .3);

float rEsfera4 = .015f;
Point3 c_esf4(-0.04, 0.35, -dJanela-rEsfera4-.073f);
Point3 cor_esf4(0, 0, 0);
Point3 dif_esf4(.1, .1, .1);
Point3 esp_esf4(.1, .1, .1);

float rEsfera5 = .015f;
Point3 c_esf5(0.04, 0.35, -dJanela-rEsfera5-.073f);
Point3 cor_esf5(0, 0, 0);
Point3 dif_esf5(.1, .1, .1);
Point3 esp_esf5(.1, .1, .1);

Point3 p0(-3, -0.2, -8);
Point3 p1( 3, -0.2, -8);
Point3 p2(-3, -0.2, -20);
Point3 cor_plain1(0, 1, 1);
Point3 dif_plain1(0, 1, 1);
Point3 esp_plain1(.7, .7, .7);

void convertDisplayParaJanela(int display_x, int display_y, float &ndc_x,
                         float &ndc_y) {
  // Adiciona 0.5 para obter o centro do pixel
  ndc_x = -wJanela/2.0f + Dx/2.0f + display_x*Dx;
  ndc_y = hJanela / 2.0f - Dy/2.0f - display_y*Dy;
}

Point3 setColor(const Vector3 &d, float disc, const Object &object,
                Point3 &p_int, const Point3 &posicaoLuz) {

  Point3 obj_color = object.getColor();
  Point3 mat_dif = object.getDiffuse();
  Point3 mat_esp = object.getSpecular();
  Vector3 surface_normal = object.getSurfaceNormal(p_int);

  Point3 cor_amb(obj_color.x*luz_amb.x, obj_color.y*luz_amb.y, obj_color.z*luz_amb.z);
  surface_normal.normalize();
  Vector3 unit_luz(p_int, posicaoLuz);
  unit_luz.normalize();
  Vector3 dir_luz(p_int, posicaoLuz);
  Vector3 d_inv(p_int, posicaoObservador);
  d_inv.normalize();

  // shadow discriminant
  bool shadow_disc = false;
  for(const auto& other : world) {
    if(other.get() == &object) continue; // se ignora
    float temp = other->Intersect(p_int, unit_luz);
    if(temp > 0 && temp < dir_luz.length()) {
      shadow_disc = true;
      break;
    }
  }
  if (shadow_disc == true){
    Point3 cor_final(cor_amb);
    return cor_final;
  }
  // Iluminação difusa
  Point3 luz_dif(1, 1, 1);
  float i_dif = std::max(0.f, dot(surface_normal, unit_luz));
  Point3 cor_dif(
    luz_dif.x*mat_dif.x*i_dif,
    luz_dif.y*mat_dif.y*i_dif,
    luz_dif.z*mat_dif.z*i_dif
  );

  // Intensidade especular
  Point3 luz_esp(1, 1, 1);
  int brightness = 50; // nível de espalhamento da luz
  Vector3 reflection = reflect(surface_normal, unit_luz);
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

    for (int l = 0; l < nLin; l++) {
      for (int c = 0; c < nCol; c++) {

        float x, y;
        convertDisplayParaJanela(c, l, x, y);
        
        //float x = -wJanela / 2.0f + Dx / 2.0f + c * Dx;
        //float y =  hJanela / 2.0f - Dy / 2.0f - l * Dy;


        Vector3 d(x - posicaoObservador.x, y - posicaoObservador.y, -dJanela);
        d.normalize();
        // P(t)= PO + t*d

        float closest_disc = 99999;
        const Object* closest_object = nullptr;
        for (const auto& object : world) {
          float temp = object->Intersect(posicaoObservador, d);
          if(temp >= 0.f && temp < closest_disc){
            closest_disc = temp;
            closest_object = object.get();
          }
        }
        Point3 p_int = posicaoObservador+d*closest_disc;
        //Vector3 hitPoint(d.x * disc, d.y * disc, d.z * disc);

        if (closest_object != nullptr) {
          Point3 cor_final = setColor(d, closest_disc, *closest_object, p_int, posicaoLuz);

          image << (int)(cor_final.x * 255) << " "
                << (int)(cor_final.y * 255) << " "
                << (int)(cor_final.z * 255) << " ";
        } else {
          image << backgroundR << " " << backgroundG << " " << backgroundB << " ";
        }
      }
      image << "\n";
    }

    image.close();
  }
}
