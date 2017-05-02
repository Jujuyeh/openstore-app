/*
 * Copyright (C) 2015 Michael Zanetti <michael.zanetti@ubuntu.com>
 * Copyright (C) 2017 Stefano Verzegnassi <verzegnassi.stefano@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.4
import Ubuntu.Components 1.3

ScrollView {
    id: rootItem
    anchors.fill: parent
    anchors.topMargin: parent.header ? parent.header.height : 0

    property var filterPattern: new RegExp()
    property string filterProperty
    property alias model: sortedFilteredAppModel.model

    signal appDetailsRequired(var appId)

    ListView {
        id: view
        model: SortFilterModel {
            id: sortedFilteredAppModel
            filter.pattern: rootItem.filterPattern
            filter.property: rootItem.filterProperty
        }

        // TODO: Move it in Main.qml or elsewhere
        onCountChanged: {
            if (count > 0 && root.appIdToOpen != "") {
                var index = appModel.findApp(root.appIdToOpen)
                if (index >= 0) {
                    pageStack.addPageToNextColumn(mainPage, Qt.resolvedUrl("AppDetailsPage.qml"), {app: appModel.app(index)})
                    root.appIdToOpen = "";
                }
            }
        }

        delegate: ListItem {
            height: layout.height + divider.height

            ListItemLayout {
                id: layout
                title.text: model.name
                summary.text: model.tagline

                UbuntuShape {
                    SlotsLayout.position: SlotsLayout.Leading
                    image: Image {
                        source: model.icon
                        height: parent.height
                        width: parent.width
                    }
                }
                Icon {
                    SlotsLayout.position: SlotsLayout.Trailing
                    height: units.gu(2)
                    width: height
                    implicitHeight: height
                    implicitWidth: width
                    visible: model.installed
                    name: "tick"
                    color: model.installedVersion >= model.version ? UbuntuColors.green : UbuntuColors.orange
                }

                ProgressionSlot {}
            }
            onClicked: {
                rootItem.appDetailsRequired(model.appId)
            }
        }

        Loader {
            anchors.centerIn: parent
            source: Qt.resolvedUrl("NoMatchEmptyState.qml")
            active: view.count == 0
        }
    }
}
