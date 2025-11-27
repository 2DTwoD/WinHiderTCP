#include "panels/main_window_with_tray.h"

#include <QMenu>

MainWindowWithTray::MainWindowWithTray(QWidget *parent): QMainWindow(parent), trayIcon(new QSystemTrayIcon(this)) {
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    auto menu = this->createMenu();
    this->trayIcon->setContextMenu(menu);

    disconnectedIcon = QIcon("pics/opened_eye_black.png");
    connectedIcon = QIcon("pics/opened_eye_white.png");
    hidedIcon = QIcon("pics/closed_eye_white.png");
    setIcon(ICON_DISCONNECTED);

    this->trayIcon->show();

    QObject::connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindowWithTray::iconActivated);
}

QMenu* MainWindowWithTray::createMenu()
{
    auto quitAction = new QAction("&quit", this);
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
            setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
            activateWindow();
            break;
        default:
            ;
    }
}

void MainWindowWithTray::hideEvent(QHideEvent *event) {
    event->ignore();
    hide();
}

void MainWindowWithTray::setIcon(IconType iconType) {
    switch (iconType) {
        case ICON_DISCONNECTED:
            this->trayIcon->setIcon(disconnectedIcon);
            this->setWindowIcon(disconnectedIcon);
            break;
        case ICON_CONNECTED:
            this->trayIcon->setIcon(connectedIcon);
            this->setWindowIcon(connectedIcon);
            break;
        case ICON_HIDED:
            this->trayIcon->setIcon(hidedIcon);
            this->setWindowIcon(hidedIcon);
            break;
    }
}
