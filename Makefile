CC=g++
CFLAGS=-I$(IDIR) -Wall

%.o: %.cc
	$(CC) -c -o $@ $< $(CFLAGS)

energy: energy.o state.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core energy