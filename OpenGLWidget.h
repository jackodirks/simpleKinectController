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
    ~OpenGLWidget();

private:
    GLuint textureId;
    bool vertFlip;
    int screen_width, screen_height;

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    QByteArray modifyImage (QByteArray imageArray, const int width, const int height);

public slots:
    void receiveByteArray(QByteArray array);
    void flipImage(bool b);
signals:
    void gotFrame();
};

#endif // OPENGLWIDGET_H
