#include "Shader.h"


Shader::Shader(const std::string VertexShaderFilePath, const std::string FragmentShaderFilePath){

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    // open shader files
    vertexShaderFile.open(VertexShaderFilePath);

        // Opening error check
    if(!vertexShaderFile.is_open())
    {
        std::cerr << "Could not open file : " << VertexShaderFilePath << std::endl;
    }

    fragmentShaderFile.open(FragmentShaderFilePath);

    if(!fragmentShaderFile.is_open())
    {
        std::cerr << "Could not open file : " << VertexShaderFilePath << std::endl;
    }


    std::stringstream vertexShaderStream, fragmentShaderStream;

    vertexShaderStream << vertexShaderFile.rdbuf();
    fragmentShaderStream << fragmentShaderFile.rdbuf();

    // close shader files
    vertexShaderFile.close();
    fragmentShaderFile.close();
    // convert stream to std::string
    vertexCode   = vertexShaderStream.str();
    fragmentCode = fragmentShaderStream.str();

    // Get shaders source code
    const char* vertexShaderCode = vertexCode.c_str();
    const char* fragmentShaderCode = fragmentCode.c_str();


    // Shaders creation
    GLuint vertexShader, fragmentShader;
    std::cout << "Creation of shaders..." << std::endl;

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
        std::cerr << "Error: vertex shader "<< std::endl;
        GLint logInfoLength = 0;
        glGetShaderiv( vertexShader, GL_INFO_LOG_LENGTH, &logInfoLength );
        if ( logInfoLength > 0 )
        {
            GLchar* infoLog = new GLchar[ logInfoLength ];
            GLsizei length = 0;
            glGetShaderInfoLog( vertexShader, logInfoLength, &length, infoLog );
            std::cerr << infoLog << std::endl;
        }
    }

    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compileStatus );
    if ( compileStatus == GL_FALSE ){
        std::cerr << "Error: fragment shader "<< std::endl;

        GLint logInfoLength = 0;
        glGetShaderiv( fragmentShader, GL_INFO_LOG_LENGTH, &logInfoLength );
        if ( logInfoLength > 0 ){
            GLchar* infoLog = new GLchar[ logInfoLength ];
            GLsizei length = 0;
            glGetShaderInfoLog( fragmentShader, logInfoLength, &length, infoLog );
            std::cerr << infoLog << std::endl;
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

            std::cerr << "\nGsShaderProgram::link() - link ERROR" << std::endl;
            std::cerr << infoLog << std::endl;

            delete[] infoLog;
        }
    }

    // Shaders linked to the program -> deleting them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    std::cout << "Shaders created and linked" << std::endl;

}
