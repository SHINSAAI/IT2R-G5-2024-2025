#include "UART_LPC17xx.h"

// __________ Dénifintion des octets utilisés __________ \\

#define LIDAR_START_HEADER 					0xA5
#define LIDAR_REQUEST_RESET 				0x40
#define LIDAR_REQUEST_STOP 					0x25
#define LIDAR_REQUEST_SCAN					0x20
#define LIDAR_REQUEST_FORCE_SCAN		0x21
#define LIDAR_REQUEST_GET_INFO			0x50

// __________ Prototypes Fonctions UART __________ \\

void Init_UART1(void);

// __________ Prototypes Fonctions LIDAR __________ \\

void LIDAR_Reset(void);
void LIDAR_Stop(void);
void LIDAR_Get_Info(void);		// Récupère les infos du lidar **Pour on ne lit pas les données renvoyées**
void LIDAR_Scan(void);
void LIDAR_Force_Scan(void);





// __________ Main __________ \\

int main(void) {
	int i;
	Init_UART1();								// Initialisation UART1
	LIDAR_Reset();
	for(i=0; i<50000; i++);
	LIDAR_Force_Scan();
	
	while(1) {
		//LIDAR_Scan();
		for(i=0; i<100000; i++);
	}
	return 0;
}





// __________ Fonctions UART __________ \\

void Init_UART1(void) {
	Driver_USART1.Initialize(NULL);
	Driver_USART1.PowerControl(ARM_POWER_FULL);
	Driver_USART1.Control(	ARM_USART_MODE_ASYNCHRONOUS |
													ARM_USART_DATA_BITS_8 |
													ARM_USART_STOP_BITS_1 |
													ARM_USART_PARITY_NONE |
													ARM_USART_FLOW_CONTROL_NONE,
													115200);
	Driver_USART1.Control(ARM_USART_CONTROL_TX, 1);
	Driver_USART1.Control(ARM_USART_CONTROL_RX, 1);
}

// __________ Fonctions LIDAR __________ \\

void LIDAR_Get_Info(void) {
	char tab[2] = {LIDAR_START_HEADER,LIDAR_REQUEST_GET_INFO};
	
	while(Driver_USART1.GetStatus().tx_busy);
	Driver_USART1.Send(tab,2);
}

void LIDAR_Scan(void) {
	char tab[2] = {LIDAR_START_HEADER,LIDAR_REQUEST_SCAN};
	
	while(Driver_USART1.GetStatus().tx_busy);
	Driver_USART1.Send(tab,2);
}

void LIDAR_Reset(void) {
	char tab[2] = {LIDAR_START_HEADER,LIDAR_REQUEST_RESET};
	
	while(Driver_USART1.GetStatus().tx_busy);
	Driver_USART1.Send(tab,2);
}

void LIDAR_Stop(void) {
	char tab[2] = {LIDAR_START_HEADER,LIDAR_REQUEST_STOP};
	
	while(Driver_USART1.GetStatus().tx_busy);
	Driver_USART1.Send(tab,2);
}

void LIDAR_Force_Scan(void) {
	char tab[2] = {LIDAR_START_HEADER,LIDAR_REQUEST_FORCE_SCAN};
	
	while(Driver_USART1.GetStatus().tx_busy);
	Driver_USART1.Send(tab,2);
}
