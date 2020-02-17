
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

//PWM Struct
struct pwmStruct {
  int old;
  int set;
  bool on;
  bool onOld;
  int out;
};
