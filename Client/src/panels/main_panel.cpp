#include "panels/main_panel.h"

#include <QVBoxLayout>

#define UPDATER_TIME_MS 500

MainPanel::MainPanel(QString title, QWidget *parent) : MainWindowWithTray(parent), winWork(new WinWork(this)),
                                        fileWork(new FileWork(this)), tcpObj(new TCPobj()),
                                        updateTimer(new QTimer(this)), title(title) {
    this->setWindowTitle(title);
    this->resize(310, 150);
    auto mainFrame = new QFrame(this);
    auto mainLayout = new QVBoxLayout(mainFrame);
    mainFrame->setLayout(mainLayout);

    comPanel = new ComPanel(mainFrame);
    bindPanel = new BindPanel(mainFrame);
    connectButton = new QPushButton("connect", mainFrame);
    disconnectButton = new QPushButton("disconnect", mainFrame);
    statusLabel = new QLabel("status: disconnected", mainFrame);
    statusLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(comPanel);
    mainLayout->addWidget(bindPanel);
    mainLayout->addWidget(connectButton);
    mainLayout->addWidget(disconnectButton);
    mainLayout->addWidget(statusLabel);
    readConfig();

    QObject::connect(connectButton, &QPushButton::clicked, this, &MainPanel::connectAction);
    QObject::connect(disconnectButton, &QPushButton::clicked, this, &MainPanel::disconnectAction);
    QObject::connect(bindPanel->getBindButton(), &QPushButton::clicked, winWork, &WinWork::startBind);
    QObject::connect(winWork, &WinWork::bindFinished, bindPanel, &BindPanel::bindFinished);
    QObject::connect(winWork, &WinWork::keyboardMouseAction, this, &MainPanel::keyboardMouseAction);
    QObject::connect(updateTimer, &QTimer::timeout, this, &MainPanel::updateAction);


    this->setCentralWidget(mainFrame);
    this->show();
    if(comPanel->isAutostart()){
        connectAction();
    }
    updateTimer->start(UPDATER_TIME_MS);
}

MainPanel::~MainPanel() {
    qDebug("MainPanel: destructor");
    tcpObj->shutdown();
    tcpObj->getThread()->wait(1000);
    saveConfig();
}

void MainPanel::connectAction() {
    qDebug("MainPanel: connect action, server with address: %s:%d", comPanel->getIP(), comPanel->getPort());
    tcpObj->connect(comPanel->getIP(), comPanel->getPort());
}

void MainPanel::disconnectAction() {
    qDebug("MainPanel: disconnect action");
    tcpObj->disconnect();
}

void MainPanel::updateAction() {
    QString status;
    if (tcpObj->connected()) {
        if(tcpObj->getSendFlag()){
            if(tcpObj->isBusy()){
                setIcon(ICON_CONNECTED);
                status = "status: connected to address: " + QString::fromLocal8Bit(comPanel->getIP()) +
                         ":" + QString::number(comPanel->getPort()) + ", busy...";
            } else {
                setIcon(ICON_HIDED);
                status = "status: connected to address: " + QString::fromLocal8Bit(comPanel->getIP()) +
                         ":" + QString::number(comPanel->getPort()) + " and hided";
            }
        } else {
            setIcon(ICON_CONNECTED);
            status = "status: connected to address: " + QString::fromLocal8Bit(comPanel->getIP()) +
                                 ":" + QString::number(comPanel->getPort());
        }
    } else if(tcpObj->connecting()) {
        status = "status: try connecting to address: " + QString::fromLocal8Bit(comPanel->getIP()) +
                      ":" + QString::number(comPanel->getPort());
    } else {
        setIcon(ICON_DISCONNECTED);
        status = tcpObj->failed()? "status: failed": "status: disconnected";
    }
    statusLabel->setText(status);
    comPanel->lock(!tcpObj->disconnected() || lockFlag);
    connectButton->setEnabled(!tcpObj->connected() && !lockFlag);
    disconnectButton->setEnabled(tcpObj->connected() && !lockFlag);
    lock(WinWork::binding() || tcpObj->isBusy());
}


void MainPanel::readConfig() {
    auto list = fileWork->readConfig().split(";");
    if(list.size() != 5) {
        qDebug("MainPanel: wrong config");
        return;
    }
    for(auto item: list){
        auto keyValue = item.split(":");
        if(keyValue.size() != 2) break;
        if(keyValue[0] == "ip"){
            comPanel->setQIP(keyValue[1]);
        } else if(keyValue[0] == "port"){
            comPanel->setQPort(keyValue[1]);
        } else if(keyValue[0] == "autostart"){
            comPanel->setQAutostart(keyValue[1]);
        } else if(keyValue[0] == "wname"){
            bindPanel->setQWinName(keyValue[1]);
        } else if(keyValue[0] == "key"){
            if(keyValue[1].size() > 1 && QString("km").contains(keyValue[1][0])){
                bindPanel->setQKey(keyValue[1]);
            }
        } else {
            qDebug("MainPanel: wrong config");
            break;
        }
    }
    comPanel->getIP();
    comPanel->getPort();
}

void MainPanel::saveConfig() {
    fileWork->saveConfig(std::move("ip:" + comPanel->getQIP() + ";port:" + comPanel->getQPort() +
                                   ";autostart:" + QString::number(comPanel->isAutostart()) +
                                   ";wname:" + bindPanel->getQWinName() + ";key:" + bindPanel->getQKey()));
}

void MainPanel::keyboardMouseAction(const QString& keyName) {
    if(keyName != bindPanel->getQKey()) return;
    qDebug("MainPanel: keyboard/mouse action");
    tcpObj->sendNewToken(keyName, bindPanel->getQWinName());
}

void MainPanel::lock(bool value) {
    lockFlag = value;
    bindPanel->lock(value);
    comPanel->lockAutoStart(value);
}

