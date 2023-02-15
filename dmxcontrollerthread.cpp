#include "dmxcontrollerthread.h"

#include "dmxcontroller.h"

DmxControllerThread::DmxControllerThread(DmxController &controller) :
    QThread{&controller},
    m_controller{controller}
{
}

void DmxControllerThread::run()
{
    while (!isInterruptionRequested())
        m_controller.sendDmxBuffer();
}
