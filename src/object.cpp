#include<snbose/object.h>

Object::Object(){}

Object::Object(Model &model){
    this->model = &model;
}

Object::Object(Mesh &mesh){
    this->model = new Model({&mesh});
}

void Object::draw(ShaderProgram &shader_program){
    shader_program.bind();
    shader_program.set_uniformm4f("u_model", this->get_model_matrix());
    this->model->draw(shader_program);
    // shader_program.unbind();
}

void Object::scale(const vec3 factor){
    this->scale_factor = factor;
}

void Object::scale(const float factor_x, const float factor_y, const float factor_z){
    this->scale_factor = vec3(factor_x, factor_y, factor_z);
}

void Object::rotate(const float angle, const vec3 axis){
    this->rotation_angle = angle;
    this->rotation_axis = axis;
}

void Object::move(const vec3 pos){
    this->translate = pos;
}

void Object::move(const float pos_x, const float pos_y, const float pos_z){
    this->translate = vec3(pos_x, pos_y, pos_z);
}

mat4 Object::get_model_matrix(){
    return transform(
        this->scale_factor,
        this->rotation_axis*this->rotation_angle,
        this->translate
    );
}

Object& Object::operator=(const Object &object){
    this->model = object.model;

    this->scale_factor = object.scale_factor;
    this->translate = object.translate;
    this->rotation_axis = object.rotation_axis;
    this->rotation_angle = object.rotation_angle;
    return *this;
}
