
struct hsv {
    byte h;       // angle in degrees
    byte s;       // percent
    byte v;       // percent
};

struct rgb {
    byte r;
    byte g;
    byte b;
};


/**
 * convert hsv color space to rgb color space
 * based on code from http://stackoverflow.com/a/22120275
 * 
 * @param hsvColor an hsv color struct
 * @return an rgb color struct
 */
rgb hsv2rgb(hsv hsvColor) {
    rgb rgbColor;
    unsigned char region, p, q, t;
    unsigned int h, s, v, remainder;

    if (hsvColor.s == 0)
    {
        rgbColor.r = hsvColor.v;
        rgbColor.g = hsvColor.v;
        rgbColor.b = hsvColor.v;
        return rgbColor;
    }

    // converting to 16 bit to prevent overflow
    h = hsvColor.h;
    s = hsvColor.s;
    v = hsvColor.v;

    region = h / 43;
    remainder = (h - (region * 43)) * 6; 

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgbColor.r = v;
            rgbColor.g = t;
            rgbColor.b = p;
            break;
        case 1:
            rgbColor.r = q;
            rgbColor.g = v;
            rgbColor.b = p;
            break;
        case 2:
            rgbColor.r = p;
            rgbColor.g = v;
            rgbColor.b = t;
            break;
        case 3:
            rgbColor.r = p;
            rgbColor.g = q;
            rgbColor.b = v;
            break;
        case 4:
            rgbColor.r = t;
            rgbColor.g = p;
            rgbColor.b = v;
            break;
        default:
            rgbColor.r = v;
            rgbColor.g = p;
            rgbColor.b = q;
            break;
    }

    return rgbColor;
}



