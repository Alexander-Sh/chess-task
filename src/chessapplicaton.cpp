#include "chessapplicaton.h"

#include <QQmlContext>

ChessApplicaton::ChessApplicaton(int &argc, char **argv)
    : QApplication(argc, argv)
{
    initialize();
}

void ChessApplicaton::initialize()
{
    m_qmlEngine.rootContext()->setContextProperty("boardModel", &m_boardModel);
    m_qmlEngine.load(QUrl(QStringLiteral("qrc:/main.qml")));
}

