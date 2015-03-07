# program's template as application.
TEMPLATE = app

# internal name of the application.
INTERNAL_NAME = parkman

# application executable filename.
TARGET = $${INTERNAL_NAME}

# resources of the application.
RESOURCES = $${INTERNAL_NAME}.qrc

# configuration options for the application.
CONFIG += qtestlib

# qt sql support for the application.
QT += sql

# headers used in the application.
HEADERS = vehicleform.h \
         customerform.h \
      transactionform.h \
           reportform.h \
         settingsform.h \
            paywizard.h \
             mainform.h \
   globaldeclarations.h \
          appsettings.h \
        emptydateedit.h \
        emptytimeedit.h \
      arithmetictools.h \
         bankingtools.h

# sources used in the application.
SOURCES = vehicleform.cpp \
         customerform.cpp \
      transactionform.cpp \
           reportform.cpp \
         settingsform.cpp \
            paywizard.cpp \
             mainform.cpp \
        emptydateedit.cpp \
        emptytimeedit.cpp \
      arithmetictools.cpp \
         bankingtools.cpp \
                 main.cpp
