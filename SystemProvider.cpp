#include "SystemProvider.h"
#include <QDebug>


SystemProvider::SystemProvider(QObject *parent)
    : QObject{parent}
{
    m_timeTimer = new QTimer(this);
    m_timeTimer->setInterval(500);
    m_timeTimer->setSingleShot(true);
    connect(m_timeTimer, &QTimer::timeout, this, &SystemProvider::setTimeTimer);
    setTimeTimer();

}


QString SystemProvider::driverName() const
{
    return m_driverName;
}

void SystemProvider::setDriverName(const QString &newDriverName)
{
    if (m_driverName == newDriverName)
        return;
    m_driverName = newDriverName;
    emit driverNameChanged();
}

QString SystemProvider::time() const
{

    return m_time;
}

void SystemProvider::setTime(const QString &newTime)
{

    if (m_time == newTime)
        return;

    m_time = newTime;
    emit timeChanged();
}

void SystemProvider::setTimeTimer()
{
    m_time = dateTime.currentDateTime().toString( "h:m ap" );
    qDebug() << m_time;
    setTime(m_time);
    m_timeTimer->start();
}
