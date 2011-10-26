/**************************************************************
**模块名:EDIT.C
**功能:EDIT模块
**更新记录:支持BCD形式,整数,小数点1,2位,负数,负数小数1,2位  注意实际上记录数据的方式是整形数, 
** 			也就是显示小数1位模式的EDIT,显示1,1.2,分别对应实际值10,12
** 			EDIT是所有控件中最复杂的一个,之后还会继续改进一些BUG.
***************************************************************/

#include "stm32f10x.h"
#include "GuiDef.h"
#include "widget.h"
#include "window.h"

/**************************************************************
**函数名:__SetOperationalWidget(09.8.5)
**功能:获取EDIT的各个常用属性,并赋值到全局变量中,使得之后的操作都针对这个EDIT
**注意事项:仅仅用于提高效率和减少代码长度之用,注意到基本EDIT操作之前,必须调用此函数获取信息 
***************************************************************/
static BPRO __base_property_edit_type;	//normal,1p,2p,pw
static BPRO __base_property_edit_style;	//3d,2d,null
static SPRO __self_property_edit_state;	//null,0p,1p,2p

//bcd_len 								//BCD长度,如果没定长限制,则此值为0
static u8 __font_height;				//字高

static COLOR __color_string;

static void __SetOperationalWidget(DX_EDIT* the_edit)
{
	__base_property_edit_type=the_edit->widget.base_property&BASE_PRO_EDIT_TYPE_MASK;	  	//normal,1p,2p,pw
	__base_property_edit_style=the_edit->widget.base_property&BASE_PRO_EDIT_STYLE_MASK;		//3d 2d null
	__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;		//null,0p,1p,2p
	
	if((the_edit->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT16)
	{
		__font_height=16;
	}
	else if((the_edit->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT24)
	{
		__font_height=24;
	}
	else if((the_edit->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT32)
		__font_height=32;

	//获取背景色
	if((*(the_edit->widget.p_ext_property))&EXT_PRO_FLASH)		//&&(闪烁标志有效)	//闪烁态
	{
		BG_COLOR=COLOR_FLASH;
	}
	else if((*(the_edit->widget.p_ext_property))&EXT_PRO_FOCUS)	//焦点
	{
		BG_COLOR=COLOR_FOCUS;
	}
	else if((*(the_edit->widget.p_ext_property))&EXT_PRO_DISABLE)	//无效
	{
		BG_COLOR=COLOR_DISABLE;
	}
	else									//静态
	{
		if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_3D)			//3D风格用自己规定的底色-SYS色
		    BG_COLOR=COLOR_SYS_FILL_RECT;
		else if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_2D)
		{	//2D底色使用color_2d
			//BG_COLOR=the_edit->color2d;
		}
		else
			BG_COLOR=the_edit->widget.far_win->widget.color;			//NULL则用窗口底色
	}

	//设置字体颜色
	if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_3D)
		__color_string=BLACK;
	else 
		__color_string=the_edit->widget.color;				
	
}

/**************************************************************
** 函数名:__DebugChecking
** 功能:调试模式下检查EDIT是否合理,不合理的一律while(1)死等,用于静态创建的时候测试用户设置问题
** 注意事项:根据DEBUG_GUI宏定义来决定是否编译
***************************************************************/
#ifdef DEBUG_GUI

static void __DebugChecking(DX_EDIT *the_edit)
{
	if(the_edit->p_tmp==NULL)
	{
		gui_asssert_failed(0);
		while(1);
	}
	if((the_edit->widget.x<MAX_X)&&(the_edit->widget.y<MAX_Y))
	{
		return;
	}
	else
	{
		gui_asssert_failed(0);
		while(1);
	}
}
#else

#define __DebugChecking(x) 

#endif

/**************************************************************
** 函数名:__TFTShowCursor
** 功能:显示一个光标
** 注意事项:TRUE为显示,FLASE则显示底色
***************************************************************/
void __TFTShowCursor(LOC_X x,LOC_Y y,u8 len,bool flash)
{
	COLOR color;
	if(flash==TRUE)
		color=__color_string;
	else
		color=BG_COLOR;
	TFTVLine(x,y,len,color);
}

/**************************************************************
** 函数名:__DrawFrame(09.8.5)
** 功能:画EDIT外框
** 注意事项:之前需要调用过__GetEditPro()
***************************************************************/
static void __DrawFrame(DX_EDIT *the_edit)
{
	///如果是隐形,则不需要画框
	if((*(the_edit->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}

	if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_3D)//3D风格
	{
		__Draw3DFrameA(the_edit->widget.x,the_edit->widget.y,the_edit->widget.length,the_edit->widget.height) ;
	}
	else if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_2D)//2D风格
	{
		TFTRect(the_edit->widget.x,the_edit->widget.y,the_edit->widget.length,the_edit->widget.height,COLOR_FRAME_2D);
	}
	else if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_NULL)//NULL风格-
	{

	}
	else if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_GLASS)//玻璃风格*暂时比较复杂,预留
	{
		//没有框
	}
}
/**************************************************************
** 函数名:__FixEditState(09.8.5)
** 功能:使EDIT从编辑态转化为完全态
** 调用:在键盘按下ENTER,焦点离开,EditDraw的时候调用
** 注意事项:在这里会判断最小值,调用前需调用__GetEditPro
**  返回TRUE就是有修正,FLASH就是不需要修正(有修正其实就是之后需要重绘)
**  是当前的在显示焦点EDIT才会更新Focus_Cursor_State
**  如果之前Focus_Cursor_State是ALL状态,则不更新Focus_Cursor_State
**  
**  *_*考虑增加EDIT框长度的限制,使得显示值不会超过框长
***************************************************************/
bool __FixEditState(DX_EDIT* the_edit)
{
	s32 *ptmp;
	s32 tmp;

	//当前焦点EDIT,则清除光标位
	if((the_edit->widget.far_win==pNow_Window)&&((*(the_edit->widget.p_ext_property))&EXT_PRO_FOCUS))
	{
		Focus_Cursor_State=FOCUS_CURSOR_ALL;
	}

	ptmp=the_edit->p_tmp;
	
	//1P属性非1P状态
	if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_1P)
	{
		if( __self_property_edit_state!=SELF_PRO_EDIT_STATE_1P)
		{
			tmp=(*ptmp)*10;
			//判断最大值
			if(tmp<=(the_edit->max_data))
			{
				//判断最小值
				if(tmp>=(the_edit->min_data))
				{
					*ptmp=tmp;
				}
				else
					*ptmp=the_edit->min_data;
			}
			else
			{
				*ptmp=the_edit->max_data;
			}

			*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
			*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_1P;
			__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
			return TRUE;
		}
	}
	//2P属性
	else if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_2P)
	{
		//1P状态
		if( __self_property_edit_state==SELF_PRO_EDIT_STATE_1P)
		{
			tmp=(*ptmp)*10;
			if(tmp<=(the_edit->max_data))
			{
				*ptmp=tmp;
			}
			else
			{
				*ptmp=the_edit->max_data;
			}
			*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
			*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_2P;
			__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
			return TRUE;
		}
		//NULL或0P状态
		else if( __self_property_edit_state!=SELF_PRO_EDIT_STATE_2P)
		{
			tmp=(*ptmp)*100;
			if(tmp<=(the_edit->max_data))
			{
				*ptmp=tmp;
			}
			else
			{
				*ptmp=the_edit->max_data;
			}
			*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
			*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_2P;
			__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
			return TRUE;
		}
	}
	//已经是完全态,则判断大小是否合理
	//判断最大值
	if(*ptmp<=(the_edit->max_data))
	{
		//判断最小值
		if(*ptmp<(the_edit->min_data))
		{	
			*ptmp=the_edit->min_data;
			return TRUE;
		}
	}
	else
	{
		*ptmp=the_edit->max_data;
		return FALSE;
	}
	return FALSE;
}
/**************************************************************
** 函数 :__Draw
** 功能:刷新EDIT空间中的数字,这里还包括刷新背景色
** 注意事项:  
**  		支持负数显示(09.7.10)
**  
***************************************************************/
static void __Draw(DX_EDIT* the_edit)
{
	LOC_X x_str;
	LOC_Y y_str;
	u8 str_length;
	char str_buf[15+10];//最大的情况8位数字+1位小数点+'\0',预留填充0,共15位
	char *str=&str_buf[15];
	u8 i,j;
	
	///如果是隐形,则需要用父窗口背景色填充
	if((*(the_edit->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		TFTRectFill(the_edit->widget.x,the_edit->widget.y,the_edit->widget.length,the_edit->widget.height,(the_edit->widget.far_win)->widget.color);
		return;
	}

	str_length=__font_height/2;

	

	//根据风格定位字符显示的位置(主要定位X位置) 并刷掉数字部分
	if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_3D)			//3D风格
	{	
		x_str=the_edit->widget.x +4;
		TFTRectFill(the_edit->widget.x+2,the_edit->widget.y+2,the_edit->widget.length-4,the_edit->widget.height-4,BG_COLOR);
	}
	else if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_2D)	//2D风格
	{	
		x_str=the_edit->widget.x +2;
		//即使BCD风格,也建议把矩形填充掉
		//if((the_edit->widget.base_property&BASE_PRO_EDIT_BCD_MASK)==BASE_PRO_EDIT_BCD_NULL)
			TFTRectFill(the_edit->widget.x+2,the_edit->widget.y+2,the_edit->widget.length-4,the_edit->widget.height-4,BG_COLOR);
	}
	else if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_NULL)	//NULL风格-
	{	
		x_str=the_edit->widget.x;
		if((the_edit->widget.base_property&BASE_PRO_EDIT_BCD_MASK)==BASE_PRO_EDIT_BCD_NULL)
			TFTRectFill(the_edit->widget.x,the_edit->widget.y,the_edit->widget.length,the_edit->widget.height,BG_COLOR);
	}
	else if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_GLASS)	//玻璃风格--保留
	{
		x_str=the_edit->widget.x;
	}
	
	y_str=GetStringLocYFromFRAME(the_edit->widget.y,the_edit->widget.height,str_length*2);
	

	//如果非密码
	if(__base_property_edit_type!=BASE_PRO_EDIT_TYPE_PW)
	{
		j=GET_EDIT_BCD_LEN(the_edit);//*_*可以归到__GetEditPro中获得
		//非8进制模式
		if(__base_property_edit_type!=BASE_PRO_EDIT_TYPE_O8)
		{
			i=NumToString(*(the_edit->p_tmp),str);
			//1位小数
			if(__self_property_edit_state==SELF_PRO_EDIT_STATE_1P)
			{
				if(i==1)	//0.X的格式必然固定.
				{
					str[3]='\0';
					str[2]=str[0];
					str[1]='.';
					str[0]='0';
					if(j>1)
						j-=2;
					else
						j=0;
				}
				else
				{
					str[i+1]='\0';
					str[i]=str[i-1];
					str[i-1]='.';
					if(j>i)
						j-=i;
					else 
						j=0;
				}
				
			}
			//2位小数
			else if(__self_property_edit_state==SELF_PRO_EDIT_STATE_2P)
			{
				str[i+2]='\0';
				str[i+1]=str[i-1];
				str[i]=str[i-2];
				str[i-2]='.';
				if(i>3)
				{
					if(j>3)
						j-=i;
					else
						j=0;
				}
				else
				{
					if(j>3)
						j-=3;
					else
						j=0;
				}
			}
			//半位小数
			else if(__self_property_edit_state==SELF_PRO_EDIT_STATE_0P)
			{
				str[i+1]='\0';
				str[i]='.';
				j=0;	//*_*暂时先不处理这个
			}
			//整数
			else
			{
				if(j>=i)
					j-=i;
			}
		}
		else//8进制
		{
			i=NumToStringO8(*(the_edit->p_tmp),str);
			if(j>=i)
				j-=i;
		}
		
		//根据J值(BCD长度)补0
		i=0;
		if(j)
		{
			for(;j!=0;j--)
			{
				str_buf[15-j]='0';
				i++;
			}
			str=&(str_buf[15-i]);
		}

		//负数前要加负号
		if((*(the_edit->p_tmp))<0)
		{
			i++;
			str_buf[15-i]='-';
			str=&(str_buf[15-i]);
		}
		if(__font_height==16)
		{
			
			if((*(the_edit->widget.p_ext_property))&EXT_PRO_DISABLE)
			{
				TFTShowString16(x_str+1,y_str+1,str,COLOR_STRING_DISABLE1,0);
				TFTShowString16(x_str,y_str,str,COLOR_STRING_DISABLE2,1);
			}
			else
			{
				TFTShowString16(x_str,y_str,str,__color_string,0);
			} 
			 
		}
		if(__font_height==24)
		{
			if((*(the_edit->widget.p_ext_property))&EXT_PRO_DISABLE)
			{
				TFTShowString24(x_str+1,y_str+1,str,COLOR_STRING_DISABLE1,0);
				TFTShowString24(x_str,y_str,str,COLOR_STRING_DISABLE2,1);
			}
			else
			{
				TFTShowString24(x_str,y_str,str,__color_string,0);
			}
		}
		else if(__font_height==32)   
		{
			/*
			if((*(the_edit->widget.p_ext_property))&EXT_PRO_DISABLE)
			{
				TFTShowString32(x_str+1,y_str+1,str,COLOR_STRING_DISABLE1,0);
				TFTShowString32(x_str,y_str,str,COLOR_STRING_DISABLE2,1);
			}
			else
			{
				TFTShowString32(x_str,y_str,str,__color_string,0);
			}
			*/
		}
	}
	//密码
	else
	{
		//密码模式中,密码位数放在self_pro中
		i=(*(the_edit->p_self_property)&SELF_PRO_EDIT_PASS_NUM_MASK);
		str[i]='\0';
		for(;i>0;i--)
			str[i-1]='*';
		if(__font_height==16)
		{
			
			if((*(the_edit->widget.p_ext_property))&EXT_PRO_DISABLE)
			{
				TFTShowString16(x_str+1,y_str+1,str,COLOR_STRING_DISABLE1,0);
				TFTShowString16(x_str,y_str,str,COLOR_STRING_DISABLE2,1);
			}
			else
			{
				TFTShowString16(x_str,y_str,str,__color_string,0);
			} 
			
		}
		if(__font_height==24)
		{
			if((*(the_edit->widget.p_ext_property))&EXT_PRO_DISABLE)
			{
				TFTShowString24(x_str+1,y_str+1,str,COLOR_STRING_DISABLE1,0);
				TFTShowString24(x_str,y_str,str,COLOR_STRING_DISABLE2,1);
			}
			else
			{
				TFTShowString24(x_str,y_str,str,__color_string,0);
			}
		}
		else if(__font_height==32)
		{
			/*
			if((*(the_edit->widget.p_ext_property))&EXT_PRO_DISABLE)
			{
				TFTShowString32(x_str+1,y_str+1,str,COLOR_STRING_DISABLE1,0);
				TFTShowString32(x_str,y_str,str,COLOR_STRING_DISABLE2,1);
			}
			else
			{
				TFTShowString32(x_str,y_str,str,__color_string,0);
			}
			*/
		}
	}
}
/**************************************************************
** 函数名:ReDrawEdit
** 功能:重绘EDIT
** 注意事项:用于一些刷新的重绘,不改变控件自身任何标志
**  			*_*EDIT比较复杂,还要根据实际使用情况再考虑改进
***************************************************************/
void ReDrawEdit(DX_EDIT* the_edit)
{
	//0.获取属性
	__SetOperationalWidget(the_edit);
	//1.画外框
	__DrawFrame(the_edit);
	//2.重绘内容
	__Draw(the_edit);
}

/**************************************************************
** 函数名:GuiEditDraw
** 功能:画EDIT
** 注意事项:此函数仅仅用于WINDOW出现的时候显示控件,不作重绘用 
** 			EDIT是编辑数字的控件,带有内部的光标状态. 
***************************************************************/
void GuiEditDraw(DX_EDIT* the_edit)
{
	__DebugChecking(the_edit);

	//0.获取属性
	__SetOperationalWidget(the_edit);
	//1.完整性分析
	__DebugChecking(the_edit);
	//2.HIDE属性处理
	if( (*(the_edit->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}

	//3.内部处理
	__FixEditState(the_edit);

	//4.画外框
	__DrawFrame(the_edit);
	
	//5.重绘
	__Draw(the_edit);
}

/**************************************************************
** 函数名:SetEditTmp/GetEditTmp
** 功能:设置EDIT控件的显示值
** 注意事项:此函数是外部调用函数,一般情况下,EDIT控件都不允许外部直接对其TMP值进行改动,应该通过此函数设置 
**  		
**  		本GUI规定,带小数点属性的EDIT,除了编辑态,其余情况一律以小数形式显示,也就是1P属性的,TMP=1就代表是0.1.TMP=10就代表1.0
**  		10.2.25:密码模式下,改变值则改变密码位长
***************************************************************/
void SetEditTmp(DX_EDIT* the_edit,s32 tmp)
{
	u8 len;

	__SetOperationalWidget(the_edit);

	if(the_edit->p_tmp!=NULL)
	{
		*(the_edit->p_tmp)=tmp;
		//密码模式
		if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_PW)
		{
			//清长度
			(*(the_edit->p_self_property))&=~SELF_PRO_EDIT_PASS_NUM_MASK;
			//如果tmp为非0,则设置新长度
			if(tmp)
			{
				len=GetNumLen(tmp);
				(*(the_edit->p_self_property))|=len;
			}
		}
	}
	

	//如果是当前显示的且非隐身,则刷新内容
	if((IsWidgetAvailable((DX_WIDGET*)the_edit)))
	{
		/*更新TMP值,则参数tmp必然为完整态时候的值,例如tmp=50,edit是1P属性,则实际上显示为5.0, 
		  所以这里会人为修正self_property_edit_state为完整态.
		*/ 
		
		if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_1P)
		{
			*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
			*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_1P;
			__self_property_edit_state=SELF_PRO_EDIT_STATE_1P;
		}
		else if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_2P)
		{
			*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
			*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_2P;
			__self_property_edit_state=SELF_PRO_EDIT_STATE_2P;
		} 
		
		//2.修正STATE-当EDIT还不是完全态的时候,调用__FixEditState会因为本来tmp 值是1.0却最后显示10.0的情况
		// 所以在设置TMP的时候不能调用__FixEditState
		//__FixEditState(the_edit);
		__Draw(the_edit);
	}
}

s32 GetEditTmp(DX_EDIT* the_edit)
{
	if(the_edit!=NULL)
	{
		return *(the_edit->p_tmp);
	}
	else 
		return 0;
}

/**************************************************************
** 函数名:UpdateEdit()
** 功能:tmp值和data值的相互转换
** 注意事项:TRUE则data->tmp,并刷新;FALSE tmp=>data 
** 		如果data指针为NULL,则不作更新,如果tmp为NULL,基本不太可能
***************************************************************/
void UpdateEdit(DX_EDIT* the_edit,bool dir)
{
	if(dir==TRUE)
	{
		if(the_edit->p_data!=NULL)
		{
			SetEditTmp(the_edit,*(the_edit->p_data));
		}
	}
	else
	{
		if(the_edit->p_data!=NULL)
		{
			*(the_edit->p_data)=*(the_edit->p_tmp);
		}
	}
}

/**************************************************************
** 函数名:KeyToChangeEdit
** 功能:键盘引入的信息对EDIT数值的影响
** 注意事项:
**  		此函数会改变 Focus_Edit_Cursor_State的值,并调用刷新重绘控件.*_*,可以考虑不在此函数里重绘控件.
**  		返回控件信息,注意如果数值没有改变,则不返回有效消息(此功能暂定)
**  		注意KeyToChangeXX系列,都不应对方向键有响应.方向键的响应又系统处理
***************************************************************/
MESSAGE_WIDGET KeyToChangeEdit(DX_EDIT* the_edit,GUIKEY key)
{
	bool reflash=FALSE;
	MESSAGE_WIDGET msg_widget=MESSAGE_NULL;	//控件消息
	s32* ptmp;
	s32 tmp, tmp2;
	
	__SetOperationalWidget(the_edit);
	ptmp=the_edit->p_tmp;//获取TMP指针
	
	//1.输入数字,
	// a.如果光标状态为满,则数值直接清0并显示为键盘值
	// b.注意判断是否大于最大值,如输入后会大于最大,则采取数值不变原则
	// c.如果清0,则清小数标志位
	// d.如果是密码模式,则和0P模式类似,但要注意改变self_pro的NUM位
	if(/*(key>=KEY_TO_GUI_0)&&*/(key<=KEY_TO_GUI_9))
	{
		//8进制8,9两个键直接忽略
		if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_O8)
		{
			if((key==KEY_TO_GUI_8)||(key==KEY_TO_GUI_9))
			{
				reflash=TRUE;
				goto end_key_num;
			}
		}
		//a.光标为满
		if(Focus_Cursor_State==FOCUS_CURSOR_ALL)
		{
			tmp2=tmp=key;
			Focus_Cursor_State=0;
			reflash=TRUE;
			//c.
			if( (__base_property_edit_type==BASE_PRO_EDIT_TYPE_1P)||(__base_property_edit_type==BASE_PRO_EDIT_TYPE_2P))
			{
				*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
				//*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_NULL;//这个可省了
				if(__base_property_edit_type!=BASE_PRO_EDIT_TYPE_PW)//非密码模式,则清空P各位
				{
					__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
				}
				else//密码模式,则清空NUM各位
				{
					*(the_edit->p_self_property)=(*(the_edit->p_self_property))&SELF_PRO_EDIT_PASS_NUM_MASK;
				}
			}
		}
		//1P属性,1P状态
		else if((__base_property_edit_type==BASE_PRO_EDIT_TYPE_1P)&&(__self_property_edit_state==SELF_PRO_EDIT_STATE_1P))
		{
			return MESSAGE_NULL;
		}
		//2P属性,2P状态
		else if((__base_property_edit_type==BASE_PRO_EDIT_TYPE_2P)&&(__self_property_edit_state==SELF_PRO_EDIT_STATE_2P))
		{
			return MESSAGE_NULL;
		}
		else//其他的可输入数字的状态(包括密码)
		{
			if(__base_property_edit_type!=BASE_PRO_EDIT_TYPE_PW)
				if((*ptmp==0)&&(__self_property_edit_state==SELF_PRO_EDIT_STATE_NULL))
					Focus_Cursor_State--;
			if(__base_property_edit_type!=BASE_PRO_EDIT_TYPE_O8)//非8进制
			{
				tmp2=tmp=*ptmp*10+key;
			}
			else
			{
				tmp2=tmp=*ptmp*8+key;
			}
		}

		
		//1P如果只有整数,最大值比较时要整数部分乘以10.
		if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_1P)
		{
			if(__self_property_edit_state==SELF_PRO_EDIT_STATE_NULL)
			{
				tmp2=tmp*10;
			}
		}
		//2P如果只有整数,比较时要*100,其他则*10
		else if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_2P)
		{
			//整数
			if(__self_property_edit_state==SELF_PRO_EDIT_STATE_NULL)
			{
				tmp2=tmp*100;
			}
			//0/1P
			else if((__self_property_edit_state==SELF_PRO_EDIT_STATE_0P)||(__self_property_edit_state==SELF_PRO_EDIT_STATE_1P))
			{
				tmp2=tmp*10;
			}
		}
		

		//最大值的限制--(密码模式也有最大值的限制,这样方便限制位数)
		if(tmp2<=the_edit->max_data)
		{
			*ptmp=tmp;
			Focus_Cursor_State++;
			reflash=TRUE;
			//如果是密码
			if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_PW)
			{
				tmp=GetNumLen(tmp);
				if(((*(the_edit->p_self_property))&SELF_PRO_EDIT_PASS_NUM_MASK)!=tmp)
					*(the_edit->p_self_property)=tmp;//这里简略了一些保护机制,直接赋值
				else
				{
					reflash=FALSE;
					goto end_key_num;
				}
			/*
				//NUM值加1的可能性:没大于最大值,原值非0并且非按下0
				if((*ptmp!=0)&&(key!=KEY_TO_GUI_0))
				{
					*(the_edit->p_self_property)=((*(the_edit->p_self_property))+1)&SELF_PRO_EDIT_PASS_NUM_MASK;
				}
				//或者是0但位数为0,则位数变1*_*按0也有*号,这个取消了,因为先按0,再按1的情况不好弄
			//	else if(((*(the_edit->p_self_property))&SELF_PRO_EDIT_PASS_NUM_MASK)==SELF_PRO_EDIT_PASS_NUM0)
			//	{
			//		*(the_edit->p_self_property)=((*(the_edit->p_self_property))+1)&SELF_PRO_EDIT_PASS_NUM_MASK;
			//	}
				else
				{
					reflash=FALSE;
					goto end_key_num;
				}
			*/
			}
		}
		else
		{
			//采取不变原则
			reflash=FALSE;
			goto end_key_num;
		}
		//非密码下,小数点的变化
		if(__base_property_edit_type!=BASE_PRO_EDIT_TYPE_PW)
		{
			//0P变1P
			if(__self_property_edit_state==SELF_PRO_EDIT_STATE_0P)
			{
				*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
				*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_1P;
				__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
			}
			//1P并且有2P属性则变2P
			else if((__base_property_edit_type==BASE_PRO_EDIT_TYPE_2P)&&(__self_property_edit_state==SELF_PRO_EDIT_STATE_1P))
			{
				*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
				*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_2P;
				__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
			}
		}
	end_key_num:
		msg_widget=MESSAGE_WIDGET_EDIT;
	}
	//2.输入小数点
	// a.如果BASE模式允许输入小数,但光标状态为满,则数值直接清0并加上一个小数点
	// b.只有BASE属性允许输入小数并且SELF属性是NULL才可以画上一个小数点(0P)
	// c.密码模式不允许输入小数
	else if(key==KEY_TO_GUI_POINT)
	{
		if((__base_property_edit_type==BASE_PRO_EDIT_TYPE_1P)||(__base_property_edit_type==BASE_PRO_EDIT_TYPE_2P))
		{
			//a.
			if(Focus_Cursor_State==FOCUS_CURSOR_ALL)
			{
				*ptmp=0;
				Focus_Cursor_State=1;
				*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
				//*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_NULL;//这个可省了
				__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
				reflash=TRUE;
				msg_widget=MESSAGE_WIDGET_EDIT;
			}
			if(__self_property_edit_state==SELF_PRO_EDIT_STATE_NULL)
			{
				//*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;//这个可以省了
				*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_0P;
				__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
				Focus_Cursor_State++;
				reflash=TRUE;
				msg_widget=MESSAGE_WIDGET_EDIT;
			}
		}
	}
	//3.输入退格
	// a.如果已经是0,则不变,如果是 FOCUS_CURSOR_ALL.则清0
	// b.如果没有小数点在,则退格
	// b.SELF中如果是2P,则变1P,如果是1P,则变0P,如果是0P,则变NULL
	else if(key==KEY_TO_GUI_BACK)
	{
		//密码模式
		if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_PW)
		{
			//如果是FOCUS_CURSOR_ALL状态,则归0
			if(Focus_Cursor_State==FOCUS_CURSOR_ALL)
			{
				*ptmp=0;
				*(the_edit->p_self_property)&=~SELF_PRO_EDIT_PASS_NUM_MASK;
			}
			//密码位数非0,都可以减1
			else if((*(the_edit->p_self_property))&SELF_PRO_EDIT_PASS_NUM_MASK)
			{
				(*(the_edit->p_self_property))--;
				*ptmp/=10;
				Focus_Cursor_State--;
			}
			else
			{
				return MESSAGE_NULL;
			}
		}
		else
		{
			//a.只有一个0
			if((*ptmp==0)&&(__self_property_edit_state==SELF_PRO_EDIT_STATE_NULL))
			{
				return MESSAGE_NULL;
			}
			//FOCUS_CURSOR_ALL则全清
			if(Focus_Cursor_State==FOCUS_CURSOR_ALL)
			{
				*ptmp=0;
				Focus_Cursor_State=1;
				*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
				//*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_NULL;//这个可省了
				__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
				msg_widget=MESSAGE_WIDGET_EDIT;
				reflash=TRUE;
			}

			//8进制
			if(__base_property_edit_type!=BASE_PRO_EDIT_TYPE_O8)
			{
				tmp=*ptmp/8;
			}
			//c. 0P->NULL(0.->0)
			if(__self_property_edit_state==SELF_PRO_EDIT_STATE_0P)
			{
				*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
				//*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_NULL;//这个可省了
				__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
			}
			else
			{	
				//c. 1P->0P(0.1->0.)
				if(__self_property_edit_state==SELF_PRO_EDIT_STATE_1P)
				{
					*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
					*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_0P;
					__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
				}
				//c. 2P->1P(0.12->0.1)
				else if(__self_property_edit_state==SELF_PRO_EDIT_STATE_2P)
				{
					*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
					*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_1P;
					__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
				}

				//b.以下处理还包括NULL的情况
				tmp=*ptmp/10;
				*ptmp=tmp;
			}
			Focus_Cursor_State--;
			if(Focus_Cursor_State==0)
				Focus_Cursor_State=1;
		}
		reflash=TRUE;
		msg_widget=MESSAGE_WIDGET_EDIT;
	}
	//4.确认,-确认键需要迎合小数点的显示原则.参考SetEditTmp
	// a.修正小数问题
	// b.如果非密码,则判断最小值,大于最小值,则置一个最小值,并改长度,暂时不用判断最大值
	// d.如果是密码,...
	else if(key==KEY_TO_GUI_ENTER)
	{
		reflash=__FixEditState(the_edit);
	//	reflash=1;
		msg_widget=MESSAGE_WIDGET_ENTER;
	}

	if(reflash==TRUE)
	{	
		__Draw(the_edit);
	}

	return msg_widget;
}


/**************************************************************
** 函数名:EditCursorFlash
** 功能:EDIT的光标闪烁
** 注意事项:在GuiSysTick20ms中调用,系统规定500MS调用一次,用于刷新光标
***************************************************************/
void EditCursorFlash(bool flash)
{
	static DX_WIDGET* old_widget=NULL;
	static LOC_Y y0;
	DX_EDIT* the_edit;
	static u8 len,bcd;
	u8 num_len;
	LOC_X x0;
	
	if((pFocus_Widget->base_property&BASE_PRO_TYPE_MASK)!=BASE_PRO_EDIT)
	{
		return;
	}
	//焦点是新的控件,则更新基本坐标
	if(old_widget!=pFocus_Widget)
	{
		old_widget=pFocus_Widget;
		the_edit=(DX_EDIT*)old_widget;
		if((the_edit->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT16)//先计算字体基础高度
		{
			len=16;
		}
		else if((the_edit->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT24)
		{
			len=24;
		}
		else
		{
			len=32;
		}
		y0=the_edit->widget.y;
		y0=GetStringLocYFromFRAME(y0,the_edit->widget.height,len);
		bcd=GET_EDIT_BCD_LEN(the_edit);
	}

	if(Focus_Cursor_State!=FOCUS_CURSOR_ALL)
	{
		__SetOperationalWidget((DX_EDIT*)old_widget);
		//非BCD属性的EDIT,光标位根据Focus_Cursor_State而定
		if(!bcd)
		{
			num_len=Focus_Cursor_State;
		}
		//BCD属性EDIT,无光标
		else
		{
			return;
		}
		//根据负数再修正一次长度
		if((*(((DX_EDIT*)old_widget)->p_tmp))<0)
		{
			num_len++;
		}
	
		x0=((DX_EDIT*)old_widget)->widget.x+num_len*len/2+4;
		__TFTShowCursor(x0,y0,len,flash);
	}
}

/**************************************************************
** 函数名:EditPassMatch
** 功能:用于判断密码是否正确
** 注意事项:正确返回TRUE,注意这里还包括了位数的判断
***************************************************************/
bool EditPassMatch(DX_EDIT* the_edit,u32 num)
{
	u8 i;
	i=(*(the_edit->p_self_property))&SELF_PRO_EDIT_PASS_NUM_MASK;
	if(i==GetNumLen(*(the_edit->p_tmp)))
	{
		if(num==*(the_edit->p_tmp))
		{
			return TRUE;
		}
	}
	return FALSE;
}
