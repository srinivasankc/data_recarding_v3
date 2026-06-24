#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Show login window first
    LoginWindow loginWindow;
    if (loginWindow.exec() == QDialog::Accepted) {
        MainWindow w;
        w.show();
        return a.exec();
    }

    return 0;
}