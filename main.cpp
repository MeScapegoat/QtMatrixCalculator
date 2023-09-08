#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <QApplication>
#include <QLabel>
#include <QLineEdit>

#include "MainWindow.h"
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
