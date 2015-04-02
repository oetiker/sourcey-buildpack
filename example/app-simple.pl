#!/usr/bin/env perl
use Mojolicious::Lite;

get '/' => sub {
  my $self = shift;
  $self->render('index');
};

app->start;
__DATA__

@@ index.html.ep
% layout 'default';
% title 'Welcome';
% use Mojo::JSON qw(decode_json);

<h1>Cloud Environment</h1>
<div>
%== join "<br/>", map { "$_: '$ENV{$_}'" } keys %ENV
</div>

<h1>VCAP_SERVICES</h1>
<pre>
%= dumper decode_json($ENV{VCAP_SERVICES});
</pre>

<h1>VCAP_APPLICATION</h1>
<pre>
%= dumper decode_json($ENV{VCAP_APPLICATION});
</pre>

@@ layouts/default.html.ep
<!DOCTYPE html>
<html>
  <head><title><%= title %></title></head>
  <body><%= content %></body>
</html>
