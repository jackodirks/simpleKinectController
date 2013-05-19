#include "Kinect.h"

const int width = 640;
const int height = 480;

Kinect::Kinect(INuiSensor *nui, QObject* parent) :QThread(parent)
{
    this->nui = nui;
    connect(&watcher,SIGNAL(finished()),this,SLOT(fireKinectAngle()));
    continueThread = true;
}

Kinect::~Kinect(){
    stopThread();
    uninitialize();
}

HRESULT Kinect::initialize(){
    HANDLE nextColorFrameEvent, videoStreamHandle;
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
    if (FAILED(hr)){
        //uninitialize();
        return hr;
    }
    this->nextColorFrameEvent.Attach(nextColorFrameEvent);
    this->videoStreamHandle.Attach(videoStreamHandle);
    return S_OK;
}

HRESULT Kinect::uninitialize(){
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
    while(continueThread){
        if (WAIT_OBJECT_0 ==  WaitForSingleObject(nextColorFrameEvent,30) ){
            NUI_IMAGE_FRAME imageFrame;
            NUI_LOCKED_RECT LockedRect;
            if(FAILED(nui->NuiImageStreamGetNextFrame(videoStreamHandle,0,&imageFrame)))continue;
            INuiFrameTexture *texture = imageFrame.pFrameTexture;
            texture->LockRect(0,&LockedRect,NULL,0);
            BYTE* curr = LockedRect.pBits;
            QByteArray byteArray = QByteArray::QByteArray((char*)curr,width*height*4);
            emit videoFrame(byteArray);
            texture->UnlockRect(0);
            nui->NuiImageStreamReleaseFrame(videoStreamHandle,&imageFrame);
        }
    }
    continueThread = true;
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
    continueThread = false;
}
