#include "panels/main_panel.h"
#include "misc/thread_builder.h"

#include <QVBoxLayout>

#define UPDATER_TIME_MS 500

MainPanel::MainPanel(QString title, QWidget *parent) : MainWindowWithTray(parent), updateTimer(new QTimer(this)),
                                        tcpObj(new TCPobj()), winWork(new WinWork(this)),
                                        clientList(std::make_unique<QSet<TCPexchanger*>>()), title(title) {
    this->setWindowTitle(title + " server");
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
                     this, &MainPanel::keyboardMouseAction);

    QObject::connect(winWork, &WinWork::freeClient,
                     this, &MainPanel::freeClient);

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
        item->getThread()->wait(1000);
    }
    tcpObj->shutdown();
    tcpObj->getThread()->wait(1000);
    comPanel->saveConfig();
}

void MainPanel::startAction() {
    qDebug("MainPanel: start action, server with address: %s:%d", comPanel->getIP().toUtf8().data(), comPanel->getPort());
    tcpObj->start(comPanel->getIP(), comPanel->getPort());
}

void MainPanel::stopAction() {
    qDebug("MainPanel: stop action");
    tcpObj->stop();
}

void MainPanel::updateAction() {
    QString status;
    if(tcpObj->started()) {
        status = "status: started in address: " + comPanel->getIP() +
                                    ":" + comPanel->getQPort();
        if(token.isValid()){
            status += ", busy(" + token.getKey() + ")";
        }
        setIcon(token.isValid()? ICON_HIDED: ICON_CONNECTED);
    } else if(tcpObj->starting()) {
        status = "status: try starting in address: " + comPanel->getIP() +
                             ":" + comPanel->getQPort();
    } else {
        setIcon(ICON_DISCONNECTED);
        status = tcpObj->failed() ? "status: failed" : "status: stopped";
    }
    statusLabel->setText(status);
    comPanel->lock(!tcpObj->stopped());
    startButton->setEnabled(tcpObj->stopped());
    stopButton->setEnabled(!tcpObj->stopped());
}

void MainPanel::newTCPexchanger(SOCKET acceptSocket) {
    auto tcpExchanger = new TCPexchanger(acceptSocket);
    QObject::connect(tcpExchanger, &TCPexchanger::newToken,
                     this, &MainPanel::newToken, Qt::DirectConnection);

    QObject::connect(tcpExchanger, &TCPexchanger::freeDone,
                     this, &MainPanel::freeDone);

    QObject::connect(tcpExchanger, &TCPexchanger::deleteTCPexchanger,
                     this, &MainPanel::deleteTCPexchanger, Qt::DirectConnection);

    QObject::connect(this, &MainPanel::tokenAccepted,
                     tcpExchanger, &TCPexchanger::tokenAccepted);

    QObject::connect(this, &MainPanel::hiderBusy,
                     tcpExchanger, &TCPexchanger::hiderBusy);

    QObject::connect(tcpExchanger, &TCPexchanger::notAccepted,
                     this, &MainPanel::resetTokenAndShowWindow);

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
        qDebug("MainPanel: keyboard/mouse action with new token");
        changeWindowVisible(true);
    }
}

void MainPanel::newToken(const Token& newToken, TCPexchanger *const sender) {
    qDebug("MainPanel: newToken");
    if(token.isValid()){
        qDebug("MainPanel: emit busy");
        emit hiderBusy();
        return;
    }
    token = newToken;
    qDebug("MainPanel: emit token accepted");
    emit tokenAccepted(sender);
    changeWindowVisible(false);
}

void MainPanel::deleteTCPexchanger(TCPexchanger *const tcpExchager) {
    clientListMutex.lock();
    bool accepted = tcpExchager->accepted();
    clientList->erase(clientList->constFind(tcpExchager));
    clientListMutex.unlock();
    if(accepted){
        resetTokenAndShowWindow();
    }
    qDebug("MainPanel: clientList size = %d", clientList->size());
}

void MainPanel::tcpObjStop() {
    resetTokenAndShowWindow();
}

void MainPanel::freeClient() {
    clientListMutex.lock();
    for(auto item: *clientList){
        item->freeClient();
    }
    clientListMutex.unlock();
}

void MainPanel::resetTokenAndShowWindow() {
    token.setValid(false);
    changeWindowVisible(true);
}
