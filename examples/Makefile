# Simple Makefile to compile SolTrack test examples

COMP = gcc
OPT = -O2
CFLAGS = -Wall -Wextra -std=c11 -pedantic -fmax-errors=10
LFLAGS = -lm -lSolTrack

OBJDIR = obj
STDOBJ = $(OBJDIR)/SolTrack.o $(OBJDIR)/SolTrack_RiseSet.o


# Default target: make all:
all: TestSolTrackFile TestSolTrackSingle


# Link object files and create a binary executable:
TestSolTrackFile: $(OBJDIR)/TestSolTrackFile.o
	$(COMP) $(LFLAGS) -o TestSolTrackFile $(OBJDIR)/TestSolTrackFile.o

TestSolTrackSingle: $(OBJDIR)/TestSolTrackSingle.o
	$(COMP) $(LFLAGS) -o TestSolTrackSingle $(OBJDIR)/TestSolTrackSingle.o


# Compile C-files and create object files (.o).
$(OBJDIR)/%.o: %.c $(OBJDIR)
	$(COMP) $(OPT) $(CFLAGS) -c $< -o $@


# Create object directory:
$(OBJDIR):
	mkdir $(OBJDIR)


clean:
	rm -f : $(OBJDIR)/*.o

cleanall: clean
	rm -f TestSolTrackFile TestSolTrackSingle
	rm -rf obj/

