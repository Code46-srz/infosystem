import QtQuick
import QtQuick.Window
import QtQuick.Controls 2.15
import QtMultimedia
import Qt.labs.platform
import infosystemk
Window {
    width: 1000
    height: 480
    visible: true
    color: "black"
    title: qsTr("infotainment System Example")

    Rectangle{
        id: leftScreen
        color: "black"
        width: parent.width / 10

        anchors{
            left: parent.left
            bottom: parent.bottom
            top: parent.top
        }

        Image {
            id: camera
            source: "qrc:/infosystemk/assets/camera.png"
            anchors{
                top:parent.top
                horizontalCenter: parent.horizontalCenter
                topMargin: 20
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    mainLoader.source = "UI/BackUpCamera.qml"
                }
            }

        }

        Image {
            id: maps
            source: "qrc:/infosystemk/assets/google-maps.png"
            anchors{
                top:parent.top
                horizontalCenter: parent.horizontalCenter
                topMargin: 100
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    //stackView.push("map_ui://Gmap.qml") // Switch to Screen2.qml
                    mainLoader.source = "UI/Gmap.qml"
                }
            }
        }

        Image {
            id: music
            source: "qrc:/infosystemk/assets/musical-note.png"
            anchors{
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
                bottomMargin: 100
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    mainLoader.source = "UI/MediaPlayer.qml"

                }
            }
        }
        Image {
            id: settings
            source: "qrc:/infosystemk/assets/settings.png"
            anchors{
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
                bottomMargin: 20
            }
        }

        Image {
            id: weather
            source: "qrc:/infosystemk/assets/weather.png"
            anchors{
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
                bottomMargin: 200
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    mainLoader.source = "UI/weatherForecast.qml"
                }
            }
        }

    }
    Rectangle{
        id:topScreen
        color: "black"
        z: 1
        anchors{
            left: leftScreen.right
            right: parent.right
            top: parent.top
        }
        height: parent.height / 10

        Text {
            id: driverName
            text: system.driverName
            color: "white"
            font.pointSize: 12
            anchors{
                left: parent.left
                verticalCenter: parent.verticalCenter
                leftMargin: 20
            }

        }
        Image {
            id: notification
            source: "qrc:/infosystemk/assets/bell.png"
            sourceSize.height: 20
            sourceSize.width: 20
            anchors{
                right: driverName.left
                verticalCenter: parent.verticalCenter
            }

        }
        WeatherModel {
            id: weatherModel
            city: "San Antonio"

        }

        Image {
            id: location
            source: "qrc:/infosystemk/assets/location.png"
            sourceSize.height: 20
            sourceSize.width: 20
            anchors{
                right: city.left
                verticalCenter: parent.verticalCenter
            }
        }

        Text {
            id: city
            text: weatherModel.city
            color: "white"
            font.pointSize: 12
            anchors{
                right: parent.right
                verticalCenter: parent.verticalCenter
                rightMargin: 200
            }
        }

        Text {
            //
            id: temp
            text: weatherModel.weather.temperature
            color: "white"
            font.pointSize: 12
            anchors{
                right: parent.right
                verticalCenter: parent.verticalCenter
                rightMargin: 100
            }

        }
        Text { //push city further out and temp further out to the left
            id: time
            text: system.time
            color: "white"
            font.pointSize: 12
            anchors{
                right: parent.right
                verticalCenter: parent.verticalCenter
                rightMargin: 10
            }

        }

    }





    Rectangle{
        id:rightScreen

        //        Image {
        //            id: rightScreenBackground
        //            source: (weatherModel.dayTransition === "night" ? "qrc:/infosystemk/assets/sys-night.jpg"
        //                                                            : "qrc:/infosystemk/assets/sys-day.jpg")
        //            fillMode: Image.PreserveAspectCrop
        //        }
        color: "black"

        anchors{
            right: parent.right
            top: topScreen.bottom
            bottom: parent.bottom
        }
        width: parent.width / 3.5
        SwipeView {
            id: view
            orientation: "Vertical"
            currentIndex: 1
            anchors.fill: parent

            CurrentWeatherIcon {
                id: current
                width: view.width - 1
                height: 2 * (view.height - 25 - 12) / 3
                imageScale: 1.0
                weatherIcon: (weatherModel.hasValidWeather
                              ? weatherModel.weather.weatherIcon
                              : "sunny")
                //weatherIconHeight: current.height

                dayTransition: weatherModel.dayTransition

                topText: (weatherModel.hasValidWeather
                          ? weatherModel.weather.temperature
                          : "??")
                bottomText: (weatherModel.hasValidWeather
                             ? weatherModel.weather.weatherDescription
                             : "No weather data")


                //! [4]
            }

            Clock{
                shift: -6
            }

            MediaPlayer {
                imageScale: 1.5
                marginTop: 100
                trackDisplay: "mediaSmall"

            }

        }
        Item {
            id: pageIndicator
            property int count: view.count
            property int currentIndex: view.currentIndex
            width: 10
            height: count * 30
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            Repeater {
                model: pageIndicator.count
                delegate: Rectangle {
                    width: pageIndicator.width
                    height: 10
                    radius: width / 2
                    color: index === pageIndicator.currentIndex ? "red" : "gray"
                    anchors.top: parent.top
                    anchors.topMargin: index * 20
                }
            }
        }
    }

    Rectangle{
        id: centerScreen
        color: "black"

        anchors{
            left: leftScreen.right
            top: topScreen.bottom
            right: rightScreen.left
            bottom: parent.bottom
        }

        Loader{
            id: mainLoader
            anchors.fill: parent

        }
    }
}



