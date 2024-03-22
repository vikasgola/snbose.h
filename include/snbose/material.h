#pragma once

#include<advmath/advmath.h>

struct Material{
    public:
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;
        Material(const vec3 ambient, const vec3 diffuse, const vec3 specular, float shininess){
            this->ambient = ambient;
            this->diffuse = diffuse;
            this->specular = specular;
            this->shininess = shininess;
        };
};

// http://devernay.free.fr/cours/opengl/materials.html
const Material MATERIAL_EMERALD(vec3(0.0215, 0.1745, 0.0215), vec3(0.07568, 0.61424, 0.07568), vec3(0.633, 0.727811, 0.633), 0.6*128.0);
const Material MATERIAL_JADE(vec3(0.135, 0.2225, 0.1575), vec3(0.54, 0.89, 0.63), vec3(0.316228, 0.316228, 0.316228), 0.1*128.0);
const Material MATERIAL_OBSIDIAN(vec3(0.05375, 0.05, 0.06625), vec3(0.18275, 0.17, 0.22525), vec3(0.332741, 0.328634, 0.346435), 0.3*128.0);
const Material MATERIAL_PEARL(  vec3(0.25, 0.20725, 0.20725), vec3(1.0, 0.829, 0.829), vec3(0.296648, 0.296648, 0.296648), 0.088*128.0);
const Material MATERIAL_RUBY(vec3(0.1745, 0.01175, 0.01175), vec3(0.61424, 0.04136, 0.04136), vec3(0.727811, 0.626959, 0.626959), 0.6*128.0);
const Material MATERIAL_TURQUOISE(vec3(0.1, 0.18725, 0.1745), vec3(0.396, 0.74151, 0.69102), vec3(0.297254, 0.30829, 0.306678), 0.1*128.0);
const Material MATERIAL_BRASS(vec3(0.329412, 0.223529, 0.027451), vec3(0.780392, 0.568627, 0.113725), vec3(0.992157, 0.941176, 0.807843), 0.21794872*128.0);
const Material MATERIAL_BRONZE(vec3(0.2125, 0.1275, 0.054), vec3(0.714, 0.4284, 0.18144), vec3(0.393548, 0.271906, 0.166721), 0.2*128.0);
const Material MATERIAL_CHROME(vec3(0.25, 0.25, 0.25), vec3(0.4, 0.4, 0.4), vec3(0.774597, 0.774597, 0.774597), 0.6*128.0);
const Material MATERIAL_COPPER(vec3(0.19125, 0.0735, 0.0225), vec3(0.7038, 0.27048, 0.0828), vec3(0.256777, 0.137622, 0.086014), 0.1*128.0);
const Material MATERIAL_GOLD(vec3(0.24725, 0.1995, 0.0745), vec3(0.75164, 0.60648, 0.22648), vec3(0.628281, 0.555802, 0.366065), 0.4*128.0);
const Material MATERIAL_SILVER(vec3(0.19225, 0.19225, 0.19225), vec3(0.50754, 0.50754, 0.50754), vec3(0.508273, 0.508273, 0.508273), 0.4*128.0);
const Material MATERIAL_BLACK_PLASTIC(vec3(0.0, 0.0, 0.0), vec3(0.01, 0.01, 0.01), vec3(0.50, 0.50, 0.50), .25*128.0);
const Material MATERIAL_CYAN_PLASTIC(vec3(0.0, 0.1, 0.06), vec3(0.0, 0.50980392, 0.50980392), vec3(0.50196078, 0.50196078, 0.50196078), .25*128.0);
const Material MATERIAL_GREEN_PLASTIC(vec3(0.0, 0.0, 0.0), vec3(0.1, 0.35, 0.1), vec3(0.45, 0.55, 0.45), .25*128.0);
const Material MATERIAL_RED_PLASTIC(vec3(0.0, 0.0, 0.0), vec3(0.5, 0.0, 0.0), vec3(0.7, 0.6, 0.6), .25*128.0);
const Material MATERIAL_WHITE_PLASTIC(vec3(0.0, 0.0, 0.0), vec3(0.55, 0.55, 0.55), vec3(0.70, 0.70, 0.70), .25*128.0);
const Material MATERIAL_YELLOW_PLASTIC(vec3(0.0, 0.0, 0.0), vec3(0.5, 0.5, 0.0), vec3(0.60, 0.60, 0.50), .25*128.0);
const Material MATERIAL_BLACK_RUBBER(vec3(0.02, 0.02, 0.02), vec3(0.01, 0.01, 0.01), vec3(0.4, 0.4, 0.4), .078125*128.0);
const Material MATERIAL_CYAN_RUBBER(vec3(0.0, 0.05, 0.05), vec3(0.4, 0.5, 0.5), vec3(0.04, 0.7, 0.7), .078125*128.0);
const Material MATERIAL_GREEN_RUBBER(vec3(0.0, 0.05, 0.0), vec3(0.4, 0.5, 0.4), vec3(0.04, 0.7, 0.04), .078125*128.0);
const Material MATERIAL_RED_RUBBER(vec3(0.05, 0.0, 0.0), vec3(0.5, 0.4, 0.4), vec3(0.7, 0.04, 0.04), .078125*128.0);
const Material MATERIAL_WHITE_RUBBER(vec3(0.05, 0.05, 0.05), vec3(0.5, 0.5, 0.5), vec3(0.7, 0.7, 0.7), .078125*128.0);
const Material MATERIAL_YELLOW_RUBBER(vec3(0.05, 0.05, 0.0), vec3(0.5, 0.5, 0.4), vec3(0.7, 0.7, 0.04), .078125*128.0);