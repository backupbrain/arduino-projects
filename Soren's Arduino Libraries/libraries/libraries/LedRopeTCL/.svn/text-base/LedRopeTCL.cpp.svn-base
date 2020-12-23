// Copyright (c) 2012, Zach Vorhies

#include <LedRopeTCL.h>
#include <SPI.h>
#include <TCL.h>

///////////////////////////////////////////////////////////////////////////////
LedRopeTCL::LedRopeTCL(int n_leds) : n_color_array_(n_leds) {
  TclClass::begin();
  color_array_ =
      static_cast<Color3i*>(malloc(sizeof(Color3i) * n_color_array_));
  set_draw_offset(0);
  FillColor(Color3i::Black());
  Draw();
}

///////////////////////////////////////////////////////////////////////////////
LedRopeTCL::~LedRopeTCL() {
  free (color_array_);
}

///////////////////////////////////////////////////////////////////////////////
void LedRopeTCL::Draw() {
  TclClass::sendEmptyFrame();
  if (draw_offset_ == 0) {
    for (int i = 0; i < n_color_array_; ++i) {
      const Color3i& color = color_array_[i];
      TclClass::sendColor(color.r_, color.g_, color.b_);
    }
  } else {
    for (int i = 0; i < n_color_array_; ++i) {
      const Color3i& color = color_array_[(i + draw_offset_) % n_color_array_];
      TclClass::sendColor(color.r_, color.g_, color.b_);
    }
  }
  TclClass::sendEmptyFrame();
}

///////////////////////////////////////////////////////////////////////////////
void LedRopeTCL::Set(int i, const Color3i& c) {
  i = Wrap(i, n_color_array_);
  color_array_[i] = c;
}

///////////////////////////////////////////////////////////////////////////////
void LedRopeTCL::FillColor(const Color3i& color) {
  for (int i = 0; i < n_color_array_; ++i) {
    color_array_[i] = color;
  }
}

///////////////////////////////////////////////////////////////////////////////
void LedRopeTCL::ApplyBlendSubtract(const Color3i& color) {
  for (int i = 0; i < n_color_array_; ++i) {
    color_array_[i].Sub(color);
  }
}

///////////////////////////////////////////////////////////////////////////////
void LedRopeTCL::ApplyBlendAdd(const Color3i& color) {
  for (int i = 0; i < n_color_array_; ++i) {
    color_array_[i].Add(color);
  }
}

///////////////////////////////////////////////////////////////////////////////
void LedRopeTCL::ApplyBlendMultiply(const Color3i& color) {
  for (int i = 0; i < n_color_array_; ++i) {
    color_array_[i].Mul(color);
  }
}

///////////////////////////////////////////////////////////////////////////////
LedRopeTCL::Color3i* LedRopeTCL::GetIterator(int i) {
  i = constrain(i, 0, n_color_array_);
  return color_array_ + i;
}

///////////////////////////////////////////////////////////////////////////////
int LedRopeTCL::Wrap(int val, int max_val) {
  if (val >= max_val) {
    // Wrap.
    return val % max_val;
  } else if (val < 0) {
    return max_val + (val % max_val);
  } else {
    return val;
  }
}

///////////////////////////////////////////////////////////////////////////////
void LedRopeTCL::Color3i::Mul(const Color3i& other_color) {
  int r = r_;
  int g = g_;
  int b = b_;

  r = r * int(other_color.r_) / 255;
  g = g * int(other_color.g_) / 255;
  b = b * int(other_color.b_) / 255;
  Set(r, g, b);
}

///////////////////////////////////////////////////////////////////////////////
void LedRopeTCL::Color3i::Mul(float scale) {
  const int s = static_cast<int>(scale * 255.0f);

  int r = static_cast<int>(r_) * s;
  int g = static_cast<int>(g_) * s;
  int b = static_cast<int>(b_) * s;

  Set(r, g, b);
}

///////////////////////////////////////////////////////////////////////////////
void LedRopeTCL::Color3i::Sub(const Color3i& color) {
  if (r_ < color.r_) r_ = 0;
  else               r_ -= color.r_;
  if (g_ < color.g_) g_ = 0;
  else               g_ -= color.g_;
  if (b_ < color.b_) b_ = 0;
  else               b_ -= color.b_;
}

///////////////////////////////////////////////////////////////////////////////
void LedRopeTCL::Color3i::Add(const Color3i& color) {
  if ((255 - r_) < color.r_) r_ = 255;
  else                       r_ += color.r_;
  if ((255 - g_) < color.g_) g_ = 255;
  else                       g_ += color.g_;
  if ((255 - b_) < color.b_) b_ = 255;
  else                       b_ += color.b_;
}

///////////////////////////////////////////////////////////////////////////////
const byte LedRopeTCL::Color3i::Get(int rgb_index) const {
  const byte* rgb = At(rgb_index);
  return rgb ? *rgb : 0;
}

///////////////////////////////////////////////////////////////////////////////
void LedRopeTCL::Color3i::Set(int rgb_index, byte val) {
  byte* rgb = At(rgb_index);
  if (rgb) {
    *rgb = val;
  }
}

///////////////////////////////////////////////////////////////////////////////
void LedRopeTCL::Color3i::Interpolate(const Color3i& other_color, float t) {
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
byte* LedRopeTCL::Color3i::At(int rgb_index) {
  switch(rgb_index) {
    case 0: return &r_;
    case 1: return &g_;
    case 2: return &b_;
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
const byte* LedRopeTCL::Color3i::At(int rgb_index) const {
  switch(rgb_index) {
    case 0: return &r_;
    case 1: return &g_;
    case 2: return &b_;
  }
  return NULL;
}
