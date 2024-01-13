import QtQuick 2.3

Item {
    id: container

    property string weatherIcon: "sunny"
    property string dayTransition: "day"
    property real imageScale: 1.0
    Image {
        id: img
        source: container.weatherIcon == "rain" ||
                    container.weatherIcon == "sleet" ||
                    container.weatherIcon == "snow" ||
                    container.weatherIcon == "thunderstorm" ?
                        "qrc:/infosystemk/assets/weather/" + container.weatherIcon + ".png" :
                        "qrc:/infosystemk/assets/weather/" + container.weatherIcon + "-" + container.dayTransition + ".png"
        smooth: true
        width: parent.width * container.imageScale // Adjust the width of the image
        height: parent.height * container.imageScale // Adjust the height of the image
        //anchors.fill: parent
        anchors.centerIn: parent
    }
}
