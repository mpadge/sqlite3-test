#' Check whether indexes have been created for database
#'
#' @param bikedb A string containing the path to the SQLite3 database to 
#' use. 
#'
#' @noRd
indexes_exist <- function (bikedb)
{
    db <- RSQLite::dbConnect (RSQLite::SQLite(), bikedb, create = FALSE)
    idx_list <- dbGetQuery (db, "PRAGMA index_list (trips)")
    RSQLite::dbDisconnect (db)
    nrow (idx_list) > 2 # 2 because city index is automatically created
}
