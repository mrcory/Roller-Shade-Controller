
struct motorSpeedStruct {
  float up;
  float dn;
  float accel;
};

struct motorInvertStruct {
  bool set;
  bool is;
};

//Invert Hold Variable
motorInvertStruct mInvert = {
  false,
  false
};
