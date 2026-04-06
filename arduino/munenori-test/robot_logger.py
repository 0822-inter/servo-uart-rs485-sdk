import serial
import json
import time
import os

# --- 設定 ---
# 先ほど確認したポート名に書き換えてください
SERIAL_PORT = '/dev/ttyACM0' 
BAUD_RATE = 115200
SAVE_FILE = 'rebonet_episodes.jsonl'

def main():
    try:
        # シリアルポートを開く
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"🚀 REBONET Logger 起動: {SERIAL_PORT} で待機中...")
        print("停止するには Ctrl+C を押してください。")

        while True:
            if ser.in_waiting > 0:
                # 1行読み込んで、不要な空白や改行を消す
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                
                # 「@DATA:」で始まる行だけを探す
                if line.startswith("@DATA:"):
                    json_str = line.replace("@DATA:", "")
                    try:
                        # JSONを解析
                        data = json.loads(json_str)
                        # 保存用に時刻を追加
                        data['unix_time'] = time.time()
                        
                        # ファイルに1行ずつ追記 (JSONL形式)
                        with open(SAVE_FILE, 'a', encoding='utf-8') as f:
                            f.write(json.dumps(data) + '\n')
                        
                        print(f"💾 データを保存しました: {data['event']}")
                    except json.JSONDecodeError:
                        print("⚠️ データが壊れていたためスキップしました")
            
            time.sleep(0.01) # CPU負荷軽減

    except serial.SerialException as e:
        print(f"❌ ポートが開けませんでした: {e}")
        print("※Arduino IDEのシリアルモニターを開いていると競合して開けません。閉じてから実行してください！")
    except KeyboardInterrupt:
        print("\n🛑 ログ記録を停止しました。")

if __name__ == "__main__":
    main()