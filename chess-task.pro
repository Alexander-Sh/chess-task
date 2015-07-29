TEMPLATE = app

QT += qml quick widgets

SOURCES += src/main.cpp \
    src/chessapplicaton.cpp \
    src/boardmodel.cpp \
    src/cellitems/chesspiece.cpp \
    src/cellitems/kingpiece.cpp \
    src/cellitems/queenpiece.cpp \
    src/cellitems/rookpiece.cpp \
    src/cellitems/knightpiece.cpp \
    src/cellitems/bishoppiece.cpp \
    src/cellitems/pawnpiece.cpp \
    src/utils.cpp

HEADERS += \
    src/chessapplicaton.h \
    src/boardmodel.h \
    src/cellitems/chesspiece.h \
    src/cellitems/kingpiece.h \
    src/cellitems/queenpiece.h \
    src/cellitems/rookpiece.h \
    src/cellitems/knightpiece.h \
    src/cellitems/bishoppiece.h \
    src/cellitems/pawnpiece.h \
    src/constants.h \
    src/utils.h

RESOURCES += qml/qml.qrc \
    resources/res.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =



