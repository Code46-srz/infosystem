#ifndef METEOMATICSBACKEND_H
#define METEOMATICSBACKEND_H

#include "providerapi.h"

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
class QNetworkReply;
QT_END_NAMESPACE

class meteoMaticsBackend : public providerApi
{
public:
    explicit meteoMaticsBackend(QObject *parent = nullptr);

    void requestWeatherInfo(const QString &city) override;
    void requestWeatherInfo(const QGeoCoordinate &coordinate) override;

private slots:
    void handleWeatherForecastReply(QNetworkReply *reply, const LocationInfo &location);
private:
    void generateWeatherRequest(const QString &city, const QGeoCoordinate &coordinate);

    QNetworkAccessManager *m_networkManager;
};

#endif // METEOMATICSBACKEND_H
