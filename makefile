CXX=g++
CXXFLAGS=-Wall 
EXENAME=roguelike.exe
SRCFILES=$(shell dir /b /s *.cpp)
OBJFILES=$(shell dir /b /s *.o)

all: obj
	$(CXX) $(CXXFLAGS) $(OBJFILES) -lpdcurses -o build\$(EXENAME)
	del $(OBJFILES)

obj:
	$(CXX) $(CXXFLAGS) -Iinclude $(SRCFILES) -lpdcurses -c

objd:
	$(CXX) -g $(CXXFLAGS) -Iinclude $(SRCFILES) -lpdcurses -c

gdb: objd
	$(CXX) -g $(CXXFLAGS) $(OBJFILES) -lpdcurses -o build\$(EXENAME)
	del $(OBJFILES)
	gdb -f build/$(EXENAME)

run:
	build\$(EXENAME)
    
clean:
	del build\$(EXENAME)
	del $(OBJFILES)
