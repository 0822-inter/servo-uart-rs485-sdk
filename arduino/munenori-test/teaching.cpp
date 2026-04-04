#include <Arduino.h>
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

// XIAO ESP32-S3 ピン設定
#define TX_PIN 43
#define RX_PIN 44
#define BAUDRATE 115200
#define NUM_SERVOS 17

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo *servos[NUM_SERVOS];
int servo_ids[NUM_SERVOS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(10);
    }
    delay(1000);

    Serial.println("\n=== 角度読み取り専用モード (物理電源OFF作戦用) ===");

    Serial1.begin(BAUDRATE, SERIAL_8N1, RX_PIN, TX_PIN);
    delay(100);
    protocol.serial = &Serial1;

    for (int i = 0; i < NUM_SERVOS; i++)
    {
        servos[i] = new FSUS_Servo(servo_ids[i], &protocol);
        // 書き込み関数は一切使わないのでエラーになりません
    }

    Serial.println("✅ 準備完了！");
    Serial.println("1. サーボの電源（青端子）を抜いて、手でポーズを作ってください。");
    Serial.println("2. ポーズができたら電源を入れてください。角度が表示されます。");
}

void loop()
{
    Serial.print("float pose[] = {");

    for (int i = 0; i < NUM_SERVOS; i++)
    {
        // queryRawAngle は動作確認済みなので安心です
        float angle = servos[i]->queryRawAngle();

        Serial.print(angle);
        if (i < NUM_SERVOS - 1)
        {
            Serial.print(", ");
        }
        delay(10);
    }

    Serial.println("};");
    delay(3000); // 3秒おきに表示
}