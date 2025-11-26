#ifndef MAIN_PANEL_H
#define MAIN_PANEL_H

#include "tcp/tcp_obj.h"
#include "tcp/tcp_exchanger.h"
#include "com_panel.h"
#include "hider/win_work.h"
#include "panels/main_window_with_tray.h"

#include <QMainWindow>
#include <QFrame>
#include <QTimer>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QSet>
#include <QCloseEvent>

class MainPanel: public MainWindowWithTray{
Q_OBJECT

private:
    TCPobj *tcpObj;
    WinWork *winWork;
    QTimer *updateTimer;
    QMutex clientListMutex;
    Token token;

    std::unique_ptr<QSet<TCPexchanger *>> clientList;

    ComPanel *comPanel;
    QPushButton *startButton;
    QPushButton *stopButton;
    QLabel *statusLabel;

    void changeWindowVisible(bool value);

    void resetTokenAndShowWindow();

public:
    explicit MainPanel(QWidget *parent = nullptr);

    ~MainPanel() override;

    void updateAction();

public slots:

    void startAction();

    void stopAction();

    void newTCPexchanger(SOCKET acceptSocket);

    void deleteTCPexchanger(TCPexchanger *const tcpExchager);

    void freeDone();

    void keyboardMouseAction(const QString &value);

    void newToken(const Token &newToken, TCPexchanger *const sender);

    void tcpObjStop();

    void freeClient();

signals:

    void tokenAccepted(TCPexchanger *source);

    void hiderBusy();
};

#endif //MAIN_PANEL_H
