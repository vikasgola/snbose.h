#include<snbose/camera.h>

Camera::Camera(){
    this->position = vec3(0.0, 0.0, 3.0);
    this->up = vec3(0.0f, 1.0f, 0.0f);
    this->direction = vec3(0.0) - this->position;
    this->view_matrix = lookat(this->position, this->position+this->direction, this->up);
    this->right = this->up.cross(this->direction).normalized();
}

void Camera::look_at(vec3 position, vec3 target, vec3 up){
    this->position = position;
    this->up = up;
    this->direction = (target-position).normalized();
    this->update();
}

void Camera::set_position(vec3 position){
    this->position = position;
    this->update();
}

void Camera::set_pitch(float pitch){
    if(pitch > 89.0f) pitch =  89.0f;
    if(pitch < -89.0f) pitch = -89.0f;
    this->pitch = pitch;
    this->update();
}

void Camera::set_yaw(float yaw){
    this->yaw = yaw;
    this->update();
}

void Camera::set_up(vec3 up){
    this->up = up;
    this->update();
}

void Camera::update(){
    this->direction.x = cosf(DEG2RAD(this->yaw))*cosf(DEG2RAD(this->pitch));
    this->direction.y = sinf(DEG2RAD(this->pitch));
    this->direction.z = sinf(DEG2RAD(this->yaw))*cosf(DEG2RAD(this->pitch));
    this->direction.normalize();
    this->right = this->up.cross(this->direction).normalized();
    this->view_matrix = lookat(this->position, this->position+this->direction, this->up);
}
