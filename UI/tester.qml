import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "2D Overlay Example"

    Canvas {
        id: canvas
        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d");
            //paint a square
            ctx.fillStyle = "blue";
            ctx.fillRect(10, 10, 100, 10);
            //paint a circle
            ctx.fillStyle = "red";r
            ctx.beginPath();
            ctx.arc(200, 200, 50, 0, 2 * Math.PI);
            ctx.fill();

        }
    }
}


