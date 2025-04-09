#include "shader.h"
#include "glm/gtc/type_ptr.hpp"

std::string get_file_contents(const char *filename) {
    std::ifstream in(filename, std::ios::binary);
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    throw errno;
}

void shader_program_create(shader_program_t *shader, const char *vertex_file,
                           const char *fragment_file) {
    std::string vertex_code = get_file_contents(vertex_file);
    std::string fragment_code = get_file_contents(fragment_file);

    const char *vertex_src = vertex_code.c_str();
    const char *fragment_src = fragment_code.c_str();

    // Create vertex shader object and get its reference
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    // Attach vertex shader source to the vertex shader object
    glShaderSource(vertex_shader, 1, &vertex_src, NULL);
    // Compile the vertex shader into machine code
    glCompileShader(vertex_shader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Create fragment shader object and get its reference
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach fragment shader source to the fragment shader object
    glShaderSource(fragment_shader, 1, &fragment_src, NULL);
    // Compile the vertex shader source to the fragment shader object
    glCompileShader(fragment_shader);

    // check for shader compile errors
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Create shader program object and get its reference
    shader->ID = glCreateProgram();
    // Attach the vertex and fragment shaders to the shader program
    glAttachShader(shader->ID, vertex_shader);
    glAttachShader(shader->ID, fragment_shader);
    // Wrap-up/link all the shaders together into the shader program
    glLinkProgram(shader->ID);

    // check for linking errors
    glGetProgramiv(shader->ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader->ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // Delete the now useless vertex and fragment shader objects
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void shader_program_activate(shader_program_t *shader) {
    glUseProgram(shader->ID);
}

void shader_program_delete(shader_program_t *shader) {
    glDeleteProgram(shader->ID);
}

void shader_set_float(shader_program_t *shader, const char *name, float value) {
    glUniform1f(glGetUniformLocation(shader->ID, name), value);
}

void shader_set_bool(shader_program_t *shader, const char *name, bool value) {
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void shader_set_int(shader_program_t *shader, const char *name, int value) {
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void shader_set_vec3(shader_program_t *shader, const char *name,
                     glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(shader->ID, name), 1,
                 glm::value_ptr(value));
}

void shader_set_mat4(shader_program_t *shader, const char *name,
                     glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, name), 1, GL_FALSE,
                       glm::value_ptr(value));
}
