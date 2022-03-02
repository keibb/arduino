
#include <BLEDevice.h>
#include <M5StickC.h>

/* スキャン制御用 */
#define DEVICE_NAME "ESP32_M5stamp"         // 対象デバイス名
#define MAX_DEVICES  8              // 最大デバイス数
#define ManufacturerId 0xffff       // 既定のManufacturer ID
const int scanning_time = 3;        // スキャン時間（秒）
BLEScan* pBLEScan;                  // Scanオブジェクトへのポインター
int prev_seq[MAX_DEVICES] = { -1, -1, -1, -1, -1, -1, -1, -1 };//デバイスごとのシーケンス番号の変化を管理する配列

/* 受信データ構造体 */
struct tmpData {
    int     device_number;          // デバイス識別番号
    bool    abnormal;               // デバイス異常
    int     seq_number;             // シーケンス番号
    float   temperature;            // 温度
    float   humidity;               // 湿度
};

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Axp.ScreenBreath(9);
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);

  pinMode(M5_BUTTON_HOME, INPUT);
  
  BLEDevice::init("");                        // BLEデバイスを作成する
  Serial.println("Client application start...");
  pBLEScan = BLEDevice::getScan();            // Scanオブジェクトを取得して、
  pBLEScan->setActiveScan(false);             // パッシブスキャンに設定する
}

uint8_t setup_flag = 1;
void loop() {
  struct tmpData td;

    // 所定時間だけスキャンして、見つかったデバイス数を取得する
    BLEScanResults foundDevices = pBLEScan->start(scanning_time);
    int count = foundDevices.getCount();

    for (int i = 0; i < count; i++) {     // 受信したアドバタイズデータを調べて
        BLEAdvertisedDevice dev = foundDevices.getDevice(i);
        std::string device_name = dev.getName();
        if (device_name == DEVICE_NAME 
                && dev.haveManufacturerData()) {   // デバイス名が一致しManufacturer dataがあって
            std::string data = dev.getManufacturerData();
            int manu_code = data[1] << 8 | data[0];
            int device_number = data[2];
            int seq_number = data[4];

            // デバイス識別番号が有効かつシーケンス番号が更新されていたら
            if (device_number >= 1 && device_number <= MAX_DEVICES
                      && seq_number != prev_seq[device_number - 1]) {
                // 受信データを取り出す
                prev_seq[device_number - 1] = seq_number;
                td.device_number = device_number;
                td.abnormal = (bool)data[3];
                td.seq_number = seq_number;
                td.temperature = (float)(data[6] << 8 | data[5]) / 100.00;
                td.humidity = (float)(data[8] << 8 | data[7]) / 100.00;

            }
        }
    }
  // put your main code here, to run repeatedly:
  Serial.begin(9600);
  displayValues("BME SENSOR", "", "");

  int buttonState = digitalRead(M5_BUTTON_HOME);
  if (buttonState == HIGH) {
        displayValues("", "", "");
    }
}

/* 受信データを表示する */
void displayData(struct tmpData* td) {
    char sTemp[10], sHumd[10];
    
    sprintf(sTemp, "%5.2fC", td->temperature);
    sprintf(sHumd, "%5.2f%%", td->humidity);
    Serial.print("Received from device ");  Serial.print(td->device_number);
    Serial.print("  Data No.");             Serial.print(td->seq_number);
    Serial.print("  Temperature: ");        Serial.print(sTemp);
    Serial.print(",  Humidity: ");          Serial.println(sHumd);
    if (td->device_number == 1) {   // (デバイス識別が1のもののみOLEDに表示）
        displayValues("BME SENSOR", sTemp, sHumd);
    }
}

/* 測定値をディスプレイに表示する */
void displayValues(char* line1, char* line2, char* line3) {
    M5.Lcd.setCursor(0, 0, 4);    M5.Lcd.printf(line1);
    M5.Lcd.setCursor(0, 20, 4);   M5.Lcd.printf(line2);
    M5.Lcd.setCursor(0, 40, 4);   M5.Lcd.printf(line3);
}
