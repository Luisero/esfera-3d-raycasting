#include "../include/Sphere.hpp"
#include "../include/Vector3.hpp"
#include <cmath>
#include <fstream>
#include <iostream>

const float wJanela = 2.f, hJanela = 1.5f;
const int nCol = 2.f*400, nLin = 1.5*400;
float Dx = wJanela / nCol;
float Dy = hJanela / nLin;
float dJanela = 2.f;
float esfColor[3] = {255,0,0};
std::string backgroundColor = "19 38 66 ";

Vector3 posicaoObservador(0, 0, 0);

void convertDisplayParaJanela(int display_x, int display_y, float &ndc_x,
                         float &ndc_y) {
  // Adiciona 0.5 para obter o centro do pixel
  ndc_x = -wJanela/2.0f + Dx/2.0f + display_x*Dx;
  ndc_y = hJanela / 2.0f - Dy/2.0f - display_y*Dy;
}

float raySphereIntersect(const Vector3 &origin, const Vector3 &dir,
                         const Vector3 &center, float radius) {
  Vector3 L = origin - center;
  float b = 2.0f * dir.dot(L);
  float c = L.dot(L) - radius * radius;

  float discriminant = b * b - 4.0f * c; // a=1
  if (discriminant < 0)
    return 0.0f;

  float sqrtD = std::sqrt(discriminant);
  float t1 = (-b - sqrtD) * 0.5f;
  float t2 = (-b + sqrtD) * 0.5f;

  float t = (t1 > t2) ? t2 : t1; // pega a interseção menor
  if (t < 0)
    return 0.0f; // está atrás do raio

  return t; // retorna a distância até a colisão
}

int main() {
  std::ofstream image("image.ppm");
  Vector3 vec(1, 1, 1);
  vec.normalize();
  float rEsfera = .2f;
  
  Sphere esf(Vector3(0, 0, -dJanela+ rEsfera), rEsfera);
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


        Vector3 d(x - 0.f, y - 0, -dJanela);
        d.normalize();
        // P(t)= O + t*d

        float disc =
            raySphereIntersect(posicaoObservador, d, esf.center, esf.radius);
        Vector3 hitPoint(d.x * disc, d.y * disc, d.z * disc);

        Vector3 posicaoLuz(-3, 2, -5);

        if (disc > 0.f) {

          image << esfColor[0] << " " << esfColor[1] << " " << esfColor[2] << " ";
        } else {

          image << backgroundColor;
        }
      }

      image << "\n";
    }

    image.close();
  }
}
