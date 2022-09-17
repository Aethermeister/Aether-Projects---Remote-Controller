#include "logger.h"

#include <QtCore/QDebug>

bool Logger::m_debug = false;

void Logger::Init(bool debug)
{
    m_debug = debug;
}

void Logger::LogDebug(const QString &message)
{
    if(m_debug)
    {
        qDebug() << message;
    }
}
