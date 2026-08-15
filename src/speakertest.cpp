//included default speaker test code from github.com/nulllaborg/i2s_audio_amplifier_module
//only changed pin numbers

// #define CLOG_PREFIX_DATE (0)
// #define CLOG_PREFIX_PID (0)
// #define CLOG_PREFIX_TID (0)

// #include <Arduino.h>
// #include "clog.hpp"
// #include "driver/gpio.h"
// #include "driver/i2s.h"

// namespace {
// #include "pcm.h"
// #define I2S_PORT I2S_NUM_0
// }  // namespace

// void setup() {
//   Serial.begin(115200);
//   CLOGI << "setup";

//   i2s_config_t i2s_config = {
//       .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
//       .sample_rate = 16000,
//       .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
//       .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//       .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
//       .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
//       .dma_buf_count = 8,
//       .dma_buf_len = 256,  
//       .use_apll = true,   
//       .tx_desc_auto_clear = true,
//       .fixed_mclk = 0
//   };

//   i2s_pin_config_t pin_config = {
//       .bck_io_num = GPIO_NUM_8,
//       .ws_io_num = GPIO_NUM_7,
//       .data_out_num = GPIO_NUM_9,
//       .data_in_num = I2S_PIN_NO_CHANGE
//   };

//   // 安装I2S驱动
//   esp_err_t err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
//   if (err != ESP_OK) {
//     CLOGE << "i2s_driver_install failed: " << err;
//     return;
//   }
  
//   err = i2s_set_pin(I2S_PORT, &pin_config);
//   if (err != ESP_OK) {
//     CLOGE << "i2s_set_pin failed: " << err;
//     return;
//   }

//   CLOGI << "Writing PCM data, size: " << sizeof(_MERRY_CHRISTMAS_PCM) << " bytes";
  
//   size_t bytes_written = 0;
//   size_t total_bytes = sizeof(_MERRY_CHRISTMAS_PCM);
//   size_t bytes_to_write = total_bytes;
//   const uint8_t* pcm_data = (const uint8_t*)_MERRY_CHRISTMAS_PCM;
  
//   while (bytes_to_write > 0) {
//     size_t chunk_size = (bytes_to_write > 1024) ? 1024 : bytes_to_write;
//     size_t chunk_written = 0;
    
//     err = i2s_write(I2S_PORT, pcm_data + (total_bytes - bytes_to_write), 
//                     chunk_size, &chunk_written, pdMS_TO_TICKS(1000));
    
//     if (err != ESP_OK) {
//       CLOGE << "i2s_write failed at offset " << (total_bytes - bytes_to_write) 
//             << ": " << err;
//       break;
//     }
    
//     bytes_written += chunk_written;
//     bytes_to_write -= chunk_written;
    
//     if (chunk_written == 0) {
//       CLOGW << "No bytes written, possible buffer full";
//       delay(10);
//     }
//   }
  
//   CLOGI << "Total bytes written: " << bytes_written << "/" << total_bytes;
//   delay(100);
//   CLOGI << "setup OK - Audio should be playing now";
// }

// void loop() {
// }