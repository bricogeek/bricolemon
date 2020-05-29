#include <Arduino.h>
#include <sam.h>
//https://github.com/GMagician/SAMD51-SmartEEprom-Manager

#if NVMCTRL_FUSES_SEEPSZ_ADDR != NVMCTRL_FUSES_SEESBLK_ADDR || ((NVMCTRL_FUSES_SEEPSZ_ADDR ^ NVMCTRL_USER) & ~0xF)
  #error Code needs to be changed
#endif

#define exec_cmdaddr(cmd,a)   do{                                                   \
                                NVMCTRL->ADDR.reg = (uint32_t)a;                    \
                                NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | cmd; \
                                while (!NVMCTRL->STATUS.bit.READY) { asm (""); }    \
                              }while(0)
#define exec_cmd(cmd)         exec_cmdaddr(cmd, NVMCTRL_USER)

#define SEEFUSESINDEX         (NVMCTRL_FUSES_SEEPSZ_ADDR - NVMCTRL_USER)

#define SEEPSZ                ((userPage[SEEFUSESINDEX] & NVMCTRL_FUSES_SEEPSZ_Msk) >> NVMCTRL_FUSES_SEEPSZ_Pos)
#define SEESBLK               ((userPage[SEEFUSESINDEX] & NVMCTRL_FUSES_SEESBLK_Msk) >> NVMCTRL_FUSES_SEESBLK_Pos)
#define MAKESEEFUSES(p,s)     ((userPage[SEEFUSESINDEX] & ~NVMCTRL_FUSES_SEEPSZ_Msk & ~NVMCTRL_FUSES_SEESBLK_Msk) | NVMCTRL_FUSES_SEEPSZ(p) | NVMCTRL_FUSES_SEESBLK(s))


const struct {
  size_t size;
  uint8_t psz, sblk;
} SEEConverter[] = {
  { 0, 0, 0}, 
  { 512, 0, 1}, 
  { 1024, 1, 1}, 
  { 2048, 2, 1}, 
  { 4096, 3, 1}, 
  { 8192, 4, 2}, 
  { 16384, 5, 3}, 
  { 32768, 6, 5}, 
  { 65536, 7, 10}
};

static uint8_t userPage[128*4];
static String receivedLine = String();


void updateFuses(uint8_t newSEEFuses) {
    while (!NVMCTRL->STATUS.bit.READY) { asm(""); }

    NVMCTRL->CTRLA.bit.WMODE = NVMCTRL_CTRLA_WMODE_MAN;

    const bool format = ((newSEEFuses ^ userPage[SEEFUSESINDEX]) & newSEEFuses);
    if (format)
      exec_cmd(NVMCTRL_CTRLB_CMD_EP);
    exec_cmd(NVMCTRL_CTRLB_CMD_PBC);

    userPage[SEEFUSESINDEX] = newSEEFuses;

    const int ei = format ? (int)sizeof(userPage) : (SEEFUSESINDEX + 1);
    for (int i = 0; i < ei; i += 16) {
      uint8_t *const qwBlockAddr = (uint8_t *const)(NVMCTRL_USER + i);
      memcpy (qwBlockAddr, &userPage[i], 16);
      exec_cmdaddr(NVMCTRL_CTRLB_CMD_WQW, qwBlockAddr);
    }
}


void showHelp(void) {
  Serial.println("SAMD51 SmartEEprom Manager V2.1\n");
  Serial.println("Please send:");
  Serial.println("'?' to get this help");
  Serial.println("'SetSize nnn' to set EEPROM size, allowed values are: 0, 512, 1024, 2048, 4096, 16384, 32768 and 65536");
  Serial.println("'GetSize' to get current EEPROM size");
  Serial.println("'SetSEEPSZ' to write directly to SEEPSZ fuse");
  Serial.println("'SetSEESBLK' to write directly to SEESBLK fuse");
}


void setSEESize(int s) {
  int si = sizeof(SEEConverter) / sizeof(SEEConverter[0]);
  while (--si >= 0 && SEEConverter[si].size != s)
    ;
  if (si < 0) {
    Serial.println("Invalid EEPROM size");
    return;
  }

  uint8_t newSEEFuses = MAKESEEFUSES(SEEConverter[si].psz, SEEConverter[si].sblk);

  if (newSEEFuses == userPage[SEEFUSESINDEX]) {
    Serial.print("EEPROM already set to ");
    Serial.println(s);
  }
  else {
    updateFuses(newSEEFuses);
    Serial.print("EEPROM size set to ");
    Serial.println(s);
  }
}


void getSEESize(void) {
  const uint8_t psz = SEEPSZ,
                sblk = SEESBLK;

  if (!psz && !sblk)
    Serial.print("EEPROM is disabled");
  else {
    Serial.print("EEPROM size is ");
         if (psz <= 2)              Serial.print(0x200 << psz);
    else if (sblk == 1 || psz == 3) Serial.print(4096);
    else if (sblk == 2 || psz == 4) Serial.print(8192);
    else if (sblk <= 4 || psz == 5) Serial.print(16384);
    else if (sblk >= 9 && psz == 7) Serial.print(65536);
    else                            Serial.print(32768);
    Serial.print(" bytes");
  }
  Serial.print(" (SEEPSZ=");
  Serial.print(psz);
  Serial.print(" SEESBLK=");
  Serial.print(sblk);
  Serial.println(")");
}


bool commandWithValue(String command, int &value) {
  int i = (command.length() + 1);
  int lineLen = receivedLine.length();
  if (receivedLine.startsWith(command) && lineLen > i && receivedLine.charAt(i - 1) == ' ') {
    String valueString = receivedLine.substring(i);
    while (i < lineLen && isDigit(receivedLine.charAt(i)))
      ++i;
    value = valueString.toInt();
    return i >= lineLen;
  }

  return false;
}


void setup() {
  memcpy(userPage, (uint8_t *const)NVMCTRL_USER, sizeof(userPage));
  Serial.begin(250000);
}


void loop() {
  static bool serialOn = false;
  int value;

  if (!Serial) return;

  if (!serialOn) {
    showHelp();
    serialOn = true;
    }

  while (Serial.available() > 0) {
    char ch = Serial.read();
    if (ch != '\r' && ch != '\n')
      receivedLine += ch;
    else {
      receivedLine.trim();
      receivedLine.toLowerCase();
      if (receivedLine == "?")
       showHelp();
      else if (commandWithValue("setsize", value))
        setSEESize(value);
      else if (receivedLine == "getsize")
        getSEESize();
      else if (commandWithValue("setseepsz", value)) {
       if (value == SEEPSZ)
          Serial.print("SEEPSZ unchanged");
       else if (value >= 0 && value <= 7)
          updateFuses(MAKESEEFUSES(value, SEESBLK));
        else
          Serial.println("0 <= SEEPSZ <= 7");
      }
      else if (commandWithValue("setseesblk", value)) {
       if (value == SEESBLK)
          Serial.print("SEESBLK unchanged");
       else if (value >= 0 && value <= 10)
          updateFuses(MAKESEEFUSES(SEEPSZ, value));
        else
          Serial.print("0 <= SEESBLK <= 10");
      }

    receivedLine = String();
    }
  }
}