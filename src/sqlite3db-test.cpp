/***************************************************************************
 *  Project:    bikedata
 *  File:       splite3db-admin.cpp
 *  Language:   C++
 *
 *  Author:     Mark Padgham 
 *  E-Mail:     mark.padgham@email.com 
 *
 *  Description:    Routines to construct sqlite3 database and associated
 *  indexes. Routines to store and add data are in 'sqlite3db-add-data.h'
 *
 *  Compiler Options:   -std=c++11
 ***************************************************************************/


#include <Rcpp.h>

#include "sqlite3db-test.h"
#include "utils.h"


//' rcpp_create_sqlite3_db
//'
//' Initial creation of SQLite3 database
//' 
//' @param bikedb A string containing the path to the Sqlite3 database to 
//'        be created.
//'
//' @return integer result code
//'
//' @noRd
// [[Rcpp::export]]
int rcpp_create_sqlite3_db (const char * bikedb)
{
    sqlite3 *dbcon;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open_v2(bikedb, &dbcon, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK)
        throw std::runtime_error ("Can't establish sqlite3 connection");

    rc = sqlite3_exec(dbcon, "SELECT InitSpatialMetadata(1);", NULL, NULL, &zErrMsg);

    // NOTE: Database structure is ordered according to the order of the NYC
    // citibike system, so each line of data from that city can be injected
    // straight into the db. All other cities require re-ordering of data to
    // this citibike sequence prior to injection into db.

    std::string createqry = "CREATE TABLE stations ("
        "    id integer primary key,"
        "    city text,"
        "    stn_id text,"
        "    name text,"
        "    longitude numeric,"
        "    latitude numeric,"
        "    UNIQUE (stn_id, name)"
        ");";

    rc = sqlite3_exec(dbcon, createqry.c_str(), NULL, NULL, &zErrMsg);

    rc = sqlite3_close_v2(dbcon);
    if (rc != SQLITE_OK)
        throw std::runtime_error ("Unable to close sqlite database");

    return rc;
}


//' rcpp_import_stn_df
//'
//' Import a data.frame of station (id, name, lon, lat) into the SQLite3
//' database. Used for London and Chicago, for both of which stations are loaded
//' within R and passed to this function.
//'
//' @param dbcon Active connection to sqlite3 database
//' @param stn_data An R DataFrame of (id, name, lon, lat) for all stations
//'
//' @return Number of stations to potentially be added to stations table (if not
//'         already there).
//'
//' @noRd
// [[Rcpp::export]]
int rcpp_import_stn_df (const char * bikedb, Rcpp::DataFrame stn_data,
        std::string city)
{
    sqlite3 *dbcon;
    char *zErrMsg = 0;

    int rc = sqlite3_open_v2 (bikedb, &dbcon, SQLITE_OPEN_READWRITE, NULL);
    if (rc != SQLITE_OK)
        throw std::runtime_error ("Can't establish sqlite3 connection");

    std::string stationqry = "INSERT OR IGNORE INTO stations "
        "(city, stn_id, name, longitude, latitude) VALUES ";

    int num_stns_old = get_stn_table_size (dbcon);

    Rcpp::CharacterVector stn_id = stn_data ["id"];
    Rcpp::CharacterVector stn_name = stn_data ["name"];
    Rcpp::CharacterVector stn_lon = stn_data ["lon"];
    Rcpp::CharacterVector stn_lat = stn_data ["lat"];

    /*
    for (unsigned i = 0; i<stn_data.nrow (); i++)
    {
        stationqry += "(\'" + city + "\',\'" + city + stn_id (i) + "\',\'" + 
            stn_name (i) + "\'," + stn_lon (i) + "," + stn_lat (i) + ")";
        if (i < (stn_data.nrow () - 1))
            stationqry += ",";
    }
    stationqry += ";";
    */

    for (unsigned i = 0; i < 10; i++)
    {
        stationqry += "(\'" + city + "\',\'" + city + stn_id (i) + "\',\'" + 
            stn_name (i) + "\'," + stn_lon (i) + "," + stn_lat (i) + ")";
        if (i < 9)
            stationqry += ",";
    }
    stationqry += ";";

    rc = sqlite3_exec(dbcon, stationqry.c_str(), NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        std::string msg = "Unable to insert stations for " + city;
        throw std::runtime_error (msg);
    }

    int num_stns_added = get_stn_table_size (dbcon) - num_stns_old;

    rc = sqlite3_close_v2(dbcon);
    if (rc != SQLITE_OK)
        throw std::runtime_error ("Unable to close sqlite database");

    return num_stns_added;
}


//' get_stn_table_size
//'
//' @param dbcon Active connection to sqlite3 database
//'
//' @return Number of stations in table
//'
//' @noRd
int get_stn_table_size (sqlite3 * dbcon)
{
    sqlite3_stmt * stmt;
    char qry_id [BUFFER_SIZE];
    sprintf(qry_id, "SELECT COUNT(*) FROM stations");
    int rc = sqlite3_prepare_v2(dbcon, qry_id, BUFFER_SIZE, &stmt, NULL);
    rc = sqlite3_step (stmt);
    int num_stns = sqlite3_column_int (stmt, 0);
    sqlite3_reset(stmt);

    return num_stns;
}
