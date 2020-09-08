CROSS_COMPILE 	=arm-none-linux-gnueabi-
CC		= $(CROSS_COMPILE)g++
AR    = $(CROSS_COMPILE)ar
CFLAGS  =-w -O3
ARCFLAGS = -march=armv7-a -mtune=cortex-a8 -mfpu=neon -ftree-vectorize -ffast-math -mfloat-abi=softfp

LIB_ARM9=./lib/arm9/libunicode_character_set.a
LIB_A8=./lib/a8/liba8.a

OBJDIR_ARM9 = obj/arm9
OBJDIR_A8 = obj/a8

vpath %.cpp src
SRCS = $(notdir $(wildcard src/*.cpp))

OBJS_ARM9 = $(patsubst %.cpp,$(OBJDIR_ARM9)/%.o,$(SRCS))
OBJS_A8 = $(patsubst %.cpp,$(OBJDIR_A8)/%.o,$(SRCS))

all: $(LIB_ARM9) $(OBJS_ARM9) $(LIB_A8) $(OBJS_A8)

$(LIB_ARM9):$(OBJS_ARM9)
	$(AR) rcs $(LIB_ARM9) $(OBJS_ARM9)

$(OBJS_ARM9): $(OBJDIR_ARM9)/%.o : %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(LIB_A8):$(OBJS_A8)
	$(AR) rcs $(LIB_A8) $(OBJS_A8)

$(OBJS_A8): $(OBJDIR_A8)/%.o : %.cpp
	$(CC) -c $(CFLAGS) $(ARCFLAGS) $< -o $@

