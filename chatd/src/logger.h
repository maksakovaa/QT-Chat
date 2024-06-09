#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
class Logger : public QObject
{
    Q_OBJECT

public:
    Logger() = default;
    ~Logger() = default;
public slots:
    void recLogEntry(const QString& entry);
signals:
    void appendLogEntry(const QString& entry);
private:
#if defined (_WIN32) || defined (_WIN64)
    QString log_file = "log.txt";
#elif defined (__linux__)
    QString log_file = "./log.txt";
#endif
};

#endif // LOGGER_H
