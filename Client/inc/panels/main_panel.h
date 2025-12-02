#ifndef MAIN_PANEL_H
#define MAIN_PANEL_H

#include "misc/thread_builder.h"
#include "tcp/tcp_obj.h"
#include "com_panel.h"
#include "bind_panel.h"
#include "main_window_with_tray.h"

#include <QMainWindow>
#include <QFrame>
#include <QTimer>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

class MainPanel: public MainWindowWithTray {
Q_OBJECT

private:
    bool lockFlag{false};
    QString title;
    WinWork *winWork;
    FileWork *fileWork;
    TCPobj *tcpObj;
    ComPanel *comPanel;
    BindPanel *bindPanel;
    QTimer* updateTimer;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QLabel *statusLabel;
    void readConfig();
    void saveConfig();
public:
    explicit MainPanel(QString title = "WinHider TCP", QWidget *parent = nullptr);

    ~MainPanel() override;

    void updateAction();

    void lock(bool value);

public slots:

    void connectAction();

    void disconnectAction();

    void keyboardMouseAction(const QString& keyName);
};

#endif //MAIN_PANEL_H
