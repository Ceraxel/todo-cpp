#include <pqxx/pqxx>
#include <iostream>

class Database {
   public:
    inline static pqxx::connection conn{
        "dbname=project user=postgres password=postgres hostaddr="};
    inline void create_table() {
            pqxx::work txn {conn};
            txn.exec("CREATE EXTENSION IF NOT EXISTS \"uuid-ossp\"");
            txn.exec(""
                     "CREATE TABLE IF NOT EXISTS accounts ("
                     "  username    text,"
                     "  password    text"
                     ")");
            txn.exec(""
                     "CREATE TABLE IF NOT EXISTS todos ("
                     "  id          uuid    DEFAULT     uuid_generate_v4 (),"
                     "  username    text NOT NULL,"
                     "  title       text NOT NULL,"
                     "  status      text NOT NULL,"
                     "  PRIMARY     KEY    (id)"
                     ")");
            txn.commit();
        std::cout << "Tables created\n";
    }
};
