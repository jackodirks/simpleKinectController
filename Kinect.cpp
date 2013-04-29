#include "Kinect.h"

Kinect::Kinect(INuiSensor *nui, QObject* parent) : QObject(parent)
{
   this->nui = nui;
}

Kinect::~Kinect(){
    uninitialize();
}

HRESULT Kinect::initialize(){
 return S_OK;
}

HRESULT Kinect::uninitialize(){
    return S_OK;
}


