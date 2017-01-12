#include <SD.h>
#include <TFT.h>
#include <Esplora.h>
#define STEP 40
String loadPokemon(int _pokemonNum)
{
  if(_pokemonNum < 1)
    _pokemonNum = 1;
  else if(_pokemonNum > 151)
    _pokemonNum = 151;
  else
    _pokemonNum -= 1;
  Serial.println("Start Loading Pokemon");
  String ret;
  File f = SD.open("pokedex.TXT",FILE_READ);
  f.seek(STEP*_pokemonNum);
  ret = f.readStringUntil(' ');
  Serial.println(ret);
  Serial.println("Done Loading");
  f.close();
  return ret;
}

PImage getBackImage(int _pokemonNum)
{
  if(_pokemonNum < 1)
    _pokemonNum = 1;
  else if(_pokemonNum > 151)
    _pokemonNum = 151;
  else
    _pokemonNum -= 1;
  String filename;
  filename += "b_";
  filename += _pokemonNum;
  filename += ".bmp";

  return EsploraTFT.loadImage(filename.c_str());
}

PImage getFrontImage(int _pokemonNum)
{
  if(_pokemonNum < 1)
    _pokemonNum = 1;
  else if(_pokemonNum > 151)
    _pokemonNum = 151;
  else
    _pokemonNum -= 1;
  String filename;
  filename += "f_";
  filename += _pokemonNum;
  filename += ".bmp";

  return EsploraTFT.loadImage(filename.c_str());
}
String getType_str(String &_PokemonData)
{
  int typeNum;
  String type = _PokemonData.substring(0,2);
  typeNum = type.toInt();
  switch(typeNum)
  {
    case 0  : return "Poison";   break;
    case 1  : return "Normal";   break;
    case 2  : return "Grass";    break;
    case 3  : return "Fire";     break;
    case 4  : return "Electric"; break;
    case 5  : return "Ground";   break;
    case 6  : return "Rock";     break;
    case 7  : return "Ice";      break;
    case 8  : return "Fighting"; break;
    case 9  : return "Psychic";  break;
    case 10 : return "Bug";      break;
    case 11 : return "Dragon";   break;
    case 12 : return "Ghost";    break;
    case 13 : return "Water";    break;
    defualt : return "BAD INPUT";break;  
  }
}
String getHP_str(String &_PokemonData)
{
  return _PokemonData.substring(3,6);
}

int getHP(String &_PokemonData)
{
  return _PokemonData.substring(3,6).toInt();
}
String getAttack_str(String &_PokemonData)
{
  return _PokemonData.substring(7,10);
}
int getAttack(String &_PokemonData)
{
  return _PokemonData.substring(7,10).toInt();
}
String getDefence_str(String &_PokemonData)
{
  return _PokemonData.substring(11,14);
}
int getDefence(String &_PokemonData)
{
  return _PokemonData.substring(11,14).toInt();
}
String getSpecialAttack_str(String &_PokemonData)
{
  return _PokemonData.substring(15,18);
}
int getSpecialAttack(String &_PokemonData)
{
  return _PokemonData.substring(15,18).toInt();
}
String getSpecialDefence_str(String &_PokemonData)
{
  return _PokemonData.substring(19,22);
}
int getSpecialDefence(String &_PokemonData)
{
  return _PokemonData.substring(19,22).toInt();
}
String getSpeed_str(String &_PokemonData)
{
  return _PokemonData.substring(23,26);
}
int getSpeed(String &_PokemonData)
{
  return _PokemonData.substring(23,26).toInt();
}
String getName(String &_PokemonData)
{
  return _PokemonData.substring(27,38);
}
