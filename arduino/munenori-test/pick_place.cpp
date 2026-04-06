#include <Arduino.h>
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

// ========================
// ハードウェア設定
// ========================
#define TX_PIN 43
#define RX_PIN 44
#define BAUDRATE 115200
#define NUM_SERVOS 17

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo *servos[NUM_SERVOS];

// ★ここが重要：現在の角度を保存しておくための「器」
float current_pose[NUM_SERVOS];

// ========================
// ポーズデータ
// ========================
float pose_home[17] = {0.00, -6.20, 3.80, -0.20, 2.20, 1.60, 46.10, -9.20, 7.70, 1.20, -7.60, 82.20, -1.50, 0.20, 7.20, -66.70, -0.20};
float pose_grab[17] = {0.00, -6.80, 3.60, -0.10, 2.10, 1.60, 46.10, -9.80, 7.50, 1.10, -7.60, 99.10, -90.10, 0.20, 97.30, -66.70, -0.20};
float pose_lift[17] = {0.00, -7.10, 8.30, 0.10, 2.40, 1.60, 46.00, -9.00, 8.10, 0.60, 47.40, 101.50, -90.10, 0.20, 97.20, -79.60, -49.60};

// ========================
// データ送信関数（ここを追加）
// ========================
void sendEpisodeToPC(String eventName)
{
    // Python側が「これはデータだ！」と判別できる目印 (@DATA:) を付ける
    Serial.print("@DATA:{\"event\": \"");
    Serial.print(eventName);
    Serial.print("\", \"joints\": [");

    for (int i = 0; i < NUM_SERVOS; i++)
    {
        Serial.print(current_pose[i], 2); // 小数点第2位まで出力
        if (i < NUM_SERVOS - 1)
            Serial.print(", ");
    }

    Serial.println("]}");
}

// ========================
// 再生関数（current_poseを更新するように変更）
// ========================
void playMotion(float target_pose[], int duration)
{
    for (int i = 0; i < NUM_SERVOS; i++)
    {
        servos[i]->setRawAngle(target_pose[i], duration);

        // ★重要：命令した角度を current_pose に記録しておく
        current_pose[i] = target_pose[i];

        delay(12);
    }
    delay(duration + 500);
}

// ========================
// セットアップ
// ========================
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
        delay(5);
    }

    Serial.println("\n>> [System Start] 初期位置（Home）へ...");
    playMotion(pose_home, 3000);
    delay(1000);
}

// ========================
// メインループ
// ========================
void loop()
{
    // ① Grab
    Serial.println(">> [Action] Grab");
    playMotion(pose_grab, 2000);
    sendEpisodeToPC("grab_done"); // データ送信

    // ② Lift
    Serial.println(">> [Action] Lift");
    playMotion(pose_lift, 1500);
    sendEpisodeToPC("lift_done"); // データ送信

    // ③ Home
    Serial.println(">> [Action] Home");
    playMotion(pose_home, 2500);
    sendEpisodeToPC("cycle_complete"); // データ送信

    Serial.println(">> 完了。5秒待機...\n");
    delay(5000);
}