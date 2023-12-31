#include <pqxx/pqxx>
#include <iostream>

class Database {
   public:
    inline static pqxx::connection conn{
        "dbname=project user=postgres password=postgres hostaddr="};
    inline void create_table() {
        pqxx::work txn {conn};
        txn.exec(""
                 "CREATE TABLE IF NOT EXISTS accounts ("
                 "  username    text,"
                 "  password    text"
                 ")");
        txn.exec(""
                 "CREATE TABLE IF NOT EXISTS todos ("
                 "  username    text,"
                 "  title       text,"
                 "  status      text"
                 ")");
        txn.commit();
        std::cout << "Tables created\n";
    }
};
