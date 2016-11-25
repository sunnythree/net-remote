
CC = mips-linux-gnu-gcc -EL -Wall -O2
MYINCUDE  =    -I../../../develop/extra/target/mips-glibc/hardfloat/directfb-1.4.2m/include/directfb/
MYINCUDE  +=   -I./include
MYLIBDIR    =    -L../../../target/isdb.nugget/tmp_image/mslib
MYLIBDIR  +=  -L../../../target/isdb.nugget/tmp_image/mslib/utopia
MYLIB          =  $(MYLIBDIR) -ldirectfb -lz -ldrvMVOP -lapiGFX -lapiGOP -llinux -ldrvVE -lapiXC  -lapiPNL -ldrvWDT
MYLIB        += -ldrvSAR  -lapiVDEC -ldrvIPAUTH -lapiJPEG  -ldirect -lfusion

MSRC  = remote-C.o 
MSRC  += my_dfb.o 
MSRC  += my_socket.o 
MSRC  += parse.o 
MSRC  += convert2ekey.o

DESBIN = remote-C.bin

export CC
export MYINCUDE
export MYLIB
.PHONY:subsystem parse.o
subsystem:parse.o $(DESBIN)

parse.o:
	cd ./tools/ && make
	cd ../

$(DESBIN):$(MSRC)
	$(CC)    -o $@ $^   $(MYINCUDE) $(MYLIB)
%.o:%.c
	$(CC)   -c  $<   $(MYINCUDE)  $(MYLIB)


clean:
	rm *.bin *.o
	rm ./tools/*.o	
