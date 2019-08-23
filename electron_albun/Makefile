vpath %.c = src/
vpath %.so = lib/
vpath %.h = head/

main:main.c install
	$(MAKE) -C src/ main

install:
	$(MAKE) -C src/ libx.so

clean:
	$(RM) main *.so
	$(MAKE) -C src/ clean
	
distclean:clean
	$(MAKE) -C src/ distclean

.PHONY:install clean distclean
