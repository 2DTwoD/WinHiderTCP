#ifndef MAIN_PANEL_H
#define MAIN_PANEL_H

#include "misc/thread_builder.h"
#include "tcp/tcp_obj.h"
#include "misc/updater.h"
#include "com_panel.h"

#include <QMainWindow>
#include <QFrame>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

class MainPanel: public QMainWindow{
Q_OBJECT
private:
    TCPobj* tcpObj;
    ComPanel* comPanel;
    Updater* updater;
    QPushButton* startButton;
    QPushButton* stopButton;
    QLabel* statusLabel;
public:
    explicit MainPanel(QWidget *parent = nullptr);

    ~MainPanel() override;
    void updateAction();
public slots:
    void startAction();
    void stopAction();
};

#endif //MAIN_PANEL_H
