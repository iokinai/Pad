import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

SettingsBasicSection {
    id: root

    required property var languageController

    title: qsTr("LANGUAGE", "Language section title")

    RowLayout {
        SectionTitleWithDescription {
            title: qsTr("Interface Language", "Interface language title")
            description: qsTr("Choose the display language", "Interface language description")
        }

        Item {
            Layout.fillWidth: true
        }

        Select {
            id: select
            model: root.languageController.languages
            textRole: "name"
            valueRole: "language"
            currentIndex: select.count > 0
                        ? select.indexOfValue(root.languageController.currentLanguage)
                        : -1

            onActivated: {
                if (currentValue) {
                    root.languageController.setLanguage(currentValue)
                }
            }
        }
    }
}
