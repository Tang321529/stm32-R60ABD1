#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "Key.h"
#include "LED.h"
#include "Buzzer.h"
#include "AD.h"
#include "Timer.h"

uint8_t KeyNum;			//定义用于接收按键键码的变量
uint8_t menu,select_number,alaem_reason;
unsigned char setnum[10]={5,0,1,0,9,0,5,0,4,0};
unsigned char get_setnum[5];
unsigned char getnum[3]={18,60,32};
uint16_t AD0, AD1;	//定义AD值变量
uint16_t Y_Num=2024;
uint8_t Mon_Num=02,D_Num=01,H_Num=59,M_Num=59,S_Num=50;			//定义在定时器中断里自增的变量
uint8_t count_h,count_m=59,count_s=50;

void num_set(void);

void menu_1()
{
	OLED_ShowNum(1,15,1,1);
	OLED_ShowString(2, 15, " ");
		
	OLED_ShowNum(1,4,Y_Num, 4);			//不断刷新显示Num变量
	OLED_ShowString(1, 8, "-");
	OLED_ShowNum(1,9,Mon_Num, 2);			//不断刷新显示Num变量
	OLED_ShowString(1, 11, "-");
	OLED_ShowNum(1,12,D_Num, 2);			//不断刷新显示Num变量
	OLED_ShowNum(2,5,H_Num, 2);			//不断刷新显示Num变量
	OLED_ShowString(2, 7, ":");
	OLED_ShowNum(2,8,M_Num, 2);			//不断刷新显示Num变量
	OLED_ShowString(2, 10, ":");
	OLED_ShowNum(2,11,S_Num, 2);			//不断刷新显示Num变量	
	
	OLED_ShowNum(3,5,count_h, 2);			//不断刷新显示Num变量
	OLED_ShowString(3, 7, ":");
	OLED_ShowNum(3,8,count_m, 2);			//不断刷新显示Num变量
	OLED_ShowString(3, 10, ":");
	OLED_ShowNum(3,11,count_s, 2);			//不断刷新显示Num变量	
	
	OLED_ShowString(1, 1, "   ");
	OLED_ShowString(2, 1, "    ");
	OLED_ShowString(3, 1, "    ");
	OLED_ShowString(4, 1, "   ");	
	
//	AD0 = AD_GetValue(ADC_Channel_0)/35;		//单次启动ADC，转换通道0
//	AD1 = AD_GetValue(ADC_Channel_1)/35;		//单次启动ADC，转换通道1

	if (Serial_GetRxFlag() == 1)	//如果接收到数据包
	{
		
		if(Serial_RxPacket[0] == 0x81)
		{
			if(Serial_RxPacket[1] == 0x02)
			{
				getnum[0]=Serial_RxPacket[4];

			}
		}
		else if(Serial_RxPacket[0] == 0x85)
		{
			if(Serial_RxPacket[1] == 0x02)
			{
				getnum[1]=Serial_RxPacket[4];

			}
		}
		else if(Serial_RxPacket[0] == 0x80)
		{
			if(Serial_RxPacket[1] == 0x03)
			{
				getnum[2]=Serial_RxPacket[4];

			}
		}

	}
	
	get_setnum[0]=setnum[0]*10 + setnum[1];
	get_setnum[1]=setnum[2]*10 + setnum[3];
	get_setnum[2]=setnum[4]*10 + setnum[5];
	get_setnum[3]=setnum[6]*10 + setnum[7];
	get_setnum[4]=setnum[8]*10 + setnum[9];
	
	if(getnum[0] > get_setnum[0] || getnum[0] < get_setnum[1])
	{
		OLED_ShowString(4, 4, "B");
	}
	if(getnum[1] > get_setnum[2] || getnum[1] < get_setnum[3])
	{
		OLED_ShowString(4, 6, "H");
	}
	if(getnum[2] > get_setnum[4])
	{
		OLED_ShowString(4, 8, "A");
	}
	else
	{
		OLED_ShowString(4, 4, "          ");
	}

}
void menu_2()
{
	OLED_ShowNum(1,15,2,1);
	OLED_ShowString(2, 15, " ");
	
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "B:");
	OLED_ShowString(2, 1, "H:");
	OLED_ShowString(3, 1, "A:");

	
	OLED_ShowString(1, 8, "-");
	OLED_ShowString(2, 8, "-");

	
	OLED_ShowString(1, 5, "   ");
	OLED_ShowString(2, 5, "   ");
	OLED_ShowString(3, 5, "   ");

	
	OLED_ShowString(1, 11, "   ");
	OLED_ShowString(2, 11, "   ");
	OLED_ShowString(3, 7, "       ");
	
//	AD0 = AD_GetValue(ADC_Channel_0)/35;		//单次启动ADC，转换通道0
//	AD1 = AD_GetValue(ADC_Channel_1)/35;		//单次启动ADC，转换通道1

	OLED_ShowNum(1,9,AD0,2);
	OLED_ShowNum(2,9,AD1,2);
	
	if (Serial_GetRxFlag() == 1)	//如果接收到数据包
	{
		OLED_ShowHexNum(4, 1, Serial_RxPacket[0], 2);	//显示接收的数据包
		OLED_ShowHexNum(4, 3, Serial_RxPacket[1], 2);
		OLED_ShowHexNum(4, 5, Serial_RxPacket[2], 2);
		OLED_ShowHexNum(4, 7, Serial_RxPacket[3], 2);
		OLED_ShowHexNum(4, 9, Serial_RxPacket[4], 2);
		OLED_ShowHexNum(4, 11, Serial_RxPacket[5], 2);
		
		if(Serial_RxPacket[0] == 0x81)
		{
			if(Serial_RxPacket[1] == 0x02)
			{
				getnum[0]=Serial_RxPacket[4];
				OLED_ShowNum(1, 3,getnum[0], 2);
			}
		}
		else if(Serial_RxPacket[0] == 0x85)
		{
			if(Serial_RxPacket[1] == 0x02)
			{
				getnum[1]=Serial_RxPacket[4];
				OLED_ShowNum(2, 3,getnum[1], 2);
			}
		}
		else if(Serial_RxPacket[0] == 0x80)
		{
			if(Serial_RxPacket[1] == 0x03)
			{
				getnum[2]=Serial_RxPacket[4];
				OLED_ShowNum(3, 3,getnum[2], 2);
			}
		}

	}
	
//	if(getnum[2]>setnum[2])
//	{alaem_reason=0;Buzzer_ON();Delay_ms(500);Buzzer_OFF();}
//	else
//	{Buzzer_OFF();}


}
void menu_3()
{
	OLED_ShowNum(1,15,3,1);

	OLED_ShowNum(2,15,select_number,1);
	
	switch(select_number)
	{
		case 0: OLED_ShowString(1,3," ");Delay_ms(100); break;
		case 1: OLED_ShowString(1,4," ");Delay_ms(100); break;
		case 2: OLED_ShowString(1,6," ");Delay_ms(100); break;
		case 3: OLED_ShowString(1,7," ");Delay_ms(100); break;
		case 4: OLED_ShowString(2,3," ");Delay_ms(100); break;
		case 5: OLED_ShowString(2,4," ");Delay_ms(100); break;
		case 6: OLED_ShowString(2,6," ");Delay_ms(100); break;
		case 7: OLED_ShowString(2,7," ");Delay_ms(100); break;
		case 8: OLED_ShowString(3,3," ");Delay_ms(100); break;
		case 9: OLED_ShowString(3,4," ");Delay_ms(100); break;
	}	
	
	OLED_ShowString(1, 1, "B:");
	OLED_ShowString(2, 1, "H:");
	OLED_ShowString(3, 1, "A:");

	OLED_ShowNum(1,3,setnum[0],1);
	OLED_ShowNum(1,4,setnum[1],1);
	OLED_ShowString(1, 5, "-");
	OLED_ShowNum(1,6,setnum[2],1);
	OLED_ShowNum(1,7,setnum[3],1);
	
	OLED_ShowNum(2,3,setnum[4],1);
	OLED_ShowNum(2,4,setnum[5],1);
	OLED_ShowString(2, 5, "-");
	OLED_ShowNum(2,6,setnum[6],1);
	OLED_ShowNum(2,7,setnum[7],1);
	
	OLED_ShowNum(3,3,setnum[8],1);
	OLED_ShowNum(3,4,setnum[9],1);
	
	OLED_ShowString(1, 8, "   ");
	OLED_ShowString(2, 8, "   ");	
	OLED_ShowString(3, 8, "   ");
	OLED_ShowString(4, 1, "            ");

	num_set();

}
void num_set()
{
	if(KeyNum==2)
	{
		select_number++;

		if(select_number==10){select_number=0;}
		
	}
	if(KeyNum==3)
	{
		setnum[select_number]++;
		if(setnum[select_number]==10){setnum[select_number]=0;}
	}
	if(KeyNum==4)
	{
		setnum[select_number]--;
		if(setnum[select_number]==0){setnum[select_number]=9;}
	}	
}

void Key_Service()
{
	switch(KeyNum)
	{
//		case 1: at24c02_init(); break;
		case 1: menu++; if(menu==3){ menu=0;}; break;
//		case 3: menu=0; break;
//		case 4: menu=2; break;
		default: break;
	}
}

void Lcd_Show()
{
	switch(menu)
	{
		case 0: menu_1(); break;
		case 1: menu_2(); break;
		case 2: menu_3(); break;
   	default: break;
	}
}


int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Key_Init();			//按键初始化
	Serial_Init();		//串口初始化
	AD_Init();					//AD初始化
	Timer_Init();
	Buzzer_Init();
	
	while (1)
	{
		KeyNum = Key_GetNum();			//获取按键键码
		Key_Service();
		Lcd_Show();
	}

}

/**
  * 函    数：TIM2中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
	{
		S_Num ++;												//Num变量自增，用于测试定时中断
		if(S_Num==60){M_Num++;S_Num=0;}
		if(M_Num==60){H_Num++;M_Num=0;}
		if(H_Num==60){H_Num=0;D_Num++;}
		if(D_Num==30){D_Num=0;Mon_Num++;}
		
		if(AD0>60)
		{
			count_s++;
			if(count_s==60){count_s=0;count_m++;}
			if(count_m==60){count_m=0;count_h++;}
		}
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
															//中断标志位必须清除
															//否则中断将连续不断地触发，导致主程序卡死
	}
}
