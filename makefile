CPPC=g++
CPPFLAGS=

main: phy_sim.cpp sdl.cpp
	[[ -d build ]] || mkdir build
	$(CPPC) $(CPPFLAGS) $? -o build/$@.out && build/$@.out

sdl:sdl.cpp
	[[ -d build ]] || mkdir build
	$(CPPC) $(CPPFLAGS) $? -o build/$@.out && build/$@.out
