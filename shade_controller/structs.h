
struct motorSpeedStruct {
  float up;
  float dn;
  float accel;
};

struct motorInvertStruct {
  bool set;
  bool is;
  bool was;
};

//Invert Hold Variable
motorInvertStruct mInvert = {
  false,
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

//FastLED Struct
struct fastLED {
  bool on;
  bool onOld;
  int set;
  int out;
  int hue;
  int sat;
  int val;
};
