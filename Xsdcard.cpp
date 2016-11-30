#include <arduino.h>

#include <SPI.h>

uint8_t SB_sdcardValid  __attribute__((weak));

static uint8_t spiPin;

#define BYTESPERSECTOR 512
static const uint16_t SD_INIT_TIMEOUT = 2000;      /** init timeout ms */
static const uint16_t SD_ERASE_TIMEOUT = 10000;   /** erase timeout ms */
static const uint16_t SD_READ_TIMEOUT = 600;      /** read timeout ms */
static const uint16_t SD_WRITE_TIMEOUT = 600;     /** write time out ms */
static const uint16_t SD_READ_RETRIES = 5;

#define DATA_START_BLOCK        0xFE
#define DATA_RES_MASK         0X1F
#define DATA_RES_ACCEPTED       0X05

// SD/MMC commands
#define GO_IDLE_STATE         0
#define SEND_OP_COND          1
#define SWITCH_FUNC           6
#define SEND_IF_COND          8
#define SEND_CSD            9
#define SEND_CID            10
#define STOP_TRANSMISSION       12
#define SEND_STATUS           13
#define SET_BLOCKLEN          16
#define READ_SINGLE_BLOCK       17
#define READ_MULTIPLE_BLOCK       18
#define WRITE_BLOCK           24
#define WRITE_MULTIPLE_BLOCK      25
#define PROGRAM_CSD           27
#define SET_WRITE_PROT          28
#define CLR_WRITE_PROT          29
#define SEND_WRITE_PROT         30
#define ERASE_WR_BLK_STAR_ADDR      32
#define ERASE_WR_BLK_END_ADDR     33
#define ERASE             38
#define LOCK_UNLOCK           42
#define APP_CMD             55
#define GEN_CMD             56
#define READ_OCR            58
#define CRC_ON_OFF            59

// SD ACMDs
#define SD_STATUS           13
#define SD_SEND_NUM_WR_BLOCKS     22
#define SD_SET_WR_BLK_ERASE_COUNT   23
#define SD_SEND_OP_COND         41
#define SD_SET_CLR_CARD_DETECT      42
#define SD_SEND_SCR           51

// R1 status bits
#define STATUS_READY          0x00
#define STATUS_IN_IDLE          0x01
#define STATUS_ERASE_RESET        0x02
#define STATUS_ILLEGAL_COMMAND      0x04
#define STATUS_CRC_ERROR        0x08
#define STATUS_ERASE_SEQ_ERROR      0x10
#define STATUS_ADDRESS_ERROR      0x20
#define STATUS_PARAMETER_ERROR      0x40
#define STATUS_START_BLOCK        0xFE

// SD card type
#define SD_CARD_TYPE_SD1        1
#define SD_CARD_TYPE_SD2        2
#define SD_CARD_TYPE_SDHC       3

//SD card errors
#define SD_CARD_ERROR_CMD0        0x01
#define SD_CARD_ERROR_CMD8        0x02
#define SD_CARD_ERROR_CMD17       0x03
#define SD_CARD_ERROR_CMD24       0x04
#define SD_CARD_ERROR_CMD58       0x06
#define SD_CARD_ERROR_ACMD41      0x08
#define SD_CARD_ERROR_READ        0x0D
#define SD_CARD_ERROR_READ_TIMEOUT    0x0F
#define SD_CARD_ERROR_WRITE       0x11
#define SD_CARD_ERROR_WRITE_PROGRAMMING 0x14
#define SD_CARD_ERROR_WRITE_TIMEOUT   0x15
#define SD_CARD_ERROR_SCK_RATE      0x16

static void SPIinitChannel()
{
pinMode(spiPin,OUTPUT);
digitalWrite(spiPin,HIGH);

}
static void SPIselect()
{
  digitalWrite(spiPin,LOW);
}
static uint8_t SPITransfer(uint8_t val)
{
  return SPI.transfer(val);
}
static void SPIdeselect()
{
  digitalWrite(spiPin,HIGH);
}


uint8_t fileIOBuffer[BYTESPERSECTOR];

static  uint8_t _card_type;

static uint8_t waitNotBusy(uint16_t timeoutMillis) 
{
  uint16_t t0 = millis();
  do 
  {
    if (SPITransfer(0xff) == 0XFF)  return 1;
  }
  while (((uint16_t)millis() - t0) < timeoutMillis);
  return 0;
}

static void spiSendLong(const uint32_t data)
{
  union 
  {
    unsigned long l;
    unsigned char c[4];
  } long2char;

  long2char.l = data;

  SPITransfer(long2char.c[3]);
  SPITransfer(long2char.c[2]);
  SPITransfer(long2char.c[1]);
  SPITransfer(long2char.c[0]);
}

static uint8_t cardCommand(uint8_t cmd, uint32_t arg) 
{
  uint8_t status_;

  SPIselect();
  waitNotBusy(300);
  SPITransfer(cmd | 0x40);
  spiSendLong(arg);

  uint8_t crc = 0xFF;
  if (cmd == GO_IDLE_STATE) crc = 0x95;  // correct crc for CMD0 with arg 0
  if (cmd == SEND_IF_COND)  crc = 0x87;  // correct crc for CMD8 with arg 0X1AA
  SPITransfer(crc);

  uint8_t i;
  for (i = 0; ((status_ = SPITransfer(0xff)) & 0X80) && i != 0XFF; i++);
  return status_;
}

static uint8_t cardAcmd(uint8_t cmd, uint32_t arg) 
{
  cardCommand(APP_CMD, 0);
  return cardCommand(cmd, arg);
}

static uint8_t waitStartBlock(void) 
{
  uint8_t status_;

  uint16_t t0 = millis();
  while ((status_ = SPITransfer(0xFF)) == 0XFF) 
  {
    if (((uint16_t)millis() - t0) > SD_READ_TIMEOUT) 
    {
      //_errorCode = SD_CARD_ERROR_READ_TIMEOUT;
      goto fail;
    }
  }
  if (status_ != STATUS_START_BLOCK) 
  {
    //_errorCode = SD_CARD_ERROR_READ;
    goto fail;
  }
  return 1;

fail:
  return 0;
}

void Xsdcard_init(int pin)
{
  SB_sdcardValid=0;
  uint16_t t0 = (uint16_t)millis();
  uint32_t arg;
  uint8_t status_;

  spiPin=pin;

  SPIinitChannel();

  while ((status_ = cardCommand(GO_IDLE_STATE, 0)) != STATUS_IN_IDLE) 
  {
    if (((uint16_t)millis() - t0) > SD_INIT_TIMEOUT) 
    {
      //_errorCode = SD_CARD_ERROR_CMD0;
      goto fail;
    }
  }


  // check SD version
  if ((cardCommand(SEND_IF_COND, 0x1AA) & STATUS_ILLEGAL_COMMAND)) 
  {
    _card_type = SD_CARD_TYPE_SD1;
  } 
  else 
  {
    // only need last byte of r7 response
    uint8_t i;
    for (i = 0; i < 4; i++) status_ = SPITransfer(0xFF);
    if (status_ != 0XAA) 
    {
      //_errorCode = SD_CARD_ERROR_CMD8;
      goto fail;
    }
    _card_type = SD_CARD_TYPE_SD2;
  }


  // initialize card and send host supports SDHC if SD2
  arg=0;
  while ((status_ = cardAcmd(SD_SEND_OP_COND, arg)) != STATUS_READY) 
  {
    // check for timeout
    if (((uint16_t)millis() - t0) > SD_INIT_TIMEOUT) 
    {
      //_errorCode = SD_CARD_ERROR_ACMD41;
      goto fail;
    }
  }


  // if SD2 read OCR register to check for SDHC card
  if (_card_type == SD_CARD_TYPE_SD2) 
  {
    if (cardCommand(READ_OCR, 0)) 
    {
      //_errorCode = SD_CARD_ERROR_CMD58;
      goto fail;
    }
    if ((SPITransfer(0xff) & 0XC0) == 0XC0) _card_type = SD_CARD_TYPE_SDHC;
    // discard rest of ocr - contains allowed voltage range
    uint8_t i;
    for (i = 0; i < 3; i++) SPITransfer(0xFF);
  }

  SB_sdcardValid=1;
fail:
  SPIdeselect();  
}

void readSector(unsigned long sector)
{
  uint8_t status_, tries;
  SB_sdcardValid=0;

  if (_card_type != SD_CARD_TYPE_SDHC)  sector <<= 9;

  tries=0;
  status_ = cardCommand(READ_SINGLE_BLOCK, sector);

  while ((status_) && (tries<SD_READ_RETRIES))
  {
    status_ = cardCommand(READ_SINGLE_BLOCK, sector);
  }
  if (status_)
  {
    //_errorCode = SD_CARD_ERROR_CMD17;
    goto fail;
  }

  status_ = waitStartBlock();
  if (!status_) 
  {
    //_errorCode=status_;
    goto fail;
  }

  
#ifdef AVR
  SPDR = 0XFF;
        uint16_t i;
        for (i = 0; i < BYTESPERSECTOR-1; i++)
    {
    while (!(SPSR & (1 << SPIF)));
    fileIOBuffer[i]=SPDR;
    SPDR = 0XFF;
    }
  while (!(SPSR & (1 << SPIF)));
  fileIOBuffer[BYTESPERSECTOR-1] =SPDR;
#else
  uint16_t i;
  for (i = 0; i < BYTESPERSECTOR; i++) 
  {
    fileIOBuffer[i] = SPITransfer(0xff);
  }
#endif
  SB_sdcardValid=1;
fail:
        SPIdeselect();
  return ;
}

void writeSector(uint32_t sector)
{
  uint8_t status_;

  if (_card_type != SD_CARD_TYPE_SDHC) sector <<= 9;

  if (cardCommand(WRITE_BLOCK, sector)) 
  {
    //_errorCode = SD_CARD_ERROR_CMD24;
    goto fail;
  }


#ifdef AVR
  SPDR = DATA_START_BLOCK;

  uint16_t i;
  for (i = 0; i < 512; i += 2) 
  {
    while (!(SPSR & (1 << SPIF)));
    SPDR = fileIOBuffer[i];
    while (!(SPSR & (1 << SPIF)));
    SPDR = fileIOBuffer[i+1];
  }

  while (!(SPSR & (1 << SPIF)));
#else
  SPITransfer(DATA_START_BLOCK);
  uint16_t i;
  for (i = 0; i < 512; i++)
  {
  SPITransfer(fileIOBuffer[i]);
  }

#endif

  SPITransfer(0xff);  // dummy crc
  SPITransfer(0xff);  // dummy crc

  status_ = SPITransfer(0xff);
  if ((status_ & DATA_RES_MASK) != DATA_RES_ACCEPTED) 
  {
    //_errorCode = SD_CARD_ERROR_WRITE;
    goto fail;
  }
  
  // wait for flash programming to complete
  if (!waitNotBusy(SD_WRITE_TIMEOUT)) 
  {
    //_errorCode = SD_CARD_ERROR_WRITE_TIMEOUT;
    goto fail;
  }
  // response is r2 so get and check two bytes for nonzero
  if (cardCommand(SEND_STATUS, 0) || SPITransfer(0xff)) 
  {
    //_errorCode = SD_CARD_ERROR_WRITE_PROGRAMMING;
    goto fail;
  }

  SB_sdcardValid=1;
fail:
        SPIdeselect();
  return;
}


