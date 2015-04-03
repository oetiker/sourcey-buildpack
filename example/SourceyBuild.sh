# if you want to see what happens in more detail
#SOURCEY_VERBOSE=1

# if you want to force sourcey to rebuild everything
#SOURCEY_REBUILD=1

# create a copy of perl
buildPerl 5.20.2

# and this one is entirely unrelated, and just here
# to show how to build a library. we do not actually need
# it for the example code in app-simple.pl to work
buildAuto https://ftp.postgresql.org/pub/source/v9.4.1/postgresql-9.4.1.tar.bz2

# build the Mojolicious perl module
# this calles cpanm internally ... 
buildPerlModule Mojolicious Mojo::Pg
