# Makefile for SolTrack

COMP = gcc
OPT = -O2
CFLAGS = -Wall -Wextra -std=c11 -pedantic -fmax-errors=10
LFLAGS = -lm

OBJDIR = obj
STDOBJ = $(OBJDIR)/SolTrack.o $(OBJDIR)/SolTrack_RiseSet.o


all: TestSolTrackAccuracy TestSolTrackFile TestSolTrackSingle TestSolTrackSpeed TestSolTrackRiseSet

# Link object files and create a binary executable:
TestSolTrackAccuracy: $(OBJDIR)/TestSolTrackAccuracy.o libSolTrack.a
	$(COMP) $(LFLAGS) -o TestSolTrackAccuracy $(OBJDIR)/TestSolTrackAccuracy.o libSolTrack.a

TestSolTrackFile: $(OBJDIR)/TestSolTrackFile.o libSolTrack.a
	$(COMP) $(LFLAGS) -o TestSolTrackFile $(OBJDIR)/TestSolTrackFile.o libSolTrack.a

TestSolTrackSingle: $(OBJDIR)/TestSolTrackSingle.o libSolTrack.a
	$(COMP) $(LFLAGS) -o TestSolTrackSingle $(OBJDIR)/TestSolTrackSingle.o libSolTrack.a

TestSolTrackSpeed: $(OBJDIR)/TestSolTrackSpeed.o libSolTrack.a
	$(COMP) $(LFLAGS) -o TestSolTrackSpeed $(OBJDIR)/TestSolTrackSpeed.o libSolTrack.a

TestSolTrackRiseSet: $(OBJDIR)/TestSolTrackRiseSet.o libSolTrack.a
	$(COMP) $(LFLAGS) -o TestSolTrackRiseSet $(OBJDIR)/TestSolTrackRiseSet.o libSolTrack.a

libSolTrack.a: $(STDOBJ)
	ar rs libSolTrack.a $(STDOBJ)


# Compile C-files and create object files (.o).
$(OBJDIR)/%.o: %.c SolTrack.c
	$(COMP) $(OPT) $(CFLAGS) -c $< -o $@



clean:
	rm -f : $(OBJDIR)/*.o

cleanall: clean
	rm -f libSolTrack.a TestSolTrackAccuracy TestSolTrackFile TestSolTrackSingle TestSolTrackSpeed TestSolTrackRiseSet

