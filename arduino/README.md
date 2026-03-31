/**
 * @file FashionStar_UartServo_Reference.h
 * @brief FashionStar 総線伺服舵機 SDK 日本語リファレンス (V316対応)
 * @author 齋藤宗矩 (九州工業大学) / Gemini
 * @date 2026-03-31
 * * 【注意】このファイルは実装用ではなく、SDKの仕様をAIや人間に伝えるための辞書です。
 */

/* =========================================================================
 * 1. 基本的なクラス構造と初期化
 * ========================================================================= */

// 通信プロトコル層 (低層)
// FSUS_Protocol protocol(BAUDRATE); 
// protocol.init(); // 必須

// 舵機制御層 (高層)
// FSUS_Servo servo(ID, &protocol);
// servo.init(); // 必須

/* =========================================================================
 * 2. 角度制御 API (書き込み)
 * ========================================================================= */

/**
 * @brief 角度指定 (単圏モード: -180.0 ~ 180.0)
 * @param rawAngle ターゲット角度 (deg)
 * @param interval 移動にかける時間 (ms) : 0指定で最速
 * @param power 最大トルク/パワー (mW) : 0指定で保護値を使用
 */
void setRawAngle(float rawAngle, uint16_t interval = 0, uint16_t power = 0);

/**
 * @brief 速度指定で動かす
 * @param velocity 回転速度 (deg/s)
 * @param t_acc 加速時間 (ms)
 * @param t_dec 減速時間 (ms)
 */
void setRawAngleByVelocity(float rawAngle, float velocity, uint16_t t_acc, uint16_t t_dec, uint16_t power);

/**
 * @brief 多圏モード (マルチターン)
 * 360度を超える回転が必要な場合（首の回転リミット解除など）に使用
 */
void setRawAngleMTurn(float rawAngle, uint32_t interval = 0, uint16_t power = 0);

/* =========================================================================
 * 3. データ読み取り API (受信)
 * ========================================================================= */

/**
 * @brief 現在角度の問い合わせ
 * 結果は servo.curRawAngle (float) に格納される
 * 失敗時は -99.0 や nan が返ることがある
 */
float queryRawAngle();

/**
 * @brief 舵機の状態一括モニター (V316以降推奨)
 * 1回のパケットで「電圧・電流・電力・温度・角度・圏数」を取得
 * 戻り値: ServoMonitorData 構造体 (isValidフラグで成否判定)
 */
ServoMonitorData ServoMonitor();

/**
 * @brief 舵機データ個別問い合わせ
 */
uint16_t queryVoltage();     // 電圧 (mV)
uint16_t queryCurrent();     // 電流 (mA)
uint16_t queryPower();       // 電力 (mW)
uint8_t  queryStatus();      // 状態フラグ (8bit)
uint16_t queryTemperature(); // 温度 (ADC値: 摂氏変換が必要)

/* =========================================================================
 * 4. モード切替・特殊制御
 * ========================================================================= */

/**
 * @brief 阻尼 (ダンピング) モード
 * ティーチング時に使用。手で動かせるが、指定した電力分だけ抵抗がかかる
 * @param power 抵抗力 (mW) : 500〜1000程度が一般的
 */
void setDamping(uint16_t power);

/**
 * @brief 扭力 (トルク) スイッチ
 * falseにすると完全に脱力（失鎖）し、手で自由に動かせる
 */
void setTorque(bool enable);

/**
 * @brief 停止命令 (V316以降)
 * StopOnControlUnloading(); // 停止して脱力
 * StopOnControlKeep(power);  // 停止してその場を維持 (鎖止)
 * StopOnControlDammping(p);  // 停止して阻尼モードへ
 */

/* =========================================================================
 * 5. エラーフラグ (queryStatusのBIT定義)
 * ========================================================================= */
/*
 * BIT[0] : 指令実行中 (1:実行中 / 0:完了)
 * BIT[1] : 指令エラー
 * BIT[2] : 堵転 (スタック/過負荷) 
 * BIT[3] : 電圧過高 (Over Voltage)
 * BIT[4] : 電圧過低 (Low Voltage)
 * BIT[5] : 電流異常
 * BIT[6] : パワー異常
 * BIT[7] : 温度異常 (Over Heat)
 */

/* =========================================================================
 * 6. 同期制御 (17軸同時動作用)
 * ========================================================================= */
/**
 * @brief 複数舵機の同時制御
 * @param servocount 制御する舵機の数
 * @param syncmode モード (1:単圏角度, 4:多圏角度など)
 * @param servoSync データの配列
 */
void SyncCommand(uint8_t servocount, uint8_t syncmode, FSUS_sync_servo servoSync[]);
