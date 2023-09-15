

//Sonia LI
//Yasser Ameer 40212780
//Peter Abou Kassam 40157317
//Faisal Bari 27517440
//phong lighting from the TA lab session code
//sphere from TA lab session
//Arm hierarchy accomplished
//assignmenent 1 code put all in one model
// shadow from the lab session
// texture from the lab session
//
#include <thread>    // for std::this_thread::sleep_for
#include <chrono>    // for std::chrono::seconds


#include <iostream>
#include <list>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL

#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cstdlib>
//#include <OpenGL/gl.h>#include <stb_image.h> // for image loading
//#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // for image loading
#include <string>
#include "OBJloader.h"    //For loading .obj files
#include "OBJloaderV2.h"  //For loading .obj files using a polygon list format
#include "shaderloader.h"

using namespace glm;
using namespace std;
//LIGHTING
//


const char* getVertexShaderSource();

const char* getFragmentShaderSource();

void DrawGround(GLuint worldmatrix, GLuint colorLocation, int shader_id);

void DrawCoordinates(GLuint worldmatrix, GLuint colorLocation, float worldXAngle, float worldYAngle);

int compileAndLinkShaders();

bool initContext();

struct TexturedColoredVertex
{
    TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv, vec3 _light)
        : position(_position), color(_color), uv(_uv), light(_light) {}

    vec3 position;
    vec3 color;
    vec2 uv;
    vec3 light;
};

// Textured Cube model
const TexturedColoredVertex texturedCubeVertexArray[] = {  // position,                            NORMAL
        TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f),vec3(-1.0f, 0.0f, 0.0f)), //left - red
        TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
        TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

        TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)),
        TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
        TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)),

        TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f)), // far - blue
        TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
        TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f)),

        TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f)),
        TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
        TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),

        TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f),vec3(0.0f, -1.0f, 0.0f)), // bottom - turquoise
        TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f),vec3(0.0f, -1.0f, 0.0f)),
        TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f),vec3(0.0f, -1.0f, 0.0f)),

        TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f),vec3(0.0f, -1.0f, 0.0f)),
        TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f),vec3(0.0f, -1.0f, 0.0f)),
        TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f),vec3(0.0f, -1.0f, 0.0f)),

        TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f)), // near - green
        TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
        TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),

        TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f)),
        TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f)),
        TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),

        TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f)), // right - purple
        TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)),
        TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)),

        TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)),
        TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f)),
        TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f)),

        TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f)), // top - yellow
        TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)),
        TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)),

        TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f)),
        TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)),
        TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f))
};

int createTexturedCubeVertexArrayObject()
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TexturedColoredVertex),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TexturedColoredVertex),
        (void*)(2 * sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
    );
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3,                            // attribute normal in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TexturedColoredVertex),
        (void*)(2 * sizeof(vec3) + sizeof(vec2))      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(3);

    return vertexArrayObject;
}



GLFWwindow* window = NULL;
GLuint loadTexture(const std::string& path) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}


const char* getDepthVertexShaderSource() {
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "uniform mat4 worldMatrix;"
        "uniform mat4 lightSpaceMatrix;"
        "void main(){"
        " gl_Position=lightSpaceMatrix * worldMatrix *  vec4(aPos.x, aPos.y, aPos.z, 1.0);"


        "}"
        ;
}


const char* getDepthFragmentShaderSource() {

    return
        "  #version 330 core\n"
        " uniform float opacity = 1.0f ; "
        "void main()"
        "{"
        "}";

}

const char* getVertexShaderSource() {
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        "layout (location = 2) in vec2 aUV;"
        "layout (location = 3) in vec3 aNormal;"
        ""
        "uniform mat4 worldMatrix;"
        "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
        "uniform mat4 projectionMatrix = mat4(1.0);"
        "uniform mat4 worldOrientationMatrix;"
        "uniform mat4 lightSpaceMatrix;"
        "uniform vec3 color;"
        "uniform vec3 lightPos;"
        ""
        "out vec3 objectColor;"
        "out vec2 vertexUV;"
        "out vec4 fragPosLightSpace;"
        "out vec3 pos;"
        "out vec3 normal;"
        "out vec3 eyeDir;"
        "out vec3 lightDir;"
        "void main()"
        "{"
        "   objectColor = aColor;"
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldOrientationMatrix * worldMatrix;"
        "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "   vertexUV = aUV;"
        ""

        ""
        "  pos = (worldMatrix * vec4(aPos, 1)).xyz;"
        "  vec3 vertexPosition_cameraspace = ( viewMatrix * worldMatrix * vec4(aPos,1)).xyz;"
        "  eyeDir = vec3(0,0,0) - vertexPosition_cameraspace;"
        ""
        "  vec3 lightPosCamera = (viewMatrix * vec4(lightPos, 1)).xyz;"
        "  lightDir = lightPosCamera + eyeDir;"
        "  normal = (transpose(inverse(viewMatrix * worldMatrix)) * vec4(aNormal, 1)).xyz;"

        " fragPosLightSpace=lightSpaceMatrix* vec4(pos,1.0);"
        "}";
}



const char* getFragmentShaderSource() {
    return
        "#version 330 core\n"
        "in vec3 vertexColor;"
        "in vec3 pos;"
        "in vec3 normal;"
        "in vec3 eyeDir;"
        "in vec3 lightDir;"
        "out vec4 FragColor;"
        //   "in vec3 FragColor1;"
        "in vec4 fragPosLightSpace;"
        "uniform vec3 objectColor;"
        "in vec2 vertexUV;"
        "uniform sampler2D textureSampler;"
        "uniform sampler2D shadowMap;"
        "uniform int useTexture = 0;"
        "uniform int useTexture1 = 0;"
        "uniform vec3 lightPos;"
        "uniform float lightPower = 10.0f;"
        " uniform float opacity = 1.0f ; "
        "void main()"
        "{"
        "   vec4 textureColor = texture( textureSampler, vertexUV );"

        "  vec3 lightColor = vec3(1,1,1);"

        ""
        "  vec3 diffuseColor = objectColor;"
        "  vec3 specularColor = vec3(0.5,0.5,0.5);"
        "  vec3 ambientColor = vec3(0.5,0.5,0.5) * objectColor;"
        ""
        "  float dist = length(lightPos - pos);"
        "  vec3 n = normalize(normal);"
        "  vec3 l = normalize(lightDir);"
        ""
        "  float cosTheta = clamp(dot(n,l), 0, 1);"
        ""
        "  vec3 E = normalize(eyeDir);"
        "  vec3 R = reflect(-l, n);"
        ""
        "  float cosAlpha = clamp(dot(E, R), 0, 1);"
        ""
        " vec3 shadowTextureCoord = fragPosLightSpace.xyz * 0.5 + 0.5; "
        "   float shadow = 1.0f;"
        "   float bias = 0.005;"
        "   if (shadowTextureCoord.z - bias > texture(shadowMap, shadowTextureCoord.xy).r)"
        "   {"
        "       shadow = 0.2f;"
        "   }"
        " vec3 L ="
        "     ambientColor +"
        "     shadow * (diffuseColor * lightColor * lightPower * cosTheta / (1 + dist * 0.5 + dist * dist * 0.05) +"
        "     specularColor * lightColor * lightPower * pow(cosAlpha,5) / (1 + dist * 0.5 + dist * dist * 0.05)); "
        " vec3 P ="
        "     ambientColor +"
        "     (diffuseColor * lightColor * lightPower * cosTheta / (1 + dist * 0.5 + dist * dist * 0.05) +"
        "     specularColor * lightColor * lightPower * pow(cosAlpha,5) / (1 + dist * 0.5 + dist * dist * 0.05)); "
        " vec4 M = vec4 (L,1.0f);"
        " vec4 B = vec4 (P,1.0f);"
        "   if (useTexture == 0 && useTexture1==0){"
        "    FragColor = B* vec4(objectColor.r, objectColor.g, objectColor.b, 1.0f); }"
        "   if (useTexture == 0 && useTexture1==1){"
        "    FragColor = M* vec4(objectColor.r, objectColor.g, objectColor.b, 1.0f); }"
        "   if (useTexture == 1 && useTexture1==0){"
        "    FragColor = B*textureColor; }"
        "   if (useTexture == 1 && useTexture1==1){"
        "    FragColor = M*textureColor; }"
        " FragColor.w = opacity ; "

        "}";
}

int compileAndLinkShaders() {
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = getFragmentShaderSource();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
int compileAndLinkTexturedShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
int compileAndLinkDepthShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void DrawGround(GLuint worldmatrix, GLuint colorLocation, int shader_id) {
    glGetUniformLocation(shader_id, "objectColor");
    mat4 gridWorldMatrix;

    gridWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -5.0f, 0.0f))
        * rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *
        scale(mat4(1.0f), vec3(300.0f, -2.0f, 300.0f));
    glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &gridWorldMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
    glDrawArrays(GL_TRIANGLES, 0, 36);


}

void DrawBleachers(GLuint worldLocationMatrix, int texturedCubeVAO, GLuint colorLocation, int shader_id, GLuint bleachersTextureID) {
    mat4 groundTennisMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(36.0f, 0.02f, 78.0f));
    //DrawBleachers
    //first row
    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(30.0f, 7.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 1.0f, 80.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-30.0f, 7.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 1.0f, 80.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    //second row
    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(35.0f, 12.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 1.0f, 80.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-35.0f, 12.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 1.0f, 80.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    //third row
    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(40.0f, 17.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 1.0f, 80.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-40.0f, 17.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 1.0f, 80.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    //fourth row
    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(45.0f, 22.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 1.0f, 80.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-45.0f, 22.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 1.0f, 80.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    //three front support pillars
    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(30.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 13.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-30.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 13.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(30.0f, 0.0f, 40.0f)) * scale(mat4(1.0f), vec3(2.0f, 13.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-30.0f, 0.0f, 40.0f)) * scale(mat4(1.0f), vec3(2.0f, 13.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(30.0f, 0.0f, -40.0f)) * scale(mat4(1.0f), vec3(2.0f, 13.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-30.0f, 0.0f, -40.0f)) * scale(mat4(1.0f), vec3(2.0f, 13.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    //three second row support pillars
    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(36.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 23.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-36.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 23.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(36.0f, 0.0f, 40.0f)) * scale(mat4(1.0f), vec3(2.0f, 23.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-36.0f, 0.0f, 40.0f)) * scale(mat4(1.0f), vec3(2.0f, 23.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(36.0f, 0.0f, -40.0f)) * scale(mat4(1.0f), vec3(2.0f, 23.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-36.0f, 0.0f, -40.0f)) * scale(mat4(1.0f), vec3(2.0f, 23.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    //three third row support pillars
    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(42.5f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 34.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-42.5f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 34.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(42.5f, 0.0f, 40.0f)) * scale(mat4(1.0f), vec3(2.0f, 34.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-42.5f, 0.0f, 40.0f)) * scale(mat4(1.0f), vec3(2.0f, 34.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(42.5f, 0.0f, -40.0f)) * scale(mat4(1.0f), vec3(2.0f, 34.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-42.5f, 0.0f, -40.0f)) * scale(mat4(1.0f), vec3(2.0f, 34.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0


    //three fourth row support pillars
    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(47.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 46.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-47.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 46.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(47.0f, 0.0f, 40.0f)) * scale(mat4(1.0f), vec3(2.0f, 46.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-47.0f, 0.0f, 40.0f)) * scale(mat4(1.0f), vec3(2.0f, 46.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(47.0f, 0.0f, -40.0f)) * scale(mat4(1.0f), vec3(2.0f, 46.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-47.0f, 0.0f, -40.0f)) * scale(mat4(1.0f), vec3(2.0f, 46.0f, 2.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    //three ground level connecting pillars
    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(38.5f, -3.0f, 0.0f)) * scale(mat4(1.0f), vec3(17.0f, 2.0f, 2.05f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-38.5f, -3.0f, 0.0f)) * scale(mat4(1.0f), vec3(17.0f, 2.0f, 2.05f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(38.5f, -3.0f, 40.0f)) * scale(mat4(1.0f), vec3(17.0f, 2.0f, 2.05f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-38.5f, -3.0f, 40.0f)) * scale(mat4(1.0f), vec3(17.0f, 2.0f, 2.05f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindVertexArray(texturedCubeVAO);
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(38.5f, -3.0f, -40.0f)) * scale(mat4(1.0f), vec3(17.0f, 2.0f, 2.05f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-38.5f, -3.0f, -40.0f)) * scale(mat4(1.0f), vec3(17.0f, 2.0f, 2.05f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0


    //bar connecting the front
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-30.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 80.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(30.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 80.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0


    //crossbar supports in the back
    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-44.0f, 11.0f, -20.0f)) * rotate(mat4(1.0f), radians(-30.0f), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 40.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(44.0f, 11.0f, -20.0f)) * rotate(mat4(1.0f), radians(-30.0f), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 40.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(-44.0f, 11.0f, 20.0f)) * rotate(mat4(1.0f), radians(30.0f), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 40.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    glBindTexture(GL_TEXTURE_2D, bleachersTextureID);
    groundTennisMatrix = translate(mat4(1.0f), vec3(44.0f, 11.0f, 20.0f)) * rotate(mat4(1.0f), radians(30.0f), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 40.0f));
    glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

}

void DrawCoordinates(GLuint worldmatrix, GLuint colorLocation, float worldXAngle, float worldYAngle) {
    // Draw X axis, colored red
    mat4 xAxis = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) *
        rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
    xAxis = xAxis * translate(mat4(1.0f), vec3(2.5f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 0.1f, 0.1f));
    glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &xAxis[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 0.0f, 0.0f)));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw Y axis, colored green
    mat4 yAxis = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) *
        rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
    yAxis = yAxis * translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 5.0f, 0.1f));
    glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &yAxis[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 1.0f, 0.0f)));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw Z axis, colored blue
    mat4 zAxis = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) *
        rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
    zAxis = zAxis * translate(mat4(1.0f), vec3(0.0f, 0.0f, 2.5f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 5.0f));
    glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &zAxis[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 0.0f, 1.0f)));
    glDrawArrays(GL_TRIANGLES, 0, 36);

}

bool initContext() {     // Initialize GLFW and OpenGL version
    glfwInit();

#if defined(PLATFORM_OSX)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    // On windows, we set OpenGL version to 2.1, to support more hardware
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

    // Create Window and rendering context using GLFW, resolution is 800x600
    window = glfwCreateWindow(1024, 768, "Comp371 - A1 (40193803)", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return false;
    }
    return true;
}


#include <vector>
#include <cmath>
void MidSectionCreation(GLuint worldmatrix, GLuint colorLocation) {

    //creating the middle

    mat4 LeftBar = translate(mat4(1.0f), vec3(-18.0f, 4.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.5f, 8.0f, 0.5f));
    glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &LeftBar[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(0.212, 0.212, 0.212)));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 RightBar = translate(mat4(1.0f), vec3(+18.0f, 4.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.5f, 8.0f, 0.5f));
    glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &RightBar[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(0.212, 0.212, 0.212)));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mat4 MiddleBar = translate(mat4(1.0f), vec3(0.0f, 8.0f, 0.0f)) * scale(mat4(1.0f), vec3(37.0f, 0.5f, 0.5f));
    glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &MiddleBar[0][0]);
    glUniform3fv(colorLocation, 1, value_ptr(vec3(0.212, 0.212, 0.212)));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //creating the NET

    for (float i = 0; i < 3.5; i = i + 0.8)
    {
        mat4 HLinesMatrix = translate(mat4(1.0f), vec3(0.0f, i + 4.5f, 0.0f)) * scale(mat4(1.0f), vec3(36.0f, 0.09f, 0.09f));
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &HLinesMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    //vertical lines
    for (float i = 0; i < 36; i = i + 1)
    {
        mat4 VLinesMatrix = translate(mat4(1.0f), vec3(i - 18.0f, 6.0f, 0.0f)) * rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.06f, 3.0f, 0.06f));
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &VLinesMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }



}
class Ball {
public:
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    GLuint vao, vbo, ebo;
    GLuint texture;
    unsigned indexCount;

    Ball() { }

    int createSphereObject()
    {
        // A vertex is a point on a polygon, it contains positions and other data (eg: colors)
        unsigned int sphereVAO;
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;
        std::vector<glm::vec3> colors;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = indices.size();

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (colors.size() > 0) {
                data.push_back(colors[i].x);
                data.push_back(colors[i].y);
                data.push_back(colors[i].z);
            }
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        float stride = (3 + 2 + 3 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
        glBindVertexArray(0); // Unbind to not modify the VAO

        return sphereVAO;
    }

    void Render(const glm::mat4& groupTranslationMatrix, const glm::mat4& groupRotationMatrix,
        const glm::mat4& groupScalingMatrix, GLint worldLocationMatrix, GLint colorLocation,
        GLenum renderingMode, GLint textureSamplerLocation, int shaderprogram) {

        //        glUseProgram(shaderprogram);
        //        glActiveTexture(GL_TEXTURE0);
        //        glBindTexture(GL_TEXTURE_2D, texture);
        //        glUniform1i(textureSamplerLocation, 0);

        glm::mat4 sphereMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(7.5f, 10.0f, 0.0f));
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 4.0f, 1.0f));
        glm::mat4 modelMatrix = groupTranslationMatrix * groupRotationMatrix * groupScalingMatrix * sphereMatrix * scaleMatrix;

        glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.0f)));

        glBindVertexArray(vao);
        glDrawElements(renderingMode, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(0);
    }

    ~Ball() {
        glDeleteTextures(1, &texture);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }
};

GLuint setupModelVBO(string path, int& vertexCount);

// Sets up a model using an Element Buffer Object to refer to vertex data
GLuint setupModelEBO(string path, int& vertexCount);

class Number

{

#include <glm/gtc/matrix_transform.hpp>
public:

    Number() {};

    Number(GLuint worldmatrix, GLuint colorLocation, GLenum polygonMode, int texturedshader, int ID, char Number, GLuint poleTexture, GLuint boxTexture, GLuint numberTexture)
    {
        glBindTexture(GL_TEXTURE_2D, numberTexture);

        this->worldmatrix = worldmatrix;
        this->colorLocation = colorLocation;
        this->polygonMode = polygonMode;
        this->letter = letter;
        this->texturedshader = texturedshader;
        this->ID = ID;
        this->numberTexture = numberTexture;
        this->boxTexture = boxTexture;
        this->poleTexture = poleTexture;
    }

    void DrawPlayerScore(int score) {
        switch (this->ID)
        {
        case 1:
            this->NumbersSide = translate(mat4(1.0f), vec3(-24.0f, 13.0f, 5.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
            break;

        case 2:
            this->NumbersSide = translate(mat4(1.0f), vec3(-24.0f, 13.0f, -5.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
            break;

        default:
            break;
        }

        switch (score)
        {

        case 0:
            DrawZero();
            break;
        case 1:
            DrawOne();
            break;
        case 2:
            DrawTwo();
            break;

        case 3:
            DrawThree();
            break;

        case 4:
            DrawFour();
            break;

        case 5:
            DrawFive();
            break;

        case 6:
            DrawSix();
            break;

        case 7:
            DrawSeven();
            break;

        case 8:
            DrawEight();
            break;

        case 9:
            DrawNine();
            break;

        default:
            break;
        }
    }

    void DrawZero() {
        glBindTexture(GL_TEXTURE_2D, numberTexture);

        mat4 HorizontalDownMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalDownMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleRight1Matrix = translate(mat4(1.0f), vec3(1.0f, 0.75f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleRight1Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleLeft1Matrix = translate(mat4(1.0f), vec3(-1.0f, 0.75f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleLeft1Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleLeft2Matrix = translate(mat4(1.0f), vec3(-1.0f, 2.25f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleLeft2Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleRight2Matrix = translate(mat4(1.0f), vec3(1.0f, 2.25f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleRight2Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);


        mat4 HorizontalUpMatrix = translate(mat4(1.0f), vec3(0.0f, 3.5f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalUpMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);
    }

    void DrawOne() {
        glBindTexture(GL_TEXTURE_2D, numberTexture);
        mat4 HorizontalDownMatrix = translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalDownMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);
    }

    void DrawTwo() {
        glBindTexture(GL_TEXTURE_2D, numberTexture);
        mat4 HorizontalDownMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalDownMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleLeftMatrix = translate(mat4(1.0f), vec3(-1.0f, 0.75f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleLeftMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 HorizontalMiddleMatrix = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalMiddleMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleRightMatrix = translate(mat4(1.0f), vec3(1.0f, 3.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleRightMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 HorizontalUpMatrix = translate(mat4(1.0f), vec3(0.0f, 4.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalUpMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

    }

    void DrawThree() {
        glBindTexture(GL_TEXTURE_2D, numberTexture);
        mat4 HorizontalDownMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalDownMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleLeft1Matrix = translate(mat4(1.0f), vec3(1.0f, 0.75f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleLeft1Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 HorizontalMiddleMatrix = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalMiddleMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleRight2Matrix = translate(mat4(1.0f), vec3(1.0f, 3.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleRight2Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 HorizontalUpMatrix = translate(mat4(1.0f), vec3(0.0f, 4.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalUpMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

    }

    void DrawFour() {
        glBindTexture(GL_TEXTURE_2D, numberTexture);
        mat4 VerticleRight1Matrix = translate(mat4(1.0f), vec3(1.0f, 0.75f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleRight1Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 HorizontalMiddleMatrix = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalMiddleMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleRight2Matrix = translate(mat4(1.0f), vec3(1.0f, 3.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleRight2Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleLeftMatrix = translate(mat4(1.0f), vec3(-1.0f, 3.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleLeftMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);


    }

    void DrawFive() {
        glBindTexture(GL_TEXTURE_2D, numberTexture);
        mat4 HorizontalDownMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalDownMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleRightMatrix = translate(mat4(1.0f), vec3(1.0f, 0.75f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleRightMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 HorizontalMiddleMatrix = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalMiddleMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleLeftMatrix = translate(mat4(1.0f), vec3(-1.0f, 3.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleLeftMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);


        mat4 HorizontalUpMatrix = translate(mat4(1.0f), vec3(0.0f, 4.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalUpMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

    }

    void DrawSix() {
        glBindTexture(GL_TEXTURE_2D, numberTexture);
        mat4 HorizontalDownMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalDownMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleRightMatrix = translate(mat4(1.0f), vec3(1.0f, 0.75f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleRightMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleLeft1Matrix = translate(mat4(1.0f), vec3(-1.0f, 0.75f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleLeft1Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 HorizontalMiddleMatrix = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalMiddleMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleLeft2Matrix = translate(mat4(1.0f), vec3(-1.0f, 3.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleLeft2Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);


        mat4 HorizontalUpMatrix = translate(mat4(1.0f), vec3(0.0f, 4.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalUpMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

    }

    void DrawSeven() {
        glBindTexture(GL_TEXTURE_2D, numberTexture);
        mat4 VerticleMatrix = translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
        modelMatrix = NumbersSide * VerticleMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 HorizontalMatrix = translate(mat4(1.0f), vec3(-1.0f, 2.5f, 0.0f)) *
            scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);
    }

    void DrawEight() {
        glBindTexture(GL_TEXTURE_2D, numberTexture);
        mat4 HorizontalDownMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalDownMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleRight1Matrix = translate(mat4(1.0f), vec3(1.0f, 0.75f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleRight1Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleLeft1Matrix = translate(mat4(1.0f), vec3(-1.0f, 0.75f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleLeft1Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 HorizontalMiddleMatrix = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalMiddleMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleLeft2Matrix = translate(mat4(1.0f), vec3(-1.0f, 3.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleLeft2Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleRight2Matrix = translate(mat4(1.0f), vec3(1.0f, 3.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleRight2Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);


        mat4 HorizontalUpMatrix = translate(mat4(1.0f), vec3(0.0f, 4.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalUpMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);
    }

    void DrawNine() {
        glBindTexture(GL_TEXTURE_2D, numberTexture);
        mat4 HorizontalDownMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalDownMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleRight1Matrix = translate(mat4(1.0f), vec3(1.0f, 0.75f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleRight1Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);


        mat4 HorizontalMiddleMatrix = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalMiddleMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleLeft2Matrix = translate(mat4(1.0f), vec3(-1.0f, 3.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleLeft2Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 VerticleRight2Matrix = translate(mat4(1.0f), vec3(1.0f, 3.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
        modelMatrix = NumbersSide * VerticleRight2Matrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);


        mat4 HorizontalUpMatrix = translate(mat4(1.0f), vec3(0.0f, 4.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
        modelMatrix = NumbersSide * HorizontalUpMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);
    }

    void DrawScoreBoard() {
        glBindTexture(GL_TEXTURE_2D, poleTexture);
        mat4 GroupTransferMatrix = translate(mat4(1.0f), vec3(-26.0f, 0.0f, 0.0f));
        mat4 VerticleMatrix = translate(mat4(1.0f), vec3(0.0f, 6.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 12.0f, 3.0f));
        modelMatrix = GroupTransferMatrix * VerticleMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(polygonMode, 0, 36);

        mat4 HorizontalMatrix = translate(mat4(1.0f), vec3(0.0f, 15.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(3.0f, 6.0f, 16.0f));
        modelMatrix = GroupTransferMatrix * HorizontalMatrix;
        glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.5, 0.5, 0.0)));
        glDrawArrays(polygonMode, 0, 36);

    }

    void changeWorldMatrix(GLuint newWorldMatrix) {
        this->worldmatrix = newWorldMatrix;

    }



    void ChangeTennisColor(float R, float G, float B) {
        color[0] = R;
        color[1] = G;
        color[2] = B;
    }
    void setPolygoneMode(GLenum polygoneMode) {
        this->polygonMode = polygoneMode;
    }

    void changeShader(int shader_id) {
        this->texturedshader = shader_id;
    }

    mat4 NumbersSide = mat4(1.0f);
    char letter;
private:
    GLuint poleTexture;
    GLuint boxTexture;
    GLuint numberTexture;
    mat4 UpperArmGroupMatrix = mat4(1.0f);
    mat4 LowerArmGroupMatrix = mat4(1.0f);
    mat4 RacketGroupMatrix = mat4(1.0f);

    GLuint worldmatrix;
    GLuint colorLocation;
    GLenum polygonMode = GL_TRIANGLES;
    mat4 modelMatrix;
    mat4 MiddleCoord;

    float color[3] = { 0.0,0.0,0.0 };

    int texturedshader;
    int ID;
};

void setGradientColor() {
    // Get the current time in seconds
    float timeValue = glfwGetTime();
    float value = std::sin(timeValue) * 0.5f + 0.5f; // Sine function oscillates between -1 and 1, so normalize it to [0, 1]

    // Navy blue components
    float navyRed = 0.0f;
    float navyGreen = 0.0f;
    float navyBlue = 0.5f;

    // Orange components
    float orangeRed = 1.0f;
    float orangeGreen = 0.5f;
    float orangeBlue = 0.0f;

    // Interpolate between navy blue and orange based on the value
    float red = (1.0f - value) * navyRed + value * orangeRed;
    float green = (1.0f - value) * navyGreen + value * orangeGreen;
    float blue = (1.0f - value) * navyBlue + value * orangeBlue;

    // Set the clear color
    glClearColor(red, green, blue, 1.0f);
}
class Tennis
{
#include <glm/gtc/matrix_transform.hpp>
public:

    Tennis() {};

    Tennis(GLuint worldmatrix, GLuint colorLocation, GLenum polygonMode, int texturedshader, int ID, char letter)
    {
        this->worldmatrix = worldmatrix;
        this->colorLocation = colorLocation;
        this->polygonMode = polygonMode;
        this->letter = letter;
        this->texturedshader = texturedshader;
        this->ID = ID;
    }
    void changeWorldMatrix(GLuint newWorldMatrix) {
        this->worldmatrix = newWorldMatrix;

    }

    //Call this function to DRAW everything
    void Draw(GLuint upperarmtexture, GLuint lowerarmtexture, GLuint rackettexture) {
        this->Draw_UpperArm(this->letter, upperarmtexture);
        this->Draw_LowerArm(this->letter, lowerarmtexture);
        this->Draw_Racket(this->letter, rackettexture);
    }

    void Draw_UpperArm(char letter, GLuint upperarmtexture) {
        glUseProgram(this->texturedshader);
        glBindTexture(GL_TEXTURE_2D, upperarmtexture);
        switch (letter)
        {
        case 'p': {
            mat4 UpperArmMatrix = translate(mat4(1.0f), vec3(1.0f, 1.0f, 0.0f)) *
                rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
            UpperArmGroupMatrix =
                UpperarmgroupTransferMatrix * UpperarmgroupRotationMatrix * UpperarmgroupScaleMatrix;
            modelMatrix = UpperArmGroupMatrix * UpperArmMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
            glDrawArrays(polygonMode, 0, 36);
            UpperArmCurrentPosition = translate(mat4(1.0f), vec3(1.0f, 1.0f, 0.0f));
            CurrentPosition = translate(mat4(1.0f), vec3(1.0f, 1.0f, 0.0f));
            break;
        }

        case 'y': {
            mat4 upperarmWorldMatrix = scale(mat4(1.0f), vec3(6.0f, 1.0f, 2.0f));
            UpperArmGroupMatrix =
                UpperarmgroupTransferMatrix * UpperarmgroupRotationMatrix * UpperarmgroupScaleMatrix;
            modelMatrix = UpperArmGroupMatrix * upperarmWorldMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.99, 0.88)));
            glDrawArrays(polygonMode, 0, 36);
            CurrentPosition = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
            break;
        }

        case 's': {
            mat4 UpperArmMatrixSonia = translate(mat4(1.0f), vec3(1.0f, 1.0f, 0.0f)) *
                rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(1.5f, 1.0f, 1.0f));
            UpperArmGroupMatrix =
                UpperarmgroupTransferMatrix * UpperarmgroupRotationMatrix * UpperarmgroupScaleMatrix;
            modelMatrix = UpperArmGroupMatrix * UpperArmMatrixSonia;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.87f, 0.72f, 0.53f)));
            glDrawArrays(polygonMode, 0, 36);
            CurrentPosition = translate(mat4(1.0f), vec3(1.0f, -1000.0f, 0.0f));
            break;
        }

        case 'f': {
            mat4 upperarmWorldMatrix = scale(mat4(1.0f), vec3(6.0f, 1.0f, 2.0f));
            UpperArmGroupMatrix =
                UpperarmgroupTransferMatrix * UpperarmgroupRotationMatrix * UpperarmgroupScaleMatrix;
            modelMatrix = UpperArmGroupMatrix * upperarmWorldMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.99, 0.88)));
            glDrawArrays(polygonMode, 0, 36);
            CurrentPosition = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
            break;
        }
        default: {
            break;
        }
        }



    }

    void Draw_LowerArm(char letter, GLuint upperarmtexture) {
        glUseProgram(this->texturedshader);
        glBindTexture(GL_TEXTURE_2D, upperarmtexture);
        switch (letter)
        {
        case 'p': {
            mat4 LowerArmMatrix = translate(mat4(1.0f), vec3(2.0f, 2.5f, 0.0f)) *
                rotate(mat4(1.0f), radians(70.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
            LowerArmGroupMatrix = UpperArmGroupMatrix * LowerarmgroupTransferMatrix * LowerarmgroupRotationMatrix *
                LowerarmgroupScaleMatrix;
            modelMatrix = LowerArmGroupMatrix * LowerArmMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
            glDrawArrays(polygonMode, 0, 36);
            break;
        }

        case 'y': {
            mat4 lowerarmWorldMatrix =
                translate(mat4(1.0f), vec3(3.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 5.0f, 2.0f));
            LowerArmGroupMatrix = UpperArmGroupMatrix * LowerarmgroupTransferMatrix * LowerarmgroupRotationMatrix *
                LowerarmgroupScaleMatrix;
            modelMatrix = LowerArmGroupMatrix * lowerarmWorldMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.99, 0.88)));

            glDrawArrays(polygonMode, 0, 36);
            break;
        }

        case's': {
            mat4 LowerArmMatrixSonia = translate(mat4(1.0f), vec3(2.0f, 2.5f, 0.0f)) *
                rotate(mat4(1.0f), radians(72.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(2.2f, 1.0f, 1.0f));
            LowerArmGroupMatrix = UpperArmGroupMatrix * LowerarmgroupTransferMatrix * LowerarmgroupRotationMatrix *
                LowerarmgroupScaleMatrix;
            modelMatrix = LowerArmGroupMatrix * LowerArmMatrixSonia;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.87f, 0.72f, 0.53f)));
            glDrawArrays(polygonMode, 0, 36);
            break;
        }
        case 'f': {
            mat4 lowerarmWorldMatrix =
                translate(mat4(1.0f), vec3(3.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 5.0f, 2.0f));
            LowerArmGroupMatrix = UpperArmGroupMatrix * LowerarmgroupTransferMatrix * LowerarmgroupRotationMatrix *
                LowerarmgroupScaleMatrix;
            modelMatrix = LowerArmGroupMatrix * lowerarmWorldMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.99, 0.88)));

            glDrawArrays(polygonMode, 0, 36);
            break;
        }

        default: {
            break;
        }
        }

    }
    void Draw_Racket(char letter, GLuint rackettexture) {
        glUseProgram(this->texturedshader);
        glBindTexture(GL_TEXTURE_2D, rackettexture);
        switch (letter)
        {
        case 'p': {                // the handle
            mat4 handleMatrix = translate(mat4(1.0f), vec3(2.3f, 4.0f, 0.25f)) *
                rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(2.5f, 0.5f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * handleMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.5, 0.5)));
            glDrawArrays(polygonMode, 0, 36);

            // the base
            mat4 baserightMatrix = translate(mat4(1.0f), vec3(3.0f, 5.5f, 0.25f)) *
                rotate(mat4(1.0f), radians(35.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(1.5f, 0.5f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * baserightMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.5, 0.5)));
            glDrawArrays(polygonMode, 0, 36);

            mat4 baseleftMatrix = translate(mat4(1.0f), vec3(1.6f, 5.5f, 0.25f)) *
                rotate(mat4(1.0f), radians(145.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(1.5f, 0.5f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * baseleftMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.5, 0.5)));
            glDrawArrays(polygonMode, 0, 36);

            //lower right side
            mat4 lowerrightMatrix = translate(mat4(1.0f), vec3(3.5f, 6.5f, 0.25f)) *
                rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(1.5f, 0.5f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * lowerrightMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.5, 0.5)));
            glDrawArrays(polygonMode, 0, 36);

            //lower left side
            mat4 lowerleftMatrix = translate(mat4(1.0f), vec3(1.1f, 6.5f, 0.25f)) *
                rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(1.5f, 0.5f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * lowerleftMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.5, 0.5)));
            glDrawArrays(polygonMode, 0, 36);


            //upper left side
            mat4 upperleftMatrix = translate(mat4(1.0f), vec3(3.0f, 7.5f, 0.25f)) *
                rotate(mat4(1.0f), radians(145.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(1.5f, 0.5f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * upperleftMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.5, 0.5)));
            glDrawArrays(polygonMode, 0, 36);

            //upper right side
            mat4 upperrightMatrix = translate(mat4(1.0f), vec3(1.6f, 7.5f, 0.25f)) *
                rotate(mat4(1.0f), radians(35.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(1.5f, 0.5f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * upperrightMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.5, 0.5)));
            glDrawArrays(polygonMode, 0, 36);


            //upperbase
            mat4 upperbaseMatrix = translate(mat4(1.0f), vec3(2.3f, 8.0f, 0.25f)) *
                rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(0.3f, 0.5f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * upperbaseMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.5, 0.5)));
            glDrawArrays(polygonMode, 0, 36);


            // horiztonal lines
            for (float i = 0; i < 2; i = i + 0.2) {
                mat4 HLinesMatrix = translate(mat4(1.0f), vec3(2.3f, i + 5.6f, 0.0f)) *
                    scale(mat4(1.0f), vec3(2.0f, 0.05f, 0.05f));
                RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                    RacketgroupScaleMatrix;
                modelMatrix = RacketGroupMatrix * HLinesMatrix;
                glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
                glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                glDrawArrays(polygonMode, 0, 36);
            }

            //vertical lines
            for (float i = 0; i < 1.8; i = i + 0.2) {
                mat4 VLinesMatrix = translate(mat4(1.0f), vec3(i + 1.5f, 6.5f, 0.0f)) *
                    rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 0.0f, 1.0f)) *
                    scale(mat4(1.0f), vec3(0.05f, 2.5f, 0.05f));
                RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                    RacketgroupScaleMatrix;
                modelMatrix = RacketGroupMatrix * VLinesMatrix;
                glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
                glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                glDrawArrays(polygonMode, 0, 36);
            }
            break;
        }

        case 'y': {
            mat4 handle1WorldMatrix =
                translate(mat4(1.0f), vec3(3.0f, 9.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 8.0f, 2.0f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * handle1WorldMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.99, 0.0)));// adding the color desired
            glDrawArrays(polygonMode, 0, 36);
            //  *rotate(mat4(1.0f), radians(testangle), vec3(0.0f, 1.0f, 0.0f))
            mat4 rackWorldMatrix =
                translate(mat4(1.0f), vec3(3.0f, 14.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 10.0f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * rackWorldMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.99, 0.0)));
            glDrawArrays(polygonMode, 0, 36);

            rackWorldMatrix = translate(mat4(1.0f), vec3(3.0f, 17.5f, -4.5f)) *
                rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 8.0f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * rackWorldMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.99, 0.0)));
            glDrawArrays(polygonMode, 0, 36);

            rackWorldMatrix = translate(mat4(1.0f), vec3(3.0f, 17.5f, 4.5f)) *
                rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 8.0f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * rackWorldMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.99, 0.0)));
            glDrawArrays(polygonMode, 0, 36);

            rackWorldMatrix =
                translate(mat4(1.0f), vec3(3.0f, 21.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 10.0f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * rackWorldMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.99, 0.0)));
            glDrawArrays(polygonMode, 0, 36);
            for (int i = 16; i < 22; ++i) {
                rackWorldMatrix = translate(mat4(1.0f), vec3(3.0f, i * 1.0f, 0.0f))
                    * rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f)) *
                    scale(mat4(1.0f), vec3(0.1f, 10.0f, 0.1f));
                RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                    RacketgroupScaleMatrix;
                modelMatrix = RacketGroupMatrix * rackWorldMatrix;
                glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
                glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.0, 0.0)));
                glDrawArrays(polygonMode, 0, 36);

            }
            for (int i = -4; i < 4; ++i) {
                rackWorldMatrix = translate(mat4(1.0f), vec3(3.0f, 17.5f, i * 1.0f))
                    * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *
                    scale(mat4(1.0f), vec3(0.1f, 8.0f, 0.1f));
                RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                    RacketgroupScaleMatrix;
                modelMatrix = RacketGroupMatrix * rackWorldMatrix;
                glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
                glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.0, 0.0)));
                glDrawArrays(polygonMode, 0, 36);

            }
            break;
        }

        case 's': {
            //Racket Head upper right side
            mat4 upperrightMatrixSonia = translate(mat4(1.0f), vec3(1.6f, 7.5f, 0.25f)) *
                rotate(mat4(1.0f), radians(35.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(1.5f, 0.35f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * upperrightMatrixSonia;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.3725f, 0.6196f, 0.6275f)));
            glDrawArrays(polygonMode, 0, 36);

            //Racket Head upper left side
            mat4 upperleftMatrixSonia = translate(mat4(1.0f), vec3(2.3f, 8.0f, 0.25f)) *
                rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(0.3f, 0.5f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * upperleftMatrixSonia;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.3725f, 0.6196f, 0.6275f)));
            glDrawArrays(polygonMode, 0, 36);

            //Racket Head lower right side
            mat4 lowerrightMatrixSonia = translate(mat4(1.0f), vec3(3.5f, 6.5f, 0.25f)) *
                rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(1.5f, 0.35f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * lowerrightMatrixSonia;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.3725f, 0.6196f, 0.6275f)));
            glDrawArrays(polygonMode, 0, 36);

            //Racket Head lower left side
            mat4 lowerleftMatrixSonia = translate(mat4(1.0f), vec3(1.1f, 6.5f, 0.25f)) *
                rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(1.5f, 0.35f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * lowerleftMatrixSonia;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.3725f, 0.6196f, 0.6275f)));
            glDrawArrays(polygonMode, 0, 36);

            //Racket Head upperbase

            mat4 upperbaseMatrixSonia = translate(mat4(1.0f), vec3(3.0f, 7.5f, 0.25f)) *
                rotate(mat4(1.0f), radians(145.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(1.5f, 0.35f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * upperbaseMatrixSonia;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.3725f, 0.6196f, 0.6275f)));
            glDrawArrays(polygonMode, 0, 36);

            mat4 handleMatrixSonia = translate(mat4(1.0f), vec3(2.3f, 4.0f, 0.25f)) *
                rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(2.5f, 0.5f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * handleMatrixSonia;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.3725f, 0.6196f, 0.6275f)));
            glDrawArrays(polygonMode, 0, 36);

            mat4 baserightMatrixSonia = translate(mat4(1.0f), vec3(3.0f, 5.5f, 0.25f)) *
                rotate(mat4(1.0f), radians(35.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(1.5f, 0.35f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * baserightMatrixSonia;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.3725f, 0.6196f, 0.6275f)));
            glDrawArrays(polygonMode, 0, 36);

            mat4 baseleftMatrixSonia = translate(mat4(1.0f), vec3(1.6f, 5.5f, 0.25f)) *
                rotate(mat4(1.0f), radians(145.0f), vec3(0.0f, 0.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(1.5f, 0.35f, 0.5f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * baseleftMatrixSonia;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.3725f, 0.6196f, 0.6275f)));
            glDrawArrays(polygonMode, 0, 36);

            // horiztonal lines
            for (float i = 0; i < 2; i = i + 0.2) {
                mat4 HLinesMatrixSonia = translate(mat4(1.0f), vec3(2.3f, i + 5.6f, 0.0f)) *
                    scale(mat4(1.0f), vec3(2.0f, 0.05f, 0.05f));
                RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                    RacketgroupScaleMatrix;
                modelMatrix = RacketGroupMatrix * HLinesMatrixSonia;
                glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
                glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                glDrawArrays(polygonMode, 0, 36);
            }

            //vertical lines
            for (float i = 0; i < 1.8; i = i + 0.2) {
                mat4 VLinesMatrixSonia = translate(mat4(1.0f), vec3(i + 1.5f, 6.5f, 0.0f)) *
                    rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 0.0f, 1.0f)) *
                    scale(mat4(1.0f), vec3(0.05f, 2.5f, 0.05f));
                RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                    RacketgroupScaleMatrix;
                modelMatrix = RacketGroupMatrix * VLinesMatrixSonia;
                glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
                glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                glDrawArrays(polygonMode, 0, 36);
            }

            break;
        }
        case 'f': {
            mat4 handle1WorldMatrix =
                translate(mat4(1.0f), vec3(3.0f, 9.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 8.0f, 2.0f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * handle1WorldMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0, 0.99, 0.0)));// adding the color desired
            glDrawArrays(polygonMode, 0, 36);
            //  *rotate(mat4(1.0f), radians(testangle), vec3(0.0f, 1.0f, 0.0f))
            mat4 rackWorldMatrix =
                translate(mat4(1.0f), vec3(3.0f, 14.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 10.0f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * rackWorldMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0, 0.99, 0.0)));// adding the color desired
            glDrawArrays(polygonMode, 0, 36);

            rackWorldMatrix = translate(mat4(1.0f), vec3(3.0f, 17.5f, -4.5f)) *
                rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 8.0f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * rackWorldMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0, 0.99, 0.0)));// adding the color desired
            glDrawArrays(polygonMode, 0, 36);

            rackWorldMatrix = translate(mat4(1.0f), vec3(3.0f, 17.5f, 4.5f)) *
                rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 8.0f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * rackWorldMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0, 0.99, 0.0)));// adding the color desired
            glDrawArrays(polygonMode, 0, 36);

            rackWorldMatrix =
                translate(mat4(1.0f), vec3(3.0f, 21.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 10.0f));
            RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                RacketgroupScaleMatrix;
            modelMatrix = RacketGroupMatrix * rackWorldMatrix;
            glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0, 0.99, 0.0)));// adding the color desired
            glDrawArrays(polygonMode, 0, 36);
            for (int i = 16; i < 22; ++i) {
                rackWorldMatrix = translate(mat4(1.0f), vec3(3.0f, i * 1.0f, 0.0f))
                    * rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f)) *
                    scale(mat4(1.0f), vec3(0.1f, 10.0f, 0.1f));
                RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                    RacketgroupScaleMatrix;
                modelMatrix = RacketGroupMatrix * rackWorldMatrix;
                glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
                glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.0, 0.0)));
                glDrawArrays(polygonMode, 0, 36);

            }
            for (int i = -4; i < 4; ++i) {
                rackWorldMatrix = translate(mat4(1.0f), vec3(3.0f, 17.5f, i * 1.0f))
                    * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *
                    scale(mat4(1.0f), vec3(0.1f, 8.0f, 0.1f));
                RacketGroupMatrix = LowerArmGroupMatrix * RacketgroupTransferMatrix * RacketgroupRotationMatrix *
                    RacketgroupScaleMatrix;
                modelMatrix = RacketGroupMatrix * rackWorldMatrix;
                glUniformMatrix4fv(worldmatrix, 1, GL_FALSE, &modelMatrix[0][0]);
                glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.0, 0.0)));
                glDrawArrays(polygonMode, 0, 36);

            }
            break;
        }
        default: {
            break;
        }
        }


    }


    void ChangeTennisPosition(mat4 grouptranslate) {
        UpperarmgroupTransferMatrix = grouptranslate;
    }

    void ChangeTennisRotation(mat4 grouprotation) {
        UpperarmgroupRotationMatrix = grouprotation;
    }

    void ChangeTennisScale(mat4 groupscale) {
        UpperarmgroupScaleMatrix = groupscale;
    }

    void ChangeTennisColor(float R, float G, float B) {
        color[0] = R;
        color[1] = G;
        color[2] = B;
    }
    void setPolygoneMode(GLenum polygoneMode) {
        this->polygonMode = polygoneMode;
    }

    void changeShader(int shader_id) {
        this->texturedshader = shader_id;
    }

    mat4 UpperarmgroupTransferMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));;
    mat4 UpperarmgroupRotationMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f));
    mat4 UpperarmgroupScaleMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));


    mat4 LowerarmgroupTransferMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));;
    mat4 LowerarmgroupRotationMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f));
    mat4 LowerarmgroupScaleMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));

    mat4 RacketgroupTransferMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));;
    mat4 RacketgroupRotationMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f));
    mat4 RacketgroupScaleMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));

    mat4 UpperArmCurrentPosition = mat4(1.0f);
    mat4 CurrentPosition = mat4(1.0f);
    char letter;
private:

    mat4 UpperArmGroupMatrix = mat4(1.0f);
    mat4 LowerArmGroupMatrix = mat4(1.0f);
    mat4 RacketGroupMatrix = mat4(1.0f);

    GLuint worldmatrix;
    GLuint colorLocation;
    GLenum polygonMode = GL_TRIANGLES;
    mat4 modelMatrix;
    mat4 MiddleCoord;
    float color[3] = { 0.0,0.0,0.0 };

    int texturedshader;
    int ID;
};
bool detectGround(glm::vec3& ballPos, glm::vec3 groundHeight, glm::vec3 groundNormal)
{
    return glm::dot(groundNormal, (ballPos - groundHeight)) < 1.0f; // radius
    return false;
}

void updateBallPos(glm::vec3& ballpos, glm::vec3& ballvel, glm::vec3& ballacc, float dt)
{
    ballvel = ballvel + ballacc * dt;
    ballpos = ballpos + ballvel * dt;
}

glm::vec3 boundaryBox(glm::vec3& ballpos, glm::vec3& racketpos)
{
    return glm::vec3(abs(ballpos.x - racketpos.x), 0.0f, abs(ballpos.z - racketpos.z));
}

bool detectCollision(glm::vec3& ballpos, glm::vec3& racketpos, glm::vec3 normal)
{
    if ((boundaryBox(ballpos, racketpos).z < 1.0f) && (boundaryBox(ballpos, racketpos).x < 1.0f) && (ballpos.y < 8.5f)) // change y based on racket height
    {
        return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    if (!initContext()) return -1;
    // Load Textures
    //

    GLuint whiteTextureID = loadTexture("../assets/textures/white.png");
    GLuint clayTextureID = loadTexture("../assets/textures/clay.jpg");
    GLuint courtTextureID = loadTexture("../assets/textures/court.jpg");
    GLuint staduimTextureID = loadTexture("../assets/textures/staduim.jpg");
    GLuint glossyTextureID = loadTexture("../assets/textures/glossy.jpg");
    GLuint greenTextureID = loadTexture("../assets/textures/green.jpg");
    GLuint blueTextureID = loadTexture("../assets/textures/blue.png");
    GLuint orangeTextureID = loadTexture("../assets/textures/orange.jpg");
    GLuint metalTextureID = loadTexture("../assets/textures/metal.jpg");
    GLuint treeTextureID = loadTexture("../assets/textures/tree.jpg");
    GLuint yellowTreeTextureID = loadTexture("../assets/textures/yellowTree.png");
    GLuint woodTextureID = loadTexture("../assets/textures/wood.jpg");
    GLuint yellowTextureID = loadTexture("../assets/textures/minion.png");
    GLuint sunTextureID = loadTexture("../assets/textures/sun.png");
    GLuint moonTextureID = loadTexture("../assets/textures/electric.png");
    GLuint racket1TextureID = loadTexture("../assets/textures/letter1.png");
    GLuint racket2TextureID = loadTexture("../assets/textures/letter2.jpg");
    GLuint racket3TextureID = loadTexture("../assets/textures/letter4.jpg");
    GLuint skinTextureID = loadTexture("../assets/textures/skin.png");
    GLuint num2ID = loadTexture("../assets/textures/letter4.png");
    GLuint grassID = loadTexture("../assets/textures/grass.png");
        GLuint player1TextureID = loadTexture("../assets/textures/player1.jpg");
    GLuint player2TextureID = loadTexture("../assets/textures/player2.jpg");

    GLuint bleachersTextureID = metalTextureID;


    // Setup models
#if defined(PLATFORM_OSX)
    string heraclesPath = "Models/heracles.obj";
    string cubePath = "Models/cube.obj";
    / string chairPath = "Models/chair.obj";
#else
    string heraclesPath = "../Assets/Models/trees.obj";

    string chairPath = "../Assets/Models/c.obj";
    string manPath = "../Assets/Models/man.obj";
    string boyPath = "../Assets/Models/ballboy.obj";
    string staduimPath = "../Assets/Models/man.obj";
    string lightPath = "../Assets/Models/lights.obj";
    string cloudPath = "../Assets/Models/cloud1.obj";
    string fencePath = "../Assets/Models/fence.obj";
    string moonPath = "../Assets/Models/moon.obj";
    string trophyPath = "../Assets/Models/trophy.obj";
#endif
    // Background Color


    int heraclesVertices;
    GLuint heraclesVAO = setupModelEBO(heraclesPath, heraclesVertices);

    int activeVertices = heraclesVertices;
    GLuint activeVAO = heraclesVAO;

    int staduimVertices;
    GLuint staduimVAO = setupModelEBO(staduimPath, staduimVertices);
    int activeVertices5 = staduimVertices;
    GLuint activeVAO5 = staduimVAO;

    int chairVertices;
    GLuint chairVAO = setupModelEBO(chairPath, chairVertices);

    int activeVertices1 = chairVertices;
    GLuint activeVAO1 = chairVAO;


    int manVertices;
    GLuint manVAO = setupModelEBO(manPath, manVertices);

    int activeVertices2 = manVertices;
    GLuint activeVAO2 = manVAO;
    int boyVertices;
    GLuint boyVAO = setupModelEBO(boyPath, boyVertices);

    int activeVertices3 = boyVertices;
    GLuint activeVAO3 = boyVAO;
    int lightVertices;
    GLuint lightVAO = setupModelEBO(lightPath, lightVertices);

    int activeVertices4 = lightVertices;
    GLuint activeVAO4 = lightVAO;
    int cloudVertices;
    GLuint cloudVAO = setupModelEBO(cloudPath, cloudVertices);

    activeVertices5 = cloudVertices;
    activeVAO5 = cloudVAO;
    int fenceVertices;
    GLuint fenceVAO = setupModelEBO(fencePath, fenceVertices);

    int activeVertices6 = fenceVertices;
    GLuint activeVAO6 = fenceVAO;


    int moonVertices;
    GLuint moonVAO = setupModelEBO(moonPath, moonVertices);

    int activeVertices7 = moonVertices;
    GLuint activeVAO7 = moonVAO;

        int trophyVertices;
    GLuint trophyVAO = setupModelEBO(trophyPath, trophyVertices);

    int activeVertices8 = trophyVertices;
    GLuint activeVAO8 = trophyVAO;

    int shaderProgram = compileAndLinkShaders();
    int depthShaderProgram = compileAndLinkDepthShaders(getDepthVertexShaderSource(), getDepthFragmentShaderSource());
    int texturedShaderProgram = shaderProgram;
    //compileAndLinkTexturedShaders(getVertexShaderSource(), getFragmentShaderSource());
    GLuint textureswitch = glGetUniformLocation(texturedShaderProgram, "useTexture");
    GLuint textureswitch1 = glGetUniformLocation(texturedShaderProgram, "useTexture1");
    int istextureinuse = 0;
    int istextureinuse1 = 0;
    // We can set the shader once, since we have only one
    glDisable(GL_CULL_FACE);
    glUseProgram(texturedShaderProgram);
    glUseProgram(depthShaderProgram);
    glUseProgram(shaderProgram);
    glEnable(GL_POLYGON_OFFSET_FILL);
    GL_LEQUAL;
    glUseProgram(texturedShaderProgram);

    // We can set the shader once, since we have only one
    //glEnable(GL_CULL_FACE);
    glUseProgram(shaderProgram);
    GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
    // Camera parameters for view transform
    //x = width
    // z = length of court
    vec3 cameraPosition(0.0f, 40.0f, 0.0f);
    vec3 cameraLookAt(0.0f, 0.0f, 0.0f); // Set the look at position to the origin
    vec3 cameraUp(0.0f, 1.0f, 0.0f);
    vec3 lightPos(20.0f, 50.0f, 10.0f);
    GLuint lightLocation = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3fv(lightLocation, 1, &lightPos[0]);

    // Other camera parameters
    float cameraSpeed = 1.0f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    float fov = 100.0f;

    mat4 projectionMatrix1 = glm::perspective(fov, // field of view in degrees
        1024.0f / 768.0f,  // aspect ratio
        0.01f, 100.0f);   // near and far (near > 0)
    // Set projection matrix for shader, this won't change
    GLuint projectionMatrixLocation1 = glGetUniformLocation(shaderProgram, "projectionMatrix1");
    glUniformMatrix4fv(projectionMatrixLocation1, 1, GL_FALSE, &projectionMatrix1[0][0]);
    mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
        1024.0f / 768.0f,  // aspect ratio
        0.01f, 100.0f);   // near and far (near > 0)

    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");

    GLuint projectionMatrixLocation_t = glGetUniformLocation(texturedShaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation_t, 1, GL_FALSE, &projectionMatrix[0][0]);

    mat4 viewMatrix = glm::lookAt(cameraPosition, cameraLookAt, cameraUp);


    glUseProgram(shaderProgram);
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    glUseProgram(texturedShaderProgram);
    GLuint viewMatrixLocation_t = glGetUniformLocation(texturedShaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation_t, 1, GL_FALSE, &viewMatrix[0][0]);


    //modifications
    mat4 WorldRotation = rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 1.0f, 1.0f));
    mat4 WorldRotationDeffault = WorldRotation;

    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    GLuint WorldMatrixRotation = glGetUniformLocation(shaderProgram, "WorldRotationMatrix");
    GLenum polygonMode = GL_TRIANGLES;
    // create sphere object
    Ball b1;

    Number one(worldMatrixLocation, colorLocation, polygonMode, texturedShaderProgram, 1, 'p', metalTextureID, metalTextureID, whiteTextureID);
    Number Two(worldMatrixLocation, colorLocation, polygonMode, texturedShaderProgram, 2, 'p', metalTextureID, metalTextureID, whiteTextureID);
    int Score1 = 0;
    int Score2 = 0;
    bool IKey = 1;

    //Create Tennis Objects

    Tennis T1(worldMatrixLocation, colorLocation, polygonMode, texturedShaderProgram, 1, 'p');
    Tennis T2(worldMatrixLocation, colorLocation, polygonMode, texturedShaderProgram, 2, 'y');

    //    T1.ChangeTennisPosition(translate(mat4(1.0f), vec3(7.0f, 0.0f, 7.0f)));
    //    T2.ChangeTennisPosition(translate(mat4(1.0f), vec3(-7.0f, 0.0f, -7.0f)) * scale(mat4(1.0f), vec3(0.35f, 0.35f, 0.35f)));

    glm::vec3 T1Pos(0.0f, 0.0f, 20.0f);
    glm::vec3 T2Pos(0.0f, 0.0f, -20.0f);

    glm::vec3 b1Pos(0.0f, 20.0f, 20.0f);
    glm::vec3 b1Vel(0.0f, 0.0f, 0.0f);
    glm::vec3 b1Acc(0.0f, -9.81f, 0.0f); // this must remain constant
    float bounceCoefficient = 0.9f;

    //T1.ChangeTennisPosition(translate(mat4(1.0f), vec3(0.0f, 0.0f, 3.0f)));
    T1.ChangeTennisPosition(translate(mat4(1.0f), T1Pos)); // position
    T2.ChangeTennisPosition(translate(mat4(1.0f), T2Pos) * scale(mat4(1.0f), vec3(0.35f, 0.35f, 0.35f))); // position


    Tennis* TCurrent = new Tennis();
    TCurrent = &T2;
    mat4* CurrentTranslationMatrix = &TCurrent->UpperarmgroupTransferMatrix;
    mat4* CurrentRotationMatrix = &TCurrent->UpperarmgroupRotationMatrix;
    mat4* CurrentScaleMatrix = &TCurrent->UpperarmgroupScaleMatrix;

    // Define and upload geometry to the GPU here ...
//    int vao = createVertexArrayObject();
    int texturedCubeVAO = createTexturedCubeVertexArrayObject();
    int sphereVAO = b1.createSphereObject();
    int previousSpaceState = GLFW_RELEASE;
    int previousMouseRightState = GLFW_RELEASE;
    int previousMouseLeftState = GLFW_RELEASE;
    int previousMouseMiddleState = GLFW_RELEASE;
    int previousXState = GLFW_RELEASE;
    int previousBState = GLFW_RELEASE;
    double previousMousePosX, previousMousePosY;
    glfwGetCursorPos(window, &previousMousePosX, &previousMousePosY);
    // For frame time
    float previousFrameTime = glfwGetTime();

    // Other OpenGL states to set once before the Game Loop
    // Enable Backface culling
    glClear(GL_COLOR_BUFFER_BIT);
    //  glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    float worldXAngle = 0.0f;
    float worldYAngle = 0.0f;

    //World Orientation
    mat4 worldOrientation = rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 1.0f, 1.0f));
    mat4 worldOrientationInitial = worldOrientation;
    GLuint depthMapFBO;// store the depth value
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    GLuint depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
    glDrawBuffer(GL_NONE);
    GLint frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    GLuint worldLocationMatrix = glGetUniformLocation(shaderProgram, "worldMatrix");
    GLuint lightSpaceMatrixLocation = glGetUniformLocation(shaderProgram, "lightSpaceMatrix");
    GLuint lightSpaceMatrixDepthLocation = glGetUniformLocation(depthShaderProgram, "lightSpaceMatrix");

    //Sonia
    bool gotUserInput = false;
    int playerMode = 0;
    float isDouble = false;
    Tennis T3(worldMatrixLocation, colorLocation, polygonMode, texturedShaderProgram, 3, 's');
    Tennis T4(worldMatrixLocation, colorLocation, polygonMode, texturedShaderProgram, 4, 'f');

    const double PI = 3.14159265358979323846;


    //glUniform1i(textureLocation, 1);
    glBindVertexArray(texturedCubeVAO);

    //
    //    while(!gotUserInput){
    //
    //        std::cout << "Please choose whether you want single (1) or double (2) player mode: ";
    //        std::cin >> playerMode;
    //        if(playerMode==2){
    //            isDouble = true;
    //            T3.ChangeTennisPosition(translate(mat4(1.0f), vec3(-7.0f, 0.0f, 7.0f))) ;
    //            T4.ChangeTennisPosition(translate(mat4(1.0f), vec3(7.0f, 0.0f, -7.0f)) *scale(mat4(1.0f), vec3(0.35f, 0.35f, 0.35f))) ;
    //
    //        }
    //
    //
    //        std::cout << "You may now start your game!\n";
    //        gotUserInput = true;
    //
    //    }
    float lightPower = 250.0f;
    int ub = 20;
    int lb = 0;
    float random[3];
    float moonHeight=0;
    float sunHeight=0;
    // Global variable to store the random value
    bool firstBounce = false;
    float rotation = 0;
    // For the other values between lb and ub

    while (!glfwWindowShouldClose(window)) {

        // Frame time calculation
        float dt = glfwGetTime() - previousFrameTime;
        //sonia
        int sec = (int)glfwGetTime();




        previousFrameTime += dt;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        updateBallPos(b1Pos, b1Vel, b1Acc, dt); // update ball pos

        /// first pass
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glBindVertexArray(texturedCubeVAO);
        glViewport(0, 0, 1024, 1024);
        glClear(GL_DEPTH_BUFFER_BIT);
        glUseProgram(depthShaderProgram);

        GLuint depthWorldMatrixLocation = glGetUniformLocation(depthShaderProgram, "worldMatrix");

        //        glActiveTexture(GL_TEXTURE0);
        //        glBindTexture(GL_TEXTURE_2D, depthTexture);

        //        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        mat4 lightViewMatrix, lightProjectionMatrix;
        lightViewMatrix = lookAt(lightPos, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
        lightProjectionMatrix = ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 200.0f);
        mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;
        glUniformMatrix4fv(lightSpaceMatrixDepthLocation, 1, GL_FALSE, &lightSpaceMatrix[0][0]);

        //draw first pass
//        glBindVertexArray(texturedCubeVAO);
        mat4 groundTennisMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(36.0f, 0.02f, 78.0f));
        glUniformMatrix4fv(depthWorldMatrixLocation, 1, GL_FALSE, &groundTennisMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.39, 0.67, 0.15)));
        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

        glBindVertexArray(sphereVAO);
        mat4 SphereWorldMatrix = translate(mat4(1.0f), b1Pos);
        glUniformMatrix4fv(depthWorldMatrixLocation, 1, GL_FALSE, &SphereWorldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawElements(GL_TRIANGLE_STRIP, b1.indexCount, GL_UNSIGNED_INT, 0);


        // Set light and material properties here, before drawing the mesh
        // You might set these once per frame, or even multiple times per frame if your light or material properties change

        glBindVertexArray(texturedCubeVAO);
        MidSectionCreation(depthWorldMatrixLocation, colorLocation);

        T1.changeWorldMatrix(depthWorldMatrixLocation);
        T1.changeShader(depthShaderProgram);
        T1.Draw(metalTextureID, metalTextureID, racket1TextureID);
        T1.changeWorldMatrix(worldMatrixLocation);

        T2.changeWorldMatrix(depthWorldMatrixLocation);
        T2.changeShader(depthShaderProgram);
        T2.Draw(metalTextureID, metalTextureID, racket2TextureID);
        T2.changeWorldMatrix(worldMatrixLocation);
        if (isDouble) {
            T3.changeWorldMatrix(depthWorldMatrixLocation);
            T3.changeShader(depthShaderProgram);
            T3.Draw(metalTextureID, metalTextureID, racket3TextureID);
            T3.changeWorldMatrix(worldMatrixLocation);

            T4.changeWorldMatrix(depthWorldMatrixLocation);
            T4.changeShader(depthShaderProgram);
            T4.Draw(metalTextureID, metalTextureID, racket3TextureID);
            T4.changeWorldMatrix(worldMatrixLocation);
        }

        ///// second passs

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        setGradientColor();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(texturedShaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glUniform1i(glGetUniformLocation(texturedShaderProgram, "shadowMap"), 0);

        glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
        GLuint textureshaderworld = glGetUniformLocation(texturedShaderProgram, "worldMatrix");

        glUniform1i(textureLocation, 1);
        glBindVertexArray(texturedCubeVAO);





        T1.changeShader(texturedShaderProgram);
        //T1.Draw(metalTextureID, metalTextureID, racket1TextureID);
        DrawBleachers(worldLocationMatrix, texturedCubeVAO, colorLocation, texturedShaderProgram, bleachersTextureID);

        one.DrawScoreBoard();



        if (IKey && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
            Score1++;
            Score2++;
            IKey = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE) {
            IKey = 1;
        }

        if (detectGround(b1Pos, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)))
        {
            b1Vel = glm::vec3(0.0f, bounceCoefficient * 10.0f, 0.0f);
            if (bounceCoefficient > 0.0f)
            {
                if (firstBounce == false)
                {
                    firstBounce = true;
                    if (b1Pos.z > 0.0f)
                    {
                        Score2 += 1;
                    }
                    else if (b1Pos.z < 0.0f)
                    {
                        Score1 += 1;
                    }
                }
                bounceCoefficient -= 0.1f;
            }
            else
            {
                b1Pos = b1Pos * glm::vec3(1.0f, 0.0f, 1.0f);
                b1Pos = b1Pos + glm::vec3(0.0f, 2.0f, 0.0f);
                bounceCoefficient = 1.0f;
                if (b1Pos.z > 0.0f)
                {
                    b1Pos = glm::vec3(0.0f, 20.0f, 20.0f) * glm::vec3(0.0f, 1.0f, -1.0f);
                    firstBounce = false;
                }
                else if (b1Pos.z < 0.0f)
                {
                    b1Pos = glm::vec3(0.0f, 20.0f, 20.0f) * glm::vec3(0.0f, 1.0f, 1.0f);
                    firstBounce = false;
                }
            }
        }
        else if (detectCollision(b1Pos, T1Pos, glm::vec3(0.0f, 0.0f, -1.0f)))
        {
            float randNum = (float)rand() / RAND_MAX;
            b1Vel = glm::vec3(10.0f * randNum, 10.0f, 20.0f) * glm::vec3(1.0f, 1.0f, -1.0f);
        }
        else if (detectCollision(b1Pos, T2Pos, glm::vec3(0.0f, 0.0f, 1.0f)))
        {
            float randNum = (float)rand() / RAND_MAX;
            b1Vel = glm::vec3(10.0f * randNum, 10.0f, 20.0f) * glm::vec3(1.0f, 1.0f, 1.0f);
        }

        one.DrawPlayerScore(Score1);
        Two.DrawPlayerScore(Score2);
        T1.changeShader(texturedShaderProgram);
        T1.Draw(metalTextureID, metalTextureID, racket1TextureID);
        T2.changeShader(texturedShaderProgram);
        T2.Draw(metalTextureID, metalTextureID, racket2TextureID);


        if (isDouble) {
            T3.changeShader(texturedShaderProgram);
            T3.Draw(metalTextureID, metalTextureID, racket3TextureID);
            T4.changeShader(texturedShaderProgram);
            T4.Draw(metalTextureID, metalTextureID, racket3TextureID);

        }
        glBindTexture(GL_TEXTURE_2D, greenTextureID);
        glBindVertexArray(sphereVAO);
        SphereWorldMatrix = translate(mat4(1.0f), b1Pos);
        glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &SphereWorldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawElements(GL_TRIANGLE_STRIP, b1.indexCount, GL_UNSIGNED_INT, 0);





        // Set light and material properties here, before drawing the mesh
        // You might set these once per frame, or even multiple times per frame if your light or material properties change

        glUseProgram(texturedShaderProgram);




        glBindTexture(GL_TEXTURE_2D, courtTextureID);
        glBindVertexArray(texturedCubeVAO);
        mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(38.0f, 0.02f, 76.0f));
        GLuint worldLocationMatrix = glGetUniformLocation(shaderProgram, "worldMatrix");
        GLuint WorldMatrixRotation = glGetUniformLocation(shaderProgram, "worldOrientationMatrix");



        glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundWorldMatrix[0][0]);
        glUniformMatrix4fv(WorldMatrixRotation, 1, GL_FALSE, &worldOrientation[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0


        //DrawGround

        glBindVertexArray(texturedCubeVAO);
        glUseProgram(texturedShaderProgram);
        textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
        textureshaderworld = glGetUniformLocation(texturedShaderProgram, "worldMatrix");
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, clayTextureID);

        glUniform1i(textureLocation, 1);
        mat4 tempworldmatrix = mat4(1.0f);
        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);

        DrawGround(worldLocationMatrix, colorLocation, texturedShaderProgram);
        DrawCoordinates(worldLocationMatrix, colorLocation, worldXAngle, worldYAngle);

        glBindVertexArray(texturedCubeVAO);
        glBindTexture(GL_TEXTURE_2D, clayTextureID);
        groundTennisMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(36.0f, 0.02f, 78.0f));
        glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundTennisMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0, 0.0, 0.0)));
        //  glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

        glBindVertexArray(texturedCubeVAO);
        glUseProgram(texturedShaderProgram);
        textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
        textureshaderworld = glGetUniformLocation(texturedShaderProgram, "worldMatrix");
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, greenTextureID);
        glUniform1i(textureLocation, 1);
        tempworldmatrix = mat4(1.0f);
        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);

        glBindVertexArray(sphereVAO);
        SphereWorldMatrix = translate(mat4(1.0f), b1Pos);
        glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &SphereWorldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawElements(GL_TRIANGLE_STRIP, b1.indexCount, GL_UNSIGNED_INT, 0);


        glBindVertexArray(texturedCubeVAO);

        glBindTexture(GL_TEXTURE_2D, whiteTextureID);

        T1.changeShader(texturedShaderProgram);
        T1.Draw(metalTextureID, metalTextureID, glossyTextureID);
        T2.Draw(metalTextureID, metalTextureID, metalTextureID);

        if (isDouble) {
            T3.Draw(metalTextureID, metalTextureID, metalTextureID);
            T4.Draw(metalTextureID, metalTextureID, metalTextureID);

        }

        //Create BALL
        glBindVertexArray(texturedCubeVAO);
        glUseProgram(texturedShaderProgram);
        glActiveTexture(GL_TEXTURE1);
        textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
        textureshaderworld = glGetUniformLocation(texturedShaderProgram, "worldMatrix");

        glUniform1i(textureLocation, 1);

        tempworldmatrix = mat4(1.0f);
        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &worldOrientation[0][0]);

        glBindTexture(GL_TEXTURE_2D, greenTextureID);
        glBindVertexArray(texturedCubeVAO);
        glUseProgram(texturedShaderProgram);
        textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
        textureshaderworld = glGetUniformLocation(texturedShaderProgram, "worldMatrix");
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, metalTextureID);
        glUniform1i(textureLocation, 1);
        tempworldmatrix = mat4(1.0f);
        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);

        MidSectionCreation(worldLocationMatrix, colorLocation);
        glUseProgram(texturedShaderProgram);
        //draw sky
//        glBindTexture(GL_TEXTURE_2D, greenTextureID);
//        glBindVertexArray(texturedCubeVAO);
//        groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(100.0f, 100.0f, 100.0f));
//
//        glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &groundWorldMatrix[0][0]);
//        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
//        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

        /////////////////////////////////////////////
//// 3D modeling part



        /// this is the trees both side

        int ub = 20;
        int lb = 0;

        // For the first value to be between 0 and 0.3

        // For the other values between lb and ub


        for (int row = 20; row < 40; row += 5) {


            for (int i = -70; i < 70; i = i + 5) {
                std::string str = "0.";
                int integerPart = (int)std::abs(i) % 3;
                std::stringstream ss;
                ss << str << integerPart;
                float result;
                ss >> result;

                glActiveTexture(GL_TEXTURE1);

                if (i % 2 == 0) {

                    glBindTexture(GL_TEXTURE_2D, yellowTreeTextureID);
                }
                else if (i % 3 == 0) {
                    glBindTexture(GL_TEXTURE_2D, orangeTextureID);
                }
                else {
                    glBindTexture(GL_TEXTURE_2D, treeTextureID);
                }


                glUniform1i(textureLocation, 1);

                if (row == 0) {
                    tempworldmatrix =
                        translate(mat4(1.0f), vec3(i, 0.0f, -50.0f - row)) * scale(mat4(1.0f), vec3(0.3f, result + 0.4f, 0.3f));;
                    glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);

                }
                else if (row == 5) {
                    tempworldmatrix =
                        translate(mat4(1.0f), vec3(i + 4.0f, 0.0f, -50.0f - row + 3.0f)) * scale(mat4(1.0f), vec3(0.3f, result + 0.1f, 0.3f));;
                    glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);

                }
                else if (row == 10) {
                    tempworldmatrix =
                        translate(mat4(1.0f), vec3(i + 2.0f, 0.0f, -50.0f - row + 2.0f)) * scale(mat4(1.0f), vec3(0.3f, result + 0.1f, 0.3f));;
                    glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);

                }
                else {
                    tempworldmatrix =
                        translate(mat4(1.0f), vec3(i + 3.0f, 0.0f, -50.0f - row + 1.0f)) * scale(mat4(1.0f), vec3(0.3f, result + 0.2f, 0.3f));;
                    glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);

                }

                glBindVertexArray(activeVAO);
                // Draw geometry
                glDrawElements(GL_TRIANGLE_STRIP, activeVertices, GL_UNSIGNED_INT, 0);
                //glDrawArrays(GL_TRIANGLES, 0, activeVertices);
                // Unbind geometry
                glBindVertexArray(0);
            }

            for (int i = -70; i < 70; i = i + 5) {
                std::string str = "0.";
                int integerPart = (int)std::abs(i) % 3;
                std::stringstream ss;
                ss << str << integerPart;
                float result;
                ss >> result;

                glActiveTexture(GL_TEXTURE1);

                if (i % 2 == 0) {

                    glBindTexture(GL_TEXTURE_2D, yellowTreeTextureID);
                }
                else if (i % 3 == 0) {
                    glBindTexture(GL_TEXTURE_2D, orangeTextureID);
                }
                else {
                    glBindTexture(GL_TEXTURE_2D, treeTextureID);
                }


                glUniform1i(textureLocation, 1);

                if (row == 0) {
                    tempworldmatrix =
                        translate(mat4(1.0f), vec3(i, 0.0f, 50.0f + row)) * scale(mat4(1.0f), vec3(0.3f, result + 0.4f, 0.3f));;
                    glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);

                }
                else if (row == 5) {
                    tempworldmatrix =
                        translate(mat4(1.0f), vec3(i + 4.0f, 0.0f, 50.0f + row + 3.0f)) * scale(mat4(1.0f), vec3(0.3f, result + 0.1f, 0.3f));;
                    glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);

                }
                else if (row == 10) {
                    tempworldmatrix =
                        translate(mat4(1.0f), vec3(i + 2.0f, 0.0f, 50.0f + row + 2.0f)) * scale(mat4(1.0f), vec3(0.3f, result + 0.1f, 0.3f));;
                    glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);

                }
                else {
                    tempworldmatrix =
                        translate(mat4(1.0f), vec3(i + 3.0f, 0.0f, 50.0f + row + 1.0f)) * scale(mat4(1.0f), vec3(0.3f, result + 0.2f, 0.3f));;
                    glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);

                }

                glBindVertexArray(activeVAO);
                // Draw geometry
                glDrawElements(GL_TRIANGLE_STRIP, activeVertices, GL_UNSIGNED_INT, 0);
                //glDrawArrays(GL_TRIANGLES, 0, activeVertices);
                // Unbind geometry
                glBindVertexArray(0);
            }
        }


        // moon
        glBindTexture(GL_TEXTURE_2D, moonTextureID);
        glBindVertexArray(activeVAO7);

        //        float moonHeight = 30*std::cos(glfwGetTime())+30;

        tempworldmatrix = translate(mat4(1.0f), vec3(-75.0f, moonHeight, -75.0f)) * rotate(mat4(1.0f), radians(20.0f), vec3(1.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(5.5f, 5.5f, 5.5f));;
        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawElements(GL_LINES, activeVertices, GL_UNSIGNED_INT, 0);


        //sun
        glBindTexture(GL_TEXTURE_2D, sunTextureID);
        glBindVertexArray(sphereVAO);
        SphereWorldMatrix = translate(mat4(1.0f), vec3(75.0f, sunHeight, -75.0f)) * scale(mat4(1.0f), vec3(6.0f, 6.0f, 6.0f));
        glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &SphereWorldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawElements(GL_TRIANGLE_STRIP, b1.indexCount, GL_UNSIGNED_INT, 0);

        //        glUniform1f(glGetUniformLocation(shaderProgram, "opacity"), 1.0f);
        //        //// the chair on both side

        //        glActiveTexture(GL_TEXTURE1);
        //        glBindTexture(GL_TEXTURE_2D, whiteTextureID);
        //        glUniform1i(textureLocation, 1);
        //        tempworldmatrix = translate(mat4(1.0f), vec3(-22.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 5.0f, 5.0f));
        //        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
        //        glBindVertexArray(activeVAO1);
        //        // Draw geometry
        //        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0, 1.0, 0.0)));
        //        glDrawElements(GL_LINES, activeVertices1, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, woodTextureID);
        tempworldmatrix = translate(mat4(1.0f), vec3(22.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(360.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 5.0f, 5.0f));
        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
        glBindVertexArray(activeVAO1);
        // Draw geometry
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawElements(GL_LINES, activeVertices1, GL_UNSIGNED_INT, 0);


        glBindVertexArray(0);
        if (Score1 == 9)
                    {
                        glBindTexture(GL_TEXTURE_2D, yellowTextureID);
                        glBindVertexArray(activeVAO8);
            
            
            
                        mat4 tempworldmatrix = translate(mat4(1.0f), vec3(0.0f, 20, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(-1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.5f, 0.5f, 0.5f));;
                        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
                        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                        glDrawElements(GL_TRIANGLE_STRIP, activeVertices8, GL_UNSIGNED_INT, 0);
            
                        glUseProgram(texturedShaderProgram);
                        glBindTexture(GL_TEXTURE_2D, player1TextureID);
                        glBindVertexArray(texturedCubeVAO);
                        mat4 celebrationMatrix = translate(mat4(1.0f), vec3(0.0f, 15.0f, 0.0f)) * rotate(mat4(1.0f), radians(180.0f), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(10.0f, 5.0f, 5.0f));
                        worldLocationMatrix = glGetUniformLocation(shaderProgram, "worldMatrix");
                        glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &celebrationMatrix[0][0]);
                        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
            
                        cameraPosition.x = 20.0f;
                        cameraPosition.y = 40.0f;
                        cameraPosition.z = 0.0f;
                        viewMatrix = glm::lookAt(cameraPosition, cameraLookAt, cameraUp);
                        viewMatrix = viewMatrix * rotate(mat4(1.0f), radians(rotation), vec3(0.0f, 1.0f, 0.0f));
                        rotation = rotation - 0.1;
                 
                        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
                        b1.~Ball();
                    }
                    else if (Score2 == 9)
                    {
                        glBindTexture(GL_TEXTURE_2D, yellowTextureID);
                        glBindVertexArray(activeVAO8);
            
            
            
                        mat4  tempworldmatrix = translate(mat4(1.0f), vec3(0.0f, 20, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(-1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.5f, 0.5f, 0.5f));;
                        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
                        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                        glDrawElements(GL_TRIANGLE_STRIP, activeVertices8, GL_UNSIGNED_INT, 0);
            
                        glUseProgram(texturedShaderProgram);
                        glBindTexture(GL_TEXTURE_2D, player2TextureID);
                        glBindVertexArray(texturedCubeVAO);
                        mat4 celebrationMatrix = translate(mat4(1.0f), vec3(0.0f, 15.0f, 0.0f)) * rotate(mat4(1.0f), radians(180.0f), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(10.0f, 5.0f, 5.0f));
                        worldLocationMatrix = glGetUniformLocation(shaderProgram, "worldMatrix");
                        glUniformMatrix4fv(worldLocationMatrix, 1, GL_FALSE, &celebrationMatrix[0][0]);
                        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
                        viewMatrix = viewMatrix * rotate(mat4(1.0f), radians(-1.1f), vec3(0.0f, 1.0f, 0.0f));
                        
                        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
            
            
                        cameraPosition.x = 20.0f;
                        cameraPosition.y = 40.0f;
                        cameraPosition.z = 0.0f;
                        viewMatrix = glm::lookAt(cameraPosition, cameraLookAt, cameraUp);
                        viewMatrix = viewMatrix * rotate(mat4(1.0f), radians(rotation), vec3(0.0f, 1.0f, 0.0f));
                        rotation = rotation - 0.1;
                        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
                        b1.~Ball();
                    }
            


        /// 4 projector of lights
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, metalTextureID);
        glUniform1i(textureLocation, 1);


        glBindVertexArray(activeVAO4);
        // Draw geometry
        tempworldmatrix = translate(mat4(1.0f), vec3(22.0f, 0.0f, 40.0)) * scale(mat4(1.0f), vec3(10.05f, 10.05f, 10.05f));;
        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawElements(GL_TRIANGLE_STRIP, activeVertices4, GL_UNSIGNED_INT, 0);



        // Draw geometry
        tempworldmatrix = translate(mat4(1.0f), vec3(22.0f, 0.0f, -40.0)) * scale(mat4(1.0f), vec3(10.05f, 10.05f, 10.05f));;
        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawElements(GL_TRIANGLE_STRIP, activeVertices4, GL_UNSIGNED_INT, 0);





        // Draw geometry
        tempworldmatrix = translate(mat4(1.0f), vec3(-22.0f, 0.0f, 40.0)) * scale(mat4(1.0f), vec3(10.05f, 10.05f, 10.05f));;
        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawElements(GL_TRIANGLE_STRIP, activeVertices4, GL_UNSIGNED_INT, 0);



        // Draw geometry
        tempworldmatrix = translate(mat4(1.0f), vec3(-22.0f, 0.0f, -40.0)) * scale(mat4(1.0f), vec3(10.05f, 10.05f, 10.05f));;
        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawElements(GL_TRIANGLE_STRIP, activeVertices4, GL_UNSIGNED_INT, 0);



        //////
//        glBindVertexArray(activeVAO5);
        // Draw
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, woodTextureID);
//        glUniform1i(textureLocation, 1);
//        tempworldmatrix = translate(mat4(1.0f), vec3(-30.0f, 0.0f, 0.f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f,1.0f, 1.0f));;
//        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
//        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
//        glDrawElements(GL_TRIANGLES, activeVertices5, GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);

//sonia
        /// MINION
        for (int row = 0; row < 20; row += 5) {

            int height = 0;
            row == 0 ? height = 5 : height;
            row == 5 ? height = 10 : height;
            row == 10 ? height = 15 : height;
            row == 15 ? height = 20 : height;

            if (sec % 2 == 0) {
                for (float i = -30; i < 40; i = i + 5) {
                    if (i == -30 || i == -20 || i == -10 || i == 0 || i == 10 || i == 20 || i == 30) {
                        glActiveTexture(GL_TEXTURE1);
                        glBindTexture(GL_TEXTURE_2D, yellowTextureID);


                        glUniform1i(textureLocation, 1);
                        tempworldmatrix = translate(mat4(1.0f), vec3(-30.0f - row, height + 5.0, i)) *
                            rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *
                            scale(mat4(1.0f), vec3(0.05f, 0.05f, 0.05f));;
                        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
                        glBindVertexArray(activeVAO2);
                        // Draw geometry
                        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                        glDrawElements(GL_TRIANGLES, activeVertices2, GL_UNSIGNED_INT, 0);

                        glBindVertexArray(0);
                    }
                    else {
                        glActiveTexture(GL_TEXTURE1);
                        glBindTexture(GL_TEXTURE_2D, yellowTextureID);
                        glUniform1i(textureLocation, 1);
                        tempworldmatrix = translate(mat4(1.0f), vec3(-30.0f - row, height + 5.0f, i)) *
                            rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *
                            scale(mat4(1.0f), vec3(0.05f, 0.05f, 0.05f));;
                        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
                        glBindVertexArray(activeVAO2);
                        // Draw geometry
                        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                        glDrawElements(GL_TRIANGLES, activeVertices2, GL_UNSIGNED_INT, 0);

                        glBindVertexArray(0);
                    }
                }

                for (float i = -30; i < 40; i = i + 5) {

                    if (i == -30 || i == -20 || i == -10 || i == 0 || i == 10 || i == 20 || i == 30) {
                        glActiveTexture(GL_TEXTURE1);
                        glBindTexture(GL_TEXTURE_2D, yellowTextureID);
                        glUniform1i(textureLocation, 1);
                        tempworldmatrix = translate(mat4(1.0f), vec3(30.0f + row, height + 10.0f, i)) *
                            rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, -1.0f, 0.0f)) *
                            scale(mat4(1.0f), vec3(0.05f, 0.05f, 0.05f));;
                        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
                        glBindVertexArray(activeVAO2);
                        // Draw geometry
                        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                        glDrawElements(GL_TRIANGLES, activeVertices2, GL_UNSIGNED_INT, 0);

                        glBindVertexArray(0);
                    }
                    else {
                        glActiveTexture(GL_TEXTURE1);
                        glBindTexture(GL_TEXTURE_2D, yellowTextureID);
                        glUniform1i(textureLocation, 1);
                        tempworldmatrix = translate(mat4(1.0f), vec3(30.0f + row, height + 5.0f, i)) *
                            rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, -1.0f, 0.0f)) *
                            scale(mat4(1.0f), vec3(0.05f, 0.05f, 0.05f));;
                        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
                        glBindVertexArray(activeVAO2);
                        // Draw geometry
                        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                        glDrawElements(GL_TRIANGLES, activeVertices2, GL_UNSIGNED_INT, 0);

                        glBindVertexArray(0);
                    }

                }
            }
            else {
                for (float i = -30; i < 40; i = i + 5) {
                    if (i == -30 || i == -20 || i == -10 || i == 10 || i == 20 || i == 30) {
                        glActiveTexture(GL_TEXTURE1);
                        glBindTexture(GL_TEXTURE_2D, yellowTextureID);
                        glUniform1i(textureLocation, 1);
                        tempworldmatrix = translate(mat4(1.0f), vec3(-30.0f - row, height + 5.0f, i)) *
                            rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *
                            scale(mat4(1.0f), vec3(0.05f, 0.05f, 0.05f));;
                        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
                        glBindVertexArray(activeVAO2);
                        // Draw geometry
                        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                        glDrawElements(GL_TRIANGLES, activeVertices2, GL_UNSIGNED_INT, 0);

                        glBindVertexArray(0);
                    }
                    else {
                        glActiveTexture(GL_TEXTURE1);
                        glBindTexture(GL_TEXTURE_2D, yellowTextureID);
                        glUniform1i(textureLocation, 1);
                        tempworldmatrix = translate(mat4(1.0f), vec3(-30.0f - row, height + 10.0f, i)) *
                            rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *
                            scale(mat4(1.0f), vec3(0.05f, 0.05f, 0.05f));;
                        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
                        glBindVertexArray(activeVAO2);
                        // Draw geometry
                        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                        glDrawElements(GL_TRIANGLES, activeVertices2, GL_UNSIGNED_INT, 0);

                        glBindVertexArray(0);
                    }
                }


                for (float i = -30; i < 40; i = i + 5) {
                    if (i == -30 || i == -20 || i == -10 || i == 10 || i == 20 || i == 30) {
                        glActiveTexture(GL_TEXTURE1);
                        glBindTexture(GL_TEXTURE_2D, yellowTextureID);
                        glUniform1i(textureLocation, 1);
                        tempworldmatrix = translate(mat4(1.0f), vec3(30.0f + row, height + 5.0f, i)) *
                            rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, -1.0f, 0.0f)) *
                            scale(mat4(1.0f), vec3(0.05f, 0.05f, 0.05f));;
                        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
                        glBindVertexArray(activeVAO2);
                        // Draw geometry
                        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                        glDrawElements(GL_TRIANGLES, activeVertices2, GL_UNSIGNED_INT, 0);

                        glBindVertexArray(0);
                    }
                    else {
                        glActiveTexture(GL_TEXTURE1);
                        glBindTexture(GL_TEXTURE_2D, yellowTextureID);
                        glUniform1i(textureLocation, 1);
                        tempworldmatrix = translate(mat4(1.0f), vec3(30.0f + row, height + 10.0f, i)) *
                            rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, -1.0f, 0.0f)) *
                            scale(mat4(1.0f), vec3(0.05f, 0.05f, 0.05f));;
                        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
                        glBindVertexArray(activeVAO2);
                        // Draw geometry
                        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                        glDrawElements(GL_TRIANGLES, activeVertices2, GL_UNSIGNED_INT, 0);

                        glBindVertexArray(0);
                    }
                }
            }
        }
        ////////////
        //ball boy

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, yellowTextureID);
        glUniform1i(textureLocation, 1);
        tempworldmatrix = translate(mat4(1.0f), vec3(22.0f, 5.0f, 5.0f)) *
            rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, -1.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(0.05f, 0.05f, 0.05f));;
        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
        glBindVertexArray(activeVAO2);
        // Draw geometry
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawElements(GL_TRIANGLES, activeVertices2, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, yellowTextureID);
        glUniform1i(textureLocation, 1);
        tempworldmatrix = translate(mat4(1.0f), vec3(-22.0f, 5.0f, 5.0f)) *
            rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, -1.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(0.05f, 0.05f, 0.05f));;
        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
        glBindVertexArray(activeVAO2);
        // Draw geometry
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        glDrawElements(GL_TRIANGLES, activeVertices2, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        /////////////////////////////// fences
        glBindTexture(GL_TEXTURE_2D, metalTextureID);
        for (float i = -65; i < 65; i = i + 5) {
            glBindVertexArray(activeVAO6);
            tempworldmatrix = translate(mat4(1.0f), vec3(55.0f, 0.0f, i)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.025f, 0.025f, 0.025f));
            glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
            glDrawElements(GL_LINES, activeVertices6, GL_UNSIGNED_INT, 0);
        }
        for (float i = -65; i < 65; i = i + 5) {
            glBindVertexArray(activeVAO6);
            tempworldmatrix = translate(mat4(1.0f), vec3(-55.0f, 0.0f, i)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, -1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.025f, 0.025f, 0.025f));
            glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
            glDrawElements(GL_LINES, activeVertices6, GL_UNSIGNED_INT, 0);
        }
        for (float i = -65; i < 65; i = i + 5) {
            glBindVertexArray(activeVAO6);
            tempworldmatrix = translate(mat4(1.0f), vec3(i, 0.0f, -55.0f)) * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.025f, 0.025f, 0.025f));
            glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
            glDrawElements(GL_LINES, activeVertices6, GL_UNSIGNED_INT, 0);
        }

        for (float i = -65; i < 65; i = i + 5) {
            glBindVertexArray(activeVAO6);
            tempworldmatrix = translate(mat4(1.0f), vec3(i, 0.0f, 55.0f)) * scale(mat4(1.0f), vec3(0.025f, 0.025f, 0.025f));;
            glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
            glDrawElements(GL_LINES, activeVertices6, GL_UNSIGNED_INT, 0);
        }


        //        glActiveTexture(GL_TEXTURE1);
        //        glBindTexture(GL_TEXTURE_2D, yellowTextureID);
        //        glUniform1i(textureLocation, 1);
        //        glBindVertexArray(activeVAO3);
        //        // Draw geometry
        //        tempworldmatrix = translate(mat4(1.0f), vec3(22.0f, 0.0f, 5.0)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, -1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.05f, 0.05f, 0.05f));;
        //        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
        //        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        //        glDrawElements(GL_TRIANGLE_STRIP, activeVertices3, GL_UNSIGNED_INT, 0);
        //
        //
        //
        //        glActiveTexture(GL_TEXTURE1);
        //        glBindTexture(GL_TEXTURE_2D, glossyTextureID);
        //        glUniform1i(textureLocation, 1);
        //        glBindVertexArray(activeVAO3);
        //        // Draw geometry
        //        tempworldmatrix = translate(mat4(1.0f), vec3(-22.0f, 0.0f, 5.0)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.05f, 0.05f, 0.05f));;
        //        glUniformMatrix4fv(textureshaderworld, 1, GL_FALSE, &tempworldmatrix[0][0]);
        //        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
        //        glDrawElements(GL_TRIANGLE_STRIP, activeVertices3, GL_UNSIGNED_INT, 0);
        //        glBindVertexArray(0);

        glUseProgram(shaderProgram);

        // CLose with escape
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }


        // Pressing the spacebar: re-position the model at a random location on the grid.

        if (previousSpaceState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // move camera to the left
        {
            int ub = 20;
            int lb = -20;
            int random[3];
            for (int i = 0; i < 3; i++) {
                random[i] = (rand() % (ub - lb + 1)) + lb;
            }
            *CurrentTranslationMatrix = translate(mat4(1.0f), vec3(random[0] + 0.0f, random[1] + 0.0f, random[2] + 0.0f));
        }
        previousSpaceState = glfwGetKey(window, GLFW_KEY_SPACE);

        //Getting which Tennis to move
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // move left
        {
            TCurrent = &T1;

        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // move left
        {
            TCurrent = &T2;
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) // move left
        {
            TCurrent = &T3;
        }


        //Getting to pick Wich part to move

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // move left
        {
            CurrentTranslationMatrix = &TCurrent->UpperarmgroupTransferMatrix;
            CurrentRotationMatrix = &TCurrent->UpperarmgroupRotationMatrix;
            CurrentScaleMatrix = &TCurrent->UpperarmgroupScaleMatrix;

        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // move left
        {
            CurrentTranslationMatrix = &TCurrent->LowerarmgroupTransferMatrix;
            CurrentRotationMatrix = &TCurrent->LowerarmgroupRotationMatrix;
            CurrentScaleMatrix = &TCurrent->LowerarmgroupScaleMatrix;

        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) // move left
        {
            CurrentTranslationMatrix = &TCurrent->RacketgroupTransferMatrix;
            CurrentRotationMatrix = &TCurrent->RacketgroupRotationMatrix;
            CurrentScaleMatrix = &TCurrent->RacketgroupScaleMatrix;

        }
        //The user can control the model position and orientation using keyboard input i.e.
        // A → move left
        // D → move right
        // W → move up
        // S → move down,
        // a → rotate left 5 degrees about Y axi
        // d → rotate right 5 degrees about Y axis.

        // Translations
        if ( glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move left
        {
            T1.UpperarmgroupTransferMatrix = T1.UpperarmgroupTransferMatrix * translate(mat4(1.0f), vec3(-0.05f, 0.0f, 0.0f));

        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move right
        {
            T1.UpperarmgroupTransferMatrix = T1.UpperarmgroupTransferMatrix * translate(mat4(1.0f), vec3(+0.05f, 0.0f, 0.0f));

        }
        if ( glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move up
        {
            T1.UpperarmgroupTransferMatrix = T1.UpperarmgroupTransferMatrix * translate(mat4(1.0f), vec3(0.0f, 0.05f, 0.0f));

        }
        if ( glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move down
        {
            T1.UpperarmgroupTransferMatrix = T1.UpperarmgroupTransferMatrix * translate(mat4(1.0f), vec3(0.0f, -0.05f, 0.0f));

        }
        if ( glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // move forward
        {
            T1.UpperarmgroupTransferMatrix = T1.UpperarmgroupTransferMatrix * translate(mat4(1.0f), vec3(0.0f, 0.0f, -0.05f));

        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // move backward
        {
            T1.UpperarmgroupTransferMatrix = T1.UpperarmgroupTransferMatrix * translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.05f));

        }



        if ( glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move left
        {
            T2.UpperarmgroupTransferMatrix = T2.UpperarmgroupTransferMatrix * translate(mat4(1.0f), vec3(-0.05f, 0.0f, 0.0f));

        }
        if ( glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move right
        {
            T2.UpperarmgroupTransferMatrix = T2.UpperarmgroupTransferMatrix * translate(mat4(1.0f), vec3(+0.05f, 0.0f, 0.0f));

        }
        if ( glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // move up
        {
            T2.UpperarmgroupTransferMatrix = T2.UpperarmgroupTransferMatrix * translate(mat4(1.0f), vec3(0.0f, 0.05f, 0.0f));

        }
        if ( glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // move down
        {
            T2.UpperarmgroupTransferMatrix = T2.UpperarmgroupTransferMatrix * translate(mat4(1.0f), vec3(0.0f, -0.05f, 0.0f));

        }
        //if ( glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // move forward
        //{
        //    T2.UpperarmgroupTransferMatrix = T2.UpperarmgroupTransferMatrix * translate(mat4(1.0f), vec3(0.0f, 0.0f, -0.05f));

        //}
        //if ( glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // move backward
        //{
        //    T2.UpperarmgroupTransferMatrix = T2.UpperarmgroupTransferMatrix * translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.05f));

    //    }
        ////Rotations
        //if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // rotate left on y-axis
        //{
        //    *CurrentRotationMatrix = *CurrentRotationMatrix * rotate(mat4(1.0f), radians(1.0f), vec3(0.0f, 1.0f, 0.0f)) * translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));

        //}
        //if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // rotate right on y-axis
        //{
        //    *CurrentRotationMatrix = *CurrentRotationMatrix * rotate(mat4(1.0f), radians(-1.0f), vec3(0.0f, 1.0f, 0.0f)) * translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));

        //}
        //if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) // rotate left on z-axis
        //{
        //    *CurrentRotationMatrix = *CurrentRotationMatrix * rotate(mat4(1.0f), radians(1.0f), vec3(0.0f, 0.0f, 1.0f)) * translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));

        //}
        //if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) // rotate right on z-axis
        //{
        //    *CurrentRotationMatrix = *CurrentRotationMatrix * rotate(mat4(1.0f), radians(-1.0f), vec3(0.0f, 0.0f, 1.0f)) * translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));

        //}

        //The user can incrementally size up the model
        // - pressing ‘U’ for scale-up
        //- ‘J’ for scale-down. Each key press should result in a small size change.
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // rotate left
        {
            *CurrentScaleMatrix = *CurrentScaleMatrix * scale(mat4(1.0f), vec3(1.1f, 1.1f, 1.1f));

        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) // rotate right
        {
            *CurrentScaleMatrix = *CurrentScaleMatrix * scale(mat4(1.0f), vec3(0.99f, 0.99f, 0.99f));

        }

        //- The world orientation is changed by using keyboard input i.e.
        //    - left arrow → Rx
        //    - right arrow → R-x
        //    - up arrow → Ry
        //    - down arrow → R-y
        //    - (Rx denotes a small anti-clockwise rotation about positive x axis, R-x about negative x axis, etc.)
       /* if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            worldOrientation = worldOrientation * rotate(mat4(1.0f), radians(1.5f), vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(WorldMatrixRotation, 1, GL_FALSE, &worldOrientation[0][0]);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            worldOrientation = worldOrientation * rotate(mat4(1.0f), radians(-1.5f), vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(WorldMatrixRotation, 1, GL_FALSE, &worldOrientation[0][0]);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            worldOrientation = worldOrientation * rotate(mat4(1.0f), radians(1.5f), vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(WorldMatrixRotation, 1, GL_FALSE, &worldOrientation[0][0]);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            worldOrientation = worldOrientation * rotate(mat4(1.0f), radians(-1.5f), vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(WorldMatrixRotation, 1, GL_FALSE, &worldOrientation[0][0]);
        }
        if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
        {
            worldOrientation = WorldRotationDeffault;
            glUniformMatrix4fv(WorldMatrixRotation, 1, GL_FALSE, &worldOrientation[0][0]);

            *CurrentTranslationMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
            *CurrentRotationMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f));
            *CurrentScaleMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
        }
*/

        //Polygone view

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // move camera to the left
        {
            polygonMode = GL_POINTS;
            TCurrent->setPolygoneMode(polygonMode);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // move camera to the left
        {
            polygonMode = GL_LINES;
            TCurrent->setPolygoneMode(polygonMode);
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) // move camera to the left
        {
            polygonMode = GL_TRIANGLES;
            TCurrent->setPolygoneMode(polygonMode);
        }
        float displacement = 0.0;

        if (previousXState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) // scale down
        {
            glUseProgram(texturedShaderProgram);
            if (istextureinuse == 0) {
                istextureinuse = 1;
                glUniform1i(textureswitch, istextureinuse);
            }
            else {
                istextureinuse = 0;
                glUniform1i(textureswitch, istextureinuse);
            }
            glUseProgram(0);
            shaderProgram = texturedShaderProgram;

        }
        previousXState = glfwGetKey(window, GLFW_KEY_X);


        if (previousBState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) // scale down
        {
            glUseProgram(texturedShaderProgram);
            if (istextureinuse1 == 0) {
                istextureinuse1 = 1;
                glUniform1i(textureswitch1, istextureinuse1);
            }
            else {
                istextureinuse1 = 0;
                glUniform1i(textureswitch1, istextureinuse1);
            }
            glUseProgram(0);
            shaderProgram = texturedShaderProgram;

        }
        previousBState = glfwGetKey(window, GLFW_KEY_B);



        glBindVertexArray(0);

        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();


        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double dx = mousePosX - previousMousePosX;
        double dy = mousePosY - previousMousePosY;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            dy = 0;
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
        {
            dx = 0;
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            if (dy > 0)
            {
                fov += 0.1;
                projectionMatrix = glm::perspective(glm::radians(fov),
                    1024.0f / 768.0f,
                    0.01f, 100.0f);

                GLuint projectionMatriLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
                glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
                dy = 0;
                dx = 0;
            }

            if (dy < 0)
            {
                fov -= 0.1;
                projectionMatrix = glm::perspective(glm::radians(fov),
                    1024.0f / 768.0f,
                    0.01f, 100.0f);

                GLuint projectionMatriLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
                glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
                dy = 0;
                dx = 0;
            }
        }
        previousMouseLeftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        previousMouseRightState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
        previousMouseMiddleState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);

        previousMousePosX = mousePosX;
        previousMousePosY = mousePosY;

        // Convert to spherical coordinates
        const float cameraAngularSpeed = 60.0f;
        cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

        // Clamp vertical angle to [-85, 85] degrees
        cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
        if (cameraHorizontalAngle > 360)
        {
            cameraHorizontalAngle -= 360;
        }
        else if (cameraHorizontalAngle < -360)
        {
            cameraHorizontalAngle += 360;
        }

        float theta = radians(cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);






        if (Score1 != 9 && Score2 != 9)
                    {
            
                        cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
                        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));
            
                        glm::normalize(cameraSideVector);
            
                        viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
            
                        GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
                        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
            
                    }
        // Create a point light


        // ... (Initialize your shaders and objects here)

        // Before rendering, update the light properties in the shader
        moonHeight = 25 * std::cos(glfwGetTime() + 0.5) + 25;
        sunHeight = -25 * std::cos(glfwGetTime() + 0.5) + 25;
        //Light Flicker
        if (sec < 3 || 6 <= sec < 9 || 12 <= sec < 15 || 18 <= sec < 21) {
            lightPower = lightPower + std::cos(sec) * 2.8f;
            GLuint lightLocation = glGetUniformLocation(texturedShaderProgram, "lightPower");
            glUniform1f(lightLocation, lightPower);


        }
        else {

            lightPower = lightPower + std::sin(sec) * 2.8f;
            GLuint lightLocation = glGetUniformLocation(texturedShaderProgram, "lightPower");
            glUniform1f(lightLocation, lightPower);



        }
        glUseProgram(0);
        glUseProgram(shaderProgram);
        GLuint viewMatrixLocation_m = glGetUniformLocation(shaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation_m, 1, GL_FALSE, &viewMatrix[0][0]);
        glUseProgram(texturedShaderProgram);
        GLuint viewMatrixLocation_mt = glGetUniformLocation(texturedShaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation_mt, 1, GL_FALSE, &viewMatrix[0][0]);
        glUseProgram(0);

    }


    // Shutdown GLFW
    glfwTerminate();

    return 0;
}

GLuint setupModelVBO(string path, int& vertexCount) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> UVs;

    // read the vertex data from the model's OBJ file
    loadOBJ(path.c_str(), vertices, normals, UVs);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);  // Becomes active VAO
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and
    // attribute pointer(s).

    // Vertex VBO setup
    GLuint vertices_VBO;
    glGenBuffers(1, &vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
        &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
        (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Normals VBO setup
    GLuint normals_VBO;
    glGenBuffers(1, &normals_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
        &normals.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
        (GLvoid*)0);
    glEnableVertexAttribArray(1);

    // UVs VBO setup
    GLuint uvs_VBO;
    glGenBuffers(1, &uvs_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(),
        GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
        (GLvoid*)0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent
    // strange bugs, as we are using multiple VAOs)
    vertexCount = vertices.size();
    return VAO;
}

GLuint setupModelEBO(string path, int& vertexCount) {
    vector<int> vertexIndices;
    // The contiguous sets of three indices of vertices, normals and UVs, used to
    // make a triangle
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec2> UVs;

    // read the vertices from the cube.obj file
    // We won't be needing the normals or UVs for this program
    loadOBJ2(path.c_str(), vertexIndices, vertices, normals, UVs);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);  // Becomes active VAO
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and
    // attribute pointer(s).

    // Vertex VBO setup
    GLuint vertices_VBO;
    glGenBuffers(1, &vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
        &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
        (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Normals VBO setup
    GLuint normals_VBO;
    glGenBuffers(1, &normals_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
        &normals.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
        (GLvoid*)0);
    glEnableVertexAttribArray(1);

    // UVs VBO setup
    GLuint uvs_VBO;
    glGenBuffers(1, &uvs_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(),
        GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
        (GLvoid*)0);
    glEnableVertexAttribArray(2);

    // EBO setup
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(int),
        &vertexIndices.front(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent
    // strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    vertexCount = vertexIndices.size();
    return VAO;
}