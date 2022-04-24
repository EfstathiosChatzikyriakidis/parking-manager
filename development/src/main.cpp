/*
 *  This file implements the main startup of the application.
 *
 *  Copyright (C) 2010  Efstathios Chatzikyriakidis (stathis.chatzikyriakidis@gmail.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* include some QT and ANSI C library headers. */
#include <QtGui>
#include <QtSql>
#include <QTest>
#include <cstdlib>
using namespace std;

/* include headers defining the interface of the sources. */
#include "mainform.h"
#include "globaldeclarations.h"

/* declare DB driver and filename. */
static const QString dbDriverStr = "QSQLITE";
static const QString dbFileNameStr = "database.db";

/* GUI string messages. */
static const QString dbConnectErrorStr       = QObject::tr("Database Connection Error");
static const QString dbDriverNotExistStr     = QObject::tr("Database driver is not available.");
static const QString dbCannotOpenStr         = QObject::tr("The database cannot open.");
static const QString dbCreationStr           = QObject::tr("Creating a database instance...");

static const QString splashDBDriversStr      = QObject::tr("Searching Available Database Drivers...");
static const QString splashDBConnectionStr   = QObject::tr("Establishing Database Connection...");
static const QString splashSearchDBStr       = QObject::tr("Searching Application's Database...");
static const QString splashCreateDBSchemaStr = QObject::tr("Creating Default Database Schema...");
static const QString splashFillDBStr         = QObject::tr("Filling Database Default Data...");
static const QString splashAppStartStr       = QObject::tr("Opening Parking Manager Application...");

/* application's splashscreen object. */
static QSplashScreen *splash = NULL;

/* alignment properties for splashscreen text. */
static const Qt::Alignment topCenter = Qt::AlignHCenter | Qt::AlignTop;

/* splashscreen text wait time. */
static const int SPLASH_TEXT_DELAY = 1500;

/* progress bar number of steps. */
static const int PBAR_MAX_STEPS = 8;

/* creates a connection to the DB. */
static bool
createDBConnection() {
    /* splashscreen message. */
    splash->showMessage(splashDBDriversStr, topCenter);
    qApp->processEvents();
    QTest::qWait(SPLASH_TEXT_DELAY);

    /* check the existence of the DB driver. */
    if (!QSqlDatabase::isDriverAvailable (dbDriverStr)) {
        QMessageBox::critical(0, dbConnectErrorStr, dbDriverNotExistStr);

        /* DB connection failed. */
        return false;
    }

    /* splashscreen message. */
    splash->showMessage(splashDBConnectionStr, topCenter);
    qApp->processEvents();
    QTest::qWait(SPLASH_TEXT_DELAY);

    /* connect to the DB with the following driver. */
    QSqlDatabase db = QSqlDatabase::addDatabase(dbDriverStr);

    /* splashscreen message. */
    splash->showMessage(splashSearchDBStr, topCenter);
    qApp->processEvents();
    QTest::qWait(SPLASH_TEXT_DELAY);

    /* use the following database name. */
    db.setDatabaseName(dbFileNameStr);

    /* if the DB cannot open for any reason print a warning. */
    if (!db.open()) {
        QMessageBox::critical(0, dbConnectErrorStr, dbCannotOpenStr);

        /* DB connection failed. */
        return false;
    }

    /* DB connection successed. */
    return true;
}

/* creates a startup DB instance. */
static void
createDBInstance() {
    /* initial step of the progress bar. */
    int step = 0;

    /* create and setup a progress bar. */
    QProgressDialog progress;
    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowTitle(appName);
    progress.setCancelButtonText(QString()); /* hide cancel button. */
    progress.setLabelText(dbCreationStr);
    progress.setMinimum(step);
    progress.setMaximum(PBAR_MAX_STEPS);

    /* start progress. */
    progress.setValue(++step);
    qApp->processEvents();

    /* splashscreen message. */
    splash->showMessage(splashCreateDBSchemaStr, topCenter);
    qApp->processEvents();
    QTest::qWait(SPLASH_TEXT_DELAY);

    /* declare a sql query object. */
    QSqlQuery query;

    /* create DB Schema. */
    query.exec("CREATE TABLE cardtype ("
               "  id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "  title TEXT NOT NULL)");

    /* continue progress. */
    progress.setValue(++step);
    qApp->processEvents();

    query.exec("CREATE TABLE customer ("
               "  id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "  name TEXT NOT NULL, "
               "  address TEXT, "
               "  city TEXT, "
               "  state TEXT, "
               "  phone TEXT,"
               "  email TEXT, "
               "  card_date TEXT,"
               "  card_money REAL,"
               "  card_id INTEGER NOT NULL, "
               "  FOREIGN KEY (card_id) REFERENCES cardtype)");

    /* continue progress. */
    progress.setValue(++step);
    qApp->processEvents();

    query.exec("CREATE TABLE vehicle ("
               "  id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "  reg_num TEXT NOT NULL, "
               "  desc TEXT, "
               "  cust_id INTEGER NOT NULL, "
               "  FOREIGN KEY (cust_id) REFERENCES customer)");

    /* continue progress. */
    progress.setValue(++step);
    qApp->processEvents();

    query.exec("CREATE TABLE transacts ("
               "  id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "  vehi_id INTEGER NOT NULL, "
               "  cust_id INTEGER NOT NULL, "
               "  start_date TEXT NOT NULL,"
               "  start_time TEXT NOT NULL,"
               "  FOREIGN KEY (vehi_id) REFERENCES vehicle, "
               "  FOREIGN KEY (cust_id) REFERENCES customer)");

    /* continue progress. */
    progress.setValue(++step);
    qApp->processEvents();

    query.exec("CREATE TABLE report ("
               "  id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "  vehicle TEXT NOT NULL, "
               "  start_date TEXT NOT NULL,"
               "  end_date TEXT NOT NULL,"
               "  start_time TEXT NOT NULL,"
               "  end_time TEXT NOT NULL,"
               "  charge REAL NOT NULL,"
               "  customer TEXT NOT NULL)");

    /* continue progress. */
    progress.setValue(++step);
    qApp->processEvents();

    /* splashscreen message. */
    splash->showMessage(splashFillDBStr, topCenter);
    qApp->processEvents();
    QTest::qWait(SPLASH_TEXT_DELAY);

    /* fill the card types. */
    query.exec("INSERT INTO cardtype (title) VALUES ('No Card Customer')");
    query.exec("INSERT INTO cardtype (title) VALUES ('Simple Member Card')");
    query.exec("INSERT INTO cardtype (title) VALUES ('Month Member Card')");
    query.exec("INSERT INTO cardtype (title) VALUES ('Year Member Card')");
    query.exec("INSERT INTO cardtype (title) VALUES ('Credit Card Member')");

    /* continue progress. */
    progress.setValue(++step);
    qApp->processEvents();

    /* insert the guest customer. */
    query.exec("INSERT INTO customer (name, card_id) VALUES ('Simple Guest', 1)");

    /* finish the progress. */
    progress.setValue(progress.maximum());
    qApp->processEvents();
}

/* main function. */
int
main(int argc, char *argv[]) {
    /* create the application. */
    QApplication app(argc, argv);

    /* setup some global CSS stylesheet instructions. */
    qApp->setStyleSheet("QToolButton { border: none; }");

    /* create a splashscreen window and show it. */
    splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/graphics/splashscreen/splash"));
    splash->show();

    /* check for an existing DB (before the connection). */
    bool existingDB = QFile::exists(dbFileNameStr);

    /* try to connect to DB. */
    if (!createDBConnection()) {
        return EXIT_FAILURE;
    }

    /* if DB does not exist create a fake one. */
    if (!existingDB) {
        createDBInstance();
    }

    /* splashscreen message. */
    splash->showMessage(splashAppStartStr, topCenter);
    qApp->processEvents();
    QTest::qWait(SPLASH_TEXT_DELAY);

    /* create the main form of the app. */
    MainForm form;

    /* show the main form. */
    form.show();

    /* finish the splashscreen. */
    splash->finish(&form);

    /* delete the splashscreen. */
    delete splash;

    /* run the application. */
    return app.exec();
}
