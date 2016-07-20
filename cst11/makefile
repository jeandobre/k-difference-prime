CC = g++
CPPFLAGS = -Wall -ansi -pedantic -O3

objects = TestLCSS.o SSTree.o Tools.o CSA.o BitRank.o \
  ReplacePattern.o wtreebwt.o bittree.o rbtree.o CHgtArray.o CRMQ.o \
  SubblockRMQ.o Parentheses.o Hash.o LcpToParentheses.o

default: $(objects)
	$(CC) -o TestLCSS $(objects)

clean:
	rm -f core *.o *~ TestLCSS 

depend:
	g++ -MM *.cpp > dependencies.mk

include dependencies.mk
