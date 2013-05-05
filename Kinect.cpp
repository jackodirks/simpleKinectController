#include "Kinect.h"
Kinect::Kinect(INuiSensor *nui, QObject* parent) :QThread(parent)
{
    nextColorFrameEvent= INVALID_HANDLE_VALUE;
    videoStreamHandle= INVALID_HANDLE_VALUE;
    this->nui = nui;
    connect(&watcher,SIGNAL(finished()),this,SLOT(fireKinectAngle()));
    threadStop = false;
}

Kinect::~Kinect(){
    stopThread();
    uninitialize();
}

HRESULT Kinect::initialize(){
    HRESULT hr;
    DWORD flags = NUI_INITIALIZE_FLAG_USES_COLOR;
    hr = nui->NuiInitialize(flags);
    if (FAILED(hr)) return hr;
    nextColorFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
    hr = nui->NuiImageStreamOpen(
                NUI_IMAGE_TYPE_COLOR,
                NUI_IMAGE_RESOLUTION_640x480,
                0,
                2,
                nextColorFrameEvent,
                &videoStreamHandle);
    if (FAILED(hr)) return hr;
    return S_OK;
}

HRESULT Kinect::uninitialize(){
    if (nextColorFrameEvent && nextColorFrameEvent != INVALID_HANDLE_VALUE)CloseHandle(nextColorFrameEvent);
    nui->NuiShutdown();
    return S_OK;
}

void Kinect::fireKinectAngle(){
    long kinectAngle;
    HRESULT hr = nui->NuiCameraElevationGetAngle(&kinectAngle);
    if (FAILED(hr)) emit error("Something went wrong when getting the Kinect Height: " + QString::number(hr));
    else emit kinectAngleChanged(kinectAngle);
}

void Kinect::run(){
    while(true){
        msleep(30);
        if (threadStop) return;
    }
}

void Kinect::setKinectAngle(long angle){       

    QFuture<void> future = QtConcurrent::run(Kinect::handleKinectHeight,angle,this);
    watcher.setFuture(future);
}

void Kinect::handleKinectHeight(long angle, Kinect* pThis){
    HRESULT hr = pThis->nui->NuiCameraElevationSetAngle(angle);
    if (FAILED(hr)) emit pThis->error("Something went wrong while changing the Kinect Height: " + QString::number(hr));
    return;
}

void Kinect::stopThread(){
    threadStop = true;
}
