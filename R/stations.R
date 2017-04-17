#' Get Chicago station data
#'
#' @param flists List of files returned from bike_unzip_files_chicago 
#'
#' @return \code{data.frame} of (id, name, lon, lat) of all stations in Chicago's
#' Divvybikes system
#'
#' @noRd
bike_get_chicago_stations <- function (flists)
{

    id <- name <- lon <- lat <- NULL
    for (f in flists$flist_csv_stns)
    {
        fi <- read.csv (f, header = TRUE)
        id <- c (id, paste0 (fi$id))
        name <- c (name, paste0 (fi$name))
        lon <- c (lon, paste0 (fi$longitude))
        lat <- c (lat, paste0 (fi$latitude))
    }
    res <- data.frame (id = id, name = name, lon = lon, lat = lat)
    res <- res [which (!duplicated (res)), ]

    return (res)
}
