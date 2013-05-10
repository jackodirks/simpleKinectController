#ifndef KINECT_H
#define KINECT_H

#include <Windows.h>
#include <NuiApi.h>
#include <QMap>
#include <QObject>
#include <atlbase.h>
#include <QByteArray>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrentRun>
#include <QThread>
#include <QMutex>

#ifdef QT_DEBUG
#include <QDebug>
#endif // QT_DEBUG

class Kinect : public QThread
{
    Q_OBJECT
public:
    Kinect(INuiSensor* nui, QObject *parent = 0);
    ~Kinect();

    HRESULT initialize();
    HRESULT uninitialize();

    void run();

    BSTR getDeviceConnectionId(){return nui->NuiDeviceConnectionId();}
    HRESULT getStatus(){return nui->NuiStatus();}


private:
    CComPtr<INuiSensor> nui;
    HANDLE nextColorFrameEvent;
    HANDLE videoStreamHandle;
    QFutureWatcher<void> watcher;
    static void handleKinectHeight(long angle, Kinect* pThis);

    bool continueThread;

public slots:
    void setKinectAngle(long angle);
    void fireKinectAngle();
    void stopThread();
signals:
    void kinectAngleChanged(long angle);
    void error(QString error);
    void videoFrame(QByteArray pBits);
};





#endif // KINECT_H
