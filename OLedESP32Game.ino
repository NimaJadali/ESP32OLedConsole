#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// Pin 21|22 for data|clock
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



// Game Object Assets and Commonly Referenced Attributes
static const unsigned char PROGMEM square_16[] = {0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111};
//static const unsigned char PROGMEM square_16[] = {0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b11000000, 0b00000011, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11111111};



// Game Object Classes
class Player {
  private:
    int width;
    int height;
  public:
    int x_coordinate;
    int y_coordinate;
    int health = 3;
    int direction_facing = 0; //0=North, 1=East, 2=South, 3=West
    bool is_shooting = false;

    Player(int width, int height, int x_coordinate, int y_coordinate)
      {  
      this->width = width;
      this->height = height;
      this->x_coordinate = x_coordinate;
      this->y_coordinate = y_coordinate;
      init();
    }
    
    void init() {
      //display.clearDisplay();
      //visualize();
      //display.display();
    }

    void visualize() {
      if(check_boundary()){
        int x = x_coordinate - width/2;
        int y = y_coordinate - height/2;
        if(x_coordinate > (display.width() - width/2)){
          if(y_coordinate > display.height() - height/2) {
            display.drawBitmap(x - display.width(), y - display.height(), square_16, width, height, 1);
          } if(y_coordinate < height/2){
            display.drawBitmap(x - display.width(), display.height() + y, square_16, width, height, 1);
          }
          display.drawBitmap(x - display.width(), y, square_16, width, height, 1);
        } else if(x_coordinate < width/2){
          if(y_coordinate > display.height() - height/2) {
            display.drawBitmap(x + display.width(), y - display.height(), square_16, width, height, 1);
          } if(y_coordinate < height/2){
            display.drawBitmap(x + display.width(), display.height() + y, square_16, width, height, 1);
          }
          display.drawBitmap(x + display.width(), y, square_16, width, height, 1);
        } 
        if(y_coordinate > display.height() - height/2) {
          display.drawBitmap(x, y - display.height(), square_16, width, height, 1);
        } else if(y_coordinate < height/2) {
          display.drawBitmap(x, display.height() + y, square_16, width, height, 1);
        }
        display.drawBitmap(x, y, square_16, width, height, 1);
        //Serial.print(x);
        //Serial.print("___");
        //Serial.println(y);
      }
    }

    bool check_boundary() {
      return true;
    }

    bool move_(int x_speed, int y_speed) {
      x_coordinate = x_coordinate + x_speed;
      y_coordinate = y_coordinate + y_speed;
      if(x_coordinate > display.width() + width/2){
        x_coordinate = x_coordinate - display.width();
      } else if(x_coordinate < -width/2){
        x_coordinate = display.width() - width/2;
      }
      if(y_coordinate > display.height() + height/2){
        y_coordinate = y_coordinate - display.height();
      } else if(y_coordinate < -height/2){
        y_coordinate = display.height() - height/2;
      }
    }
};

Player player(16, 16, display.width()/2, display.height()/2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("yeet");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setTextWrap(true);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int L_left_right_v = analogRead(34);
  int L_click_v = analogRead(35);
  int L_up_down_v = analogRead(32);


  int R_left_right_v = analogRead(27);
  int R_click_v = analogRead(26);
  int R_up_down_v = analogRead(25);

  Serial.println(R_left_right_v);
  Serial.println(R_click_v);
  Serial.println(R_up_down_v);
  Serial.println();

  //Serial.println(click_v);
  player.move_(-1 * (L_left_right_v-2047)/500, (L_up_down_v-2047)/500);

  display.clearDisplay();
  player.visualize();
  display.display();

  delay(50);
}
