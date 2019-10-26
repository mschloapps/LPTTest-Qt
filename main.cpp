
#include <QApplication>
#include "charttimer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // create and show new instance of chart
    ChartTimer window;
    //window.resize(800, 600);
    window.show();
    return a.exec();
}
