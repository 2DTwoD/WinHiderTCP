#include "panels/main_panel.h"
#include "misc/thread_builder.h"

#include <QVBoxLayout>

#define UPDATER_TIME_MS 500

MainPanel::MainPanel(QWidget *parent) : MainWindowWithTray(parent), updateTimer(new QTimer(this)),
                                        tcpObj(new TCPobj(this)), winWork(new WinWork(this)),
                                        clientList(std::make_unique<QSet<TCPexchanger*>>()) {
    this->setWindowTitle("WinHider TCP server");
    this->resize(310, 100);
    auto mainFrame = new QFrame(this);
    auto mainLayout = new QVBoxLayout(mainFrame);
    mainFrame->setLayout(mainLayout);

    comPanel = new ComPanel(mainFrame);
    startButton = new QPushButton("start", mainFrame);
    stopButton = new QPushButton("stop", mainFrame);
    statusLabel = new QLabel("status: stopped", mainFrame);
    statusLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(comPanel);
    mainLayout->addWidget(startButton);
    mainLayout->addWidget(stopButton);
    mainLayout->addWidget(statusLabel);

    QObject::connect(startButton, &QPushButton::clicked, this, &MainPanel::startAction);

    QObject::connect(stopButton, &QPushButton::clicked, this, &MainPanel::stopAction);

    QObject::connect(updateTimer, &QTimer::timeout, this, &MainPanel::updateAction);

    QObject::connect(tcpObj, &TCPobj::tcpObjStop,
                     this, &MainPanel::tcpObjStop, Qt::DirectConnection);

    QObject::connect(tcpObj, &TCPobj::newTCPexchanger,
                     this, &MainPanel::newTCPexchanger, Qt::DirectConnection);

    QObject::connect(winWork, &WinWork::keyboardMouseAction,
                     this, &MainPanel::keyboardMouseAction, Qt::DirectConnection);

    QObject::connect(winWork, &WinWork::freeClient,
                     this, &MainPanel::freeClient, Qt::DirectConnection);


    this->setCentralWidget(mainFrame);

    if(comPanel->isAutostart()){
        startAction();
    } else {
        this->show();
    }
    updateTimer->start(UPDATER_TIME_MS);
}

MainPanel::~MainPanel() {
    qDebug("MainPanel: destructor");
    for(auto item: *clientList){
        item->shutdown();
    }
    tcpObj->shutdown();
    comPanel->saveConfig();
}

void MainPanel::startAction() {
    qDebug("MainPanel: start action, server with address: %s:%d", comPanel->getIP(), comPanel->getPort());
    tcpObj->start(comPanel->getIP(), comPanel->getPort());
}

void MainPanel::stopAction() {
    qDebug("MainPanel: stop action");
    tcpObj->stop();
}

void MainPanel::updateAction() {
    if(tcpObj->started()) {
        statusLabel->setText("status: started in address: " + QString::fromUtf8(comPanel->getIP()) +
                             ":" + QString::number(comPanel->getPort()));
    } else if(tcpObj->starting()) {
        statusLabel->setText("status: try starting in address: " + QString::fromUtf8(comPanel->getIP()) +
                             ":" + QString::number(comPanel->getPort()));
    } else {
        statusLabel->setText(tcpObj->failed() ? "status: failed" : "status: stopped");
    }
    comPanel->lock(!tcpObj->stopped());
    startButton->setEnabled(tcpObj->stopped());
    stopButton->setEnabled(!tcpObj->stopped());
}

void MainPanel::newTCPexchanger(SOCKET acceptSocket) {
    auto tcpExchanger = new TCPexchanger(acceptSocket);
    QObject::connect(tcpExchanger, &TCPexchanger::newToken,
                     this, &MainPanel::newToken, Qt::DirectConnection);

    QObject::connect(tcpExchanger, &TCPexchanger::freeDone,
                     this, &MainPanel::freeDone, Qt::DirectConnection);

    QObject::connect(tcpExchanger, &TCPexchanger::deleteTCPexchanger,
                     this, &MainPanel::deleteTCPexchanger, Qt::DirectConnection);

    QObject::connect(this, &MainPanel::tokenAccepted,
                     tcpExchanger, &TCPexchanger::tokenAccepted, Qt::DirectConnection);

    QObject::connect(this, &MainPanel::hiderBusy,
                     tcpExchanger, &TCPexchanger::hiderBusy, Qt::DirectConnection);

    newThread(this, tcpExchanger);
    clientListMutex.lock();
    clientList->insert(tcpExchanger);
    clientListMutex.unlock();
    qDebug("MainPanel: clientList size = %d", clientList->size());

}


void MainPanel::changeWindowVisible(bool value) {
    if(token.getName().isEmpty()) return;
    winWork->changeWindowVisible(value, token.getName());
}


void MainPanel::freeDone() {
    qDebug("MainPanel: freeDone");
    token.setValid(false);
}

void MainPanel::keyboardMouseAction(const QString &keyName) {
    if (token.isValid() && keyName == token.getKey()) {
        qDebug("WinWork: keyboard/mouse action with new token");
        changeWindowVisible(true);
    }
}

void MainPanel::newToken(const Token& newToken, TCPexchanger *const sender) {
    qDebug("WinWork: newToken");
    if(token.isValid()){
        qDebug("WinWork: emit busy");
        emit hiderBusy();
        return;
    }
    token = newToken;
    qDebug("WinWork: emit token accepted");
    emit tokenAccepted(sender);
    changeWindowVisible(false);
}

void MainPanel::deleteTCPexchanger(TCPexchanger *const tcpExchager) {
    clientListMutex.lock();
    if(tcpExchager->accepted()){
        resetTokenAndShowWindow();
    }
    clientList->erase(clientList->constFind(tcpExchager));
    qDebug("MainPanel: clientList size = %d", clientList->size());
    clientListMutex.unlock();
}

void MainPanel::tcpObjStop() {
    resetTokenAndShowWindow();
}

void MainPanel::freeClient() {
    for(auto item : *clientList){
        item->freeClient();
    }
}

void MainPanel::resetTokenAndShowWindow() {
    token.setValid(false);
    changeWindowVisible(true);
}