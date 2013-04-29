#include "KinectManager.h"

KinectManager::KinectManager()
{
    selectedKinect = -1;
    kinectMap.clear();
    nameMap.clear();
}

KinectManager::~KinectManager(){

}

HRESULT KinectManager::initialize(){
    //Make the initial list:
    HRESULT hr;
    int kinectCount;

    hr = NuiGetSensorCount(&kinectCount);
    if (FAILED(hr))return hr;
    for (int i = 0; i < kinectCount; i++)
    {
        INuiSensor * nui;
        QString text = "Kinect" + QString::number(i) + " ";
        hr = NuiCreateSensorByIndex(i, &nui);
        if (FAILED(hr)){
            emit error("Error while trying to create Kinect:" + hresultToQstring(hr));
            continue;
        }
        QSharedPointer<Kinect> kinect(new Kinect(nui));
        kinectMap.insert(i,kinect);
        hr = kinect->getStatus();
        nameMap.insert(i,"Kinect"+QString::number(i)+ (hresultToQstring(hr) == "" ? "" : "<" +hresultToQstring(hr) + ">"));
    }
    emit mapChanged(nameMap);
    QMapIterator<int, QSharedPointer<Kinect>> it (kinectMap);
    while (it.hasNext()){
        it.next();
        if ((FAILED (it.value()->getStatus()))) continue;
        selectedKinect = it.key();
        it.value()->initialize();
        break;
    }
    emit changeSelection(selectedKinect);
    if (selectedKinect == -1) emit error("No usable Kinect found");
    NuiSetDeviceStatusCallback(OnSensorStatusChanged, this);
    return S_OK;
}

QString KinectManager::hresultToQstring(HRESULT hr){
    switch(hr){
    case S_OK:
        return "";
    case S_NUI_INITIALIZING:
        return "Still initializing";
    case E_NUI_NOTCONNECTED:
        return "Kinect not connected";
    case E_NUI_NOTGENUINE:
        return "Device is not a valid Kinect";
    case E_NUI_NOTSUPPORTED:
        return"The Kinect is not a supported Kinect";
    case E_NUI_INSUFFICIENTBANDWIDTH:
        return "Not enough USB bandwidth available";
    case E_NUI_NOTPOWERED:
        return "The Kinect is not powered";
    case E_NUI_NOTREADY:
        return "Unspecified error (E_NUI_NOT_READY)";
    case E_OUTOFMEMORY:
        return "Could not allocate memory";
    case E_POINTER:
        return "Unknown error (E_POINTER)";
    default:
        return "Unknown error (" + QString::number(hr)+")";
    }
}

void CALLBACK KinectManager::OnSensorStatusChanged( HRESULT hr, const OLECHAR* instanceName, const OLECHAR*, void* userData)
{
    KinectManager* pThis = (KinectManager*)userData;
    //First: find the Index that belongs to this Kinect
    int index = -1;
    QMapIterator<int, QSharedPointer<Kinect>> it (pThis->kinectMap);
    while (it.hasNext()){
        if (it.value()->getDeviceConnectionId() == instanceName){
            index = it.key();
            break;
        }
    }
    //If the index is still -1 this is a new Kinect:
    if (index == -1){
        for (index = 0; pThis->kinectMap[index] != nullptr; ++index); //Find the first empty int
        //Try to make the Kinect
        INuiSensor* nui;
        HRESULT hresult = NuiCreateSensorById(instanceName,&nui);
        if (FAILED(hresult)){
            emit pThis->error("Error while trying to create Kinect:" + pThis->hresultToQstring(hr));
            return;
        }
        QSharedPointer<Kinect> kinect(new Kinect(nui));
        pThis->kinectMap.insert(index,kinect);
        pThis->nameMap.insert(index,"Kinect"+QString::number(index)+ (pThis->hresultToQstring(hr) == "" ? "" : "<" +pThis->hresultToQstring(hr) + ">"));
        emit pThis->mapChanged(pThis->nameMap);
    }

}
