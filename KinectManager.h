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

    QMap<int,QString> nameMap;
    QMap<int, QSharedPointer<Kinect>> kinectMap;
    int selectedKinect;

signals:
    void mapChanged(QMap<int,QString> kinectMap);
    void selectionChanged(int i);
    void error (QString error);
public slots:
    void changeSelected(int index = -1);
};
#endif // KINECTMANAGER_H
