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

    Shader(const std::string VertexShaderFilePath, const std::string FragmentShaderFilePath){

        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vertexShaderFile;
        std::ifstream fragmentShaderFile;
            // ensure ifstream objects can throw exceptions:
        //vertexShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        //fragmentShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        //try{
            // open shader files
            std::cout << "coucou" << std::endl;
            vertexShaderFile.open(VertexShaderFilePath);

            if(!vertexShaderFile.is_open())
                std::cout << "prout\n";

            std::cout << "coucou" << std::endl;
            fragmentShaderFile.open(FragmentShaderFilePath);

            if(!vertexShaderFile.is_open())
                std::cout << "prout2\n";

            std::cout << "coucou" << std::endl;


            std::stringstream vertexShaderStream, fragmentShaderStream;

            vertexShaderStream << vertexShaderFile.rdbuf();
            fragmentShaderStream << fragmentShaderFile.rdbuf();

            // close shader files
            vertexShaderFile.close();
            fragmentShaderFile.close();
            // convert stream to std::string
            vertexCode   = vertexShaderStream.str();
            fragmentCode = fragmentShaderStream.str();

            std::cout << "vertex : " << vertexCode << std::endl << "fragment : " << fragmentCode << std::endl;
        //}
        //catch(std::ifstream::failure e){
        //    std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        //}

        const char* vertexShaderCode = vertexCode.c_str();
        const char* fragmentShaderCode = fragmentCode.c_str();


        // Shaders creation
        GLuint vertexShader, fragmentShader;
        std::cout << "Shaders Creation" << std::endl;

            //Vertex shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        std::cout << "glCreateVertex" << std::endl;
        glShaderSource( vertexShader, 1, &vertexShaderCode, nullptr );
        std::cout << "glSourceVertex" << std::endl;
        glCompileShader(vertexShader);
        std::cout << "glCompileVertex" << std::endl;
        std::cout << "vertex shader creation" << std::endl;
            //Fragment shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource( fragmentShader, 1, &fragmentShaderCode, nullptr );
        glCompileShader(fragmentShader);
        std::cout << "fragment shader creation" << std::endl;

            // Error check
        GLint compileStatus;
        glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compileStatus );
        if ( compileStatus == GL_FALSE ){
            std::cout << "Error: vertex shader "<< std::endl;
            GLint logInfoLength = 0;
            glGetShaderiv( vertexShader, GL_INFO_LOG_LENGTH, &logInfoLength );
            if ( logInfoLength > 0 )
            {
                GLchar* infoLog = new GLchar[ logInfoLength ];
                GLsizei length = 0;
                glGetShaderInfoLog( vertexShader, logInfoLength, &length, infoLog );
                std::cout << infoLog << std::endl;
            }
        }

        glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compileStatus );
        if ( compileStatus == GL_FALSE ){
            std::cout << "Error: fragment shader "<< std::endl;

            GLint logInfoLength = 0;
            glGetShaderiv( fragmentShader, GL_INFO_LOG_LENGTH, &logInfoLength );
            if ( logInfoLength > 0 ){
                GLchar* infoLog = new GLchar[ logInfoLength ];
                GLsizei length = 0;
                glGetShaderInfoLog( fragmentShader, logInfoLength, &length, infoLog );
                std::cout << infoLog << std::endl;
            }
        }

        // Shader program creation
        _shaderId = glCreateProgram();
        std::cout << "Shader program creation" << std::endl;
            // Linking shaders to the program
        glAttachShader(_shaderId, vertexShader);
        glAttachShader(_shaderId, fragmentShader);
        std::cout << "link to shader" << std::endl;

        glLinkProgram(_shaderId);

        // Check for linking errors
        GLint linkStatus = 0;
        glGetProgramiv( _shaderId, GL_LINK_STATUS, &linkStatus );
        if ( linkStatus == GL_FALSE ){
            GLint logInfoLength = 0;
            glGetProgramiv( _shaderId, GL_INFO_LOG_LENGTH, &logInfoLength );
            if ( logInfoLength > 0 ){
                // Return logs
                GLchar* infoLog = new GLchar[ logInfoLength ];
                GLsizei length = 0;
                glGetProgramInfoLog( _shaderId, logInfoLength, &length, infoLog );

                std::cout << "\nGsShaderProgram::link() - link ERROR" << std::endl;
                std::cout << infoLog << std::endl;

                delete[] infoLog;
            }
        }

        // Shaders linked to the program -> deleting them
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

    }

    // Other Functions
        // Shader activation
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
