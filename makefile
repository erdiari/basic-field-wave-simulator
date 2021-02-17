CPPC=g++
CPPFLAGS=-lSDL2 -g#link sdl2

main:phy_sim.cpp
	[[ -d build ]] || mkdir build
	$(CPPC) $(CPPFLAGS) $? -o build/$@.out && build/$@.out

debug:phy_sim.cpp
	[[ -d build ]] || mkdir build
	$(CPPC) $(CPPFLAGS) $? -o build/$@.out && gdb build/$@.out

sdl:sdl.cpp
	[[ -d build ]] || mkdir build
	$(CPPC) $(CPPFLAGS) $? -o build/$@.out && build/$@.out
