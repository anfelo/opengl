#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cut_off;
    float outer_cut_off;

    float constant;
    float linear;
    float quadratic;
};

uniform Light u_light;
uniform Material u_material;
uniform vec3 u_view_pos;

void main()
{
    // ambient
    vec3 ambient = u_light.ambient * vec3(texture(u_material.diffuse, TexCoords));

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(u_light.position - FragPos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = u_light.diffuse * diff * vec3(texture(u_material.diffuse, TexCoords));

    // specular
    vec3 view_dir = normalize(u_view_pos - FragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * spec * vec3(texture(u_material.specular, TexCoords));

    // spot light (soft-edges)
    float theta = dot(light_dir, normalize(-u_light.direction));
    float epsilon = u_light.cut_off - u_light.outer_cut_off;
    float intensity = clamp((theta - u_light.outer_cut_off) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    // attenuation
    float distance = length(u_light.position - FragPos);
    float attenuation = 1.0 / (u_light.constant + u_light.linear * distance +
                    u_light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}
