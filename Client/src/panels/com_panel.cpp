#include "panels/com_panel.h"

#include <QLabel>
#include <QRegularExpression>
#include <QHBoxLayout>
#include <QRegularExpressionValidator>

ComPanel::ComPanel(QWidget *parent): QFrame(parent) {
    auto layout = new QHBoxLayout(this);
    this->setLayout(layout);

    auto ipLabel = new QLabel("IP:", this);
    auto portLabel = new QLabel("Port:", this);
    ipLineEdit = new QLineEdit("127.0.0.1", this);
    portLineEdit = new QLineEdit("55555", this);
    portLineEdit->setFixedWidth(40);
    autoStartCheckBox = new QCheckBox("Autostart", this);

    QRegularExpression ipRE(R"(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.){3}(25[0-5]|(2[0-4]|1\d|[1-9]|)\d)$)");
    auto ipValidator = new QRegularExpressionValidator(ipRE, this);
    ipLineEdit->setValidator(ipValidator);

    QRegularExpression portRE("\\d+");
    auto portValidator = new QRegularExpressionValidator(portRE, this);
    portLineEdit->setValidator(portValidator);

    layout->addWidget(ipLabel);
    layout->addWidget(ipLineEdit);
    layout->addWidget(portLabel);
    layout->addWidget(portLineEdit);
    layout->addWidget(autoStartCheckBox);
}

void ComPanel::lock(bool lockFlag) {
    ipLineEdit->setEnabled(!lockFlag);
    portLineEdit->setEnabled(!lockFlag);
}

void ComPanel::checkIP() {
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
}

void ComPanel::checkPort() {
    uint16_t res = portLineEdit->text().toUShort();
    if(!res) res = 55555;
    portLineEdit->setText(QString::number(res));
}

QString ComPanel::getIP() {
    checkIP();
    return ipLineEdit->text();
}

uint16_t ComPanel::getPort() {
    checkPort();
    return portLineEdit->text().toUShort();
}

QString ComPanel::getQPort() {
    return QString::number(getPort());
}

void ComPanel::setIP(const QString& value) {
    ipLineEdit->setText(value);
    checkIP();
}

void ComPanel::setPort(const QString& value) {
    portLineEdit->setText(value);
    checkPort();
}

uint16_t ComPanel::isAutostart() {
    return autoStartCheckBox->isChecked();
}

ComPanel::~ComPanel() {
    qDebug("ComPanel: destructor");
}

void ComPanel::lockAutoStart(bool lockFlag) {
    autoStartCheckBox->setEnabled(!lockFlag);
}

void ComPanel::setAutostart(const QString& value) {
    autoStartCheckBox->setChecked(value.toUInt() > 0);
}