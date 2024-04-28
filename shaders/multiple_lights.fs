#version 330 core

out vec4 color;

in vec3 normal;
in vec3 frag_pos;
in vec2 tex_cords;

uniform vec3 u_camera_pos;

struct Material {
    float shininess;
};
uniform Material material;

const int POINT_MAX_LIGHTS = 2;
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform PointLight point_lights[POINT_MAX_LIGHTS];

const int DIRECTIONAL_MAX_LIGHTS = 1;
struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight directional_lights[DIRECTIONAL_MAX_LIGHTS];

const int SPOT_MAX_LIGHTS = 1;
struct SpotLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
    float inner_cutoff;
    float outer_cutoff;
};
uniform SpotLight spot_lights[SPOT_MAX_LIGHTS];

uniform sampler2D u_texture_diffuse1;
uniform sampler2D u_texture_specular1;

vec3 calculatePointLightStrength(
    PointLight light,
    vec3 m_ambient,
    vec3 m_diffuse,
    vec3 m_specular,
    float m_shininess
){
    vec3 n_normal = normalize(normal);

    vec3 ambient = light.ambient*m_ambient;

    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 diffu = light.diffuse*max(dot(light_dir, n_normal), 0.0)*m_diffuse;

    vec3 refl_dir = reflect(-light_dir, n_normal);
    vec3 view_dir = normalize(u_camera_pos - frag_pos);
    float pre_speular = pow(max(dot(view_dir, refl_dir), 0.0), m_shininess);
    vec3 specular = light.specular*(pre_speular*m_specular);

    float d = length(light.position - frag_pos);
    float attenuation = 1.0f / (light.constant + d*light.linear + d*d*light.quadratic);

    return (ambient+diffu+specular)*attenuation;
}

vec3 calculateDirectionalLightStrength(
    DirectionalLight light,
    vec3 m_ambient,
    vec3 m_diffuse,
    vec3 m_specular,
    float m_shininess
){
    vec3 n_normal = normalize(normal);

    vec3 ambient = light.ambient*m_ambient;

    vec3 light_dir = normalize(-light.direction);
    vec3 diffu = light.diffuse*max(dot(light_dir, n_normal), 0.0)*m_diffuse;

    vec3 refl_dir = reflect(-light_dir, n_normal);
    vec3 view_dir = normalize(u_camera_pos - frag_pos);
    float pre_speular = pow(max(dot(view_dir, refl_dir), 0.0), m_shininess);
    vec3 specular = light.specular*(pre_speular*m_specular);

    return (ambient+diffu+specular);
}

vec3 calculateSpotLightStrength(
    SpotLight light,
    vec3 m_ambient,
    vec3 m_diffuse,
    vec3 m_specular,
    float m_shininess
){
    vec3 n_normal = normalize(normal);

    vec3 ambient = light.ambient*m_ambient;

    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 diffu = light.diffuse*max(dot(light_dir, n_normal), 0.0)*m_diffuse;

    vec3 refl_dir = reflect(-light_dir, n_normal);
    vec3 view_dir = normalize(u_camera_pos - frag_pos);
    float pre_speular = pow(max(dot(view_dir, refl_dir), 0.0), m_shininess);
    vec3 specular = light.specular*(pre_speular*m_specular);

    float theta = dot(light_dir, normalize(-light.direction));
    float epsilon = light.inner_cutoff - light.outer_cutoff;
    float intensity = clamp((theta-light.outer_cutoff)/epsilon, 0.0f, 1.0f);

    diffu *= intensity;
    specular *= intensity;

    return ambient+diffu+specular;
}

void main(){
    vec3 total_color = vec3(0.0);

    // directional light calculations
    for(int i=0;i<DIRECTIONAL_MAX_LIGHTS;i++){
        total_color += calculateDirectionalLightStrength(
            directional_lights[i],
            texture(u_texture_diffuse1, tex_cords).rgb,
            texture(u_texture_diffuse1, tex_cords).rgb,
            texture(u_texture_specular1, tex_cords).rgb,
            material.shininess
        );
    }
    // point light calculations
    for(int i=0;i<POINT_MAX_LIGHTS;i++){
        total_color += calculatePointLightStrength(
            point_lights[i],
            texture(u_texture_diffuse1, tex_cords).rgb,
            texture(u_texture_diffuse1, tex_cords).rgb,
            texture(u_texture_specular1, tex_cords).rgb,
            material.shininess
        );
    }
    // spot light calculations
    for(int i=0;i<SPOT_MAX_LIGHTS;i++){
        total_color += calculateSpotLightStrength(
            spot_lights[i],
            texture(u_texture_diffuse1, tex_cords).rgb,
            texture(u_texture_diffuse1, tex_cords).rgb,
            texture(u_texture_specular1, tex_cords).rgb,
            material.shininess
        );
    }

    color = vec4(total_color, 1.0);
}