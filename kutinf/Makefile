CXX     = g++ # egyelőre csakis gcc a beágyazott helper függvények miatt

DBGFLAGS = -Wall -Wextra
#-DDEBUG -g

PARAFLAGS= -DVLEN=4 -DTHREADED -fopenmp -I/usr/include/SDL2 -D_REENTRANT
OPTIMFLAGS= -O1
# -march=znver1 -mprefer-vector-width=256 -flto -fdevirtualize-at-ltrans -fno-plt -fno-common -fipa-pta -fno-semantic-interposition -fgraphite-identity -floop-nest-optimize

CXXFLAGS = --std=c++17 $(DBGFLAGS) $(OPTIMFLAGS) $(PARAFLAGS)
LDFLAGS = -flto=6 -pthread -lSDL2 

SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc=.o)
HDRS = $(SRCS:.cc=.h)

all: field

field: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(LDFLAGS)

TAGS: $(SRCS)
	ctags -Re $(SRCS)

.PHONY: clean
clean:
	rm -fv $(OBJS) field TAGS

.PHONY: lint
lint:
	clang-tidy $(SRCS) $(HDRS) -- $(CXXFLAGS)
