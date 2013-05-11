#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QTimer>
#include <memory>
#include "OpenGLWidget.h"

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
    std::unique_ptr<QTimer> timer;
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
signals:
    void dropDownBoxUpdated(QString str);
    void updateKinectAngle(long angle);
    void receiveVGAArray(QByteArray array);
};

#endif // MAINWINDOW_H
