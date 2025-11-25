#ifndef MAIN_PANEL_H
#define MAIN_PANEL_H

#include "misc/thread_builder.h"
#include "tcp/tcp_obj.h"
#include "com_panel.h"
#include "bind_panel.h"

#include <QMainWindow>
#include <QFrame>
#include <QTimer>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

class MainPanel: public QMainWindow {
Q_OBJECT

private:
    bool lockFlag{false};
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
    explicit MainPanel(QWidget *parent = nullptr);

    ~MainPanel() override;

    void updateAction();

public slots:

    void lockAll();

    void unlockAll();

    void connectAction();

    void disconnectAction();

    void keyboardMouseAction(const QString& keyName);
};

#endif //MAIN_PANEL_H
