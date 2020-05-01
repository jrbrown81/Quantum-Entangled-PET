#!/usr/bin/perl
# $Id: whist.pl,v 1.3 2010/01/10 20:58:26 g4ai Exp $

use strict;

# Weighted histogram
# Reads from standard input, writes to standard output suitable for gnuplot.
# Assumes input is of the form "weight x [y]
# (To get an unweighted histogram simply put weight=1)
#
# Lines beginning with '#' are printed and ignored.
#
# Usage: whist.pl <xmin> <xmax> <nbinsx> [<ymin> <ymax> <nbinsy>]
# cat datafile | whist.pl -300 300 20 >x.hist
#
# For example, suppose you have unweighted data 3 columns
# and you wish to make a histogram of the 2nd item on each line.
# cat datafile | awk '{print(1,$2)}' | whist.pl 0 10 10 >x.hist
# Note: the weight of 1 is supplied explicitly in print(1,$2).



# Adds 0 pseudo-bin at end to get gnuplot to draw last bin "with steps".


#
# Then in gnuplot:
# set title "Ionising dose by cos(polar angle) at entry to radiating sphere"
# set xlabel "cos(theta)"
# set ylabel "TID (arbitray units)"
# set yrange [0:*]
# set style data step
# set terminal postscript eps color linewidth 2
# set output "x.eps"
# plot "x.hist" title "legend" with steps
# Then:
# gv x.eps

if (@ARGV != 3 && @ARGV != 6) {
  print STDERR "Usage: whist.pl <xmin> <xmax> <nbinsx> [<ymin> <ymax> <nbinsy>]\n";
  exit;
}
my $TWOD = 0;
if (@ARGV == 6) {$TWOD = 1;}

my $XMIN = $ARGV[0];
my $XMAX = $ARGV[1];
my $NBINSX = $ARGV[2];
my $XWIDTH = ($XMAX - $XMIN) / $NBINSX;
my $YMIN = 0;
my $YMAX = 0;
my $NBINSY = 0;
my $YWIDTH = 0;
if ($TWOD) {
  $YMIN = $ARGV[3];
  $YMAX = $ARGV[4];
  $NBINSY = $ARGV[5];
  $YWIDTH = ($YMAX - $YMIN) / $NBINSY;
}

if ($TWOD) {
  print "# XMIN XMAX NBINSX YMIN YMAX NBINSY\n";
  print "# ", $XMIN, " ", $XMAX, " ", $NBINSX, " ", $YMIN, " ", $YMAX, " ", $NBINSY, "\n";
} else {
  print "# XMIN XMAX NBINSX\n";
  print "# ", $XMIN, " ", $XMAX, " ", $NBINSX, "\n";
}

my $wlox = 0;
my $whix = 0;
my $wloy = 0;
my $whiy = 0;
my $win = 0;
my $wout = 0;
my $wtot = 0;
my $w2tot = 0;
my $elox = 0;
my $ehix = 0;
my $eloy = 0;
my $ehiy = 0;
my $ein = 0;
my $eout = 0;
my @xloEdge;
my @yloEdge;
for (my $i = 0; $i <= $NBINSX; $i++) {
  $xloEdge[$i] = $XMIN + $XWIDTH * $i;
}
if ($TWOD) {
  for (my $i = 0; $i <= $NBINSY; $i++) {
    $yloEdge[$i] = $YMIN + $YWIDTH * $i;
  }
}
my @w;
my @e;
my (@w2, @w);
my (@e2, @e);
if ($TWOD) {
  for (my $j = 0; $j < $NBINSY; $j++) {
    for (my $i = 0; $i < $NBINSX; $i++) {
      $w2[$i][$j] = 0;  # Weighted sum
      $e2[$i][$j] = 0;  # Error (standard deviation) on weighted sum
    }
  }
} else {
  for (my $i = 0; $i < $NBINSX; $i++) {
    $w[$i] = 0;  # Weighted sum
    $e[$i] = 0;  # Error (standard deviation) on weighted sum
  }
}
my $xsum = 0;
my $xesum = 0;
my $x2sum = 0;
my $ysum = 0;
my $yesum = 0;
my $y2sum = 0;

my $nEvents = 0;
my $line = <STDIN>;
do {
  my @a = split /[\ \n]\s*/, $line;
  if ($a[0] =~ /#/) {
    print $line;
  } else {
    $nEvents++;
    my $WEIGHT = $a[0];
    $wtot += $WEIGHT;
    $w2tot += $WEIGHT * $WEIGHT;
    my $x = $a[1];
    my $y = 0; if ($TWOD) {$y = $a[2];}
    $xsum += $WEIGHT * $x;
    $x2sum += $WEIGHT * $x *$x;
    if ($TWOD) {
      $ysum += $WEIGHT * $y;
      $y2sum += $WEIGHT * $y *$y;
    }
    my $isOut = 0;
    if ($x < $XMIN) {
      $isOut = 1;
      $wlox += $WEIGHT;
      $wout += $WEIGHT;
      $elox += $WEIGHT * $WEIGHT;
      $eout += $WEIGHT * $WEIGHT;
    } elsif ($x >= $XMAX) {
      $isOut = 1;
      $whix += $WEIGHT;
      $wout += $WEIGHT;
      $ehix += $WEIGHT * $WEIGHT;
      $eout += $WEIGHT * $WEIGHT;
    }
    if ($TWOD) {
      if ($y < $YMIN) {
	$isOut = 1;
	$wloy += $WEIGHT;
	$wout += $WEIGHT;
	$eloy += $WEIGHT * $WEIGHT;
	$eout += $WEIGHT * $WEIGHT;
      } elsif ($y >= $YMAX) {
	$isOut = 1;
	$whiy += $WEIGHT;
	$wout += $WEIGHT;
	$ehiy += $WEIGHT * $WEIGHT;
	$eout += $WEIGHT * $WEIGHT;
      }
    }
    if (!$isOut) {
      if ($TWOD) {
	$win += $WEIGHT;
	$ein += $WEIGHT * $WEIGHT;
	my $i = ($x - $XMIN) / $XWIDTH;
	my $j = ($y - $YMIN) / $YWIDTH;
	$w2[$i][$j] += $WEIGHT;
	$e2[$i][$j] += $WEIGHT * $WEIGHT;
      } else {
	$win += $WEIGHT;
	$ein += $WEIGHT * $WEIGHT;
	my $i = ($x - $XMIN) / $XWIDTH;
	$w[$i] += $WEIGHT;
	$e[$i] += $WEIGHT * $WEIGHT;
      }
    }
  }
} while(defined($line = <STDIN>));

$elox = sqrt($elox);
$ehix = sqrt($ehix);
$eloy = sqrt($eloy);
$ehiy = sqrt($ehiy);
$eout = sqrt($eout);
$ein = sqrt($ein);
my $etot = sqrt($w2tot);
if ($TWOD) {
  for (my $j = 0; $j < $NBINSY; $j++) {
    for (my $i = 0; $i < $NBINSX; $i++) {
      $e2[$i][$j] = sqrt($e2[$i][$j]);
    }
  }
} else {
  for (my $i = 0; $i < $NBINSX; $i++) {
    $e[$i] = sqrt($e[$i]);
  }
}
my $xmean = 0;
my $x2mean = 0;
my $xvariance = 0;
my $xstddev = 0;
my $xstddev = 0;
my $xemean = 0;
my $ymean = 0;
my $y2mean = 0;
my $yvariance = 0;
my $ystddev = 0;
my $yemean = 0;
if ($wtot > 0.) {
  if ($nEvents) {
    $xesum = sqrt($x2sum);
    $xmean = $xsum / $wtot;
    $x2mean = $x2sum / $wtot;
    if ($TWOD) {
      $yesum = sqrt($y2sum);
      $ymean = $ysum / $wtot;
      $y2mean = $y2sum / $wtot;
    }
  }
  if ($nEvents > 1) {  # See http://en.wikipedia.org/wiki/Weighted_mean.
    $xvariance = ($x2mean - $xmean * $xmean) * $wtot * $wtot / ($wtot * $wtot - $w2tot);
    $xstddev = sqrt($xvariance);
    $xemean = sqrt($xvariance / $nEvents);  #?????????????????????????
    if ($TWOD) {
      my $yvariance = ($y2mean - $ymean * $ymean) * $wtot * $wtot / ($wtot * $wtot - $w2tot);
      $yemean = sqrt($yvariance / $nEvents);  #???????????????????????
    }
  } else {
    $xemean = -1;
    $xvariance = -1;
    $xstddev =  -1;
    $yemean = -1;
    $yvariance = -1;
    $ystddev =  -1;
  }
}

if ($TWOD) {
  print "# wlox whix wloy whiy wout win wtot\n";
  print "# ", $wlox, " ", $whix, " ", $wloy, " ", $whiy, " ", $wout, " ", $win, " ", $wtot, "\n";
  print "# +/- ", $elox, " ", $ehix, " ", $eloy, " ", $ehiy, " ", $eout, " ", $ein, " ", $etot, "\n";
  print "# Weighted sum: ";
  print "X: ", $xsum, " +/- ", $xesum, " ";
  print "Y: ", $ysum, " +/- ", $yesum;
  print " weighted mean ";
  print "X: ", $xmean, " +/- ", $xemean, " ";
  print "Y: ", $ymean, " +/- ", $yemean;
  print " standard deviation: ";
  print "X: ", $xstddev, " ";
  print "Y: ", $ystddev, "\n";
  print "# xloEdge yloEdge weight error\n";
  for (my $j = 0; $j < $NBINSY; $j++) {
    for (my $i = 0; $i < $NBINSX; $i++) {
      print " ", $xloEdge[$i], " ", $yloEdge[$j];
      print " ", $w2[$i][$j], " ", $e2[$i][$j];
      print "\n";
    }
    print "\n";
  }
} else {
  print "# wlox whix wout win wtot\n";
  print "# ", $wlox, " ", $whix, " ", $wout, " ", $win, " ", $wtot, "\n";
  print "# +/- ", $elox, " ", $ehix, " ", $eout, " ", $ein, " ", $etot, "\n";
  print "# Weighted sum: ";
  print $xsum, " +/- ", $xesum;
  print ", weighted mean: ";
  print $xmean, " +/- ", $xemean;
  print ", standard deviation: ";
  print $xstddev, "\n";
  print "# xloEdge weight error\n";
  for (my $i = 0; $i < $NBINSX; $i++) {
    print " ", $xloEdge[$i], " ", $w[$i], " ", $e[$i], "\n";
  }
}
