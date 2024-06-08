#include "logger.h"
#include "settings.h"
Logger::Logger()
{
    file.setFileName(log_file);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    slog.setDevice(&file);
}

Logger::~Logger()
{
    file.close();
}

void Logger::recLogEntry(const QString& entry)
{
    QString temp = logTimeStamp();
    temp += entry;
    slog << temp;
    slog << "\n";
    emit appendLogEntry(temp);
}
