#pragma once
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QRegularExpression>
#include <unordered_map>

QString curDateTime();
QString logTimeStamp();

class Settings
{
public:
	Settings();
	~Settings();
    const QString& get(const QString& name);
private:
	void saveConfig();
    QString& getValue(QString& str);
    std::unordered_map<QString, QString> conf;
    QString delim_settings = " = ";
#if defined (_WIN32) || defined (_WIN64)
    QString cfgPath = "settings.ini";
#elif defined (__linux__)
    QString cfgPath = "./settings.ini";
#endif
};
