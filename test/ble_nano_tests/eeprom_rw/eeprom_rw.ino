/* LoadAndSaveSettings
 * Joghurt 2010
 * Demonstrates how to load and save settings to the EEPROM
 */
// Contains EEPROM.read() and EEPROM.write()
#include <EEPROM.h>

// ID of the settings block
#define CONFIG_VERSION "ls1"

// Tell it where to store your config data in EEPROM
#define CONFIG_START 32

// Example settings structure
struct StoreStruct {
  // This is for mere detection if they are your settings
  char version[4];
  // The variables of your settings
  int a, b;
  char c;
  long d;
  float e[6];
} storage = {
  CONFIG_VERSION,
  // The default values
  220, 1884,
  'c',
  10000,
  {4.5, 5.5, 7, 8.5, 10, 12}
};

void loadConfig() {
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2])
    for (unsigned int t=0; t<sizeof(storage); t++)
      *((char*)&storage + t) = EEPROM.read(CONFIG_START + t);
}

void saveConfig() {
  for (unsigned int t=0; t<sizeof(storage); t++)
    EEPROM.write(CONFIG_START + t, *((char*)&storage + t));
}

void setup() {
  loadConfig();
}

void loop() {
  // [...]
  int i = storage.c - 'a';
  // [...]

  // [...]
  storage.c = 'a';
  if (ok)
    saveConfig();
  // [...]
}
