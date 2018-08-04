HardwareSerial & ESPport = Serial;
#include <TimerOne.h>

//Список номеров телефонов
//String phone_numbers[] = { "+380630000000"};  //1 телефонный номер
String phone_numbers[] = {"+380631598264","+380671507560"};  //несколько
///целая розтяжка 10 high
#define Feeder_pin 8     
#define Roztazka_pin 11   //на розтяжку
int offmyself = 600; //600 sec будет включена а потом войдет в режим сна 

void setup() {
	pinMode(Feeder_pin, OUTPUT);
	digitalWrite(Feeder_pin, HIGH);  
	Timer1.initialize(1000000);  //1000000  = 1 секунда
	Timer1.attachInterrupt(OffTicker);
	pinMode(LED_BUILTIN, OUTPUT);

	ESPport.begin(115200);

	/*
	ESPport.println("at+cpin=\"1111\"");
	*/
	delay(1000);

	CallAll();

	if (digitalRead(Roztazka_pin) == HIGH)  //если разтяжка порвана
	{
		CallAll(); //Позвонили по всем номерам второй раз
		//Sms("+380630000000","Porvana Roztiazka");   //отправка СМС
	}
	W8REadyGsmModule(); //Подождали пока модем закончит текущие звонки.
	Off(); 
}
void CallAll()
{
	for (int i = 0;i != (sizeof((phone_numbers)) / sizeof((phone_numbers[0])));i++)
	{
		String command = "ATD" + phone_numbers[i] + ';';
		W8REadyGsmModule();
		ESPport.println(command.c_str());
	}
}
void W8REadyGsmModule() //Ждем готовность модуля
{       
	while (1)
	{
		ESPport.println("AT+CPAS");
		if (ESPport.find("+CPAS: 0")) break;
		delay(100);
	}
}
void Sms(String number,String tekstSMS)
{
		W8REadyGsmModule();
		ESPport.println("AT+CMGS=\""+ number +'\"'); // даем команду на отправку смс
		delay(100);
		ESPport.print(tekstSMS);  // отправляем текст
		ESPport.print((char)26);          // символ завершающий передачу
}


void OffTicker(void)
{
	if ((--offmyself) == 0)
	Off();
}
void Off()
{
	digitalWrite(Feeder_pin, LOW);
}
void loop() {}
