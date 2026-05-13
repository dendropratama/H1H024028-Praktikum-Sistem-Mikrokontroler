## Jawaban Pertanyaan Praktikum 5.5

### 1. Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!

Ketiga task pada program FreeRTOS tidak benar-benar berjalan secara bersamaan karena Arduino Uno menggunakan single core processor. Task berjalan secara bergantian dengan sangat cepat sehingga terlihat seperti berjalan bersamaan (concurrent).

Mekanisme ini diatur oleh scheduler FreeRTOS. Scheduler membagi waktu eksekusi CPU kepada setiap task menggunakan teknik multitasking. Pada program terdapat tiga task yaitu:

- `TaskBlink1` untuk LED pada pin 8
- `TaskBlink2` untuk LED pada pin 7
- `Taskprint` untuk menampilkan counter pada serial monitor

Fungsi `vTaskDelay()` digunakan untuk memberikan jeda pada task. Saat sebuah task delay, scheduler akan menjalankan task lain yang siap dieksekusi. Karena pergantian berlangsung sangat cepat, LED dapat berkedip dengan interval berbeda dan serial monitor tetap menampilkan data secara realtime.

---

### 2. Bagaimana cara menambahkan task keempat? Jelaskan langkahnya!

Langkah menambahkan task keempat:

1. Membuat fungsi task baru

```cpp
void TaskBlink3(void *pvParameters)
{
  pinMode(6, OUTPUT);

  while(1)
  {
    Serial.println("Task4");

    digitalWrite(6, HIGH);
    vTaskDelay(400 / portTICK_PERIOD_MS);

    digitalWrite(6, LOW);
    vTaskDelay(400 / portTICK_PERIOD_MS);
  }
}
```

2. Menambahkan `xTaskCreate()` pada fungsi `setup()`

```cpp
xTaskCreate(
  TaskBlink3,
  "task4",
  128,
  NULL,
  1,
  NULL
);
```

3. Menambahkan LED dan resistor pada pin D6.

Setelah ditambahkan, scheduler FreeRTOS akan mengatur task baru bersama task lainnya sehingga semua task dapat berjalan secara concurrent.

---

### 3. Modifikasilah program dengan menambah sensor potensiometer untuk mengontrol kecepatan LED! Bagaimana hasilnya?

Program dimodifikasi dengan menambahkan potensiometer pada pin analog A0. Nilai analog digunakan untuk mengubah delay LED sehingga kecepatan kedip LED dapat berubah secara dinamis.

### Program

```cpp
#include <Arduino_FreeRTOS.h>

void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );
void Taskprint( void *pvParameters );
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  xTaskCreate(
  TaskBlink1
  , "task1"
  , 128
  , NULL
  , 1
  , NULL );
  xTaskCreate(
  TaskBlink2
  , "task2"
  , 128
  , NULL
  , 1 
  , NULL );
  xTaskCreate(
  Taskprint
  , "task3"
  , 128
  , NULL
  , 1
  , NULL );
  vTaskStartScheduler();
  }
void loop(){
}
void TaskBlink1(void *pvParameters) {
  pinMode(8, OUTPUT);
  while(1){
    Serial.println("Task1");
    digitalWrite(8, HIGH);
    vTaskDelay( 200 / portTICK_PERIOD_MS );
    digitalWrite(8, LOW);
    vTaskDelay( 200 / portTICK_PERIOD_MS );
    }
}
void TaskBlink2(void *pvParameters){
  pinMode(7, OUTPUT);
  while(1){
    Serial.println("Task2");
    digitalWrite(7, HIGH);
    vTaskDelay( 300 / portTICK_PERIOD_MS );
    digitalWrite(7, LOW);
    vTaskDelay( 300 / portTICK_PERIOD_MS );
  }
}
void Taskprint(void *pvParameters) {
  int counter = 0;
  while(1){
    counter++;
    Serial.println(counter);
    vTaskDelay(500 / portTICK_PERIOD_MS); 
  }
}
```

### Hasil Percobaan

Hasil percobaan menunjukkan bahwa putaran potensiometer mempengaruhi kecepatan kedip LED. Semakin besar nilai potensiometer maka delay semakin lama sehingga LED berkedip lebih lambat. Sebaliknya, jika nilai potensiometer kecil maka LED berkedip lebih cepat. Nilai delay juga tampil secara realtime pada serial monitor.

---

# Praktikum 5.6 Monitoring Sensor dengan FreeRTOS

## Jawaban Pertanyaan Praktikum

### 1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!

Kedua task berjalan secara bergantian menggunakan mekanisme multitasking dari FreeRTOS. Arduino Uno hanya memiliki satu inti prosesor sehingga task tidak benar-benar berjalan bersamaan.

Scheduler FreeRTOS akan membagi waktu CPU kepada setiap task secara cepat sehingga task terlihat berjalan secara concurrent. Ketika satu task memasuki kondisi delay, scheduler akan memindahkan eksekusi ke task lain yang siap dijalankan.

---

### 2. Apakah program ini berpotensi mengalami race condition? Jelaskan!

Ya, program berpotensi mengalami race condition apabila beberapa task mengakses resource yang sama secara bersamaan tanpa sinkronisasi.

Contohnya pada penggunaan `Serial.println()`. Jika beberapa task menulis data ke serial monitor secara bersamaan maka output dapat bercampur atau tidak teratur.

Untuk mengatasi race condition, FreeRTOS menyediakan mekanisme sinkronisasi seperti mutex dan semaphore agar hanya satu task yang dapat mengakses resource tertentu pada satu waktu.

---

### 3. Modifikasilah program menggunakan sensor DHT22 sehingga informasi yang ditampilkan dinamis! Bagaimana hasilnya?

Program dimodifikasi menggunakan sensor DHT22 untuk membaca suhu dan kelembaban secara realtime. Data sensor kemudian ditampilkan pada serial monitor menggunakan task FreeRTOS.

### Program

```cpp
#include <Arduino_FreeRTOS.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void TaskDHT(void *pvParameters);

void setup() {
  Serial.begin(9600);
  dht.begin();

  xTaskCreate(
    TaskDHT,
    "taskDHT",
    128,
    NULL,
    1,
    NULL
  );

  vTaskStartScheduler();
}

void loop() {
}

void TaskDHT(void *pvParameters)
{
  while(1)
  {
    float suhu = dht.readTemperature();
    float kelembaban = dht.readHumidity();

    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" C ");

    Serial.print("Kelembaban: ");
    Serial.print(kelembaban);
    Serial.println(" %");

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
```

### Hasil Percobaan

Hasil percobaan menunjukkan bahwa sensor DHT22 dapat membaca suhu dan kelembaban lingkungan secara realtime. Data yang tampil pada serial monitor berubah secara dinamis sesuai kondisi sekitar. FreeRTOS memungkinkan pembacaan sensor tetap berjalan dengan baik bersamaan dengan task lainnya sehingga sistem multitasking menjadi lebih efisien.
