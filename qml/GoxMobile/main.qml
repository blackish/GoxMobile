import QtQuick 1.1
import com.nokia.meego 1.0
import btctrader 1.0

PageStackWindow {
    id: appWindow

    initialPage: tradeList

    Page{
        tools: commonTools
        id: mainPage
        GraphicWidgetDeclarative {
            id: graphicWidget
            anchors.fill: parent
        }
        anchors.fill: parent
    }
    Page {
        tools: commonTools
        id: tradeDepth
        OrdersWidgetDeclarative {
            id: ordersWidget
            anchors.fill: parent
        }
        anchors.fill: parent
    }

    Page {
        tools: commonTools
        id: tradeList
        TradeListDeclarative {
            id: tradeListWidget
            anchors.fill: parent
            width: 500
            height: 500
        }
        anchors.fill: parent
    }

    BTCTrader {
        id: btctrader
        graphicWidget: graphicWidget
        ordersWidget: ordersWidget
        tradeList: tradeListWidget
    }

    TradeHistory {
        id: tradeHistory
    }
    MakeTrade {
        id: makeTrade
    }

    ToolBarLayout {
        id: commonTools
        visible: true
        ToolIcon { iconId: "toolbar-gallery"; onClicked: { myMenu.close(); if ( pageStack.currentPage != mainPage ) {pageStack.replace(mainPage);}; } }
        ToolIcon { iconId: "toolbar-callhistory"; onClicked: { myMenu.close(); if ( pageStack.currentPage != tradeDepth ) {pageStack.replace(tradeDepth); }; }  }
        ToolIcon { iconId: "toolbar-content-ovi-music"; onClicked: { myMenu.close(); if ( pageStack.currentPage != tradeHistory ) {pageStack.replace(tradeHistory); }; } }
        ToolIcon { iconId: "toolbar-list"; onClicked: { myMenu.close(); if ( pageStack.currentPage != tradeList ) {pageStack.replace(tradeList); }; } }
        ToolIcon { iconId: "toolbar-tools"; onClicked: { myMenu.close(); if ( pageStack.currentPage != makeTrade ) {pageStack.replace(makeTrade); }; } }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status === DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem { text: qsTr("Sample menu item") }
        }
    }
}
