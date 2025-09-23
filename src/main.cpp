#include "../include/Sphere.hpp"
#include "../include/Vector3.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <memory>

const int width = 400, height = 300;
std::string backgroundColor = "19 38 66 ";
Vector3 posicaoObservador(0, 0, 0);
float timePassed = 0.f;

void convertDisplayToNdc(int display_x, int display_y, float &ndc_x,
                         float &ndc_y) {
  float centered_x = display_x + 0.5f;
  float centered_y = display_y + 0.5f;

  ndc_x = centered_x - width / 2.0f;
  ndc_y = height / 2.0f - centered_y;
}

void setPixel(Vector3 point, sf::RenderWindow *window, sf::Color color) {
  sf::RectangleShape pixel;
  pixel.setSize(sf::Vector2f(1.f, 1.f));
  pixel.setFillColor(color);
  pixel.setPosition(sf::Vector2(point.x, point.y));
  window->draw(pixel);
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

  float t = (t1 > t2) ? t2 : t1;
  if (t < 0)
    return 0.0f;

  return t;
}

int main() {
  std::ofstream image("image.ppm");
  sf::RenderWindow Window(sf::VideoMode(width, height), "raycast esfera",
                          sf::Style::Close | sf::Style::Titlebar);
  Window.setFramerateLimit(60);

  Sphere esf(Vector3(0, 0, -10), 3);

  if (image.is_open()) {
    image << "P3\n";
    image << width << " " << height << "\n";
    image << 255 << "\n";

    Vector3 posicaoLuz(3, 2, esf.center.z);
    esf.center.z -= 10.f;

    while (Window.isOpen()) {
      Window.clear(sf::Color(19, 38, 66));

          //posicaoLuz.z += std::sin(timePassed) * 2.0f; // movimento da luz
        //    posicaoLuz.x += std::cos(timePassed) * 3.f;
      // esf.center.z += std::sin(timePassed) / 2.5f;
      timePassed += 0.1f;

      sf::Vector2i position = sf::Mouse::getPosition(Window);
      float posMouseX, posMouseY;
      convertDisplayToNdc(position.x, position.y, posMouseX, posMouseY);
      posicaoLuz.x = posMouseX;
      posicaoLuz.y = posMouseY;

      sf::Event e;
      while (Window.pollEvent(e)) {
        if (e.type == sf::Event::Closed) {
          Window.close();
        }
      }

      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
          float ndc_x, ndc_y;
          convertDisplayToNdc(x, y, ndc_x, ndc_y);

          Vector3 d(ndc_x, ndc_y, -width);
          d.normalize();

          float t =
              raySphereIntersect(posicaoObservador, d, esf.center, esf.radius);
          if (t > 0.0f) {
            // ponto de colisão
            Vector3 hitPoint =
                posicaoObservador + Vector3(d.x * t, d.y * t, d.z * t);

            // normal no ponto
            Vector3 normal = hitPoint - esf.center;
            normal.normalize();

            // direção para a luz
            Vector3 dirLuz = posicaoLuz - hitPoint;
            float distLuz = dirLuz.length();
            dirLuz.normalize();

            // checa se está em sombra
            float sombraHit =
                raySphereIntersect(Vector3(normal.x * 0.001f + hitPoint.x,
                                           normal.y * 0.001f + hitPoint.y,
                                           normal.z * 0.001f + hitPoint.z),
                                   dirLuz, esf.center, esf.radius);
            bool emSombra = (sombraHit > 0.0f && sombraHit < distLuz);

            // iluminação difusa (Lambert)
            float intensidade = std::max(0.0f, normal.dot(dirLuz));
            if (emSombra)
              intensidade *= 0.2f; // mais escuro na sombra

            int r = (int)(intensidade * 255);
            int g = (int)(intensidade * 100);
            int b = (int)(intensidade * 50);

            setPixel(Vector3(x, y, 1.f), &Window, sf::Color(r, g, b));
          }
        }
      }

      Window.display();
    }

    image.close();
  }
}
