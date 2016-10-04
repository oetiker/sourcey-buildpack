#!/usr/bin/env perl
 
use Dancer2;

get '/' => sub {
   my $err;
   template 'index.tt', {
       'err' => $err,
   };
};
 
get '/hello/:name' => sub {
    return "Why, hello there " . params->{name};
};

get '/envs' => sub {
  my $self = shift;
  my ($key, $envstring);
  for $key (keys %ENV){
    $envstring .= "$key : $ENV{$key}<br>\n";
  }
  return $envstring;
};

dance;
