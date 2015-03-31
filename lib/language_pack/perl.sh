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
    ./Configure \
        -Ui_db \
        -Dprefix=$PREFIX \
        -Dprivlib=.../../lib/perl \
        -Dsitelib=.../../lib/perl5 \
        -Duserelocatableinc \
        -Dusethreads -des
    make
    make install
    cd /
    rm -rf $WORKDIR
    
    wget --no-check-certificate -O $PREFIX/bin/cpanm cpanmin.us && \
        chmod 755 $PREFIX/bin/cpanm
fi
