#ifndef LOGGER_H
#define LOGGER_H

#include <QtCore/QString>

class Logger
{
public:
    static void Init(bool debug);
    static void LogDebug(const QString& message);

private:
    static bool m_debug;
};

#endif // LOGGER_H
