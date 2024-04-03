#include<snbose/light.h>

Light::Light(Type type, vec3 ambient, vec3 diffuse, vec3 specular) :
    type(type),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular){
}

void Light::set_direction(vec3 direction){
    this->direction = direction;
}

void Light::draw_as(Model model){
    this->model = model;
}

void Light::set_attenuation(float constant, float linear, float quadratic){
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}

void Light::set_cutoff(float inner, float outer){
    this->inner_cutoff = cosf(DEG2RAD(inner));
    this->outer_cutoff = cosf(DEG2RAD(outer));
}