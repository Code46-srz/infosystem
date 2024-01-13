import QtQuick
import QtMultimedia as Multimedia
import QtQuick.Controls
import QtQuick3D
// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Cl

Rectangle {
    id : cameraUI
    property int wheelState: 0
    property real rightAngle: 0.0
    property real leftAngle: 0.0
    property real increment: 0.100
    property real limit: 2.0

    anchors.fill: parent
    state: "PhotoCapture"


    states: [
        State {
            name: "PhotoCapture"
            StateChangeScript {
                script: {
                    camera.start()
                }
            }
        }

    ]

    Multimedia.CaptureSession {
        id: captureSession
        camera: Multimedia.Camera {
            id: camera
        }

        imageCapture: Multimedia.ImageCapture {
            id: imageCapture
        }

        videoOutput: viewfinder
    }


    Multimedia.VideoOutput {
        id: viewfinder
        anchors.fill: parent
        fillMode: Multimedia.VideoOutput.Stretch
        // autoOrientation: true

    }
    function handleScroll(delta) {

        if (delta > 0) {
            // Scrolling up
            if (rightAngle > 0) {
                rightAngle = Math.max(rightAngle - increment, 0.0);
            } else {
                leftAngle = Math.min(leftAngle + increment, limit);
            }
        } else {
            // Scrolling down
            if (leftAngle > 0) {
                leftAngle = Math.max(leftAngle - increment, 0.0);
            } else {
                rightAngle = Math.min(rightAngle + increment, limit);
            }
        }

        console.log("rightAngle:" + rightAngle + " leftAngle:" + leftAngle);
    }
    MouseArea{
        anchors.fill: parent
        onWheel: {
            handleScroll((wheel.angleDelta.y > 0) ? 1 : -1);
        }
    }

    View3D {
        id: view3D
        anchors.fill: parent
        anchors.bottomMargin: -250
        environment: sceneEnvironment

        SceneEnvironment {
            id: sceneEnvironment
            antialiasingMode: SceneEnvironment.MSAA
            antialiasingQuality: SceneEnvironment.High
        }

        Node {
            id: scene
            DirectionalLight {
                id: directionalLight
                x: 3068.612
                y: 3510.133
                brightness: 3.94
                z: 99.70618
                eulerRotation.z: -177.91873
                eulerRotation.y: 85.78448
                eulerRotation.x: -67.75933
            }

            //                PerspectiveCamera {
            //                    id: sceneCamera
            //                    x: 2498.833
            //                    y: 1009.594
            //                    eulerRotation.z: 0
            //                    eulerRotation.y: 90
            //                    eulerRotation.x: -22
            //                    z: 0.00016
            //                }

            //                LineGuide {
            //                    id: lineGuide
            //                    scale.z: 200
            //                    scale.y: 200
            //                    scale.x: 200
            ////                    rightAngle: console.log(rightAngle)
            ////                    leftAngle: console.log(eftAngle)
            //                    //delta: console.log(wheelState)
            //                }

            //                PerspectiveCamera {
            //                    id: sceneCamera
            //                    x: 1754.013
            //                    y: 486.431
            //                    eulerRotation.z: 0
            //                    eulerRotation.y: 90
            //                    eulerRotation.x: -15.5
            //                    z: 0
            //                }

            //                LineGuide {
            //                    id: lineGuide
            //                    eulerRotation.z: 4.58366
            //                    eulerRotation.y: 0
            //                    eulerRotation.x: 0
            //                    scale.z: 200
            //                    scale.y: 200
            //                    scale.x: 200
            //                }
            PerspectiveCamera {
                id: sceneCamera
                x: 590.766
                y: 294.556
                eulerRotation.z: -0
                eulerRotation.y: 89.50001
                eulerRotation.x: -26.5
                z: 5.15547
            }

            LineGuide {
                id: lineGuide
                eulerRotation.z: 18.33898
                eulerRotation.y: 0
                eulerRotation.x: 0
                scale.z: 44.79322
                scale.y: 200
                scale.x: 44.27016
            }
        }
    }

    Item {
        id: __materialLibrary__
        DefaultMaterial {
            id: defaultMaterial
            objectName: "Default Material"
            diffuseColor: "#4aee45"
        }
    }

}




