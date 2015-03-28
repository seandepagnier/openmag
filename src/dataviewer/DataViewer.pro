!include("../make/config.mk") {
  message("config.mk not found, please run ./configure to generate it")
}

!isEmpty( USE_CCACHE ) {
  QMAKE_CXX="ccache g++"
}

HEADERS     = DataViewer.h \
              Settings.h \
              ConsoleWidget.h \
              DataTree.h \
              DataComboBox.h \
              DataClearButton.h \
              DataProcess.h

RESOURCES   = DataViewer.qrc

FORMS       = DataViewer.ui \
              Settings.ui \

SOURCES     = DataViewer.cpp \
              Settings.cpp \
              ConsoleWidget.cpp \
              DataTree.cpp \
              DataComboBox.cpp \
              DataClearButton.cpp \
              DataProcess.cpp \
              main.cpp

CONFIG  += qt

# install
target.path = ../bin
INSTALLS += target
