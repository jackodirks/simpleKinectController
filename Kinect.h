#ifndef KINECT_H
#define KINECT_H

#include <Windows.h>
#include <NuiApi.h>
#include <QMap>
#include <QObject>
#include <memory>

class Kinect : public QObject
{
    Q_OBJECT
public:
    Kinect(INuiSensor* nui, QObject *parent = 0);
    ~Kinect();

    HRESULT initialize();
    HRESULT uninitialize();

    BSTR getDeviceConnectionId(){nui->NuiDeviceConnectionId();}
    HRESULT getStatus(){nui->NuiStatus();}

private:
    std::unique_ptr<INuiSensor> nui;
};



#endif // KINECT_H
