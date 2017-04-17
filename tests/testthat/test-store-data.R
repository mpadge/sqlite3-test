context ("store data in db")

require (testthat)

test_that ('read data', {
               nt <- store_bikedata (data_dir = "..", bikedb = "testdb")
               expect_equal (nt, 200)
               expect_true (file.remove ("testdb"))
})
