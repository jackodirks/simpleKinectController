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

    QMap<int,QString> nameMap;
    QMap<int, QSharedPointer<Kinect>> kinectMap;
    int selectedKinect;

signals:
    void mapChanged(QMap<int,QString> kinectMap);
    void changeSelection(int i);
    void error (QString error);
};
#endif // KINECTMANAGER_H
