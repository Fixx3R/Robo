#include "iarduino_HC_SR04_int.h"

volatile	iarduino_HC_SR04_int_volatile_class	iHCSR04iVC;

//		������� ��������� ����������:
void	HC_func_IRQ0()	{	if(iHCSR04iVC.flgREQUEST[0])	{	if(digitalRead(iHCSR04iVC.pinECHO[0]))	{	iHCSR04iVC.tmrSTART[0]=micros();	}			else			{	iHCSR04iVC.tmrDURATION[0]=micros()-iHCSR04iVC.tmrSTART[0];	iHCSR04iVC.flgREQUEST[0]=false;}}}
void	HC_func_IRQ1()	{	if(iHCSR04iVC.flgREQUEST[1])	{	if(digitalRead(iHCSR04iVC.pinECHO[1]))	{	iHCSR04iVC.tmrSTART[1]=micros();	}			else			{	iHCSR04iVC.tmrDURATION[1]=micros()-iHCSR04iVC.tmrSTART[1];	iHCSR04iVC.flgREQUEST[1]=false;}}}
void	HC_func_IRQ2()	{	if(iHCSR04iVC.flgREQUEST[2])	{	if(digitalRead(iHCSR04iVC.pinECHO[2]))	{	iHCSR04iVC.tmrSTART[2]=micros();	}			else			{	iHCSR04iVC.tmrDURATION[2]=micros()-iHCSR04iVC.tmrSTART[2];	iHCSR04iVC.flgREQUEST[2]=false;}}}
void	HC_func_IRQ3()	{	if(iHCSR04iVC.flgREQUEST[3])	{	if(digitalRead(iHCSR04iVC.pinECHO[3]))	{	iHCSR04iVC.tmrSTART[3]=micros();	}			else			{	iHCSR04iVC.tmrDURATION[3]=micros()-iHCSR04iVC.tmrSTART[3];	iHCSR04iVC.flgREQUEST[3]=false;}}}
void	HC_func_IRQ4()	{	if(iHCSR04iVC.flgREQUEST[4])	{	if(digitalRead(iHCSR04iVC.pinECHO[4]))	{	iHCSR04iVC.tmrSTART[4]=micros();	}			else			{	iHCSR04iVC.tmrDURATION[4]=micros()-iHCSR04iVC.tmrSTART[4];	iHCSR04iVC.flgREQUEST[4]=false;}}}
//						|	���� ���� ��������� ������		|	���� ������ ������� ����				|	��������� ����� ������ ��������		|	���� ������ ������ ����	|	���������� ������������ ��������							���������� ���� ������� ������
//						|	(��� ����� ������� distance)	|	(�������� ������� �� ������ pinECHO)	|										|	(������� �����)			|	(����� �� ������� ���� ����� �� ����������� � ��������)
//		��������� ������� ��������� �������� ��������� ������� ����������. ����� ����������� �� ���������� ������� digitalRead �� ������ �� ���������� ����� ������������ ��������, ��� ��� �������� ������������ � � ������ � � ����� ��������

//		������������ ������� � ���������� ������������ ����������:					//
		iarduino_HC_SR04_int::iarduino_HC_SR04_int(uint8_t i, uint8_t j){			//
//			��������� ������ ������� � ����������:									//
			pinTRIG = i;															//	����� ������ TRIG
			pinECHO =(digitalPinToInterrupt(j)==NOT_AN_INTERRUPT)?2:j;				//	����� ������ ECHO
			numIRQ  = digitalPinToInterrupt(pinECHO);								//	����� ����������
			iHCSR04iVC.pinECHO[numIRQ]=pinECHO;										//	����� ������ ECHO ��� ������� ��������� ����������
//			������������� ����� ������ �������:										//	
			pinMode(pinTRIG, OUTPUT);												//	����� pinTRIG �������� �������
			pinMode(pinECHO, INPUT);												//	����� pinECHO �������� ������
//			��������� ��������� ���������� numIRQ ����� �� ������� HC_func_IRQ:		//
			uint8_t k = numIRQ;														//	��������� ����� ����������
			#ifdef __SAM3X8E__ 														//	� ��� ����������������� �� ���� __SAM3X8E__ , �������� Arduino Due
			k = pinECHO;															//	��������� ����� ������
			#endif																	//
			switch(numIRQ){															//
				case 0:	attachInterrupt(k, HC_func_IRQ0, CHANGE); break;			//	��� ���������� � ������� 0 ��������� ������� HC_func_IRQ0
				case 1:	attachInterrupt(k, HC_func_IRQ1, CHANGE); break;			//	��� ���������� � ������� 1 ��������� ������� HC_func_IRQ1
				case 2:	attachInterrupt(k, HC_func_IRQ2, CHANGE); break;			//	��� ���������� � ������� 2 ��������� ������� HC_func_IRQ2
				case 3:	attachInterrupt(k, HC_func_IRQ3, CHANGE); break;			//	��� ���������� � ������� 3 ��������� ������� HC_func_IRQ3
				case 4:	attachInterrupt(k, HC_func_IRQ4, CHANGE); break;			//	��� ���������� � ������� 4 ��������� ������� HC_func_IRQ4
			}																		//	
//			������������� ������� ����������� 0 �� ������ TRIG						//	
			digitalWrite(pinTRIG, LOW);												//	
}																					//
																					//
//		������ ����������:															//
long	iarduino_HC_SR04_int::distance(int8_t i){									//	i: ������� ����������� � �C
			if(micros()<iHCSR04iVC.tmrSTART[numIRQ]){								//	���� ��������� ������������ micros(), �� ...
				iHCSR04iVC.tmrSTART[numIRQ]=0;										//	���������� ����� ���������� ������� (����� ��������� �������� �� ������ ECHO �������)
				iHCSR04iVC.flgREQUEST[numIRQ]=false;								//	���������� ���� ������� ������������� ������� ������
			}																		//
			if(iHCSR04iVC.tmrSTART[numIRQ]+50000<micros()){                         //  ���� � ���������� ������� ������ ����� 50 ��, �� ...
				iHCSR04iVC.flgREQUEST[numIRQ];										//	���������� ���� ������� ������������� ������� ������ (��� ��� ������ �������� �����)
			}																		//
			if(iHCSR04iVC.tmrSTART[numIRQ]+20000<micros()){                         //  ���� � ���������� ������� ������ ����� 20 ��, �� ... (���� ���������� ����, �� ���� ���� �������� ��� �� �� �����������, � �� ����� ����� �������� ��������)
			if(!iHCSR04iVC.flgREQUEST[numIRQ]){										//	���� ������ ��� �� ���������, �� ...
				iHCSR04iVC.flgREQUEST[numIRQ]=true;									//	������������� ���� ������� ������������� ������� ������
				digitalWrite(pinTRIG, HIGH);										//	������������� ������� ���������� 1 �� ������ TRIG
				delayMicroseconds(10);												//	��� 10 ���
				digitalWrite(pinTRIG, LOW);											//	������������� ������� ����������� 0 �� ������ TRIG
			}																		//
			}																		//
			long j = iHCSR04iVC.tmrDURATION[numIRQ];								//	�������� ����� ��������
			j *= sqrt(273+i);														//	�������� ����� �� ������ �� ����������� � ���������
			j /= 1000;																//	����� ���������� ����� �� 1000
			if(j>400){j=400;}														//	���� ��������� ������ 400 �� (4 �) ������ ��� ������
			if(averaging<=0){valDATA=float(j);}else{								//	���� ���������� ���������� �� ���������, �� ������� ��������� � ���� ��������� ���������, ����� ...
				valDATA *= float(averaging);										//	�������� ���������� ��������� �� ����������� ���������� - 1
				valDATA += j;														//	��������� � ����������� �������� ������� ���������
				valDATA /= float(averaging)+1.0;									//	����� ���������� �������� �� ����������� ����������
			}																		//
			return long(valDATA);													//	������� ���������
}