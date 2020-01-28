SRC = list.c slide.c
DL = bmpauto2.c jpgauto2.c

OBJ = $(SRC:%.c=%.o)
TAR = $(DL:%2.c=lib%.so)
JRC = $(DL:%.c=%.o)

CC = arm-none-linux-gnueabi-gcc

CPPFLAGS += -I ../head

LDFLAGS += -L /home/gec/abc/lib -ljpeg
LDFLAGS += -L ../lib -lx
LDFLAGS += -ldl

RPATH = -Wl,-rpath=lib/

libx.so:$(OBJ)
	$(CC) -shared -fPIC -o $@ $(OBJ)
	cp $@ ../
	mv $@ ../lib

$(OBJ):%.o:%.c
	$(CC) $^ -o $@ -c -fPIC $(CPPFLAGS) $(LDFLAGS)

main:main.c $(TAR)
	$(CC) $< -o $@ $(CPPFLAGS) $(LDFLAGS) $(RPATH)
	mv $@ ..

$(TAR):lib%.so:%2.o $(JRC)
	 $(CC) -shared -fPIC -o $@ $<
	cp $@ ../
	mv $@ ../lib
	
$(JRC):%.o:%.c
	 $(CC) $< -o $@ -c -fPIC $(CPPFLAGS) $(LDFLAGS)

clean:
	$(RM) $(JRC)
	$(RM) $(OBJ)

distclean:clean
	$(RM) ../lib/libbmpauto.so ../lib/libjpgauto.so
	$(RM) ../lib/libx.so

.PHONY:clean distclean
