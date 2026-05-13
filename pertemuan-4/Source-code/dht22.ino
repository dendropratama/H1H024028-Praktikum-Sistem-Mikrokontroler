#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>


#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

struct readings{
  float temp;
  float h;
};

QueueHandle_t my_queue;

void setup() {
  Serial.begin(9600);

  dht.begin();

  my_queue = xQueueCreate(1, sizeof(struct readings));

  xTaskCreate(read_data, "read sensors", 128, NULL, 0, NULL);
  xTaskCreate(display, "display", 128, NULL, 0, NULL);
}

void loop() {}

void read_data(void *pvParameters){
  struct readings x;

  for(;;){
    x.temp = dht.readTemperature();
    x.h = dht.readHumidity();

    xQueueSend(my_queue, &x, portMAX_DELAY);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void display(void *pvParameters){
  struct readings x;

  for(;;){
    if(xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS){

      Serial.print("Temperature = ");
      Serial.print(x.temp);
      Serial.println(" C");

      Serial.print("Humidity = ");
      Serial.print(x.h);
      Serial.println(" %");
    }
  }
}