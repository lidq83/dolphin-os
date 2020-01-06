#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "typedef.h"
#include "mm.h"

ErrorStatus HSEStartUpStatus;
USART_InitTypeDef USART_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;

static usb_buff_s *_rx_buff = NULL;
static usb_buff_s *_tx_buff = NULL;

static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len);

extern LINE_CODING linecoding;

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  _rx_buff = malloc(sizeof(usb_buff_s));
  _tx_buff = malloc(sizeof(usb_buff_s));

  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable USB_DISCONNECT GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

  /* Configure USB pull-up pin */
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

  /* Configure the EXTI line 18 connected internally to the USB IP */
  EXTI_ClearITPendingBit(EXTI_Line18);
  EXTI_InitStructure.EXTI_Line = EXTI_Line18;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

int usb_buff_append(usb_buff_s *buf, char ch)
{
  buf->buff[buf->head] = ch;
  buf->head++;
  buf->head %= USB_BUFF_SIZE;
  if (buf->head == buf->foot)
  {
    buf->foot++;
    buf->foot %= USB_BUFF_SIZE;
  }
  return 0;
}

int usb_buff_size(usb_buff_s *buf)
{
  int size = buf->head - buf->foot;
  if (size < 0)
  {
    size += USB_BUFF_SIZE;
  }
  return size;
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

  /* Enable the USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USB Wake-up interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config(FunctionalState NewState)
{
  if (NewState == DISABLE)
  {
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
}

/*******************************************************************************
* Function Name : void USB_Config(void)
* Description   : USBϵͳ��ʼ��
*******************************************************************************/
void USB_Config(void)
{
  Set_System();

  Set_USBClock();

  USB_Interrupts_Config();

  USB_Init();
}

/*******************************************************************************
* Function Name : uint32_t USB_RxRead(uint8_t *buffter, uint32_t buffterSize)
* Description   : ��USB���ջ����ж�����
*******************************************************************************/
uint32_t USB_RxRead(uint8_t *buffter, uint32_t buffterSize)
{
  uint32_t size = usb_buff_size(_rx_buff);
  uint32_t read_cnt = 0;
  for (uint32_t i = 0; i < buffterSize && i < size; i++)
  {
    buffter[i] = _rx_buff->buff[_rx_buff->foot];
    _rx_buff->foot++;
    _rx_buff->foot %= USB_BUFF_SIZE;
    read_cnt++;
  }

  return read_cnt;
}
/*******************************************************************************
* Function Name : uint32_t USB_RxWrite(uint8_t *buffter, uint32_t writeLen)
* Description   : д���ݵ�USB���ջ�����
*******************************************************************************/
uint32_t USB_RxWrite(uint8_t *buffter, uint32_t writeLen)
{
  for (uint32_t i = 0; i < writeLen; i++)
  {
    usb_buff_append(_rx_buff, buffter[i]);
  }
  return writeLen;
}
/*******************************************************************************
* Function Name : uint32_t USB_TxRead(uint8_t *buffter, uint32_t buffterSize)
* Description   : ��USB���ͻ����ж�����
*******************************************************************************/
uint32_t USB_TxRead(uint8_t *buffter, uint32_t buffterSize)
{
  uint32_t size = usb_buff_size(_tx_buff);
  uint32_t read_cnt = 0;
  for (uint32_t i = 0; i < buffterSize && i < size; i++)
  {
    buffter[i] = _tx_buff->buff[_tx_buff->foot];
    _tx_buff->foot++;
    _tx_buff->foot %= USB_BUFF_SIZE;
    read_cnt++;
  }

  return read_cnt;
}
/*******************************************************************************
* Function Name : uint32_t USB_TxWrite(uint8_t *buffter, uint32_t writeLen)
* Description   : д���ݵ�USB���ͻ�����
*******************************************************************************/
uint32_t USB_TxWrite(uint8_t *buffter, uint32_t writeLen)
{
  for (uint32_t i = 0; i < writeLen; i++)
  {
    usb_buff_append(_tx_buff, buffter[i]);
  }
  return 0;
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
*******************************************************************************/
void Get_SerialNum(void)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(uint32_t *)ID1;
  Device_Serial1 = *(uint32_t *)ID2;
  Device_Serial2 = *(uint32_t *)ID3;

  Device_Serial0 += Device_Serial2;

  if (Device_Serial0 != 0)
  {
    IntToUnicode(Device_Serial0, &Virtual_Com_Port_StringSerial[2], 8);
    IntToUnicode(Device_Serial1, &Virtual_Com_Port_StringSerial[18], 4);
  }
}

/*******************************************************************************
* Function Name  : HexToChar.
* Description    : Convert Hex 32Bits value into char.
*******************************************************************************/
static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len)
{
  uint8_t idx = 0;

  for (idx = 0; idx < len; idx++)
  {
    if (((value >> 28)) < 0xA)
    {
      pbuf[2 * idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2 * idx] = (value >> 28) + 'A' - 10;
    }

    value = value << 4;

    pbuf[2 * idx + 1] = 0;
  }
}
