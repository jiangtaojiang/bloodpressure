/**************************************************************
模块名:widget.c 
功能:控件的通用功能函数
更新记录:2009-11-18 加入画通用3D框函数,对于EDIT,CHECK这类,用凹的3D框,按键用凸框
***************************************************************/


#include "stm32f10x.h"

#include "GUIDef.h"
#include "WIDGET.h"
#include "WINDOW.h"
#include "mem.h"

DX_WIDGET* pFocus_Widget;	//指向当前焦点空间的指针
u8 Focus_Cursor_State;		//当前焦点状态的辅助记录变量
/**************************************************************
** 函数名:GetStringXFromFRAME/GetStringYFromFRAME
** 功能:根据边框大小和字符长度,获取字符应有的起点
** 注意事项:获取X需要文本的指针和宽度,获取Y需要文本的高度,一般是16或24 
** 			*_*后期引入的一些辅助函数,使得下面的函数可以简化,主要在EDIT部分
***************************************************************/
//str_length是一个字符的宽度,注意是一个字符,汉字有两个字符.
LOC_X GetStringLocXFromFRAME(LOC_X x0,LOC_X length,u8 str_length,char*str)
{
	u8 i=0;
	while(i<30)//最大允许的字符宽度暂定为30
	{
		if(*str++==0)
			break;
		i++;
	}
	return(x0+(length+1-i*str_length)/2);
}

LOC_Y GetStringLocYFromFRAME(LOC_Y y0,LOC_Y height,u8 str_height)
{
	return(y0+(height-str_height+2)/2);//这里+2是为字库的修正~
}

//09.11.26增加的几何运算辅助函数,获取线段在线段中间的起始位置
u32 GetMiddleFromLine(u32 xy0,u32 xy1,u32 length)
{
	return(xy0+(xy1-xy0-length)/2);
}

//修正后的对齐函数,不分XY了
//获取字符的字节数,例如'你',返回2 'A',返回1
u32 GetStringSize(char* str)
{
	u8 i=0;
	while(i<30)//最大允许的字符宽度暂定为30
	{
		if(*str++==0)
			break;
		i++;
	}
	return i;
}




/**************************************************************
** 函数名:__Draw3DFrameA/B
** 功能:画3D框,这是通用的WINDOW 3D框
** 注意事项:A是凹框,B是凸框
***************************************************************/
void __Draw3DFrameA(LOC_X x0,LOC_Y y0,LOC_X length,LOC_Y height)
{
	LOC_X x1;
	LOC_X y1;

	x1=x0+length-1;
	y1=y0+height-1;
	//外上框
	TFTHLine(x0,y0,length-1,COLOR_FRAME_A_OU);
	TFTVLine(x0,y0+1,height-2,COLOR_FRAME_A_OU);
	//外下框
	TFTHLine(x0,y1,length,COLOR_FRAME_A_OD);
	TFTVLine(x1,y0,height-1,COLOR_FRAME_A_OU);
	//内框上
	TFTHLine(x0+1,y0+1,length-3,COLOR_FRAME_A_IU);
	TFTVLine(x0+1,y0+2,height-4,COLOR_FRAME_A_IU);
	//内框下
	TFTHLine(x0+1,y1-1,length-2,COLOR_FRAME_A_ID);
	TFTVLine(x1-1,y0+1,height-3,COLOR_FRAME_A_IU);
}
//凸框
void __Draw3DFrameB(LOC_X x0,LOC_Y y0,LOC_X length,LOC_Y height)
{
	LOC_X x1;
	LOC_X y1;

	x1=x0+length-1;
	y1=y0+height-1;
	//外框
	TFTRect(x0,y0,length,height,COLOR_FRAME_B);

	//画中框上
	TFTHLine(x0+1,y0+1,length-3,COLOR_FRAME_B_U);
	TFTVLine(x0+1,y0+2,height-4,COLOR_FRAME_B_U);
	
	//画中框下
	TFTHLine(x0+1,y1-1,length-3,COLOR_FRAME_B_D);
	TFTVLine(x1-1,y0+1,height-2,COLOR_FRAME_B_D);

	//画内框
	TFTHLine(x0+2,y1-2,length-4,COLOR_FRAME_B_I);
	TFTVLine(x1-2,y0+2,height-5,COLOR_FRAME_B_I);
}

//凹框2--按键按下后的凹框*_*如果仅仅是按键用,这个函数可不保留,看情况日后再改
void __Draw3DFrameC(LOC_X x0,LOC_Y y0,LOC_X length,LOC_Y height)
{
	TFTRect(x0,y0,length,height,COLOR_FRAME_B);
	TFTRect(x0+1,y0+1,length-2,height-2,COLOR_FRAME_B_I);
}



/**************************************************************
** 函数名:__PointInRect__
** 功能:检查一个POINT是否在矩形内
** 注意事项:为了加快效率,矩形的表达方式采用x0,y0,length,height. 
** 			*_*点在边沿上不算在矩形内 
***************************************************************/
bool __PointInRect__(LOC_X x,LOC_Y y,LOC_X x0,LOC_Y y0,u16 length,u16 height)
{
	if((x>x0)&&(x<(x0+length))&&(y>y0)&&(y<(y0+height)))
	{
		return TRUE;
	}
	else 
		return FALSE;
}
/**************************************************************
** 函数名:__PointInRect
** 功能:检查一个POINT是否在矩形内
** 注意事项:和__PointInRect__相比,这里使用了x0,y0,x1,y1来判断
** 			*_*点在边沿上不算在矩形内 
***************************************************************/
bool __PointInRect(LOC_X x,LOC_Y y,LOC_X x0,LOC_Y y0,LOC_X x1,LOC_Y y1)
{
	if((x>x0)&&(x<x1)&&(y>y0)&&(y<y1))
	{
		return TRUE;
	}
	else 
		return FALSE;
}

/**************************************************************
** 函数名:__MakeSearchRectL/R/U/D
** 功能:根据输入的四个坐标(矩阵的四个点),制造一个新的扩张矩阵,主要用于方向键移动控件的自动搜索控制
** 注意事项:扩张到最后不能扩张,则返回FALSE
***************************************************************/
#define SEARCH_WINC_AREA		25		//宽度每次扩张
#define SEARCH_DINC_AREA		50		//深度每次扩张
static bool __MakeSearchRectL(LOC_X* px0,LOC_Y* py0,LOC_X* px1,LOC_Y* py1)
{
	bool w0_state=TRUE,w1_state=TRUE,d_state=TRUE;
	
	//深度扩张
	if(*px0==MIN_X)
		d_state=FALSE;
	else if(*px0>SEARCH_DINC_AREA)
		*px0-=SEARCH_DINC_AREA;
	else
		*px0=MIN_X;

	//宽度扩张
	if(*py0==MIN_Y)
		w0_state=FALSE;
	else if(*py0>SEARCH_WINC_AREA)
		*py0-=SEARCH_WINC_AREA;
	else
		*py0=MIN_Y;

	if(*py1==MAX_Y)
		w1_state=FALSE;
	else if(*py1<MAX_Y-SEARCH_WINC_AREA)
		*py1+=SEARCH_WINC_AREA;
	else
		*py1=MAX_Y;

	if((w0_state!=FALSE)||(w1_state!=FALSE)||(d_state!=FALSE))
	{
		return TRUE;
	}
	return FALSE;
}

static bool __MakeSearchRectR(LOC_X* px0,LOC_Y* py0,LOC_X* px1,LOC_Y* py1)
{
	bool w0_state=TRUE,w1_state=TRUE,d_state=TRUE;
	
	//深度扩张
	if(*px1==MAX_X)
		d_state=FALSE;
	else if(*px1<MAX_X-SEARCH_DINC_AREA)
		*px1+=SEARCH_DINC_AREA;
	else
		*px1=MAX_X;

	//宽度扩张
	if(*py0==MIN_Y)
		w0_state=FALSE;
	else if(*py0>SEARCH_WINC_AREA)
		*py0-=SEARCH_WINC_AREA;
	else
		*py0=MIN_Y;

	if(*py1==MAX_Y)
		w1_state=FALSE;
	else if(*py1<MAX_Y-SEARCH_WINC_AREA)
		*py1+=SEARCH_WINC_AREA;
	else
		*py1=MAX_Y;

	if((w0_state!=FALSE)||(w1_state!=FALSE)||(d_state!=FALSE))
	{
		return TRUE;
	}
	return FALSE;
}
static bool __MakeSearchRectU(LOC_X* px0,LOC_Y* py0,LOC_X* px1,LOC_Y* py1)
{
	bool w0_state=TRUE,w1_state=TRUE,d_state=TRUE;
	
	//深度扩张
	if(*py0==MIN_Y)
		d_state=FALSE;
	else if(*py0>SEARCH_DINC_AREA)
		*py0-=SEARCH_DINC_AREA;
	else
		*py0=MIN_Y;

	//宽度扩张
	if(*px0==MIN_X)
		w0_state=FALSE;
	else if(*px0>SEARCH_WINC_AREA)
		*px0-=SEARCH_WINC_AREA;
	else
		*px0=MIN_X;

	if(*px1==MAX_X)
		w1_state=FALSE;
	else if(*px1<MAX_X-SEARCH_WINC_AREA)
		*px1+=SEARCH_WINC_AREA;
	else
		*px1=MAX_X;

	if((w0_state!=FALSE)||(w1_state!=FALSE)||(d_state!=FALSE))
	{
		return TRUE;
	}
	return FALSE;
}
static bool __MakeSearchRectD(LOC_X* px0,LOC_Y* py0,LOC_X* px1,LOC_Y* py1)
{
	bool w0_state=TRUE,w1_state=TRUE,d_state=TRUE;
	
	//深度扩张
	if(*py1==MAX_Y)
		d_state=FALSE;
	else if(*py1<MAX_Y-SEARCH_DINC_AREA)
		*py1-=SEARCH_DINC_AREA;
	else
		*py1=MAX_Y;

	//宽度扩张
	if(*px0==MIN_X)
		w0_state=FALSE;
	else if(*px0>SEARCH_WINC_AREA)
		*px0-=SEARCH_WINC_AREA;
	else
		*px0=MIN_X;

	if(*px1==MAX_X)
		w1_state=FALSE;
	else if(*px1<MAX_X-SEARCH_WINC_AREA)
		*px1+=SEARCH_WINC_AREA;
	else
		*px1=MAX_X;

	if((w0_state!=FALSE)||(w1_state!=FALSE)||(d_state!=FALSE))
	{
		return TRUE;
	}
	return FALSE;
}
/**************************************************************
** 函数名:FeelingDistanceCnt 
** 功能 :获取两个点之间的感性视觉距离
** 注意事项: 暂时很简单地X,Y叠加,因为这个视觉距离跟纵横有关系,到时候可能需要分开两个函数
***************************************************************/
u32 FeelingDistanceCnt(LOC_X x0,LOC_Y y0,LOC_X x1,LOC_Y y1)
{
	u32 feel_distance;
	if(x0>x1)
	{
		feel_distance=x0-x1;
	}
	else
	{
		feel_distance=x1-x0;
	}

	if(y0>y1)
	{
		feel_distance+=y0-y1;
	}
	else
	{
		feel_distance+=y1-y0;
	}
	return feel_distance;
}

/**************************************************************
** 函数名:GetNumLen
** 功能:获取数字的位数
** 注意事项:获取的是10进制的位,最小也会返回1,因为0也占了1位
			如果是负数,会先转化为正再作处理(09.7.10)
***************************************************************/
u8 GetNumLen(s32 num)
{
	if(num<0)//转换为正数
	{
		num=0-num;
	}

	if(num>9999999)
		return 8;
	if(num>999999)
		return 7;
	if(num>99999)
		return 6;
	if(num>9999)
		return 5;
	if(num>999)
		return 4;
	if(num>99)
		return 3;
	if(num>9)
		return 2;
	return 1;
}
/**************************************************************
** 函数名:NumToString/O8
** 功能:把十进制数字变换成字符串
** 注意事项:虽然NUM是32位输入,但这里系统规定了数字长度不能超过8位,这对一般的应用而言比较足够 
**  		返回字符串长度 ,返回的长度不算'\0',如果数字是0,则字符串长度也为1,
**  		支持负数显示(09,7.10)
**  		O8后缀的,代表8禁止显示方式,返回的也是8进制显示时候的字长
***************************************************************/
u8 NumToString(s32 num,char* str)
{
	u8 i;
	u8 j;
	u8 num_data[8]={0,0,0,0,0,0,0,0};
	
	if(num<0)//转换为正数
	{
		num=0-num;
	}
	
	while(num>99999999)
	{
		num-=100000000;		//保证数字不超过8位十进制
	}

	//2.数字分解-也可以用除法运算,哪个速度快有待验证
	while(num>9999999)
	{
		num_data[7]++;num-=10000000;
	}
	while(num>999999)
	{
		num_data[6]++;num-=1000000;
	}
	while(num>99999)
	{
		num_data[5]++;num-=100000;
	}
	while(num>9999)
	{
		num_data[4]++;num-=10000;
	}
	while(num>999)
	{
		num_data[3]++;num-=1000;
	}
	while(num>99)
	{
		num_data[2]++;num-=100;
	}
	while(num>9)
	{
		num_data[1]++;num-=10;
	}
	while(num>0)
	{
		num_data[0]++;num-=1;
	}

	i=8;
	j=0;
	do
	{
		i--;
		if((num_data[i])||(j!=0))
		{
			*str++='0'+num_data[i];
			if(j==0)
			{
				j=i+1;
			}
		}
	}while(i!=0);
	//后期加的改动,即使NUM为0,也要有字符串0产生
	if(j==0)
	{
		*str++='0';
		j=1;
	}
	*str='\0';
	return j;
}
u8 NumToStringO8(s32 num,char* str)
{
	u8 i;
	u8 j;
	u8 num_data[8]={0,0,0,0,0,0,0,0};
	
	if(num<0)//转换为正数
	{
		num=0-num;
	}
	
	while(num>077777777)
	{
		num-=0100000000;		
	}

	//2.数字分解-也可以用除法运算,哪个速度快有待验证
	while(num>07777777)
	{
		num_data[7]++;num-=010000000;
	}
	while(num>0777777)
	{
		num_data[6]++;num-=01000000;
	}
	while(num>077777)
	{
		num_data[5]++;num-=0100000;
	}
	while(num>07777)
	{
		num_data[4]++;num-=010000;
	}
	while(num>0777)
	{
		num_data[3]++;num-=01000;
	}
	while(num>077)
	{
		num_data[2]++;num-=0100;
	}
	while(num>7)
	{
		num_data[1]++;num-=010;
	}
	while(num>0)
	{
		num_data[0]++;num-=1;
	}

	i=8;
	j=0;
	do
	{
		i--;
		if((num_data[i])||(j!=0))
		{
			*str++='0'+num_data[i];
			if(j==0)
			{
				j=i+1;
			}
		}
	}while(i!=0);
	//后期加的改动,即使NUM为0,也要有字符串0产生
	if(j==0)
	{
		*str++='0';
		j=1;
	}
	*str='\0';
	return j;
}


/**************************************************************
** 函数名:__AutoMoveWidget
** 功能:自动计算并移动控件焦点
** 注意事项: 此函数被KeyToChangeWidget调用, 
**  		如果此函数产生了新的焦点,则返回新的焦点指针 ,否则返回NULL
**  		为了提高效率,里面的分配内存和装载坐标可以考虑用static变量和自动判断是否有刷新而选择性更新,暂时为每次都更新
**  		10.2.19:单纯扩张矩阵移动焦点还不合理,改进为在扩张矩阵中找到的控件都
**  		做一个对比,找到最合理的控件
**  		*_*还有改进和优化代码的余地
***************************************************************/
static DX_WIDGET* __AutoMoveWidget(DX_WIDGET* the_widget,GUIKEY key)
{
	u8 widget_no,i,self,next_no;
	LOC_X* widget_x;
	LOC_Y* widget_y;			//中心点库
	u32 tmp_distance,min_distance=0xffffffff;	//视觉距离
	
	LOC_X search_x0,search_x1;	//搜索的矩阵
	LOC_Y search_y0,search_y1;
	bool (*make_search_rect)(LOC_X*,LOC_Y*,LOC_X*,LOC_Y*);
	DX_WINDOW* the_window;
//	DX_WIDGET* tmp_widget;
	
	
	the_window=the_widget->far_win;
	widget_no=the_window->max_widget;
	//如果窗口的控件数少于一个,则直接返回
	if(widget_no<=1)
	{
		return NULL;
	}
	//如果控件锁焦,则直接返回
	if(*(the_widget->p_ext_property)&EXT_PRO_FLOCK)
	{
		return NULL;
	}
	//根据本窗口所有的控件数,分配坐标缓冲数组
	widget_x=BCNew(widget_no*sizeof(LOC_X));
	if(widget_x==NULL)
	{
		return NULL;
	}
	widget_y=BCNew(widget_no*sizeof(LOC_Y));
	if(widget_y==NULL)
	{
		BCDelete(widget_x);
		return NULL;
	}
	 
	//获取各个控件的中心坐标,即使某些控件为只读
	for(i=0;i<widget_no;i++)
	{
		widget_x[i]=the_window->p_child[i]->x+the_window->p_child[i]->length/2;
		widget_y[i]=the_window->p_child[i]->y+the_window->p_child[i]->height/2;
	}
	//获取本控件编号
	self=*(the_window->p_focu_no);
	   
	//制造基础搜索矩阵-基础矩阵其实为一个点
	search_x0=widget_x[self];
	search_x1=widget_x[self];
	search_y0=widget_y[self];
	search_y1=widget_y[self];

	//选取矩阵生成函数
	if(key==KEY_TO_GUI_LEFT)	
		make_search_rect=__MakeSearchRectL;	//左搜索
	else if(key==KEY_TO_GUI_RIGHT)
		make_search_rect=__MakeSearchRectR;	//右搜索
	else if(key==KEY_TO_GUI_UP)
		make_search_rect=__MakeSearchRectU;	//上搜索
	else if(key==KEY_TO_GUI_DOWN)
		make_search_rect=__MakeSearchRectD;	//下搜索

	while( (*make_search_rect)(&search_x0,&search_y0,&search_x1,&search_y1) !=FALSE)//一直搜索到矩阵不能扩张
	{
		//搜索控件
		for(i=0;i<widget_no;i++)
		{
			//自己不搜索
			if(i==self)
				continue;
			//禁止焦点的控件,不搜索
			if( ((the_widget->far_win->p_child[i]->base_property&BASE_PRO_FORBID_FOCUS)))
				continue;
			//无效控件不搜索
			if((*(the_widget->far_win->p_child[i]->p_ext_property))&EXT_PRO_DISABLE)
				continue;
			//隐形控件不搜索
			if((*(the_widget->far_win->p_child[i]->p_ext_property))&EXT_PRO_HIDE)
				continue;
			
			//判断当前点是不是在搜索矩阵内
			if(__PointInRect(widget_x[i],widget_y[i],search_x0,search_y0,search_x1,search_y1)==TRUE)
			{
				//如果在搜索矩阵内,则计算感性视觉距离
				tmp_distance=FeelingDistanceCnt(widget_x[self],widget_y[self],widget_x[i],widget_y[i]);
				if(tmp_distance<min_distance)
				{
					min_distance=tmp_distance;
					next_no=i;
				}
			}
		}
		//找到了最近的
		if(min_distance!=0xffffffff)
		{
			//释放内存
			BCDelete(widget_x);
			BCDelete(widget_y);
			//返回移动到的控件
			return the_widget->far_win->p_child[next_no];
		}
	}


	//到达这一步,则代表搜索不到
	//释放内存
	BCDelete(widget_x);
	BCDelete(widget_y);
	return NULL;
}

/**************************************************************
** 函数名:ReDrawWidget
** 功能 :重绘控件
** 注意事项:仅仅负责重绘,用于一些控件被刷新的时候调用,不改变标志位
***************************************************************/
void ReDrawWidget(DX_WIDGET *the_widget)
{
	u8 widget_type;
	widget_type=the_widget->base_property&BASE_PRO_TYPE_MASK; //获取控件类型
	switch(widget_type)
	{
		case BASE_PRO_BUTTON:
			ReDrawButton( (DX_BUTTON*)the_widget);
			break;
			
		case BASE_PRO_EDIT:
			ReDrawEdit( (DX_EDIT*)the_widget);
			break;

		case BASE_PRO_COMBO:
			ReDrawCombo( (DX_COMBO*)the_widget);
			break;
		case BASE_PRO_CHECK:
			ReDrawCheck( (DX_CHECK*)the_widget);
			break;
		case BASE_PRO_PROGRESS:
			ReDrawProgress( (DX_PROGRESS*)the_widget);
			break;
	}
}

/***************************
**函数名:GuiWidgetDraw
**功能:不分控件的类型来画控件
**注意事项:这个函数一般就在新WINDOW出现的时候才会调用它画出各个控件,在这个函数中对静态设置的控件内容进行一定程度的检查 
**			包括自动修正和一些静态创建时候的严重问题的处理:如while(1)的死循环,使得用户在最开始的调试中就能找到静态创建时候的问题 
***************************/
void GuiWidgetDraw(DX_WIDGET *the_widget)
{
	u8 widget_type;
	
	widget_type=the_widget->base_property&BASE_PRO_TYPE_MASK; //获取控件类型
	//*_*增加控件SHOW函数
	GuiWidgetReponse(the_widget,WIDGET_PARAM_SHOW);
	switch(widget_type)
	{
		//case BASE_PRO_WINDOW:
		//..WINDOW部分省略掉,由WindowDraw函数负责
		//break;

		case BASE_PRO_BUTTON:
			//按键在刚显示出来的时候,都不允许处于按下状态
			*(((DX_BUTTON*)the_widget)->p_self_property)&=~SELF_PRO_PUSH_DOWN;
			GuiButtonDraw( (DX_BUTTON*)the_widget);
			break;
			
		case BASE_PRO_EDIT:
			GuiEditDraw( (DX_EDIT*)the_widget);
			break;

		case BASE_PRO_COMBO:
			GuiComboDraw( (DX_COMBO*)the_widget);
			break;
		case BASE_PRO_CHECK:
			GuiCheckDraw( (DX_CHECK*)the_widget);
			break;
		case BASE_PRO_PROGRESS:
			GuiProgressDraw( (DX_PROGRESS*)the_widget);
			break;
/*		case OBJ_STATIC:
		//	GuiStaticDraw( (DX_STATIC*)the_obj,op);
			break;
		case OBJ_STATICA:
		//	GuiStaticADraw( (DX_STATICA*)the_obj,op);
			break;
		case OBJ_PROGRAMBAR:
		//	GuiProgramBarDraw((DX_PROGRAMBAR*)the_obj);
			break;			
*/		default:
			break;
	}
}

/**************************************************************
** 函数名:UpdateWidget
** 功能:刷新控件
** 注意事项:暂时没用,
***************************************************************/
void UpdateWidget(DX_WIDGET* the_widget)
{
	
}

/**************************************************************
** 函数名:SetWidgetFlash
** 功能:设置控件闪烁功能
** 注意事项:仅仅改变FLASH_EN标志位,闪烁的功能在GUI TICK中进行 
** 			注意FLASH位是又程序自动改变的,人为只能改变EN位 
***************************************************************/
void SetWidgetFlash(DX_WIDGET* the_widget,bool State)
{
	if(the_widget!=NULL)
	{
		if(State==TRUE)
		{
			*(the_widget->p_ext_property)|=EXT_PRO_FLASH_EN;
		}
		else
		{
			*(the_widget->p_ext_property)&=~EXT_PRO_FLASH_EN;

		}
	}
}

/**************************************************************
** 函数名:SetWidgetFLock 
** 功能 :设置控件锁焦
** 注意事项: 
***************************************************************/
void SetWidgetFLock(DX_WIDGET* the_widget,bool State)
{
	if(the_widget!=NULL)
	{
		if(State==TRUE)
		{
			*(the_widget->p_ext_property)|=EXT_PRO_FLOCK;
		}
		else
		{
			*(the_widget->p_ext_property)&=~EXT_PRO_FLOCK;
		}
	}
}

/**************************************************************
** 函数名:SetWidgetHide
** 功能:设置控件隐藏功能
** 注意事项:除了改变EXT属性外,还需要刷新控件-根据控件当前是否可用来刷新 
** 			如果控件被设置隐藏,需要保证其非焦 
***************************************************************/
void SetWidgetHide(DX_WIDGET* the_widget,bool State)
{
	if(the_widget!=NULL)
	{
		if(State==TRUE)
		{
			//之前非隐藏的-变成隐藏
			if(!(*(the_widget->p_ext_property)&EXT_PRO_HIDE))
			{
				if(IsWidgetAvailable(the_widget))
				{
					ReDrawWidget(the_widget);
				}
				*(the_widget->p_ext_property)|=EXT_PRO_HIDE;//**特别注意,调用完IsWidgetAvailable才能置位HIDE,否则ReDrawWidget就不生效
				SetNextFocus(the_widget);					//保证其非焦
			}
		}
		else
		{
			//之前是隐藏的
			if(*(the_widget->p_ext_property)&EXT_PRO_HIDE)
			{
				*(the_widget->p_ext_property)&=~EXT_PRO_HIDE;
				if(IsWidgetAvailable(the_widget))
				{
					ReDrawWidget(the_widget);
				}
			}
		}
	}
}

/**************************************************************
** 函数名:SetWidgetDisable
** 功能:设置控件不可用
** 注意事项:除了改变EXT属性外,还要刷新控件-根据控件当前是否可用 
** 			如果控件被设置不可用,需保证其非焦 
***************************************************************/
void SetWidgetDisable(DX_WIDGET* the_widget,bool State)
{
	if(State==TRUE)
	{
		//之前非DISABEL
		if(!(*(the_widget->p_ext_property)&EXT_PRO_DISABLE))
		{
			*(the_widget->p_ext_property)|=EXT_PRO_DISABLE;
			if(IsWidgetAvailable(the_widget))
			{
				ReDrawWidget(the_widget);
			}
			SetNextFocus(the_widget);					//保证其非焦
		}
	}
	else
	{
		//之前是DISABLE
		if(*(the_widget->p_ext_property)&EXT_PRO_DISABLE)
		{
			*(the_widget->p_ext_property)&=~EXT_PRO_DISABLE;
			if(IsWidgetAvailable(the_widget))
			{
				ReDrawWidget(the_widget);
			}
		}
	}
}

/**************************************************************
** 函数名:__SetWidgetFocus
** 功能:设置焦点
** 注意事项:成功则返回旧焦点 否则返回NULL.此函数没有调用画焦点功能,仅仅改变窗口句柄和控件句柄的状态
***************************************************************/
static DX_WIDGET* __SetWidgetFocus(DX_WIDGET* the_widget)
{
	u8 i;
	DX_WINDOW* the_window;
	DX_WIDGET* old_widget;
	if((*(the_widget->p_ext_property))&EXT_PRO_FOCUS)//已经是焦点了,不必设置
	{
		return NULL;
	}
	if((*(the_widget->p_ext_property))&EXT_PRO_HIDE)//HIDE属性,不能设置
	{
		return NULL;
	}
	if((*(the_widget->p_ext_property))&EXT_PRO_DISABLE)//DISABLE属性,不能设置
	{
		return NULL;
	}
	if(the_widget->base_property&BASE_PRO_FORBID_FOCUS)//BASE_PRO_FORBID_FOCUS属性,不能设置
	{
		return NULL;
	}

	//搜索要置焦点的控件,如果搜索不到,则代表输入参数有误
	the_window=the_widget->far_win;

	for(i=0;i<the_window->max_widget;i++)
	{
		if(the_window->p_child[i]==the_widget)						
		{
			//保存旧焦点
			old_widget=the_window->p_child[*(the_window->p_focu_no)];
			//更新新的焦点号
			*(the_window->p_focu_no)=i;		
			goto true_widget;
		}
	}
	return(NULL);

true_widget:
	//去掉旧焦点
	*(old_widget->p_ext_property)&=~EXT_PRO_FOCUS;

	//设置新焦点
	*(the_widget->p_ext_property)|=EXT_PRO_FOCUS;
	//更新pFocus
	if(the_window==pNow_Window)
	{
		pFocus_Widget=the_widget;
	}
	return old_widget;
}

/************************************************************** 
** 函数名:SetNextFocus
** 功能:系统随机的改变焦点,
** 注意事项:暂时规定,系统改变焦点,不调用任何响应函数. 
**  		另外由于是系统改变,所以不论当前焦点是什么状态,焦点都应该移动到下一个控件.
** 			目的是尽最大可能使得 让参数控件非焦
************************************************************/
void SetNextFocus(DX_WIDGET* the_widget)
{
	u32 i;
	u8 self;
	DX_WINDOW* the_window;
	//参数控件非焦点,则无需调用这个函数
	if((!(*(the_widget->p_ext_property))&EXT_PRO_FOCUS))
	{
		return;
	}
	the_window=the_widget->far_win;
	self=*(the_window->p_focu_no);
	for(i=0;i<the_window->max_widget;i++)
	{
		if(i==self)
			continue;
		if(__SetWidgetFocus(the_window->p_child[i])!=NULL)
			return;
	}
}
/**************************************************************
**函数名:KeyToChangeWidget
**功能:单击的键对控件的影响
**注意事项:此函数被KeyToGui调用,调用前需确保the_widget非空 并且为当前焦点.
** 			这个函数会改变Focus_Cursor_State
** 			改变的同时会调用响应函数
***************************************************************/
void KeyToChangeWidget(DX_WIDGET* the_widget,GUIKEY key)
{
	DX_WIDGET* new_widget;
	DX_WIDGET* old_widget;
	MESSAGE_WIDGET msg_widget;

	//当前控件为禁焦的,则不需要处理
	if((the_widget->base_property)&BASE_PRO_FORBID_FOCUS)
	{
		return ;
	}

	//方向键对于所有控件都用同一处理方式
	if((key>=KEY_TO_GUI_LEFT)&&(key<=KEY_TO_GUI_DOWN))		//方向键
	{
		new_widget=__AutoMoveWidget(the_widget,key);
		if(new_widget!=NULL)	//移动到新的焦点
		{
			//设置新的焦点
			old_widget=__SetWidgetFocus(new_widget);
			if(old_widget!=NULL)
			{
				//运行失去焦点响应函数FOCUS_LOST
				//..
				GuiWidgetReponse((DX_WIDGET*)old_widget,WIDGET_PARAM_FOCUS_LOST);
				//绘制旧控件
				ReDrawWidget(old_widget);
				
				//如果是EDIT则改变Focus_Cursor_State状态*_*放这里有点勉强,先顶着用
				if((new_widget->base_property&BASE_PRO_TYPE_MASK)==BASE_PRO_EDIT)
				{
					Focus_Cursor_State=FOCUS_CURSOR_ALL;
				}
				//运行获取焦点响应函数FOCUS_GET
				//..
				GuiWidgetReponse((DX_WIDGET*)new_widget,WIDGET_PARAM_FOCUS_GET);
				//绘制新控件
				ReDrawWidget(new_widget);
			}
		}
		return;
	}

	
	//其它键则根据是什么控件,分别作出处理*_*这里写得不太面向对象,先顶着用
	if((the_widget->base_property&BASE_PRO_TYPE_MASK)==BASE_PRO_EDIT)//对EDIT有效
	{
		msg_widget=KeyToChangeEdit((DX_EDIT*)the_widget,key);
		if(msg_widget==MESSAGE_WIDGET_EDIT)//编辑
		{
			GuiWidgetReponse(the_widget,WIDGET_PARAM_EDIT);
		}
		else if(msg_widget==MESSAGE_WIDGET_ENTER)//回车
		{
			GuiWidgetReponse(the_widget,WIDGET_PARAM_ENTER);
		}
	}
	else if((the_widget->base_property&BASE_PRO_TYPE_MASK)==BASE_PRO_BUTTON)//对BUTTON
	{
		msg_widget=KeyToChangeButton((DX_BUTTON*)the_widget,key);
		if(msg_widget==MESSAGE_WIDGET_ENTER)//回车
		{
			GuiWidgetReponse(the_widget,WIDGET_PARAM_ENTER);
		}
	}
	else if((the_widget->base_property&BASE_PRO_TYPE_MASK)==BASE_PRO_CHECK)//对CHECK
	{
		msg_widget=KeyToChangeCheck((DX_CHECK*)the_widget,key);
		if(msg_widget==MESSAGE_WIDGET_EDIT)//编辑
		{
			GuiWidgetReponse(the_widget,WIDGET_PARAM_EDIT);
		}
		else if(msg_widget==MESSAGE_WIDGET_ENTER)//回车
		{
			GuiWidgetReponse(the_widget,WIDGET_PARAM_ENTER);
		}
	}
	//对combo
	else if((the_widget->base_property&BASE_PRO_TYPE_MASK)==BASE_PRO_COMBO)//对COMBO
	{
		msg_widget=KeyToChangeCombo((DX_COMBO*)the_widget,key);
		if(msg_widget==MESSAGE_WIDGET_EDIT)//编辑
		{
			GuiWidgetReponse(the_widget,WIDGET_PARAM_EDIT);
		}
		else if(msg_widget==MESSAGE_WIDGET_ENTER)//回车
		{
			GuiWidgetReponse(the_widget,WIDGET_PARAM_ENTER);
		}
	}
}


/**************************************************************
** 函数名 :SetWidgetFocus
** 功能 :人为强行更换焦点
** 注意事项: 成功则返回旧的焦点,失败返回NULL,此函数调用__SetWidgetFocus函数
***************************************************************/
DX_WIDGET* SetWidgetFocus(DX_WIDGET* new_widget)
{
	DX_WIDGET* old_widget;

	old_widget=__SetWidgetFocus(new_widget);
	if(old_widget!=NULL)
	{
		//运行失去焦点响应函数FOCUS_LOST
		GuiWidgetReponse((DX_WIDGET*)old_widget,WIDGET_PARAM_FOCUS_LOST);
		//绘制旧控件
		GuiWidgetDraw(old_widget);
		
		//如果是EDIT则改变Focus_Cursor_State状态*_*放这里有点勉强,先顶着用
		if((new_widget->base_property&BASE_PRO_TYPE_MASK)==BASE_PRO_EDIT)
		{
			Focus_Cursor_State=FOCUS_CURSOR_ALL;
		}
		//运行获取焦点响应函数FOCUS_GET
		//..
		GuiWidgetReponse((DX_WIDGET*)new_widget,WIDGET_PARAM_FOCUS_GET);
		//绘制新控件
		GuiWidgetDraw(new_widget);
	}
	return old_widget;
}


/*********************************************************************
** 函数名:WidgetFlash
** 功能:处理控件的闪烁
** 注意事项:
**********************************************************************/
void WidgetFlash(bool flash)
{
	u32 i;
	DX_WIDGET* pFlash_Widget;

	//整个窗口控件的刷新*_*这里之后要考虑加入Z轴上的拼接窗体
	if(pNow_Window!=NULL)
	{
		//闪烁控件的刷新
		for(i=0;i<pNow_Window->max_widget;i++)
		{
			if(*(pNow_Window->p_child[i]->p_ext_property)&EXT_PRO_FLASH_EN)
			{
				pFlash_Widget=pNow_Window->p_child[i];
				if(flash==TRUE)
				{
					if(!(*(pFlash_Widget->p_ext_property)&EXT_PRO_FLASH))
					{
						*(pFlash_Widget->p_ext_property)|=EXT_PRO_FLASH;
						//刷新
						ReDrawWidget(pFlash_Widget);
					}
				}
				else
				{
					if(*(pFlash_Widget->p_ext_property)&EXT_PRO_FLASH)
					{
						un_flash:
						*(pFlash_Widget->p_ext_property)&=~EXT_PRO_FLASH;
						//刷新
						ReDrawWidget(pFlash_Widget);
					}
				}
			}
			//当有控件没有EN位却有FLASH位时,代表认为取消掉EN位的(而正好FALSH位置位),于是清除掉FLASH位并刷新
			else if(*(pNow_Window->p_child[i]->p_ext_property)&EXT_PRO_FLASH)
			{
				pFlash_Widget=pNow_Window->p_child[i];
				goto un_flash;
			}
			
		}
	}
}

