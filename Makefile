all:
	cd main; make
	cd renderer; make
	cd test; make

clean:
	cd main; make clean
	cd renderer; make clean
	cd test; make clean

distclean:
	cd main; make distclean
	cd renderer; make distclean
	cd test; make distclean
