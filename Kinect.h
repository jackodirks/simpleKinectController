#ifndef KINECT_H
#define KINECT_H

#include <Windows.h>
#include <NuiApi.h>
#include <QList>

class Kinect
{
public:
    Kinect();
    ~Kinect();
    static HRESULT getSensors (QList<INuiSensor *> &sensorList);
};

class KinectManager
{

};

#endif // KINECT_H
