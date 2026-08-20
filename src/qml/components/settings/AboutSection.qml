import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

SettingsBasicSection {
    title: qsTr("ABOUT", "About section title")

    ColumnLayout {
        spacing: 10
        Repeater {
            model: [
                [qsTr("Application", "Application name text"), superApp.applicationName],
                [qsTr("Version", "Application version text"), superApp.applicationVersion],
                [qsTr("GitHub", "Application github url text"), superApp.applicationGitHub],
            ]

            RowLayout {
                Text {
                    text: modelData[0]
                    font.pixelSize: 13
                    color: superApp.theme.textMuted
                }

                Item {
                    Layout.fillWidth: true
                }

                Text {
                    text: modelData[1]
                    font.pixelSize: 13
                    color: superApp.theme.text
                    font.weight: Font.Bold
                }
            }
        }
    }
}
