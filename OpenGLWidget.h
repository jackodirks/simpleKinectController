#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <QByteArray>
#include <QGLWidget>
#include <memory>



class OpenGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = 0);

private:
    GLuint textureId;

    int screen_width, screen_height;

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public slots:
    void receiveByteArray(QByteArray array);

};

#endif // OPENGLWIDGET_H
