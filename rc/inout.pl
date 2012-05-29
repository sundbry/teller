#!/usr/bin/perl

open(my $inFile, "<", $ARGV[0].".sigin");
open(my $outFile, ">", $ARGV[0].".sigout");

my $lastOp = "none";
my $op = "none";
my $sum = 0;
my $haveFirstPulse = 0;
my $haveFirstSpace = 0;

while(<$inFile>) {
	my $line = $_;
	my $val = 0;
	$op = "none";

	if($line =~ /\sspace:\s(\d+)$/) {
		$op = "space";
		$val = $1;
	}

	if($haveFirstSpace && $line =~ /\spulse:\s(\d+)$/) {
		$op = "pulse";
		$val = $1;
	}


	if($op ne "none") {
		if($op eq $lastOp) {
			$sum = $sum + $val;
		}
		else {
			if($haveFirstPulse && $lastOp ne "none") {
				print $outFile $lastOp." ".$sum."\n";
			}
			$sum = $val;
		}
		$lastOp = $op;
	}

	if(!$haveFirstSpace && $op eq "space") {
		$haveFirstSpace = 1;
	}
	if(!$haveFirstPulse && $op eq "pulse") {
		$haveFirstPulse = 1;
	}
}

if($lastOp eq "pulse" && $sum > 0) {
	print $outFile $lastOp." ".$sum."\n";
}

close($inFile);
close($outFile);
