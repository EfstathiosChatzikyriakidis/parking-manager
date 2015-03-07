/* header defining the interface of the source. */
#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

/* include some QT libraries. */
#include <QDialog>

/* include header defining the interface of the source. */
#include "appsettings.h"

/* use these classes. */
class QDialogButtonBox;
class QPushButton;
class QDoubleSpinBox;
class QSpinBox;
class QLabel;

/* GUI string messages. */
static const QString setsWinTitleStr         = QObject::tr("Manage Settings");

static const QString parkingCapacityLabelStr = QObject::tr("&Parking Capacity :");
static const QString chargeTimesliceLabelStr = QObject::tr("&Payment Timeslice :");
static const QString chargePerSliceLabelStr  = QObject::tr("&Charge / Timeslice :");
static const QString chargePrecisionLabelStr = QObject::tr("&Charge Precision :");
static const QString saveButtonStr           = QObject::tr("&Save");

/* class which implements the settings gui form. */
class SettingsForm : public QDialog
{
    Q_OBJECT

    public:
        SettingsForm(const appSettings sets, QWidget *parent = 0);
        void done(const int result);

    private slots:
        void writeSettings();

    private:
        appSettings sets;

        QLabel *parkingCapacityLabel;
        QLabel *chargeTimesliceLabel;
        QLabel *chargePerSliceLabel;
        QLabel *chargePrecisionLabel;

        QSpinBox *parkingCapacitySpin;
        QSpinBox *chargeTimesliceSpin;
        QDoubleSpinBox *chargePerSliceSpin;
        QSpinBox *chargePrecisionSpin;

        QPushButton *saveButton;
        QPushButton *closeButton;

        QDialogButtonBox *buttonBox;
};

#endif // SETTINGSFORM_H
