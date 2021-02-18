#include "phy_sim.h"

void test(){
  ConstantSpace Space(10,10,0,0,2.0,2.0,5);
  Space.add_div_source(2, 5, 2);
  Space.add_div_source(8, 5, 2);
  Space.get_changefield().AreaPrint();
  Space.get_changefield().PrintVectorLenght();
}

int main(){
  test();
  return EXIT_SUCCESS;
}
