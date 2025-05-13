CXXFLAGS = -Wall -Wextra -std=c++11 -lssl -lcrypto

SRCS = imapcl.cpp error.cpp config.cpp imap.cpp imaps.cpp
OBJS = $(SRCS:.cpp=.o)

all: imapcl

imapcl: $(OBJS)
	g++ -o imapcl $(OBJS) $(CXXFLAGS)

%.o: %.cpp
	g++ $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) imapcl

test:
	./imapcl eva.fit.vutbr.cz -o output -a auth.conf -b Sent

ftest:
	./imapcl eva.fit.vutbr.cz -o output -a auth.conf -b INBOX

stest:
	./imapcl eva.fit.vutbr.cz -o output -a auth.conf -T -c cert.pem

tar:
	tar -cf xsmirn02.tar Makefile README.md imapcl.cpp imap.cpp imap.h imaps.cpp imaps.h error.cpp error.h config.cpp config.h xsmirn02.pdf