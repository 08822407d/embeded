#define Enable_longPress_Continuous
#define SIGNAL_INPUT_PIN		1
#define SCREEN_ROTAION			3
#define ADC_SAMPLE_RATE			80000
// #define ADC_SAMPLE_RATE			SOC_ADC_SAMPLE_FREQ_THRES_HIGH
#define SAMPLE_BUFFLEN_BASE		256U
#define SAMPLE_BUFFLEN			((4 * SAMPLE_BUFFLEN_BASE))

#define ADC_READ_MAX_VAL		4095
// #define ADC_READ_MAX_VAL		8191
#define ADC_VOLTREAD_CAP		3100


#ifdef AMOLED
#  define SPR_TXT_SIZE			2
#  define GRID_SIZE				60
#else
#  define SPR_TXT_SIZE			1
#  define GRID_SIZE				40
#endif


#define BG_DARK_GRAY	        0x2104
#define BACK_GROUND_COLOR		BG_DARK_GRAY