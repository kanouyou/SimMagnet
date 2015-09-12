TARGET   = SimCooling.exe
TARGET1  = RunAnalysis.exe
CXX      = g++
SRC1     = QuenchMain.cpp QSuperconduct.cpp QMaterial.cpp FillData.cpp
SRC2     = runAnalysis.cpp QAnalysis.cpp
OBJECTS  = $(SRC1:.cpp=.o)
OBJECTS1 = $(SRC2:.cpp=.o)
CXXLIBS  = 
CXXFLAGS = -Wall -O3

OBJS = $(OBJECTS) $(OBJECTS1)
SRCS = $(SRC1) $(SRC2)

ROOTFLAGS = `root-config --cflags`
ROOTLIBS  = `root-config --glibs`

CXXLIBS  += $(ROOTLIBS)
CXXFLAGS += $(ROOTFLAGS)

INSTALLDIR = ./bin

.PHONY: all
all: $(TARGET) $(TARGET1)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXLIBS) $^ -o $@

$(TARGET1): $(OBJECTS1)
	$(CXX) $(CXXLIBS) $^ -o $@

$(OBJS): $(SRCS)
	$(CXX) $(CXXFLAGS) -c $(SRCS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJECTS) $(TARGET1) $(OBJECTS1)

.PHONY: install
install:
	mkdir -p $(INSTALLDIR)
	cp -p $(TARGET) $(TARGET1) $(INSTALLDIR)
