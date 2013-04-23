#include "Kinect.h"

Kinect::Kinect()
{
}

Kinect::~Kinect(){

}

HRESULT Kinect::getSensors(QList<INuiSensor *>& sensorList){
    HRESULT hr;
    int kinectCount;
    INuiSensor * nui;
    hr = NuiGetSensorCount(&kinectCount);
    if (FAILED(hr))return hr;
    for (int i = 0; i < kinectCount; i++)
    {
        hr = NuiCreateSensorByIndex(i, &nui);
        if (FAILED(hr))
        {
            continue;
        }
        // Get the status of the sensor, and if connected, if we can initialize it.
        hr = nui->NuiStatus();
        if (S_OK == hr)
        {
            sensorList+=nui;
        }
        nui->Release();
    }
    hr = S_OK;
    return hr;
}
