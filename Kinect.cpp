#include "Kinect.h"

Kinect::Kinect(INuiSensor *nui, QObject* parent) : QObject(parent)
{
    nextColorFrameEvent= INVALID_HANDLE_VALUE;
    videoStreamHandle= INVALID_HANDLE_VALUE;
    this->nui = nui;
}

Kinect::~Kinect(){
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
#ifdef QT_DEBUG
    DWORD error = GetLastError();
    qDebug()<< error;
#endif
    nui->NuiShutdown();
    return S_OK;
}

void Kinect::setKinectAngle(long angle){
    HRESULT hr = nui->NuiCameraElevationSetAngle(angle);
    if (FAILED(hr)) emit error("Something went wrong while changing the Kinect Height: " + QString::number(hr));
    else fireKinectAngle();
}

void Kinect::fireKinectAngle(){
    long kinectAngle;
    HRESULT hr = nui->NuiCameraElevationGetAngle(&kinectAngle);
    if (FAILED(hr)) emit error("Something went wrong when getting the Kinect Height: " + QString::number(hr));
    else emit kinectAngleChanged(kinectAngle);
}

void Kinect::kinectProcessThread(){

}


