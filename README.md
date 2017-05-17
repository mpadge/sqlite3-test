[![Build Status](https://travis-ci.org/mpadge/sqlite3-test.svg)](https://travis-ci.org/mpadge/sqlite3-test) 
[![Build status](https://ci.appveyor.com/api/projects/status/github/mpadge/sqlite3-test?svg=true)](https://ci.appveyor.com/project/mpadge/sqlite3-test) 

# sqlite3-test

Repo just for testing and resolving `SQLite3` problem in 
[`bikedata` repo](https://github.com/mpadge/bikedata.git).

Current problems:

1. ~~Travis fails because of hash sum mismatch for 
[specified `sqlite3` version](https://launchpad.net/~travis-ci/+archive/ubuntu/sqlite3).~~
   Fixed by switching to `trusty` build instead of `precise`, for which default
   sqlite (3.8.2 instead of 3.7.9) is sufficient.
2. Appveyor fails because it can't find the `sqlite3.h` headers (the
   [`win-builder.r-project` machines](https://win-builder.r-project.org/) in
   contrast have no such issues).
