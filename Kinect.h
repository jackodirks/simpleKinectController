#ifndef KINECT_H
#define KINECT_H

#include <Windows.h>
#include <NuiApi.h>
#include <QMap>
#include <QObject>
#include <atlbase.h>

class Kinect : public QObject
{
    Q_OBJECT
public:
    Kinect(INuiSensor* nui, QObject *parent = 0);
    ~Kinect();

    HRESULT initialize();
    HRESULT uninitialize();

    BSTR getDeviceConnectionId(){return nui->NuiDeviceConnectionId();}
    HRESULT getStatus(){return nui->NuiStatus();}

private:
    CComPtr<INuiSensor> nui;
    static void releaseINuiSensor(INuiSensor* sensor){
        sensor->Release();
    }
};





#endif // KINECT_H
