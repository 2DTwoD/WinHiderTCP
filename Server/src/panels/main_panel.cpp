#include "panels/main_panel.h"

#include <QVBoxLayout>

#define UPDATER_TIME_MS 500

MainPanel::MainPanel(QWidget *parent) : QMainWindow(parent), updateTimer(new QTimer(parent)),
                                        tcpObj(new TCPobj(this)) {
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


    this->setCentralWidget(mainFrame);
    this->show();
    if(comPanel->isAutostart()){
        startAction();
    }
    updateTimer->start(UPDATER_TIME_MS);
}

MainPanel::~MainPanel() {
    qDebug("MainPanel: destructor");
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
    if (tcpObj->started()) {
        comPanel->lock();
        QString str = "status: started, address: " + QString::fromLocal8Bit(comPanel->getIP()) +
                                       ":" + QString::number(comPanel->getPort());
        statusLabel->setText(str);
    } else {
        comPanel->unlock();
        statusLabel->setText(tcpObj->failed()? "status: failed": "status: stopped");
    }
    startButton->setEnabled(!tcpObj->started());
    stopButton->setEnabled(tcpObj->started());
}

