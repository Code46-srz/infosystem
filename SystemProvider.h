#ifndef SYSTEMPROVIDER_H
#define SYSTEMPROVIDER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>

class SystemProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString driverName READ driverName WRITE setDriverName NOTIFY driverNameChanged FINAL)
    Q_PROPERTY(QString time READ time NOTIFY timeChanged FINAL)
public:
    explicit SystemProvider(QObject *parent = nullptr);

    QString driverName() const;
    void setDriverName(const QString &newDriverName);

    QString time() const;
    void setTime(const QString &newTime);
    void setTimeTimer();

signals:

    void driverNameChanged();
    void timeChanged();
    void timeTimerChanged();

private:
    QString m_driverName;
    QString m_time;
    QTimer * m_timeTimer;
    QDateTime dateTime;
};

#endif // SYSTEMPROVIDER_H
