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

#include <string>
#include <vector>
#include <map>

#define BUFFER_SIZE 512

// [[Rcpp::depends(BH)]]
#include <Rcpp.h>
#include "utils.h"

// from sqlite3db-add-data.h
#include "read-station-files.h"
#include "sqlite3db-utils.h"


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
