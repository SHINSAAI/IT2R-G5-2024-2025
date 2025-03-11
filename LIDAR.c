#include "UART_LPC17xx.h"
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "GLCD_Fonts.h"                 // Keil.MCB1700::Board Support:Graphic LCD


// __________ Dénifintion des octets utilisés __________ \\

#define LIDAR_START_HEADER 					0xA5
#define LIDAR_RESPONSE_HEADER				0x5A
#define LIDAR_REQUEST_RESET 				0x40
#define LIDAR_REQUEST_STOP 					0x25
#define LIDAR_REQUEST_SCAN					0x20
#define LIDAR_REQUEST_FORCE_SCAN		0x21
#define LIDAR_REQUEST_GET_INFO			0x50

extern GLCD_FONT GLCD_Font_16x24;

// __________ Prototypes Fonctions UART __________ \\

void Init_UART1(void);

// __________ Prototypes Fonctions LIDAR __________ \\

void LIDAR_Reset(void);
void LIDAR_Stop(void);
void LIDAR_Get_Info(void);		// Récupère les infos du lidar **Pour on ne lit pas les données renvoyées**
void LIDAR_Scan(void);
void LIDAR_Force_Scan(void);
void LIDAR_Attente_Header(void);



// __________ Main __________ \\

int main(void) {
	int i;
	unsigned char reception[50], header[10] = {0};
	GLCD_Initialize();
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
	Init_UART1();								// Initialisation UART1
	//LIDAR_Get_Info();
	for(i=0; i<50000; i++);
	GLCD_ClearScreen();
	
	while(1) {
		//LIDAR_Scan();
		//for(i=0; i<100000; i++);
		LIDAR_Get_Info();
		
		// Attente du header
		
		while(Driver_USART1.GetStatus().rx_busy);			// On attend que la réception soit libre
		while(header[0] != LIDAR_START_HEADER) {										// tant qu'on a pas 0xA5 on attend (premier bit du header)
			while(Driver_USART1.GetStatus().rx_busy);
			Driver_USART1.Receive(header, 1);
		}
		while(Driver_USART1.GetStatus().rx_busy);			// On attend que la réception soit libre
		while(header[0] != 0x5A) {										// tant qu'on a pas 0x5A on attend (premier bit du header)
			while(Driver_USART1.GetStatus().rx_busy);
			Driver_USART1.Receive(header, 1);
		}
		
		//Driver_USART1.Receive(header, 5);
		
		// Reception trame
		
		while(Driver_USART1.GetStatus().rx_busy);
		Driver_USART1.Receive(reception, 25);
		for(i=0; i<25; i++) {
			if(i<20) {
				GLCD_DrawChar(16*i,0,reception[i]);
			} else {
				GLCD_DrawChar(16*(i-20),24,reception[i]);
			}
		}
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
