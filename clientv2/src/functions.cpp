#include "functions.h"
#include <chrono>

bool nonLatinChk(const QString& text)
{
    std::string letters = "абвгдеёжщийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    if(text.toStdString().find_first_of(letters) != std::string::npos)
    {
        return false;
    }
    return true;
}

bool noSpacesChk(const QString& text)
{
    if (text.toStdString().find_first_of(" ") != std::string::npos)
	{
        return false;
	}
	return true;
}

bool lengthChk(const QString& text)
{
	if (text.size() < 4)
	{
		return false;
	}
	return true;
}

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
