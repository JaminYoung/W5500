


#include "wizchip_conf.h"
#include "spi.h"



void W5500_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );
  
	// ???????SPI-CS?? (PA0)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25Q64/NRF24L01						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPI1_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTBʱ��ʹ�� 
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );//SPI2ʱ��ʹ�� 	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOB

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);  //PB13/14/15����

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;   //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		    //ʱ�����յ�
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	    //���ݲ����ڵ�1��ʱ����
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	    //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		    //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//������Ԥ��Ƶֵ:������Ԥ��ƵֵΪ256 ��ʼ��ʱΪ�����ģʽ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure); 
 
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	
//	SPI1_ReadWriteByte(0xff);//��������		 
 

}   
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
  
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI�ٶ� 
	SPI_Cmd(SPI1,ENABLE); 

} 

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
void SPI1_WriteByte(u8 TxData)
{		
	while((SPI1->SR&SPI_I2S_FLAG_TXE)==0);   //??????          
	SPI1->DR=TxData;                                         //????byte 
	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==0); //???????byte  
	SPI1->DR;     
}


u8 SPI1_ReadByte()
{

	while((SPI1->SR&SPI_I2S_FLAG_TXE)==0);   //??????              
	SPI1->DR=0xFF;                                               //???????????????? 
	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==0); //???????byte  
	return SPI1->DR;       
}



/**
  * @brief  ?????
  * @retval None
  */
void SPI_CrisEnter(void)
{
    __set_PRIMASK(1);
}
/**
  * @brief  ?????
  * @retval None
  */
void SPI_CrisExit(void)
{
    __set_PRIMASK(0);
}
 
/**
  * @brief  ?????????
  * @retval None
  */
void SPI_CS_Select(void)
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_0);
}
/**
  * @brief  ?????????
  * @retval None
  */
void SPI_CS_Deselect(void)
{
    GPIO_SetBits(GPIOA,GPIO_Pin_0);
}
void register_wizchip()
{
	// First of all, Should register SPI callback functions implemented by user for accessing WIZCHIP 
	/* Critical section callback */
	reg_wizchip_cris_cbfunc(SPI_CrisEnter, SPI_CrisExit);   
	/* Chip selection call back */
	reg_wizchip_cs_cbfunc(SPI_CS_Select, SPI_CS_Deselect);
	
	/* SPI Read & Write callback function */
	reg_wizchip_spi_cbfunc(SPI1_ReadByte, SPI1_WriteByte);  
}



























