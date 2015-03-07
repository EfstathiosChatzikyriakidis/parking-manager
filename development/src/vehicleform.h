/* header defining the interface of the source. */
#ifndef VEHICLEFORM_H
#define VEHICLEFORM_H

/* include some QT libraries. */
#include <QDialog>

/* use these classes. */
class QSqlRelationalTableModel;
class QDataWidgetMapper;
class QDialogButtonBox;
class QPushButton;
class QComboBox;
class QLineEdit;
class QLabel;

/* GUI string messages. */
static const QString vehiWinTitleStr    = QObject::tr("Manage Vehicles");

static const QString descLabelStr       = QObject::tr("&Description :");

static const QString transactButtonStr  = QObject::tr("&Start Transaction");

static const QString noCapacityInDBStr  = QObject::tr("There is no more capacity for vehicles.");
static const QString vehicleReservedStr = QObject::tr("The vehicle is in the parking.");
static const QString deleteVehicleStr   = QObject::tr("Do you want to delete the vehicle?");

/* class which implements the vehicle gui form and data model. */
class VehicleForm : public QDialog
{
    Q_OBJECT

    public:
        /* vehicles index fields enumeration data type. */
        typedef enum vehicleField {
            Vehicle_Id = 0,
            Vehicle_Name,
            Vehicle_Description,
            Vehicle_CustomerId
        } vehicleField;

        VehicleForm(const int capacity, const int id, QWidget *parent = 0);
        void done(const int result);

    private slots:
        void addVehicle();
        void deleteVehicle();
        void transactionVehicle();

    private:
        void lockGUI();
        void unlockGUI();
        void clearGUI();

        int capacity;

        QSqlRelationalTableModel *tableModel;
        QDataWidgetMapper *mapper;

        QLabel *nameLabel;
        QLabel *descLabel;
        QLabel *customerLabel;

        QLineEdit *nameEdit;
        QLineEdit *descEdit;
        QComboBox *customerComboBox;

        QPushButton *addButton;
        QPushButton *deleteButton;
        QPushButton *transactionButton;
        QPushButton *closeButton;

        QDialogButtonBox *buttonBox;
};

#endif // VEHICLEFORM_H
