#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <windows.h>
#include <gl/glew.h>
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
    unsigned char blackScreen[640*480*4];
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public slots:
    void receiveByteArray(QByteArray array);
    void flipImage(bool b);
signals:
    void gotFrame();
};

#endif // OPENGLWIDGET_H
