ZIP=zip

ARCHIVE=supplementary-tests

all: pack

pack: $(ARCHIVE).$(ZIP)

$(ARCHIVE).$(ZIP): *
	@echo \#Packing $@ ...
	$(ZIP) -r $@ * -x Makefile TODO.txt *.ippcode
	@echo

clean:
	rm -f $(ARCHIVE).$(ZIP)
