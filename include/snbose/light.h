#pragma once
#include<advmath/advmath.h>
#include<snbose/object.h>

class Light: public Object {
    public:
        enum Type {DIRECTIONAL, POINT, SPOT};
        Light(Type type, vec3 ambient, vec3 diffuse, vec3 specular);
        void draw_as(Model model);

        void set_direction(vec3 direction);
        void set_attenuation(float constant, float linear, float quadratic);
        void set_cutoff(float inner, float outer);

        Type type;
    private:
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        vec3 direction = vec3(0.0f, 0.0f, -1.0f);

        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;

        float inner_cutoff = cosf(DEG2RAD(12.5f));
        float outer_cutoff = cosf(DEG2RAD(17.5f));

        friend class Renderer;
};

