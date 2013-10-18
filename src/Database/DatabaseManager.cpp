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

}

bool DatabaseManager::connect()
{
    if (driver != nullptr) // TODO change this, connect only if needed
        return false;
    driver = get_driver_instance();

    Config::Config::getConfig().lookupValue("server.database.address", address);
    Config::Config::getConfig().lookupValue("server.database.username", username);
    Config::Config::getConfig().lookupValue("server.database.password", password);
    Config::Config::getConfig().lookupValue("server.database.schema", schema);

    try
    {
        /* create a database connection using the Driver */
        con = driver->connect(address, username, password);

        /* alternate syntax using auto_ptr to create the db connection */
        //auto_ptr  con (driver -> connect(url, user, password));
        /* turn off the autocommit */
        //    con->setAutoCommit(0);

        //    std::cout << "\nDatabase connection\'s autocommit mode = " << con->getAutoCommit() << std::endl;

        /* select appropriate database schema */
        con->setSchema(schema);

        /* create a statement object */
        stmt = con->createStatement();
        return true;
    }
    catch (sql::SQLException &e)
    {
        LOG_ERROR << "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )" << std::endl;
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
        LOG_ERROR << "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )" << std::endl;
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
        LOG_ERROR << "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )" << std::endl;
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
        LOG_ERROR << "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )" << std::endl;
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
        LOG_ERROR << "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
    return nullptr;
}

} /* namespace Database */
