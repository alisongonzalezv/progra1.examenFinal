#include "PizzaWindow.h"

#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    PizzaWindow window;
    window.show();
    return app.exec();
}
