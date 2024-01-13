import QtQuick 2.3
//import currentForecastIcon.qml 1.0

Rectangle{
    id: rootWeather
    radius: 20

    Image {
        id: background
        anchors.fill: parent
        //if weather is night, use night background
        //else use day background
        source: (weatherModel.dayTransition === "night" ? "qrc:/infosystemk/assets/sys-night.jpg"
                                                    : "qrc:/infosystemk/assets/sys-day.jpg")
        fillMode: Image.PreserveAspectCrop

    }

    Item {
        id: main
        anchors.fill: parent

        Column {
            spacing: 6

            anchors {
                fill: parent
                topMargin: 6; bottomMargin: 6; leftMargin: 6; rightMargin: 6
            }

            Rectangle {
                id: header
                height: 25
                width: 50
                color:"transparent"
                Text {
                    id: city
                    text: (weatherModel.hasValidCity ? weatherModel.city : "Unknown location")
                          + (weatherModel.useGps ? " (GPS)" : "")
                    anchors.fill: parent
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Image{
                    id:world
                    source: "qrc:/infosystemk/assets/world.png"
                    //add height
                    height: 20
                    width: 20
                    anchors.left: city.right
                    anchors.leftMargin: 10
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (weatherModel.useGps) {
                            weatherModel.useGps = false
                            weatherModel.city = "San Antonio"
                        } else {
                            switch (weatherModel.city) {
                            case "San Antonio":
                                weatherModel.city = "Oslo"
                                break
                            case "Oslo":
                                weatherModel.city = "Helsinki"
                                break
                            case "Helsinki":
                                weatherModel.city = "New York"
                                break
                            case "New York":
                                weatherModel.useGps = true
                                break
                            }
                        }
                    }
                }
            }

        //! [3]
            CurrentWeatherIcon {
                id: current

                width: main.width -12
                height: 2 * (main.height - 25 - 12) / 3

                weatherIcon: (weatherModel.hasValidWeather
                              ? weatherModel.weather.weatherIcon
                              : "sunny")

                dayTransition: weatherModel.dayTransition
        //! [3]
                topText: (weatherModel.hasValidWeather
                          ? weatherModel.weather.temperature
                          : "??")
                bottomText: (weatherModel.hasValidWeather
                             ? weatherModel.weather.weatherDescription
                             : "No weather data")
                imageScale: 0.8
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        weatherModel.refreshWeather()
                    }
                }
        //! [4]
            }
        //! [4]

            Row {
                id: iconRow
                spacing: 10

                width: main.width
                height: (main.height - 25 - 24) / 3
                //center the repeate
                property int daysCount: weatherModel.forecast.length
                property real iconWidth: (daysCount > 0) ? (iconRow.width / daysCount) - 10
                                                         : iconRow.width
                property real iconHeight: iconRow.height
                Repeater {

                    model: weatherModel.forecast
                    delegate: ForecastIcon {
                        id: forecast1
                        width: iconRow.iconWidth
                        height: iconRow.iconHeight
                        topText: (weatherModel.hasValidWeather ? model.dayOfWeek : "??")
                        bottomText: (weatherModel.hasValidWeather ? model.temperature : ("??" + "/??"))
                        weatherIcon: (weatherModel.hasValidWeather ? model.weatherIcon : "sunny")
                    }
                }
            }
        }
    }

}
