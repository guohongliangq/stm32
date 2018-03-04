#include<stm32f10x_gpio.h>
#include<stm32f10x_usart.h>
#include<stdio.h>

void RCC_Configuration(void)
{
  RCC_DeInit();
 
  RCC_HSICmd(ENABLE);
 
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

  if(1)
  { 
 //  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
  
  // FLASH_SetLatency(FLASH_Latency_2);
  
   RCC_HCLKConfig(RCC_SYSCLK_Div1); //AHB???????SYSCLK
  
   RCC_PCLK2Config(RCC_HCLK_Div1);  //APB1???HCLK/2,??HCLK?AHB??
  
   RCC_PCLK1Config(RCC_HCLK_Div2);  //APB2???HCLK
  
   RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_9);
  
   RCC_PLLCmd(ENABLE);
  
   while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
 
   RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
   while(RCC_GetSYSCLKSource() != 0x08);
   }
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
//        RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
  
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 | RCC_APB1Periph_I2C2, ENABLE);
  
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}
void uart1Configure(void)
{
	USART_InitTypeDef uartstructure;
	GPIO_InitTypeDef gpioaInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//RCC_PCLK1Config(RCC_HCLK_Div8);
	//RCC_HCLKConfig(RCC_SYSCLK_Div1);
	gpioaInitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	gpioaInitStructure.GPIO_Pin=GPIO_Pin_10;
	gpioaInitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC,&gpioaInitStructure);
	
	gpioaInitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpioaInitStructure.GPIO_Pin=GPIO_Pin_11;
	
	GPIO_Init(GPIOC,&gpioaInitStructure);
	
	gpioaInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	gpioaInitStructure.GPIO_Pin=GPIO_Pin_0;
	gpioaInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC,&gpioaInitStructure);
	
	gpioaInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	gpioaInitStructure.GPIO_Pin=GPIO_Pin_5;
	gpioaInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&gpioaInitStructure);
	
	uartstructure.USART_BaudRate=57600;
	uartstructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	uartstructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	uartstructure.USART_Parity=USART_Parity_No;
	uartstructure.USART_StopBits=USART_StopBits_1;
	uartstructure.USART_WordLength=USART_WordLength_8b;

	
	USART_Init(USART3, &uartstructure);
	
	USART_Cmd(USART3,ENABLE);
}
void delay(uint16_t ms)
{
	 uint32_t i = 0;
	 while (ms--)
	 {
		 i=7000;
		 while(i--);
			
	 }
}
int fputc(int ch ,FILE *fp)
{
	USART_SendData(USART3,(unsigned char)ch);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
	
	return (ch);
}
int fgetc(FILE *f)
{
	
		while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART3);
}

int main(void)
{
	RCC_Configuration();
	uart1Configure();
	while(1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_0);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		printf("this is a test \r\n");
		printf("\r\n#####################################\r\n");
		USART_SendData(USART3,0xa5);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
		USART_SendData(USART3,0xa9);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
		USART_SendData(USART3,0xad);
	
		delay(1000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_0);
		delay(1000);
	}
	return 0;
}

