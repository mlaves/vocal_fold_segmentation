#include "mainwindow.h"
#include "tabletapplication.h"
#include "tabletcanvas.h"

int main(int argc, char *argv[])
{
    TabletApplication app(argc, argv);
    TabletCanvas *canvas = new TabletCanvas;
    app.setCanvas(canvas);

    MainWindow maiWin(canvas);
    maiWin.show();

    return app.exec();
}
