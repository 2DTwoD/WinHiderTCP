#include <QApplication>

#include "panels/main_panel.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainPanel mainPanel;
    return QApplication::exec();
}
