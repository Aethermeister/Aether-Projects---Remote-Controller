#ifndef LOGGER_H
#define LOGGER_H

#include <QtCore/QString>

class Logger
{
public:
    static void Init(bool debug);
    static void LogDebug(const QString& message);
    static void LogInfo(const QString& message);

private:
    static bool m_debug;

    static QString Timestamp();
};

#endif // LOGGER_H
