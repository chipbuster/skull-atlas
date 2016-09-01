#! /usr/bin/perl

# Combines two .off files into a single output file.

use strict;

my $inp1 = shift;
my $inp2 = shift;
my $outp = shift;

open(my $INP1, "<", $inp1) or die "Could not open input [$inp1]: $!\n";
open(my $INP2, "<", $inp2) or die "Could not open input [$inp2]: $!\n";
open(my $OUTP, ">", $outp) or die "Could not open input [$outp]: $!\n";

my $col1 = "255 0 0 0";
my $col2 = "0 0 255 0";

# Get number of faces for first
my $h1 = <$INP1>; #ignore the OFF line
$h1 = <$INP1>;
$h1 =~ /(\d+)\s+(\d+)\s+(\d+)/;
my $nv1 = $1;
my $nf1 = $2;

my $h2 = <$INP2>; #ignore the OFF line
$h2 = <$INP2>;
$h2 =~ /(\d+)\s+(\d+)\s+(\d+)/;
my $nv2 = $1;
my $nf2 = $2;

# Print out all the info
my $nv = $nv1 + $nv2;
my $nf = $nf1 + $nf2;
print $OUTP "COFF\n"; # Color OFF file
print $OUTP "$nv $nf 0\n"; # Ignore edges

# Print all the vertices from 1
for my $i (1..$nv1) {
  my $line = <$INP1>;
  chomp $line;
  print $OUTP "$line $col1\n";
}
# Print all the vertices from 2
for my $i (1..$nv2) {
  my $line = <$INP2>;
  chomp $line;
  print $OUTP "$line $col2\n";
}
# Print all the faces for each.
while(<$INP1>) {
  print $OUTP $_;
}
# Need to adjust faces for 2
while(<$INP2>) {
  my @pcs = split;
  print $OUTP "$pcs[0]";
  for (@pcs[1..$#pcs]) {
    my $v_up = $_ + $nv1;
    print $OUTP " $v_up";
  }
  print $OUTP "\n";
}

close($INP1);
close($INP2);
close($OUTP);
