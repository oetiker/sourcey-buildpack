#
# Some helpful function for the compile call
#

# make sure we find anything we preinstall
export PATH=$BUILD_DIR$PREFIX/bin:$PATH
# tell pkg-config where to look for *.pc files
export PKG_CONFIG_PATH=$BUILD_DIR$PREFIX/lib/pkgconfig
# just to make sure those who are not compiled propperly still work
export LD_LIBRARY_PATH=$BUILD_DIR$PREFIX/lib
# find our libraries
export LDFLAGS="-Wl,-rpath -Wl,$PREFIX/lib -L$BUILD_DIR$PREFIX/lib"
# find our includes
export CPPFLAGS="-I$BUILD_DIR$PREFIX/include"
# find our own perl modules
export PERL5LIB=$BUILD_DIR$PREFIX/lib/perl5



# automatically build a configure make install type tool

function buildAuto () {
   URL=$1
   shift
   wget $URL -O - | tar xf -
   pushd "`ls -1frt| grep /| tail -1`"
   ./configure --prefix=$PREFIX "$@"
   make install DESTDIR=$BUILD_DIR
   popd
}

# build a copy of perl

function buildPerl () {
   PERL_VER=$1
   wget http://www.cpan.org/src/5.0/perl-${PERL_VER}.tar.gz  -O - | tar xf -
   pushd "`ls -1frt| grep /| tail -1`"
   ./Configure \
     -Ui_db \
     -Dprefix=$PREFIX \
     -Dprivlib=.../../lib/perl \
     -Dsitelib=.../../lib/perl5 \
     -Duserelocatableinc \
     -Dusethreads -des
   make
   make install DESTDIR=$DESTDIR
}

# build a perl module

function buildPerlModule () {
   mkdir -p $DESTDIR$PREFIX/bin
   wget --no-check-certificate -O $DESTDIR$PREFIX/bin/cpanm cpanmin.us
   chmod 755 $DESTDIR$PREFIX/bin/cpanm
   export PERL_CPANM_OPT="--notest"
   if [ ! -f $DESTDIR$PREFIX/bin/perl ]; then
        PERL_CPANM_OPT="${PERL_CPANM_OPT} --local-lib-contained $PREFIX"
   fi
   cpanm "$@"
}


