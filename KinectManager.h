#ifndef KINECTMANAGER_H
#define KINECTMANAGER_H
#include "Kinect.h"
#include <QSharedPointer>

class KinectManager : public QObject
{
    Q_OBJECT
public:
    KinectManager();
    ~KinectManager();
    HRESULT initialize();

private:
    QString hresultToQstring(HRESULT hr);
    static void CALLBACK KinectManager::OnSensorStatusChanged( HRESULT hr, const OLECHAR* instanceName, const OLECHAR* uniqueDeviceName, void* userData);
    HRESULT initKinect(QSharedPointer<Kinect> kinect);
    HRESULT uninitKinect(QSharedPointer<Kinect> kinect);
    void changeSelected(int index = -1);

    QMap<int,QString> nameMap;
    QMap<int, QSharedPointer<Kinect>> kinectMap;
    int selectedKinect;

signals:
    void mapChanged(QMap<int,QString> kinectMap);
    void selectionChanged(QString str);
    void error (QString error);
    void kinectAngleChanged(long height); //outgoing signal
    void changeKinectAngle(long angle); //incoming signal
public slots:
    void changeSelected(QString str);
};
#endif // KINECTMANAGER_H
