# if you want to see what happens in more detail
#SOURCEY_VERBOSE=1

# if you want to force sourcey to rebuild everything
#SOURCEY_REBUILD=1

# create a copy of perl
buildPerl 5.24.0

# and this one is entirely unrelated, and just here
# to show how to build a library. we do not actually need
# it for the example code in app-simple.pl to work
#buildAuto https://ftp.postgresql.org/pub/source/v9.5.1/postgresql-9.5.1.tar.bz2

# build the Dacner2 perl module
# this calles cpanm internally ... 
buildPerlModule Dancer2
