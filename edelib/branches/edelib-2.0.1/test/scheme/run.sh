#!/bin/sh 

export EDELIB_SCHEME_INIT="../../sslib/init.ss:../../sslib/init-2.ss:../../sslib/theme.ss"
bin="../../tools/edelib-script/edelib-script"
ret=0

run_file() {
	tmp_file=".out"
	$bin $1 | tee -a $tmp_file

	grep FAILED $tmp_file 1> /dev/null
	[ "x$?" = "x0" ] && ret=1

	rm -f $tmp_file
}

run_file unittest.ss
run_file r5rs.ss
run_file edelib.ss
run_file sslib.ss

exit $ret
