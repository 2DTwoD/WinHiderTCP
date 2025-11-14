#include <QVBoxLayout>
#include <iostream>
#include "panels/main_panel.h"

MainPanel::MainPanel(QWidget *parent) : QMainWindow(parent) {
    this->setWindowTitle("WinHider TCP server");
    this->resize(300, 100);

    tcpObj = new TCPobj();
    updater = new Updater();

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
    QObject::connect(updater, &Updater::update, this, &MainPanel::updateAction);

    this->setCentralWidget(mainFrame);
    this->show();
}

MainPanel::~MainPanel() {
    std::cout << "MainPanel destructor" << std::endl;
    tcpObj->shutdown();
    updater->shutdown();
}

void MainPanel::startAction() {
    std::cout << comPanel->getIP() << ":" << comPanel->getPort() << std::endl;
    tcpObj->start(comPanel->getIP(), comPanel->getPort());
}

void MainPanel::stopAction() {
    tcpObj->stop();
}

void MainPanel::updateAction() {
    if (tcpObj->started()) {
        comPanel->lock();
        QString str = "started, address: " + QString::fromLocal8Bit(comPanel->getIP()) +
                                       ":" + QString::number(comPanel->getPort());
        statusLabel->setText(str);
    } else {
        comPanel->unlock();
        statusLabel->setText("stopped");
    }
    startButton->setEnabled(!tcpObj->started());
    stopButton->setEnabled(tcpObj->started());
}
