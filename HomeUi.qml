import QtQuick 2.15
import QtQuick.Controls
import QtCharts
import QtQuick.Layouts

Item {
    id: root
    function loadSeriesData(){
        homeController.distroByLevel.setSchoolYear(schoolYearCombo.currentText);
        homeController.distroByLevel.loadData();
        klassDistributionSeries.clear()
        for(var i = 0; i < homeController.distroByLevel.countData(); i++)
        {
            var label = homeController.distroByLevel.labelAt(i);
            var value = homeController.distroByLevel.valueAt(i);
            console.log(homeController.distroByLevel.labelAt(i));
            console.log(homeController.distroByLevel.valueAt(i));
            klassDistributionSeries.append(label, value);
        }
    }

    function setSerieColors(){
        klassDistributionSeries.find("Seconde").color = secondRect.color
        klassDistributionSeries.find("Première").color = premiereRect.color
        klassDistributionSeries.find("Terminale").color = terminalRect.color
    }

    function getCurrentDay(){
        var today = new Date();
        var day = today.getDate();
        return day;
    }

    function getCurrentMonth(){
        var today = new Date();
        var month = today.getMonth();
        return month;
    }

    function getCurrentYear(){
        var today = new Date();
        var year = today.getFullYear()
        return year;
    }

    function getCurrentFullMonth()
    {
        var today = new Date();
        var month = Qt.formatDate(today, "MMMM")
        return month
    }

    Component.onCompleted:{
        homeController.currentSchoolYear = schoolYearCombo.currentText
        loadSeriesData()
        setSerieColors()
        getCurrentDay()
    }

    Rectangle{
        id: rect_
        anchors.fill: parent
        color: "white"

        Text {
            id: schoolNameLabel
            anchors.horizontalCenter: rect_.horizontalCenter
            anchors.top: rect_.top
            anchors.topMargin: 10
            text: homeController.schoolName + " " + homeController.currentSchoolYear
            font.pixelSize: 18
            font.bold: true
        }

        ComboBox{
            id: schoolYearCombo
            anchors.top: schoolNameLabel.bottom
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.topMargin: 12

            model: homeController.schoolYearModel
            onActivated: {
                homeController.currentSchoolYear = schoolYearCombo.currentText
                loadSeriesData()
                setSerieColors()
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
                        text: homeController.students
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
                        text: homeController.males
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
                        text: homeController.females
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
            //anchors.right: parent.right
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
                        label: "Seconde"
                        value: 234
                        color: secondRect.color
                    }
                    PieSlice{
                        label: "Première"
                        value: 33
                        color: premiereRect.color
                    }
                    PieSlice{
                        label: "Terminale"
                        value: 4
                        color: terminalRect.color
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
                            text: "Seconde"
                            color:  "white"
                            font.bold: true
                            font.pixelSize: 24
                        }

                        Text {
                            id: studentsNumberLabelSecond
                            text: homeController.distroByLevel.secondCount
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
                            text: "Première"
                            color:  "white"
                            font.bold: true
                            font.pixelSize: 24
                        }

                        Text {
                            id: studentsNumberLabelPremiere
                            text: homeController.distroByLevel.premiereCount
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
                            text: "Terminale"
                            color:  "white"
                            font.bold: true
                            font.pixelSize: 24
                        }

                        Text {
                            id: studentsNumberLabelterminal
                            text: homeController.distroByLevel.terminalCount
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
            width: 250
            height: 200
            Rectangle{
                id: monthName
                width: parent.width
                height: 30
                color: "#fe5f55"
                anchors.top: parent.top
                anchors.topMargin: 3
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 6
                Text {
                    anchors.verticalCenter: monthName.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: getCurrentFullMonth()
                    font.bold: true
                    color: "white"
                }
            }


            GridLayout {
                anchors.top: monthName.bottom
                anchors.topMargin: 6
                anchors.horizontalCenter: parent.horizontalCenter
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
                    month: getCurrentMonth()
                    year: getCurrentYear()
                    locale: Qt.locale("fr_FR")
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    delegate: Text {
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        //opacity: model.month === control.month ? 1 : 0
                        text: model.day
                        //font: control.font
                        color: getCurrentDay() === model.day? "red" : "black"
                        font.bold: getCurrentDay() === model.day? true : false

                        required property var model
                    }

                }
            }
        }

    }


}
