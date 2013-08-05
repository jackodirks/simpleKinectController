#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QGLWidget(parent)
{
    screen_width = 640;
    screen_height = 480;
    vertFlip = false;
    std::fill_n(blackScreen,640*480*4,0);
}

OpenGLWidget::~OpenGLWidget(){
    glDeleteTextures(1,&textureId); //Remove the texture space in the Graphical Memory
}

void OpenGLWidget::initializeGL(){
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
    }
    glEnable(GL_TEXTURE_2D); //Enables the drawing of 2D textures
    glGenTextures(1,&textureId); //Generates space in the Graphical Memory for a (1) texture and binds this space to textureID
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width(), height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*) NULL);
    glBindTexture(GL_TEXTURE_2D, textureId); //Binds the GL_TEXTURE_2D to the textureId
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width(), height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*) blackScreen);
    glBindTexture(GL_TEXTURE_2D, textureId); //Binds the GL_TEXTURE_2D to the textureId
}

void OpenGLWidget::resizeGL(int width, int height){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 1, -1);
    if (vertFlip){
        flipImage(vertFlip);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGLWidget::paintGL(){
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(width(), 0, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(width(), height(), 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0, height(), 0.0f);
    glEnd();
}

void OpenGLWidget::receiveByteArray(QByteArray array){
    //array = modifyImage(array, width(),height());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width(), height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*)array.data());
    glBindTexture(GL_TEXTURE_2D,textureId);
    updateGL();
    emit gotFrame();
}

void OpenGLWidget::flipImage(bool b){
        vertFlip = b;
        glMatrixMode(GL_PROJECTION);
        glScalef(-1,1,1);
        glTranslatef(-width(),0,0);
}
