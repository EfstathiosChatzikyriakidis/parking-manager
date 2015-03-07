/* header defining the interface of the source. */
#ifndef CUSTOMERFORM_H
#define CUSTOMERFORM_H

/* include some QT libraries. */
#include <QDialog>

/* include header defining the interface of the source. */
#include "emptydateedit.h"

/* use these classes. */
class QSqlRelationalTableModel;
class QDataWidgetMapper;
class QDialogButtonBox;
class QPushButton;
class QComboBox;
class QLineEdit;
class QLabel;

/* GUI string messages. */
static const QString custWinTitleStr   = QObject::tr("Manage Customers");

static const QString addressLabelStr   = QObject::tr("&Address :");
static const QString cityLabelStr      = QObject::tr("&City :");
static const QString stateLabelStr     = QObject::tr("&State :");
static const QString phoneLabelStr     = QObject::tr("&Phone :");
static const QString emailLabelStr     = QObject::tr("&Email :");
static const QString cardTypeLabelStr  = QObject::tr("Card &Type :");
static const QString cardDateLabelStr  = QObject::tr("Card &Date :");
static const QString cardMoneyLabelStr = QObject::tr("Card &Money :");

static const QString cardButtonStr     = QObject::tr("&Add / Edit Card");

static const QString cardInputMoneyStr = QObject::tr("Please input money for the card :");
static const QString deleteCustomerStr = QObject::tr("Do you want to delete the customer?");
static const QString cardCreationStr   = QObject::tr("Do you want to add / edit member card?");

/* class which implements the customer gui form and data model. */
class CustomerForm : public QDialog
{
    Q_OBJECT

    public:
        /* customer index fields enumeration data type. */
        typedef enum customerField {
            Customer_Id = 0,
            Customer_Name,
            Customer_Address,
            Customer_City,
            Customer_State,
            Customer_Phone,
            Customer_Email,
            Customer_CardDate,
            Customer_CardMoney,
            Customer_CardId
        } customerField;

        CustomerForm(const int id, QWidget *parent = 0);
        void done(const int result);

    private slots:
        void addEditCard();
        void addCustomer();
        void deleteCustomer();
        void handleCardType(const int index);

    private:
        void lockGUI();
        void unlockGUI();
        void clearGUI();

        QSqlRelationalTableModel *tableModel;
        QDataWidgetMapper *mapper;

        int previousCardType;

        QLabel *nameLabel;
        QLabel *addressLabel;
        QLabel *cityLabel;
        QLabel *stateLabel;
        QLabel *phoneLabel;
        QLabel *emailLabel;
        QLabel *cardTypeLabel;
        QLabel *cardDateLabel;
        QLabel *cardMoneyLabel;

        QLineEdit *nameEdit;
        QLineEdit *addressEdit;
        QLineEdit *cityEdit;
        QLineEdit *stateEdit;
        QLineEdit *phoneEdit;
        QLineEdit *emailEdit;
        QLineEdit *cardMoneyEdit;
        QComboBox *cardComboBox;

        EmptyDateEdit *cardDateEdit;

        QPushButton *addButton;
        QPushButton *cardButton;
        QPushButton *deleteButton;
        QPushButton *closeButton;

        QDialogButtonBox *buttonBox;
};

#endif // CUSTOMERFORM_H
