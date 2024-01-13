import QtQuick 2.15
import QtMultimedia 6.5
import QtQuick.Controls 2.15


Rectangle {
    id: rootMediaPlayer
    color: "black"
    property real imageScale: 1.0
    property real side: Math.min(parent.width, parent.height) / 2
    property int marginTop: 60
    property string trackDisplay: "normal"
    state: trackDisplay
    //property real name: value
    //z:2


        Image {
            id: albumArt
            smooth: true
            width: parent.side * rootMediaPlayer.imageScale // Adjust the width of the image
            height: parent.side * rootMediaPlayer.imageScale // Adjust the height of the image
            source: player.AlbumArt ? "image://imgProvider/" + player.AlbumArt : "/infosystemk/assets/defaultMusic"
            //anchors.centerIn: rootMediaPlayer
            anchors{
            top: parent.top
            topMargin: marginTop
            horizontalCenter: parent.horizontalCenter
            }

           // horizontalCenter: parent.horizontalCenter
        }
        Text{
            id: songTitle
            text: player.title ? player.title : "No Title"

             color: "white"
            anchors{
                top: albumArt.bottom
                topMargin: 15
                horizontalCenter: parent.horizontalCenter
            }
        }
        Text{
            id: songArtist
            text: player.artist ? player.artist : "No Artist"
            color: "white"
            anchors{
                top: songTitle.bottom
                topMargin: 5
                //center
                horizontalCenter: parent.horizontalCenter


            }
        }


    Item {
        id: trackTime
        anchors{
            right: parent.right
            rightMargin: 50
            bottom: parent.bottom
            bottomMargin: 50
        }


    Text {//left side countdown of timer for track
        id: position
        anchors{
            right: duration.left
            //rightMargin: 20
        }

           //set playser.position to 0

           text: (player.state === 0 ? "0:00/" :formatRemainingTime(player.position)+"/");


        fontSizeMode: Text.Fit; font.pixelSize: 15
        color: "white"
        function formatRemainingTime(milliseconds) {
            var remainingMinutes = Math.floor(milliseconds / 60000);
            //console.log(remainingMinutes);
            var remainingSeconds = Math.floor((milliseconds % 60000) / 1000);
            // Return formatted string
            console.log(remainingSeconds)
            console.log(remainingMinutes)
            console.log(remainingMinutes + ":" + (remainingSeconds < 10 ? '0' : '') + remainingSeconds);
            return remainingMinutes + ":" + (remainingSeconds < 10 ? '0' : '') + remainingSeconds;
        }

    }

    Text{
        id: duration
        text:Math.floor((player.duration/ 60000) % 60) + ":" + Math.floor((player.duration/ 1000) % 60)
        color: "white"
        fontSizeMode: Text.Fit; font.pixelSize: 15
    }
}
    states:[
        State {
            name: "mediaSmall"
            AnchorChanges {
                target: trackTime
                 //anchors.left: songTitle.right
                 anchors.top: albumArt.bottom
                 anchors.right: parent.right
                 //anchors.bottom: undefined
                 //anchors.horizontalCenter: parent.horizontalCenter
            }
          PropertyChanges {
              target: trackTime
              anchors.topMargin: 15
              anchors.leftMargin: 50
              rightMargin: undefined
              bottomMargin: undefined
              //anchors.horizontalCenter: parent.horizontalCenter

          }
        }
]

    Image{
        id:rewindButton
        source: "/infosystemk/assets/rewind-button.png"
        anchors{
            right:  actionButton.left
            bottom: parent.bottom
            bottomMargin: 25
            rightMargin: 50
//          leftMargin: 180
        }
    }

    Image {
        id: actionButton
        source:(player.state === 0 || player.state === 2) ? "/infosystemk/assets/play.png" : "/infosystemk/assets/pause.png";

        anchors{
            //center horizontally and vertically
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: 25
        }

        MouseArea{
            anchors.fill: parent
            onPressed:{
                player.state === 1 ? player.pause() : player.play();
            }
        }
    }

    Image {
        id: fowardButton
        source: "/infosystemk/assets/forward-button.png"

        anchors{
            left: actionButton.right
            bottom: parent.bottom
            bottomMargin: 25
            leftMargin: 50
//            rightMargin: 180
        }
    }
    Rectangle {
       height: 10
       //give it the width of the parent
       width: parent.width
       Rectangle{

           gradient: Gradient {
               GradientStop { position: 0.0; color: "red" }
               GradientStop { position: 0.33; color: "yellow" }
               GradientStop { position: 1.0; color: "green" }
           }
       }

        anchors{
            bottom: parent.bottom
        }
    }
}


