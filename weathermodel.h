#ifndef WEATHERMODEL_H
#define WEATHERMODEL_H


#include <QObject>
#include <providerapi.h>
#include <QtQml/qqmllist.h>
#include <QtPositioning/qgeopositioninfo.h>
#include <QtPositioning/qgeopositioninfosource.h>
#include <QQmlEngine>

class WeatherData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString dayOfWeek READ dayOfWeek WRITE setDayOfWeek NOTIFY dataChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon WRITE setWeatherIcon NOTIFY dataChanged)
    Q_PROPERTY(QString weatherDescription READ weatherDescription WRITE setWeatherDescription NOTIFY dataChanged)
    Q_PROPERTY(QString temperature READ temperature WRITE setTemperature NOTIFY dataChanged)
    QML_ANONYMOUS


public:
     explicit WeatherData(QObject *parent = 0);
     WeatherData(const WeatherData &other); //passing its own type
     WeatherData(const WeatherInfo &other); //passing a struct

     QString dayOfWeek() const;
     QString weatherIcon() const;
     QString weatherDescription() const;
     QString temperature() const;



 public slots:
     void setDayOfWeek(const QString &newDayOfWeek);
     void setWeatherIcon(const QString &newWeatherIcon);
     void setWeatherDescription(const QString &newWeatherDescription);
     void setTemperature(const QString &newTemperature);


 signals:
     void dataChanged();


 private:
     QString m_dayOfWeek;
     QString m_weatherIcon;
     QString m_weatherDescription;
     QString m_temperature;

};

Q_DECLARE_METATYPE(WeatherData)
class WeatherModelPrivate;

//holdup
class WeatherModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ready
                   READ ready
                       NOTIFY readyChanged)
    Q_PROPERTY(bool hasSource
                   READ hasSource
                       NOTIFY readyChanged)
    Q_PROPERTY(bool hasValidCity
                   READ hasValidCity
                       NOTIFY cityChanged)
    Q_PROPERTY(bool hasValidWeather
                   READ hasValidWeather
                       NOTIFY weatherChanged)
    Q_PROPERTY(bool useGps
                   READ useGps WRITE setUseGps
                       NOTIFY useGpsChanged)
    Q_PROPERTY(QString city
                   READ city WRITE setCity
                       NOTIFY cityChanged)
    Q_PROPERTY(WeatherData *weather
                   READ weather
                       NOTIFY weatherChanged)
    Q_PROPERTY(QQmlListProperty<WeatherData> forecast
                   READ forecast
                       NOTIFY weatherChanged)
    Q_PROPERTY(QString dayTransition READ dayTransition

                       NOTIFY dayTransitionChanged)
    QML_ELEMENT

public:
    explicit WeatherModel(QObject *parent = 0);//it connects to the app model class?? hmmmmm!!!! tf
    ~WeatherModel();

    bool ready() const; //a fear to love oneself is to love another
    bool hasSource() const;
    bool useGps() const;
    bool hasValidCity() const;
    bool hasValidWeather() const;
    void setUseGps(bool value);
    QString city() const;
    void setCity(const QString &value);
    WeatherData *weather() const; //access to current weatherdata in the appmodel
    QQmlListProperty<WeatherData> forecast() const;// storing the weather data in a list

    QString dayTransition() const;


public slots:
    Q_INVOKABLE void refreshWeather();//im guessin recieves a signal from weatherclass

    //! [2] private slots are used to connect to signals internal functions
private slots:
    void positionUpdated(QGeoPositionInfo gpsPos);
    void positionError(QGeoPositionInfoSource::Error e);
    void handleWeatherData(const LocationInfo &location, const QList<WeatherInfo> &weatherDetails);
    void errorMsg();

    //! [3]
signals:
    void readyChanged();
    void useGpsChanged();
    void cityChanged();
    void weatherChanged();
    void dayTransitionChanged();

private:

    bool applyWeatherData(const QString &city, const QList<WeatherInfo> &weatherDetails);
    void requestWeatherByCoordinates();
    void requestWeatherByCity();
    void registerBackend(qsizetype index);
    //void deregisterCurrentBackend();



    WeatherModelPrivate *d; //private pointer to AppModelPrivate type class

    //! [4]

};

#endif // WEATHERMODEL_H
