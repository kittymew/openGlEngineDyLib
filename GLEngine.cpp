//
//  GLEngine.cpp
//  openGlDyLib
//
//  Created by Hoin Kang on 2022/11/03.
//

#include "GLEngine.h"

#include <iostream>

#include "Object.h"

#define EXPORT __attribute__((visibility("default")))

EXPORT
bool GLEngine::init()
{
    bool ret = initOpenGL();
    if(ret == false)
        return false;
    
    initVertexes();
    
    initShader();
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //================================================
    
    return true;
}

bool GLEngine::initOpenGL()
{
    // OPENGL init;
    // 1. glfw 초기화
    if(glfwInit() != GL_TRUE)
    {
        std::cout << stderr << " Failed to initialize GLFW" << std::endl;
        return false;
    }

    // window와 context 생성 전에 설정할 수 있는 옵션들(window 생성 전에 설정해야함)
    // 옵셜들은 glfwInit() 함수 호출 시 디폴트로 초기화됨
    // 특정 플랫폼에서만 영향을 미치는 옵션 존재, 다른 플랫폼에서는 알아서 무시하므로 따로 체크할 필요 없음
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // 2. window 생성
    window_ = glfwCreateWindow(800, 600, "WhiteCat", NULL, NULL);
    if(window_ == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window_);
    
    // 3. glad 초기화
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // current context가 필요함!
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return false;
    }
    
    return true;
}

bool GLEngine::initVertexes()
{
    initTriangleVertex();
    initRectangleVertex();
    
    return true;
}

void GLEngine::initTriangleVertex()
{
    // vertex 입력
    float vertices[] = {
         0.0f,  0.5f, 0.0f,  // 상단
        -0.5f, -0.5f, 0.0f,  // 좌하단
        0.5f, -0.5f, 0.0f,  // 우하단
    };
    
    unsigned int indices[] = {  // 0부터 시작한다는 것을 명심하세요!
        0, 1, 2,   // 첫 번째 삼각형
    };
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    
    // VAO - vertex buffer data와 속성 정보를 저장
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 버퍼에 데이터 복사

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // vertex 설정 지정 - vertex 데이터를 어떻게 해석할지 설정
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // vertex 속성을 사용하도록 지정
    
    umapVAO_.insert({"triangle", VAO});
}

void GLEngine::initRectangleVertex()
{
    // vertex 입력
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // 우측 상단
         0.5f, -0.5f, 0.0f,  // 우측 하단
        -0.5f, -0.5f, 0.0f,  // 좌측 하단
        -0.5f,  0.5f, 0.0f   // 좌측 상단
    };
    unsigned int indices[] = {  // 0부터 시작한다는 것을 명심하세요!
        0, 3, 1,   // 첫 번째 삼각형
        1, 3, 2    // 두 번째 삼각형
    };
    
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    
    // VAO - vertex buffer data와 속성 정보를 저장
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 버퍼에 데이터 복사

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // vertex 설정 지정 - vertex 데이터를 어떻게 해석할지 설정
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // vertex 속성을 사용하도록 지정
    
    umapVAO_.insert({"rectangle", VAO});
}

bool GLEngine::initShader()
{
    // vertex shader
    const char* vertexShaderSource = "#version 330 core\nlayout (location = 0) in vec3 aPos; void main() { gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); }";
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // compile 이 성공했는지 유무를 success에 반환
    if(success == 0)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // 컴파일 실패 시 에러 로그 저장
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader (pixel shader)
    const char* fragmentShaderSource = "#version 330 core\nout vec4 FragColor; void main() { FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); } ";
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(success == 0)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // shader program - 생성한 쉐이더를 실제 렌더링 시에 사용할 수 있도록 연결
    GLuint shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, vertexShader); // 순서는 상관 없을까? 타입별로 알아서 정렬될까?
    glAttachShader(shaderProgram, fragmentShader); // -> 상관없음. 쉐이더 타입 별로 연결 시 순서대로 이어주나 보다.
    glLinkProgram(shaderProgram); // attach 로 추가된 쉐이더를 연결
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(success == 0)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(shaderProgram); // 이후 수행되는 랜더링 명령은 여기서 지정된 프로그램을 사용하게 된다

    glDeleteShader(vertexShader); // link 된 후에는 쉐이더를 삭제해도 무방함 (프로그램 삭제 시 같이 삭제됨)
    glDeleteShader(fragmentShader);
    
    umapShaderProgram.insert({"default", shaderProgram});
    
    return true;
}

EXPORT
void GLEngine::update()
{
    for(Object* pObj : vecObj_)
    {
        pObj->update();
    }
    
    if(glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window_, true);
        setIsEnd(true);
    }
}

EXPORT
void GLEngine::render()
{
    // 렌더링
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 상태 설정 함수 (state-setting)
    glClear(GL_COLOR_BUFFER_BIT); // 상태 사용 함수 (state-using)

    glViewport(0, 0, 800, 600);
    
    for(Object* pObj : vecObj_)
    {
        pObj->render();
    }

    glfwSwapBuffers(window_); // 컬러 버퍼를 교체
    glfwPollEvents(); // 이벤트가 발생하였는지 확인
}

EXPORT
void GLEngine::destroy()
{
    for(Object* pObj : vecObj_)
    {
        if(pObj)
        {
            delete pObj;
        }
    }
    
    for(auto iter : umapVAO_)
    {
        glDeleteVertexArrays(1, &iter.second);
        glDeleteBuffers(1, &iter.second);
    }
    
    for(auto iter : umapShaderProgram)
    {
        glDeleteProgram(iter.second);
    }
    

    glfwTerminate(); // 모든 할당된 채 남아있는 데이터들을 destroy함. glfwInit()이 성공한 후 무조건 호출해야 함.
}

