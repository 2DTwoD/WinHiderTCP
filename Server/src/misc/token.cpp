#include "misc/token.h"

Token::Token(): valid(false), key(""), wname("") {
}

Token::Token(bool valid): Token() {
    setValid(valid);
}


Token::Token(Token &tokenObject): valid(tokenObject.valid), key(tokenObject.key),
                                  wname(tokenObject.wname){
}

Token::Token(Token &&tokenObject) noexcept: valid(tokenObject.valid), key(tokenObject.key),
                                            wname(std::move(tokenObject.wname)) {
}

Token &Token::operator=(const Token &tokenObject) {
    mutex.lock();
    valid = tokenObject.valid;
    key = tokenObject.key;
    wname = tokenObject.wname;
    mutex.unlock();
    return *this;
}

Token &Token::operator=(Token &&tokenObject) noexcept {
    mutex.lock();
    valid = tokenObject.valid;
    key = std::move(tokenObject.key);
    wname = std::move(tokenObject.wname);
    mutex.unlock();
    return *this;
}

void Token::setValid(bool value) {
    mutex.lock();
    valid = value;
    mutex.unlock();
}

void Token::setKey(QString&& value) {
    mutex.lock();
    key = std::move(value);
    mutex.unlock();
}

void Token::setWName(QString&& value) {
    mutex.lock();
    wname = std::move(value);
    mutex.unlock();
}

bool Token::isValid() {
    mutex.lock();
    auto result = valid;
    mutex.unlock();
    return result;
}

QString Token::getKey() {
    mutex.lock();
    auto result = key;
    mutex.unlock();
    return result;
}

QString Token::getName() {
    mutex.lock();
    auto result = wname;
    mutex.unlock();
    return result;
}

void Token::print() {
    qDebug("Token: print: valid: %d, key: %d, wname: %s",
           isValid(), getKey().toUtf8().data(), getName().toUtf8().data());
}

Token::~Token() {
    qDebug("Token: destructor");
}
