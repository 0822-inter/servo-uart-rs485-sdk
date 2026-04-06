#include <Arduino.h>
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define TX_PIN 43
#define RX_PIN 44
#define BAUDRATE 115200
#define NUM_SERVOS 17

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo *servos[NUM_SERVOS];

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial1.begin(BAUDRATE, SERIAL_8N1, RX_PIN, TX_PIN);
    protocol.serial = &Serial1;

    for (int i = 0; i < NUM_SERVOS; i++)
    {
        servos[i] = new FSUS_Servo(i, &protocol);
        servos[i]->init();

        // 1軸ずつ「0度」へ移動（ゆっくり1.5秒かける）
        Serial.printf("ID:%d を0度に固定中...\n", i);
        servos[i]->setRawAngle(0.0, 1500);
        delay(1600); // 1軸終わるまで待つ
    }

    Serial.println("\n✅ 全軸の0度固定が完了しました。");
    Serial.println("この状態で、腕や足が『真下』を向くように組み直してください。");
}

void loop()
{
    // 何もしない（0度を維持し続ける）
}