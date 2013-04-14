#ifndef DATABASEMANAGER_H_
#define DATABASEMANAGER_H_

#include "Database.h"

#include <string>
namespace Database
{

class DatabaseManager
{
public:
    static void InitInstance()
    {
        if (instance == nullptr)
            instance = new DatabaseManager();
    }
    static DatabaseManager* Instance();

    /// Connexion a MySql
    void connect();

    /// Execution d'une requête MySql (Update/Insert/Delete)
    void execute(std::string request);
    /// Execution d'une requête MySql (Select)
    sql::ResultSet* querry(std::string request);

    /// Execution d'une requête MySqk préparé (Update/Insert/Delete)
    void execute(sql::PreparedStatement *pstmt);
    /// Execution d'une requête MySqk préparé (Select)
    sql::ResultSet* querry(sql::PreparedStatement *pstmt);


private:
    DatabaseManager();
    static DatabaseManager* instance;
    virtual ~DatabaseManager();

    /// Driver MySql
    sql::Driver* driver;
    /// Connection MySql
    sql::Connection* con;

    sql::Statement* stmt;
};

} /* namespace Database */
#endif /* DATABASEMANAGER_H_ */
