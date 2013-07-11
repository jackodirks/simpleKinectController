#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QGLWidget(parent)
{
    screen_width = 640;
    screen_height = 480;
    vertFlip = false;
}

OpenGLWidget::~OpenGLWidget(){
    glDeleteTextures(1,&textureId); //Remove the texture space in the Graphical Memory
}

void OpenGLWidget::initializeGL(){
    glEnable(GL_TEXTURE_2D); //Enables the drawing of 2D textures
    glGenTextures(1,&textureId); //Generates space in the Graphical Memory for a (1) texture and binds this space to textureID
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width(), height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*) NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glClearColor(0,0,0,0);
    glClearDepth(1.0f);
}

void OpenGLWidget::resizeGL(int width, int height){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 1, -1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGLWidget::paintGL(){
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
    array = modifyImage(array, width(),height());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width(), height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*) array.data());
    glBindTexture(GL_TEXTURE_2D,textureId);
    updateGL();
}

QByteArray OpenGLWidget::modifyImage(QByteArray imageArray, const int width, const int height){
    if (vertFlip){
        /* Each pixel constist of four unisgned chars: Red Green Blue Alpha.
         * The field is normally 640*480, this means that the whole picture is in fact 640*4 uChars wide.
         * The whole ByteArray is onedimensional, this means that 640*4 is the amount of red of the first pixel of the second row
         */
        QByteArray tempArray = imageArray;
        for (int h = 0; h < height; ++h){
            for (int w = 0; w < width/2; ++w){
                for (int i = 0; i < 4; ++i){
                    imageArray.data()[h*width*4 + 4*w + i] = tempArray.data()[h*width*4 + (4*width - 4*w) + i ];
                    imageArray.data()[h*width*4 + (4*width - 4*w) + i] = tempArray.data()[h*width*4 + 4*w + i];
                }
            }
        }
    }
    return imageArray;
}

void OpenGLWidget::flipImage(bool b){
    vertFlip = b;
}
