#include "weathermodel.h"
#include <QDateTime>
#include <QtPositioning/qgeocircle.h>
#include <QtPositioning/qgeopositioninfosource.h>
#include <meteomaticsbackend.h>
#include <QtCore/qloggingcategory.h>
#include <QTime>
#include <QDebug>

Q_LOGGING_CATEGORY(requestsLog, "wapp.requests")

WeatherData::WeatherData(QObject *parent)
    : QObject{parent}
{

}

WeatherData::WeatherData(const WeatherData &other)
    : QObject{nullptr},  // initialize QObject base class
    m_dayOfWeek{other.m_dayOfWeek},
    m_weatherIcon{other.m_weatherIcon},
    m_weatherDescription{other.m_weatherDescription},
    m_temperature{other.m_temperature}
{
    // Additional initialization if needed
}

WeatherData::WeatherData(const WeatherInfo &other)
    : QObject{nullptr} //we are setting the the variables with data from the other struct
    , m_dayOfWeek{other.m_dayOfWeek}
    , m_weatherIcon{other.m_weatherIconId}
    , m_weatherDescription{other.m_weatherDescription}
    , m_temperature{other.m_temperature}
{

}



QString WeatherData::dayOfWeek() const
{
    return m_dayOfWeek;
}

void WeatherData::setDayOfWeek(const QString &newDayOfWeek)
{
    if (m_dayOfWeek == newDayOfWeek)
        return;
    m_dayOfWeek = newDayOfWeek;
    emit dataChanged();
}

QString WeatherData::weatherIcon() const
{
    return m_weatherIcon;
}

void WeatherData::setWeatherIcon(const QString &newWeatherIcon)
{
    //qDebug() << newWeatherIcon;
    if (m_weatherIcon == newWeatherIcon)
        return;
    m_weatherIcon = newWeatherIcon;
    emit dataChanged();
}

QString WeatherData::weatherDescription() const
{
    return m_weatherDescription;
}

void WeatherData::setWeatherDescription(const QString &newWeatherDescription)
{
    if (m_weatherDescription == newWeatherDescription)
        return;
    m_weatherDescription = newWeatherDescription;
    emit dataChanged();
}

QString WeatherData::temperature() const
{
    return m_temperature;
}

void WeatherData::setTemperature(const QString &newTemperature)
{
    if (m_temperature == newTemperature)
        return;
    m_temperature = newTemperature;
    emit dataChanged();
}

class WeatherDataCache
{
public:
    WeatherDataCache() = default;

    using WeatherDataPair = QPair<QString, QList<WeatherInfo>>;

    WeatherDataPair getWeatherData(const QString &name) const; //gets data by name
    WeatherDataPair getWeatherData(const QGeoCoordinate &coordinate) const; //gets data by coordinate(GPS)

    void addCacheElement(const LocationInfo &location, const QList<WeatherInfo> &info);//function adds data to cache

    static bool isCacheResultValid(const WeatherDataPair &result);//functions that checks validity of data

private:
    struct CacheItem
    {
        qint64 m_cacheTime;
        QList<WeatherInfo> m_weatherData;
    };

    QMap<QString, CacheItem> m_cityCache;

    QGeoCoordinate m_gpsLocation; //default values
    QString m_gpsName;
    CacheItem m_gpsData;

    static const qint64 kCacheTimeoutInterval = 3600; // 1 hour
    static const int kCircleRadius = 3000; // 3 km
};

WeatherDataCache::WeatherDataPair WeatherDataCache::getWeatherData(const QString &name) const
{
    if (m_cityCache.contains(name)) {
        const qint64 currentTime = QDateTime::currentSecsSinceEpoch();
        const auto &item = m_cityCache.value(name);
        if (currentTime - item.m_cacheTime < kCacheTimeoutInterval)
            return qMakePair(name, item.m_weatherData);
    }
    return qMakePair(QString(), QList<WeatherInfo>());
}

WeatherDataCache::WeatherDataPair
WeatherDataCache::getWeatherData(const QGeoCoordinate &coordinate) const
{
    if (m_gpsLocation.isValid() && !m_gpsName.isEmpty()) {
        const QGeoCircle area(m_gpsLocation, kCircleRadius);
        if (area.contains(coordinate)) {
            const qint64 currentTime = QDateTime::currentSecsSinceEpoch();
            if (currentTime - m_gpsData.m_cacheTime < kCacheTimeoutInterval)
                return qMakePair(m_gpsName, m_gpsData.m_weatherData);
        }
    }

    return qMakePair(QString(), QList<WeatherInfo>()); //idk where this weatherInfo is defined?
}

void WeatherDataCache::addCacheElement(const LocationInfo &location,
                                       const QList<WeatherInfo> &info)
{
    // It it expected that we have valid QGeoCoordinate only when the weather
    // is received based on coordinates.

    //we will use this to add directly to set our weathedata
    const qint64 currentTime = QDateTime::currentSecsSinceEpoch();
    if (location.m_coordinate.isValid()) {
        m_gpsLocation = location.m_coordinate;
        m_gpsName = location.m_name;
        m_gpsData = { currentTime, info };
    } else {
        m_cityCache[location.m_name] = { currentTime, info };
    }
}

bool WeatherDataCache::isCacheResultValid(const WeatherDataCache::WeatherDataPair &result)
{
    return !result.first.isEmpty() && !result.second.isEmpty();
}

class WeatherModelPrivate
{
public:
    QGeoPositionInfoSource *src = nullptr;
    QGeoCoordinate coord;
    QString city;
    WeatherData now;///
    QList<WeatherData*> forecast;
    QQmlListProperty<WeatherData> *fcProp = nullptr;
    bool ready = false;
    bool useGps = true;
    WeatherDataCache m_dataCache;
    providerApi *m_currentBackend = nullptr;
    QString m_dayTransition;
};



//STAND ALONE FUNCTIONS
static void forecastAppend(QQmlListProperty<WeatherData> *prop, WeatherData *val)
{
    Q_UNUSED(val);
    Q_UNUSED(prop);
}

static WeatherData *forecastAt(QQmlListProperty<WeatherData> *prop, qsizetype index)
{
    WeatherModelPrivate *d = static_cast<WeatherModelPrivate*>(prop->data);
    //auto count = d->forecast.at(index);
    //qDebug() << "forecastAt:" << d->forecast.at(index);
    return d->forecast.at(index);
}

static qsizetype forecastCount(QQmlListProperty<WeatherData> *prop)
{
    WeatherModelPrivate *d = static_cast<WeatherModelPrivate*>(prop->data);
    auto count = d->forecast.size();
    //qDebug() << "forecastCount:" << count;
    return d->forecast.size();
}

static void forecastClear(QQmlListProperty<WeatherData> *prop)
{
    static_cast<WeatherModelPrivate*>(prop->data)->forecast.clear();
}
//END OF STAND ALONE FUNCTIONS


WeatherModel::WeatherModel(QObject *parent) :
    QObject(parent),
    d(new WeatherModelPrivate) //initialzed a private object
{
    //! [0]
    d->fcProp = new QQmlListProperty<WeatherData>(this, d, forecastAppend,
                                                  forecastCount,
                                                  forecastAt,
                                                  forecastClear);


    //d->m_supportedBackends.push_back(new meteoMaticsBackend(this));
    d->m_currentBackend = new meteoMaticsBackend(this);
    connect(d->m_currentBackend, &providerApi::weatherInformation,
            this, &WeatherModel::handleWeatherData);
    connect(d->m_currentBackend, &providerApi::errorOccurred,
            this, &WeatherModel::errorMsg);



    //registerBackend(0);

    //! [1]
    d->src = QGeoPositionInfoSource::createDefaultSource(this);

    if (d->src) {
        d->useGps = true;
        connect(d->src, &QGeoPositionInfoSource::positionUpdated,
                this, &WeatherModel::positionUpdated);
        connect(d->src, &QGeoPositionInfoSource::errorOccurred,
                this, &WeatherModel::positionError);
        d->src->startUpdates();
    } else {
        d->useGps = false;
        d->city = "Brisbane";
        emit cityChanged();
        requestWeatherByCity();
    }
}
//! [1]

WeatherModel::~WeatherModel()
{
    if (d->src)
        d->src->stopUpdates();
    if (d->fcProp)
        delete d->fcProp;
    qDeleteAll(d->forecast);
    delete d;
}

//! [2]
void WeatherModel::positionUpdated(QGeoPositionInfo gpsPos)
{
    d->coord = gpsPos.coordinate();

    if (!d->useGps)
        return;

    requestWeatherByCoordinates();
}
//! [2]

void WeatherModel::positionError(QGeoPositionInfoSource::Error e)
{
    Q_UNUSED(e);
    qWarning() << "Position source error. Falling back to simulation mode.";

    // activate simulation mode
    if (d->useGps) {
        d->useGps = false;
        d->city = "Brisbane";
        emit cityChanged();
        requestWeatherByCity();
    }
}

void WeatherModel::refreshWeather()
{
    if (d->city.isEmpty()) {
        qCDebug(requestsLog) << "refreshing weather skipped (no city)";
        return;
    }
    qCDebug(requestsLog) << "refreshing weather";
    requestWeatherByCity();
}

void WeatherModel::handleWeatherData(const LocationInfo &location, //
                                 const QList<WeatherInfo> &weatherDetails)
{

    if (applyWeatherData(location.m_name, weatherDetails)) {
        //if setting the values was successful, we can cache them
        d->m_dataCache.addCacheElement(location, weatherDetails);
    }
}

void WeatherModel::errorMsg()
{

        qWarning("No information available at the time");

}

bool WeatherModel::applyWeatherData(const QString &city, const QList<WeatherInfo> &weatherDetails)
{
    // Check that we didn't get outdated weather data. The city should match,
    // if only we do not use GPS.
        //we will compare with weatherdata object now
    if (city != d->city && !d->useGps)
        return false;

    if (city != d->city && d->useGps) {
        d->city = city;
        emit cityChanged();
    }

    // delete previous forecast
    qDeleteAll(d->forecast);
    d->forecast.clear();

    // The first item in the list represents current weather.
    if (!weatherDetails.isEmpty()) {
        d->now.setTemperature(weatherDetails.first().m_temperature);
        d->now.setWeatherIcon(weatherDetails.first().m_weatherIconId);
        d->now.setWeatherDescription(weatherDetails.first().m_weatherDescription);
    }

    // The other items represent weather forecast. The amount of items depends
    // on the provider backend.
    for (qsizetype i = 1; i < weatherDetails.size(); ++i) {
        WeatherData *forecastEntry = new WeatherData(weatherDetails.at(i));
        d->forecast.append(forecastEntry);
        //qDebug() << "forecastEntry:" << d->forecast;
    }



    if (!d->ready) {
        d->ready = true;
        emit readyChanged();
    }

    emit weatherChanged();

    return true;
}

void WeatherModel::requestWeatherByCoordinates()
{
    const auto cacheResult = d->m_dataCache.getWeatherData(d->coord);

    if (WeatherDataCache::isCacheResultValid(cacheResult))
        applyWeatherData(cacheResult.first, cacheResult.second);

    else if (d->m_currentBackend)//change it to the backend i have
        d->m_currentBackend->requestWeatherInfo(d->coord);
}

void WeatherModel::requestWeatherByCity()
{
    const auto cacheResult = d->m_dataCache.getWeatherData(d->city);

    if (WeatherDataCache::isCacheResultValid(cacheResult))
        //qDebug() << "cacheResult is valid" << cacheResult.first << cacheResult.second;
        applyWeatherData(cacheResult.first, cacheResult.second);
    else if (d->m_currentBackend)
        d->m_currentBackend->requestWeatherInfo(d->city);
}

//void WeatherModel::registerBackend(qsizetype index)
//{
//    if (index >= 0 && index < d->m_supportedBackends.size()) {
//        d->m_currentBackend = d->m_supportedBackends.at(index);
//        d->m_currentBackendIndex = index;
//        //qDebug(requestsLog) << "registering backend" << index;
//        connect(d->m_currentBackend, &providerApi::weatherInformation,
//                this, &WeatherModel::handleWeatherData);

//        connect(d->m_currentBackend, &providerApi::errorOccurred,
//                this, &WeatherModel::errorMsg);
//    }
//}

//void WeatherModel::deregisterCurrentBackend()
//{
//    if (d->m_currentBackend) {
//        disconnect(d->m_currentBackend, &providerApi::weatherInformation,
//                   this, &WeatherModel::handleWeatherData);
//        disconnect(d->m_currentBackend, &providerApi::errorOccurred,
//                   this, &WeatherModel::switchToNextBackend);
//        d->m_currentBackend = nullptr;
//    }
//}

bool WeatherModel::hasValidCity() const
{
    return (!(d->city.isEmpty()) && d->city.size() > 1 && d->city != "");
}

bool WeatherModel::hasValidWeather() const
{
    return hasValidCity() && (!(d->now.weatherIcon().isEmpty()) &&
                              (d->now.weatherIcon().size() > 1) &&
                              d->now.weatherIcon() != "");
}

WeatherData *WeatherModel::weather() const
{
    return &(d->now);
}

QQmlListProperty<WeatherData> WeatherModel::forecast() const
{
    return *(d->fcProp);
}

bool WeatherModel::ready() const
{
    return d->ready;
}

bool WeatherModel::hasSource() const
{
    return (d->src != NULL);
}

bool WeatherModel::useGps() const
{
    return d->useGps;
}

void WeatherModel::setUseGps(bool value)
{
    d->useGps = value;
    if (value) {
        d->city = "";
        emit cityChanged();
        emit weatherChanged();
        // if we already have a valid GPS position, do not wait until it
        // updates, but query the city immediately
        if (d->coord.isValid())
            requestWeatherByCoordinates();
    }
    emit useGpsChanged();
}

QString WeatherModel::city() const
{
    return d->city;
}

void WeatherModel::setCity(const QString &value)
{
    d->city = value;

    emit cityChanged();
    requestWeatherByCity();
}



QString WeatherModel::dayTransition() const
{    QString time = QTime::currentTime().toString("hh:mm");
    //newDayTransition = time;
    //qDebug() << "time:" << time;
    if (time >= "06:00" && time <= "18:00")
    {
        d->m_dayTransition = "day";
    }
    else{
        d->m_dayTransition = "night";
    }
    //qDebug() << "dayTransition:" << d->m_dayTransition;

    return d->m_dayTransition;
    //emit dayTransitionChanged();
}


