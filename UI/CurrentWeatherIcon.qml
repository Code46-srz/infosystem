import QtQuick 2.3

Item {
    id: current

    property string topText: "20*"
    property string bottomText: "Mostly cloudy"
    property string weatherIcon: "sunny"
    property string dayTransition: "day"
    property real imageScale: 1.0
//    property int weatherIconHeight: parent.height / 2
//    property int weatherIconWidth: parent.width / 4

    //property real smallSide: (current.width < current.height ? current.width : current.height)
    property real side: Math.min(parent.width, parent.height) / 2

    Text {
        text: current.topText
        color: "white"
        font.pointSize: 28
        anchors {
            top: current.top
            right: current.right
            topMargin: 5
            leftMargin: 5
        }
    }

    WeatherIcon {
        weatherIcon: current.weatherIcon
        dayTransition: current.dayTransition
        anchors.centerIn: parent
        height: current.side//parent.height / 2
        width: current.side//parent.width / 4
        imageScale: current.imageScale

    }

    Text {
        text: current.bottomText
        color:"white"
        font.pointSize: 23
        wrapMode: Text.WordWrap
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        anchors {
            bottom: current.bottom
            right: current.right
            rightMargin: 5
        }
    }
}
