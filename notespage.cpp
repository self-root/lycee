#include "notespage.h"
#include "ui_notespage.h"
#include "controller.h"
#include "databaseaccess.h"
#include "clipboardparser.h"
#include <QClipboard>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

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
    updateActionState();
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
    ui->classCombo->addItem("--", -1);
    for (const Klass &klass : Controller::instance()->klasses)
    {
        ui->classCombo->addItem(klass.className(), klass.classId());
    }
    on_classCombo_activated(0);
}


void NotesPage::on_classCombo_activated(int index)
{
    currentClassID = ui->classCombo->itemData(index).toInt();
    currentStudent = Student();
    ui->studentNameEdit->clear();
    clearGradeForm();
    gradesView->loadStudentGrades(currentClassID, ui->trimestreCombo->currentIndex() + 1);
    setUpsubjectLists(currentClassID);
    if (ui->trimestreCombo->currentIndex() == 2 && hasValidContext())
    {
        showMoyenneGeneralTab();
    }
    else if (!hasValidContext())
    {
        hideMoyenneGeneralTab();
    }
    updateActionState();
}


void NotesPage::on_trimestreCombo_activated(int index)
{
    on_classCombo_activated(ui->classCombo->currentIndex());
    if (index == 2)
    {
        showMoyenneGeneralTab();
    }
    else
    {
        hideMoyenneGeneralTab();
    }
}

void NotesPage::setUpsubjectLists(int klassID)
{
    std::vector<Subject> subjects = DatabaseAccess::instance()->getSubjectByClass(klassID);
    ui->subjectCombo->clear();
    for (const auto &s : subjects)
    {
        ui->subjectCombo->addItem(s.subjectName(), s.subjectId());
    }
    currentClassID = klassID;
    updateActionState();
}

void NotesPage::onStudentSelected(const Student &student)
{
    ui->studentNameEdit->setText(student.name());
    currentStudent = student;
    setGradeData();
}

void NotesPage::setGradeData()
{
    clearGradeForm();
    const Subject subject = gradesView->model->subjectFromID(ui->subjectCombo->currentData().toInt());
    GradeMetaData gradeMeta = gradesView->model->gradeFor(currentStudent, subject);
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
    if (!hasValidContext())
    {
        QMessageBox::warning(this, "Classe requise", "Selectionnez une annee scolaire et une classe.");
        return;
    }
    qDebug() << "create PDF";
    int trimester = ui->trimestreCombo->currentIndex() + 1;
    QString schoolYear = ui->schoolYearCombo->currentText();
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilter("PDF (*.pdf)");
    fileDialog.setDefaultSuffix("pdf");

    if (fileDialog.exec())
    {
        auto files = fileDialog.selectedFiles();
        if (files.length() > 0)
        {

            Controller::instance()->createTotalisationPDF(
                currentClassID,
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
    if (!hasValidContext())
    {
        QMessageBox::warning(this, "Classe requise", "Selectionnez une annee scolaire et une classe.");
        return;
    }
    qDebug() << "Create excel";
    int trimester = ui->trimestreCombo->currentIndex() + 1;
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilter("Excel (*.xlsx)");
    fileDialog.setDefaultSuffix("xlsx");

    if (fileDialog.exec())
    {
        auto files = fileDialog.selectedFiles();
        if (files.length() > 0)
        {

            Controller::instance()->createTotalisationExcel(
                currentClassID,
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

bool NotesPage::hasValidContext() const
{
    return currentClassID > 0 && ui->schoolYearCombo->currentIndex() > 0;
}

void NotesPage::updateActionState()
{
    if (!pasteAction)
        return;
    const bool enabled = hasValidContext();
    pasteAction->setEnabled(enabled);
    computeAction->setEnabled(enabled);
    createTranscriptAction->setEnabled(enabled);
    gradesToPDFAction->setEnabled(enabled);
    gradesToExcelAction->setEnabled(enabled);
    ficheDeNoteAction->setEnabled(enabled);
    generalComputeAction->setEnabled(enabled && ui->trimestreCombo->currentIndex() == 2);
}

void NotesPage::showMoyenneGeneralTab()
{
    if (generalAVGView == nullptr)
    {
        generalAVGView = new FinalAVGView;
        tabWidget->addTab(generalAVGView, "Moyenne Générale");
    }

    generalComputeAction->setEnabled(hasValidContext());
    generalAVGView->model->loadData(currentClassID);
}

void NotesPage::hideMoyenneGeneralTab()
{
    generalComputeAction->setEnabled(false);
    if (generalAVGView != nullptr)
    {
        delete generalAVGView;
        generalAVGView = nullptr;
    }
}


void NotesPage::on_subjectCombo_activated(int index)
{
    Q_UNUSED(index)
    setGradeData();
}

void NotesPage::on_saveGradeBtn_clicked()
{
    const Subject subject = gradesView->model->subjectFromID(ui->subjectCombo->currentData().toInt());
    if (!hasValidContext() || currentStudent.id() < 1 || subject.subjectId() < 1)
    {
        QMessageBox::warning(this, "Selection requise", "Selectionnez une classe, un eleve et une matiere.");
        return;
    }
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
        grade.subjectName = subject.subjectName();
        gradesView->model->addGrade(grade, currentStudent, subject);
        Controller::instance()->checkDbError();
        if (grade.id > 0)
            currentGrade = grade;
    }
}

void NotesPage::onPaste(bool _)
{
    Q_UNUSED(_);
    if (!hasValidContext())
    {
        QMessageBox::warning(this, "Classe requise", "Selectionnez une classe avant d'importer des notes.");
        return;
    }
    QClipboard *clipboard = QApplication::clipboard();
    const GradeClipboardParseResult result = ClipBoardParser::parseGradeClipboard(clipboard->text());
    if (result.grades.empty())
    {
        QMessageBox::warning(this, "Import impossible", result.errors.join("\n"));
        return;
    }
    QString preview = QString("Classe: %1\nMatiere: %2\nLignes valides: %3\nLignes rejetees: %4")
                          .arg(DatabaseAccess::instance()->classByID(currentClassID).className(), result.subjectName)
                          .arg(result.grades.size())
                          .arg(result.errors.size());
    if (!result.errors.isEmpty())
        preview += "\n\n" + result.errors.mid(0, 5).join("\n");
    int ret = QMessageBox::question(this, "Apercu de l'import", preview + "\n\nContinuer ?",
                                    QMessageBox::Ok | QMessageBox::Cancel);
    switch (ret)
    {
    case QMessageBox::Ok:{
        QStringList rejected = result.errors;
        const int imported = gradesView->model->importGrades(result, rejected);
        Controller::instance()->checkDbError();
        QString summary = QString("%1 note(s) importee(s).\n%2 ligne(s) rejetee(s).")
                              .arg(imported).arg(rejected.size());
        if (!rejected.isEmpty())
            summary += "\n\n" + rejected.mid(0, 8).join("\n");
        QMessageBox::information(this, "Import termine", summary);
        break;
    }
    case QMessageBox::Cancel:
    default:
        break;
    }

}

void NotesPage::onCompute(bool _)
{
    Q_UNUSED(_);
    if (hasValidContext())
        gradesView->model->computeAVG();
}

void NotesPage::onComputeFinalAVG(bool _)
{
    Q_UNUSED(_)
    qDebug() << "Compute finalAVG";
    if (hasValidContext() && generalAVGView)
        generalAVGView->model->computeFinalAVG();
}

void NotesPage::onCreateTranscript(bool _)
{
    Q_UNUSED(_)
    qDebug() << __FUNCTION__;
    //int currentTrimester = ui->trimestreCombo->currentIndex() + 1;

    if (!hasValidContext())
        return;
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilter("PDF (*.pdf)");
    fileDialog.setDefaultSuffix("pdf");

    if (fileDialog.exec())
    {
        auto files = fileDialog.selectedFiles();
        if (files.length() > 0)
        {
            Controller::instance()->createTranscript(
                currentClassID,
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

    if (!hasValidContext())
        return;
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilter("PDF (*.pdf)");
    fileDialog.setDefaultSuffix("pdf");

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
