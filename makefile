CC = g++
CPPFLAGS =  -Wall -ansi -pedantic -O3
LIB_DIR=strmat

objects = kdifferenceprime4.o $(LIB_DIR)/SSTree.o $(LIB_DIR)/Tools.o $(LIB_DIR)/CSA.o $(LIB_DIR)/BitRank.o \
  $(LIB_DIR)/ReplacePattern.o $(LIB_DIR)/wtreebwt.o $(LIB_DIR)/bittree.o $(LIB_DIR)/rbtree.o $(LIB_DIR)/CHgtArray.o $(LIB_DIR)/CRMQ.o \
  $(LIB_DIR)/SubblockRMQ.o $(LIB_DIR)/Parentheses.o $(LIB_DIR)/Hash.o $(LIB_DIR)/LcpToParentheses.o

all: clean k4 default

k4:
	$(CC)  -Wfatal-errors -Wall  -O3  -Wextra -std=c++11 -c arvore.cpp -o arvore.o 

default: $(objects)
	$(CC) -o arvore $(objects) 

clean:
	-rm -f core *.o  *~ arvore 