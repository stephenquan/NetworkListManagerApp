import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import ArcGIS.AppFramework.Labs.NetworkListManager 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Network List Manager")

    QtObject {
        id: styles
        property int textPointSize: 12
    }

    Page {
        anchors.fill: parent

        ListView {
            id: listView

            anchors.fill: parent
            anchors.margins: 10

            model: listModel

            delegate: Frame {
                width: listView.width

                Text {
                    width: parent.width

                    text: new Date(timeStamp) + ": " + connectivity + " - " + connectivityString
                    font.pointSize: styles.textPointSize
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }
            }
        }
    }

    NetworkListManager {
        id: networkListManager

        onConnectivityChanged: listModel.log( connectivity, connectivityString )

        Component.onCompleted: listModel.log( connectivity, connectivityString )
    }

    ListModel {
        id: listModel

        function log(connectivity, connectivityString) {
            append( {
                       timeStamp: Date.now(),
                       connectivity: connectivity,
                       connectivityString: connectivityString
                   } );

        }
    }
}
