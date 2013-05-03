#ifndef KINECT_H
#define KINECT_H

#include <Windows.h>
#include <NuiApi.h>
#include <QMap>
#include <QObject>
#include <atlbase.h>
#include <QByteArray>

#ifdef QT_DEBUG
#include <QDebug>
#endif // QT_DEBUG

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
    void fireKinectAngle();

private:
    CComPtr<INuiSensor> nui;
    HANDLE nextColorFrameEvent;
    HANDLE videoStreamHandle;
    void kinectProcessThread();

public slots:
    void setKinectAngle(long angle);
signals:
    void kinectAngleChanged(long angle);
    void error(QString error);
    void videoFrame(QByteArray pBits, int width, int height);
};





#endif // KINECT_H
