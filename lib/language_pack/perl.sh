#!/bin/bash
# $0 install-dir perl-version

# build perl including a copy of cpanm

PERL_VER=$1
shift

. `dirname $0`/sdbs.inc

if [ x$PREFIX = x ]; then
   echo $0 install-dir perl-version
   exit 1
fi

# unset PERL settings in sdbs for building perl
export PERL5LIB=
export PERL=
export PERL_CPANM_HOME=
export PERL_CPANM_OPT=

if prepare http://www.cpan.org/src/5.0 perl-${PERL_VER}.tar.gz; then
    make clean || true
    ./Configure -de \
        -Ui_db \
        -Dprivlib=$PREFIX/lib/perl \
        -Dsitelib=$PREFIX/lib/perl5 \
        -Dprefix=$PREFIX \
        -Dperlpath="$PREFIX/bin/perl" \
        -Dstartperl="#!$PREFIX/bin/perl" \
        -Duserelocatableinc
        -Dusethreads -de
    make
    make install
    touch $WORKDIR/perl-${PERL_VER}.tar.gz.ok
    
    wget --no-check-certificate -O $PREFIX/bin/cpanm cpanmin.us && \
        chmod 755 $PREFIX/bin/cpanm
fi
