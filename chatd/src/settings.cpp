#include "settings.h"
#include <chrono>

QString curDateTime()
{
	auto utcTime = std::chrono::system_clock::now();
	auto utcTime2 = std::chrono::floor<std::chrono::seconds>(utcTime);
	auto curTime = std::chrono::zoned_time{ std::chrono::current_zone(), utcTime2 };
    return QString::fromStdString(std::format("{0:%Y.%m.%d} {0:%T}", curTime));
}

QString logTimeStamp()
{
	return "[" + curDateTime() + "] ";
}

Settings::Settings()
{
    QFile file (cfgPath);
    QString str;
    if (file.open(QIODevice::ReadOnly))
	{
        QTextStream stream(&file);
        while (!stream.atEnd())
		{
            str = stream.readLine();
            if (str.startsWith("mysql_server_ip"))
			{
				conf["mysql_srv_ip"] = getValue(str);
			}
            else if (str.startsWith("mysql_login"))
			{
				conf["mysql_login"] = getValue(str);
			}
            else if (str.startsWith("mysql_pass"))
			{
				conf["mysql_pass"] = getValue(str);
			}
            else if (str.startsWith("mysql_database"))
			{
				conf["mysql_database"] = getValue(str);
			}
            else if (str.startsWith("mysql_table_users"))
			{
				conf["mysql_table_users"] = getValue(str);
			}
            else if (str.startsWith("mysql_table_messages"))
			{
				conf["mysql_table_AM"] = getValue(str);
			}
            else if (str.startsWith("mysql_table_all_msg_status"))
			{
				conf["mysql_table_AMS"] = getValue(str);
			}
            else if (str.startsWith("mysql_table_private_msg"))
			{
				conf["mysql_table_PM"] = getValue(str);
			}
            else if (str.startsWith("server_ip"))
			{
				conf["server_ip"] = getValue(str);
			}
            else if (str.startsWith("chat_port"))
			{
				conf["chat_port"] = getValue(str);
			}
		}
        file.close();
	}
	else
	{
        conf["mysql_srv_ip"] = "192.168.0.1";
		conf["mysql_login"] = "root";
		conf["mysql_pass"] = "YrZBQJIijJ9W0mlpqrj4";
		conf["mysql_database"] = "console_chat";
		conf["mysql_table_users"] = "users";
		conf["mysql_table_AM"] = "messages";
		conf["mysql_table_AMS"] = "all_msg_status";
		conf["mysql_table_PM"] = "p_messages";
		conf["server_ip"] = "127.0.0.1";
        conf["chat_port"] = "63540";
		saveConfig();
	}
}

Settings::~Settings()
{
	saveConfig();
}

const QString& Settings::get(const QString& name)
{
	return conf[name];
}

void Settings::saveConfig()
{
    QFile file(cfgPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {

    }
    else
    {
        QTextStream config(&file);
        config << ("mysql_server_ip = " + get("mysql_srv_ip") + "\n");
        config << ("mysql_login = " + get("mysql_login") + "\n");
        config << ("mysql_pass = " + get("mysql_pass") + "\n");
        config << ("mysql_database = " + get("mysql_database") + "\n");
        config << ("mysql_table_users = " + get("mysql_table_users") + "\n");
        config << ("mysql_table_messages = " + get("mysql_table_AM") + "\n");
        config << ("mysql_table_all_msg_status = " + get("mysql_table_AMS") + "\n");
        config << ("mysql_table_private_msg = " + get("mysql_table_PM") + "\n");
        config << ("server_ip = " + get("server_ip") + "\n");
        config << ("chat_port = " + get("chat_port") + "\n");
    }
}

QString& Settings::getValue(QString& str)
{
    return str.remove(QRegularExpression(".*" + delim_settings));
}
