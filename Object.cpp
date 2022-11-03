//
//  Object.cpp
//  openGlDyLib
//
//  Created by Hoin Kang on 2022/11/04.
//

#include "GLEngine.h"
#include "Object.h"

Object::Object()
{

}

void Object::update()
{
    
}

void Object::render()
{
    glUseProgram(shaderProgram_);
    glBindVertexArray(vao_);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
