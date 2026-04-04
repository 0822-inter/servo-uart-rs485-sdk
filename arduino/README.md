# 総合バスサーボモーターSDK使用マニュアル（Arduino Uno）

## 目次

1. [ライブラリのインストール](#1-バスサーボモーターのarduinoライブラリのインストール)
2. [書き込み時の注意事項](#2-書き込み時の注意事項)
3. [ソフトウェアシリアルによるログ出力](#3-ソフトウェアシリアルによるログ出力)
4. [サーボオブジェクトの作成と初期化](#4-サーボオブジェクトの作成と初期化)
5. [サーボ通信チェック](#5-サーボ通信チェック)
6. [サーボのダンピングモード](#6-サーボのダンピングモード)
7. [サーボ角度の取得](#7-サーボ角度の取得)
8. [サーボのホイールモード](#8-サーボのホイールモード)
9. [サーボ角度の設定](#9-サーボ角度の設定)
10. [サーボのブロッキング待機](#10-サーボのブロッキング待機)
11. [サーボ角度の設定 - マルチターンモード](#11-サーボ角度の設定---マルチターンモード)
12. [サーボのトルクスイッチ](#12-サーボのトルクスイッチ)
13. [サーボのキャリブレーション](#13-サーボのキャリブレーション)
14. [サーボ速度の設定](#14-サーボ速度の設定)
15. [サーボデータの読み取り](#15-サーボデータの読み取り)
16. [原点の設定](#16-原点の設定)
17. [マルチターン角度のリセット](#17-マルチターン角度のリセット)
18. [非同期コマンド](#18-非同期コマンド)
19. [データモニタリング](#19-データモニタリング)
20. [制御モード停止コマンド](#20-制御モード停止コマンド)
21. [同期制御 / 同期データモニタリング](#21-同期制御--同期データモニタリング)

---

## 1. バスサーボモーターのArduinoライブラリのインストール

`fashionstar-uart-servo-arduino-V2` フォルダを、Arduino IDEのインストールパス内にある `libraries` フォルダにそのままコピーしてください。

---

## 2. 書き込み時の注意事項

UNOにはハードウェアシリアルポートが1つしかないため、書き込み用ポートとバスサーボモーター制御用ポートを時分割で共用する必要があります。書き込みを行う際は、UC01変換基板とUNOの接続を切断してください。書き込み完了後、再度UC01とUNOを接続してください。

| 状態 | UC01 -- UNO | PC -- UNO |
|------|-------------|-----------|
| 書き込み時 | 切断 | 接続 |
| 使用時 | 接続 | 切断 |

---

## 3. ソフトウェアシリアルによるログ出力

1. USB-TTL変換モジュールのドライバをインストールしてください
2. ArduinoとUSB-TTL変換モジュールをジャンパー線で接続してください

| Arduino Uno | USB-TTL変換モジュール |
|---|---|
| pin6（ソフトシリアルRx） | Tx（モジュールの送信端） |
| pin7（ソフトシリアルTx） | Rx（モジュールの受信端） |
| GND | GND |

3. USB-TTL変換モジュールをPCのUSBポートに接続してください
4. シリアルモニターを開き、USB-TTL変換モジュールのポート番号を選択し、設定を行ってください
5. `Arduino IDE > Examples > fashionstar-uart-servo-arduino-V2 > software_serial` を開き、サンプルをArduino Unoに書き込んでください

---

## 4. サーボオブジェクトの作成と初期化

```cpp
#include "FashionStar_UartServoProtocol.h" // バスサーボモーター通信プロトコル
#include "FashionStar_UartServo.h"         // バスサーボモーターSDK
```

- **`FashionStar_UartServoProtocol`**：サーボの低レベル通信プロトコル処理（データフレームの送受信・データ検証など）を担います。
- **`FashionStar_UartServo`**：プロトコルの上位層にあたるサーボSDKです。

バスサーボモーター通信プロトコルオブジェクト `FSUS_Protocol` を作成します。コンストラクタにはArduinoとバスサーボモーターの通信ボーレートを指定します（デフォルト：115200）。

```cpp
#define BAUDRATE 115200          // ボーレート
FSUS_Protocol protocol(BAUDRATE); // プロトコル
```

`FSUS_Servo` サーボオブジェクトを作成します。サーボのID（範囲：0〜254）と通信プロトコルオブジェクトのポインタ `&protocol` を渡してください。

```cpp
#define SERVO_ID 0                          // サーボID番号
FSUS_Servo uservo(SERVO_ID, &protocol);     // サーボの作成
```

`setup()` 関数内で通信プロトコルとサーボオブジェクトを初期化します。

```cpp
void setup() {
    protocol.init(); // サーボ通信プロトコルの初期化
    uservo.init();   // バスサーボモーターの初期化
}
```

---

## 5. サーボ通信チェック

### 5.1. API - `ping`

`ping()` 関数でサーボの通信チェックを行い、サーボがオンラインかどうかを確認します。

```cpp
bool isOnline = uservo.ping(); // サーボ通信チェック
```

### 5.2. サンプルコード

`servo_ping.ino`

```cpp
/*
 * サーボ通信チェック
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2023/03/13
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define SERVO_ID 0
#define BAUDRATE 115200

#if defined(ARDUINO_AVR_UNO)
    #include <SoftwareSerial.h>
    #define SOFT_SERIAL_RX 6
    #define SOFT_SERIAL_TX 7
    SoftwareSerial softSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);
    #define DEBUG_SERIAL softSerial
    #define DEBUG_SERIAL_BAUDRATE 4800

#elif defined(ARDUINO_AVR_MEGA2560)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_ESP32)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_STM32)
    #include <HardwareSerial.h>
    HardwareSerial Serial1(PA10, PA9);
    HardwareSerial Serial3(PB11, PB10);
    #define DEBUG_SERIAL Serial1
    #define DEBUG_SERIAL_BAUDRATE (uint32_t)115200
#endif

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo uservo(SERVO_ID, &protocol);

void setup() {
    protocol.init();
    uservo.init();
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
    DEBUG_SERIAL.println("Start To Ping Servo\n");
}

void loop() {
    bool isOnline = uservo.ping();
    String message = "servo #" + String(uservo.servoId, DEC) + " is ";
    if (isOnline) {
        message += "online";
    } else {
        message += "offline";
    }
    DEBUG_SERIAL.println(message);
    delay(1000);
}
```

**ログ出力**

```
Start To Ping Servo

servo #0 is online.
servo #0 is online.
servo #0 is online.
servo #0 is online.
```

---

## 6. サーボのダンピングモード

### 6.1. API - `setDamping`

サーボをダンピングモードに設定します。

```cpp
void FSUS_Servo::setDamping(FSUS_POWER_T power)
```

**入力パラメータ**

- `power`：サーボの出力（単位：mW）。値が大きいほどダンピング力も大きくなります。

**使用例**

```cpp
#define DAMPING_POWER 800 // ダンピングモードの出力（mW）: 500, 800, 1000

uservo.setDamping(DAMPING_POWER);
```

### 6.2. サンプルコード

`servo_damping.ino`

```cpp
/*
 * サーボをダンピングモードに設定
 * DAMPING_POWER の値を変えて、異なるダンピング力を体感してください
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2023/03/13
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define SERVO_ID 0
#define BAUDRATE 115200
#define DAMPING_POWER 800

#if defined(ARDUINO_AVR_UNO)
    #include <SoftwareSerial.h>
    #define SOFT_SERIAL_RX 6
    #define SOFT_SERIAL_TX 7
    SoftwareSerial softSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);
    #define DEBUG_SERIAL softSerial
    #define DEBUG_SERIAL_BAUDRATE 4800

#elif defined(ARDUINO_AVR_MEGA2560)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_ESP32)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_STM32)
    #include <HardwareSerial.h>
    HardwareSerial Serial1(PA10, PA9);
    HardwareSerial Serial3(PB11, PB10);
    #define DEBUG_SERIAL Serial1
    #define DEBUG_SERIAL_BAUDRATE (uint32_t)115200
#endif

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo uservo(SERVO_ID, &protocol);

void setup() {
    protocol.init();
    uservo.init();
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
    DEBUG_SERIAL.println("Set Servo Mode To Damping");
    uservo.setDamping(DAMPING_POWER);
}

void loop() {
    // TODO
}
```

**ログ出力**

```
Set Servo Mode To Damping
```

---

## 7. サーボ角度の取得

### 7.1. API - `queryAngle`

サーボの現在の実際の角度を取得します。サーボに角度照会コマンドを送り、角度値をサーボオブジェクトの `curAngle` プロパティに代入します。

```cpp
FSUS_SERVO_ANGLE_T FSUS_Servo::queryAngle()
```

**入力パラメータ**：なし

**出力パラメータ**：`curAngle` — サーボの現在の実際の角度

**使用例**

```cpp
// 例1
float curAngle = uservo.queryAngle();

// 例2
uservo.queryAngle(); // 角度を照会して更新
uservo.curAngle;     // 現在の実際の角度にアクセス
```

### 7.2. サンプルコード

`servo_query_angle.ino`

```cpp
/*
 * サーボ角度の読み返し実験
 * 手でサーボを動かすと、角度を読み返してシリアルに出力します
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2023/03/13
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define SERVO_ID 0
#define DAMPING_POWER 800
#define BAUDRATE 115200

#if defined(ARDUINO_AVR_UNO)
    #include <SoftwareSerial.h>
    #define SOFT_SERIAL_RX 6
    #define SOFT_SERIAL_TX 7
    SoftwareSerial softSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);
    #define DEBUG_SERIAL softSerial
    #define DEBUG_SERIAL_BAUDRATE 4800

#elif defined(ARDUINO_AVR_MEGA2560)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_ESP32)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_STM32)
    #include <HardwareSerial.h>
    HardwareSerial Serial1(PA10, PA9);
    HardwareSerial Serial3(PB11, PB10);
    #define DEBUG_SERIAL Serial1
    #define DEBUG_SERIAL_BAUDRATE (uint32_t)115200
#endif

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo uservo(SERVO_ID, &protocol);

void setup() {
    protocol.init();
    uservo.init();
    uservo.setDamping(DAMPING_POWER);
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
    DEBUG_SERIAL.println("Query Servo Angle\n");
}

void loop() {
    uservo.queryRawAngle();
    String message = "Status Code: " + String(uservo.protocol->responsePack.recv_status, DEC)
                   + " servo #" + String(uservo.servoId, DEC)
                   + " , Current Angle = " + String(uservo.curRawAngle, 1) + " deg";
    DEBUG_SERIAL.println(message);
    delay(1000);
}
```

**ログ出力**

```
Query Servo Angle

Status Code: 0 servo #0 , Current Angle = -99.0
Status Code: 0 servo #0 , Current Angle = -99.0
Status Code: 0 servo #0 , Current Angle = -99.0
```

---

## 8. サーボのホイールモード

### 8.1. API - `wheelStop`

ホイールモードで回転を停止します。

```cpp
void FSUS_Servo::wheelStop()
```

**入力パラメータ**：なし

### 8.2. API - `wheelRun`

ホイールを連続回転させます。

```cpp
void FSUS_Servo::wheelRun(uint8_t is_cw)
```

**入力パラメータ**

- `is_cw`：回転方向
  - `0`：反時計回り
  - `1`：時計回り

### 8.3. API - `wheelRunNTime`

ホイールを指定時間だけ回転させます。

```cpp
void FSUS_Servo::wheelRunNTime(uint8_t is_cw, uint16_t time_ms)
```

**入力パラメータ**

- `is_cw`：回転方向（`0`：反時計回り、`1`：時計回り）
- `time_ms`：回転時間（単位：ms）

### 8.4. API - `wheelRunNCircle`

ホイールを指定回転数だけ回転させます。

```cpp
void FSUS_Servo::wheelRunNCircle(uint8_t is_cw, uint16_t circle_num)
```

**入力パラメータ**

- `is_cw`：回転方向（`0`：反時計回り、`1`：時計回り）
- `circle_num`：回転数

### 8.5. サンプルコード

`servo_wheel_mode.ino`

```cpp
/*
 * サーボのホイールモードテスト
 * 注意: 拡張ボード通電後、Arduino の RESET ボタンを押してください
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2023/03/13
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define BAUDRATE 115200
#define SERVO_ID 0

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo uservo(SERVO_ID, &protocol);

/* 連続回転と停止のテスト */
void testWheelRunAndStop() {
    uservo.wheelRun(FSUS_CCW); // 反時計回りに連続回転
    delay(2000);
    uservo.wheelStop();
    delay(2000);
    uservo.wheelRun(FSUS_CW);  // 時計回りに連続回転
    delay(2000);
    uservo.wheelStop();
    delay(2000);
}

/* 指定時間回転のテスト */
void testWheelRunNTime() {
    uservo.wheelRunNTime(FSUS_CW, 5000);  // 5秒間時計回りに回転
    delay(5000);
    uservo.wheelRunNTime(FSUS_CCW, 5000); // 5秒間反時計回りに回転
    delay(5000);
}

/* 指定回転数回転のテスト */
void testWheelRunNCircle() {
    uint16_t nCircle = 2;
    uint16_t delayMsEstimate = (uint16_t)(360.0 * nCircle / uservo.speed * 1000);
    uservo.wheelRunNCircle(FSUS_CW, 2);  // 時計回りに2回転
    delay(delayMsEstimate);
    uservo.wheelRunNCircle(FSUS_CCW, 2); // 反時計回りに2回転
    delay(delayMsEstimate);
}

void setup() {
    protocol.init();
    uservo.init();
    uservo.setSpeed(100); // 回転速度を100°/sに設定

    // testWheelRunAndStop();
    // testWheelRunNTime();
    testWheelRunNCircle();
}

void loop() {
}
```

---

## 9. サーボ角度の設定

### 9.1. API - `setAngle`

サーボの角度を設定します。

```cpp
// サーボの生の角度を設定
void FSUS_Servo::setRawAngle(FSUS_SERVO_ANGLE_T rawAngle, FSUS_INTERVAL_T interval, FSUS_POWER_T power)
void FSUS_Servo::setRawAngle(FSUS_SERVO_ANGLE_T rawAngle, FSUS_INTERVAL_T interval)
void FSUS_Servo::setRawAngle(FSUS_SERVO_ANGLE_T rawAngle)
```

**入力パラメータ**

- `rawAngle`：目標角度（単位：°）
- `interval`：回転周期（単位：ms）
- `power`：最大出力（単位：mW）

### 9.2. API - `setRawAngleByInterval`

```cpp
// 周期を指定してサーボの生の角度を設定
void FSUS_Servo::setRawAngleByInterval(FSUS_SERVO_ANGLE_T rawAngle, FSUS_INTERVAL_T interval,
                                        FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power)
```

**入力パラメータ**

- `rawAngle`：目標角度（単位：°）
- `interval`：回転周期（単位：ms）
- `t_acc`：加速時間
- `t_dec`：減速時間
- `power`：最大出力（単位：mW）

### 9.3. API - `setRawAngleByVelocity`

```cpp
// 速度を指定してサーボの生の角度を設定
void FSUS_Servo::setRawAngleByVelocity(FSUS_SERVO_ANGLE_T rawAngle, FSUS_SERVO_SPEED_T velocity,
                                        FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power)
```

**入力パラメータ**

- `rawAngle`：目標角度（単位：°）
- `velocity`：回転速度（単位：°/s）
- `t_acc`：加速時間
- `t_dec`：減速時間
- `power`：最大出力（単位：mW）

### 9.4. API - `isStop`

サーボが回転中か静止中かを判定します。この関数は実行時にサーボの現在角度を照会し、`targetAngle` との差が制御デッドバンド以下かどうかを返します。

```cpp
bool FSUS_Servo::isStop()
```

**返り値**

- `true`：サーボが目標角度に到達し、静止している
- `false`：サーボがまだ目標角度に到達しておらず、回転中

### 9.5. API - `setRange`

サーボの角度範囲を設定します。

```cpp
void FSUS_Servo::setAngleRange(FSUS_SERVO_ANGLE_T minAngle, FSUS_SERVO_ANGLE_T maxAngle)
```

**入力パラメータ**

- `minAngle`：角度の下限
- `maxAngle`：角度の上限

### 9.6. サンプルコード

`servo_set_angle.ino`

```cpp
/*
 * サーボの角度設定（シングルターンモード）
 * 注意: 拡張ボード通電後、Arduino の RESET ボタンを押してください
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2023/03/13
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define SERVO_ID 0
#define BAUDRATE 115200

#if defined(ARDUINO_AVR_UNO)
    #include <SoftwareSerial.h>
    #define SOFT_SERIAL_RX 6
    #define SOFT_SERIAL_TX 7
    SoftwareSerial softSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);
    #define DEBUG_SERIAL softSerial
    #define DEBUG_SERIAL_BAUDRATE 4800

#elif defined(ARDUINO_AVR_MEGA2560)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_ESP32)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_STM32)
    #include <HardwareSerial.h>
    HardwareSerial Serial1(PA10, PA9);
    HardwareSerial Serial3(PB11, PB10);
    #define DEBUG_SERIAL Serial1
    #define DEBUG_SERIAL_BAUDRATE (uint32_t)115200
#endif

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo uservo(SERVO_ID, &protocol);

uint16_t interval;
uint16_t t_acc;
uint16_t t_dec;
float velocity;

void waitAndReport() {
    uservo.wait();
    DEBUG_SERIAL.println("Real Angle = " + String(uservo.curRawAngle, 1)
                       + " Target Angle = " + String(uservo.targetRawAngle, 1));
    delay(2000);
}

void setup() {
    protocol.init();
    uservo.init();
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
    DEBUG_SERIAL.println("Set Servo Angle");
}

void loop() {
    DEBUG_SERIAL.println("Set Angle = 90°");
    uservo.setRawAngle(90.0);
    waitAndReport();
    delay(2000);

    DEBUG_SERIAL.println("Set Angle = -90°");
    uservo.setRawAngle(-90);
    waitAndReport();
    delay(2000);

    DEBUG_SERIAL.println("Set Angle = 90° - Set Interval = 500ms");
    interval = 1000;
    t_acc = 100;
    t_dec = 100;
    uservo.setRawAngleByInterval(90, interval, t_acc, t_dec, 0);
    waitAndReport();
    delay(2000);

    DEBUG_SERIAL.println("Set Angle = -90° - Set Velocity = 200°/s");
    velocity = 200.0;
    t_acc = 100;
    t_dec = 100;
    uservo.setRawAngleByVelocity(-90, velocity, t_acc, t_dec, 0);
    waitAndReport();
    delay(2000);
}
```

**ログ出力**

```
Set Angle = 90°
Real Angle = 89.7 Target Angle = 90.0
Set Angle = -90°
Real Angle = -89.6 Target Angle = -90.0
Set Angle = 90° - Set Interval = 500ms
Real Angle = 89.7 Target Angle = 90.0
Set Angle = -90° - Set Velocity = 200°/s
Real Angle = -89.6 Target Angle = -90.0
```

---

## 10. サーボのブロッキング待機

### 10.1. API - `wait`

サーボが目標角度に到達するまでブロッキング待機します。

```cpp
void FSUS_Servo::wait()
```

**入力パラメータ**：なし

**出力パラメータ**：なし

### 10.2. サンプルコード

`servo_wait.ino`

```cpp
/*
 * wait() 関数のテスト - サーボが目標位置に到達するまで角度をポーリング
 * 注意: 拡張ボード通電後、Arduino の RESET ボタンを押してください
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2023/03/13
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define SERVO_ID 0
#define BAUDRATE 115200

#if defined(ARDUINO_AVR_UNO)
    #include <SoftwareSerial.h>
    #define SOFT_SERIAL_RX 6
    #define SOFT_SERIAL_TX 7
    SoftwareSerial softSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);
    #define DEBUG_SERIAL softSerial
    #define DEBUG_SERIAL_BAUDRATE 4800

#elif defined(ARDUINO_AVR_MEGA2560)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_ESP32)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_STM32)
    #include <HardwareSerial.h>
    HardwareSerial Serial1(PA10, PA9);
    HardwareSerial Serial3(PB11, PB10);
    #define DEBUG_SERIAL Serial1
    #define DEBUG_SERIAL_BAUDRATE (uint32_t)115200
#endif

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo uservo(SERVO_ID, &protocol);

void setup() {
    protocol.init();
    uservo.init();
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
    DEBUG_SERIAL.println("Test Wait");
}

void loop() {
    DEBUG_SERIAL.println("Set Angle = 90.0");
    uservo.setAngle(90.0);
    uservo.wait();
    DEBUG_SERIAL.println("Real Angle = " + String(uservo.curRawAngle, 2));

    DEBUG_SERIAL.println("Set Angle = -90.0");
    uservo.setAngle(-90);
    uservo.wait();
    DEBUG_SERIAL.println("Real Angle = " + String(uservo.curRawAngle, 2));
}
```

**ログ出力**

```
Set Angle = -90.0
Real Angle = -89.00
Set Angle = 90.0
Real Angle = 89.80
Set Angle = -90.0
Real Angle = -89.00
Set Angle = 90.0
Real Angle = 89.80
```

---

## 11. サーボ角度の設定 - マルチターンモード

### 11.1. API - `setRawAngleMTurn`

```cpp
// サーボの生の角度を設定（マルチターン）
void FSUS_Servo::setRawAngleMTurn(FSUS_SERVO_ANGLE_T rawAngle, FSUS_INTERVAL_T_MTURN interval, FSUS_POWER_T power)
void FSUS_Servo::setRawAngleMTurn(FSUS_SERVO_ANGLE_T rawAngle, FSUS_INTERVAL_T_MTURN interval)
void FSUS_Servo::setRawAngleMTurn(FSUS_SERVO_ANGLE_T rawAngle)
```

**入力パラメータ**

- `rawAngle`：目標角度（単位：°）
- `interval`：回転周期（単位：ms）
- `power`：最大出力（単位：mW）

### 11.2. API - `setRawAngleMTurnByInterval`

```cpp
// 周期を指定してサーボの生の角度を設定（マルチターン）
void FSUS_Servo::setRawAngleMTurnByInterval(FSUS_SERVO_ANGLE_T rawAngle, FSUS_INTERVAL_T_MTURN interval,
                                             FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power)
```

**入力パラメータ**

- `rawAngle`：目標角度（単位：°）
- `interval`：回転周期（単位：ms）
- `t_acc`：加速時間（単位：ms）
- `t_dec`：減速時間（単位：ms）
- `power`：最大出力（単位：mW）

### 11.3. API - `setRawAngleMTurnByVelocity`

```cpp
// 速度を指定してサーボの生の角度を設定（マルチターン）
void FSUS_Servo::setRawAngleMTurnByVelocity(FSUS_SERVO_ANGLE_T rawAngle, FSUS_SERVO_SPEED_T velocity,
                                             FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power)
```

**入力パラメータ**

- `rawAngle`：目標角度（単位：°）
- `velocity`：回転速度（単位：°/s）
- `t_acc`：加速時間（単位：ms）
- `t_dec`：減速時間（単位：ms）
- `power`：最大出力（単位：mW）

### 11.4. サンプルコード

`servo_set_angle_mturn.ino`

```cpp
/*
 * サーボの角度設定（マルチターンモード）
 * 注意: 拡張ボード通電後、Arduino の RESET ボタンを押してください
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2023/03/13
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define SERVO_ID 0
#define BAUDRATE 115200

#if defined(ARDUINO_AVR_UNO)
    #include <SoftwareSerial.h>
    #define SOFT_SERIAL_RX 6
    #define SOFT_SERIAL_TX 7
    SoftwareSerial softSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);
    #define DEBUG_SERIAL softSerial
    #define DEBUG_SERIAL_BAUDRATE 4800

#elif defined(ARDUINO_AVR_MEGA2560)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_ESP32)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_STM32)
    #include <HardwareSerial.h>
    HardwareSerial Serial1(PA10, PA9);
    HardwareSerial Serial3(PB11, PB10);
    #define DEBUG_SERIAL Serial1
    #define DEBUG_SERIAL_BAUDRATE (uint32_t)115200
#endif

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo uservo(SERVO_ID, &protocol);

uint32_t interval;
uint16_t t_acc;
uint16_t t_dec;
float velocity;

void waitAndReport() {
    uservo.wait();
    DEBUG_SERIAL.println("Real Angle = " + String(uservo.curRawAngle, 1)
                       + " Target Angle = " + String(uservo.targetRawAngle, 1));
    delay(2000);
}

void setup() {
    protocol.init();
    uservo.init();
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
    DEBUG_SERIAL.println("Set Servo Angle");
}

void loop() {
    DEBUG_SERIAL.println("Set Angle = 900°");
    uservo.setRawAngleMTurn(900.0);
    waitAndReport();

    DEBUG_SERIAL.println("Set Angle = -900.0°");
    uservo.setRawAngleMTurn(-900.0);
    waitAndReport();

    DEBUG_SERIAL.println("Set Angle = 900° - Set Interval = 10s");
    interval = 10000;
    t_acc = 100;
    t_dec = 100;
    uservo.setRawAngleMTurnByInterval(900, interval, t_acc, t_dec, 0);
    waitAndReport();

    DEBUG_SERIAL.println("Set Angle = -900° - Set Velocity = 200°/s");
    velocity = 200.0;
    t_acc = 100;
    t_dec = 100;
    uservo.setRawAngleMTurnByVelocity(-900, velocity, t_acc, t_dec, 0);
    waitAndReport();
}
```

**ログ出力**

```
Set Servo Angle
Set Angle = 900°
Real Angle = 899.0 Target Angle = 900.0
Set Angle = -900.0°
Real Angle = -899.0 Target Angle = -900.0
Set Angle = 900° - Set Interval = 10s
Real Angle = 899.0 Target Angle = 900.0
Set Angle = -900° - Set Velocity = 200°/s
Real Angle = -899.0 Target Angle = -900.0
```

---

## 12. サーボのトルクスイッチ

### 12.1. API - `setTorque`

```cpp
void FSUS_Servo::setTorque(bool enable)
```

**入力パラメータ**

- `enable`：トルクの有効/無効
  - `true`：トルクを有効にする
  - `false`：トルクを無効にする

**使用例**

```cpp
uservo.setTorque(true); // トルクを有効にする
```

### 12.2. サンプルコード

`servo_torque.ino`

```cpp
/*
 * サーボのトルクスイッチテスト
 * 注意: 拡張ボード通電後、Arduino の RESET ボタンを押してください
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2023/03/13
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define BAUDRATE 115200
#define SERVO_ID 0

FSUS_Protocol protocol;
FSUS_Servo uservo(SERVO_ID, &protocol);

void setup() {
    protocol.init();
    uservo.init();
    uservo.setTorque(true);   // トルクを有効にする
    // uservo.setTorque(false); // トルクを無効にする
}

void loop() {
}
```

---

## 13. サーボのキャリブレーション

### 13.1. API - `calibration`

`FSUS_Servo` クラスにはキャリブレーション関連のパラメータが2つあります：

```cpp
class FSUS_Servo {
public:
    float kAngleReal2Raw; // 角度と位置の比例係数
    float bAngleReal2Raw; // 角度と位置の変換オフセット
};
```

実際の角度と生の角度の対応関係：

```
angleRaw = kAngleReal2Raw * angleReal + bAngleReal2Raw
```

**関数プロトタイプ**

```cpp
void FSUS_Servo::calibration(FSUS_SERVO_ANGLE_T rawA, FSUS_SERVO_ANGLE_T realA,
                              FSUS_SERVO_ANGLE_T rawB, FSUS_SERVO_ANGLE_T realB)
```

**入力パラメータ**

- `rawA`：位置Aでのサーボ生の角度
- `realA`：位置Aでのサーボ実際の角度
- `rawB`：位置Bでのサーボ生の角度
- `realB`：位置Bでのサーボ実際の角度

**使用例**

```cpp
// キャリブレーションポイントの設定
#define SERVO_REAL_ANGLE_A 90    // 実際の角度
#define SERVO_RAW_ANGLE_A -86.2  // 生の角度
#define SERVO_REAL_ANGLE_B -90   // 実際の角度
#define SERVO_RAW_ANGLE_B 91.9   // 生の角度

uservo.calibration(
    SERVO_RAW_ANGLE_A, SERVO_REAL_ANGLE_A,
    SERVO_RAW_ANGLE_B, SERVO_REAL_ANGLE_B);
```

係数を直接指定するオーバーロードもあります：

```cpp
void FSUS_Servo::calibration(float kAngleReal2Raw, float bAngleReal2Raw);
```

### 13.2. API - `angleReal2Raw`

実際の角度を生の角度に変換します。

```cpp
FSUS_SERVO_ANGLE_T FSUS_Servo::angleReal2Raw(FSUS_SERVO_ANGLE_T realAngle);
```

### 13.3. API - `angleRaw2Real`

生の角度を実際の角度に変換します。

```cpp
FSUS_SERVO_ANGLE_T FSUS_Servo::angleRaw2Real(FSUS_SERVO_ANGLE_T rawAngle);
```

### 13.4. サンプルコード

`servo_calibration.ino`

```cpp
/*
 * サーボのキャリブレーションテスト
 * 注意: 拡張ボード通電後、Arduino の RESET ボタンを押してください
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2023/03/13
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define SERVO_ID 0
#define BAUDRATE 115200

#define SERVO_REAL_ANGLE_A 90
#define SERVO_RAW_ANGLE_A -86.2
#define SERVO_REAL_ANGLE_B -90
#define SERVO_RAW_ANGLE_B 91.9

#if defined(ARDUINO_AVR_UNO)
    #include <SoftwareSerial.h>
    #define SOFT_SERIAL_RX 6
    #define SOFT_SERIAL_TX 7
    SoftwareSerial softSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);
    #define DEBUG_SERIAL softSerial
    #define DEBUG_SERIAL_BAUDRATE 4800

#elif defined(ARDUINO_AVR_MEGA2560)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_ESP32)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_STM32)
    #include <HardwareSerial.h>
    HardwareSerial Serial1(PA10, PA9);
    HardwareSerial Serial3(PB11, PB10);
    #define DEBUG_SERIAL Serial1
    #define DEBUG_SERIAL_BAUDRATE (uint32_t)115200
#endif

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo uservo(SERVO_ID, &protocol);

void setup() {
    protocol.init();
    uservo.init();
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
    DEBUG_SERIAL.println("Set Servo Angle");
    uservo.calibration(
        SERVO_RAW_ANGLE_A, SERVO_REAL_ANGLE_A,
        SERVO_RAW_ANGLE_B, SERVO_REAL_ANGLE_B);
    DEBUG_SERIAL.println("kAngleReal2Raw = " + String(uservo.kAngleReal2Raw, 2)
                       + "; bAngleReal2Raw = " + String(uservo.bAngleReal2Raw, 2));
}

void loop() {
    DEBUG_SERIAL.println("Set Angle = 90°");
    uservo.setAngle(90.0);
    uservo.wait();
    delay(2000);

    DEBUG_SERIAL.println("Set Angle = -90°");
    uservo.setAngle(-90);
    uservo.wait();
    delay(2000);
}
```

**ログ出力**

```
Set Servo Angle

kAngleReal2Raw = -0.99; bAngleReal2Raw = 2.85

Set Angle = 90
Set Angle = -90
```

---

## 14. サーボ速度の設定

### 14.1. API - `setSpeed`

```cpp
void FSUS_Servo::setSpeed(FSUS_SERVO_SPEED_T speed)
```

**入力パラメータ**

- `speed`：サーボの平均回転速度（単位：°/s）

---

## 15. サーボデータの読み取り

### 15.1. API

```cpp
uint16_t FSUS_Servo::queryVoltage();     // 電圧を取得（単位：mV）
uint16_t FSUS_Servo::queryCurrent();     // 電流を取得（単位：mA）
uint16_t FSUS_Servo::queryPower();       // 電力を取得（単位：mW）
uint16_t FSUS_Servo::queryTemperature(); // 温度を取得（単位：ADC）
uint8_t  FSUS_Servo::queryStatus();      // 動作状態を取得
```

### 15.2. サンプルコード

`servo_data_read.ino`

```cpp
/*
 * サーボデータ読み取り実験
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2024/08/14
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define SERVO_ID 0
#define BAUDRATE 115200

#if defined(ARDUINO_AVR_UNO)
    #include <SoftwareSerial.h>
    #define SOFT_SERIAL_RX 6
    #define SOFT_SERIAL_TX 7
    SoftwareSerial softSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);
    #define DEBUG_SERIAL softSerial
    #define DEBUG_SERIAL_BAUDRATE 4800

#elif defined(ARDUINO_AVR_MEGA2560)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_ESP32)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_STM32)
    #include <HardwareSerial.h>
    HardwareSerial Serial1(PA10, PA9);
    HardwareSerial Serial3(PB11, PB10);
    #define DEBUG_SERIAL Serial1
    #define DEBUG_SERIAL_BAUDRATE (uint32_t)115200
#endif

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo uservo(SERVO_ID, &protocol);

uint16_t voltage;
uint16_t current;
uint16_t power;
uint16_t temperature;
uint8_t status;

void setup() {
    protocol.init();
    uservo.init();
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
    DEBUG_SERIAL.println("Start To Test Servo Data Read \n");
}

void loop() {
    voltage = uservo.queryVoltage();
    DEBUG_SERIAL.println("voltage: " + String((float)voltage, 1) + " mV");
    delay(100);

    current = uservo.queryCurrent();
    DEBUG_SERIAL.println("current: " + String((float)current, 1) + " mA");
    delay(100);

    power = uservo.queryPower();
    DEBUG_SERIAL.println("power: " + String((float)power, 1) + " mW");
    delay(100);

    temperature = uservo.queryTemperature();
    temperature = 1 / (log(temperature / (4096.0f - temperature)) / 3435.0f + 1 / (273.15 + 25)) - 273.15;
    DEBUG_SERIAL.println("temperature: " + String((float)temperature, 1) + " Celsius");

    /*
     * 動作状態ビットの意味：
     * BIT[0] - 命令実行中に1、完了後に0
     * BIT[1] - 命令実行エラー時に1、次の正常実行後に0
     * BIT[2] - ロック時に1、解除後に0
     * BIT[3] - 過電圧時に1、正常復帰後に0
     * BIT[4] - 低電圧時に1、正常復帰後に0
     * BIT[5] - 電流エラー時に1、正常復帰後に0
     * BIT[6] - 電力エラー時に1、正常復帰後に0
     * BIT[7] - 温度エラー時に1、正常復帰後に0
     */
    status = uservo.queryStatus();
    char binStr[9];
    for (int i = 7; i >= 0; i--) {
        binStr[7 - i] = (status & (1 << i)) ? '1' : '0';
    }
    binStr[8] = '\0';
    DEBUG_SERIAL.print("WorkState: ");
    DEBUG_SERIAL.println(binStr);

    if (bitRead(status, 3)) {
        DEBUG_SERIAL.println("voltage_high");
    }
    if (bitRead(status, 4)) {
        DEBUG_SERIAL.println("voltage_low");
    }
    delay(1000);
}
```

---

## 16. 原点の設定

> **注意事項**
> - ブラシレス磁気エンコーダーサーボ専用
> - アンロック状態でのみ使用可能

### 16.1. API - `SetOriginPoint`

```cpp
void FSUS_Servo::SetOriginPoint();
```

**入力パラメータ**：なし

**返り値**：なし

### 16.2. サンプルコード

`servo_set_origin_point.ino`

```cpp
/*
 * サーボの原点設定
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2024/08/14
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define SERVO_ID 0
#define BAUDRATE 115200

#if defined(ARDUINO_AVR_UNO)
    #include <SoftwareSerial.h>
    #define SOFT_SERIAL_RX 6
    #define SOFT_SERIAL_TX 7
    SoftwareSerial softSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);
    #define DEBUG_SERIAL softSerial
    #define DEBUG_SERIAL_BAUDRATE 4800

#elif defined(ARDUINO_AVR_MEGA2560)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_ESP32)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_STM32)
    #include <HardwareSerial.h>
    HardwareSerial Serial1(PA10, PA9);
    HardwareSerial Serial3(PB11, PB10);
    #define DEBUG_SERIAL Serial1
    #define DEBUG_SERIAL_BAUDRATE (uint32_t)115200
#endif

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo uservo(SERVO_ID, &protocol);

void setup() {
    protocol.init();
    uservo.init();
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);

    uservo.setTorque(0);
    uservo.queryRawAngle();
    DEBUG_SERIAL.println("Before Set Origin Point: Servo Angle: " + String(uservo.curRawAngle, 1) + " deg");
    uservo.SetOriginPoint();
    delay(1000);
    uservo.queryRawAngle();
    DEBUG_SERIAL.println("After Set Origin Point: Servo Angle: " + String(uservo.curRawAngle, 1) + " deg");
}

void loop() {
    uservo.queryRawAngle();
    String message = "Status Code: " + String(uservo.protocol->responsePack.recv_status, DEC)
                   + " servo #" + String(uservo.servoId, DEC)
                   + " , Current Angle = " + String(uservo.curRawAngle, 1) + " deg";
    DEBUG_SERIAL.println(message);
    delay(1000);
}
```

---

## 17. マルチターン角度のリセット

> **注意事項**
> - 磁気エンコーダーサーボ専用
> - アンロック状態でのみ使用可能

### 17.1. API - `ResetMultiTurnAngle`

```cpp
void FSUS_Servo::ResetMultiTurnAngle();
```

**入力パラメータ**：なし

**返り値**：なし

### 17.2. サンプルコード

`servo_reset_multiturn_angle.ino`

```cpp
/*
 * サーボのマルチターン角度リセット
 * 注意: 拡張ボード通電後、Arduino の RESET ボタンを押してください
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2024/12/17
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define SERVO_ID 0
#define BAUDRATE 115200

#if defined(ARDUINO_AVR_UNO)
    #include <SoftwareSerial.h>
    #define SOFT_SERIAL_RX 6
    #define SOFT_SERIAL_TX 7
    SoftwareSerial softSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);
    #define DEBUG_SERIAL softSerial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_AVR_MEGA2560)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_ESP32)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_STM32)
    #include <HardwareSerial.h>
    HardwareSerial Serial1(PA10, PA9);
    HardwareSerial Serial3(PB11, PB10);
    #define DEBUG_SERIAL Serial1
    #define DEBUG_SERIAL_BAUDRATE (uint32_t)115200
#endif

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo uservo(SERVO_ID, &protocol);

uint32_t interval;
uint16_t t_acc;
uint16_t t_dec;
float velocity;

void setup() {
    protocol.init();
    uservo.init();
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
    DEBUG_SERIAL.println("Set Servo Angle");
}

void loop() {
    uservo.setRawAngleMTurn(1000.0);
    delay(7000);
    uservo.queryRawAngleMTurn();
    String message = "Status Code: " + String(uservo.protocol->responsePack.recv_status, DEC)
                   + " servo #" + String(uservo.servoId, DEC)
                   + " , Current Angle = " + String(uservo.curRawAngle, 1) + " deg";
    DEBUG_SERIAL.println(message);
    delay(1000);
    uservo.StopOnControlUnloading(); // 停止（マルチターンリセット前に停止が必要）
    delay(10);
    uservo.ResetMultiTurnAngle();    // マルチターンリセット
    delay(10);
    uservo.queryRawAngleMTurn();
    message = "Status Code: " + String(uservo.protocol->responsePack.recv_status, DEC)
            + " servo #" + String(uservo.servoId, DEC)
            + " , Current Angle = " + String(uservo.curRawAngle, 1) + " deg";
    DEBUG_SERIAL.println(message);
    delay(1000);

    uservo.setRawAngleMTurn(-1000.0);
    delay(7000);
    uservo.queryRawAngleMTurn();
    message = "Status Code: " + String(uservo.protocol->responsePack.recv_status, DEC)
            + " servo #" + String(uservo.servoId, DEC)
            + " , Current Angle = " + String(uservo.curRawAngle, 1) + " deg";
    DEBUG_SERIAL.println(message);
    delay(1000);
    uservo.StopOnControlUnloading();
    delay(10);
    uservo.ResetMultiTurnAngle();
    delay(10);
    uservo.queryRawAngleMTurn();
    message = "Status Code: " + String(uservo.protocol->responsePack.recv_status, DEC)
            + " servo #" + String(uservo.servoId, DEC)
            + " , Current Angle = " + String(uservo.curRawAngle, 1) + " deg";
    DEBUG_SERIAL.println(message);
    delay(1000);
}
```

---

## 18. 非同期コマンド

> **注意事項**
> - ブラシレス磁気エンコーダーサーボ V316 以降のバージョン専用

### 18.1. API

```cpp
void FSUS_Servo::BeginAsync();             // 非同期コマンドの開始
void FSUS_Servo::EndAsync(uint8_t cancel); // 非同期コマンドの終了
```

**入力パラメータ**

- `cancel`：`0` で実行、`1` でキャンセル

バッファに蓄積できるコマンド：

- 生の角度設定
- 周期指定の生の角度設定
- 速度指定の生の角度設定
- マルチターンの生の角度設定
- マルチターン+周期指定の生の角度設定
- マルチターン+速度指定の生の角度設定

### 18.2. サンプルコード

`servo_async.ino`

```cpp
/*
 * サーボの非同期コマンド
 * 注意: 拡張ボード通電後、Arduino の RESET ボタンを押してください
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2024/12/17
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define SERVO_ID 3
#define BAUDRATE 115200

#if defined(ARDUINO_AVR_UNO)
    #include <SoftwareSerial.h>
    #define SOFT_SERIAL_RX 6
    #define SOFT_SERIAL_TX 7
    SoftwareSerial softSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);
    #define DEBUG_SERIAL softSerial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_AVR_MEGA2560)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_ESP32)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_STM32)
    #include <HardwareSerial.h>
    HardwareSerial Serial1(PA10, PA9);
    HardwareSerial Serial3(PB11, PB10);
    #define DEBUG_SERIAL Serial1
    #define DEBUG_SERIAL_BAUDRATE (uint32_t)115200
#endif

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo uservo(SERVO_ID, &protocol);

void waitAndReport() {
    uservo.wait();
    DEBUG_SERIAL.println("Real Angle = " + String(uservo.curRawAngle, 1)
                       + " Target Angle = " + String(uservo.targetRawAngle, 1));
    delay(2000);
}

void setup() {
    protocol.init();
    uservo.init();
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
    DEBUG_SERIAL.println("Set Servo Angle");
}

void loop() {
    uservo.setRawAngle(0.0);
    uservo.BeginAsync();        // 非同期コマンド開始
    delay(1000);
    uservo.setRawAngle(90.0);  // コマンドをバッファに蓄積
    delay(1000);
    uservo.EndAsync(0);         // 非同期終了・蓄積コマンドを実行
    delay(1000);

    uservo.BeginAsync();
    delay(1000);
    uservo.setRawAngle(180.0);
    delay(1000);
    uservo.EndAsync(1);         // 非同期終了・蓄積コマンドをキャンセル
    delay(1000);
}
```

---

## 19. データモニタリング

> **注意事項**
> - ブラシレス磁気エンコーダーサーボ V316 以降のバージョン専用

### 19.1. API

```cpp
ServoMonitorData FSUS_Servo::ServoMonitor();
```

**入力パラメータ**：なし

**返り値**：サーボの各種データ（`ServoMonitorData` 構造体）

### 19.2. サンプルコード

`servo_monitor.ino`

```cpp
/*
 * サーボのデータモニタリング
 * 注意: 拡張ボード通電後、Arduino の RESET ボタンを押してください
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2024/12/17
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define SERVO_ID 0
#define BAUDRATE 115200

#if defined(ARDUINO_AVR_UNO)
    #include <SoftwareSerial.h>
    #define SOFT_SERIAL_RX 6
    #define SOFT_SERIAL_TX 7
    SoftwareSerial softSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);
    #define DEBUG_SERIAL softSerial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_AVR_MEGA2560)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_ESP32)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_STM32)
    #include <HardwareSerial.h>
    HardwareSerial Serial1(PA10, PA9);
    HardwareSerial Serial3(PB11, PB10);
    #define DEBUG_SERIAL Serial1
    #define DEBUG_SERIAL_BAUDRATE (uint32_t)115200
#endif

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo uservo(SERVO_ID, &protocol);

void setup() {
    protocol.init();
    uservo.init();
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
    DEBUG_SERIAL.println("Servo Monitor");
}

void loop() {
    ServoMonitorData data = uservo.ServoMonitor();

    if (data.isValid) {
        DEBUG_SERIAL.println("Servo Monitor Data (Valid):");
        DEBUG_SERIAL.print("Servo ID: ");     DEBUG_SERIAL.println(data.servoId);
        DEBUG_SERIAL.print("Voltage: ");      DEBUG_SERIAL.println(data.voltage);
        DEBUG_SERIAL.print("Current: ");      DEBUG_SERIAL.println(data.current);
        DEBUG_SERIAL.print("Power: ");        DEBUG_SERIAL.println(data.power);
        DEBUG_SERIAL.print("Temperature: "); DEBUG_SERIAL.println(data.temperature);
        DEBUG_SERIAL.print("Status: ");       DEBUG_SERIAL.println(data.status);
        DEBUG_SERIAL.print("Angle: ");        DEBUG_SERIAL.println(data.angle);
        DEBUG_SERIAL.print("Turns: ");        DEBUG_SERIAL.println(data.turns);
        delay(2000);
    } else {
        DEBUG_SERIAL.println("Failed to receive valid servo data.");
        delay(2000);
    }
}
```

**ログ出力**

```
Servo Monitor Data (Valid):
Servo ID: 0
Voltage: 12051.00
Current: 30.00
Power: 361.00
Temperature: 2015.00
Status: 0
Angle: 4999.90
Turns: 13.00
```

---

## 20. 制御モード停止コマンド

> **注意事項**
> - ブラシレス磁気エンコーダーサーボ V316 以降のバージョン専用

### 20.1. API

```cpp
void StopOnControlMode(uint8_t method, uint16_t power); // 制御モード停止
void StopOnControlUnloading();                          // 停止後アンロック（脱力）
void StopOnControlKeep(uint16_t power);                 // 停止後ロック保持
void StopOnControlDammping(uint16_t power);             // 停止後ダンピング状態
```

**入力パラメータ**

- `method`：停止方式
  - `0x10`：停止後アンロック（脱力）
  - `0x11`：停止後ロック保持
  - `0x12`：停止後ダンピング状態
- `power`：トルク設定後の出力（単位：mW）。0 または保護値より大きい場合は保護値で動作。

### 20.2. サンプルコード

`servo_stop.ino`

```cpp
/*
 * サーボ角度設定（マルチターン）- 停止コマンド
 * 注意: 拡張ボード通電後、Arduino の RESET ボタンを押してください
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2024/12/17
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define SERVO_ID 1
#define BAUDRATE 115200

#if defined(ARDUINO_AVR_UNO)
    #include <SoftwareSerial.h>
    #define SOFT_SERIAL_RX 6
    #define SOFT_SERIAL_TX 7
    SoftwareSerial softSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);
    #define DEBUG_SERIAL softSerial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_AVR_MEGA2560)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_ESP32)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_STM32)
    #include <HardwareSerial.h>
    HardwareSerial Serial1(PA10, PA9);
    HardwareSerial Serial3(PB11, PB10);
    #define DEBUG_SERIAL Serial1
    #define DEBUG_SERIAL_BAUDRATE (uint32_t)115200
#endif

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo uservo(SERVO_ID, &protocol);

uint32_t interval;
uint16_t t_acc;
uint16_t t_dec;
float velocity;

void setup() {
    protocol.init();
    uservo.init();
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
    DEBUG_SERIAL.println("Set Servo Angle");
}

void loop() {
    DEBUG_SERIAL.println("Set Angle = 10000°, StopOnControlUnloading\r\n");
    interval = 500; t_acc = 100; t_dec = 100;
    uservo.setRawAngleMTurnByInterval(10000, interval, t_acc, t_dec, 0);
    delay(2000);
    uservo.StopOnControlUnloading(); // 停止・アンロック（手で動かせる）
    delay(5000);

    DEBUG_SERIAL.println("Set Angle = -10000°, StopOnControlKeep\r\n");
    interval = 500; t_acc = 100; t_dec = 100;
    uservo.setRawAngleMTurnByInterval(-10000, interval, t_acc, t_dec, 0);
    delay(2000);
    uservo.StopOnControlKeep(100);   // 停止・ロック保持（手で動かせない）
    delay(5000);

    DEBUG_SERIAL.println("Set Angle = 10000°, StopOnControlDammping\r\n");
    interval = 500; t_acc = 100; t_dec = 100;
    uservo.setRawAngleMTurnByInterval(10000, interval, t_acc, t_dec, 0);
    delay(2000);
    uservo.StopOnControlDammping(500); // 停止・ダンピング（ダンピング感あり、手で動かせる）
    delay(5000);

    DEBUG_SERIAL.println("Set Angle = -10000°, StopOnControlKeep\r\n");
    interval = 500; t_acc = 100; t_dec = 100;
    uservo.setRawAngleMTurnByInterval(-10000, interval, t_acc, t_dec, 0);
    delay(6000);
}
```

---

## 21. 同期制御 / 同期データモニタリング

> **注意事項**
> - ブラシレス磁気エンコーダーサーボ V316 以降のバージョン専用
> - Arduino UNO の RAM は 2KB のみのため、同期制御は最大 12 サーボまでサポート

### 21.1. API

```cpp
// 同期制御
void FSUS_Servo::SyncCommand(uint8_t servocount, uint8_t syncmode, FSUS_sync_servo servoSync[]);

// 同期データモニタリング
void FSUS_Servo::SyncMonitorCommand(uint8_t servocount, FSUS_sync_servo servoSync[], ServoMonitorData* data);
```

**同期制御モード（`syncmode`）**

| モード | 内容 |
|--------|------|
| 1 | シングルターン角度制御 |
| 2 | シングルターン角度 - 周期指定 |
| 3 | シングルターン角度 - 速度指定 |
| 4 | マルチターン角度制御 |
| 5 | マルチターン角度 - 周期指定 |
| 6 | マルチターン角度 - 速度指定 |

### 21.2. サンプルコード

`servo_sync_command.ino`

```cpp
/*
 * 同期制御 / 同期モニタリング
 * 注意: 拡張ボード通電後、Arduino の RESET ボタンを押してください
 * --------------------------
 * 作者: 深圳市華馨京科技有限公司
 * ウェブサイト: https://fashionrobo.com/
 * 更新日: 2024/12/30
 */
#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h"

#define SERVO_ID 0
#define BAUDRATE 115200

#if defined(ARDUINO_AVR_UNO)
    #include <SoftwareSerial.h>
    #define SOFT_SERIAL_RX 6
    #define SOFT_SERIAL_TX 7
    SoftwareSerial softSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);
    #define DEBUG_SERIAL softSerial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_AVR_MEGA2560)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_ESP32)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200

#elif defined(ARDUINO_ARCH_STM32)
    #include <HardwareSerial.h>
    HardwareSerial Serial1(PA10, PA9);
    HardwareSerial Serial3(PB11, PB10);
    #define DEBUG_SERIAL Serial1
    #define DEBUG_SERIAL_BAUDRATE (uint32_t)115200
#endif

FSUS_Protocol protocol(BAUDRATE);
FSUS_Servo uservo(SERVO_ID, &protocol);

FSUS_sync_servo servoSyncArray[18];

void setup() {
    protocol.init();
    uservo.init();
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
}

int mode;
int count;
ServoMonitorData servodata[1];

void loop() {
    mode = 1;
    count = 4;

    for (int i = 0; i < count; i++) {
        servoSyncArray[i].servoId            = i;
        servoSyncArray[i].angle              = 90;
        servoSyncArray[i].interval           = 1000;
        servoSyncArray[i].interval_multiturn = 1000;
        servoSyncArray[i].velocity           = 360;
        servoSyncArray[i].t_acc              = 100;
        servoSyncArray[i].t_dec              = 100;
        servoSyncArray[i].power              = 0;
    }
    uservo.SyncCommand(count, mode, servoSyncArray);
    delay(2000);

    uservo.SyncMonitorCommand(count, servoSyncArray, servodata);
    delay(2000);
    for (int i = 0; i < count; i++) {
        DEBUG_SERIAL.print("id: ");          DEBUG_SERIAL.println(servodata[i].servoId);
        DEBUG_SERIAL.print("voltage: ");     DEBUG_SERIAL.println(servodata[i].voltage);
        DEBUG_SERIAL.print("current: ");     DEBUG_SERIAL.println(servodata[i].current);
        DEBUG_SERIAL.print("power: ");       DEBUG_SERIAL.println(servodata[i].power);
        DEBUG_SERIAL.print("temperature: "); DEBUG_SERIAL.println(servodata[i].temperature);
        DEBUG_SERIAL.print("status: ");      DEBUG_SERIAL.println(servodata[i].status);
        DEBUG_SERIAL.print("angle: ");       DEBUG_SERIAL.println(servodata[i].angle);
        DEBUG_SERIAL.print("turns: ");       DEBUG_SERIAL.println(servodata[i].turns);
        delay(1000);
    }
    delay(2000);

    mode = 1;
    count = 4;
    for (int i = 0; i < count; i++) {
        servoSyncArray[i].servoId            = i;
        servoSyncArray[i].angle              = 0;
        servoSyncArray[i].interval           = 1000;
        servoSyncArray[i].interval_multiturn = 1000;
        servoSyncArray[i].velocity           = 360;
        servoSyncArray[i].t_acc              = 100;
        servoSyncArray[i].t_dec              = 100;
        servoSyncArray[i].power              = 0;
    }
    uservo.SyncCommand(count, mode, servoSyncArray);
    delay(2000);

    uservo.SyncMonitorCommand(count, servoSyncArray, servodata);
    delay(2000);
    for (int i = 0; i < count; i++) {
        DEBUG_SERIAL.print("id: ");          DEBUG_SERIAL.println(servodata[i].servoId);
        DEBUG_SERIAL.print("voltage: ");     DEBUG_SERIAL.println(servodata[i].voltage);
        DEBUG_SERIAL.print("current: ");     DEBUG_SERIAL.println(servodata[i].current);
        DEBUG_SERIAL.print("power: ");       DEBUG_SERIAL.println(servodata[i].power);
        DEBUG_SERIAL.print("temperature: "); DEBUG_SERIAL.println(servodata[i].temperature);
        DEBUG_SERIAL.print("status: ");      DEBUG_SERIAL.println(servodata[i].status);
        DEBUG_SERIAL.print("angle: ");       DEBUG_SERIAL.println(servodata[i].angle);
        DEBUG_SERIAL.print("turns: ");       DEBUG_SERIAL.println(servodata[i].turns);
        delay(1000);
    }
    delay(2000);
}
```

**ログ出力**

```
id: 0
voltage: 11427.00
current: 30.00
power: 342.00
temperature: 2035.00
status: 0
angle: 0.10
turns: 0.00

id: 1
voltage: 11654.00
current: 30.00
power: 349.00
temperature: 2053.00
status: 0
angle: 0.10
turns: 0.00

id: 2
voltage: 11553.00
current: 30.00
power: 346.00
temperature: 2108.00
status: 0
angle: 0.10
turns: 0.00

id: 3
voltage: 11543.00
current: 30.00
power: 346.00
temperature: 2041.00
status: 0
angle: 0.10
turns: 0.00
```
