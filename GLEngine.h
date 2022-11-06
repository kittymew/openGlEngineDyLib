//
//  GLEngine.h
//  openGlDyLib
//
//  Created by Hoin Kang on 2022/11/03.
//

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Object;

class GLEngine
{
private:
    class GLFWwindow* window_;
    
private:
    std::vector<Object*> vecObj_;
    
    std::unordered_map<std::string, GLuint> umapShaderProgram;
    std::unordered_map<std::string, GLuint> umapVAO_;
    
    bool isEnd_ = false;
    
public:
    bool init();
    
    bool initOpenGL();
    bool initVertexes();
    bool initShader();
    
    void initTriangleVertex();
    void initRectangleVertex();
    
public:
    bool isEnd() { return isEnd_; };
    
    void setIsEnd(bool isEnd) { isEnd_ = true; }
    
    GLuint getShaderProgram(std::string_view name) { return umapShaderProgram[name.data()]; }
    GLuint getVAO(std::string_view name) { return umapVAO_[name.data()]; }
    
    void update();
    
    void render();
    
    void destroy();
    
};

