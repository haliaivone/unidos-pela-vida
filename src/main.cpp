#include <Arduino.h>

/*
 * Projeto: 4 LEDs e 4 Botões - Arduino Uno
 * Descrição: Cada botão controla o seu LED correspondente.
 *            Pressionar o botão liga/desliga o LED (toggle).
 *
 * Ligações:
 * ┌─────────┬────────────┬──────────────────────────────────────┐
 * │ Pino    │ Componente │ Notas                                │
 * ├─────────┼────────────┼──────────────────────────────────────┤
 * │ D2      │ LED 1      │ Com resistência de 220Ω para GND     │
 * │ D3      │ LED 2      │ Com resistência de 220Ω para GND     │
 * │ D4      │ LED 3      │ Com resistência de 220Ω para GND     │
 * │ D5      │ LED 4      │ Com resistência de 220Ω para GND     │
 * │ D6      │ Botão 1    │ INPUT_PULLUP (outro terminal ao GND) │
 * │ D7      │ Botão 2    │ INPUT_PULLUP (outro terminal ao GND) │
 * │ D8      │ Botão 3    │ INPUT_PULLUP (outro terminal ao GND) │
 * │ D9      │ Botão 4    │ INPUT_PULLUP (outro terminal ao GND) │
 * └─────────┴────────────┴──────────────────────────────────────┘
 */

// ── Pinos dos LEDs ──────────────────────────────────────────────
const int LED_PINS[4] = {2, 3, 4, 5};

// ── Pinos dos Botões ────────────────────────────────────────────
const int BTN_PINS[4] = {6, 7, 8, 9};

// ── Estado dos LEDs (true = aceso) ──────────────────────────────
bool ledState[4] = {false, false, false, false};

// ── Último estado lido dos botões (para debounce) ───────────────
bool lastBtnState[4]    = {HIGH, HIGH, HIGH, HIGH};
bool currentBtnState[4] = {HIGH, HIGH, HIGH, HIGH};

// ── Tempo do último evento de debounce por botão ────────────────
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};
const unsigned long DEBOUNCE_DELAY = 50; // ms

// ────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);
  Serial.println("=== 4 LEDs + 4 Botões iniciado ===");

  for (int i = 0; i < 4; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);

    // INPUT_PULLUP: sem resistência externa necessária
    pinMode(BTN_PINS[i], INPUT_PULLUP);
  }
}

// ────────────────────────────────────────────────────────────────
void loop() {
  for (int i = 0; i < 4; i++) {
    int reading = digitalRead(BTN_PINS[i]);

    // Resetar o timer de debounce se a leitura mudou
    if (reading != lastBtnState[i]) {
      lastDebounceTime[i] = millis();
    }

    // Verificar se passou tempo suficiente (debounce estável)
    if ((millis() - lastDebounceTime[i]) > DEBOUNCE_DELAY) {
      // Se o estado confirmado mudou
      if (reading != currentBtnState[i]) {
        currentBtnState[i] = reading;

        // Acionar apenas na descida (botão pressionado = LOW com PULLUP)
        if (currentBtnState[i] == LOW) {
          ledState[i] = !ledState[i];
          digitalWrite(LED_PINS[i], ledState[i] ? HIGH : LOW);

          Serial.print("Botão ");
          Serial.print(i + 1);
          Serial.print(" pressionado → LED ");
          Serial.print(i + 1);
          Serial.println(ledState[i] ? " LIGADO" : " DESLIGADO");
        }
      }
    }

    lastBtnState[i] = reading;
  }
}