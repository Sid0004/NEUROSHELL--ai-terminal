// Qt GUI entry for a basic terminal-like window
#include <QApplication>
#include "TerminalWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    TerminalWindow window;
    window.show();

    return app.exec();
}
