#include "mainwindow.h"
#include "Main_MPW.h"
#include "Stacked_W.h"

#include <QApplication>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow *w = new MainWindow();
    Stacked_W *ch = new Stacked_W();
    w->setCentralWidget(ch);
    w->show();


    return a.exec();
}
