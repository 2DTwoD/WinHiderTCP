#ifndef TOKEN_H
#define TOKEN_H

#include <QString>
#include <QMutex>

#include <winsock2.h>


class Token{
private:
    QMutex mutex;
    bool valid;
    WPARAM key;
    QString wname;
public:
    Token();
    Token(bool isValid);
    Token(Token& tokenObject);
    Token(Token&& tokenObject) noexcept;
    Token& operator=(const Token& tokenObject);
    Token& operator=(Token&& tokenObject) noexcept ;
    void setValid(bool value);
    void setKey(WPARAM value);
    void setWName(QString &&value);
    void setToken(bool valid, WPARAM key, QString &&wname);
    bool isValid();
    WPARAM getKey();
    QString getName();
    void print();
};

#endif //TOKEN_H
