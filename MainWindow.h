#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QTimer>
#include <memory>
#include "OpenGLWidget.h"
#include <QMutex>
#include <QLabel>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    
private:
    Ui::MainWindow *ui;
    std::unique_ptr<QTimer> clearErrorTimer;
    std::unique_ptr<QTimer> fpsTimer;
    int frames;
    std::unique_ptr<QMutex> fpsMutex;
    std::unique_ptr<QLabel> fpsLabel;
private slots:
    void setNewValue(int val);
public slots:
    void setDropDownList(QMap<int,QString> map);
    void setComboBox(QString str);
    void displayError(QString error);
    void kinectAngle(long angle);
private slots:
    void emptyErrorLabel();
    void buttonPressToUpdateKinectAngle();
    void gotFrame();
    void fpsTimeOut();
signals:
    void dropDownBoxUpdated(QString str);
    void updateKinectAngle(long angle);
    void receiveVGAArray(QByteArray array);
    void setStatus(QString msg, int time);
};

#endif // MAINWINDOW_H
