#include "../include/Cilinder.hpp"
#include "../include/Point3.hpp"
#include <vector>
#include <cmath>

Point3 Cilinder::getCenter() const {
    return this->center;
}

Point3 Cilinder::getColor() const {
    return this->color;
}

Point3 Cilinder::getDiffuse() const {
    return this->diffuse_color;
}

Point3 Cilinder::getSpecular() const {
    return this->specular_color;
}

Cilinder::Cilinder(Point3 &c_base, float height, float radius, Vector3 &dc, bool bottom_lid, bool upper_lid, Point3 &color, Point3 &dif_color, Point3 &esp_color) {
    this->center = c_base;
    this->height = height;
    this->radius = radius;
    Vector3 vetor_cilindro = dc;
    vetor_cilindro.normalize();
    this->dc = vetor_cilindro;
    this->bottom_lid = bottom_lid;
    this->upper_lid = upper_lid;
    this->color = color;
    this->diffuse_color = dif_color;
    this->specular_color = esp_color;

    std::vector<std::vector<float>> dc_matriz;

    //this->M.resize(3, std::vector<float>(3));

    // fazendo a matriz resultante do produto entre o vetor dc coluna e vetor dc transposto (vetor linha)
    for(int i = 0; i < 3; i++){
        std::vector<float> linha;
        for(int j = 0; j < 3; j++){
            const float val = this->dc[i]*this->dc[j];
            linha.push_back(val);
        }
        dc_matriz.push_back(std::vector<float>(linha));
    }
    // fazendo a subtração do vetor identidade pelo dc_matriz
    for(int i = 0; i < 3; i++){
        std::vector<float> linha;
        for(int j = 0; j < 3; j++){
            float val = 0 - dc_matriz[i][j];
            if(i == j) val = 1 - dc_matriz[i][j];
            linha.push_back(val);
        }
        this->M.push_back(std::vector<float>(linha));
    }
}

Vector3 Cilinder::getSurfaceNormal(const Point3 &p_int) const {
    return dc;
}

bool Cilinder::Intersect(const Point3 &origin, const Vector3 &dir, float t_min, float t_max, HitRecord &hr) const {
    // calculando os coeficientes a, b e c da equação do cilindro
    // _trans = transposto
    // a = dir_trans*M*dir
    // a_vc = M*dir (vetor coluna do a)
    Vector3 a_vc(0, 0, 0);
    for(int i = 0; i < 3; i++){
        float val = 0;
        for(int j = 0; j < 3; j++){
            val += this->M[i][j]*dir[j];     
        }
        if(i == 0) a_vc.x = val;
        if(i == 1) a_vc.y = val;
        if(i == 2) a_vc.z = val;
    }
    //a_vc.normalize();
    double a = dot(dir, a_vc);
    // w = origem - centro da base
    Vector3 w(center, origin);
    // w.normalize();
    // o b_vc é o mesmo cálculo do a_vc (M*dir)
    // b = 2*w_trans*M*dir
    double b = 2*dot(w, a_vc);
    // c_vc = M*w (vetor coluna do c)
    Vector3 c_vc(0, 0, 0);
    for(int i = 0; i < 3; i++){
        float val = 0;
        for(int j = 0; j < 3; j++){
            val += this->M[i][j]*w[j];     
        }
        if(i == 0) c_vc.x = val;
        if(i == 1) c_vc.y = val;
        if(i == 2) c_vc.z = val;
    }
    //c_vc.normalize();
    // c = w_trans*M*w - raio²
    double c = dot(w, c_vc) - radius*radius;

    // discriminante
    double disc = b*b - 4*a*c;

    // nao perfurou nem tangenciou
    if (disc < 0) return false;
    // valores de t
    double t1 = (-b + std::sqrt(disc))/(2*a);
    double t2 = (-b - std::sqrt(disc))/(2*a);

    if(t2 > t_min && t2 < t_max){
        Point3 p_int = origin + t2*dir;
        Vector3 s(center, p_int);
        // P' = Cb+t*dc
        // t = (P-Cb)*dc
        // (Cb-P) = s
        Point3 p_linha = center+(dot(s, dc)*dc);
        Vector3 center_to_p_linha(center, p_linha);
        // raio bateu pra trás do centro
        if(dot(dc, center_to_p_linha) < 0){
            if(bottom_lid){
                // se bateu no fundo
                double denominator = dot(dir, dc);
                double t_fundo = 0;
                Vector3 observador_pro_fundo(origin, center);
                if(denominator != 0){
                    t_fundo = dot(observador_pro_fundo, dc)/denominator;
                }
                Point3 p_int_fundo = origin + t_fundo*dir;
                Vector3 center_to_fundo(center, p_int_fundo);
                if(center_to_fundo.length() <= radius){
                    hr.t = t_fundo;
                    hr.p_int = p_int_fundo;
                    hr.normal = dc;
                    hr.obj_ptr = this;
                } else return false;
                return true;
            }
            return false;
        }
        // raio bateu fora do cilindro
        if(center_to_p_linha.length() > height){
            if(upper_lid){
                // se bateu na tampa
                double denominator = dot(dir, dc);
                double t_tampa = 0;
                Point3 centro_tampa = center + (dc*height);
                Vector3 observador_pra_tampa(origin, centro_tampa);
                if(denominator != 0){
                    t_tampa = dot(observador_pra_tampa, dc)/denominator;
                }
                Point3 p_int_tampa = origin + t_tampa*dir;
                Vector3 center_to_tampa(centro_tampa, p_int_tampa);
                if(center_to_tampa.length() <= radius){
                    hr.t = t_tampa;
                    hr.p_int = p_int_tampa;
                    hr.normal = dc;
                    hr.obj_ptr = this;
                } else return false;
                return true;
            }
            return false;
        }
        
        // r = M*s
        Vector3 r(0, 0, 0);
        for(int i = 0; i < 3; i++){
            double val = 0;
            for(int j = 0; j < 3; j++){
                val += this->M[i][j]*s[j];     
            }
            if(i == 0) r.x = val;
            if(i == 1) r.y = val;
            if(i == 2) r.z = val;
        }
        // normal da superficie no p_int
        r.normalize();

        hr.t = t2;
        hr.p_int = p_int;
        hr.normal = r;
        hr.obj_ptr = this;

        if(dot(r, dir) > 0) { hr.normal = 0-r; }

        return true;
    }
    else if(t1 > t_min && t1 < t_max){
        Point3 p_int = origin + t1*dir;
        Vector3 s(center, p_int);
        // P' = Cb+t*dc
        // t = (P-Cb)*dc
        // (Cb-P) = s
        Point3 p_linha = center+(dot(s, dc)*dc);
        Vector3 center_to_p_linha(center, p_linha);
        // raio bateu pra trás do centro
        if(dot(dc, center_to_p_linha) < 0) return false;
        // raio bateu fora do cilindro
        if(center_to_p_linha.length() > height) return false;

        // r = M*s
        Vector3 r(0, 0, 0);
        for(int i = 0; i < 3; i++){
            double val = 0;
            for(int j = 0; j < 3; j++){
                val += this->M[i][j]*s[j];     
            }
            if(i == 0) r.x = val;
            if(i == 1) r.y = val;
            if(i == 2) r.z = val;
        }
        // normal da superficie no p_int
        r.normalize();

        hr.t = t1;
        hr.p_int = p_int;
        hr.normal = r;
        hr.obj_ptr = this;

        if(dot(r, dir) > 0.0f) { hr.normal = 0.0f-r; }

        return true;
    } else return false;
}