#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <windows.h>
#include <gl/glew.h>
#include <QByteArray>
#include <QGLWidget>
#include <memory>
#include <QTimer>

class OpenGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();

private:
    GLuint textureId;
    bool vertFlip;
    int screen_width, screen_height, openGLMajorVersion;
    unsigned char blackScreen[640*480*4];
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    std::unique_ptr<QTimer> blackoutTimer; //This timer resets the screen to black if there has not been a fram for 3 seconds.

public slots:
    void receiveByteArray(QByteArray array);
    void flipImage(bool b);
    void blackOutScreen();
signals:
    void gotFrame();
};

#endif // OPENGLWIDGET_H
