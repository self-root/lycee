import QtQuick 2.15
import QtQuick.Controls
import QtCharts
import QtQuick.Layouts

Item {
    Rectangle{
        id: root
        anchors.fill: parent
        color: "white"

        Text {
            id: schoolNameLabel
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter;
            text: homeController.schoolName + " " + schoolYearCombo.currentText
            font.pixelSize: 18
            font.bold: true
        }

        ComboBox{
            id: schoolYearCombo
            anchors.top: schoolNameLabel.bottom
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.topMargin: 12

            model: ListModel {
                id: listModel
                ListElement{text: "2024 - 2025"}
                ListElement{text: "2023 - 2024"}
            }
        }

        Rectangle{
            id: studentNumberRect
            anchors.top: schoolYearCombo.bottom
            anchors.left: parent.left
            anchors.topMargin: 10
            anchors.leftMargin: 10
            width: 280
            height: 100
            color: "#008000"
            radius: 8
            Row{
                spacing: 6
                anchors.centerIn: parent
                Image {
                    height: 60
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "qrc:/icons/images/students.svg"
                }
                Column{

                    spacing: 6
                    Text {
                        id: topLabel
                        text: "Nombre d'élèves"
                        color:  "white"
                        font.bold: true
                        font.pixelSize: 24
                    }

                    Text {
                        id: studentsNumberLabel
                        text: "450"
                        color:  "white"
                        font.bold: true
                        font.pixelSize: 24
                    }
                }
            }


        }

        Rectangle{
            id: boysNumberRect
            anchors.top: schoolYearCombo.bottom
            anchors.left: studentNumberRect.right
            anchors.topMargin: 10
            anchors.leftMargin: 20
            width: 180
            height: 100
            color: "#fe5f55"
            radius: 8
            Row{
                anchors.centerIn: parent
                spacing: 10
                Image {
                    source: "qrc:/icons/images/male.svg"
                    height: 60
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                }
                Column{

                    spacing: 6
                    Text {
                        id: topLabel_
                        text: "Garçon"
                        color:  "white"
                        font.bold: true
                        font.pixelSize: 24
                    }

                    Text {
                        id: boysNumberLabel
                        text: "250"
                        color:  "white"
                        font.bold: true
                        font.pixelSize: 24
                    }
                }
            }


        }

        Rectangle{
            id: girlsNumberRect
            anchors.top: schoolYearCombo.bottom
            anchors.left: boysNumberRect.right
            anchors.topMargin: 10
            anchors.leftMargin: 20
            width: 180
            height: 100
            color: "#8338ec"
            radius: 8
            Row{
                anchors.centerIn: parent
                spacing: 10
                Image {
                    source: "qrc:/icons/images/female.svg"
                    height: 60
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                }
                Column{
                    spacing: 6
                    Text {
                        id: girlLabel
                        text: "Fille"
                        color:  "white"
                        font.bold: true
                        font.pixelSize: 24
                    }

                    Text {
                        id: girlsNumberLabel
                        text: "300"
                        color:  "white"
                        font.bold: true
                        font.pixelSize: 24
                    }
                }
            }


        }



        Rectangle{
            id: charRect
            anchors.top: schoolYearCombo.bottom
            anchors.left: girlsNumberRect.right
            anchors.topMargin: 10
            anchors.leftMargin: 20
            width: 400
            height: 400
            ChartView{
                id: chart
                title: "élèves distribution par niveau"
                anchors.fill: parent
                legend.alignment: Qt.AlignRight
                antialiasing: true
                animationOptions: ChartView.AllAnimations

                PieSeries{
                    id: klassDistributionSeries

                    PieSlice{
                        label: "Second"
                        value: 230
                    }
                    PieSlice{
                        label: "Premiere"
                        value: 123
                    }
                    PieSlice{
                        label: "terminale"
                        value: 210
                    }
                }
            }
        }

        Rectangle{
            id: summaryRect
            anchors.top: boysNumberRect.bottom
            anchors.left: parent.left
            anchors.right: charRect.left
            anchors.topMargin: 80
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            height: 160

            Row{
                spacing: 8
                anchors.fill: parent

                Rectangle{
                    id: secondRect
                    width: 160
                    height: 100
                    color: "#ffbc0a"
                    radius: 8
                    Column{
                        anchors.centerIn: parent
                        spacing: 6
                        Text {
                            id: topLabelSecond
                            text: "Second"
                            color:  "white"
                            font.bold: true
                            font.pixelSize: 24
                        }

                        Text {
                            id: studentsNumberLabelSecond
                            text: "450"
                            color:  "white"
                            font.bold: true
                            font.pixelSize: 24
                        }
                    }
                }

                Rectangle{
                    id: premiereRect
                    width: 160
                    height: 100
                    color: "#0582ca"
                    radius: 8
                    Column{
                        anchors.centerIn: parent
                        spacing: 6
                        Text {
                            id: topLabelpremiere
                            text: "Premiere"
                            color:  "white"
                            font.bold: true
                            font.pixelSize: 24
                        }

                        Text {
                            id: studentsNumberLabelPremiere
                            text: "345"
                            color:  "white"
                            font.bold: true
                            font.pixelSize: 24
                        }
                    }
                }

                Rectangle{
                    id: terminalRect
                    width: 160
                    height: 100
                    color: "#1f2421"
                    radius: 8
                    Column{
                        anchors.centerIn: parent
                        spacing: 6
                        Text {
                            id: topLabelTerminal
                            text: "Terminal"
                            color:  "white"
                            font.bold: true
                            font.pixelSize: 24
                        }

                        Text {
                            id: studentsNumberLabelterminal
                            text: "200"
                            color:  "white"
                            font.bold: true
                            font.pixelSize: 24
                        }
                    }
                }
            }
        }

        Rectangle{
            id: calendarRect
            anchors.top: charRect.bottom
            anchors.left: parent.left
            anchors.leftMargin:  10
            anchors.topMargin: 10
            GridLayout {
                columns: 2
                DayOfWeekRow {
                    locale: grid.locale
                    Layout.column: 1
                    Layout.fillWidth: true
                }

                WeekNumberColumn {
                    month: grid.month
                    year: grid.year
                    locale: grid.locale
                    Layout.fillHeight: true
                }

                MonthGrid {
                    id: grid
                    month: Calendar.August
                    year: 2024
                    locale: Qt.locale("fr_FR")
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }

    }
}
