#ifndef MAIN_WINDOW_WITH_TRAY_H
#define MAIN_WINDOW_WITH_TRAY_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QImage>
#include <QColor>

enum IconType{
    ICON_DISCONNECTED = 0,
    ICON_CONNECTED,
    ICON_HIDED
};

class MainWindowWithTray: public QMainWindow {
Q_OBJECT

private:
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QMenu *createMenu();

    QIcon disconnectedIcon;
    QIcon connectedIcon;
    QIcon hidedIcon;

    bool iconIsNull();
    QImage getColoredImage(QImage src, QColor color);

public:
    explicit MainWindowWithTray(QWidget *parent);
    void setIcon(IconType iconType);

protected:

    void hideEvent(QHideEvent *event) override;

public slots:

    void iconActivated(QSystemTrayIcon::ActivationReason);
};

#endif //MAIN_WINDOW_WITH_TRAY_H
