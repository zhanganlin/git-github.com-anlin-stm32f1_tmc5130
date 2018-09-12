#include <math.h>
#include "HAL.h"
#include "ADCs.h"
#include "stm32f10x.h"
#include "IOMap.h"
#include "stm32f10x_adc.h"

#define ADC1_DR_ADDRESS  ((uint32)0x4001204C)

static void init(void);
static void deInit(void);

ADCTypeDef ADCs =
{
	.AIN0   = &ADCValue[0],
	.AIN1   = &ADCValue[1],
	.AIN2   = &ADCValue[2],
	.DIO4   = &ADCValue[3],
	.DIO5   = &ADCValue[4],
	.VM     = &ADCValue[5],
	.init   = init,
	.deInit  = deInit
};

void init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	ADC_DeInit();

	/* Enable peripheral clocks *************************************************/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	HAL.IOs->config->reset(&HAL.IOs->pins->AIN0);
	HAL.IOs->config->reset(&HAL.IOs->pins->AIN1);
	HAL.IOs->config->reset(&HAL.IOs->pins->AIN2);
	HAL.IOs->config->reset(&HAL.IOs->pins->VM_MEAS);


	/* DMA2_Stream0 channel0 configuration **************************************/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS;                 //DMA外设基地址  即ADC地址
    DMA_InitStructure.DMA_MemoryBaseAddr=(u32)&ADCValue;              //DMA内存基地址  即要存放数据的地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //外设作为数据传输的来源
    DMA_InitStructure.DMA_BufferSize = 6;                                       //DMA 缓存大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //外设地址不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //内存地址不变
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据宽度16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //内存数据宽度
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             //工作在循环缓存模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         //设定DMA通道X 的优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //DMA 通道X 没有设置 内存到内存传输
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                //配置DMA 通道1 参数
    /* Enable DMA channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC Common Init **********************************************************/
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;                       //独立ADC模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE ;                          //启动扫描模式，扫描模式用于多通道采集
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                     //开启连续转换模式，即不停地进行ADC转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;    //转换由软件而不使用外部触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                 //采集数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 6;                                //要转换的通道数目3
    ADC_Init(ADC1,&ADC_InitStructure);                                     //配置设定好的工作方式

	/*配置ADC时钟，为PCLK2的8分频，即9MHz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    
    /*配置ADC1的通道11为55.        5个采样周期，采样顺序为1 */   //见   库函数P49
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 6, ADC_SampleTime_55Cycles5);

    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);         //使能ADC1 的DMA 请求        见库函数 P45
    
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);           //使能ADC1 见库函数 P44    注意：函数ADC_Cmd 只能在其他ADC函数设置后被调用

    //在开启AD 转换前  启动AD 自校准
    /*复位校准寄存器 */   
    ADC_ResetCalibration(ADC1);      //库函数 P46 页

    /*等待校准寄存器复位完成 */
    while(ADC_GetResetCalibrationStatus(ADC1));

    /* 开启 ADC校准 */
    ADC_StartCalibration(ADC1);
    /* 等待校准完成*/
    while(ADC_GetCalibrationStatus(ADC1));

    /* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);  
}

static void deInit(void)
{
	 ADC_DeInit();
}
