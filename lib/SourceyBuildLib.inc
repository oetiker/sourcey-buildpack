#
# Some helpful function for the compile call
#
BASEDIR=$1

SOURCEY=sourcey
# where we build stuff
PREFIX=$HOME/app/$SOURCEY
# make sure we find anything we preinstall
export PATH=$PREFIX/bin:$PATH
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

function progressor () {
   title=$1
   countdown=$2
   echo "        $1"
   tee outcap.$$ | perl -e 'my $d='$countdown'; my $inc = $d ? 1 : -1; $|=1;while (<>){printf "\b" x 10 . "%10d",$d+=$inc}' 
   if test ${PIPESTATUS[0] -eq 1; then
      echo "         ### Problem ###"
      cat outcap.$$
      exit 1
   fi
   rm outcap.$$
   echo
}

for fn in $BASEDIR/lib/*.func; do
    source $fn
done
