//
//  Object.h
//  openGlDyLib
//
//  Created by Hoin Kang on 2022/11/04.
//

#pragma once

class Object
{
protected:
    GLuint shaderProgram_;
    GLuint vao_;
    
public:
    virtual void update();
    virtual void render();
    
public:
    Object();
    
};
