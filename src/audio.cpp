//Audio
#include "AudioTools.h"
#include "Filter.h"
#include "AudioLibs/AudioSTK.h"
#include "modular-ui.h"

#define MAX_AUDIO_LEVEL 32800
#define MAX_BRIGHTNESS 255
#define CHANNELS 2
#define BITS_PER_SAMPLE 16
#define SAMPLE_RATE 44100

ExponentialFilter<long> ADCFilter(5, 0);

I2SStream i2sStream;
VolumePrint volume;
StreamCopy copier(volume, i2sStream);

int audioLevel = 0;

void setupAudio()
{
  AudioLogger::instance().begin(Serial, AudioLogger::Error);
    
  auto config = i2sStream.defaultConfig(RXTX_MODE);
  config.i2s_format = I2S_STD_FORMAT;
  config.bits_per_sample = BITS_PER_SAMPLE;
  config.channels = CHANNELS;
  config.sample_rate = SAMPLE_RATE;
  config.is_master = true;
  config.use_apll = false;
  config.pin_ws = 25;
  config.pin_bck = 5;
  config.pin_data = 26;
  config.pin_data_rx = 35;
  config.pin_mck = 0;  

 
  i2sStream.begin(config);
  volume.begin(config);
}

void handleAudio()
{
  copier.copy();
  float vol = volume.volume(1);
  Serial.println(vol);
  float percent = vol / MAX_AUDIO_LEVEL;
  float audioBrightness = percent * MAX_BRIGHTNESS;

  ADCFilter.Filter(audioBrightness);
  audioLevel = ADCFilter.Current();
}
