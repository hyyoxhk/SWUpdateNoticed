import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: homeId
    anchors.fill: parent

    function convertDoubleToInt (x) {
        return x < 0 ? Math.ceil(x) : Math.floor(x);
    }

    ProgressBar {
        id: pgbId
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 40
        value: 0
    }

    Connections {
        target: SWUpdateIpc
        function onMsgChanged() {
            pgbId.value = SWUpdateIpc.msg["cur_percent"] / 100;
            console.debug("pgbId.value: " + pgbId.value)
            console.debug("msg.cur_percent: " + SWUpdateIpc.msg["cur_percent"])
            // or
            // pgbId.value = SWUpdateIpc.msg["cur_percent"]
        }
    }
}
