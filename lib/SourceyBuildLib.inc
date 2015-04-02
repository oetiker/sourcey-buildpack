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
   countdown=${2:-0}
   echo "       $1"
   if [ $SOURCEY_VERBOSE ]; then
      cat
   else
       tee outcap.$$ | perl -e 'my $start=time;my $d='${countdown}'; my $inc = $d ? -1 : 1; $|=1;while (<>){$d += $inc; if (time - $start > 5){ $start=time; print "          $d lines ".($inc < 0 ? "to go" : "...")."\n"}} print "          $d lines total\n" if $inc > 0'
       if test ${PIPESTATUS[0]} -eq 1; then
           echo "### Problem ###"
           cat outcap.$$
           exit 1
       fi
       rm outcap.$$
   fi
}

for fn in $BASEDIR/lib/*.func; do
    source $fn
done
