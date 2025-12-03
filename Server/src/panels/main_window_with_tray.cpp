#include "panels/main_window_with_tray.h"

#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QGraphicsEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

MainWindowWithTray::MainWindowWithTray(QWidget *parent): QMainWindow(parent), trayIcon(new QSystemTrayIcon(this)) {
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    disconnectedIcon = QIcon("pics/opened_eye.png");

    connectedIcon = QIcon(QPixmap::fromImage(getColoredImage(QImage("pics/opened_eye.png"),
                                                             QColor(255, 255, 255))));

    hidedIcon = QIcon(QPixmap::fromImage(getColoredImage(QImage("pics/closed_eye.png"),
                                                         QColor(255, 255, 255))));
    setIcon(ICON_DISCONNECTED);

    auto menu = this->createMenu();
    this->trayIcon->setContextMenu(menu);
    this->trayIcon->show();

    QObject::connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindowWithTray::iconActivated);
}

QImage MainWindowWithTray::getColoredImage(QImage src, QColor color){
    if(src.isNull()) return QImage();
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(src));
    QGraphicsColorizeEffect effect;
    effect.setColor(color);
    effect.setStrength(1.0);
    item.setGraphicsEffect(&effect);
    scene.addItem(&item);
    QImage res(src);
    QPainter ptr(&res);
    scene.render(&ptr, QRectF(), src.rect());
    return res;
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
    if(!iconIsNull()) {
        event->ignore();
        hide();
    }
}

void MainWindowWithTray::setIcon(IconType iconType) {
    if(iconIsNull()) return;
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

bool MainWindowWithTray::iconIsNull() {
    return disconnectedIcon.isNull() || hidedIcon.isNull() || hidedIcon.isNull();
}
