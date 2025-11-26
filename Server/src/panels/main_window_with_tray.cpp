#include "panels/main_window_with_tray.h"

#include <QWidget>
#include <QMainWindow>
#include <QMenu>

MainWindowWithTray::MainWindowWithTray(QWidget *parent): QMainWindow(parent), trayIcon(new QSystemTrayIcon(this)) {
//    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    // Tray icon menu
    auto menu = this->createMenu();
    this->trayIcon->setContextMenu(menu);

    // App icon
    auto appIcon = QIcon("eye.png");
    this->trayIcon->setIcon(appIcon);
    this->setWindowIcon(appIcon);

    // Displaying the tray icon
    this->trayIcon->show();     // Note: without explicitly calling show(), QSystemTrayIcon::activated signal will never be emitted!

    // Interaction
    QObject::connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindowWithTray::iconActivated);
}

QMenu* MainWindowWithTray::createMenu()
{
    // App can exit via Quit menu
    auto quitAction = new QAction("&Quit", this);
    QObject::connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    auto menu = new QMenu(this);
    menu->addAction(quitAction);

    return menu;
}

void MainWindowWithTray::iconActivated(QSystemTrayIcon::ActivationReason reason_)
{
    switch (reason_) {
        case QSystemTrayIcon::Trigger:
            show();
            break;
        default:
            ;
    }
}


void MainWindowWithTray::closeEvent(QCloseEvent *event)
{
//    event->ignore();
//    hide();
}

void MainWindowWithTray::hideEvent(QHideEvent *event) {
    event->ignore();
    hide();
}
