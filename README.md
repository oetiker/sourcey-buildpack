# sourcey-buildpack for cloudfoundry

While it is nice to just throw a bunch of php, ruby, java or python files at
a cloud foundry service and see them magically turn into a running web applications. Alas for me this
does normally not work so well since I always need a few extra libraries
or even an unsupported scripting language like Perl.

Enters `Sourcey-buildpack`. It allows you to easily compile a bunch of libraries and binaries from source
taking care of fixing installation paths such that the end result happily lives
in `/home/vcap/app/thirdparty` and even knows that it does so, without the need for any
`LD_LIBRARY_PATH` setting or other path magic.

The Sourcey-buildpack expects to find two special files in your application directory:

`SourceyBuild3rdParty.sh` (optional) is used at buildtime to compile all the binaries you need.

`SourceyBuildApp.sh` (optional) is used at buildtime of the actual application if this needs any building.

`SourceyStart.sh` (mandatory) is used at runtime to launch the application.

## `SourceyBuild3rdParty.sh`

In this script you install software using the classic autotools approach.
With just two variables pre-configured for your installation fun:

`PREFIX` pointing to `/home/vcap/app/thirdparty` as this is the place where your compiled software
will end up at runtime.

`BUILD_DIR` pointing to `/tmp/staged/app` where the cloudfoundry
droplet build process expects to find the results of your efforts to reside.

```shell
wget http://cool-site.com/source.tar.gz
tar xf source.tar.gz
cd source
./configure --prefix=$PREFIX
make install DESTDIR=$BUILD_DIR
```
Sourcey does two things:

1. It moves `$BUILD_PATH$PREFIX` up to `$BUILD_PATH`, and thus making sure that at runtime
   your binaries will end up in $PREFIX again.

2. It creates a copy of `$BUILD_DIR/thirdparty` in `$CACHE_DIR` and tags it with the md5 sum of your `SourceyBuild3rdParty.sh`.
   If you re-deploy the same app again, without changing `SourceyBuild3rdParty.sh`. The content of the `$CACHE_DIR` will be used
   in stead of rebuilding everything.

To make live a bit simpler still, Sourcey provides a bunch of helper functions which simplify the build process.

### `buildAuto <url> [options]`

Essentially does the same as described in detail above, just simpler. If you want to specify extra configure options,
just add them as extra arguments at the end of the function call:

```shell
buildAuto http://cool-site.com/source.tar.gz --my-very-cool-option=42 --do-not-read-this
```

### `buildPerl <version>`

This is how it all got started. How to write a decent web application without Perl. Since most cloudfoundry setups are on ubuntu lucid (10.04) still, perl is also at 5.10.1. Which is about 10 years out of date.

```shell
buildPerl 5.20.2
```

### `buildPerlModule [any cpanm option]`

This is a wrapper for cpanm which you can use to install extra perl modules. The new modules will get installed into your freshly installed
perl setup, or if you have not done that, the system perl will be used and the modules will go to `/home/vcap/app/thirdpary/lib/perl5`.
Sourcey will take care of setting the `PERL5LIB` variable accordingly.

## `SourceyBuildApp.sh`

This script can do whatever you deem necessary to get your actual application into shape for execution. Nothing will be cached. If you
push an update for your application, this script will run again.


## `SourceyStart.sh`

This one gets executed when your application should be started. Sourcey will take care of setting the PATH variable so that all these
shiny new 3rd party tools get found automatically. At the end of your `SourceyStart.sh` someone should be listening ```localhost:$PORT``` for incoming web requests.

In order for your application to integrate with the cloudfoundry infrastructure, you want to json decode the content of the
environment variables `VCAP_SERVICES` and `VCAP_APPLICATION`.


## Example

See the example directory for inspiration as to how this would look in reality.
