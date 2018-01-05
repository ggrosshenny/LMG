#ifndef SHADER_H
#define SHADER_H


#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{

public:
    unsigned int _shaderId;

    // Constructor
    Shader(const std::string VertexShaderFilePath, const std::string FragmentShaderFilePath){

        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vertexShaderFile;
        std::ifstream fragmentShaderFile;
            // ensure ifstream objects can throw exceptions:
        vertexShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fragmentShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try{
            // open shader files
            vertexShaderFile.open(VertexShaderFilePath);
            fragmentShaderFile.open(FragmentShaderFilePath);
            std::stringstream vertexShaderStream, fragmentShaderStream;

            vertexShaderStream << vertexShaderStream.rdbuf();
            fragmentShaderStream << fragmentShaderStream.rdbuf();
            // close shader files
            vertexShaderFile.close();
            fragmentShaderFile.close();
            // convert stream to std::string
            vertexCode   = vertexShaderStream.str();
            fragmentCode = fragmentShaderStream.str();
        }
        catch(std::ifstream::failure e){
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        const char* vertexShaderCode = vertexCode.c_str();
        const char* fragmentShaderCode = fragmentCode.c_str();


        // Shaders creation
        GLuint vertexShader, fragmentShader;

            //Vertex shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource( vertexShader, 1, &vertexShaderCode, nullptr );
        glCompileShader(vertexShader);
            //Fragment shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource( fragmentShader, 1, &fragmentShaderCode, nullptr );
        glCompileShader(fragmentShader);

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
            // Linking shaders to the program
        glAttachShader(_shaderId, vertexShader);
        glAttachShader(_shaderId, fragmentShader);

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
        glUniform1i(glGetUniformLocation(_shaderId, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(_shaderId, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(_shaderId, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(_shaderId, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(_shaderId, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(_shaderId, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(_shaderId, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(_shaderId, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string &name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(_shaderId, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(_shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(_shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(_shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

};


#endif
