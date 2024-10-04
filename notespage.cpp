#include "notespage.h"
#include "ui_notespage.h"
#include "controller.h"
#include "databaseaccess.h"
#include "clipboardparser.h"
#include <QClipboard>
#include <QFileDialog>

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
    totalisationForm = new TotalisationCreateForm;
    QObject::connect(totalisationForm, &TotalisationCreateForm::totalisationPdf, this, &NotesPage::onToPDF);
    QObject::connect(totalisationForm, &TotalisationCreateForm::totalisationExcel, this, &NotesPage::onToExcel);

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
    on_classCombo_activated(0);
    if (index == 2)
    {
        generalComputeAction->setEnabled(true);
        generalAVGView = new FinalAVGView;
        tabWidget->addTab(generalAVGView, "Moyenne Générale");
        generalAVGView->model->loadData(Controller::instance()->klassByName(ui->classCombo->currentText()).classId());
    }
    else
    {
        generalComputeAction->setEnabled(false);
        if (generalAVGView != nullptr)
        {
            delete generalAVGView;
            generalAVGView = nullptr;
        }
    }
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
    ui->schoolYearCombo->clear();
    ui->schoolYearCombo->addItem("--");
    Controller::instance()->getSchoolyears();
    for (const QString &year: Controller::instance()->schoolYears)
        ui->schoolYearCombo->addItem(year);
}

void NotesPage::onToPDF(Order order, FilterBy by)
{
    qDebug() << "create PDF";
    Klass klass = Controller::instance()->klassByName(ui->classCombo->currentText());
    int trimester = ui->trimestreCombo->currentIndex() + 1;
    QString schoolYear = ui->schoolYearCombo->currentText();
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilter("PDF (*.pdf)");

    if (fileDialog.exec())
    {
        auto files = fileDialog.selectedFiles();
        if (files.length() > 0)
        {

            Controller::instance()->createTotalisationPDF(
                klass.classId(),
                trimester,
                files.at(0),
                schoolYear,
                order,
                by
            );
        }
    }
}

void NotesPage::onToExcel(Order order, FilterBy by)
{
    qDebug() << "Create excel";
    Klass klass = Controller::instance()->klassByName(ui->classCombo->currentText());
    int trimester = ui->trimestreCombo->currentIndex() + 1;
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilter("Excel (*.xlsx)");

    if (fileDialog.exec())
    {
        auto files = fileDialog.selectedFiles();
        if (files.length() > 0)
        {

            Controller::instance()->createTotalisationExcel(
                klass.classId(),
                trimester,
                files.at(0),
                order,
                by
            );
        }
    }
}

void NotesPage::clearGradeForm()
{
    ui->gradeSpin->clear();
    ui->skipSubjectCheck->setChecked(false);
}

void NotesPage::setupToolBar()
{
    toolbar = new QToolBar;
    //toolbar->setIconSize(QSize(32, 32));
    toolbar->setMovable(true);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    pasteAction = new QAction(QIcon(":/icons/images/paste.png"), "Coller");
    computeAction = new QAction(QIcon(":/icons/images/calculator.png"), "Moyenne");
    generalComputeAction = new QAction(QIcon(":/icons/images/compute1.png"), "Moyenne générale");
    createTranscriptAction = new QAction(QIcon(":/icons/images/transcript.png"), "Créer Bulletin");
    gradesToPDFAction = new QAction(QIcon(":/icons/images/topdf.png"), "Totalisation PDF");
    gradesToExcelAction = new QAction(QIcon(":/icons/images/toexcel.png"), "Totalisation Excel");
    ficheDeNoteAction = new QAction(QIcon(":/icons/images/spreadsheet.png"), "Fiche de notes");
    generalComputeAction->setEnabled(false);
    toolbar->addAction(pasteAction);
    toolbar->addAction(computeAction);
    toolbar->addAction(generalComputeAction);
    toolbar->addAction(createTranscriptAction);
    toolbar->addAction(gradesToPDFAction);
    toolbar->addAction(gradesToExcelAction);
    toolbar->addAction(ficheDeNoteAction);
    ui->mainLayout->insertWidget(0, toolbar);

    QObject::connect(pasteAction, &QAction::triggered, this, &NotesPage::onPaste);
    QObject::connect(computeAction, &QAction::triggered, this, &NotesPage::onCompute);
    QObject::connect(generalComputeAction, &QAction::triggered, this, &NotesPage::onComputeFinalAVG);
    QObject::connect(createTranscriptAction, &QAction::triggered, this, &NotesPage::onCreateTranscript);
    QObject::connect(gradesToPDFAction, &QAction::triggered, this, &NotesPage::onSaveGradesPDFAction);
    QObject::connect(gradesToExcelAction, &QAction::triggered, this, &NotesPage::onSaveGradesExcelAction);
    QObject::connect(ficheDeNoteAction, &QAction::triggered, this, &NotesPage::onFicheDeNote);
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

void NotesPage::onComputeFinalAVG(bool _)
{
    Q_UNUSED(_)
    qDebug() << "Compute finalAVG";
    generalAVGView->model->computeFinalAVG();
}

void NotesPage::onCreateTranscript(bool _)
{
    Q_UNUSED(_)
    qDebug() << __FUNCTION__;
    //int currentTrimester = ui->trimestreCombo->currentIndex() + 1;

    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilter("PDF (*.pdf)");

    if (fileDialog.exec())
    {
        auto files = fileDialog.selectedFiles();
        if (files.length() > 0)
        {
            Controller::instance()->createTranscript(
                Controller::instance()->klassByName(ui->classCombo->currentText()).classId(),
                ui->trimestreCombo->currentIndex() + 1,
                files.at(0),
                ui->schoolYearCombo->currentText()
            );
        }
    }
}

void NotesPage::onSaveGradesPDFAction(bool _)
{
    Q_UNUSED(_)
    totalisationForm->open(Target::PDF);
}

void NotesPage::onSaveGradesExcelAction(bool _)
{
    Q_UNUSED(_)
    totalisationForm->open(Target::Excel);
}

void NotesPage::onFicheDeNote(bool _)
{
    Q_UNUSED(_)

    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilter("PDF (*.pdf)");

    if (fileDialog.exec())
    {
        auto files = fileDialog.selectedFiles();
        if (files.length() > 0)
        {
            Controller::instance()->createFicheDeNote(gradesView->model->getCurrentClass(),
                                                      ui->schoolYearCombo->currentText(),
                                                      ui->trimestreCombo->currentIndex() + 1,
                                                      files.at(0));
        }
    }
}

