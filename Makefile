#COMPLIER

CXX = g++
NVXX = nvcc
PROTOC = /home/hibbert/library/usr/local/bin/protoc

#FLAGS
CXXFLAGS = -std=c++11
CPPFLAGS = -I/home/hibbert/library/usr/local/include -I/home/hibbert/grpc/grpc/include -pthread
NVXXFLAGS = 
LINKFLAGS = -L/usr/local/cuda/lib64 -lcuda -lcudart
LDFLAGS += -L/home/hibbert/library/usr/local/lib -L/home/hibbert/grpc/grpc/libs/opt -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lpthread -ldl
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

#OTHER
TARGETS = test1.out

PROTOS_PATH = ./proto
SRC_CPP = ./main.cpp
SRC_CU = ./cuda.cu
SRC_PROTO = helloworld.pb.h helloworld.grpc.pb.h

OBJS_CPP := $(patsubst %.cpp, %.o, $(SRC_CPP))
OBJS_CU := $(patsubst %.cu, %.o, $(SRC_CU))
OBJS_PROTO := $(patsubst %.pb.h, %.pb.o, $(SRC_PROTO))

vpath %.proto $(PROTOS_PATH)

#MAKE
all:$(TARGETS)

test1.out: $(OBJS_PROTO) $(OBJS_CPP) $(OBJS_CU)
	$(CXX) $^ $(LDFLAGS) -o $@ $(LINKFLAGS)

%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(CPPFLAGS)

%.o: %.cu
	$(NVXX) -c $< -o $@ $(NVXXFLAGS)

clean:
	rm -rf *.o $(TARGETS)
