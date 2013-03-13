/*
 * stm32f4_discovery_audio_mic.c
 *
 *  Created on: 12.03.2013
 *      Author: tipok
 */

#include "main.h"
#include "stm32f4_discovery_audio_mic.h"


#ifdef USE_ST_FILTER
#include "pdm_filter.h"
static PDMFilter_InitStruct Filter;
#else
#include "pdm_fir.h"
static struct pdm_fir_filter Filter2;
#endif

/* Private define ------------------------------------------------------------*/
/* SPI Configuration defines */
#define SPI_SCK_PIN                       GPIO_Pin_10
#define SPI_SCK_GPIO_PORT                 GPIOB
#define SPI_SCK_GPIO_CLK                  RCC_AHB1Periph_GPIOB
#define SPI_SCK_SOURCE                    GPIO_PinSource10
#define SPI_SCK_AF                        GPIO_AF_SPI2

#define SPI_MOSI_PIN                      GPIO_Pin_3
#define SPI_MOSI_GPIO_PORT                GPIOC
#define SPI_MOSI_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define SPI_MOSI_SOURCE                   GPIO_PinSource3
#define SPI_MOSI_AF                       GPIO_AF_SPI2

#define AUDIO_MIC_SPI_IRQHANDLER          SPI2_IRQHandler

/* Current state of the audio microphone interface intialization */
static uint32_t AudioMicInited = 0;
/* Audio recording number of channels (1 for Mono or 2 for Stereo) */
static uint32_t AudioRecChnlNbr = 1;
/* Audio recording Samples format (from 8 to 16 bits) */
static uint32_t AudioRecBitRes = 16;
/* Main buffer pointer for the recorded data storing */
uint16_t* pAudioRecBuf_8Khz;
/* Current size of the recorded buffer */
static uint32_t AudioRecCurrSize = 0;

static __IO uint32_t recBuffFillness = 0;
__IO uint32_t Data_Status =0;



#ifdef USE_ST_FILTER
#define INTERNAL_BUFF_SIZE     64
/* Temporary data sample */
static uint16_t InternalBuffer[INTERNAL_BUFF_SIZE];
#else
#define INTERNAL_BUFF_SIZE     8
#endif
static uint32_t InternalBufferSize = 0;



static void MicListener_GPIO_Init(void);
static void MicListener_NVIC_Init(void);
static void MicListener_SPI_Init(uint32_t Freq);

/**
  * @brief  Initialize wave recording
  * @param  AudioFreq: Sampling frequency
  *         BitRes: Audio recording Samples format (from 8 to 16 bits)
  *         ChnlNbr: Number of input microphone channel
  * @retval None
  */
uint32_t MicListenerInit(uint32_t AudioFreq, uint32_t BitRes, uint32_t ChnlNbr)
{
  /* Check if the interface is already initialized */
  if (AudioMicInited)
  {
    /* No need for initialization */
    return 0;
  }
  else
  {
//    /* Enable CRC module */
//    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;

#ifdef USE_ST_FILTER
    /* Filter LP & HP Init */
    //Filter.LP_HZ = 8000;
    Filter.LP_HZ = 4000;
    Filter.HP_HZ = 250;
    Filter.Fs = 16000;
    Filter.Out_MicChannels = 1;
    Filter.In_MicChannels = 1;

    PDM_Filter_Init((PDMFilter_InitStruct *)&Filter);
#else
    pdm_fir_flt_init(&Filter2);
#endif
    /* Configure the GPIOs */
    MicListener_GPIO_Init();

    /* Configure the interrupts (for timer) */
    MicListener_NVIC_Init();

    /* Configure the SPI */
    MicListener_SPI_Init(AudioFreq);

    /* Set the local parameters */
    AudioRecBitRes = BitRes;
    AudioRecChnlNbr = ChnlNbr;

    /* Set state of the audio recorder to initialized */
    AudioMicInited = 1;

    /* Return 0 if all operations are OK */
    return 0;
  }
}

/**
  * @brief  Start audio recording
  * @param  pbuf: pointer to a buffer
  *         size: Buffer size
  * @retval None
  */
uint8_t MicListenerStart(uint16_t* pbuf, uint32_t size)
{
/* Check if the interface has already been initialized */
  if (AudioMicInited)
  {
    /* Store the location and size of the audio buffer */
    pAudioRecBuf_8Khz = pbuf;
    AudioRecCurrSize = size;

    /* Enable the Rx buffer not empty interrupt */
    SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
    /* The Data transfer is performed in the SPI interrupt routine */
    /* Enable the SPI peripheral */
    I2S_Cmd(SPI2, ENABLE);

    /* Return 0 if all operations are OK */
    return 0;
  }
  else
  {
    /* Cannot perform operation */
    return 1;
  }
}

/**
  * @brief  Stop audio recording
  * @param  None
  * @retval None
  */
uint32_t MicListenerStop(void)
{
  /* Check if the interface has already been initialized */
  if (AudioMicInited)
  {

    /* Stop conversion */
    I2S_Cmd(SPI2, DISABLE);

    /* Return 0 if all operations are OK */
    return 0;
  }
  else
  {
    /* Cannot perform operation */
    return 1;
  }
}


/**
  * @brief  This function handles AUDIO_REC_SPI global interrupt request.
  * @param  None
  * @retval None
*/

void AUDIO_MIC_SPI_IRQHANDLER(void)
{
   u16 app;

  /* Check if data are available in SPI Data register */
  if (SPI_GetITStatus(SPI2, SPI_I2S_IT_RXNE) != RESET)
  {
	/* actually, we receiving 64kSamples/s due we dont care about left/right channel. */
    app = SPI_I2S_ReceiveData(SPI2);
#ifdef USE_ST_FILTER
    InternalBuffer[InternalBufferSize++] = HTONS(app);
    InternalBufferSize++;
    /* Check to prevent overflow condition */
    if (InternalBufferSize >= INTERNAL_BUFF_SIZE)
    {
      InternalBufferSize = 0;
      //got 64 samples of 16bit => 16 samples of output
      PDM_Filter_64_LSB((uint8_t *)  InternalBuffer,
    		            (uint16_t *) &pAudioRecBuf_8Khz[PCM_OUT_SIZE*recBuffFillness],
    		            10, (PDMFilter_InitStruct *)&Filter);
      recBuffFillness++;
      if(recBuffFillness >= REC_BUFFERS) {
    	  Data_Status = 1;
    	  recBuffFillness=0;
      }
    }
#else
    //pdm_fir_flt_put(&Filter2, HTONS(app));
    pdm_fir_flt_put(&Filter2, app);
    InternalBufferSize++;
    /* Check to prevent overflow condition */
    if (InternalBufferSize >= INTERNAL_BUFF_SIZE)
    {
      InternalBufferSize = 0;
      pAudioRecBuf_8Khz[recBuffFillness] = pdm_fir_flt_get(&Filter2, 16);
      recBuffFillness++;
      if(recBuffFillness >= REC_BUFFERS * PCM_OUT_SIZE) {
    	  Data_Status = 1;
    	  recBuffFillness=0;
      }
    }
#endif
  }
}




/**
  * @brief  Initialize GPIO for wave recorder.
  * @param  None
  * @retval None
  */
static void MicListener_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(SPI_SCK_GPIO_CLK | SPI_MOSI_GPIO_CLK, ENABLE);

  /* Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(SPI_SCK_GPIO_CLK | SPI_MOSI_GPIO_CLK, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
  GPIO_Init(SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* Connect SPI pins to AF5 */
  GPIO_PinAFConfig(SPI_SCK_GPIO_PORT, SPI_SCK_SOURCE, SPI_SCK_AF);

  /* SPI MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPI_MOSI_PIN;
  GPIO_Init(SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(SPI_MOSI_GPIO_PORT, SPI_MOSI_SOURCE, SPI_MOSI_AF);
}

/**
  * @brief  Initialize SPI peripheral.
  * @param  Freq :Audio frequency
  * @retval None
  */
static void MicListener_SPI_Init(uint32_t Freq)
{
  I2S_InitTypeDef I2S_InitStructure;

  /* Enable the SPI clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);

  /* SPI configuration */
  SPI_I2S_DeInit(SPI2);
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_32k;
  I2S_InitStructure.I2S_Standard = I2S_Standard_LSB;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_High;
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterRx;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
  /* Initialize the I2S peripheral with the structure above */
  I2S_Init(SPI2, &I2S_InitStructure);

  /* Enable the Rx buffer not empty interrupt */
  SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
}


/**
  * @brief  Initialize the NVIC.
  * @param  None
  * @retval None
  */
static void MicListener_NVIC_Init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  /* Configure the SPI interrupt priority */
  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
