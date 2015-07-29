#ifndef CHESSAPPLICATON_H
#define CHESSAPPLICATON_H

#include <QApplication>
#include <QQmlApplicationEngine>

#include "boardmodel.h"

class ChessApplicaton : public QApplication
{
public:
    ChessApplicaton(int &argc, char **argv);

private:
    void initialize();

private:
    QQmlApplicationEngine m_qmlEngine;
    BoardModel m_boardModel;

};

#endif // CHESSAPPLICATON_H
