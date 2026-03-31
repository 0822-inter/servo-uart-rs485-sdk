#include <HardwareSerial.h>

// ライブラリが ESP32 では Serial2 を使う
// まずライブラリと同じ Serial2 を試す
HardwareSerial &ServoSerial = Serial2;

void setup()
{
    Serial.begin(115200);
    delay(2000);

    // ライブラリのデフォルトと同じ初期化
    ServoSerial.begin(115200);
    delay(100);
    Serial.println("=== 正しいフレームでテスト ===");
}

void queryServo(uint8_t servoId)
{
    // 正しいフレーム: [0x12][0x4C][CMD=0x0A][DataLen=0x01][ServoID][Checksum]
    uint8_t checksum = (0x0A + 0x01 + servoId) % 256;
    uint8_t frame[] = {0x12, 0x4C, 0x0A, 0x01, servoId, checksum};

    while (ServoSerial.available())
        ServoSerial.read(); // バッファクリア

    Serial.printf("送信 ID=%d: ", servoId);
    for (int i = 0; i < 6; i++)
        Serial.printf("0x%02X ", frame[i]);
    Serial.println();

    ServoSerial.write(frame, 6);
    ServoSerial.flush();
    delay(50);

    // レスポンスヘッダーは 0x05 0x1C
    Serial.print("受信: ");
    int count = 0;
    while (ServoSerial.available())
    {
        Serial.printf("0x%02X ", ServoSerial.read());
        count++;
    }
    if (count == 0)
        Serial.print("(なし)");
    Serial.println();
}

void loop()
{
    for (uint8_t id = 1; id <= 3; id++)
    {
        queryServo(id);
        delay(200);
    }
    delay(2000);
}