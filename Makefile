#COMPLIER

CXX = g++
NVXX = nvcc

CXXFLAGS = 
NVXXFLAGS = 

TARGETS = test1.out

SRC_CPP = ./main.cpp
SRC_CU = ./cuda.cu

OBJS_CPP := $(patsubst %.cpp, %.o, $(SRC_CPP))
OBJS_CU := $(patsubst %.cu, %.o, $(SRC_CU))

all:$(TARGETS)

test1.out: $(OBJS_CPP) $(OBJS_CU)
	$(NVXX) $^ -o $@

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.o: %.cu
	$(NVXX) -c $< -o $@ $(NVXXFLAGS)
