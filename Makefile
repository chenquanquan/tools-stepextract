COMPILE ?= 
CC := $(COMPILE)gcc
LD := $(COMPILE)ld
OBJDUMP := $(COMPILE)objdump
#LD_FLAG := -lm -lc -lgcc
LD_FLAG := -lm
CC_FLAG := $(LD_FLAG)

# compile object
objs += main.o

stepextract.bin: $(objs)
	$(CC) -oS $(LD_FLAG) -o $@ $(objs)
	$(OBJDUMP) -d $@ > $@.asm

%.o:%.c
	$(CC) -Wall $(CC_FLAG) -c $<

.PHONY:clean
clean:
	rm -rf *.o *.bin *.asm 

.PHONY:distclean
distclean:
	rm -rf *.o *.bin *.asm tags cscope.*
	rm -rf Session.vim vim.viminfo filenametags *.dat
