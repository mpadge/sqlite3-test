context ("store data in db")

require (testthat)

test_that ('read data', {
               data_dir <- getwd ()
               expect_equal (length (list.files (data_dir, pattern = '.zip')), 0)
               bike_write_test_data (data_dir = data_dir)
               bikedb <- file.path (getwd (), "testdb")
               expect_true (!file.exists (bikedb))
               nt <- store_bikedata (data_dir = data_dir, bikedb = bikedb)
               expect_equal (nt, 1198)

               expect_true (file.remove ("testdb"))
               expect_equal (length (list.files (data_dir, pattern = '.zip')), 6)
               expect_equal (bike_rm_test_data (data_dir = data_dir), 6)
})
