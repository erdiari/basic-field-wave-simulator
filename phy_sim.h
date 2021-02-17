///////////////////////////////////////////////////////////////////////////////
//                                  Inclusions                               //
///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include <cstdlib>
#include <iostream> // TODO added as debug tool will be deleted when finished.
#include <valarray>

///////////////////////////////////////////////////////////////////////////////
//                                  Classes                                  //
///////////////////////////////////////////////////////////////////////////////
// TODO Write comments. EVERYWHERE!

// emulated 2D array.
// class Array2d{
// private:
// public:
//   int w;
//   int h;
//   std::valarray<std::valarray<double>> array;
//   Array2d(){};
//   Array2d(int width, int height){
//     w= width;
//     h=height;
//     // std::valarray<std::valarray<double>> array(w);
//     array = 0;
//   }
//   double get(int i, int j) {return array[index(i,j)];}; // calculations are done so that 1d indexing becomes equal to 2d indexing.
//   int set(int i, int j, double val) {
//     const int k = index(i,j);
//     array[k] = val;
//     return EXIT_SUCCESS;
//   };
// };

class VectorArea2D{
  // TODO replace field_arr with valarray at some point.
public :
  unsigned int w;
  unsigned int h;
  float*** field_arr; // VectorArea array
  void FillVal(float x_bias, float y_bias);
  void initialize(int width, int height){
    w = width;
    h = height;
    field_arr = new float **[w];
    for (int i = 0; i < w; i++) {
      field_arr[i] = new float *[h];
      for (int j = 0; j < h; j++) {
        field_arr[i][j] = new float[2]; // 2 as in 2 dimensions
      };
    };
  };
  VectorArea2D(int width, int height) {initialize(width,height);};
  void AreaPrint();
  // void AreaLenghtPrint(); //FIXME segmentation error
  std::valarray<double> GetVectorLenght();
  void PrintVectorLenght();
  // float GetValue(int i, int j, int d) const {return field_arr[i][j][d];};
};

class Source{
private:
  int n;
public:
  int x,y;
  float pow_coeff;
  bool is_perp = false;
  // Source(){};
  void initialize(int x_pos, int y_pos, float power,bool i_perp, float space_coeff, int ooepsilon) {
    x = x_pos;
    y = y_pos;
    pow_coeff = power;
    is_perp = i_perp;
    n = ooepsilon * sqrt(power * space_coeff); // Number of points to calculate
  };
  void calculate_effect(VectorArea2D area, float space_coeff);
};

class SourcePlane{
public:
  int num_of_sources = 0;
  Source* Sources;
  void add_source(int x, int y, float power,bool is_perp, float space_coeff, int ooepsilon);
  VectorArea2D calculate_effect_vectors(int w, int h, float space_coeff);
};

class ConstantSpace {
  // TODO make an exception at the places with sources. Dont calculate
  // anything there.
public:
  int w, h;
  float x_bias, y_bias;
  float DivCoeff;  // An coefficent between 1 and 16
  float CurlCoeff; // An coefficent between 1 and 16
  int ooeps; // 1/Epsilon, how precise you want your calculations to be.
  SourcePlane Sources;
  ConstantSpace(int width, int height, float x_bis, float y_bias,
                      float div_coeff, float curl_coeff, int one_over_epsilon);
  VectorArea2D get_changefield();
  void add_div_source(int x, int y, float pow){
    Sources.add_source(x, y, pow, false, DivCoeff, ooeps);
  };
  void add_curl_source(int x, int y, float pow) {
    Sources.add_source(x, y, pow, true, DivCoeff, ooeps);
  };
};

///////////////////////////////////////////////////////////////////////////////
//                                Functions //
///////////////////////////////////////////////////////////////////////////////

void VectorArea2D::AreaPrint() {
  std::cout << w << "x" << h << std::endl;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      std::cout << "(" << field_arr[i][j][0] << "," << field_arr[i][j][1]
                << ") ";
    };
    std::cout << ' ' << std::endl;
  };
};

void VectorArea2D::FillVal(float x, float y) {
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      field_arr[i][j][0] = x;
      field_arr[i][j][1] = y;
    };
  };
};

std::valarray<double> VectorArea2D::GetVectorLenght(){
  std::valarray<double> vec_len(w*h);
  for (int i = 0; i < w; ++i) {
    for (int j = 0; j < h; ++j) {
      vec_len[i*w+j] = sqrt((double)(pow(field_arr[i][j][0], 2) + pow(field_arr[i][j][1], 2)));
    };
  };
  return vec_len;
};

void VectorArea2D::PrintVectorLenght(){
  std::valarray<double> vec_len = GetVectorLenght();
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      std::cout << "(" << vec_len[i*w +j] << ") ";
    };
    std::cout << "" << std::endl;
  };
};

void Source::calculate_effect(VectorArea2D area, float space_coeff) {
  // We calculate one quarter than add it to difference places since there is
  // symmetry. //
  // std::cout << space_coeff << "," << pow_coeff << std::endl;
  const float max = space_coeff * pow_coeff; // Maximum it can be.
  // std::cout << max << std::endl;
  for (int r = 1; r <= n; r++) {   // we don't calculate r=0
    for (int k = 1; k <= r; k++) { // we also skip one axis since skipped axis
                                   // will be calculated by rotation.
      const float f = max / pow(r, 2);
      const float cos = k / float(r);
      const float sin = (r - k) / float(r);
      const float f_x = cos * f;
      const float f_y = sin * f;
      for (int xp = 1; xp > -2; xp -= 2) {
        for (int yp = 1; yp > -2; yp -= 2) {
          int x_pos = x + xp * k;
          int y_pos = y + yp * abs(r - k);
          // rotate 90 degrees. Kinda  lame but i don't want to spend mode time
          // on this.
          int xxp;
          int yyp;
          if (is_perp) {
            if (xp == yp) {xxp = -xp; yyp = yp;}
            else {yyp = -yp; xxp = xp;};
          } else {
            xxp = xp;
            yyp = yp;
          };
          if (x_pos >= 0 && y_pos >= 0 && x_pos < area.w && y_pos < area.h){
            area.field_arr[x_pos][y_pos][0] += f_x * xxp;
            area.field_arr[x_pos][y_pos][1] += f_y * yyp;
          };
        };
      };
    };
  };
};

void SourcePlane::add_source(int x, int y, float power, bool is_perp, float space_coeff, int ooepsilon) {
  // Since i don't plan to add sources dynamically i didn't use dynamic list.
  // TODO Write a new function such that you can add multiple sources without
  // all this copying and such.
  // TODO maybe write a function that reads through an array given by gui.
  num_of_sources++;
  Source* New_arr = new Source[num_of_sources];
  for (int i = 0; i < num_of_sources - 1; i++) {
    New_arr[i] = Sources[i];
  };
  Source new_source;
  new_source.initialize(x, y, power, is_perp, space_coeff, ooepsilon);
  New_arr[num_of_sources- 1] = new_source;
  Sources = New_arr;
};

VectorArea2D SourcePlane::calculate_effect_vectors(int w, int h, float space_coeff) {
  VectorArea2D effect_vectors(w, h);
  effect_vectors.FillVal(0, 0);
  for (int i = 0; i < num_of_sources; i++) {
    Sources[i].calculate_effect(effect_vectors, space_coeff);
  };
  return effect_vectors;
};

VectorArea2D ConstantSpace::get_changefield() {
  // VectorArea2D ResDiv =
  return Sources.calculate_effect_vectors(w, h, DivCoeff); // result div
  // VectorArea2D ChangeField(w, h);
  // for (int i = 0; i < w; i++) {
  //   for (int j = 0; j < h; j++) {
  //     ChangeField.field_arr[i][j][0] = ResDiv.field_arr[i][j][0];
  //     ChangeField.field_arr[i][j][1] = ResDiv.field_arr[i][j][1];
  //   };
  // };
  // return ChangeField;
};

ConstantSpace::ConstantSpace(int width, int height, float x_b, float y_b,
                             float div_coeff, float curl_coeff,
                             int one_over_eps) {
  w = width;
  h = height;
  x_bias = x_b;
  y_bias = y_b;
  DivCoeff = div_coeff;
  CurlCoeff = curl_coeff;
  ooeps = one_over_eps;
};
