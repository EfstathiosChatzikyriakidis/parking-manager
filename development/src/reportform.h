/* header defining the interface of the source. */
#ifndef REPORTFORM_H
#define REPORTFORM_H

/* include some QT libraries. */
#include <QDialog>

/* use these classes. */
class QDialogButtonBox;
class QSqlTableModel;
class QPushButton;
class QTableView;
class QLineEdit;
class QDateEdit;
class QLabel;

/* GUI string messages. */
static const QString repoWinTitleStr  = QObject::tr("Transactions Report");

static const QString startDateStr     = QObject::tr("Start Date");
static const QString endDateStr       = QObject::tr("End Date");
static const QString startTimeStr     = QObject::tr("Start Time");
static const QString endTimeStr       = QObject::tr("End Time");
static const QString chargeStr        = QObject::tr("Charge");

static const QString fromDateLabelStr = QObject::tr("&From Date :");
static const QString toDateLabelStr   = QObject::tr("&To Date: ");

static const QString showButtonStr    = QObject::tr("&Show");
static const QString clearButtonStr   = QObject::tr("&Clear All");
static const QString showAllButtonStr = QObject::tr("&Show All");

static const QString datesRelationStr = QObject::tr("Please check if [To Date] is bigger than [From Date].");
static const QString clearReportStr   = QObject::tr("Do you want to clear the report?");

/* class which implements the report gui form and data model. */
class ReportForm : public QDialog
{
    Q_OBJECT

    public:
        /* report index fields enumeration data type. */
        typedef enum reportField {
            Report_Id = 0,
            Report_Vehicle,
            Report_StartDate,
            Report_EndDate,
            Report_StartTime,
            Report_EndTime,
            Report_Charge,
            Report_Customer
        } reportField;

        ReportForm(QWidget *parent = 0);
        void done(const int result);

    private slots:
        void custShowReport();
        void vehiShowReport();
        void dateShowReport();
        void showAllReport();
        void clearReport();

    private:
        void createReportPanel();
        void fixReportSize();

        QSqlTableModel *tableModel;

        QWidget *reportPanel;
        QTableView *reportView;

        QLabel *custFilterLabel;
        QLabel *vehiFilterLabel;
        QLabel *fromDateFilterLabel;
        QLabel *toDateFilterLabel;

        QDateEdit *fromDateFilterEdit;
        QDateEdit *toDateFilterEdit;

        QLineEdit *custFilterEdit;
        QLineEdit *vehiFilterEdit;

        QPushButton *closeButton;
        QPushButton *dateShowButton;
        QPushButton *custShowButton;
        QPushButton *vehiShowButton;
        QPushButton *showAllButton;
        QPushButton *clearButton;

        QDialogButtonBox *buttonBox;
};

#endif // REPORTFORM_H
