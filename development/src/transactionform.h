/* header defining the interface of the source. */
#ifndef TRANSACTIONFORM_H
#define TRANSACTIONFORM_H

/* include some QT libraries. */
#include <QDialog>

/* include headers defining the interface of the sources. */
#include "emptydateedit.h"
#include "emptytimeedit.h"
#include "globaldeclarations.h"
#include "appsettings.h"

/* use these classes. */
class QSqlRelationalTableModel;
class QDataWidgetMapper;
class QDialogButtonBox;
class QPushButton;
class QLineEdit;
class QLabel;

/* GUI string messages. */
static const QString tranWinTitleStr     = QObject::tr("Manage Transactions");

static const QString startDateLabelStr   = QObject::tr("&Start Date :");
static const QString startTimeLabelStr   = QObject::tr("&Start Time :");

static const QString completeButtonStr   = QObject::tr("&Complete Transaction");

static const QString deleteTransactStr   = QObject::tr("Do you want to delete the transaction?");
static const QString completeTransactStr = QObject::tr("Do you want to complete the transaction?");
static const QString cardExpiredStr      = QObject::tr("Customer's card is expired. Renew the card.");
static const QString notManyCardMoneyStr = QObject::tr("Not enough money in the card because charge is : ");
static const QString transactSuccessStr  = QObject::tr("The transaction has been completed.");

/* class which implements the transaction gui form and data model. */
class TransactionForm : public QDialog
{
    Q_OBJECT

    public:
        /* transaction index fields enumeration data type. */
        typedef enum transactionField {
            Transaction_Id = 0,
            Transaction_VehicleId,
            Transaction_CustomerId,
            Transaction_StartDate,
            Transaction_StartTime
        } transactionField;

        TransactionForm(const appSettings sets, QWidget *parent = 0);
        void done(const int result);

    private slots:
        void deleteTransaction();
        void completeTransaction();

    private:
        int getCustomerId(const int tran_id);
        int checkCardType(const int cust_id);

        int calculateTime(const QDate start_date, const QDate end_date,
                          const QTime start_time, const QTime end_time);

        bool completePayment(const int card_type,
                             const int cust_id,
                             const QString cust_name,
                             const double charge);

        void storeInReport(const QString cust_name, const QString vehi_name,
                           const QDate start_date, const QDate end_date,
                           const QTime start_time, const QTime end_time,
                           const double charge);

        bool chargeCustomerCard(const int cust_id, const double charge);
        double calculateCharge(const int card_type, const int time);
        double getCardMoney (const int cust_id);

        void lockGUI();
        void unlockGUI();
        void clearGUI();

        appSettings sets;

        QSqlRelationalTableModel *tableModel;
        QDataWidgetMapper *mapper;

        QLabel *vehicleLabel;
        QLabel *customerLabel;
        QLabel *startDateLabel;
        QLabel *startTimeLabel;

        QLineEdit *vehicleEdit;
        QLineEdit *customerEdit;

        EmptyDateEdit *startDateEdit;
        EmptyTimeEdit *startTimeEdit;

        QPushButton *firstButton;
        QPushButton *previousButton;
        QPushButton *nextButton;
        QPushButton *lastButton;
        QPushButton *deleteButton;
        QPushButton *completeButton;
        QPushButton *closeButton;

        QDialogButtonBox *buttonBox;
};

#endif // TRANSACTIONFORM_H
