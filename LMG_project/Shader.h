#ifndef SHADER_H
#define SHADER_H


/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>

// System
#include <cstdio>

// Graphics
// - GLEW (always before "gl.h")
#include <GL/glew.h>
// - GL
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

// - GLUT
#include <GL/glut.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// SOIL
#include <SOIL/SOIL.h>


class Shader{

public:
    GLint _shaderId;

    // Constructors
    Shader(){}

    /**
     * @brief Shader                    Constructor of the Shader object : open & read shader files / input variables management / binding to program / etc
     * @param VertexShaderFilePath      Path to the vertex shader text file
     * @param FragmentShaderFilePath    Path to the fragment shader text file
     *
     */
    Shader(const std::string VertexShaderFilePath, const std::string FragmentShaderFilePath);


    // Other Functions
    /**
     * @brief use   Activate the current shader
     */
    void use(){
        glUseProgram(_shaderId);
    }
        // Setters
    void setBool(const std::string &name, bool value) const
    {
        GLint uniformLocation = glGetUniformLocation(_shaderId, name.c_str());
        if(uniformLocation >= 0)
            glUniform1i(uniformLocation, (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    {
        GLint uniformLocation = glGetUniformLocation(_shaderId, name.c_str());
        if(uniformLocation >= 0)
            glUniform1i(uniformLocation, value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    {
        GLint uniformLocation = glGetUniformLocation(_shaderId, name.c_str());
        if(uniformLocation >= 0)
            glUniform1f(uniformLocation, value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        GLint uniformLocation = glGetUniformLocation(_shaderId, name.c_str());
        if(uniformLocation >= 0)
            glUniform2fv(uniformLocation, 1, &value[0]);
    }
    void setVec2(const std::string &name, float x, float y) const
    {
        GLint uniformLocation = glGetUniformLocation(_shaderId, name.c_str());
        if(uniformLocation >= 0)
            glUniform2f(uniformLocation, x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
        GLint uniformLocation = glGetUniformLocation(_shaderId, name.c_str());
        if(uniformLocation >= 0)
            glUniform3fv(uniformLocation, 1, &value[0]);
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    {
        GLint uniformLocation = glGetUniformLocation(_shaderId, name.c_str());
        if(uniformLocation >= 0)
            glUniform3f(uniformLocation, x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        GLint uniformLocation = glGetUniformLocation(_shaderId, name.c_str());
        if(uniformLocation >= 0)
            glUniform4fv(uniformLocation, 1, &value[0]);
    }
    void setVec4(const std::string &name, float x, float y, float z, float w)
    {
        GLint uniformLocation = glGetUniformLocation(_shaderId, name.c_str());
        if(uniformLocation >= 0)
            glUniform4f(uniformLocation, x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        GLint uniformLocation = glGetUniformLocation(_shaderId, name.c_str());
        if(uniformLocation >= 0)
            glUniformMatrix2fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat));
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        GLint uniformLocation = glGetUniformLocation(_shaderId, name.c_str());
        if(uniformLocation >= 0)
            glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat));
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        GLint uniformLocation = glGetUniformLocation(_shaderId, name.c_str());
        if(uniformLocation >= 0)
            glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat));
    }

};


#endif
