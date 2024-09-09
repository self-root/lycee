#include "rwexcel.h"
#include "controller.h"
#include "databaseaccess.h"
#include "avgcalculator.h"

#include <xlnt/xlnt.hpp>

RWExcel::RWExcel(QObject *parent)
    : QObject{parent}
{

}

void RWExcel::createTotalisation(int classID,
                                 int trimester,
                                 QString out,
                                 Order by,
                                 FilterBy filter)
{
    QMap<QString, QString> schoolInfo_ = Controller::instance()->getSchoolSettings();
        std::vector<Student> students = DatabaseAccess::instance()->loadStudentsByClass(classID);
        std::vector<TrimesterAVG> trimesterAVGs = DatabaseAccess::instance()->getTrimesterAVGs(trimester, classID);
        std::vector<Subject> subjects = DatabaseAccess::instance()->getSubjectByClass(classID);
        std::vector<StudentGrade> grades;

        Klass klass = DatabaseAccess::instance()->classByID(classID);
        DatabaseAccess::instance()->studentGrades(classID, trimester, grades, students, subjects);
        xlnt::workbook wb;
        xlnt::worksheet sheet = wb.active_sheet();
        sheet.title("Totalisation");

        QStringList header;

        Utils::makeTotalisationHeader(header, subjects);

        addHeader(header, sheet);

        if (filter == FilterBy::Number)
        {
            if (by == Order::Ask)
            {
                std::sort(std::begin(students), std::end(students), [](const Student &a, const Student &b){
                    return a.number() < b.number();
                });
            }
            else
            {
                std::sort(std::begin(students), std::end(students), [](const Student &a, const Student &b){
                    return a.number() > b.number();
                });
            }

            for (std::size_t i = 0; i < students.size(); i++)
            {

                sheet.cell(xlnt::cell_reference(xlnt::column_t(1), i+2)).value(students.at(i).number());
                sheet.cell(xlnt::cell_reference(xlnt::column_t(2), i+2)).value(students.at(i).name().toStdString());
                for (std::size_t j = 0; j < subjects.size(); j++)
                {
                    GradeMetaData grade = Utils::gradeFor(students.at(i), subjects.at(j), grades);
                    if (grade.skip)
                        sheet.cell(xlnt::cell_reference(xlnt::column_t(j+3), i+2)).value("NC");
                    else
                        sheet.cell(xlnt::cell_reference(xlnt::column_t(j+3), i+2)).value(grade.grade);
                }
                TrimesterAVG trimAVG = Utils::trimAVGFor(students.at(i), trimesterAVGs);
                sheet.cell(xlnt::cell_reference(xlnt::column_t(subjects.size() + 3), i+2)).value(trimAVG.total);
                sheet.cell(xlnt::cell_reference(xlnt::column_t(subjects.size() + 4), i+2)).value(trimAVG.avg);
                sheet.cell(xlnt::cell_reference(xlnt::column_t(subjects.size() + 5), i+2)).value(trimAVG.rank);

            }

        }

        else
        {
            if (by == Order::Ask)
            {
                AVGCalculator::sortAVG(trimesterAVGs);
            }

            else
            {
                AVGCalculator::sortAVG(trimesterAVGs, false);
            }

            for (std::size_t i = 0; i < trimesterAVGs.size(); i++)
            {
                Student student = Utils::studentFor(trimesterAVGs.at(i), students);
                sheet.cell(xlnt::cell_reference(xlnt::column_t(1), i+2)).value(student.number());
                sheet.cell(xlnt::cell_reference(xlnt::column_t(2), i+2)).value(student.name().toStdString());
                for (std::size_t j = 0; j < subjects.size(); j++)
                {
                    GradeMetaData grade = Utils::gradeFor(student, subjects.at(j), grades);
                    if (grade.skip)
                       sheet.cell(xlnt::cell_reference(xlnt::column_t(j+3), i+2)).value("NC");
                    else
                        sheet.cell(xlnt::cell_reference(xlnt::column_t(j+3), i+2)).value(grade.grade);
                }
                sheet.cell(xlnt::cell_reference(xlnt::column_t(subjects.size() + 3), i+2)).value(trimesterAVGs.at(i).total);
                sheet.cell(xlnt::cell_reference(xlnt::column_t(subjects.size() + 4), i+2)).value(trimesterAVGs.at(i).avg);
                sheet.cell(xlnt::cell_reference(xlnt::column_t(subjects.size() + 5), i+2)).value(trimesterAVGs.at(i).rank);
            }
        }

        wb.save(out.toStdString());
}

void RWExcel::createFinalTotalisation(int classID, const QString out, Order order, FilterBy filter)
{
        std::vector<Student> students = DatabaseAccess::instance()->loadStudentsByClass(classID);
        std::vector<TrimesterAVG> trimesterAVGs = DatabaseAccess::instance()->getTrimesterAVGs(3, classID);
        std::vector<Subject> subjects = DatabaseAccess::instance()->getSubjectByClass(classID);
        std::vector<StudentGrade> grades;
        std::vector<FinalAVG> finals = DatabaseAccess::instance()->getFinalAVGs(classID);
        AVGCalculator::sortFinalAVG(finals);

        Klass klass = DatabaseAccess::instance()->classByID(classID);
        DatabaseAccess::instance()->studentGrades(classID, 3, grades, students, subjects);
        xlnt::workbook wb;
        xlnt::worksheet sheet = wb.active_sheet();
        sheet.title("Totalisation");

        QStringList header;

        Utils::makeFinalTotalisationHeader(header, subjects);

        addHeader(header, sheet);

        if (filter == FilterBy::Number)
        {
            if (order == Order::Ask)
            {
                std::sort(std::begin(students), std::end(students), [](const Student &a, const Student &b){
                    return a.number() < b.number();
                });
            }
            else
            {
                std::sort(std::begin(students), std::end(students), [](const Student &a, const Student &b){
                    return a.number() > b.number();
                });
            }

            for (std::size_t i = 0; i < students.size(); i++)
            {

                sheet.cell(xlnt::cell_reference(xlnt::column_t(1), i+2)).value(students.at(i).number());
                sheet.cell(xlnt::cell_reference(xlnt::column_t(2), i+2)).value(students.at(i).name().toStdString());
                for (std::size_t j = 0; j < subjects.size(); j++)
                {
                    GradeMetaData grade = Utils::gradeFor(students.at(i), subjects.at(j), grades);
                    if (grade.skip)
                        sheet.cell(xlnt::cell_reference(xlnt::column_t(j+3), i+2)).value("NC");
                    else
                        sheet.cell(xlnt::cell_reference(xlnt::column_t(j+3), i+2)).value(grade.grade);
                }
                FinalAVG final = Utils::finalAVGFor(students.at(i), finals);
                TrimesterAVG trimAVG = Utils::trimAVGFor(students.at(i), trimesterAVGs);
                sheet.cell(xlnt::cell_reference(xlnt::column_t(subjects.size() + 3), i+2)).value(trimAVG.total);
                sheet.cell(xlnt::cell_reference(xlnt::column_t(subjects.size() + 4), i+2)).value(trimAVG.avg);
                sheet.cell(xlnt::cell_reference(xlnt::column_t(subjects.size() + 5), i+2)).value(final.avg());
                sheet.cell(xlnt::cell_reference(xlnt::column_t(subjects.size() + 6), i+2)).value(final.rank());
            }

        }

        else
        {
            if (order == Order::Ask)
            {
                AVGCalculator::sortFinalAVG(finals);
            }

            else
            {
                AVGCalculator::sortFinalAVG(finals, false);
            }

            for (std::size_t i = 0; i < finals.size(); i++)
            {
                Student student = Utils::studentForFinal(finals.at(i), students);
                sheet.cell(xlnt::cell_reference(xlnt::column_t(1), i+2)).value(student.number());
                sheet.cell(xlnt::cell_reference(xlnt::column_t(2), i+2)).value(student.name().toStdString());
                for (std::size_t j = 0; j < subjects.size(); j++)
                {
                    GradeMetaData grade = Utils::gradeFor(student, subjects.at(j), grades);
                    if (grade.skip)
                       sheet.cell(xlnt::cell_reference(xlnt::column_t(j+3), i+2)).value("NC");
                    else
                        sheet.cell(xlnt::cell_reference(xlnt::column_t(j+3), i+2)).value(grade.grade);
                }
                sheet.cell(xlnt::cell_reference(xlnt::column_t(subjects.size() + 3), i+2)).value(trimesterAVGs.at(i).total);
                sheet.cell(xlnt::cell_reference(xlnt::column_t(subjects.size() + 4), i+2)).value(trimesterAVGs.at(i).avg);
                sheet.cell(xlnt::cell_reference(xlnt::column_t(subjects.size() + 5), i+2)).value(finals.at(i).avg());
                sheet.cell(xlnt::cell_reference(xlnt::column_t(subjects.size() + 6), i+2)).value(finals.at(i).rank());
            }
        }

        wb.save(out.toStdString());
}

void RWExcel::addHeader(const QStringList &header, xlnt::worksheet &sheet)
{
    for (std::size_t col = 0; col < header.size(); col++)
    {
        sheet.cell(xlnt::cell_reference(col+1, 1)).value(header.at(col).toStdString());
    }
}

template<typename T>
void RWExcel::insertColumn(const std::vector<T> &column, int colIndex, xlnt::worksheet &sheet)
{
    for (std::size_t row = 0; row < column.size(); row++)
    {
        sheet.cell(xlnt::cell_reference(colIndex, row+1)).value(column.at(row));
    }
}
