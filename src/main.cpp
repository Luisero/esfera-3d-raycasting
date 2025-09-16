#include <iostream>
#include <fstream>
#include <cmath>
#include "../include/Vector3.hpp"
#include "../include/Sphere.hpp"

const int width = 800, height = 600;
std::string backgroundColor = "19 38 66 ";
Vector3 posicaoObservador(0,0,0);

void convertDisplayToNdc(int display_x, int display_y, float& ndc_x, float& ndc_y) {
    // Adiciona 0.5 para obter o centro do pixel
    float centered_x = display_x + 0.5f;
    float centered_y = display_y + 0.5f;

    // Converte a coordenada X
    ndc_x = centered_x - width / 2.0f;

    // Converte e inverte a coordenada Y
    ndc_y = height / 2.0f - centered_y;
}



float raySphereIntersect(
    const Vector3& origin, const Vector3& dir,
    const Vector3& center, float radius)
{
    Vector3 L = origin - center;
    float b = 2.0f * dir.dot(L);
    float c = L.dot(L) - radius * radius;

    float discriminant = b*b - 4.0f * c; // a=1
    if (discriminant < 0) return 0.0f;

    float sqrtD = std::sqrt(discriminant);
    float t1 = (-b - sqrtD) * 0.5f;
    float t2 = (-b + sqrtD) * 0.5f;

    float t = (t1 > 0) ? t1 : t2; // pega a interseção mais próxima positiva
    if (t < 0) return 0.0f; // está atrás do raio

    return t; // retorna a distância até a colisão
}

int main()
{
    std::ofstream image("image.ppm");
    Vector3 vec(1,1,1);
    vec.normalize();
    
    if(image.is_open())
    {
        image << "P3\n";
        image << width << " " << height << "\n";
        image << 255 << "\n";
       
        Sphere esf(Vector3(0,0,-10),3);

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                
                float ndc_x, ndc_y;
                convertDisplayToNdc(x,y,ndc_x,ndc_y);
                
                Vector3 d(ndc_x - 0.f, ndc_y-0, -width);
                d.normalize();
                //P(t)= O + t*d 
                
                float disc = raySphereIntersect(posicaoObservador,d,esf.center,esf.radius);
                Vector3 hitPoint(d.x*disc, d.y*disc, d.z*disc);
                if(hitPoint.x !=0)
                {

                    std::cout << hitPoint.x << " " << hitPoint.y << " "<< hitPoint.z;
                }
                Vector3 posicaoLuz(-3,2,-5);
                
                

                if(disc >0.f)
                {
                    
                    image<< "200 100 50 ";
                }
                else{

                    image << backgroundColor;
                }
            }

            image << "\n";
            
            
        }
        



        image.close();
    }
}
