#include "phy_sim.h"
// #include "sdl.cpp"

void test(){
  ConstantSpace Space(10,10,0,0,2.0,2.0,5);
  // Space.divplane.add_source(50, 25, 2, Space.DivCoeff, Space.ooeps);
  // Space.divplane.add_source(50, 75, 2, Space.DivCoeff, Space.ooeps);
  Space.add_div_source(2, 5, 2);
  Space.add_div_source(8, 5, 2);
  // Space.add_curl_source(75, 50, 2);
  Space.get_changefield().AreaPrint();
  Space.get_changefield().PrintVectorLenght();
}

int main(){
  test();
  return EXIT_SUCCESS;
}
