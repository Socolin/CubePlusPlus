#include "DatabaseManager.h"

#include "Logging/Logger.h"
#include "Config/Config.h"

namespace Database
{

DatabaseManager* DatabaseManager::instance = nullptr;
DatabaseManager* DatabaseManager::Instance()
{
    return instance;
}

DatabaseManager::DatabaseManager()
    : driver(nullptr)
    , con(nullptr)
    , stmt(nullptr)
{
}

DatabaseManager::~DatabaseManager()
{
    delete con;
    delete stmt;
}

bool DatabaseManager::connect()
{
    if (driver != nullptr) // TODO change this, connect only if needed
        return false;
    driver = get_driver_instance();

    Config::Config::GetConfig().lookupValue("server.database.address", address);
    Config::Config::GetConfig().lookupValue("server.database.username", username);
    Config::Config::GetConfig().lookupValue("server.database.password", password);
    Config::Config::GetConfig().lookupValue("server.database.schema", schema);

    try
    {
        /* create a database connection using the Driver */
        con = driver->connect(address, username, password);

        bool autoreconnect = true;
        con->setClientOption("OPT_RECONNECT", &autoreconnect);

        /* select appropriate database schema */
        con->setSchema(schema);

        /* create a statement object */
        stmt = con->createStatement();
        return true;
    }
    catch (sql::SQLException &e)
    {
        LOG_ERROR << "[SQL]" << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
    return false;
}

void DatabaseManager::execute(std::string request)
{
    try
    {
        stmt->execute(request);
    }
    catch (sql::SQLException &e)
    {
        LOG_ERROR << "[SQL]" << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
}

sql::ResultSet* DatabaseManager::querry(std::string request)
{
    try
    {
        return stmt->executeQuery(request);
    }
    catch (sql::SQLException &e)
    {
        LOG_ERROR << "[SQL]" << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
    return nullptr;
}

void DatabaseManager::execute(sql::PreparedStatement* pstmt)
{
    try
    {
        pstmt->execute();
    }
    catch (sql::SQLException &e)
    {
        LOG_ERROR << "[SQL]" << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
}

sql::ResultSet* DatabaseManager::querry(sql::PreparedStatement* pstmt)
{
    try
    {
        return pstmt->executeQuery();
    }
    catch (sql::SQLException &e)
    {
        LOG_ERROR << "[SQL]" << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
    return nullptr;
}

} /* namespace Database */
