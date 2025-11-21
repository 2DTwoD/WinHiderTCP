#include "panels/main_panel.h"

#include <QVBoxLayout>

MainPanel::MainPanel(QWidget *parent) : QMainWindow(parent),
                                        tcpObj(new TCPobj(this)), updater(new Updater(this)) {
    this->setWindowTitle("WinHider TCP client");
    this->resize(310, 100);
    auto mainFrame = new QFrame(this);
    auto mainLayout = new QVBoxLayout(mainFrame);
    mainFrame->setLayout(mainLayout);

    comPanel = new ComPanel(mainFrame);
    connectButton = new QPushButton("connect", mainFrame);
    disconnectButton = new QPushButton("disconnect", mainFrame);
    statusLabel = new QLabel("status: disconnected", mainFrame);
    statusLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(comPanel);
    mainLayout->addWidget(connectButton);
    mainLayout->addWidget(disconnectButton);
    mainLayout->addWidget(statusLabel);

    QObject::connect(connectButton, &QPushButton::clicked, this, &MainPanel::startAction);
    QObject::connect(disconnectButton, &QPushButton::clicked, this, &MainPanel::stopAction);
    QObject::connect(updater, &Updater::update, this, &MainPanel::updateAction);

    this->setCentralWidget(mainFrame);
    this->show();
    if(comPanel->isAutostart()){
        startAction();
    }
}

MainPanel::~MainPanel() {
    qDebug("MainPanel: destructor");
    tcpObj->shutdown();
    updater->shutdown();
    comPanel->saveConfig();
}

void MainPanel::startAction() {
    qDebug("MainPanel: start action, server with address: %s:%d", comPanel->getIP(), comPanel->getPort());
    //tcpObj->start(comPanel->getIP(), comPanel->getPort());
}

void MainPanel::stopAction() {
    qDebug("MainPanel: stop action");
    //tcpObj->stop();
}

void MainPanel::updateAction() {
    if (tcpObj->started()) {
        comPanel->lock();
        QString str = "status: connected to address: " + QString::fromLocal8Bit(comPanel->getIP()) +
                                       ":" + QString::number(comPanel->getPort());
        statusLabel->setText(str);
    } else {
        comPanel->unlock();
        statusLabel->setText(tcpObj->failed()? "status: failed": "status: disconnected");
    }
    connectButton->setEnabled(!tcpObj->started());
    disconnectButton->setEnabled(tcpObj->started());
}

