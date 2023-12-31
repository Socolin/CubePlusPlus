#ifndef DATABASEMANAGER_H_
#define DATABASEMANAGER_H_

#include "Util/Singleton.h"

#include "Database.h"

#include <string>
namespace Database
{

class DatabaseManager : public Util::Singleton<DatabaseManager>
{
    friend Util::Singleton<DatabaseManager>;
    DatabaseManager();
public:
    virtual ~DatabaseManager();

    /// Connexion a MySql
    bool connect();

    /// Execution d'une requête MySql (Update/Insert/Delete)
    void execute(std::string request);
    /// Execution d'une requête MySql (Select)
    sql::ResultSet* querry(std::string request);

    /// Execution d'une requête MySqk préparé (Update/Insert/Delete)
    void execute(sql::PreparedStatement *pstmt);
    /// Execution d'une requête MySqk préparé (Select)
    sql::ResultSet* querry(sql::PreparedStatement *pstmt);


private:
    static DatabaseManager* instance;

    /// Driver MySql
    sql::Driver* driver;
    /// Connection MySql
    sql::Connection* con;

    sql::Statement* stmt;

    std::string address;
    std::string username;
    std::string password;
    std::string schema;
};

} /* namespace Database */
#endif /* DATABASEMANAGER_H_ */
