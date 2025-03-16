OBJ=common.o command-line.o display.o usb.o scsi.o pci.o pci_id.o
FLAGS=-g -DPACKAGE_NAME=\"devlist\" -DPACKAGE_TARNAME=\"devlist\" -DPACKAGE_VERSION=\"1.0\" -DPACKAGE_STRING=\"devlist\ 1.0\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -D_FILE_OFFSET_BITS=64 -DHAVE_LIBZ=1 -DHAVE_LIBCAP=1 -DHAVE_LIBCRYPTO=1 -DHAVE_LIBSSL=1 -DHAVE_PROMPT_HISTORY=\"Y\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_LIBUSEFUL_5_LIBUSEFUL_H=1 -DHAVE_LIBUSEFUL_5=1 -DHAVE_LIBUSEFUL_5=1
LIBS=-lUseful-5 -lUseful-5 -lssl -lcrypto -lcap -lz 
LIBUSEFUL_BUNDLED=

all: $(OBJ) 
	gcc $(FLAGS) -o devlist main.c $(OBJ) $(LIBS) $(LIBUSEFUL_BUNDLED)

libUseful-bundled/libUseful.a:
	$(MAKE) -C libUseful-bundled

common.o: common.c common.h
	gcc $(FLAGS) -c common.c

display.o: display.c display.h
	gcc $(FLAGS) -c display.c

usb.o: usb.c usb.h
	gcc $(FLAGS) -c usb.c

pci.o: pci.c pci.h
	gcc $(FLAGS) -c pci.c

scsi.o: scsi.c scsi.h
	gcc $(FLAGS) -c scsi.c

pci_id.o: pci_id.c pci_id.h
	gcc $(FLAGS) -c pci_id.c

command-line.o: command-line.c command-line.h
	gcc $(FLAGS) -c command-line.c


clean:
	rm *.o *.orig */*.o */*.a */*.so */*.so.*
