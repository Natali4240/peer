#include <vector>
#include <string_view>
#include <ostream>
using namespace std;

struct Properties {
    string_view db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;

};

struct Filter {
    int min_age, max_age;
    string_view name_filter;
};

string CreateString(const DBHandler& db, const Filter& filter) {
    ostringstream query_str;
    query_str << "from Persons "s
        << "select Name, Age "s
        << "where Age between "s << filter.min_age << " and "s << filter.max_age << " "s
        << "and Name like '%"s << db.Quote(filter.name_filter) << "%'"s;
    return query_str.str();
}

vector<Person> LoadPersons(const Properties& properties, const Filter& filter) {
    DBConnector connector(properties.db_allow_exceptions, properties.db_log_level);
    DBHandler db;
    if (properties.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(properties.db_name, properties.db_connection_timeout);
    }
    else {
        db = connector.Connect(properties.db_name, properties.db_connection_timeout);
    }
    if (!properties.db_allow_exceptions && !db.IsOK()) {
        return {};
    }
    
    DBQuery query(CreateString(db, filter));

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({ move(name), age });
    }
    return persons;
}