#ifndef WIN_WORK_H
#define WIN_WORK_H
#include <windows.h>
#include <string>
#include <QString>

void toLower(std::string& str);

void changeWindowVisible(const QString& winName, bool visible);

#endif //WIN_WORK_H
