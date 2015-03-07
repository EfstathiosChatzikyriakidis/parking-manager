/* header defining the interface of the source. */
#ifndef MAINFORM_H
#define MAINFORM_H

/* include some QT libraries. */
#include <QWidget>

/* include header defining the interface of the source. */
#include "appsettings.h"

/* use these classes. */
class QSqlRelationalTableModel;
class QDialogButtonBox;
class QModelIndex;
class QPushButton;
class QToolButton;
class QTableView;
class QSplitter;
class QLabel;

/* GUI string messages. */
static const QString vehiclesButtonStr  = QObject::tr("Manage &Vehicles");
static const QString customersButtonStr = QObject::tr("Manage &Customers");
static const QString transactsButtonStr = QObject::tr("Manage &Transactions");
static const QString reportButtonStr    = QObject::tr("Transactions &Report");
static const QString guestVehiclesStr   = QObject::tr("&Show Guest Vehicles");
static const QString quitButtonStr      = QObject::tr("&Quit");
static const QString settingsButtonStr  = QObject::tr("&Settings");

static const QString vehiclesOfStr      = QObject::tr("Ve&hicles of %1");
static const QString vehiclesStr        = QObject::tr("Ve&hicles");
static const QString customersStr       = QObject::tr("&Customers");

static const QString nameStr            = QObject::tr("Name");
static const QString cardStr            = QObject::tr("Card Type");
static const QString phoneStr           = QObject::tr("Phone");
static const QString descStr            = QObject::tr("Description");

/* class which implements the main gui form. */
class MainForm : public QWidget
{
    Q_OBJECT

    public:
        MainForm();

    private slots:
        void updateVehicleView();
        void updateCustomerView();
        void editVehicles();
        void editCustomers();
        void editTransactions();
        void editSettings();
        void reportTransactions();
        void handleAbout();
        void handleGuestVehicles(const bool buttonPressed);

    private:
        void createCustomerPanel();
        void createVehiclePanel();
        void establishSettings();

        appSettings sets;

        QSqlRelationalTableModel *customerModel;
        QSqlRelationalTableModel *vehicleModel;

        QWidget *customerPanel;
        QWidget *vehiclePanel;

        QLabel *customerLabel;
        QLabel *vehicleLabel;

        QTableView *customerView;
        QTableView *vehicleView;

        QSplitter *splitter;

        QPushButton *editButtonVehicle;
        QPushButton *editButtonCustomer;
        QPushButton *editButtonTransaction;
        QPushButton *buttonReportTransaction;
        QPushButton *buttonGuestVehicles;
        QPushButton *settingsButton;
        QPushButton *quitButton;
        QToolButton *aboutButton;

        QDialogButtonBox *buttonBox;
};

#endif // MAINFORM_H
