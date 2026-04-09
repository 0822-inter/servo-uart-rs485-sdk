# servo-uart-rs485-sdk

> **UART / RS485 Bus Servo SDK** — Multi-platform SDK collection for Fashion Star (华馨京科技) UART & RS485 series servo motors.

---

## 📖 English

### Overview

This repository provides SDK examples and sample code for controlling **Fashion Star (Huaxinjing Technology) UART & RS485 series servo motors** across a wide range of platforms and programming languages.

The servos communicate over UART or RS485 serial bus, enabling precise position control, speed control, and status feedback from multiple servos on a single bus.

For the full communication protocol specification and additional documentation, visit the official wiki:
👉 [Fashion Star Wiki](https://wiki.fashionrobo.com/)

---

### Repository Structure

| Folder | Description |
|---|---|
| `PC-Software` | PC host software tools: **Develop-US** (GUI control panel) and **Serial Port Debugger** (serial monitor) |
| `STM32F103` | STM32F103 SDK — supports both HAL library and Standard Peripheral Library |
| `STM32F407` | STM32F407 SDK — supports both HAL library and Standard Peripheral Library |
| `C++` | C++ SDK for desktop/embedded Linux use |
| `C#` | C# SDK for Windows/.NET applications |
| `python` | Python SDK for desktop/Raspberry Pi use |
| `micro-python` | MicroPython SDK for microcontrollers (e.g. ESP32, Raspberry Pi Pico) |
| `arduino` | Arduino SDK (compatible with Uno, Mega 2560, ESP32, etc.) |
| `ros2` | ROS 2 Python SDK for robotics middleware integration |

---

### Supported Platforms

| Platform | Language |
|---|---|
| STM32F103 | C (HAL / Standard Library) |
| STM32F407 | C (HAL / Standard Library) |
| Arduino (Uno, Mega, ESP32, etc.) | C++ / Arduino |
| Desktop / Linux / Raspberry Pi | Python, C++, C# |
| Microcontrollers (ESP32, Pico) | MicroPython |
| ROS 2 | Python |

---

### Quick Start

1. **Choose your platform folder** from the table above.
2. **Read the SDK documentation** for that platform on the [Fashion Star Wiki](https://wiki.fashionrobo.com/).
3. **Connect your servo** via UART or RS485 to your host device.
4. **Open the sample project** in your IDE and flash/run it.
5. **Use the PC Software** in `PC-Software/` to debug and configure servo IDs, zero positions, and parameters.

---

### Communication Protocol

This SDK targets Fashion Star's proprietary **UART / RS485 serial bus protocol**.

Key features of the protocol:
- Half-duplex serial communication (UART or RS485)
- Multi-servo addressing via servo ID
- Commands for: angle control, speed control, torque enable/disable, status query, parameter read/write
- Checksum-based packet validation

Full protocol documentation: [UART/RS485 Protocol Reference](https://wiki.fashionstar.com.hk/zh/servo/uart/protocols/uart-rs485-protocol/)

---

### Language Breakdown

| Language | Share |
|---|---|
| C | 94.7% |
| C++ | 2.3% |
| Assembly | 1.3% |
| C# | 1.2% |
| Python | 0.4% |
| CMake | 0.1% |

---

### Changelog

| Date | Notes |
|---|---|
| 2025/07/02 | Repository created, synced with official release |
| 2025/07/04 | Documentation synced |
| 2025/07/15 | ROS2 SDK added |
| 2025/07/22 | PC host software updated |
| 2025/09/11 | Command calculation spreadsheet updated |

---

### Related Links

- 🌐 [Official Wiki (Fashion Star)](https://wiki.fashionrobo.com/)
- 📦 [SDK Overview](https://wiki.fashionstar.com.hk/zh/sdk/)
- 📄 [UART Servo Datasheet](https://wiki.fashionstar.com.hk/zh/servo/uart/datasheet/)
- 🔌 [UART/RS485 Protocol Spec](https://wiki.fashionstar.com.hk/zh/servo/uart/protocols/uart-rs485-protocol/)
- 🖥️ [PC Configuration Software Manual](https://wiki.fashionstar.com.hk/zh/software/servo/pc-config-software/)
- 🤖 [Original Repository (servodevelop)](https://github.com/servodevelop/servo-uart-rs485-sdk)

---
---

## 📖 日本語

### 概要

このリポジトリは、**華馨京科技（Fashion Star）製 UART / RS485 シリーズバスサーボモーター**を制御するための、マルチプラットフォーム対応 SDK サンプル集です。

サーボは UART または RS485 シリアルバスで通信し、1本のバスに接続した複数のサーボを対象に、精密な角度制御・速度制御・状態フィードバックを実現します。

通信プロトコルの仕様や追加ドキュメントは、公式 Wiki を参照してください：
👉 [Fashion Star Wiki](https://wiki.fashionrobo.com/)

---

### リポジトリ構成

| フォルダ | 説明 |
|---|---|
| `PC-Software` | PC ホストソフト：**Develop-US**（GUI コントロールパネル）、**Serial Port Debugger**（シリアルモニター） |
| `STM32F103` | STM32F103 SDK（HAL ライブラリ・標準ライブラリ対応） |
| `STM32F407` | STM32F407 SDK（HAL ライブラリ・標準ライブラリ対応） |
| `C++` | デスクトップ・組み込み Linux 向け C++ SDK |
| `C#` | Windows / .NET アプリ向け C# SDK |
| `python` | デスクトップ・Raspberry Pi 向け Python SDK |
| `micro-python` | マイコン向け MicroPython SDK（ESP32、Raspberry Pi Pico 等） |
| `arduino` | Arduino SDK（Uno、Mega 2560、ESP32 等対応） |
| `ros2` | ロボットミドルウェア ROS 2 向け Python SDK |

---

### 対応プラットフォーム

| プラットフォーム | 言語 |
|---|---|
| STM32F103 | C（HAL / 標準ライブラリ） |
| STM32F407 | C（HAL / 標準ライブラリ） |
| Arduino（Uno, Mega, ESP32 等） | C++ / Arduino |
| デスクトップ / Linux / Raspberry Pi | Python、C++、C# |
| マイコン（ESP32、Pico） | MicroPython |
| ROS 2 | Python |

---

### クイックスタート

1. 上記の表から **使用するプラットフォームのフォルダ**を選ぶ。
2. [Fashion Star Wiki](https://wiki.fashionrobo.com/) でそのプラットフォーム向けの **SDK ドキュメント**を確認する。
3. UART または RS485 でホストデバイスに **サーボを接続する**。
4. IDE でサンプルプロジェクトを開き、**ビルド・書き込み・実行する**。
5. `PC-Software/` 内の **PC ソフト**を使って、サーボ ID・ゼロ位置・パラメータをデバッグ・設定する。

---

### 通信プロトコルについて

この SDK は Fashion Star 独自の **UART / RS485 シリアルバスプロトコル**に対応しています。

プロトコルの主な特徴：
- 半二重シリアル通信（UART または RS485）
- サーボ ID によるマルチサーボアドレッシング
- 角度制御・速度制御・トルク ON/OFF・状態クエリ・パラメータ読み書きなどのコマンドに対応
- チェックサムによるパケット検証

プロトコルの詳細仕様：[UART/RS485 プロトコルリファレンス](https://wiki.fashionstar.com.hk/zh/servo/uart/protocols/uart-rs485-protocol/)

---

### 言語構成比

| 言語 | 割合 |
|---|---|
| C | 94.7% |
| C++ | 2.3% |
| Assembly | 1.3% |
| C# | 1.2% |
| Python | 0.4% |
| CMake | 0.1% |

---

### 更新履歴

| 日付 | 内容 |
|---|---|
| 2025/07/02 | リポジトリ作成、公式リリースと同期 |
| 2025/07/04 | ドキュメント同期 |
| 2025/07/15 | ROS2 SDK 追加 |
| 2025/07/22 | PC ホストソフト更新 |
| 2025/09/11 | コマンド計算スプレッドシート更新 |

---

### 関連リンク

- 🌐 [公式 Wiki（Fashion Star）](https://wiki.fashionrobo.com/)
- 📦 [SDK 一覧](https://wiki.fashionstar.com.hk/zh/sdk/)
- 📄 [UART サーボ データシート](https://wiki.fashionstar.com.hk/zh/servo/uart/datasheet/)
- 🔌 [UART/RS485 プロトコル仕様書](https://wiki.fashionstar.com.hk/zh/servo/uart/protocols/uart-rs485-protocol/)
- 🖥️ [PC 設定ソフト マニュアル](https://wiki.fashionstar.com.hk/zh/software/servo/pc-config-software/)
- 🤖 [オリジナルリポジトリ（servodevelop）](https://github.com/servodevelop/servo-uart-rs485-sdk)
