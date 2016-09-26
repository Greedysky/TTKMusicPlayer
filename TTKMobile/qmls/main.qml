import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Window 2.0

ApplicationWindow {
    id: ttkMainWindow
    visible: true
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight

    property bool exitApp: false
    property real pixelDensity: 4.46
    property real multiplierHeight: ttkMainWindow.height/480
    property real multiplierWidth: ttkMainWindow.width/320

    function dpHeight(numbers) {
        return Math.round(numbers*((pixelDensity*25.4)/160)*multiplierHeight);
    }
    function dpWidth(numbers) {
        return Math.round(numbers*((pixelDensity*25.4)/160)*multiplierHeight);
    }

    TTKTheme {
        id: ttkTheme
    }

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        StackView {
            id: ttkMainStackView
            width: ttkMainWindow.width
            height: ttkMainWindow.height - ttkMusicBar.height
            focus: true
            Keys.onReleased:
                if(event.key === Qt.Key_Back && ttkMainStackView.depth > 1)
                {
                     ttkMainStackView.pop();
                     event.accepted = true;
                }
            initialItem: TTKMainPage {

            }
        }

        TTKMusicBar {
            id: ttkMusicBar
        }
    }

    TTKMessageBox {
        id: ttkMessageBox
        title: '提示'
        content: '是否继续退出'
        okText: '继续退出'
        cancelText: '放弃'
        onResult: {
            if(val === 1) {
                ttkMainWindow.exitApp = true;
                ttkMainWindow.close();
                Qt.quit();
            }
        }
    }

    onClosing: {
        if(ttkMainStackView.depth > 1) {
            ttkMainStackView.pop();
            close.accepted = false;
        }
        else {
            close.accepted = ttkMainWindow.exitApp;
            if(!close.accepted) {
                ttkMessageBox.visible = true;
            }
        }
    }
}
