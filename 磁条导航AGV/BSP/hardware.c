#include "stm32f10x.h"
#include "hardware.h"
#include "rs485config.h"
#include "commtype.h"
static void COM_GPIO_init(GPIO_InitTypeDef *GPIO_InitStructure);
static void USART_Config(USART_TypeDef* USARTx, u32 rate);//��������

static void COM_GPIO_init(GPIO_InitTypeDef *GPIO_InitStructure)
{
	GPIO_InitStructure->GPIO_Mode = GPIO_Mode_AF_PP;     //�����������
	GPIO_InitStructure->GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure->GPIO_Pin = COM1_OUT;	            //USART1  TX
	GPIO_Init(GPIOA, GPIO_InitStructure);

	GPIO_InitStructure->GPIO_Pin = COM1_IN;	            // USART1 RX
	GPIO_InitStructure->GPIO_Mode = GPIO_Mode_IN_FLOATING; // ���ÿ�©����
	GPIO_Init(GPIOB, GPIO_InitStructure);

	GPIO_InitStructure->GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure->GPIO_Pin = COM2_OUT;
	GPIO_InitStructure->GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, GPIO_InitStructure);

	GPIO_InitStructure->GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure->GPIO_Pin = COM2_IN;
	GPIO_Init(GPIOA, GPIO_InitStructure);

	GPIO_InitStructure->GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure->GPIO_Pin = COM3_OUT;
	GPIO_InitStructure->GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, GPIO_InitStructure);

	GPIO_InitStructure->GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure->GPIO_Pin = COM3_IN;
	GPIO_Init(GPIOB, GPIO_InitStructure);
}

/*---------------------------------------------------------------------------------------------*/
void hardware_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʼ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOF \
		| RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB1Periph_USART2\
		| RCC_APB1Periph_USART3, ENABLE);

	COM_GPIO_init(&GPIO_InitStructure);
	USART_Config(COM1, 115200);
	USART_Config(COM2, 115200);
	USART_Config(COM3, 115200);
	//	uart_init(COM2, 115200);
	AGV_control_com_config();
	//GPIO_SetBits(GPIOF, GPIO_Pin_11);   //����485Ϊ���״̬���ݲ��ı���״̬
	change_rs485_mode(MSG_SEND);
}
/*----------------------------------------------------------------------------*/
void change_rs485_mode(u8 msgdir)
{
	if (msgdir == MSG_SEND)
	{
		GPIO_SetBits(GPIOF, GPIO_Pin_11);   //����485Ϊ���״̬���ݲ��ı���״̬
	}
	else if (msgdir == MSG_GET)
	{
		GPIO_ResetBits(GPIOF, GPIO_Pin_11);
	}
}

/*---------------------------------------------------------------------------------------------*/
void NVIC_configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			     	//���ô����ж�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			     	//���ô����ж�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			     	//���ô����ж�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*---------------------------------------------------------------------------------------------*/
static void USART_Config(USART_TypeDef* USARTx, u32 rate)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = rate;					//����115200bps
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;				//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(USARTx, &USART_InitStructure);							//���ô��ڲ�������
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);                      //ʹ�ܽ����ж�
	USART_Cmd(USARTx, ENABLE);
}







