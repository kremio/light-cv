
import processing.serial.*;

// The serial port:
Serial myPort;    

//Last CV value
int CV = -1;

int margin = 10;
int keyWidth = 0;

PGraphics shadowKeyboard;

void setup() { 
  size(1000, 200);
  keyWidth = (width-2*margin) / (5 * 8); //5 octaves
  createShadeKeyboard();
  // List all the available serial ports:
  printArray(Serial.list());

  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[4], 9600);
}

/* A copy of the keyboard using shades of gray to figure out what key is being pressed */
void createShadeKeyboard() {
  shadowKeyboard = createGraphics(width, height);
  shadowKeyboard.beginDraw();
  int noteColor = 0;
  //Draw the white keys

  int n = 0;
  for (int k = 0; k < width; k += keyWidth) {
    shadowKeyboard.fill( noteColor );
    shadowKeyboard.stroke( noteColor );
    shadowKeyboard.rect(k, 0, keyWidth, height);
    if ( n == 2 || n == 6 ) {
      noteColor += 1;
    } else {
      noteColor += 2;
    }
    if (n == 6) {
      n = 0;
    } else {
      n++;
    }
  }

  //Draw the black keys
  noteColor = 1;
  n = 0;
  for (int k = keyWidth/2; k < width; k += keyWidth) {
    shadowKeyboard.fill( noteColor );
    shadowKeyboard.stroke( noteColor );
    if (n != 2 && n != 6 ) {
      shadowKeyboard.rect(k, 0, keyWidth, height / 2);
      noteColor += 2;
    } else {
      noteColor += 1;
    }
    if (n == 6) {
      n = 0;
    } else {
      n++;
    }
  }
  shadowKeyboard.endDraw();
}

int getNoteAt(int x, int y) {
  shadowKeyboard.loadPixels();
  int n = shadowKeyboard.pixels[x + y * width] >> 16 & 0xFF;
  // 0 is C1 is 3 * 12 semitones under C4
  return -3 * 12 + n;
}

void draw() {
  background(125);
  /*
Draw a piano keyboard from C1 to B7
   */
  //Draw the white keys
  fill(255);
  stroke(50);
  int n = 0;
  for (int k = 0; k < width; k += keyWidth) {
    if ( n == 3 * 7) { //C4
      fill(200);
    } else {
      fill(255);
    }
    rect(k, 0, keyWidth, height);
    n++;
  }

  //Draw the black keys
  fill(0);
  stroke(50);
  n = 0;
  for (int k = keyWidth/2; k < width; k += keyWidth) {
    if (n != 2 && n != 6 ) {
      rect(k, 0, keyWidth, height / 2);
    }
    if (n == 6) {
      n = 0;
    } else {
      n++;
    }
  }
}

void mousePressed() {
  int n = getNoteAt(mouseX, mouseY);
  myPort.write( n+"" );
}

void mouseDragged() {
  int n = getNoteAt(mouseX, mouseY);
  myPort.write( n+"" );
}

void mouseReleased() {
  myPort.write("OFF");
}