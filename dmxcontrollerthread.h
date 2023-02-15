#pragma once

#include <QThread>

class DmxController;

class DmxControllerThread : public QThread
{
    Q_OBJECT

public:
    explicit DmxControllerThread(DmxController &controller);

    void run() override;

private:
    DmxController &m_controller;
};
