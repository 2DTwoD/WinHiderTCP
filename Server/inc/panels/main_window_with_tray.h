#ifndef MAIN_WINDOW_WITH_TRAY_H
#define MAIN_WINDOW_WITH_TRAY_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>

class MainWindowWithTray: public QMainWindow {
Q_OBJECT

private:
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QMenu *createMenu();

public:
    explicit MainWindowWithTray(QWidget *parent);

protected:

    void closeEvent(QCloseEvent *event) override;
    void hideEvent(QHideEvent *event) override;

public slots:

    void iconActivated(QSystemTrayIcon::ActivationReason);
};

#endif //MAIN_WINDOW_WITH_TRAY_H
