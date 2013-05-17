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
    changeSelected();
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

void KinectManager::changeSelected(QString str){
    changeSelected(nameMap.key(str));
}

void KinectManager::changeSelected(int i){
    if (i != -1 && i == selectedKinect) return; //This is already the selected Kinect, do nothing
    if (i == -1){ //"default": check if there is a Kinect currently active and if yes do nothing
        if (selectedKinect != -1){
            emit selectionChanged(nameMap.value(selectedKinect));
            return; //If there is no Kinect chosen pick the default one (first usable one in the map)
        }
        else{
            QMapIterator<int, QSharedPointer<Kinect>> it (kinectMap);
            while (it.hasNext()){
                it.next();
                if ((FAILED (it.value()->getStatus()))) continue;
                selectedKinect = it.key();
				if (SUCCEEDED(initKinect(it.value()))) break;
            }
            emit selectionChanged(nameMap.value(selectedKinect));
            if (selectedKinect == -1) emit error("No usable Kinect found.");
        }
    }
    else if (kinectMap.value(i) == nullptr || FAILED(kinectMap.value(i)->getStatus())){ //unusable kinect (or non-existing one)
        emit selectionChanged(nameMap.value(selectedKinect));
        emit error("This Kinect is not usable");
    }
    else {
        HRESULT hr = uninitKinect(kinectMap.value(selectedKinect));
        if (FAILED(hr)){
            emit error("Failed to select Kinect:" + hresultToQstring(hr));
            selectedKinect = -1;
            emit selectionChanged(nameMap.value(selectedKinect));
            return;
        }
        hr = initKinect(kinectMap.value(i));
        if (FAILED(hr)){
            emit error("Failed to select Kinect:" + hresultToQstring(hr));
            selectedKinect = -1;
            emit selectionChanged(nameMap.value(selectedKinect));
            return;
        }
        selectedKinect = i;
        emit selectionChanged(nameMap.value(selectedKinect));
    }
}

HRESULT KinectManager::initKinect(QSharedPointer<Kinect> kinect){
    HRESULT hr = kinect->initialize();
	if (FAILED(hr)){
        emit error("Error while initializing Kinect (HRESULT " + QString::number(hr) + ")");
		return hr;
	}
    connect(kinect.data(),SIGNAL( kinectAngleChanged(long)),this,SIGNAL(kinectAngleChanged(long)));
    connect(this,SIGNAL(changeKinectAngle(long)),kinect.data(),SLOT(setKinectAngle(long)));
    connect(kinect.data(),SIGNAL(error(QString)),this,SIGNAL(error(QString)));
    connect(this,SIGNAL(stopThread()),kinect.data(),SLOT(stopThread()));
    connect(kinect.data(),SIGNAL(videoFrame(QByteArray)),this,SIGNAL(sendKinectByteArray(QByteArray)));
    kinect->fireKinectAngle();
    kinect->start();
    return hr;
}

HRESULT KinectManager::uninitKinect(QSharedPointer<Kinect> kinect){
    emit stopThread();
    kinect->wait();
    disconnect(kinect.data(),SIGNAL(kinectAngleChanged(long)),this,SIGNAL(kinectAngleChanged(long)));
    disconnect(this,SIGNAL(changeKinectAngle(long)),kinect.data(),SLOT(setKinectAngle(long)));
    disconnect(kinect.data(),SIGNAL(error(QString)),this,SIGNAL(error(QString)));
    disconnect(this,SIGNAL(stopThread()),kinect.data(),SLOT(stopThread()));
    disconnect(kinect.data(),SIGNAL(videoFrame(QByteArray)),this,SIGNAL(sendKinectByteArray(QByteArray)));
    return kinect ->uninitialize();
}

void CALLBACK KinectManager::OnSensorStatusChanged( HRESULT hr, const OLECHAR* instanceName, const OLECHAR*, void* userData)
{
    KinectManager* pThis = (KinectManager*)userData;
    //First: find the Index that belongs to this Kinect
    int index = -1;
    QMapIterator<int, QSharedPointer<Kinect>> it (pThis->kinectMap);
    while (it.hasNext()){
        it.next();
        if (wcscmp(it.value()->getDeviceConnectionId(),instanceName) == 0){
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
        pThis->changeSelected();
    } else {
        if (hr == E_NUI_NOTCONNECTED){  //The Kinect has been disconnected
            pThis->uninitKinect(pThis->kinectMap.value(index));
            pThis->kinectMap.remove(index);
            pThis->nameMap.remove(index);
            pThis->selectedKinect = -1;
        } else {    //The status of the kinect has changed and it needs to be updated
            pThis->nameMap.insert(index,"Kinect"+QString::number(index)+ (pThis->hresultToQstring(hr) == "" ? "" : "<" +pThis->hresultToQstring(hr) + ">"));
            if (FAILED(hr) && pThis->selectedKinect == index){
                pThis->uninitKinect(pThis->kinectMap.value(index));
                pThis->selectedKinect = -1;

            }
        }
        emit pThis->mapChanged(pThis->nameMap);
        pThis->changeSelected();
    }

}
