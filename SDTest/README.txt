                                      /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
                                                                      ArduinoDex Guide
                                      /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
                                             A reference document for the Arduino Pokemon project written by Tom Hoxey
                                             
DATABASE--------------------------------------------------------------------------------------------------------------------------------------------------------------
  The Arduino will read the pokemon from a txt file in the following format:
    02;045;049;049;065;065;045;Bulbasaur
    02;060;062;063;080;080;060;Ivysaur    
    02;080;082;083;100;100;080;Venusaur   
    03;039;052;043;060;050;065;Charmander 
    ETC.
  This is a quick guide on how to read the data from the database:

    0. Type             |  Len[02]  | Index[00]
    1. HP               |  Len[03]  | Index[03]
    2. Attack           |  Len[03]  | Index[07]
    3. Defense          |  Len[03]  | Index[11]
    4. Special Attack   |  Len[03]  | Index[15]
    5. Special Defense  |  Len[03]  | Index[19]
    6. Speed            |  Len[03]  | Index[23]
    7. Name             |  Len[11]  | Index[27]
    8. End              |  Len[00]  | Index[39]

  The type IDs are in the following order:
    |00. Poison    | 01. Normal    | 02. Grass     | 03. Fire      | 04. Electric  | 05. Ground    | 06. Rock      |
    |07. Ice       | 08. Fighting  | 09. Psychic   | 10. Bug       | 11. Dragon    | 12. Ghost     | 13. Water     |
DATABASE--------------------------------------------------------------------------------------------------------------------------------------------------------------
/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
USING THE DATABASE----------------------------------------------------------------------------------------------------------------------------------------------------

    You must have the latest version of the "SD_Contents" on your SD card to use the database.

    STATS:
        Each stat above can be accessed in two forms using the "pokedex.h" file, and a correctly formatted SD card.

        The basic process for this is in two steps:
        1. Load a pokemon
            This is done using "loadPokemon([pokemonID])"
            It returns a pokemon's database entry (pokemonEntry)
        2. Request the stat you require
            This is done using get[stat](pokemonEntry)
            If you want the stat as a string (e.g. to put onto the screen), use get[stat]_str(pokemonEntry)
            Note: getType_str will return the name of the type e.g. "Fire" as opposed to "03"
            Note: getName returns a string since it is not a number
    IMAGES:
        Each pokemon has two images, the front image and back image.
        Loading these images is very simple, just use get[Front/Back]Image.
        This will return a "PImage", a format the Arduino uses for the TFT screen
    EXAMPLE:
        Using Charmander as an example: [03;039;052;043;060;050;065;Charmander ]
        If for example you wanted to load in the Name Type and HP of Charmander as well as pushing his front image to screen you would do the following:
        //.......................................... CODE SNIPPET ..........................................
            String myPokemon = loadPokemon(4); //4 is Chamanders pokemon ID
            String myPokemonName = getName(myPokemon);
            String myPokemontype = getType_str(myPokemon); // _str returns it as a String
            int myPokemonHP = getHP(myPokemon);
            PImage myPokemonImg = loadFrontImage(4);
            EsploraTFT.image(myPokemonImg, Xpos, Ypos);
            myPokemonImg.close(); //Important tidying up!
        //.......................................... CODE SNIPPET ..........................................
        
USING THE DATABASE----------------------------------------------------------------------------------------------------------------------------------------------------
/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
BASIC POKEMON FACTS---------------------------------------------------------------------------------------------------------------------------------------------------
  Basic Pokemon Information that should be useful:
    0. Type             |  The type of the pokemon determines how effective it is against other pokemon
    1. HP               |  This is the base figure that will determine the pokemons hit points
    2. Attack           |  This is a figure to determine attack in the battle equation
    3. Defense          |  This is a figure to determine defense in the battle equation
    4. Special Attack   |  This is a figure to determine special attack in the battle equation (Not currently used but included for expansion)
    5. Special Defense  |  This is a figure to determine special defense in the battle equation (Not currently used but included for expansion)
    6. Speed            |  This is used to determine which pokemon goes first
    7. Name             |  The name of the pokemon

  The types interact as follows, interations are if the type is attacking:
    00. Poison    |   2x Grass      | 0.5x Poison   | 0.5x Ground   | 2x Bug        | 0.5x Rock     | 0.5x Ghost    |
    01. Normal    |   0.5x Rock     | 0x Ghost      |
    02. Grass     |   0.5x Fire     | 2x Water      | 0.5x Grass    | 0.5x Poison   | 2x Ground     | 0.5x Bug      | 2x Rock        | 0.5x Dragon |
    03. Fire      |   0.5x Fire     | 0.5x Water    | 2x Grass      | 2x Ice        | 2x Bug        | 0.5x Rock     | 0.5x Dragon    |
    04. Electric  |   2x Water      | 0.5x Electric | 0.5x Grass    | 0x Ground     | 0.5x Dragon
    05. Ground    |   2x Fire       | 2x Electric   | 0.5x Grass    | 2x Poison     | 0.5x Bug      | 2x Rock       |
    06. Rock      |   2x Fire       | 2x Ice        | 0.5x Fighting | 0.5x Ground   | 2x Bug        |
    07. Ice       |   0.5x Water    | 2x Grass      | 0.5x Ice      | 2x Ground     | 2x Dragon     |
    08. Fighting  |   2x Normal     | 2x Ice        | 0.5x Poison   | 0.5x Psychic  | 0.5x Bug      | 2x Rock       | 0x Ghost       |
    09. Psychic   |   2x Fighting   | 2x Poison     | 0.5x Psychic  |
    10. Bug       |   0.5x Fire     | 2x Grass      | 0.5x Fighting | 2x Poison     | 2x Psychic    |
    11. Dragon    |   2x Dragon     |
    12. Ghost     |   0x Normal     | 0x Psychic    | 2x Ghost
    13. Water     |   2x Fire       | 0.5x Grass    | 0.5x Water   | 2x Ground      | 2x Rock       | 0.5x Dragon   |

BASIC POKEMON FACTS---------------------------------------------------------------------------------------------------------------------------------------------------
/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
BATTLING--------------------------------------------------------------------------------------------------------------------------------------------------------------
  The Damage Equation is as follows:
                /   2 x Level + 10       Attack                  \
    DAMAGE   = |   ----------------  X  --------- X Base + 2      | X Modifier
                \        250             Defense                 /

    MODIFIER = ( STAB X Type X Critical X Other X (random(0.85, 1))

    STAB is the same attack type bonus, i.e, if a rock pokemon uses a rock move it does 1.5x damage.
    Moves have a chance of critical hitting, which becomes 2 if successful
    Base is the base attack of the move
    Attack is of the attacking pokemon but defese is of the defending pokemon
    Some moves use special attack and special defense instead (This is not added but could be in future)
    
BATTLING--------------------------------------------------------------------------------------------------------------------------------------------------------------
/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
POKEDEX---------------------------------------------------------------------------------------------------------------------------------------------------------------
001 Bulbasaur    002 Ivysaur      003 Venusaur     004 Charmander   005 Charmeleon   006 Charizard    007 Squirtle     008 Wartortle    009 Blastoise    010 Caterpie     
011 Metapod      012 Butterfree   013 Weedle       014 Kakuna       015 Beedrill     016 Pidgey       017 Pidgeotto    018 Pidgeot      019 Rattata      020 Raticate     
021 Spearow      022 Fearow       023 Ekans        024 Arbok        025 Pikachu      026 Raichu       027 Sandshrew    028 Sandslash    029 Nidoran      030 Nidorina     
031 Nidoqueen    032 Nidoran      033 Nidorino     034 Nidoking     035 Clefairy     036 Clefable     037 Vulpix       038 Ninetales    039 Jigglypuff   040 Wigglytuff   
041 Zubat        042 Golbat       043 Oddish       044 Gloom        045 Vileplume    046 Paras        047 Parasect     048 Venonat      049 Venomoth     050 Diglett      
051 Dugtrio      052 Meowth       053 Persian      054 Psyduck      055 Golduck      056 Mankey       057 Primeape     058 Growlithe    059 Arcanine     060 Poliwag      
061 Poliwhirl    062 Poliwrath    063 Abra         064 Kadabra      065 Alakazam     066 Machop       067 Machoke      068 Machamp      069 Bellsprout   070 Weepinbell   
071 Victreebel   072 Tentacool    073 Tentacruel   074 Geodude      075 Graveler     076 Golem        077 Ponyta       078 Rapidash     079 Slowpoke     080 Slowbro      
081 Magnemite    082 Magneton     083 Farfetch     084 Doduo        085 Dodrio       086 Seel         087 Dewgong      088 Grimer       089 Muk          090 Shellder     
091 Cloyster     092 Gastly       093 Haunter      094 Gengar       095 Onix         096 Drowzee      097 Hypno        098 Krabby       099 Kingler      100 Voltorb      
101 Electrode    102 Exeggcute    103 Exeggutor    104 Cubone       105 Marowak      106 Hitmonlee    107 Hitmonchan   108 Lickitung    109 Koffing      110 Weezing      
111 Rhyhorn      112 Rhydon       113 Chansey      114 Tangela      115 Kangaskhan   116 Horsea       117 Seadra       118 Goldeen      119 Seaking      120 Staryu       
121 Starmie      122 Mr           123 Scyther      124 Jynx         125 Electabuzz   126 Magmar       127 Pinsir       128 Tauros       129 Magikarp     130 Gyarados     
131 Lapras       132 Ditto        133 Eevee        134 Vaporeon     135 Jolteon      136 Flareon      137 Porygon      138 Omanyte      139 Omastar      140 Kabuto       
141 Kabutops     142 Aerodactyl   143 Snorlax      144 Articuno     145 Zapdos       146 Moltres      147 Dratini      148 Dragonair    149 Dragonite    150 Mewtwo
151 Mew
POKEDEX---------------------------------------------------------------------------------------------------------------------------------------------------------------
