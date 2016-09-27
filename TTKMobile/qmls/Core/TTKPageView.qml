import QtQuick 2.5

ListView{
    id: ttkPageView
    width: parent.width
    height: parent.height
    model: pageModel

    default property alias content: pageModel.children

    VisualItemModel {
        id: pageModel
    }

    orientation: ListView.Horizontal
    snapMode: ListView.SnapOneItem
    highlightRangeMode: ListView.StrictlyEnforceRange
    cacheBuffer: 3
    boundsBehavior: Flickable.StopAtBounds

    TTKListViewIndicator {
        target : ttkPageView
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        z: 2
    }
}
