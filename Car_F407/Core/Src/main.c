/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <BasicInfor/map.h>
#include "LED/led.h"
#include "Screen/screen.h"
#include "Screen/bmp.h"
#include "Screen/lcd.h"
#include "Screen/image.h"
#include "Car/TB6612.h"
#include "UART/bsp_uart.h"
#include <BasicInfor/tool.h>
#include <Car/speedControl.h>
#include <Screen/oled.h>
#include "BasicInfor/dormancy.h"
#include "BasicInfor/debug.h"
#include "Flash/W25Qxx.h"
#include "FATFS/bsp_fatfs.h"
#include "BasicInfor/statusControl.h"
#include "Balance//control.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Font_Size 24
#define Font_Size_Small 16

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//打点计数 软定时器
uint32_t uwTick_OLED;
uint32_t uwTick_SendData;
uint32_t uwTick_ReceiveData;
//显示缓存
char String_Line[2000];
//数据发送缓存
uint16_t TX_String[40];

//控制速度步进
float Speed_Step = 2.5;
float Direction_Step = 2.5;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void OLED_Proc();
void SendData_Proc();
void ProcessReceivedData();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM14_Init();
    MX_TIM1_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_TIM5_Init();
    MX_ADC1_Init();
    MX_TIM6_Init();
    MX_USART1_UART_Init();
    MX_TIM7_Init();
    MX_TIM13_Init();
    MX_SPI1_Init();
    MX_SPI2_Init();
    MX_TIM8_Init();
    MX_USART2_UART_Init();
    MX_SDIO_SD_Init();
    MX_FATFS_Init();
    MX_I2C1_Init();
    MX_TIM11_Init();
  /* USER CODE BEGIN 2 */

	//初始化呼吸灯 指示运行
	HAL_TIM_Base_Start_IT(&htim14);
	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
	//清空休眠计数
	Clean_Dormancy_Count();
	//初始化文件系统
	Init_FATFS();
	//使能定时器6中断 定期更新速度
	HAL_TIM_Base_Start_IT(&htim6);
	//初始化OLED
	Screen_Init();

	//初始化串口
	Init_USART();

	//初始化小车控制系统
	InitCar();
	//初始化地图
	InitMap();
	//初始化MPU
	InitMPU();
	//定期更新偏航角
	HAL_TIM_Base_Start_IT(&htim11);
	//测试
	//SetMotorSpeed(Motor_3, 100);
	//SetCarSpeed(10, 0, 0);

	LED2_H();
  /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
	while (1)
	{
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
		OLED_Proc();
		SendData_Proc();
	}
    /* USER CODE END 3 */
}

/**
    * @brief System Clock Configuration
    * @retval None
    */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                                            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

//显示屏
void OLED_Proc()
{
	if (uwTick - uwTick_OLED < 500)
	{
		return;
	}
	uwTick_OLED = uwTick;

	sprintf(String_Line, "A:%5.1f   B:%5.1f", Motor_Actual_Speeds[0], Motor_Actual_Speeds[1]);
	Screen_ShowStringLine(0, String_Line, Font_Size);
	sprintf(String_Line, "C:%5.1f   D: %5.1f", Motor_Actual_Speeds[2], Motor_Actual_Speeds[3]);
	Screen_ShowStringLine(1, String_Line, Font_Size);

	sprintf(String_Line, "A:%5.1f   B:%5.1f", Motor_Expected_Speeds[0], Motor_Expected_Speeds[1]);
	Screen_ShowStringLine(3, String_Line, Font_Size);
	sprintf(String_Line, "C:%5.1f   D:%5.1f", Motor_Expected_Speeds[2], Motor_Expected_Speeds[3]);
	Screen_ShowStringLine(4, String_Line, Font_Size);

	sprintf(String_Line, "Yaw:%5.1f", YawAngle);
	Screen_ShowStringLine(6, String_Line, Font_Size);

	UpdateInfraredData();
	sprintf(String_Line, "     %c %c %c %c", Infrared_Datas[Infrared_Right] ? '#' : ' ', Infrared_Datas[Infrared_Center_Right] ? '#' : ' ', Infrared_Datas[Infrared_Center_Left] ? '#' : ' ', Infrared_Datas[Infrared_Left] ? '#' : ' ');
	Screen_ShowStringLine(7, String_Line, Font_Size);

	sprintf(String_Line, "CPU: %.2f C    MPU: %.2f C  ", GetInternalTemperature(), Get_MPU_Temperature());
	Screen_ShowStringLine(13, String_Line, Font_Size_Small);
	sprintf(String_Line, "%.2f V         %d s  ", GetBatteryLevel(), (int) uwTick / 1000);
	Screen_ShowStringLine(14, String_Line, Font_Size_Small);

}

//向上位机发送当前小车数据
void SendData_Proc()
{
	if (uwTick - uwTick_SendData < 500)
	{
		return;
	}
	uwTick_SendData = uwTick;

//发送数据

	TX_String[0] = ((uint16_t) (int16_t) (Motor_Expected_Speeds[0] * 100));
	TX_String[1] = ((uint16_t) (int16_t) (Motor_Expected_Speeds[1] * 100));
	TX_String[2] = ((uint16_t) (int16_t) (Motor_Expected_Axis_Speeds[0] * 100));
	TX_String[3] = ((uint16_t) (int16_t) (Motor_Expected_Axis_Speeds[2] * 100));

	HAL_UART_Transmit(&huart1, (uint8_t*) TX_String, Motor_Number * 2, 0xFFFF);
}

//接收数据
void ProcessReceivedData()
{
	short speed = 0;
	Direction dir;
	uint8_t point = 0;
	if (RxData_Flag[UART_1] == RX_UnRead)
	{
		//清空标志位
		CleanRxData(UART_1);
		//清空休眠计数
		Clean_Dormancy_Count();

		switch (RxData[UART_1][0])
		{
			case 'A':
				switch (RxData[UART_1][1])
				{
					case '1':	//上
						AddCarSpeed(Speed_Step * 2, 0, 0);
						break;
					case '2':	//右上
						AddCarSpeed(Speed_Step, 0, -Direction_Step);
						break;
					case '3':	//右
						AddCarSpeed(0, 0, -Direction_Step * 2);
						break;
					case '4':	//右下
						AddCarSpeed(-Speed_Step, 0, -Direction_Step);

						break;
					case '5':	//下
						AddCarSpeed(-Speed_Step * 2, 0, 0);
						break;
					case '6':	//左下
						AddCarSpeed(-Speed_Step, 0, Direction_Step);
						break;
					case '7':	//左
						AddCarSpeed(0, 0, Direction_Step * 2);
						break;
					case '8':	//左上
						AddCarSpeed(Speed_Step, 0, Direction_Step);
						break;
					case '9':	//停止
						Status_Control_Clear();
						break;
					case '0':	//开始
						Status_Control_Start(Command_Mode_OneTime, 15);
						break;
				}
				break;
			case 'B':

				for (uint8_t i = 0; i < Motor_Number; i++)
				{
					speed = (RxData[UART_1][i * 3 + 2] - '0') * 10 + (RxData[UART_1][i * 3 + 3] - '0');
					if (RxData[UART_1][i * 3 + 1] == '-')
					{
						speed = -speed;
					}
					//设置速度
					//只有关闭巡线才能生效
					Set_Motor_ExpectedSpeed(i, speed);
				}

				break;
			case 'C':
				//设置起点 和 朝向
				dir = RxData[UART_1][1] - '0';
				point = RxData[UART_1][2] - '0';
				SetDefaultValue(dir, point);

				break;
			case 'D':
				//设置终点并开始寻路
				point = RxData[UART_1][1] - '0';
				StartWayFindingAndRuning(point);
				break;
			default:
				break;
		}
	}
}

//定时器中断
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM14)
	{
		//更新呼吸灯 PWM占空比
		Periodic_Update_BreathingLights();
		//接收上位机数据
		ProcessReceivedData();
	}
	else if (htim->Instance == TIM6)
	{
		//每1ms

		//系统控制小车
		Periodic_Status_Control();
		//更新实际值
		//Periodic_Update_Car_ActualSpeed();
		//更新理论值
		//Periodic_UpdateAndSet_Car_ExpectedSpeed();
		if (Is_Car_Runing() == 1)
		{
			//清空休眠计数
			Clean_Dormancy_Count();
		}
	}
	else if (htim->Instance == TIM7)
	{
		//每1s

		//休眠
		Periodic_Dormancy_Counter();
	}
	else if (htim->Instance == TIM11)
	{
		//20ms
		//更新偏航角
		UpdateYawAngle();
	}
}

//外部中断 按键
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//清空休眠计数
	Clean_Dormancy_Count();

	if (GPIO_Pin == Key_Stop_Pin)
	{
		//紧急制动按钮

		SetCarSpeed(0, 0, 0);
	}

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
