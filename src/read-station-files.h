#pragma once
/***************************************************************************
 *  Project:    bikedata
 *  File:       read-station-files.h
 *  Language:   C++
 *
 *  Author:     Mark Padgham 
 *  E-Mail:     mark.padgham@email.com 
 *
 *  Description:    Routines to read and store data on bike docking stations in
 *                  the stations table of the SQLite3 database.
 *
 *  Compiler Options:   -std=c++11
 ***************************************************************************/

#include <unordered_set>
#include <curl/curl.h>

#include "sqlite3db-utils.h"

int import_to_station_table (sqlite3 * dbcon,
    std::map <std::string, std::string> stationqry);
int rcpp_import_stn_df (const char * bikedb, Rcpp::DataFrame stn_data,
        std::string city);


//' import_to_station_table
//'
//' Inserts data into the table of stations in the database. Applies to those
//' cities for which station data are included and read as part of the actual
//' raw trips data: ny, boston, la.
//'
//' @param dbcon Active connection to sqlite3 database
//' @param stationqry Station query constructed during reading of data with
//'        rcpp_import_to_trip_table ()
//'
//' @return integer result code
//'
//' @noRd
int import_to_station_table (sqlite3 * dbcon,
    std::map <std::string, std::string> stationqry)
{
    char *zErrMsg = 0;
    int rc;

    int n = 0;

    // http://stackoverflow.com/questions/19337029/insert-if-not-exists-statement-in-sqlite
    std::string fullstationqry = "INSERT OR IGNORE INTO stations "
        "(city, stn_id, name, latitude, longitude) VALUES ";
    fullstationqry += stationqry.begin ()->second;
    for (auto thisstation = std::next (stationqry.begin ());
            thisstation != stationqry.end (); ++thisstation)
    {
        fullstationqry += ", " + thisstation->second;
    }
    fullstationqry += ";";

    rc = sqlite3_exec(dbcon, fullstationqry.c_str(), NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK)
        throw std::runtime_error ("Unable to insert stations into station table");

    std::string qry = "SELECT AddGeometryColumn"
                      "('stations', 'geom', 4326, 'POINT', 'XY');";
    rc = sqlite3_exec(dbcon, qry.c_str (), NULL, 0, &zErrMsg);

    qry = "UPDATE stations SET geom = MakePoint(longitude, latitude, 4326);";
    rc = sqlite3_exec(dbcon, qry.c_str (), NULL, 0, &zErrMsg);

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

    for (unsigned i = 0; i<stn_data.nrow (); i++)
    {
        stationqry += "(\'" + city + "\',\'" + city + stn_id (i) + "\',\'" + 
            stn_name (i) + "\'," + stn_lon (i) + "," + stn_lat (i) + ")";
        if (i < (stn_data.nrow () - 1))
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
