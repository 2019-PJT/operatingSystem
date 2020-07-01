

CC = gcc
INC = -I${CURDIR}/include/
CFLAGS = -g $(INC)

OBJS_LAB = lab_sched.o lab_sched_test.o 

SRCS = $(OBJS_LAB:.o=.c)

TARGET_LAB = lab_sched

.SUFFIXES : .c .o

.c.o:
	@echo "Compilingi  scheduler simulator $< ..."
	$(CC) -c $(CFLAGS) -o $@ $<

$(TARGET_LAB) : $(OBJS_LAB)
	$(CC) -o $(TARGET_LAB) $(OBJS_LAB)

all : $(TARGET_LAB)

dep : 
	gccmaedep $(INC) $(SRCS)

clean :
	@echo "Cleaning lab scheduler simulator $< ..."
	rm -rf $(OBJS_LAB) $(TARGET_LAB) 

new :
	$(MAKE) clean
	$(MAKE)
