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
        if (FAILED(hr)) emit error("Error while trying to create Kinect:" + hresultToQstring(hr));
        Kinect kinect(nui);
        kinectMap.insert(i,kinect);
        hr = kinect.getStatus();
        nameMap.insert(i,"Kinect"+QString::number(i)+ hresultToQstring(hr) == "" ? "" : "<" +hresultToQstring(hr) + ">");
    }

    NuiSetDeviceStatusCallback(OnSensorStatusChanged, this);
    return S_OK;
}

//HRESULT KinectManager::fillMaps(){
//{

//            {


//        switch(hr)
//        {

//        default:
//            break;
//        }
//        hr = nui->NuiStatus();

//        switch(hr)
//        {

//        }
//        nameMap.insert(i,text);
//        //nui->Release();
//    }
//    emit mapChanged(nameMap);
//    if (kinectMap.isEmpty()){
//        emit error("No usable Kinects found");
//    }
//    else if (kinect == NULL){
//        kinect = new Kinect(kinectMap.begin().value());
//        kinect->initialize();
//        emit changeSelection(kinectMap.begin().key());
//        emit error("");
//    }
//    return S_OK;
//}

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

void CALLBACK KinectManager::OnSensorStatusChanged( HRESULT hr, const OLECHAR* instanceName, const OLECHAR* uniqueDeviceName, void* userData)
{
    KinectManager* pThis = (KinectManager*)userData;
}
