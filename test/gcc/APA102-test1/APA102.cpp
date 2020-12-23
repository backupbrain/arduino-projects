/**
 * APA102 Parsing/Displaying class
 * 
 * This class takes in RGB color data and converts it to APA102 binary data,
 * and retrieves APA102 binary data and builds an RGB color table
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-12-26
 */

#include "APA102.h"

APA102::APA102(uint8_t numPixels): _numPixels(numPixels) {
  this->_colors = (RGBColor*)malloc(sizeof(RGBColor)*numPixels);
}


/*
APA102::~APA102() {
  free(this->_colors);
  this->_colors = NULL;
}
/* */

void APA102::resetCarat() {
  this->_frameReadCursor = 0;
  this->_colorReadCursor = -1;
}

void APA102::resetReadCarat() {
  this->_readCarat = 0;
}
void APA102::resetWriteCarat() {
  this->_writeCarat = 0;
}



void APA102::putRGBColor(RGBColor color) {
  this->_colors[_writeCarat++] = color;
} 

RGBColor APA102::getRGBColor(uint8_t index) {
  return this->_colors[index];
}


void APA102::putAPA102(uint32_t packet) {
  // decode apa102 packet
  RGBColor color = this->APA1022RGB(packet);
  this->putRGBColor(color);
}

uint32_t APA102::getAPA102(uint8_t index) {
  RGBColor color = this->getRGBColor(index);
  //this->printRGB();
  return this->RGB2APA102(color);
}


uint32_t APA102::RGB2APA102(RGBColor color) {
  // begin with zeros
  uint32_t apa102 = 0xff << 24;
  // pack in as RGB
  apa102 |= color.red << 16;
  apa102 |= color.green << 8;
  apa102 |= color.blue;
  //apa102 |= 0xff00;

  //fprintf(stdout, "%08X ", apa102);

  return apa102;
}
RGBColor APA102::APA1022RGB(uint32_t apa102) {
  // unpack as RGB
  RGBColor color;
  color.red = (apa102 >> 16) && 0xff;
  color.green = (apa102 >> 8) && 0xff;
  color.blue = apa102 && 0xff;
}
 

int APA102::parseOctoBytes(uint32_t apa102) {
    // if we are in a header, we should expect 0x0 * 4.

    // on led_global state, we should see some value 0x111NNNN

    // all other led colors are read accordingly

    // if we are in an LED state ...
    // whenever we see a 0x11111111 (x4) we are at a footer

    fprintf(stdout, "%08X ", apa102);
    printf("\n");

    uint8_t input[4];
    input[3] = apa102 & 0xff;
    input[2] = (apa102 >> 8) & 0xff;
    input[1] = (apa102 >> 16) & 0xff;
    input[0] = (apa102 >> 24) & 0xff;

    for (int i=0; i<4; i++) {
      fprintf(stdout, "%02X ", input[i]);
    }

    printf("\n");
    /*

    int state = self::STATE_COLOR;
    // headers start with 0x00
    bool isHeader = (input[0] == 0x0);


    // if this is a header, then the last input was a footer.  remove that from the list

    if (isHeader) {

        if (frames.size() > 0) {
            ArrayList<RGB> frame = frames.get(frames.size()-1);
            lastColor = null;
            if (frame.size() > 0) {
                frame.remove(frame.size() - 1);
            }
        }
        frames.add(new ArrayList<RGB>());
        fprintf(stdout, "Frame header: creating frame %d", length(frames)-1 );

        state = STATE_HEADER;

    } else {
        // add the previous color if we have a new color.
        // this prevents us from appending the frame end as a color
        ArrayList<RGB> frame = frames[length(frames)-1];
        //if (lastColor != null) frame.add(lastColor);

        RGBColor rgbColor;
        rgbColor.red = input[1];
        rgbColor.green = input[2];
        rgbColor.blue = input[3];

        // append color value to this frame
        //lastColor = rgbColor;
        frame.add(rgbColor);
        Log.d(TAG, "inserting color "+(length(frame)-1)+" into frame "+(length(frames)-1)+": "+rgbColor.toString());
    }
    return state;
    /* */
    return 1;

}

void APA102::printRGB() {
  printf("RGB:");
  // loop through colors, print out binary output
  for (int i=0; i<this->_numPixels; i++) {   
    fprintf(stdout, "%02X %02X %02X, ", this->_colors[i].red, this->_colors[i].green, this->_colors[i].blue);
  }
  printf("\n");
}



void APA102::printAPA102() {
  printf("APA102: ");
  // loop through colors, print out binary output
  for (int i=0; i<this->_numPixels; i++) {
    uint32_t apa102 = this->getAPA102(i);
    fprintf(stdout, "%08X ", apa102);
  }
  printf("\n");
}

