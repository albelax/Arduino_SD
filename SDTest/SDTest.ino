#include <SD.h>
#include <TFT.h>
#include <SPI.h>
#include <Esplora.h>

#define HEIGHT 128 //Height of the screen
#define WIDTH  160 //Width of the screen


uint8_t p;
// SD Chip Select pin
const char SD_card = 8;
const short step = 56;
short pos = 149;
short oldPos = pos;

String lookForBreak(String _str, char _breakSym, int _breaktot)
{
  int prevBreak;
  String str = _str;
  String ret;
  int _breakcount = 0;
  if(_breaktot == 0)
  {
    for(int i = 0; i < step; ++i)
    {
      if(_str[i] == _breakSym)
      {
        str.remove(i);
        return str;
      }
    }
    
  }
  else
  {
    for(int i = 0; i < step; ++i)
    {
      if(_str[i] == _breakSym && _breakcount == _breaktot-1)
      {
        prevBreak = i+1;
      }
      if(_str[i] == _breakSym &&  _breakcount == _breaktot)
      {
        str.remove(i);
        for(int j = prevBreak; j < i; ++j)
        {
          ret += str[j];
        }
        return ret;
      }
      else if(_str[i] == _breakSym && _breakcount != _breaktot)
      {
        ++_breakcount;
      }
    }
      
  }
  return "ERROR, no break character";
}
void setup() 
{
  SPI.begin();
  
  EsploraTFT.begin();
  EsploraTFT.background(0, 0, 0);
  EsploraTFT.stroke(255,255,255);
  // initialize the serial port
  Serial.begin(9600);
  EsploraTFT.text("ID:", 10, 10);
  EsploraTFT.text("Name:", 10, 20);
  EsploraTFT.text("Type:", 10, 30);
  // try to access the SD card
  Serial.print("Initializing SD card...\n");
  if (!SD.begin(SD_card)) 
  {
    Serial.println("Failed");
    return;
  }
  Serial.println("Succeded");
}

void loop()
{
  
  String id = loadPokemon(pos, step, 0);
  String name = loadPokemon(pos, step, 1);
  String type = loadPokemon(pos, step, 2);
  // String imageName = "f_"+loadPokemon(pos, step, 0)+".bmp";
  // PImage pokemonImage = EsploraTFT.loadImage(imageName.c_str());
  
  if(Esplora.readButton(SWITCH_LEFT) == LOW && pos > 0)
        {
          EsploraTFT.stroke(0, 0, 0);
          EsploraTFT.text(id.c_str(), 50, 10);
          EsploraTFT.text(name.c_str(), 50, 20);
          EsploraTFT.text(type.c_str(), 50, 30);
          delay(50);
          pos -= 1;          
        }
  else if(Esplora.readButton(SWITCH_RIGHT) == LOW && pos < 150)
        {
          EsploraTFT.stroke(0, 0, 0);
          EsploraTFT.text(id.c_str(), 50, 10);
          EsploraTFT.text(name.c_str(), 50, 20);
          EsploraTFT.text(type.c_str(), 50, 30);
          delay(50);
          pos += 1;          
        }
        // if(pos != oldPos)
        // {
        //   EsploraTFT.image(pokemonImage, 10, 40);
        //   oldPos = pos;
        // }
  EsploraTFT.text(type.c_str(), 50, 30);
  EsploraTFT.text(name.c_str(), 50, 20);
  EsploraTFT.text(id.c_str(), 50, 10);
  EsploraTFT.stroke(255, 255, 255);


}

String loadPokemon(short _pokemonNum, short const _step, int _entry)
{
  Serial.println("Start Loading Pokemon");
  String ret;
  String pokemonEntry;
  File f = SD.open("pokedex.txt",FILE_READ);
  f.seek(_step*_pokemonNum);
  while (f.position() != (_pokemonNum * _step) + _step) 
  {
    pokemonEntry = f.readStringUntil('\n');
  }
  Serial.println(pokemonEntry);
  ret = lookForBreak(pokemonEntry, ';', _entry);
  Serial.println("Done Loading");
  f.close();
  return ret;
}


