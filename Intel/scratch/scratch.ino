
class Rectangle {
    int width, height;
  public:
    Rectangle (int,int);
    int area () {return (width*height);}
};

Rectangle::Rectangle (int a, int b) {
  width = a;
  height = b;
}

int main () {
  Rectangle rect (3,4);
  Rectangle rectb (5,6);
  Serial.print("rect area: ");
  Serial.println(rect.area());
  
  Serial.print("rect area: ");
  Serial.println(rectb.area());
  return 0;
}
