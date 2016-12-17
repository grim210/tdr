#!/bin/sh

DELSTRING="aclocal.m4 ar-lib autom4te.cache compile config.h config.h.in config.guess config.log config.status config.sub configure depcomp install-sh libtool ltmain.sh Makefile Makefile.in missing stamp-h1"
RM="rm -rf"
M4_GARBAGE="m4/libtool.m4 m4/ltoptions.m4 m4/ltsugar.m4 m4/ltversion.m4 m4/lt~obsolete.m4"

$RM $M4_GARBAGE

$RM $DELSTRING
cd ./main && $RM $DELSTRING && cd ..
cd ./main/src/ && $RM $DELSTRING && cd ../..
cd ./renderer && $RM $DELSTRING && cd ..
cd ./samples && $RM $DELSTRING && cd ..
