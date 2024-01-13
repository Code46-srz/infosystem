import QtQuick
import QtQuick3D


    Node {
        id: node
        property int  delta: 0


        // Resources
        PrincipledMaterial {
            id: material_001_material
            baseColor: "#ffcc0506"
            roughness: 0.5
            cullMode: PrincipledMaterial.NoCulling
            alphaMode: PrincipledMaterial.Opaque
        }
        PrincipledMaterial {
            id: material_002_material
            baseColor: "#ffcc3e06"
            roughness: 0.5
            cullMode: PrincipledMaterial.NoCulling
            alphaMode: PrincipledMaterial.Opaque
        }


        MorphTarget {
            id: morphTarget //right left angle
            weight: leftAngle //rightAngle //0.0
            attributes: MorphTarget.Position | MorphTarget.Normal
        }
        MorphTarget {
            id: morphTarget7 //right right angle
            weight:rightAngle
            attributes: MorphTarget.Position | MorphTarget.Normal
        }

        MorphTarget { //left right angle
            id: morphTarget11
            weight: rightAngle
            attributes: MorphTarget.Position | MorphTarget.Normal
        }
        MorphTarget {//left left angle
            id: morphTarget12
            weight: leftAngle
            attributes: MorphTarget.Position | MorphTarget.Normal
        }

        // Nodes:
        Node {
            id: root
            Model {
                id: cube_002
                position: Qt.vector3d(6.42338, 0, -2.84965)
                rotation: Qt.quaternion(0, 0, 1, 0)
                scale: Qt.vector3d(1.20059, 0.002626, 0.125446)
                source: "meshes/cube_002_mesh.mesh"
                materials: [
                    material_001_material,
                    material_002_material
                ]
                morphTargets: [
                    morphTarget,
                    morphTarget7
                ]
            }
            Node {
                id: empty
                position: Qt.vector3d(7.64949, 0, 0.00014019)
            }
            Model {
                id: cube_003
                position: Qt.vector3d(6.42338, 0, 2.83223)
                rotation: Qt.quaternion(0, 0, 0, 1)
                scale: Qt.vector3d(1.20059, 0.002626, 0.121481)
                source: "meshes/cube_003_mesh.mesh"
                materials: [
                    material_001_material,
                    material_002_material
                ]
                morphTargets: [
                    morphTarget11,
                    morphTarget12
                ]
            }
        }
    }


    // Animations:

//}
