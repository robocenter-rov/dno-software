#pragma once

#ifdef _DEBUG
#define LOG(str) Serial.print(str);
#define LOGLN(str) Serial.println(str);
#else
#define LOG(str)
#define LOGLN(str)
#endif
