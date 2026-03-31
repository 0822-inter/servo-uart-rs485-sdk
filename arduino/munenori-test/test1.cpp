#include <FashionStar_UartServo.h>

// D1, D2 ではなく数字で直接指定してみる
#define TX_PIN 2 // もしD1なら
#define RX_PIN 1 // もしD2なら
#define BAUDRATE 115200

FSUS_Protocol protocol(BAUDRATE);

void setup()
{
    Serial.begin(115200);
    Serial2.begin(BAUDRATE, SERIAL_8N1, RX_PIN, TX_PIN);

    // ライブラリの内部シリアルを強制的にSerial2に紐付け
    // これがエラーになる場合は、行を消してOKです
    // protocol.serial = &Serial2;
    protocol.serial = &Serial2;
    delay(3000);
    Serial.println("Robot Motor Test Start...");

    // トルク設定（エラー回避のため一旦コメントアウトするか、角度送信で代用します）
    // 角度を一度送れば、通常は自動的にトルクが入ります。
}

void loop()
{
    Serial.println("Moving ID 1 to 120 degrees...");

    // 修正ポイント：引数を4つにします！
    // 第4引数の「0」は通常「最大パワー」や「デフォルト」を意味します
    protocol.sendSetAngle(1, 120.0, 1000, 0);
    delay(2000);

    Serial.println("Moving ID 1 to 90 degrees...");
    protocol.sendSetAngle(1, 90.0, 1000, 0);
    delay(2000);
}