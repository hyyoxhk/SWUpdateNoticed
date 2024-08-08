import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    id: mainWindowId
    width: swidth
    height: sheight
    maximumWidth: swidth
    maximumHeight: sheight
    minimumWidth: swidth
    minimumHeight: sheight
    //color: "transparent"

    flags: Qt.FramelessWindowHint
    //{{ Displayed in the taskbar when the window is at a minimumsize state
           | Qt.WindowSystemMenuHint
           | Qt.WindowMinimizeButtonHint
           | Qt.Window

    Rectangle {
        id: workAreaRectId
        width: parent.width
        height: parent.height

        Loader {
            id: workAreaRectLoaderId
            anchors.fill: parent
            source: "qrc:/qml/SWUpdate.qml"
        }
    }

    Connections {
        target: SWUpdateIpc
        function onStart() {
            mainWindowId.visible = true
            console.debug("qml onStart");
        }

        function onStop() {
            mainWindowId.visible = false
            console.debug("qml onStop");
        }
    }
}
