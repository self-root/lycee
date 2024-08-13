#include "notespage.h"
#include "ui_notespage.h"
#include "controller.h"
#include "databaseaccess.h"
#include "clipboardparser.h"
#include <QClipboard>

NotesPage::NotesPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NotesPage)
{
    ui->setupUi(this);
    tabWidget = new QTabWidget;
    gradesView = new GradesTableView;
    tabWidget->addTab(gradesView, "Notes");
    ui->mainLayout->addWidget(tabWidget);
    loadSchoolYear();

    setupToolBar();

    QObject::connect(gradesView, &GradesTableView::studentSelected, this, &NotesPage::onStudentSelected);
}

NotesPage::~NotesPage()
{
    delete ui;
}

void NotesPage::on_schoolYearCombo_activated(int index)
{
    Q_UNUSED(index)
    QString schoolYear = ui->schoolYearCombo->currentText();
    Controller::instance()->getKlassList(schoolYear);
    ui->classCombo->clear();
    ui->classCombo->addItem("--");
    for (const Klass &klass : Controller::instance()->klasses)
    {
        ui->classCombo->addItem(klass.className());
    }
}


void NotesPage::on_classCombo_activated(int index)
{
    Q_UNUSED(index);
    Klass klass = Controller::instance()->klassByName(ui->classCombo->currentText());
    gradesView->loadStudentGrades(klass.classId(), ui->trimestreCombo->currentIndex() + 1);
    setUpsubjectLists(klass.classId());
}


void NotesPage::on_trimestreCombo_activated(int index)
{
    Q_UNUSED(index)
    on_classCombo_activated(0);
}

void NotesPage::setUpsubjectLists(int klassID)
{
    std::vector<Subject> subjects = DatabaseAccess::instance()->getSubjectByClass(klassID);
    ui->subjectCombo->clear();
    for (const auto &s : subjects)
    {
        ui->subjectCombo->addItem(s.subjectName());
    }
}

void NotesPage::onStudentSelected(const Student &student)
{
    ui->studentNameEdit->setText(student.name());
    setGradeData();
}

void NotesPage::setGradeData()
{
    clearGradeForm();
    GradeMetaData gradeMeta = gradesView->model->gradeFor(ui->studentNameEdit->text(), ui->subjectCombo->currentText());
    if (gradeMeta.id > 0)
    {
        ui->gradeSpin->setValue(gradeMeta.grade);
        ui->skipSubjectCheck->setChecked(gradeMeta.skip);
    }
    currentGrade = gradeMeta;
}

void NotesPage::loadSchoolYear()
{
    for (const QString &year: Controller::instance()->schoolYears)
        ui->schoolYearCombo->addItem(year);
}

void NotesPage::clearGradeForm()
{
    ui->gradeSpin->clear();
    ui->skipSubjectCheck->setDisabled(true);
}

void NotesPage::setupToolBar()
{
    toolbar = new QToolBar;
    toolbar->setIconSize(QSize(32, 32));
    toolbar->setMovable(true);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    pasteAction = new QAction(QIcon(":/icons/images/paste.png"), "Coller");
    computeAction = new QAction(QIcon(":/icons/images/calculator.png"), "Calculler Moyenne");
    toolbar->addAction(pasteAction);
    toolbar->addAction(computeAction);
    ui->mainLayout->insertWidget(0, toolbar);

    QObject::connect(pasteAction, &QAction::triggered, this, &NotesPage::onPaste);
    QObject::connect(computeAction, &QAction::triggered, this, &NotesPage::onCompute);
}


void NotesPage::on_subjectCombo_activated(int index)
{
    Q_UNUSED(index)
    setGradeData();
}

void NotesPage::on_saveGradeBtn_clicked()
{
    if (currentGrade.id > 0)
    {

        GradeMetaData grade = currentGrade;
        grade.grade = ui->gradeSpin->value();
        grade.skip = ui->skipSubjectCheck->isChecked();
        if (grade != currentGrade)
        {
            qDebug() << "Update grade";
            gradesView->model->updateGrade(grade);
            Controller::instance()->checkDbError();
            currentGrade = grade;
        }
    }

    else
    {
        qDebug() << "Add new grade";
        GradeMetaData grade = currentGrade;
        grade.grade = ui->gradeSpin->value();
        grade.skip = ui->skipSubjectCheck->isChecked();
        grade.subjectName = ui->subjectCombo->currentText();
        gradesView->model->addGrade(grade, ui->studentNameEdit->text());
        Controller::instance()->checkDbError();
        if (grade.id > 0)
            currentGrade = grade;
    }
}

void NotesPage::onPaste(bool _)
{
    Q_UNUSED(_);
    QClipboard *clipboard = QApplication::clipboard();
    //qDebug() << clipboard->text();
    std::vector<ClipboardGrade> grades = ClipBoardParser::parseGradeClipboard(clipboard->text());
    if (!grades.empty())
    {
        QString subjectName = "";
        try {
            subjectName = grades.at(0).name;
        } catch (...) {
            return;
        }

        for (std::size_t i = 1; i < grades.size(); ++i) {
            GradeMetaData gradeMeta;
            gradeMeta.grade = grades.at(i).grade;
            gradeMeta.subjectName = subjectName;
            gradesView->model->studentGradeFromClipboard(gradeMeta, grades.at(i).studentName);
        }
        Controller::instance()->checkDbError();
    }
}

void NotesPage::onCompute(bool _)
{
    Q_UNUSED(_);
    gradesView->model->computeAVG();
}

