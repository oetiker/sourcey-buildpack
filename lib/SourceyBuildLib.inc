#
# Some helpful function for the compile call
#

# make sure we find anything we preinstall
export PATH=$PREFIX/bin:$PATH
# tell pkg-config where to look for *.pc files
export PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig
# just to make sure those who are not compiled propperly still work
export LD_LIBRARY_PATH=$PREFIX/lib
# find our libraries
export LDFLAGS="-Wl,-rpath -Wl,$PREFIX/lib -L$PREFIX/lib"
# find our includes
export CPPFLAGS="-I$PREFIX/include"
# find our own perl modules
export PERL5LIB=$PREFIX/lib/perl5

function error() {
  echo "       *** ERROR ***"
  if [ ${SOURCEY_VERBOSE:-0} = 0 ]; then
     tail -n50 $WORK_DIR/sourcey.outcap | perl -pe 's/^/       /';
  fi
  exit 1
}

trap 'error' ERR

function progressor () {
   title=$1
   countdown=${2:-0}
   echo "       $title"
   if [ ${SOURCEY_VERBOSE:-0} = 1 ]; then
      cat
   else
       tee $WORK_DIR/sourcey.outcap | perl -e 'my $start=time;my $d='${countdown}'; my $inc = $d ? -1 : 1; $|=1;while (<>){$d += $inc; if (time - $start > 5){ $start=time; print "          $d lines ".($inc < 0 ? "to go" : "...")."\n"}} print "          $d lines total\n" if $inc > 0'
   fi
}

for fn in $BUILDPACK_DIR/lib/*.func; do
    source $fn
done
