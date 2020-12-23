// Copyleft (c) 2012, Zach Vorhies
// Public domain, no rights reserved.
// This object holds a frame buffer and effects can be applied. This is a higher level
// object than the TCL class which this object uses for drawing.

#ifndef LED_REPE_TCL_H_
#define LED_REPE_TCL_H_

#include <Arduino.h>
#include "tcl.h"

// LedRopeTCL is a C++ wrapper around the Total Control Lighting LED rope
// device driver (TCL.h). This wrapper includes automatic setup of the LED
// rope and allows the user to use a graphics-state like interface for
// talking to the rope. A copy of the rope led state is held in this class
// which makes blending operations easier. After all changes by the user
// are applied to the rope, the hardware is updated via an explicit Draw()
// command.
//
// Whole-rope blink Example:
//  #include <SPI.h>
//  #include <TCL.h>          // From CoolNeon (https://bitbucket.org/devries/arduino-tcl)
//  #include "LedRopeTCL.h"
//  LedRopeTCL led_rope(100); // 100 led-strand.
//
//  void setup() {}  // No setup necessary for Led rope.
//  void loop() {
//    led_rope.FillColor(LedRopeTCL::Color3i::Black());
//    led_rope.Draw();
//    delay(1000);
//    led_rope.FillColor(LedRopeTCL::Color3i::White());
//    led_rope.Draw();
//    delay(1000);
//  }

struct Color3i {
  static Color3i Black() { return Color3i(0x0, 0x0, 0x0); }
  static Color3i White() { return Color3i(0xff, 0xff, 0xff); }
  static Color3i Red() { return Color3i(0xff, 0x00, 0x00); }
  static Color3i Orange() { return Color3i(0xff, 0xff / 2,00); }
  static Color3i Yellow() { return Color3i(0xff, 0xff,00); }
  static Color3i Green() { return Color3i(0x00, 0xff, 0x00); }
  static Color3i Cyan() { return Color3i(0x00, 0xff, 0xff); }
  static Color3i Blue() { return Color3i(0x00, 0x00, 0xff); }
  Color3i(byte r, byte g, byte b) { Set(r,g,b); }
  Color3i() { Set(0xff, 0xff, 0xff); }
  Color3i(const Color3i& other) { Set(other); }

  void Set(byte r, byte g, byte b) { r_ = r; g_ = g; b_ = b; }
  void Set(const Color3i& c) { Set(c.r_, c.g_, c.b_); }
  void Mul(const Color3i& other_color);
  void Mulf(float scale);  // Input range is 0.0 -> 1.0
  void Mul(byte val) {
    Mul(Color3i(val, val, val));
  }
  void Sub(const Color3i& color);
  void Add(const Color3i& color);
  const byte Get(int rgb_index) const;
  void Set(int rgb_index, byte val);
  void Fill(byte val) { Set(val, val, val); }
  byte MaxRGB() const {
    byte max_r_g = r_ > g_ ? r_ : g_;
    return max_r_g > b_ ? max_r_g : b_;
  }

  inline void Print(HardwareSerial* serial) const {
	serial->print("RGB:\t");
    serial->print(r_); serial->print(",\t");
    serial->print(g_); serial->print(",\t");
    serial->println(b_);
  }

  void Interpolate(const Color3i& other_color, float t);

  byte* At(int rgb_index);
  const byte* At(int rgb_index) const;

  byte r_, g_, b_;
};


struct ColorHSV {
  ColorHSV() : h_(0), s_(0), v_(0) {}
  ColorHSV(float h, float s, float v) {
    Set(h,s,v);
  }
  explicit ColorHSV(const Color3i& color) {
    FromRGB(color);
  }
  ColorHSV(const ColorHSV& other) {
    Set(other);
  }
  void Set(const ColorHSV& other) {
	Set(other.h_, other.s_, other.v_);
  }
  void Set(float h, float s, float v) {
    h_ = h;
    s_ = s;
    v_ = v;
  }
  inline void Print(HardwareSerial* serial) {
	serial->print("HSV:\t");
    serial->print(h_); serial->print(",\t");
    serial->print(s_); serial->print(",\t");
    serial->println(v_);
  }

  bool operator==(const ColorHSV& other) const {
    return h_ == other.h_ && s_ == other.s_ && v_ == other.v_;
  }
  bool operator!=(const ColorHSV& other) const {
    return !(*this == other);
  }
  void FromRGB(const Color3i& rgb) {
    typedef double FloatT;
    FloatT r = (FloatT) rgb.r_/255.f;
    FloatT g = (FloatT) rgb.g_/255.f;
    FloatT b = (FloatT) rgb.b_/255.f;
    FloatT max_rgb = max(r, max(g, b));
    FloatT min_rgb = min(r, min(g, b));
    v_ = max_rgb;

    FloatT d = max_rgb - min_rgb;
    s_ = max_rgb == 0 ? 0 : d / max_rgb;

    if (max_rgb == min_rgb) {
      h_ = 0; // achromatic
    } else {
      if (max_rgb == r) {
        h_ = (g - b) / d + (g < b ? 6 : 0);
      } else if (max_rgb == g) {
        h_ = (b - r) / d + 2;
      } else if (max_rgb == b) {
        h_ = (r - g) / d + 4;
      }
      h_ /= 6;
    }
  }

  Color3i ToRGB() const {
    typedef double FloatT;
    FloatT r, g, b;

    int i = int(h_ * 6);
    FloatT f = h_ * 6.0 - static_cast<FloatT>(i);
    FloatT p = v_ * (1.0 - s_);
    FloatT q = v_ * (1.0 - f * s_);
    FloatT t = v_ * (1.0 - (1.0 - f) * s_);

    switch(i % 6){
      case 0: r = v_, g = t, b = p; break;
      case 1: r = q, g = v_, b = p; break;
      case 2: r = p, g = v_, b = t; break;
      case 3: r = p, g = q, b = v_; break;
      case 4: r = t, g = p, b = v_; break;
      case 5: r = v_, g = p, b = q; break;
    }

    return Color3i(round(r * 255), round(g * 255), round(b * 255));
  }

  float h_, s_, v_;
};

class FrameBufferBase {
 public:
  FrameBufferBase(Color3i* array, int n_pixels)
  		: color_array_(array), n_color_array_(n_pixels) {}
  virtual ~FrameBufferBase() {}

  void Set(int i, const Color3i& c) { color_array_[i] = c; }
  void Set(int i, int length, const Color3i& color) {
    for (int j = 0; j < length; ++j) {
      Set(i + j, color);
    }
  }

  void FillColor(const Color3i& color) {
    for (int i = 0; i < n_color_array_; ++i) {
      color_array_[i] = color;
    }
  }

  void ApplyBlendSubtract(const Color3i& color) {
    for (int i = 0; i < n_color_array_; ++i) {
	  color_array_[i].Sub(color);
    }
  }

  void ApplyBlendAdd(const Color3i& color) {
    for (int i = 0; i < n_color_array_; ++i) {
      color_array_[i].Add(color);
    }
  }

  void ApplyBlendMultiply(const Color3i& color) {
    for (int i = 0; i < n_color_array_; ++i) {
      color_array_[i].Mul(color);
    }
  }

  Color3i* GetIterator(int i) {
    return color_array_ + i;
  }
  const Color3i& Get(int i) const { return color_array_[i]; }

  // Length in pixels.
  int length() const { return n_color_array_; }

 protected:
  Color3i* color_array_;
  int n_color_array_;
};

class FrameBuffer : public FrameBufferBase {
 public:
  FrameBuffer(int n_pixels)
      : FrameBufferBase(static_cast<Color3i*>(malloc(sizeof(Color3i) * n_pixels)),
                        n_pixels) {
  }

  virtual ~FrameBuffer() {
    free(color_array_);
    color_array_ = NULL;
    n_color_array_ = 0;
  }
};

class LedRopeTCL : public FrameBuffer {
 public:
  LedRopeTCL(int n_pixels);
  virtual ~LedRopeTCL();

  void Draw();
  void DrawSequentialRepeat(int repeat);
  void DrawRepeat(const int* value_array, int array_length);
  void set_draw_offset(int val) {
    draw_offset_ = constrain(val, 0, n_color_array_);
  }
  
  // The following classes should eventually be split into a LedRopePainterTCL
    
  void RawBeginDraw() {
    PreDrawSetup();
    tcl_.sendEmptyFrame();
  }
  void RawDrawPixel(const Color3i& c) {
    tcl_.sendColor(c.r_, c.g_, c.b_);
  }
  void RawDrawPixel(byte r, byte g, byte b) {
    tcl_.sendColor(r, g, b);
  }
  
  void RawCommitDraw() {
    tcl_.sendEmptyFrame();
  }

 protected:
  void PreDrawSetup() {
    if (!lazy_initialized_) {
      tcl_.begin();
      tcl_.sendEmptyFrame();
      // Clears upto 100,000 lights.
      for (long i = 0; i < 100000; ++i) {
        RawDrawPixel(0, 0, 0);
      }
      tcl_.sendEmptyFrame();
      lazy_initialized_ = true;
    }
  }
  int draw_offset_;
  TclClass tcl_;
  bool lazy_initialized_;
};




///////////////////////////////////////////////////////////////////////////////
LedRopeTCL::LedRopeTCL(int n_pixels)
	: FrameBuffer(n_pixels), draw_offset_(0), lazy_initialized_(false) {
}

///////////////////////////////////////////////////////////////////////////////
LedRopeTCL::~LedRopeTCL() {
}

///////////////////////////////////////////////////////////////////////////////
void LedRopeTCL::Draw() {
  RawBeginDraw();

  const Color3i* begin = GetIterator(0);
  const Color3i* middle = GetIterator(draw_offset_);
  const Color3i* end = GetIterator(length());

  for (const Color3i* it = middle; it != end; ++it) {
    RawDrawPixel(*it);
  }
  for (const Color3i* it = begin; it != middle; ++it) {
    RawDrawPixel(*it);
  }
  RawCommitDraw();
}

///////////////////////////////////////////////////////////////////////////////
void LedRopeTCL::DrawSequentialRepeat(int repeat) {
  RawBeginDraw();

  const Color3i* begin = GetIterator(0);
  const Color3i* middle = GetIterator(draw_offset_);
  const Color3i* end = GetIterator(length());

  for (const Color3i* it = middle; it != end; ++it) {
    for (int i = 0; i < repeat; ++i) {
      RawDrawPixel(it->r_, it->g_, it->b_);
    }
  }
  for (const Color3i* it = begin; it != middle; ++it) {
    for (int i = 0; i < repeat; ++i) {
      RawDrawPixel(it->r_, it->g_, it->b_);
    }
  }
  RawCommitDraw();
}

///////////////////////////////////////////////////////////////////////////////
void LedRopeTCL::DrawRepeat(const int* value_array, int array_length) {
  RawBeginDraw();
  
  // Make sure that the number of colors to repeat does not exceed the length
  // of the rope.
  const int len = min(array_length, n_color_array_);

  for (int i = 0; i < len; ++i) {
     const Color3i* cur_color = GetIterator(i);  // Current color.
     const int repeat_count = value_array[i];
     // Repeatedly send the same color down the led rope.
     for (int k = 0; k < repeat_count; ++k) {
       RawDrawPixel(cur_color->r_, cur_color->g_, cur_color->b_);
     }
  }
  // Finish the drawing.
  RawCommitDraw();
}

///////////////////////////////////////////////////////////////////////////////
void Color3i::Mul(const Color3i& other_color) {
  int r = r_;
  int g = g_;
  int b = b_;

  r = r * int(other_color.r_) / 255;
  g = g * int(other_color.g_) / 255;
  b = b * int(other_color.b_) / 255;
  Set(r, g, b);
}

///////////////////////////////////////////////////////////////////////////////
void Color3i::Mulf(float scale) {
  const int s = static_cast<int>(scale * 255.0f);

  int r = static_cast<int>(r_) * s / 255;
  int g = static_cast<int>(g_) * s / 255;
  int b = static_cast<int>(b_) * s / 255;

  Set(r, g, b);
}

///////////////////////////////////////////////////////////////////////////////
void Color3i::Sub(const Color3i& color) {
  if (r_ < color.r_) r_ = 0;
  else               r_ -= color.r_;
  if (g_ < color.g_) g_ = 0;
  else               g_ -= color.g_;
  if (b_ < color.b_) b_ = 0;
  else               b_ -= color.b_;
}

///////////////////////////////////////////////////////////////////////////////
void Color3i::Add(const Color3i& color) {
  if ((255 - r_) < color.r_) r_ = 255;
  else                       r_ += color.r_;
  if ((255 - g_) < color.g_) g_ = 255;
  else                       g_ += color.g_;
  if ((255 - b_) < color.b_) b_ = 255;
  else                       b_ += color.b_;
}

///////////////////////////////////////////////////////////////////////////////
const byte Color3i::Get(int rgb_index) const {
  const byte* rgb = At(rgb_index);
  return rgb ? *rgb : 0;
}

///////////////////////////////////////////////////////////////////////////////
void Color3i::Set(int rgb_index, byte val) {
  byte* rgb = At(rgb_index);
  if (rgb) {
    *rgb = val;
  }
}

///////////////////////////////////////////////////////////////////////////////
void Color3i::Interpolate(const Color3i& other_color, float t) {
  if (0.0f >= t) {
    Set(other_color);
  } else if (1.0f <= t) {
    return;
  }

  Color3i new_color = other_color;
  new_color.Mul(1.0f - t);
  this->Mul(t);
  this->Add(new_color);
}

///////////////////////////////////////////////////////////////////////////////
byte* Color3i::At(int rgb_index) {
  switch(rgb_index) {
    case 0: return &r_;
    case 1: return &g_;
    case 2: return &b_;
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
const byte* Color3i::At(int rgb_index) const {
  switch(rgb_index) {
    case 0: return &r_;
    case 1: return &g_;
    case 2: return &b_;
  }
  return NULL;
}


#endif  // LED_REPE_TCL_H_
