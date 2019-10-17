.phony: all clean

EXE = ge
IMGUI_BASE = imgui

CXXFLAGS = -I./$(IMGUI_BASE)
CXXFLAGS += -g -Wall
CXXFLAGS += `sdl2-config --cflags`

LIBS += -lGL -ldl `sdl2-config --libs`

SOURCES = main.cpp

SOURCES += $(wildcard $(IMGUI_BASE)/*.cpp)

OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))


%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_BASE)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)


$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean: 
	rm -f $(EXE) $(OBJS)
