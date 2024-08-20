#ifndef PDFCREATOR_H
#define PDFCREATOR_H

#include <QObject>
#include <QMap>

#include <QLocale>

#include "utils.h"
//#include "subject.h"

#include "databaseaccess.h"
#include "student.h"
#include "trimesteravg.h"
#include "studentgrade.h"
#include "finalavg.h"

#include "databaseaccess.h"

#include "subject.h"

class PdfCreator : public QObject
{
    Q_OBJECT
public:

    explicit PdfCreator(QObject *parent = nullptr);

public slots:
    void createTranscript(int classID, int trimester, QString out , const QString &schoolYear);
    void createFinalTranscipt(int classID, QString out , const QString &schoolYear);
    void createTotalisationPDF(int classID,
                               int trimester,
                               QString out,
                               const QString &schoolYear,
                               Order by,
                               FilterBy filter);
    void  createFinalTotalisationPDF(int classID,
                                     QString out ,
                                     const QString &schoolYear,
                                     Order order,
                                     FilterBy by);
private:
    QLocale locale;
    QString filePath;
    QMap<QString, QString> schoolInfo;
    DatabaseAccess *dbAccess = nullptr;




    QString appreciation(double grade20);

    double classAverage(const std::vector<TrimesterAVG> &avgs);

    QString writeHtml(const QString &html);

    QString transcriptHeader = R"(
            <table>
                <tr>

                    <td colspan="3" align="center">Bulletin de Note Trimestre %1</td>

                </tr>
                <tr>
                    <td>%2</td>
                    <td></td>
                    <td align="right">Année scolaire: %3</td>
                </tr>
                <tr>
                    <td>%4</td>
                </tr>
                <tr>
                    <td>Numéro : %5</td>
                </tr>
                <tr>
                    <td colspan="3">Nom et Prénom: %6</td>
                </tr>
                <tr>
                    <td>Classe: %7</td>
                </tr>
                <tr>
                    <td>Matricule: %8</td>
                    <td colspan="2">Situation: %9</td>
                </tr>
            </table>
    )";

    QString tableHeader = R"(
        <tr>
            <td style="border: 1px solid black">Matière</td>
            <td style="border: 1px solid black">Note/20</td>
            <td style="border: 1px solid black">Coef</td>
            <td style="border: 1px solid black">Node def</td>
            <td style="border: 1px solid black">Appréciation</td>
            <td style="border: 1px solid black">Emargement</td>
        </tr>
    )";

    QString subjectRows = R"(
        <tr>
            <td style="border: 1px solid black">%1</td>
            <td style="border: 1px solid black">%2</td>
            <td style="border: 1px solid black">%3</td>
            <td style="border: 1px solid black">%4</td>
            <td style="border: 1px solid black">%5</td>
            <td style="border: 1px solid black"></td>
        </tr>
    )";

    QString tootal_rank_part = R"(
        <tr>
            <td style="border: 1px solid black;">Total</td>
            <td style="border: 1px solid black;">%1</td>
            <td style="border: 1px solid black;">%2</td>
            <td style="border: 1px solid black;">%3</td>
            <td style="border: 1px solid black;"> </td>
            <td style="border: 1px solid black;"> </td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">Moyenne</td>
            <td style="border: 1px solid black;">%4</td>
            <td rowspan="2" colspan="4" align="center" style="border: 1px solid black;">Moyenne de la classe: %5</td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">Rang</td>
            <td style="border: 1px solid black;">%6/%7</td>
        </tr>
    )";

    QString final_total_rank_part = R"(
        <tr>
            <td style="border: 1px solid black;">Total</td>
            <td style="border: 1px solid black;">%1</td>
            <td style="border: 1px solid black;">%2</td>
            <td style="border: 1px solid black;">%3</td>
            <td style="border: 1px solid black;"> </td>
            <td style="border: 1px solid black;"> </td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">Moyenne</td>
            <td style="border: 1px solid black;">%4</td>
            <td colspan="4" align="center" style="border: 1px solid black;">Moyenne de la classe: %5</td>

        </tr>

        <tr>
            <td style="border: 1px solid black;">Rang</td>
            <td style="border: 1px solid black;">%6/%7</td>
            <td colspan="4" align="center" style="border: 1px solid black;">Rang final: %8/%9</td>
        </tr>
    )";

    QString final_footer = R"(
        <table>
            <tr>
                <td colspan='3'> Moyenne Trim 1: %1 Moyenne Trim 2: %2 Moyenne G : %3</td>
            </tr>
            <tr>

                <td colspan='3'>OBSERVATION GENERALE OU DECISION DU CONSEIL DE CLASSE</td>

            </tr>
            <tr>
                <td colspan='3'>Admis en classe supérieure / Redouble sa classe</td>
            </tr>
            <tr>
                <td>Remise à sa famille</td>
                <td colspan='2'>Motif: </td>
            </tr>
            <tr>
                <td>Parent</td>
                <td align="center" colspan='2'>%4 ,le %5<br>Proviseur,</td>

            </tr>
            <tr>
                <td style="height: 40px;"></td>
            </tr>
            <tr>
                <td> </td>
                <td align="right" colspan='2' align='center'>%6</td>
            </tr>
        </table>
    )";

    QString footer = R"(
    <table>
        <tr>

            <td colspan="3">OBSERVATION GENERALE OU DECISION DU CONSEIL DE CLASSE</td>

        </tr>
        <tr>
            <td colspan="3">Très-Bien / Bien / Assez-Bien / Moyen / Insuffisant /Très-Bien</td>
        </tr>
        <tr>
            <td>Parent</td>
            <td align="right">%1 ,le %2<br>Proviseur,</td>

        </tr>
        <tr>
            <td style="height: 40px;"></td>
        </tr>
        <tr>
             <td> </td>
            <td colspan="2" align="center">%3</td>
        </tr>
    </table>
    )";

    QString totalisation_header = R"(
        <table style='width: 800%;'>
            <tr>
                <td colspan='4' style='text-align: center;'>Totalisation de Note Trimestre %1</td>
            </tr>
            <tr>
                <td style='text-align: left;'>%2</td>
                <td style='color: white;'>text<td>
                <td style='color: white;'>text<td>
                <td style='text-align: right;'>Année Scolaire: %3</td>
            </tr>
            <tr>
                <td style='text-align: left;'>%4</td>
                <td style='color: white;'>text<td>
                <td style='color: white;'>text<td>
                <td style='text-align: right;'>Classe: %5</td>
            </tr>
        </table>
    )";
signals:
    void pdfCreated();
    void totalisationPDFCreated(const QString &filePath);
    void finalTotalisationExcelCreated(const QString &filePath);

};

#endif // PDFCREATOR_H
