#include <Arduino.h>
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

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

    Serial.println("\n=== 17軸 関節マッピング（点呼）テスト ===");

    Serial1.begin(BAUDRATE, SERIAL_8N1, RX_PIN, TX_PIN);
    delay(100);
    protocol.serial = &Serial1;

    for (int i = 0; i < NUM_SERVOS; i++)
    {
        servos[i] = new FSUS_Servo(servo_ids[i], &protocol);
        servos[i]->init();
    }

    Serial.println("⚠️ 5秒後に【順番に】ピクッと動き出します！ロボットが倒れないよう支えてください！");
    delay(5000);
}

void loop()
{
    for (int i = 0; i < NUM_SERVOS; i++)
    {
        int current_id = servo_ids[i];
        bool isOnline = servos[i]->ping();

        if (isOnline)
        {
            Serial.printf("\n🤖 [ID: %2d] を動かします...\n", current_id);

            // 【安全対策】現在の角度を読み取って、そこから相対的に動かす
            float current_angle = servos[i]->queryRawAngle();

            // 現在地から +20度 の位置へ、500ミリ秒かけて移動
            servos[i]->setRawAngle(current_angle + 20.0, 500);
            delay(600);

            // 元の角度へ戻る
            servos[i]->setRawAngle(current_angle, 500);
            delay(1000);

            Serial.println("   -> 今動いた関節をメモしてください！");
        }
        else
        {
            Serial.printf("\n❌ [ID: %2d] は応答なし。スキップします。\n", current_id);
        }

        delay(1000); // 次のモーターにいく前に1秒待機
    }

    Serial.println("\n✅ 全モーターの点呼完了！10秒後に最初から繰り返します...");
    Serial.println("---------------------------------------------------");
    delay(10000);
}