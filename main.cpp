#include "MainWindow.h"
#include <QApplication>
#include "KinectManager.h"
#include <QMap>

int main(int argc, char *argv[])
{
    //initialize GUI
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    KinectManager manager;
    QObject::connect(&manager,SIGNAL( mapChanged(QMap<int,QString>)),&w,SLOT(setDropDownList(QMap<int,QString>)));
    QObject::connect(&manager,SIGNAL(changeSelection(int)),&w,SLOT(setComboBox(int)));
    HRESULT hr = manager.initialize();
    if (hr == E_NOT_SET)  w.displayError("No usable Kinect found");
    else if (FAILED(hr)) w.displayError("Someting went wrong while making the list of kinects: " + QString::number(hr));
    //if (kinectList.size() == 0) w.displayError("No Kinects to be found.");

    return a.exec();
}


