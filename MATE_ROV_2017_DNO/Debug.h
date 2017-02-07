#pragma once

#ifdef _DEBUG
#define LOG(str) Serial.print(str);
#define LOGHEX(str) Serial.print(str, HEX);
#define LOGLN(str) Serial.println(str); delay(50);
#else
#define LOG(str)
#define LOGLN(str)
#endif
