#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <windows.h>
#include <gl\gl.h>
#include <QGLWidget>
#include <memory>

#define imageWidth = 640;
#define imageHeight = 480;

class OpenGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = 0);

private:
//    GLuint textureId;
//    GLubyte data;
//    int screen_width, screen_height;

//    void initializeGL();
//    void resizeGL(int w, int h);
//    void paintGL();


};

#endif // OPENGLWIDGET_H
