import QtQuick 2.3

Item {
    id: clock

    property int hours
    property int minutes
    property int seconds
    property real shift
    property bool internationalTime: true
    function timeChanged() {
        var date = new Date;
        hours = internationalTime ? date.getUTCHours() + Math.floor(clock.shift) : date.getHours()
        //night = ( hours < 7 || hours > 19 )
        minutes = internationalTime ? date.getUTCMinutes() + ((clock.shift % 1) * 60) : date.getMinutes()
        seconds = date.getUTCSeconds();
    }

    Timer {
        interval: 100; running: true; repeat: true;
        onTriggered: clock.timeChanged()
    }
    Image { id: clockFace
        height: 250
        width: 250
        //center the image
        anchors.centerIn: parent
        source: "qrc:/infosystemk/assets/clock-face.png";
        //visible: clock.night === false

        Image {
            anchors.horizontalCenter: clockFace.horizontalCenter
            y: 36
            source: "qrc:/infosystemk/assets/hours.png";
            height: 90
            width: 5

            transform: Rotation {
                id: hourRotation
                origin.x: 1.5
                origin.y: 88
                angle: (clock.hours * 30) + (clock.minutes * 0.5)
                Behavior on angle {
                    SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
                }
            }
        }
        Image {
            anchors.horizontalCenter: clockFace.horizontalCenter
            //anchors.centerIn: clockFace
            y: 46
            //anchors.top: clockFace.width / 2 - width /2
            source: "qrc:/infosystemk/assets/minutes.png"
            height: 80
            width: 5

            transform: Rotation {
                id: minuteRotation
                origin.x:  3.5; origin.y: 78;
                angle:clock.minutes * 6
                Behavior on angle {
                    SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
                }
            }
        }
        Image {
            anchors.horizontalCenter: clockFace.horizontalCenter
            y: 36
            source: "qrc:/infosystemk/assets/seconds.png"
            height: 90
            width: 2

            transform: Rotation {
                id: secondRotation
                origin.x: 1.5
                origin.y: 88
                angle: clock.seconds * 6
                Behavior on angle {
                    //SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
               }
          }
        }

        Image {
            z: 1
            anchors.centerIn: clockFace
            source:"qrc:/infosystemk/assets/center.png"
        }
    }
}



//transform: Rotation {
// id: secondRotation
// origin.x: 3.5
// origin.y: 78
// angle: clock.seconds * 6
// Behavior on angle {
//     SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
// }
//}
