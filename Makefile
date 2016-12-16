all:
	cd main; make
	cd renderer; make
	cd samples; make

clean:
	cd main; make clean
	cd renderer; make clean
	cd samples; make clean

distclean:
	cd main; make distclean
	cd renderer; make distclean
	cd samples; make distclean
