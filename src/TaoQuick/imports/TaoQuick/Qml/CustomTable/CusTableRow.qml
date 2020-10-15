import QtQuick 2.9
import QtQuick.Controls 2.2
import "."
import ".."
import "../.."

Rectangle {
    id: deviceRow
    height: visible ? CusConfig.fixedHeight : 0
    visible: dataObj["isVisible"]

    property bool isSelected: dataObj["isSelected"]
    property bool isChecked: dataObj["isChecked"]
    property bool isAlternate: dataObj["isAlternate"]
    onIsCheckedChanged: {
        checkBox.notify = false
        checkBox.checked = isChecked
        checkBox.notify = true
    }
    property var dataObj
    property var widthList
    property var xList
    property var roles
    property color textColor: CusConfig.textColor
    property bool showOnlineState: true

    signal checkedChanged(bool checked)
    color: isSelected ? CusConfig.controlColor_pressed : ( isAlternate ? CusConfig.controlColor : CusConfig.alterColor )
    Item {
        id: checkBoxItem
        width: widthList[0]
        height: parent.height

        CusCheckBox {
            id: checkBox
            height: CusConfig.fixedHeight
            width: height
            anchors.centerIn: parent
            property bool notify: true
            onCheckedChanged: {
                if(notify) {
                    deviceRow.checkedChanged(checked)
                }
            }
        }
        CusImage {
            visible: showOnlineState
            readonly property string cameraOnlineImg: CusConfig.imagePathPrefix
                                                      + "Icon_Camera_Online.png"
            readonly property string cameraOfflineImg: CusConfig.imagePathPrefix
                                                       + "Icon_Camera_Offline.png"
            source: dataObj["online"] ? cameraOnlineImg : cameraOfflineImg
            width: 16
            height: 16
            anchors {
                verticalCenter: parent.verticalCenter
                left: checkBox.right
                leftMargin: -10
            }
        }
    }
    Repeater {
        model: roles
        Item {
            id: columnItem
            x: xList[index + 1]
            width: widthList[index + 1]
            height: parent.height
            CusLabel {
                id: textLabel
                text: qsTr(String(dataObj[roles[index]]))
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: textColor
            }
        }
    }
}
