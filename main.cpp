#include "MainWindow.h"
#include <QApplication>
#include "KinectManager.h"
#include <QMap>
#include <Qt>

int main(int argc, char *argv[])
{
    //initialize GUI
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    KinectManager manager;
    qRegisterMetaType<QMap<int,QString>>("QMap<int,QString>");
    QObject::connect(&manager,SIGNAL( mapChanged(QMap<int,QString>)),&w,SLOT(setDropDownList(QMap<int,QString>)));
    QObject::connect(&manager,SIGNAL(selectionChanged(QString)),&w,SLOT(setComboBox(QString)));
    QObject::connect(&manager,SIGNAL(error(QString)),&w,SLOT(displayError(QString)));
    QObject::connect(&manager,SIGNAL(kinectAngleChanged(long)),&w,SLOT(kinectAngle(long)));

    QObject::connect(&w,SIGNAL(dropDownBoxUpdated(QString)),&manager,SLOT(changeSelected(QString)));
    QObject::connect(&w,SIGNAL(updateKinectAngle(long)),&manager,SIGNAL(changeKinectAngle(long)));

    HRESULT hr = manager.initialize();
    if (FAILED(hr)) w.displayError("Something big happend: " + QString::number(hr));

    return a.exec();
}


