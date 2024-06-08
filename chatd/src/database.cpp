#include "database.h"

void Database::connect()
{
    getConfig();
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(mysql_srv_ip);
    db.setUserName(mysql_login);
    db.setPassword(mysql_pass);
    bool connect = db.open();
    if(connect)
    {
        emit logUpdated("[MYSQL] " + tr("Connection established!"));
    }
    else
    {
        emit logUpdated("[MYSQL] " + tr("Error connect to MYSQL database") + QString(db.lastError().text()));
    }
    firstRun();
}

void Database::setConfigPtr(Settings* ptr)
{
    Config = ptr;
}

void Database::setUsrBasePtr(UsrBase* ptr)
{
    UserBase = ptr;
}

void Database::setAllMsgBasePtr(MsgBase* ptr)
{
    AllMsgBase = ptr;
}

void Database::setPerMsgBasePtr(MsgBase *ptr)
{
    PerMsgBase = ptr;
}

void Database::getConfig()
{
    mysql_srv_ip = Config->get("mysql_srv_ip");
    mysql_login = Config->get("mysql_login");
    mysql_pass = Config->get("mysql_pass");
    mysql_database = Config->get("mysql_database");
    mysql_table_users = Config->get("mysql_table_users");
    mysql_table_AM = Config->get("mysql_table_AM");
    mysql_table_AMS = Config->get("mysql_table_AMS");
    mysql_table_PM = Config->get("mysql_table_PM");
}

void Database::sendRequest(const QString& reqName)
{
    QSqlQuery query;
    bool result = query.exec(m_query);
    if(result)
    {
        emit logUpdated("[MYSQL] " + reqName);
    }
    else
    {
        emit logUpdated("[MYSQL] " + QString(query.lastError().text()));
    }
    m_query.clear();
    query.finish();
}

void Database::firstRun()
{
    m_query = "CREATE DATABASE IF NOT EXISTS " + mysql_database;
    sendRequest("CREATE DATABASE");

    m_query = "USE " + mysql_database;
    sendRequest("USE DATABASE");

    m_query = "CREATE TABLE IF NOT EXISTS " + mysql_table_users + " "
                "(id SERIAL NOT NULL, uuid BINARY(16) PRIMARY KEY DEFAULT (UUID_TO_BIN(UUID())), "
                "name VARCHAR(100) NOT NULL, "
                "login VARCHAR(100) UNIQUE NOT NULL, "
                "pwd VARCHAR(200), "
                "deleted INTEGER NOT NULL DEFAULT 0)";
    sendRequest("CREATE TABLE USERS");

    m_query = "CREATE TABLE IF NOT EXISTS " + mysql_table_AM +
              " (id SERIAL PRIMARY KEY, "
              "msg_from BINARY(16) NOT NULL REFERENCES " + mysql_table_users + "(uuid), "
              "date DATETIME NOT NULL, "
              "message VARCHAR(255) NOT NULL)";
    sendRequest("CREATE TABLE ALL_MSGBASE");

    m_query = "CREATE TABLE IF NOT EXISTS " + mysql_table_PM +
              " (id SERIAL PRIMARY KEY, "
              "msg_from BINARY(16) NOT NULL REFERENCES " + mysql_table_users + "(uuid), "
              "msg_to BINARY(16) NOT NULL REFERENCES " + mysql_table_users + "(uuid), "
              "date DATETIME NOT NULL, "
              "message VARCHAR(255) NOT NULL, "
              "status INTEGER DEFAULT 0)";
    sendRequest("CREATE TABLE PRIVATE_MSGBASE");

    m_query = "CREATE TABLE IF NOT EXISTS " + mysql_table_AMS + " " +
              "(id SERIAL PRIMARY KEY, " +
              "msg_id integer NOT NULL REFERENCES " + mysql_table_AM + "(id), " +
              "for_user BINARY(16) NOT NULL REFERENCES " + mysql_table_users + "(uuid), " +
              "status INTEGER DEFAULT 0)";
    sendRequest("CREATE TABLE ALL MSG STATUS");

    m_query = "CREATE PROCEDURE IF NOT EXISTS status_on_create_msg(in a INT) "
              "BEGIN "
              "DECLARE i INT DEFAULT 1; "
              "DECLARE user_count INT DEFAULT 0; "
              "DECLARE user_uuid BINARY(16); "
              "SET user_count = (SELECT count(u.id) FROM " + mysql_table_users + " u); "
              "WHILE i <= user_count DO "
              "SET user_uuid = (SELECT u.uuid FROM " + mysql_table_users + " u WHERE u.id = i); "
              "INSERT INTO " + mysql_table_AMS + " (msg_id, for_user) VALUES (a, user_uuid); "
              "SET i = i+1; "
              "END WHILE; "
              "END;";
    sendRequest("CREATE PROCEDURE status_on_create_msg");

    m_query = "CREATE TRIGGER IF NOT EXISTS on_create_msg AFTER INSERT ON "
              + mysql_table_AM + " "
              "FOR EACH ROW "
              "BEGIN "
              "CALL status_on_create_msg( NEW.id ); "
              "END;";
    sendRequest("CREATE TRIGGER on_create_msg");

    m_query = "CREATE PROCEDURE IF NOT EXISTS status_on_create_usr(in new_uuid BINARY(16)) "
              "BEGIN "
              "DECLARE i INT DEFAULT 1; "
              "DECLARE msg_count INT DEFAULT 0; "
              "SET msg_count = (SELECT count(id) FROM " + mysql_table_AM + "); "
              "IF msg_count >= 1 THEN "
              "WHILE i <= msg_count DO "
              "INSERT INTO " + mysql_table_AMS + " (msg_id, for_user) VALUES (i, new_uuid); "
              "SET i = i+1; "
              "END WHILE; "
              "END IF; "
              "END;";
    sendRequest("CREATE PROCEDURE status_on_create_usr");

    m_query = "CREATE TRIGGER IF NOT EXISTS on_create_usr AFTER INSERT ON " +
              mysql_table_users + " "
              "FOR EACH ROW "
              "BEGIN "
              "CALL status_on_create_usr( NEW.uuid ); "
              "END;";
    sendRequest("CREATE TRIGGER on_create_usr");

    getUsrBase();
    getAllMsgBase();
    getPerMsgBase();
}

void Database::getUsrBase()
{
    UserBase->resetBase();
    m_query = "SELECT BIN_TO_UUID(uuid) uuid, name, login, pwd, deleted  FROM " + mysql_table_users + " ORDER BY id";
    QSqlQuery query(m_query);
    while (query.next())
    {
        User newUser(query.value(0).toString(), query.value(1).toString(), query.value(2).toString(), query.value(3).toString(), query.value(4).toInt());
        UserBase->addInBase(newUser);
        emit logUpdated("[MYSQL] " + tr("Added in base user ") + query.value(1).toString());
    }
    if(UserBase->getCount() == 0)
    {
        User newUser("Администратор", "admin", "da23890e111536e631be13a069ebc5432c9cf28cdbc5deb2a70770ec9597db6d");
        regUser(newUser);
    }
    emit usrBaseUpdated();
}

void Database::regUser(User &newUser)
{
    m_query = "INSERT INTO " + mysql_table_users + " "
              "(name, login, pwd) VALUES('" +
              newUser.getName() + "', '" + newUser.getLogin() +
              "', '" + newUser.getPwd() + "')";
    sendRequest("INSERT USER IN USERBASE");
    getUsrBase();
    emit updUserBase();
}

void Database::setUserStatus(const QString& uuid, int newStatus)
{
    m_query = "UPDATE " + mysql_table_users + " "
              "SET deleted = " + QString::number(newStatus) + " "
              "WHERE BIN_TO_UUID(uuid) = '" + uuid + "'";
    sendRequest("CHANGE USER STATUS");
    getUsrBase();
    emit updUserBase();
}

void Database::getAllMsgBase()
{
    logUpdated("[MYSQL] Get main chat messages...");
    AllMsgBase->resetBase();
    m_query = "SELECT m.id, BIN_TO_UUID(m.msg_from), m.date, m.message FROM " +
               mysql_table_AM + " m";
    QSqlQuery query(m_query);
    while (query.next())
    {
        Message newMsg(query.value(0).toInt(),
                       query.value(1).toString(),
                       "ALL", query.value(2).toString(),
                       query.value(3).toString(), 1);
        AllMsgBase->addInBase(newMsg);
        emit logUpdated("[MYSQL] " + tr("Added in base message"));
    }
    if(AllMsgBase->getCount() == 0)
        emit logUpdated("[MYSQL] AllMsgBase is emty");
    emit allMsgBaseUpdated();
}

void Database::getPerMsgBase()
{
    logUpdated("[MYSQL] Get private messages...");
    PerMsgBase->resetBase();
    m_query = "SELECT pm.id, BIN_TO_UUID(pm.msg_from), "
              "BIN_TO_UUID(pm.msg_to), pm.date, pm.message, pm.status "
              "FROM " + mysql_table_PM + " pm "
              "ORDER BY pm.id";
    QSqlQuery query(m_query);
    while (query.next())
    {
        Message newMsg(query.value(0).toInt(),
                       query.value(1).toString(),
                       query.value(2).toString(),
                       query.value(3).toString(),
                       query.value(4).toString(),
                       query.value(5).toInt());;
        PerMsgBase->addInBase(newMsg);
        emit logUpdated("[MYSQL] " + tr("Added in base message"));
    }
    if(PerMsgBase->getCount() == 0)
        emit logUpdated("[MYSQL] PerMsgBase is emty");
    emit allMsgBaseUpdated();
}

void Database::regPrivateMsg(Message &newMsg)
{
    m_query = "INSERT INTO " + mysql_table_PM + " "
              "(msg_from, msg_to, date, message) "
              "VALUES(UUID_TO_BIN('" + newMsg.get_mfrom() + "'),"
              " UUID_TO_BIN('" + newMsg.get_mto() + "'), "
              "'" + newMsg.get_mdate() + "', "
              "'" + newMsg.get_mtext() + "')";
    sendRequest(tr("New private message registered"));
    emit updatePrivateMsgBaseRequired();
}

void Database::regPublicMsg(Message &newMsg)
{
    m_query = "INSERT INTO " + mysql_table_AM + " "
              "(msg_from, date, message) "
              "VALUES(UUID_TO_BIN('" + newMsg.get_mfrom() + "'), "
              "'" + newMsg.get_mdate() + "', "
              "'" + newMsg.get_mtext() + "')";
    sendRequest(tr("New public message registered"));
    emit updatePublicMsgBaseRequired();
}

void Database::getAllMsgBase(const QString& uuid, QTcpSocket* cliSocket)
{
    m_query = "SELECT m.id, BIN_TO_UUID(m.msg_from), m.date, m.message, ams.status "
              "FROM " + mysql_table_AM + " m "
              "JOIN " + mysql_table_AMS + " ams ON m.id = ams.msg_id "
              "WHERE ams.for_user = UUID_TO_BIN('" + uuid + "') "
              "ORDER BY m.id";
    QSqlQuery query(m_query);
    int i = 0;
    while (query.next())
    {
        Message newMsg(query.value(0).toInt(),
                       query.value(1).toString(),
                       "ALL",
                       query.value(2).toString(),
                       query.value(3).toString(),
                       query.value(4).toInt());
        emit sendAMtoCli(newMsg, cliSocket);
        ++i;
    }
    if (i == 0)
    {
        logUpdated("[MYSQL] AllMsgBase is emty");
    }
}

void Database::getPerMsgBase(const QString &uuid, QTcpSocket* cliSocket)
{
    m_query = "SELECT pm.id, BIN_TO_UUID(pm.msg_from), "
              "BIN_TO_UUID(pm.msg_to), pm.date, pm.message, pm.status "
              "FROM " + mysql_table_PM + " pm "
              "WHERE pm.msg_from = UUID_TO_BIN('" + uuid + "') OR pm.msg_to = UUID_TO_BIN('" + uuid + "') "
              "ORDER BY pm.id";
    QSqlQuery query(m_query);
    int i = 0;
    while (query.next())
    {
        Message newMsg(query.value(0).toInt(),
                       query.value(1).toString(),
                       query.value(2).toString(),
                       query.value(3).toString(),
                       query.value(4).toString(),
                       query.value(5).toInt());
        emit sendPMtoCli(newMsg, cliSocket);
        ++i;
    }
    if (i == 0)
    {
        logUpdated("[MYSQL] PerMsgBase is emty");
    }
}
