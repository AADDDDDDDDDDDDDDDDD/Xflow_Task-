#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <sqlite3.h>
#include <string>

bool initialize_db(sqlite3* &db);
bool insert_data(sqlite3* db, const std::string& to_field, const std::string& from_field, const std::string& call_id);
void close_db(sqlite3* db);

#endif
