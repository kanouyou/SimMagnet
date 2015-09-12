TARGET   = SimCooling.exe
TARGET1  = RunAnalysis.exe
CXX      = g++
OBJECTS  = QuenchMain.o QSuperconduct.o QMaterial.o FillData.o
OBJECTS1 = runAnalysis.o QAnalysis.o
CXXLIBS  = 
CXXFLAGS = -Wall -O3

ROOTFLAGS = `root-config --cflags`
ROOTLIBS  = `root-config --evelibs`

CXXLIBS  += $(ROOTLIBS)
CXXFLAGS += $(ROOTFLAGS)

INSTALLDIR = ./bin

.PHONY: all
all: $(TARGET) $(TARGET1)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXLIBS) $^ -o $@

$(TARGET1): $(OBJECTS1)
	$(CXX) $(CXXLIBS) $^ -o $@

.o:.cpp
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJECTS) $(TARGET1) $(OBJECTS1)

.PHONY: install
install:
	mkdir -p $(INSTALLDIR)
	cp -p $(TARGET) $(TARGET1) $(INSTALLDIR)
