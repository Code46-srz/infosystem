#ifndef PROVIDERAPI_H
#define PROVIDERAPI_H

#include <QObject>
#include <QtPositioning/qgeocoordinate.h>
struct WeatherInfo
{ // stores weather information for the inhereted backend class
    QString m_dayOfWeek;
    QString m_weatherIconId;
    QString m_weatherDescription;
    QString m_temperature; //set
    QString m_time;
};

struct LocationInfo
{
    QString m_name;// (city)
    QGeoCoordinate m_coordinate; //defines a position on the earth
};

class providerApi : public QObject
{
    Q_OBJECT
public:
    explicit providerApi(QObject *parent = nullptr);

    virtual void requestWeatherInfo(const QString &city) = 0;//virtual methods meant to be overridden
    virtual void requestWeatherInfo(const QGeoCoordinate &coordinate) = 0;
signals:
    void weatherInformation(const LocationInfo &location, const QList<WeatherInfo> &weatherDetails);
    void errorOccurred();
};

#endif // PROVIDERAPI_H
