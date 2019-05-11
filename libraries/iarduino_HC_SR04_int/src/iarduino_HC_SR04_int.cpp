#include "iarduino_HC_SR04_int.h"

volatile	iarduino_HC_SR04_int_volatile_class	iHCSR04iVC;

//		ФУНКЦИИ ОБРАБОТКИ ПРЕРЫВАНИЙ:
void	HC_func_IRQ0()	{	if(iHCSR04iVC.flgREQUEST[0])	{	if(digitalRead(iHCSR04iVC.pinECHO[0]))	{	iHCSR04iVC.tmrSTART[0]=micros();	}			else			{	iHCSR04iVC.tmrDURATION[0]=micros()-iHCSR04iVC.tmrSTART[0];	iHCSR04iVC.flgREQUEST[0]=false;}}}
void	HC_func_IRQ1()	{	if(iHCSR04iVC.flgREQUEST[1])	{	if(digitalRead(iHCSR04iVC.pinECHO[1]))	{	iHCSR04iVC.tmrSTART[1]=micros();	}			else			{	iHCSR04iVC.tmrDURATION[1]=micros()-iHCSR04iVC.tmrSTART[1];	iHCSR04iVC.flgREQUEST[1]=false;}}}
void	HC_func_IRQ2()	{	if(iHCSR04iVC.flgREQUEST[2])	{	if(digitalRead(iHCSR04iVC.pinECHO[2]))	{	iHCSR04iVC.tmrSTART[2]=micros();	}			else			{	iHCSR04iVC.tmrDURATION[2]=micros()-iHCSR04iVC.tmrSTART[2];	iHCSR04iVC.flgREQUEST[2]=false;}}}
void	HC_func_IRQ3()	{	if(iHCSR04iVC.flgREQUEST[3])	{	if(digitalRead(iHCSR04iVC.pinECHO[3]))	{	iHCSR04iVC.tmrSTART[3]=micros();	}			else			{	iHCSR04iVC.tmrDURATION[3]=micros()-iHCSR04iVC.tmrSTART[3];	iHCSR04iVC.flgREQUEST[3]=false;}}}
void	HC_func_IRQ4()	{	if(iHCSR04iVC.flgREQUEST[4])	{	if(digitalRead(iHCSR04iVC.pinECHO[4]))	{	iHCSR04iVC.tmrSTART[4]=micros();	}			else			{	iHCSR04iVC.tmrDURATION[4]=micros()-iHCSR04iVC.tmrSTART[4];	iHCSR04iVC.flgREQUEST[4]=false;}}}
//						|	Если были запрошены данные		|	Если датчик отпрвил звук				|	Сохраняем время начала импульса		|	Если датчик принял звук	|	Определяем длительность импульса							Сбрасываем флаг запроса данных
//						|	(был вызов функции distance)	|	(появился импульс на выводе pinECHO)	|										|	(импульс исчез)			|	(время за которое звук дошел до препятствия и вернулся)
//		Несколько функций позволяют ускорить обработку каждого прерывания. Время затраченное на выполнение функции digitalRead не влияет на полученное время длительности импульса, так как зедержка присутствует и в начале и в конце импульса

//		КОНФИГУРАЦИЯ ВЫВОДОВ И НАЗНАЧЕНИЕ ОБРАБОТЧИКОВ ПРЕРЫВАНИЙ:					//
		iarduino_HC_SR04_int::iarduino_HC_SR04_int(uint8_t i, uint8_t j){			//
//			Сохраняем номера выводов и прерываний:									//
			pinTRIG = i;															//	Номер вывода TRIG
			pinECHO =(digitalPinToInterrupt(j)==NOT_AN_INTERRUPT)?2:j;				//	Номер вывода ECHO
			numIRQ  = digitalPinToInterrupt(pinECHO);								//	Номер прерывания
			iHCSR04iVC.pinECHO[numIRQ]=pinECHO;										//	Номер вывода ECHO для функций обработки прерываний
//			Устанавливаем режим работы выводов:										//	
			pinMode(pinTRIG, OUTPUT);												//	Вывод pinTRIG является выходом
			pinMode(pinECHO, INPUT);												//	Вывод pinECHO является входом
//			Назначаем обработку прерывания numIRQ одной из функций HC_func_IRQ:		//
			uint8_t k = numIRQ;														//	Сохраняем номер прерывания
			#ifdef __SAM3X8E__ 														//	А для микроконтроллеров на базе __SAM3X8E__ , нопример Arduino Due
			k = pinECHO;															//	Сохраняем номер вывода
			#endif																	//
			switch(numIRQ){															//
				case 0:	attachInterrupt(k, HC_func_IRQ0, CHANGE); break;			//	Для прерывания с номером 0 назначаем функцию HC_func_IRQ0
				case 1:	attachInterrupt(k, HC_func_IRQ1, CHANGE); break;			//	Для прерывания с номером 1 назначаем функцию HC_func_IRQ1
				case 2:	attachInterrupt(k, HC_func_IRQ2, CHANGE); break;			//	Для прерывания с номером 2 назначаем функцию HC_func_IRQ2
				case 3:	attachInterrupt(k, HC_func_IRQ3, CHANGE); break;			//	Для прерывания с номером 3 назначаем функцию HC_func_IRQ3
				case 4:	attachInterrupt(k, HC_func_IRQ4, CHANGE); break;			//	Для прерывания с номером 4 назначаем функцию HC_func_IRQ4
			}																		//	
//			устанавливаем уровень логического 0 на выводе TRIG						//	
			digitalWrite(pinTRIG, LOW);												//	
}																					//
																					//
//		ЧТЕНИЕ РЕЗУЛЬТАТА:															//
long	iarduino_HC_SR04_int::distance(int8_t i){									//	i: текущая температура в °C
			if(micros()<iHCSR04iVC.tmrSTART[numIRQ]){								//	Если произошло переполнение micros(), то ...
				iHCSR04iVC.tmrSTART[numIRQ]=0;										//	Сбрасываем время последнего запроса (время установки импульса на выходе ECHO датчика)
				iHCSR04iVC.flgREQUEST[numIRQ]=false;								//	Сбрасываем флаг наличия отправленного запроса данных
			}																		//
			if(iHCSR04iVC.tmrSTART[numIRQ]+50000<micros()){                         //  Если с последнего запроса прошло более 50 мс, то ...
				iHCSR04iVC.flgREQUEST[numIRQ];										//	Сбрасываем флаг наличия отправленного запроса данных (так как датчик возможно завис)
			}																		//
			if(iHCSR04iVC.tmrSTART[numIRQ]+20000<micros()){                         //  Если с последнего запроса прошло более 20 мс, то ... (если отправлять чаще, то есть шанс получать эхо не от препятствия, а от более более удалённых объектов)
			if(!iHCSR04iVC.flgREQUEST[numIRQ]){										//	Если запрос уже не отправлен, то ...
				iHCSR04iVC.flgREQUEST[numIRQ]=true;									//	Устанавливаем флаг наличия отправленного запроса данных
				digitalWrite(pinTRIG, HIGH);										//	Устанавливаем уровень логической 1 на выводе TRIG
				delayMicroseconds(10);												//	Ждём 10 мкс
				digitalWrite(pinTRIG, LOW);											//	Устанавливаем уровень логического 0 на выводе TRIG
			}																		//
			}																		//
			long j = iHCSR04iVC.tmrDURATION[numIRQ];								//	Получаем время импульса
			j *= sqrt(273+i);														//	Умножаем время на корень из температуры в Кельвинах
			j /= 1000;																//	Делим полученное число на 1000
			if(j>400){j=400;}														//	Если результат больше 400 см (4 м) значит это ошибка
			if(averaging<=0){valDATA=float(j);}else{								//	Если усреднение результата не требуется, то текущий результат и есть выводимый результат, иначе ...
				valDATA *= float(averaging);										//	Умножаем предыдущий результат на коэффициент усреднения - 1
				valDATA += j;														//	Добавляем к полученному значению текущий результат
				valDATA /= float(averaging)+1.0;									//	Делим полученное значение на коэффициент усреднения
			}																		//
			return long(valDATA);													//	Выводим результат
}