#include <QApplication>
#include "mainwindow.h"
#include "loginwindow.h"
#include "userwindow.h"
#include "selectquizwindow.h"
#include "commonHeaders.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginWindow lw;
    lw.show();

    //UserWindow uw;
    //uw.show();

    //SelectQuizWindow sqw;
    //sqw.show();

    //MainWindow w;
    //w.show();
    
    return a.exec();
}
