#include <SD.h>
#include <TFT.h>
#include <SPI.h>

uint8_t p;
// SD Chip Select pin
const char SD_card = 8;
const short step = 56;
const short pos = 150;

void setup() 
{
  SPI.begin();
    
  EsploraTFT.begin();
  EsploraTFT.background(0, 0, 0);
  EsploraTFT.stroke(255,255,255);
  
  // initialize the serial port
  Serial.begin(9600);
 
  // try to access the SD card
  Serial.print("Initializing SD card...\n");
  if (!SD.begin(SD_card)) 
  {
    Serial.println("Failed");
    return;
  }
  Serial.println("Succeded");
  
  String p = loadPokemon(pos, step);
  Serial.println(p);
}

void loop()
{
 
}

String loadPokemon(short const _pokemonNum, short const _step)
{
  String ret;
  File f = SD.open("pokedex.txt",FILE_READ);
  f.seek(_step*_pokemonNum);
  while (f.position() != (_pokemonNum * _step) + _step) 
  {
    ret = f.readStringUntil('\n');
  }
  
  f.close();
  return ret;
}

