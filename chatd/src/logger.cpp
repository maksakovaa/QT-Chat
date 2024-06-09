#include "logger.h"
#include "settings.h"
#include <fstream>
#include <iostream>

void Logger::recLogEntry(const QString& entry)
{
    std::fstream log;
    log.open(log_file.toStdString(), std::ios::in | std::ios::app);
    QString temp = logTimeStamp();
    temp += entry;
    log << temp.toStdString();
    log << "\n";
    log.close();
    emit appendLogEntry(temp);
}
