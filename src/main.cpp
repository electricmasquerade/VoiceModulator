//actual code for the mic and speaker, cobbled together from the testing files

#define CLOG_PREFIX_DATE (0)
#define CLOG_PREFIX_PID (0)
#define CLOG_PREFIX_TID (0)

#include <Arduino.h>
#include "clog.hpp"
#include "driver/gpio.h"
#include "driver/i2s.h"

namespace {
#include "pcm.h"
#define I2S_PORT I2S_NUM_0
}  // namespace

// you shouldn't need to change these settings
#define SAMPLE_BUFFER_SIZE 512
#define SAMPLE_RATE 16000
// most microphones will probably default to left channel but you may need to tie the L/R pin low
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
// pins are shared besides the in and out
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_8
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_7
#define I2S_MIC_SERIAL_DATA_IN GPIO_NUM_6
#define I2S_MIC_SERIAL_DATA_OUT GPIO_NUM_9

// don't mess around with this - definitely messing with this, need to combine the two i2s for speaker and mic into one "device"
static i2s_config_t i2s_config = {
    .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 128,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0};

// and don't mess around with this - too late
static i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_MIC_SERIAL_DATA_OUT,
    .data_in_num = I2S_MIC_SERIAL_DATA_IN};


//TODO: put all processing into another .h file later
static void process_audio(int32_t* samples, const size_t num_samples, const float gain = 1.0f, const float phase_offset = 0.0f) {

  for (size_t i = 0; i < num_samples; ++i) {
    //test with gain adjustment, but use a buffer value to prevent overflow

    float result = samples[i] * gain * sin(phase_offset);
    if (result > static_cast<float>(INT32_MAX)) {
      result = static_cast<float>(INT32_MAX);
    } else if (result < static_cast<float>(INT32_MIN)) {
      result = static_cast<float>(INT32_MIN);
    }
    samples[i] = static_cast<int32_t>(result);
  } 


}

void setup() {
  Serial.begin(115200);
  CLOGI << "setup";

  // 安装I2S驱动 - leaving this cause why not, means install i2S driver
  esp_err_t err = i2s_driver_install(I2S_PORT, &i2s_config, 0, nullptr);
  if (err != ESP_OK) {
    CLOGE << "i2s_driver_install failed: " << err;
    return;
  }
  
  err = i2s_set_pin(I2S_PORT, &pin_config);
  if (err != ESP_OK) {
    CLOGE << "i2s_set_pin failed: " << err;
    return;
  }
}


static int32_t raw_samples[SAMPLE_BUFFER_SIZE];

//phase accumulator for sine modulation and frequency for offset or whatever
static float phase;
static float carrier_freq = 60.0f;
void loop() {
  phase += 2.0f * M_PI * carrier_freq / SAMPLE_RATE;
  if (phase > 2 * M_PI) {
    phase -= 2 * M_PI;
  }
    // read from the I2S device
  size_t bytes_read = 0;


  //the read/write loop is literally this simple i guess
  i2s_read(I2S_PORT, raw_samples, sizeof(int32_t) * SAMPLE_BUFFER_SIZE, &bytes_read, portMAX_DELAY);
    
  process_audio(raw_samples, bytes_read / sizeof(int32_t), 2.0f, phase);

  i2s_write(I2S_PORT, raw_samples, bytes_read, &bytes_read, portMAX_DELAY);

}