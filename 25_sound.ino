#include <driver/i2s.h>
#include <math.h>

#define I2S_BCK  10
#define I2S_WS   12
#define I2S_DOUT 11
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 128

int16_t squareTable[TABLE_SIZE]; 
int16_t sineTable[TABLE_SIZE]; 
uint32_t phaseAccum = 0;        

void initSound() {
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        squareTable[i] = (i < TABLE_SIZE/2)? WAVE_AMP:-1*WAVE_AMP;
        sineTable[i] =  (int16_t)(sinf(i * 2.0f * PI / TABLE_SIZE) * WAVE_AMP);
    
        soundTable[i] = sineTable[i];
    }
    
    i2s_config_t config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 16,
        .dma_buf_len = BUFFER_SIZE,
        .use_apll = false,
        .tx_desc_auto_clear = true
    };
    
    i2s_pin_config_t pins = {
        .bck_io_num = I2S_BCK,
        .ws_io_num = I2S_WS,
        .data_out_num = I2S_DOUT,
        .data_in_num = I2S_PIN_NO_CHANGE
    };
    
    i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pins);
    
    Serial.println("Sine wavetable ready. m=mute u=unmute");
}

void updateSound() {
    if (Serial.available()) {
        char c = Serial.read();
        if (c == 'm') noteActive = false;
        if (c == 'u') noteActive = true;
    }
    
    int16_t buffer[BUFFER_SIZE * 2];
    size_t written;
    
    if (noteActive) {
        uint32_t phaseInc = (uint32_t)(currentFrequency * TABLE_SIZE * 65536.0f / SAMPLE_RATE);
        for (int i = 0; i < BUFFER_SIZE; i++) {
            int idx = (phaseAccum >> 16) % TABLE_SIZE;
            updateWaveTable();
            int16_t sample = masterVolume*(1.0*currentVelocity/(1.0*MAX_VELOCITY))*soundTable[idx];
            buffer[i * 2] = sample;
            buffer[i * 2 + 1] = sample;
            phaseAccum += phaseInc;
        }
    } else {
        memset(buffer, 0, sizeof(buffer));
    }
    
    i2s_write(I2S_NUM_0, buffer, sizeof(buffer), &written, portMAX_DELAY);
}

void updateWaveTable(){
    for(int i = 0; i < TABLE_SIZE; i++){
        int16_t sineSample = sineTable[i];
        int16_t squareSample = squareTable[i];
        soundTable[i] = sineSample + (int16_t)((squareSample - sineSample)*waveMix);
    }
}