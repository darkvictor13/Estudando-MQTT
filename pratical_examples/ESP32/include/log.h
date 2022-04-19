#ifndef LOG
#define LOG

#include <esp_log.h>

#define LOGE(...) ESP_LOGE(TAG, __VA_ARGS__)
#define LOGD(...) ESP_LOGD(TAG, __VA_ARGS__)

#endif // LOG
