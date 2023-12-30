#include <pqxx/pqxx>

class Database {
   public:
    inline static pqxx::connection conn{
        "dbname=projects user=postgres password=postgres hostaddr="};
    inline void create_table() {
        pqxx::work txn {conn};
        txn.exec(""
                 "CREATE TABLE IF NOT EXISTS accounts ("
                 "  username    text,"
                 "  password    text"
                 ")");
        txn.exec(""
                 "CREATE TABLE IF NOT EXISTS todos ("
                 "  title       text,"
                 "  status      text"
                 ")");
        txn.commit();
    }
};
