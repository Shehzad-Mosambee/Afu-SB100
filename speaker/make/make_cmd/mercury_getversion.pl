#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 1)
{
	$input_file = $ARGV[0];
}
else
{
	return 1;
}

$MANUFACTURE_IND = 0;
$PRODUCT_MODEL_IND = 1;
$SW_MAJOR_IND = 2;
$SW_MINOR_IND = 3;
$SW_BUILDID_IND = 4;
$FILE_TYPE = 5;

$MAX_IND = $FILE_TYPE + 1;
$VERSION_STRING[$MAX_IND] = 0;

open (inf,$input_file) || die ("file error!!!");
@alllines = <inf>;
close (inf);
$cnt = 0;
foreach $myline (@alllines)
{
	$_=$myline;
	chomp;
	$_=~s/^\s+|\s+$//g;

	if($_ =~ m/(\#define) (.*) (.*)/)
	{	
		$tmp=$3;
		#print $tmp."\r\n";
		$tmp=~s/"//g;
		$VERSION_STRING[$cnt] = $tmp;
		$cnt++;
		if ($cnt == $MAX_IND)
		{
			last;
		}
	} 
}

$ver_str = "".$VERSION_STRING[$SW_MAJOR_IND]." ".$VERSION_STRING[$SW_MINOR_IND]." ".$VERSION_STRING[$SW_BUILDID_IND];

print $ver_str;