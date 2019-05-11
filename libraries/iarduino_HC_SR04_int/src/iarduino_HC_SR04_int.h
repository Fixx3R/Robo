//	Библиотека для работы с ультразвуковым датчиком расстояния HC-SR04: http://iarduino.ru/shop/Sensory-Datchiki/ultrazvukovogo-datchika-hc-sr04-rasstoyaniya-dvizheniya.html
//  Версия: 1.0.1 (с использованием прерываний)
//  Последнюю версию библиотеки Вы можете скачать по ссылке: http://iarduino.ru/file/283.html
//  Подробное описание функции бибилиотеки доступно по ссылке: http://wiki.iarduino.ru/page/ultrazvukovoy-datchik-izmereniya-rasstoyaniya-hc-sr04/
//  Библиотека является собственностью интернет магазина iarduino.ru и может свободно использоваться и распространяться!
//  При публикации устройств или скетчей с использованием данной библиотеки, как целиком, так и её частей,
//  в том числе и в некоммерческих целях, просим Вас опубликовать ссылку: http://iarduino.ru
//  Автор библиотеки: Панькин Павел sajaem@narod.ru
//  Если у Вас возникли технические вопросы, напишите нам: shop@iarduino.ru

#ifndef iarduino_HC_SR04_int_h
#define iarduino_HC_SR04_int_h

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class	iarduino_HC_SR04_int{
	public:			iarduino_HC_SR04_int(uint8_t, uint8_t);	//	Инициализация модуля				(вывод TRIG, вывод ECHO)
		long		distance			(int8_t=23);		//	Функция определения расстояния		([t°C])
		long		averaging = 0;							//	Коэффициент усреднения показаний	(0-без усреднений, 1-минимальное усреднение, ... 100-сильное усреднение, ... 2147483648-через день показания дойдут до реальных)
	private:												//	
		uint8_t		pinTRIG;								//	Номер вывода подключённого к TRIG
		uint8_t		pinECHO;								//	Номер вывода подключённого к ECHO
		uint8_t		numIRQ;									//	Номер прерывания для вывода  ECHO
		float		valDATA;								//	Переменная для рассчета ответа
};

class	iarduino_HC_SR04_int_volatile_class{
	public:
		bool		flgREQUEST[5];							//	Флаг наличия запроса данных
		uint8_t		pinECHO[5];								//	Флаг начала импульса ответа
		uint32_t	tmrSTART[5];							//	Время отправки запроса данных
		uint32_t	tmrDURATION[5];							//	Время длительности имульса ответа
};


#endif
