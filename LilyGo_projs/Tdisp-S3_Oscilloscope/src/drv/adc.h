#ifndef _ADC_H_
#define _ADC_H_

	#include <driver/adc.h>
	#include <esp_adc_cal.h>
	#include <esp_wifi.h>


	#define ADC_START_STOP_MICROS	240
	#define ADC_RESULT_BYTE			4
	#define ADC_CONV_LIMIT_EN		0
	#define GET_UNIT(x)				((x>>3) & 0x1)


	#define ONCE_SAMPLES_BUFFLEN	((ONCE_SAMPLES * ADC_RESULT_BYTE))
	#define ONCE_SAMPLES_ALIGN		((~(ONCE_SAMPLES - 1)))
	#define SAMPLE_BUFFLEN			((MAX_SAMPLE_COUNT & ONCE_SAMPLES_ALIGN))
	#define BUFFLEN_BYTES			((SAMPLE_BUFFLEN * ADC_RESULT_BYTE))


	class SignalInfo {
	public:
		float		MeanVolt;
		uint32_t	MaxVal;		// Raw Value from ADC, not true Voltage
		uint32_t	MinVal;		// Raw Value from ADC, not true Voltage
		uint32_t	Freq;
		uint32_t	Period;
		uint32_t	TrigIdx_0;
		uint32_t	TrigIdx_1;

		bool		IsDigital;
		uint32_t	SampleRate = ADC_SAMPLE_RATE;
		uint32_t	SampleNum;
		uint32_t	SampleBuff[SAMPLE_BUFFLEN];

		uint32_t	interpolateSample(float index);
	};



	/* adc.cpp */
	extern SignalInfo CurrentWave;
	extern void config_adc(void);
	extern float to_voltage(uint32_t reading);
	extern void ADC_Sampling(SignalInfo *Wave);

#endif /* _ADC_H_ */