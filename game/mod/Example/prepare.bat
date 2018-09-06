del errors.txt
del debug.txt
del data\resource.zip
del data\resource.dat

rem compression level is disabled by -e0 so the scripts load faster. Turn it on if you want.
pkzip data\resource.dat *.c *.pss -rp -e0
