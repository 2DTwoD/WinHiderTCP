#include "panels/com_panel.h"

#include <QLabel>
#include <QRegularExpression>
#include <QHBoxLayout>
#include <QRegularExpressionValidator>

ComPanel::ComPanel(QWidget *parent): QFrame(parent), fileWork(new FileWork(this)) {
    auto layout = new QHBoxLayout(this);
    this->setLayout(layout);

    auto ipLabel = new QLabel("IP:", this);
    auto portLabel = new QLabel("Port:", this);
    ipLineEdit = new QLineEdit("0.0.0.0", this);
    portLineEdit = new QLineEdit("55555", this);
    portLineEdit->setFixedWidth(40);
    autoStartCheckBox = new QCheckBox("Autostart", this);

    QRegularExpression ipRE(R"(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.){3}(25[0-5]|(2[0-4]|1\d|[1-9]|)\d)$)");
    auto ipValidator = new QRegularExpressionValidator(ipRE, this);
    ipLineEdit->setValidator(ipValidator);
    ipLineEdit->setMaxLength(15);

    QRegularExpression portRE("\\d+");
    auto portValidator = new QRegularExpressionValidator(portRE, this);
    portLineEdit->setValidator(portValidator);
    portLineEdit->setMaxLength(5);

    layout->addWidget(ipLabel);
    layout->addWidget(ipLineEdit);
    layout->addWidget(portLabel);
    layout->addWidget(portLineEdit);
    layout->addWidget(autoStartCheckBox);
    readConfig();
}

char *ComPanel::getIP() {
    auto list = ipLineEdit->text().split('.');
    bool ok = true;
    if(list.size() != 4) {
        ok = false;
    }
    for(auto item: list){
        if(!ok) {
            break;
        }
        item.toUShort(&ok);
    }
    if(!ok) ipLineEdit->setText("127.0.0.1");

    return ipLineEdit->text().toLocal8Bit().data();
}

uint16_t ComPanel::getPort() {
    uint16_t res = portLineEdit->text().toUShort();
    if(!res) res = 55555;
    portLineEdit->setText(QString::number(res));
    return res;
}

QString ComPanel::getQIP() {
    return ipLineEdit->text();
}

QString ComPanel::getQPort() {
    return portLineEdit->text();
}

uint16_t ComPanel::isAutostart() {
    return autoStartCheckBox->isChecked();
}

void ComPanel::readConfig() {
    auto list = fileWork->readConfig().split(";");
    for(auto item: list){
        auto keyValue = item.split(":");
        if(keyValue.size() != 2) break;
        if(keyValue[0] == "ip"){
            ipLineEdit->setText(keyValue[1]);
        } else if(keyValue[0] == "port"){
            portLineEdit->setText(keyValue[1]);
        } else if(keyValue[0] == "autostart"){
            autoStartCheckBox->setChecked(keyValue[1].toUInt() > 0);
        } else {
            break;
        }
    }
    getIP();
    getPort();
}

void ComPanel::saveConfig() {
    fileWork->saveConfig(std::move("ip:" + getQIP() + ";port:" + getQPort() +
    ";autostart:" + QString::number(isAutostart())));
}

ComPanel::~ComPanel() {
    qDebug("ComPanel: destructor");
}

void ComPanel::lock(bool value) {
    ipLineEdit->setEnabled(!value);
    portLineEdit->setEnabled(!value);
}
