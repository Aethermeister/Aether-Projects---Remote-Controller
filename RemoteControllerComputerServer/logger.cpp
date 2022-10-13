#include "logger.h"

#include <QtCore/QDebug>
#include <QtCore/QDateTime>

bool Logger::m_debug = false;

void Logger::Init(bool debug)
{
    m_debug = debug;
}

void Logger::LogDebug(const QString &message)
{
    if(m_debug)
    {
        qDebug().noquote() << Timestamp() << message;
    }
}

void Logger::LogInfo(const QString &message)
{
    qDebug().noquote() << Timestamp() << message;
}

QString Logger::Timestamp()
{
    return QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss]");
}
