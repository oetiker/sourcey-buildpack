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

for fn in $(ls -1 $1/lib/*.func); do
    source $fn
done
