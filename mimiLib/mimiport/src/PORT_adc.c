#include <stdio.h>
#ifndef LINUX
#include "adc.h"
#include "main.h"
#endif
unsigned short int PORT_adc_get(int adc_chanel)
{
#ifdef LINUX
	printf("get adc from chanel: %d\r\n", adc_chanel);
	return 0;
#endif

#ifndef LINUX
	unsigned short int ADC_val = 0;
	ADC_ChannelConfTypeDef sConfig = {0};
	uint32_t Channel;

	if (0 == adc_chanel)
	{
		Channel = ADC_CHANNEL_0;
	}
	if (1 == adc_chanel)
	{
		Channel = ADC_CHANNEL_1;
	}
	if (2 == adc_chanel)
	{
		Channel = ADC_CHANNEL_2;
	}
	if (3 == adc_chanel)
	{
		Channel = ADC_CHANNEL_3;
	}
	if (4 == adc_chanel)
	{
		Channel = ADC_CHANNEL_4;
	}
	if (5 == adc_chanel)
	{
		Channel = ADC_CHANNEL_5;
	}

	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
	sConfig.Channel = Channel;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 10); //ÂÖÑ¯×ª»»
	ADC_val = HAL_ADC_GetValue(&hadc1);
	return ADC_val;
#endif
}