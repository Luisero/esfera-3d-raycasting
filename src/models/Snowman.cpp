#include "../../include/models/Snowman.hpp"
#include <cmath>
#include <stdio.h>

Snowman::Snowman(Point3 &center, float sink_factor){
  /*
  components indices
  0 - base sphere
  1 - middle sphere
  2 - top sphere
  3 - left arm
  4 - right arm
  5 - fedora base
  6 - fedora body
  7 - nose
  8 - left eye
  9 - right eye
  10, 11, 12, 13, 14 - smile from left to right
  */

  // setting the colors
  Point3 body_color(.9, .9, .9);
  Point3 arms_color(0.3, 0.18, 0.02);
  Point3 fedora_color(0.3, 0.18, 0.02);
  Point3 eye_color(0, 0, 0);
  Point3 nose_color(0.93, 0.56, 0.12);
  Point3 spec_color(.1, .1, .1);
  // setting the coordinates of the center of the base sphere
  float base_x = center.x;
  float base_y = center.y;
  float base_z = center.z;
  Point3 base_sphere_center(base_x, base_y, base_z);
  float base_sphere_radius = .18f;
  // putting the first sphere in the snowman
  this->components.push_back(std::make_unique<Sphere>(base_sphere_center, base_sphere_radius, body_color, body_color, spec_color));
  this->center = center;
  this->sink_factor = sink_factor;
  
  // setting up the middle sphere
  float middle_sphere_radius = base_sphere_radius-base_sphere_radius/6;
  float middle_sphere_x = base_x;
  float middle_sphere_y = base_y + middle_sphere_radius+base_sphere_radius - sink_factor;
  float middle_sphere_z = base_z;
  Point3 middle_sphere_center(middle_sphere_x, middle_sphere_y, middle_sphere_z);
  this->components.push_back(std::make_unique<Sphere>(
    middle_sphere_center, 
    middle_sphere_radius, 
    body_color, 
    body_color, 
    spec_color)); // putting the second sphere in the snowman

  // setting up the top sphere
  float top_sphere_radius = middle_sphere_radius-middle_sphere_radius/3;
  float top_sphere_x = base_x;
  float top_sphere_y = middle_sphere_y + top_sphere_radius+middle_sphere_radius - sink_factor;
  float top_sphere_z = base_z;
  Point3 top_sphere_center(top_sphere_x, top_sphere_y, top_sphere_z);
  this->components.push_back(std::make_unique<Sphere>(
    top_sphere_center, 
    top_sphere_radius, 
    body_color, 
    body_color, 
    spec_color)); // putting the third sphere in the snowman

  // setting the position of the arms
  float arm_height = base_sphere_radius;
  float arm_radius = middle_sphere_radius/9;
  float left_arm_x = middle_sphere_center.x - middle_sphere_radius + arm_radius;
  float right_arm_x = middle_sphere_center.x + middle_sphere_radius - arm_radius;
  float arm_y = middle_sphere_y;
  float arm_z = base_z;
  Vector3 left_arm_dc(-1, .2, 0);
  Vector3 right_arm_dc(1, .2, 0);
  Point3 left_arm_center(left_arm_x, arm_y, arm_z);
  Point3 right_arm_center(right_arm_x, arm_y, arm_z);
  this->components.push_back(std::make_unique<Cilinder>(
    left_arm_center, 
    arm_height, 
    arm_radius, 
    left_arm_dc, 
    true, 
    true, 
    arms_color, 
    arms_color, 
    spec_color)); // putting the left arm in the snowman

  this->components.push_back(std::make_unique<Cilinder>(
    right_arm_center, 
    arm_height, 
    arm_radius, 
    right_arm_dc, 
    true, 
    true, 
    arms_color, 
    arms_color, 
    spec_color)); // putting the right arm in the snowman

  // setting the position of the fedora base
  float fedora_base_radius = top_sphere_radius*1.2f;
  float fedora_base_x = top_sphere_x + top_sphere_radius/2.5f;
  float fedora_base_y = top_sphere_y + top_sphere_radius - 0.01f;
  float fedora_base_z = top_sphere_z - fedora_base_radius/3;
  float fedora_base_height = top_sphere_radius/10;
  Point3 fedora_base_center(fedora_base_x, fedora_base_y, fedora_base_z);
  Vector3 fedora_dc(1, 2, 0);
  this->components.push_back(std::make_unique<Cilinder>(
    fedora_base_center, 
    fedora_base_height, 
    fedora_base_radius, 
    fedora_dc, 
    true, 
    true, 
    fedora_color, 
    fedora_color, 
    spec_color)); // putting the fedora base on the snowman

  // setting the position of the fedora body
  float fedora_radius = fedora_base_radius/2;
  float fedora_x = fedora_base_x;
  float fedora_y = fedora_base_y;
  float fedora_z = fedora_base_z;
  float fedora_height = fedora_base_height*20;
  Point3 fedora_center(fedora_x, fedora_y, fedora_z);
  this->components.push_back(std::make_unique<Cilinder>(
    fedora_center, 
    fedora_height, 
    fedora_radius, 
    fedora_dc, 
    true, 
    true, 
    fedora_color, 
    fedora_color, 
    spec_color)); // putting the fedora body in the snowman

  // setting the position of the nose
  float nose_radius = top_sphere_radius/10;
  float nose_x = top_sphere_x;
  float nose_y = top_sphere_y;
  float nose_z = top_sphere_z + top_sphere_radius;
  float nose_height = top_sphere_radius/2;
  Point3 nose_vertice(nose_x, nose_y, nose_z + nose_height);
  Point3 nose_center(nose_x, nose_y, nose_z);
  this->components.push_back(std::make_unique<Cone>(
    nose_center, 
    nose_radius, 
    true, 
    nose_vertice, 
    nose_color, 
    nose_color, 
    spec_color)); // putting the nose in the snowman

  // setting up the eyes
  float eye_radius = top_sphere_radius/6.666f;
  float right_eye_x_offset = top_sphere_x + top_sphere_radius/2.5f;
  float left_eye_x_offset = top_sphere_x - top_sphere_radius/2.5f;
  float eye_y = top_sphere_y + top_sphere_radius/5;
  // calculating the z
  float eye_z = top_sphere_z + 
                std::sqrt(std::pow(top_sphere_radius, 2) 
                - std::pow((right_eye_x_offset - top_sphere_x), 2) 
                - std::pow(eye_y - top_sphere_y, 2));
  Point3 left_eye_center(left_eye_x_offset, eye_y, eye_z);
  Point3 right_eye_center(right_eye_x_offset, eye_y, eye_z);
  this->components.push_back(std::make_unique<Sphere>(
    left_eye_center, 
    eye_radius, 
    eye_color, 
    eye_color, 
    spec_color));
  this->components.push_back(std::make_unique<Sphere>(
    right_eye_center, 
    eye_radius, 
    eye_color, 
    eye_color, 
    spec_color));
}

bool Snowman::Intersect(const Point3 &origin, const Vector3 &dir, float t_min, float t_max, HitRecord &hr) const {
  HitRecord hr_temp;
  float temp_t_max = t_max;
  bool hit_anything = false;
  for(const auto& part : components){
    if(part->Intersect(origin, dir, t_min, temp_t_max, hr_temp)){
      temp_t_max = hr_temp.t;
      hit_anything = true;
    }
  }
  if(!hit_anything) return false;

  hr = hr_temp;
  return true;
}