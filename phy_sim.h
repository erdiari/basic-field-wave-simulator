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
//                                  TODO fix the names of all stuff.
//                                  It is all over the place.

class VectorArea2D{
  // 2X2 Array of vectors /////////////////////////////////////////////////////
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
  std::valarray<double> GetVectorLenght();
  void PrintVectorLenght();
  float GetValue(int i, int j, int d) const {return field_arr[i][j][d];};
};

class Source{
private:
  int n;
public:
  int x,y; // Positional arguments
  float pow_coeff;
  bool is_perp;
  void initialize(int x_pos, int y_pos, float power,
                  bool i_perp, float space_coeff,
                  int ooepsilon) {
    x = x_pos; // X pos
    y = y_pos; // Y pos
    pow_coeff = power; // Power coefficent. Can be negative.
    is_perp = i_perp; // if true will create perpendecular vector.
    n = ooepsilon * sqrt(fabs(power) * space_coeff); // Number of points to calculate
  };
  void calculate_effect(VectorArea2D area, float space_coeff); // Calculate effect. Will add on to the passed area.
};

class SourcePlane{
  // A class to hold and do calculations with sources. ////////////////////////
public:
  int num_of_sources = 0;
  Source* Sources; // Pointer to array of sources.
  void add_source(int x, int y, float power,bool is_perp, float space_coeff, int ooepsilon); // Add an source
  VectorArea2D calculate_effect_vectors(int w, int h, float space_coeff);
};

class ConstantSpace {
  // TODO make an exception at the places with sources. Dont calculate
  // anything there.
public:
  int w, h; // Width and height of given space.
  float x_bias, y_bias; //Unaffected normal values of x and values.
  float DivCoeff;  // An coefficent to calculate perpenducal sources.
  float CurlCoeff; // An coefficent to calculate straight sources.
  int ooeps; // 1/Epsilon, how precise you want your calculations to be.
  SourcePlane Sources; // Sources.
  ConstantSpace(int width, int height, float x_bis, float y_bias,
                      float div_coeff, float curl_coeff, int one_over_epsilon);
  VectorArea2D get_changefield(); //Get Change Vector Field
  void add_div_source(int x, int y, float pow){
    Sources.add_source(x, y, pow, false, DivCoeff, ooeps);
  };
  void add_curl_source(int x, int y, float pow) {
    Sources.add_source(x, y, pow, true, DivCoeff, ooeps);
  };
};

///////////////////////////////////////////////////////////////////////////////
//                                Functions                                  //
///////////////////////////////////////////////////////////////////////////////

void VectorArea2D::AreaPrint() {
  // Print the vector space.
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
  const float max = space_coeff * pow_coeff; // Maximum it can be.
  for (int r = 1; r <= n; r++) {   // we don't calculate r=0
    for (int k = 0; k <= r; k++) { // we also skip one axis since skipped axis
                                   // will be calculated by rotation.
      const float f = max / pow(r, 2); // f value to 
      const float _cos = k / (float) r; // cos value
      const float _sin = (r - k) / (float) r; // sin value

      float f_x = f * _cos; // X component of function.
      float f_y = f * _sin; // Y component of the function.
      // rotate 90 degrees. Kinda  lame but i don't want to spend mode time
      // on this.
      if (is_perp) {
        float holder = f_x;
        f_x = f_y;
        f_y = -holder;
      };
      const int x_pos_off = k;       // x position offset.
      const int y_pos_off = (r - k); // y position offset.
      // Another lame implementation.
      // This time it is the implementation putting our values to coordinate quarters.
      if (x_pos_off == 0){ 
        const int yp_pos = y + y_pos_off;
        if (yp_pos < area.h) {
          area.field_arr[x][yp_pos][0] += f_x;
          area.field_arr[x][yp_pos][1] += f_y;
        };
        const int yn_pos = y - y_pos_off;
        if ( yn_pos >= 0){
          area.field_arr[x][yn_pos][0] -= f_x;
          area.field_arr[x][yn_pos][1] -= f_y;
        };
      } else if (y_pos_off == 0) {
          const int xp_pos = x + x_pos_off;
          if (xp_pos < area.w){
            area.field_arr[xp_pos][y][0] += f_x;
            area.field_arr[xp_pos][y][1] += f_y;
          };
          const int xn_pos = x - x_pos_off;
          if (xn_pos >= 0) {
            area.field_arr[xn_pos][y][0] -= f_x;
            area.field_arr[xn_pos][y][1] -= f_y;
          };
      } else {
          for (int v1 = 1; v1 > -2; v1 -= 2) {
            for (int v2 = 1; v2 > -2; v2 -= 2) {
              int x_pos = v1 * x_pos_off + x;
              int y_pos = v2 * y_pos_off + y;
              if (x_pos >= 0 && x_pos < area.w && y_pos >= 0 && y_pos < area.h){
                area.field_arr[x_pos][y_pos][0] += f_x * v1;
                area.field_arr[x_pos][y_pos][1] += f_y * v2;
              }
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
  return Sources.calculate_effect_vectors(w, h, DivCoeff); // result div
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
