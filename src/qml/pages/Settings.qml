import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

ColumnLayout {
    ColumnLayout {
        Layout.topMargin: 48
        Layout.bottomMargin: 48
        Layout.leftMargin: 40
        Layout.rightMargin: 40

        spacing: 40
        Layout.alignment: Qt.AlignHCenter
        Layout.fillWidth: true
        Layout.maximumWidth: 520

        SettingsHeader {
        }

        ColumnLayout {
            spacing: 32

            AppearanceSection {
                Layout.fillWidth: true
            }

            LanguageSection {
                Layout.fillWidth: true
                languageController: superApp.languageController
            }

            AboutSection {
                Layout.fillWidth: true
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
}
