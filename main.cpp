#include "MainWindow.h"
#include <QApplication>
#include "Kinect.h"
#include <QMap>


int main(int argc, char *argv[])
{
    //initialize GUI
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //Initialize first Kinect (if available)
    QMap<int, QString>;
    HRESULT hr = Kinect::getSensors(kinectList);
    if (FAILED(hr)) w.displayError("Someting went wrong while making the list of kinects: " + QString::number(hr));
    if (kinectList.size() == 0) w.displayError("No Kinects to be found.");

    return a.exec();
}


