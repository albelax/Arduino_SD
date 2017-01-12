//Made by Tom and Alberto 
//A simple pokedex application for the arduino that displays how to use pokedex.h


#include "pokedex.h"
#define HEIGHT 128 //Height of the screen
#define WIDTH  160 //Width of the screen

const char SD_card = 8;
const short step = 40;
<<<<<<< HEAD
short pos = 0;
=======
short pos = -1;
>>>>>>> origin/master
short oldPos = pos;
PImage img;

String pokemonData;
String id;
String name;
String type;
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
  if(Esplora.readButton(SWITCH_LEFT) == LOW && pos > 0)
  {
    EsploraTFT.stroke(0, 0, 0);
    EsploraTFT.text(id.c_str(), 50, 10);
    EsploraTFT.text(name.c_str(), 50, 20);
    EsploraTFT.text(type.c_str(), 50, 30);
    pos -= 1;          
   }
   else if(Esplora.readButton(SWITCH_RIGHT) == LOW && pos < 150)
   {
     EsploraTFT.stroke(0, 0, 0);
     EsploraTFT.text(id.c_str(), 50, 10);
     EsploraTFT.text(name.c_str(), 50, 20);
     EsploraTFT.text(type.c_str(), 50, 30);
     pos += 1;          
    }

    else if(Esplora.readButton(SWITCH_UP) == LOW && pos < 140)
   {
     EsploraTFT.stroke(0, 0, 0);
     EsploraTFT.text(id.c_str(), 50, 10);
     EsploraTFT.text(name.c_str(), 50, 20);
     EsploraTFT.text(type.c_str(), 50, 30);
     pos += 10;          
    }

   else if(Esplora.readButton(SWITCH_DOWN) == LOW && pos > 10)
   {
     EsploraTFT.stroke(0, 0, 0);
     EsploraTFT.text(id.c_str(), 50, 10);
     EsploraTFT.text(name.c_str(), 50, 20);
     EsploraTFT.text(type.c_str(), 50, 30);
     pos -= 10;          
    }
    
    if(pos != oldPos)
    {
      if(Esplora.readSlider() < 500)
        img = getFrontImage(pos+1);
      else
        img = getBackImage(pos+1);
<<<<<<< HEAD
      id = pos;
      pokemonData = loadPokemon(pos);
      name = getName(pokemonData);//pokemonData.substring(27,40);
      type = getType_str(pokemonData);//lookForBreak(pokemonData, ';', 0);
=======
      id = pos+1;
      pokemonData = loadPokemon(pos, step);
      name = getName(pokemonData);//pokemonData.substring(27,40);
      type = getType(pokemonData);//lookForBreak(pokemonData, ';', 0);
>>>>>>> origin/master
         
      EsploraTFT.stroke(255, 255, 255);
      EsploraTFT.text(type.c_str(), 50, 30);
      EsploraTFT.text(name.c_str(), 50, 20);
      EsploraTFT.text(id.c_str(), 50, 10);
      EsploraTFT.image(img, 0, 45);
      img.close();
      oldPos = pos;
<<<<<<< HEAD
    } 
}


=======
    }

  
}

String loadPokemon(int _pokemonNum, int const _step)
{
  Serial.println("Start Loading Pokemon");
  String ret;
  File f = SD.open("pokedex.txt",FILE_READ);
  f.seek(_step*_pokemonNum);
//  while (f.position() < (_pokemonNum * _step) + _step) 
//  {
    ret = f.readStringUntil(' ');
//  }
  Serial.println(ret);
  Serial.println("Done Loading");
  f.close();
  return ret;
}

PImage getBackImage(int _PokemonNum)
{
  String filename;
  filename += "b_";
  filename += _PokemonNum;
  filename += ".bmp";

  return EsploraTFT.loadImage(filename.c_str());
}

PImage getFrontImage(int _PokemonNum)
{
  String filename;
  filename += "f_";
  filename += _PokemonNum;
  filename += ".bmp";

  return EsploraTFT.loadImage(filename.c_str());
}
String getType(String &_PokemonData)
{
  return _PokemonData.substring(0,2);
}

String getHP_str(String &_PokemonData)
{
  return _PokemonData.substring(3,6);
}

int getHP(String &_PokemonData)
{
  return 5;
}
String getAttack(String &_PokemonData)
{
  return _PokemonData.substring(7,10);
}

String getDefence(String &_PokemonData)
{
  return _PokemonData.substring(11,14);
}

String getSpecialAttack(String &_PokemonData)
{
  return _PokemonData.substring(15,18);
}

String getSpecialDefence(String &_PokemonData)
{
  return _PokemonData.substring(19,22);
}

String getSpeed(String &_PokemonData)
{
  return _PokemonData.substring(23,26);
}

String getName(String &_PokemonData)
{
  return _PokemonData.substring(27,38);
}


>>>>>>> origin/master
