# -*- coding: utf-8 -*-

ARCH:=$(shell uname -m)
CC=gcc
EXEC_M=mandelbrot-$(ARCH)
EXEC_J=julia-$(ARCH)
CFLAGS=-Wall -Werror -std=c99 -pedantic -O2 -march=native -pipe -fomit-frame-pointer -pthread -fopenmp -flto
SDL_CFLAGS:=$(shell sdl-config --cflags)
SDL_LFLAGS:=$(shell sdl-config --libs)
SRC=$(wildcard *.c)
OBJ_M=$(SRC:%.c=build/mdlbr/%.o)
OBJ_J=$(SRC:%.c=build/julia/%.o)

CONVERT=convert
CONVERT_OPTS=-define png:color-type=3  -define png:bit-depth=8  -quality 90
BMP=$(wildcard *.bmp)
PNG=$(BMP:%.bmp=%.png)
DELAY=100

export OMP_NUM_THREADS:=$(shell nproc --all)

#------------------------------------------------------------------------------

.PHONY: all png clean mrproper

#------------------------------------------------------------------------------

all: $(EXEC_M) $(EXEC_J)

$(EXEC_M): $(OBJ_M)
	$(CC)  $(CFLAGS) $(SDL_LFLAGS)  -o $@  $^

$(EXEC_J): $(OBJ_J)
	$(CC)  $(CFLAGS) $(SDL_LFLAGS)  -o $@  $^

$(OBJ_M): build/mdlbr/%.o: %.c
	@mkdir -p $(@D)
	$(CC)  $(CFLAGS) $(SDL_CFLAGS)  -o $@  -c $<

$(OBJ_J): build/julia/%.o: %.c
	@mkdir -p $(@D)
	$(CC)  $(CFLAGS) $(SDL_CFLAGS) -DJULIA  -o $@  -c $<

#------------------------------------------------------------------------------

png: $(PNG)

anim: png
	$(CONVERT)  -delay $(DELAY)  -loop 0  *.png  $@.gif

$(PNG): %.png: %.bmp
	$(CONVERT)  $<  $(CONVERT_OPTS)  $@
	-@rm  $<

#------------------------------------------------------------------------------

mrproper: clean
	-@rm -v  $(EXEC_M) $(EXEC_J)

clean:
	-@rm -Rv  build/

#------------------------------------------------------------------------------

