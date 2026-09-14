import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts


SettingsBasicSection {
    id: root
    title: qsTr("APPEARANCE", "Settings appearance section")

    RowLayout {
        SectionTitleWithDescription {
            title: qsTr("Theme", "Appearance theme text title")
            description: qsTr("Switch between light and dark mode", "Appearance theme text description")
        }

        Item {
            Layout.fillWidth: true
        }

        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 12

            RowLayout {
                spacing: 6

                Loader {
                    sourceComponent: superApp.theme.themeTag === Theme.Dark ? iconMoon : iconSun
                }

                Text {
                    text: superApp.theme.themeTag === Theme.Dark ? qsTr("Dark", "Appearance dark theme text") : qsTr("Light", "Appearance light theme text")
                    font.pixelSize: 12
                    font.weight: Font.Bold
                    color: superApp.theme.textMuted
                }
            }

            Toggle {
                checked: superApp.theme.themeTag === Theme.Dark
                onCheckedChanged: {
                    superApp.setTheme(checked ? Theme.Dark : Theme.Light)
                }
            }

            Component {
                id: iconSun
                Sun {
                    size: 15
                    color: superApp.theme.accent
                }
            }

            Component {
                id: iconMoon
                Moon {
                    size: 15
                    color: superApp.theme.accent
                }
            }
        }
    }
}
