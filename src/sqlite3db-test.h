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
#include <string>
#include <vector>
#include <map>
#include <sqlite3.h>

#include <curl/curl.h>


#define BUFFER_SIZE 512

int rcpp_create_sqlite3_db (const char * bikedb);
int rcpp_import_stn_df (const char * bikedb, Rcpp::DataFrame stn_data,
        std::string city);
int get_stn_table_size (sqlite3 * dbcon);


