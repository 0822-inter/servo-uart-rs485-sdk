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

    Serial.println("\n=== 🎯 関節の回転方向（極性）特定プログラム ===");

    Serial1.begin(BAUDRATE, SERIAL_8N1, RX_PIN, TX_PIN);
    delay(100);
    protocol.serial = &Serial1;

    for (int i = 0; i < NUM_SERVOS; i++)
    {
        servos[i] = new FSUS_Servo(servo_ids[i], &protocol);
        servos[i]->init();
    }

    Serial.println("\n【使い方】");
    Serial.println("シリアルモニタの上の入力欄に、動かしたいID(0〜16)を半角数字で入力してEnterを押してください。");
    Serial.println("対象のモーターが「現在地から +20度」の方向へゆっくり動き、2秒後に元の位置に戻ります。");
    Serial.println("---------------------------------------------------");
}

void loop()
{
    // シリアルモニタから入力があるかチェック
    if (Serial.available() > 0)
    {
        // 入力された数字（ID）を読み取る
        int target_id = Serial.parseInt();

        // 改行コードなどがバッファに残るのを防ぐため読み捨てる
        while (Serial.available() > 0)
        {
            Serial.read();
        }

        // 入力されたIDが0〜16の範囲内かチェック
        if (target_id >= 0 && target_id < NUM_SERVOS)
        {
            bool isOnline = servos[target_id]->ping();

            if (isOnline)
            {
                Serial.printf("\n🤖 [ID: %2d] を +20度 方向へ動かします...\n", target_id);

                float current_angle = servos[target_id]->queryRawAngle();

                // +20度の位置へ、1000ミリ秒(1秒)かけて移動
                servos[target_id]->setRawAngle(current_angle + 10.0, 1000);
                delay(2000); // 動き終わってから1秒キープ

                // 元の角度へ戻る
                Serial.println("   -> 元の位置に戻ります。");
                servos[target_id]->setRawAngle(current_angle, 1000);
                delay(1000);

                Serial.println("✅ テスト完了。次のIDを入力してください。");
            }
            else
            {
                Serial.printf("\n❌ [ID: %2d] は応答がありません。電源と配線を確認してください。\n", target_id);
            }
        }
        else
        {
            Serial.println("\n⚠️ エラー: 0から16までの数字を入力してください。");
        }
    }
}