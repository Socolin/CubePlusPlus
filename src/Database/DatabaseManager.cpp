#include "DatabaseManager.h"

#include "Logging/Logger.h"

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

void DatabaseManager::connect()
{
    if (driver != nullptr) // TODO change this, connect only if needed
        return;
    driver = get_driver_instance();

    /* create a database connection using the Driver */
    con = driver->connect("tcp://127.0.0.1:3306", "mcpp", "mcpp");

    /* alternate syntax using auto_ptr to create the db connection */
    //auto_ptr  con (driver -> connect(url, user, password));
    /* turn off the autocommit */
//    con->setAutoCommit(0);

//    std::cout << "\nDatabase connection\'s autocommit mode = " << con->getAutoCommit() << std::endl;

    /* select appropriate database schema */
    con->setSchema("mcpp");

    /* create a statement object */
    stmt = con->createStatement();
}

void DatabaseManager::execute(std::string request)
{
    try
    {
        stmt->execute(request);
    }
    catch (sql::SQLException &e)
    {
        LOG_ERROR << "# ERR: " << e.what();
        LOG_ERROR << " (MySQL error code: " << e.getErrorCode();
        LOG_ERROR << ", SQLState: " << e.getSQLState() << " )" << std::endl;
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
        LOG_ERROR << "# ERR: " << e.what();
        LOG_ERROR << " (MySQL error code: " << e.getErrorCode();
        LOG_ERROR << ", SQLState: " << e.getSQLState() << " )" << std::endl;
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
        LOG_ERROR << "# ERR: " << e.what();
        LOG_ERROR << " (MySQL error code: " << e.getErrorCode();
        LOG_ERROR << ", SQLState: " << e.getSQLState() << " )" << std::endl;
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
        LOG_ERROR << "# ERR: " << e.what();
        LOG_ERROR << " (MySQL error code: " << e.getErrorCode();
        LOG_ERROR << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
    return nullptr;
}

} /* namespace Database */
