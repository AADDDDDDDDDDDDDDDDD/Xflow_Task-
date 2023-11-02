#include <sqlite3.h>
#include <iostream>

bool initialize_db(sqlite3* &db) {
    // Open the database
    if (sqlite3_open("sip_data.db", &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Create the table
    char *errMsg;
    const char *sql = "CREATE TABLE IF NOT EXISTS sip_data("
                      "to_field TEXT,"
                      "from_field TEXT,"
                      "call_id TEXT);";

    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool insert_data(sqlite3* db, const std::string& to_field, const std::string& from_field, const std::string& call_id) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO sip_data(to_field, from_field, call_id) VALUES(?,?,?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, to_field.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, from_field.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, call_id.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting data: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

void retrieve_and_display_data(sqlite3* db) {
    const char* sql = "SELECT * FROM sip_data;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Error preparing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << "to_field: " << sqlite3_column_text(stmt, 0) << ", ";
        std::cout << "from_field: " << sqlite3_column_text(stmt, 1) << ", ";
        std::cout << "call_id: " << sqlite3_column_text(stmt, 2) << std::endl;
    }
    
    sqlite3_finalize(stmt);
}

void close_db(sqlite3* db) {
    sqlite3_close(db);
}

int main() {
    sqlite3* db;
    if (!initialize_db(db)) {
        return 1; // Exit with an error code
    }

    // Insert data into the database
    if (!insert_data(db, "TestTo", "TestFrom", "TestCallID")) {
        close_db(db);
        return 1; // Exit with an error code
    }

    // Retrieve and display data
    retrieve_and_display_data(db);

    // Close the database
    close_db(db);

    return 0; // Exit with a success code
}
