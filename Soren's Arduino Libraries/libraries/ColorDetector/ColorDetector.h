// Copyleft Zach Vorhies 2014
// No rights reserved.
// Tested on an ArduinoMega. Connect pins to RX and TX.
// Usage:
//  ColorDetector<HardwareSerial> cd(Serial3);
//  void setup() { cd.begin(); }
//  void loop() {
//    if (cd.Update()) {  // True when new readings have occured.
//      cd.PrintValues(Serial);  // print values.
//    }
//  }

#include "Arduino.h"


template<class StreamT>  // StreamT == HardwareSerial or SoftSerial.
class ColorDetector {
public:
    ColorDetector(StreamT& s)
    : input_(""), stream_(s),
    r(0), b(b), g(0),
    r_lx(0), b_lx(0), g_lx(0), total_lx(0),
    saturated(false) {
        input_.reserve(kStrLen);  // Upto 36 characters for the rgb+lx data.
    }
    
    void begin() {
        stream_.begin(38400);
        stream_.write("M3\r");  // Sets mode0 of the sensor.
    }
    
    // Returns true if new values were parsed.
    bool Update() {
        while (stream_.available() > 0) {
            //if the hardware serial port_3 receives a char
            char inchar = (char)stream_.read();                              //get the char we just received
            input_ += inchar;                                          //add it to the inputString
            if(inchar == '\r') {
                saturated = input_.indexOf('*') != -1;
                
                
                // Time to parse out the string into values.
                int* values[] = {&r, &g, &b, &r_lx, &g_lx, &b_lx, &total_lx};
                const int values_size = sizeof(values)/sizeof(values[0]);
                int i = 0;
                
                char cpy_str[kStrLen];
                input_.toCharArray(cpy_str, kStrLen);
                input_ = "";  // resets the input.
                
                char* p = cpy_str;
                
                char *str;
                
                while (i < values_size && (str = strtok_r(p, ",", &p)) != NULL) { // delimiter is the comma
                    String cpy = str;
                    *values[i] = cpy.toInt();
                    ++i;
                }
                return true;
            }
            // No update yet.
            return false;
        }
    }
    
    
    void PrintValues(Stream& o) {
        o.println("ColorDetector:");
        o.print("  RGB       = ("); o.print(r); o.print(","); o.print(g); o.print(","); o.print(b); o.println(")");
        o.print("  RGB Lux   = ("); o.print(r_lx); o.print(","); o.print(g_lx); o.print(","); o.print(b_lx); o.println(")");
        o.print("  Total Lux = "); o.println(total_lx);
        o.print("  Saturated = "); o.println(saturated? "true" : "false");
        /*
        
        int* values[] = {&r, &b, &g, &r_lx, &b_lx, &g_lx, &total_lx};
        const int array_size = sizeof(values) / sizeof(values[0]);
        for (int i = 0; i < array_size; ++i) {
            output.print(*values[i]);
            if (i < array_size - 1) {
                output.print(",");
            }
        }

        if (saturated) {
            output.print(",<saturated>");
        }
        output.println();
         */
    }
    
    int r, b, g;
    int r_lx, b_lx, g_lx, total_lx;
    bool saturated;
    
private:
    static const int kStrLen = 38;
    String input_;
    StreamT& stream_;
};



