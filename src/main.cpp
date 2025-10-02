#include "../include/Sphere.hpp"
#include "../include/Vector3.hpp"
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iostream>

const float wJanela = 2.f, hJanela = 1.5f;
const int nCol = 2.f*400, nLin = 1.5*400;
float Dx = wJanela / nCol;
float Dy = hJanela / nLin;
float dJanela = 10;
std::string backgroundColor = "100 100 100 ";

// Luz ambiente
Point3 cor_esf(.8, .1, .9);
Point3 luz_amb(.03, .03, .03);
Point3 cor_amb(cor_esf.x*luz_amb.x, cor_esf.y*luz_amb.y, cor_esf.z*luz_amb.z);

Point3 posicaoObservador(0, 0, 0);

void convertDisplayParaJanela(int display_x, int display_y, float &ndc_x,
                         float &ndc_y) {
  // Adiciona 0.5 para obter o centro do pixel
  ndc_x = -wJanela/2.0f + Dx/2.0f + display_x*Dx;
  ndc_y = hJanela / 2.0f - Dy/2.0f - display_y*Dy;
}

float raySphereIntersect(const Point3 &origin, const Vector3 &dir,
                         const Point3 &center, float radius) {
  Vector3 L(origin, center);
  float b = 2.0f * dot(dir, L);
  float c = dot(L, L) - radius * radius;

  float discriminant = b * b - 4.0f * c; // a=1
  if (discriminant < 0)
    return 0.0f;

  float sqrtD = std::sqrt(discriminant);
  float t1 = (-b - sqrtD) * 0.5f;
  float t2 = (-b + sqrtD) * 0.5f;

  float t = std::min(t1, t2); // pega a interseção menor
  if (t < 0)
    return 0.0f; // está atrás do raio

  return t; // retorna a distância até a colisão
}

int main() {
  std::ofstream image("image.ppm");
  float rEsfera = .2f;
  Point3 c_esf(0, 0, -dJanela+rEsfera);
  
  if (image.is_open()) {
    image << "P3\n";
    image << nCol << " " << nLin << "\n";
    image << 255 << "\n";


    for (int l = 0; l < nLin; l++) {
      for (int c = 0; c < nCol; c++) {

        float x, y;
        convertDisplayParaJanela(c, l, x, y);
        
        //float x = -wJanela / 2.0f + Dx / 2.0f + c * Dx;
        //float y =  hJanela / 2.0f - Dy / 2.0f - l * Dy;


        Vector3 d(x - posicaoObservador.x, y - posicaoObservador.y, -dJanela);
        d.normalize();
        // P(t)= O + t*d

        float disc =
            raySphereIntersect(posicaoObservador, d, c_esf, rEsfera);
        Vector3 hitPoint(d.x * disc, d.y * disc, d.z * disc);

        Point3 posicaoLuz(6, 6, 2);

        if (disc > 0.f) {
          Point3 p_int = posicaoObservador+d*disc;

          Vector3 normal(p_int, c_esf);
          normal.normalize();
          Vector3 dir_luz(posicaoLuz, p_int);
          dir_luz.normalize();
          Vector3 d_inv(posicaoObservador, p_int);
          d_inv.normalize();

          // Iluminação difusa
          Point3 luz_dif(1, 1, 1);
          Point3 mat_dif(.75, .05, .85);
          float i_dif = std::max(0.0f, dot(normal, dir_luz));
          Point3 cor_dif(
            luz_dif.x*mat_dif.x*i_dif,
            luz_dif.y*mat_dif.y*i_dif,
            luz_dif.z*mat_dif.z*i_dif
          );

          // Intensidade especular
          Point3 luz_esp(1, 1, 1);
          Point3 mat_esp(.35, .35, .35);
          int brightness = 50; // nível de espalhamento da luz
          Vector3 reflection(reflect(normal, dir_luz));
          float i_esp = pow(std::max(0.0f, dot(reflection, d_inv)), brightness);
          Point3 cor_esp(
            luz_esp.x*mat_esp.x*i_esp,
            luz_esp.y*mat_esp.y*i_esp,
            luz_esp.z*mat_esp.z*i_esp
          );

          Point3 cor_final(cor_amb+cor_dif+cor_esp);
          cor_final.clamp();

          image << (int)(cor_final.x * 255.999) << " "
                << (int)(cor_final.y * 255.999) << " "
                << (int)(cor_final.z * 255.999) << " ";
        } else {
          image << backgroundColor;
        }
      }
      image << "\n";
    }

    image.close();
  }
}
