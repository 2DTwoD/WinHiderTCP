#ifndef TOKEN_H
#define TOKEN_H

#include <QString>

#include <winsock2.h>

struct Token{
    Token();
    Token(Token& tokenObject);
    Token(Token&& tokenObject) noexcept;
    Token& operator=(Token& tokenObject);
    Token& operator=(Token&& tokenObject);
    bool isValid;
    WPARAM key;
    QString wname;
};

#endif //TOKEN_H
