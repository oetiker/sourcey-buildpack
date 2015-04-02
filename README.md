# sourcey-buildpack for cloudfoundry

It is simply amazing to see these demos where they throw a bunch of php,
ruby, java or python files at a cloudfoundry instance and they get magically
turned into a running web applications.  Alas for me this does normally not
work so well since I always need a few extra libraries or even an
unsupported scripting language like Perl.

Enters `sourcey-buildpack`. It allows you to easily compile a bunch of
libraries and binaries from source. It takes care of fixing installation paths
such that the end result happily lives in `/home/vcap/app/sourcey` and
even knows that it does so without the need for any `LD_LIBRARY_PATH`
setting or other path magic.

The Sourcey-buildpack expects to find up to three special files in your application directory:

`SourceyBuild.sh` (optional) is used at build-time to compile all the binaries you need.

`SourceyBuildApp.sh` (optional) is used at build-time of the actual application if this needs any building.

`SourceyStart.sh` (mandatory) is used at runtime to launch the application.

## `SourceyBuild.sh`

This script builds the third party software you require. You are free to move about the cabin as long
as the result of your effort ends up in the right location. There are two environment variables
which help you in this:

`PREFIX` pointing to `$HOME/app/sourcey` as this is the place where your compiled software
will end up at runtime. (Note that HOME at build time points to /home/vcap while HOME at runtime points to /home/vcap/app!).

`BUILD_DIR` pointing to the dirctory where the cloudfoundry
droplet build process expects to find the files necessary for running your app to reside.

At the end of your script all software must be installed in `$PREFIX`.

For a classic autotools packaged application, your setup instructions might look like this:

```shell
wget http://cool-site.com/source.tar.gz
tar xf tool.tar.gz
cd tool
./configure --prefix=$PREFIX
make install
```

When your script has run through, Sourcey does two things:

1. It moves `$PREFIX` up to `$BUILD_DIR`, and thus making sure that at runtime
   your binaries will end up in $PREFIX again.

2. It creates a copy of `$BUILD_DIR/sourcey` in `$CACHE_DIR` and tags it with the md5 sum of your `SourceyBuild3rdParty.sh`.
   If you re-deploy the same app again, without changing `SourceyBuild3rdParty.sh`. The content of the `$CACHE_DIR` will be used
   in stead of rebuilding everything.

To make live a bit simpler still, Sourcey provides a bunch of helper functions:

### `buildAuto <url> [options]`

Essentially does the same as described in detail above, just simpler. If you want to specify extra configure options,
just add them as extra arguments at the end of the function call:

```shell
buildAuto http://mysite/tool.tar.gz --default-answer=42 --switch-off
```

### `buildPerl <version>`

This is how it all got started. How to write a decent web application without Perl. Since most cloudfoundry setups are on Ubuntu lucid (10.04) still, perl is also at 5.10.1 which is about 100 years out of date. With this call you get a fresh copy.

```sh
buildPerl 5.20.2
```

### `buildPerlModule [any cpanm option]`

This is a wrapper for cpanm which you can use to install extra perl modules. The new modules will get installed into your freshly installed
perl setup, or if you have not done that, the system perl will be used and the modules will go to `/home/vcap/app/sourcey/lib/perl5`.
Sourcey will take care of setting the `PERL5LIB` variable accordingly.

## `SourceyBuildApp.sh`

This script can do whatever you deem necessary to get your actual application into shape for execution. Nothing will be cached. If you
push an update for your application, this script will run again.


## `SourceyStart.sh`

This one gets executed when your application should be started. Sourcey will take care of setting the PATH variable so that all these
shiny new 3rd party tools get found automatically. At the end of your `SourceyStart.sh` someone should be listening ```localhost:$PORT``` for incoming web requests.

In order for your application to integrate with the cloudfoundry infrastructure, you want to json decode the content of the
environment variables `VCAP_SERVICES` and `VCAP_APPLICATION`.

## Debugging

If things are not going according to plan. You can put the folling variables into your `SourceyBuild.sh` file.

`SOURCEY_VERBOSE=1` will cause all output generated at build time to be sent to STDOUT. Note that this does look like an environment variable, but infact the compile script runs grep
on your SourceyBuild.sh to find it.

`SOURCEY_REBUILD=1` will ignore any cached copy of your binaries.

## Example

The code in the example directory demonstrates how to setup a simple
Mojolicious Perl app.

The following instructions assume you have already setup a cloudfoundry
account and you have logged yourself in with `cf login`

```sh
cd example
cf push $USER-sourcey-demo -b https://github.com/oetiker/sourcey-buildpack
```


