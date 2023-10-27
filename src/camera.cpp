#include<snbose/camera.h>
#include<cmath>

Camera::Camera(){
    this->position = vec3(0.0, 0.0, 10.0);
    this->up = vec3(0.0f, 1.0f, 0.0f);
    this->direction = vec3(0.0f, 0.0f, 1.0f);
    this->view_matrix = lookat(this->position, this->position+this->direction, this->up);
    this->right = this->up.cross(this->direction).normalized();
    this->yaw = RAD2DEG(this->up.angle(this->direction));
}

void Camera::look_at(vec3 position, vec3 direction, vec3 up){
    this->position = position;
    this->up = up;
    this->direction = direction.normalized();
    this->yaw = RAD2DEG(this->up.angle(this->direction));
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
    this->direction.x = cosf(DEG2RAD(this->yaw))*cosf(DEG2RAD(this->pitch));
    this->direction.y = sinf(DEG2RAD(this->pitch));
    this->direction.z = sinf(DEG2RAD(this->yaw))*cosf(DEG2RAD(this->pitch));
    this->direction.normalize();
    this->update();
}

void Camera::set_yaw(float yaw){
    this->yaw = yaw;
    this->direction.x = cosf(DEG2RAD(this->yaw))*cosf(DEG2RAD(this->pitch));
    this->direction.y = sinf(DEG2RAD(this->pitch));
    this->direction.z = sinf(DEG2RAD(this->yaw))*cosf(DEG2RAD(this->pitch));
    this->direction.normalize();
    this->update();
}

void Camera::set_up(vec3 up){
    this->up = up;
    this->update();
}

void Camera::update(){
    this->right = this->up.cross(this->direction).normalized();
    this->view_matrix = lookat(this->position, this->position+this->direction, this->up);
}

void Camera::set_orthographic(float left, float right, float bottom, float top, float near, float far){
    this->projection = ortho(left, right, bottom, top, near, far);
}

void Camera::set_perspective(float fov, float aspect_ratio, float near, float far){
    this->projection = perspective(fov, aspect_ratio, near, far);
}
