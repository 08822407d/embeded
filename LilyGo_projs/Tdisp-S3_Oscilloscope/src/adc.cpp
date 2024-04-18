#include "functions.h"


esp_adc_cal_characteristics_t	adc_chars;

void characterize_adc() {
	esp_adc_cal_characterize(
		ADC_UNIT_1,
		// (adc_atten_t)ADC_CHANNEL,
		ADC_ATTEN_DB_11,
		ADC_WIDTH_BIT_12,
		1100,
		&adc_chars);
}
