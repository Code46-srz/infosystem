#include "meteomaticsbackend.h"
#include <QtCore/qloggingcategory.h>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtCore/qurlquery.h>
#include <QtCore/qtimezone.h>
#include <QtNetwork/qnetworkreply.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <chrono>
#include <iomanip>
#include <ctime>

Q_DECLARE_LOGGING_CATEGORY(requestsLog)
using namespace Qt::StringLiterals;
static constexpr qsizetype kMaxEntries = 7; //its the size of the array??

static QString weatherCodeToIcon(int code)//todo: add night version
{
    switch (code) {
    case 0:
        return "clearsky"_L1;//done
    case 1:
        return "few-clouds"_L1;//done
    case 2:
        return "few-clouds"_L1;
    case 3:
        return "few-clouds"_L1;
    case 45:
    case 48:
        return "fog"_L1;//done
    case 51:
    case 53:
    case 55:
    case 56:
    case 57:
        return "rain"_L1;//done
    case 61:
    case 63:
    case 65:
    case 80:
    case 81:
    case 82:
        return "rain"_L1;
    case 66:
    case 67:
        return "sleet"_L1;//done
    case 71:
    case 73:
    case 75:
    case 77:
    case 85:
    case 86:
        return "snow"_L1;//done
    case 95:
    case 96:
    case 99:
        return "thunderstorm"_L1;//done
    }


    return "sunny"_L1; // default
}
static QString nightDaySwitch(QString time)
{
    if (!time.isEmpty())
    {
        time < "06.00" && time > "18.00" ? time = "night"_L1 : time = "day"_L1;
    }
    qDebug() << time;
    return time;
}
static QString weatherCodeToDescription(int code)
{
    switch (code) {
    case 0:
        return "clear sky"_L1;
    case 1:
        return "mainly clear"_L1;
    case 2:
        return "partly cloudy"_L1;
    case 3:
        return "overcast"_L1;
    case 45:
    case 48:
        return "fog"_L1;
    case 51:
    case 53:
    case 55:
    case 56:
    case 57:
        return "drizzle"_L1;
    case 61:
    case 63:
    case 65:
        return "rain"_L1;
    case 66:
    case 67:
        return "freezing rain"_L1;
    case 71:
    case 73:
    case 75:
    case 77:
    case 85:
    case 86:
        return "snow"_L1;
    case 80:
    case 81:
    case 82:
        return "pouring rain"_L1;
    case 95:
    case 96:
    case 99:
        return "thunderstorm"_L1;
    }
    return QString();
}

//supported cities coralate with qml
static QGeoCoordinate coordinateForCity(const QString &city)
{
    if (city == "San Antonio"_L1)
        return QGeoCoordinate(29.4241, -98.4936);
    else if (city == "Oslo"_L1)
        return QGeoCoordinate(59.8939, 10.7151);
    else if (city == "Helsinki"_L1)
        return QGeoCoordinate(60.1103, 24.8786);
    else if (city == "New York"_L1)
        return QGeoCoordinate(40.6977, -74.1198);

    return QGeoCoordinate();
}

static QString niceTemperatureString(double t)//formats the temperature to a string
{
    return QString::number(qRound(t)) + QChar(0xB0);
}

meteoMaticsBackend::meteoMaticsBackend(QObject *parent)
    : providerApi{parent},
    m_networkManager{new QNetworkAccessManager(this)}
{

}

void meteoMaticsBackend::requestWeatherInfo(const QString &city)//for city
{
    const auto coordinate = coordinateForCity(city);
    if (!coordinate.isValid()) {
        qCDebug(requestsLog) << "Weather request for unknown city:" << city;
        emit errorOccurred();
        return;
    }
    generateWeatherRequest(city, coordinate);
}

void meteoMaticsBackend::requestWeatherInfo(const QGeoCoordinate &coordinate)//for coordinates
{
    generateWeatherRequest(QString(), coordinate);
}


void meteoMaticsBackend::generateWeatherRequest(const QString &city, const QGeoCoordinate &coordinate)
{
    QUrl url("https://api.open-meteo.com/v1/forecast"_L1);

    QUrlQuery query;
    query.addQueryItem("latitude"_L1, QString::number(coordinate.latitude()));
    query.addQueryItem("longitude"_L1, QString::number(coordinate.longitude()));
    query.addQueryItem("daily"_L1, "weathercode,temperature_2m_max,temperature_2m_min"_L1);
    query.addQueryItem("temperature_unit"_L1, "fahrenheit"_L1);
    query.addQueryItem("current_weather"_L1, "true"_L1);
    query.addQueryItem("timezone"_L1, QString::fromLatin1(QTimeZone::systemTimeZoneId()));
    query.addQueryItem("timeformat"_L1, "unixtime"_L1);

    url.setQuery(query);
    //qDebug() << url ;
    // We always need to have a non-empty "city" name to display to the user.
    // Other backends provide this information, but here we just use the string
    // representation of QGeoCoordinate if the city is unknown.
    // If the city is known, we pass an empty QGeoCoordinate object, so that
    // the WeatherDataCache is populated correctly.
    const LocationInfo info = city.isEmpty()
                                  ? LocationInfo{coordinate.toString(QGeoCoordinate::DegreesMinutesWithHemisphere),
                                                 coordinate}//passing the coordinates if string format
                                  : LocationInfo{city, QGeoCoordinate()};

    //idk why but we need to pass the url to QNetworkRequest smh
    QNetworkReply *reply = m_networkManager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, this, [this, reply, info]() {
        handleWeatherForecastReply(reply, info);
    });
}

void meteoMaticsBackend::handleWeatherForecastReply(QNetworkReply *reply,
                                                  const LocationInfo &location)
{

    if (!reply) { //checking if response is empty
        emit errorOccurred();
        return;
    }

    bool parsed = false;

    QList<WeatherInfo> weatherData;

    if (!reply->error()) {
        const QJsonDocument document = QJsonDocument::fromJson(reply->readAll());//read from request
        const QJsonObject documentObject = document.object();


        const QJsonValue offsetVal = documentObject.value("utc_offset_seconds"_L1);
        const qint64 utcOffset = !offsetVal.isUndefined() ? offsetVal.toInteger() : -1;

        // current weather
        WeatherInfo currentWeather;

        const QJsonObject currWeatherObj = documentObject.value("current_weather"_L1).toObject();
        const QJsonValue currTemp = currWeatherObj.value("temperature"_L1);

        if (currTemp.isDouble())//if the temperature is a double pass it to the niceTemp func
            currentWeather.m_temperature = niceTemperatureString(currTemp.toDouble());

        const QJsonValue weatherCode = currWeatherObj.value("weathercode"_L1);//weather code


        if (weatherCode.isDouble()) {
            const int code = weatherCode.toInt(-1);
            if (code >= 0) {
                currentWeather.m_weatherIconId = weatherCodeToIcon(code);
                currentWeather.m_weatherDescription = weatherCodeToDescription(code);
            }
        }

        //get the timeformat in a QString variable
        if (!currentWeather.m_temperature.isEmpty()
            && !currentWeather.m_weatherIconId.isEmpty()
            && !currentWeather.m_weatherDescription.isEmpty()
            ){
            weatherData.append(currentWeather);
        } else {
            qCDebug(requestsLog) << "Failed to extract current weather";
        }


        // forecast weather
        const QJsonObject dailyData = documentObject.value("daily"_L1).toObject();
        const QJsonArray days = dailyData.value("time"_L1).toArray();
        const QJsonArray weatherCodes = dailyData.value("weathercode"_L1).toArray();
        const QJsonArray maxTemperatures = dailyData.value("temperature_2m_max"_L1).toArray();
        const QJsonArray minTemperatures = dailyData.value("temperature_2m_min"_L1).toArray();

        if (days.size() >= kMaxEntries && weatherCodes.size() >= kMaxEntries
            && maxTemperatures.size() >= kMaxEntries
            && minTemperatures.size() >= kMaxEntries) {

            for (qsizetype i = 0; i < kMaxEntries; ++i)
            {
                WeatherInfo info;

                const qint64 unixTime = days.at(i).toInteger(-1);

                if (unixTime > 0) {
                    const QDateTime date = QDateTime::fromSecsSinceEpoch(unixTime + utcOffset);
                    info.m_dayOfWeek = date.toString("ddd"_L1);
                }
                const int code = weatherCodes.at(i).toInt(-1);
                if (code >= 0)
                    info.m_weatherIconId = weatherCodeToIcon(code);
                const double minTemp = minTemperatures.at(i).toDouble();
                const double maxTemp = maxTemperatures.at(i).toDouble();
                info.m_temperature = niceTemperatureString(minTemp) + u'/'
                                     + niceTemperatureString(maxTemp);

                if (!info.m_dayOfWeek.isEmpty() && !info.m_temperature.isEmpty()
                    && !info.m_weatherIconId.isEmpty()) {
                    weatherData.append(info);
                } else {
                    qCDebug(requestsLog) << "Failed to extract weather forecast";
                    break;
                }
            }
        }

        parsed = (utcOffset != -1) && (weatherData.size() == (kMaxEntries + 1));
    }

    if (parsed) {
        emit weatherInformation(location, weatherData);
    } else {
        emit errorOccurred();
        if (reply->error())
            qCDebug(requestsLog) << reply->errorString();
        else
            qCDebug(requestsLog, "Failed to parse weather JSON.");
    }

    reply->deleteLater();//deleteLater is just a function
}

