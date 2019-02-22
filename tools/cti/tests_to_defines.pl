#!/usr/bin/perl

$DEFINES = '';
$DESC = '';
%SUBDESC = ();
%DESC = ();

$INF = shift;
$SINF = shift;
$OUT_DEFS = shift;
$OUT_DESC = shift;

open(IN,'<',$SINF) or die("open $SINF");
while(<IN>){
	tr/\r\n//d;
	next if(m/^#/);
	next if($_ eq '');
	
	if( m/^([\d,]+)\t(\d+)\t(.+)$/ ){
		$tests = $1;
		$subtest = $2;
		$subtitle = $3;

		@tests = split(/,/,$tests);
		foreach (@tests){
			$SUBDESC{$_} .= "\t\t\t\"$subtest\":\"$subtitle\",\n";
		}
	}
}
close(IN);

open(IN,'<',$INF) or die("open $INF");
while(<IN>){
	tr/\r\n//d;
	next if(m/^#/);
	next if($_ eq '');

	if( m/^(\d+)\t([a-zA-Z0-9]+)\t(.+)$/ ){
		$test = $1;
		$nom = $2;
		$desc = $3;

		$DEFINES .= "#define CTI_TEST_" . uc($nom) . "\t$test\n";

		if( defined $SUBDESC{$test} ){
			$DESC .= "\t\t\"$test\":{\n" . $SUBDESC{$test} . "\t\t\t\"0\":\"$nom\"},\n";
		} else {
			$DESC .= "\t\t\"$test\":{\"0\":\"$nom\"},\n";
		}
		$DESC{$test} = $desc;
	}
}
close(IN);

open(OUT,'>',$OUT_DEFS) or die("open $OUT_DEFS for write");
print OUT "#ifndef _CTI_TEST_\n";
print OUT "#define _CTI_TEST_\n\n";
print OUT $DEFINES;
print OUT "\n#endif // _CTI_TEST_\n";
close(OUT);

open(OUT,'>',$OUT_DESC) or die("open $OUT_DESC for write");
print OUT <<EOT;
{
	"vendor":"additionsecurity.com",
	"format":1,
	"version":20160301,
	"events":{
EOT

print OUT $DESC;
print OUT "\t\t\"0\":{\"0\":\"-\"}\n";
print OUT "\t}\n}\n";
close(OUT);



