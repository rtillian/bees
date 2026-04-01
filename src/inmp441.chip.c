#include "wokwi-api.h"
#include <stdio.h>

typedef struct {
  pin_t pin_ws;
  pin_t pin_sck;
  pin_t pin_sd;
  pin_t pin_lr;
  uint32_t volume_attr;
} chip_state_t;

static void chip_init(void) {
  chip_state_t *state = (chip_state_t*)malloc(sizeof(chip_state_t));

  state->pin_ws = pin_init("WS", INPUT);
  state->pin_sck = pin_init("SCK", INPUT);
  state->pin_sd  = pin_init("SD", OUTPUT);
  state->pin_lr  = pin_init("L/R", INPUT);
  state->volume_attr = attr_init("volume", 2048);

  printf("INMP441 Custom Chip initialized\n");
}

static void chip_loop(void) {
  // Hier wird später die I²S-Simulation implementiert
  // Für den Anfang geben wir einfach den Volume-Wert aus
}

void chip_main(void) {
  chip_init();
  while (1) {
    chip_loop();
    timer_sleep(1000);   // 1ms
  }
}