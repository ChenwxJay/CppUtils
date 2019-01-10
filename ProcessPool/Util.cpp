#include "Util.h"


typedef void (*handler)(int) Handler 

//添加信号处理函数
void AddSigHandler(int signal,Handler handler,bool restart = true){
   struct sigaction sigac;
   memset(&sigac,0,sizeof(sigac));
   //填充信号处理器
   sigac.sa_handler = handler;
   //判断是否需要使能重启选项
   if(restart == true)
   {
   	 sigac.sa_flags |= SA_RESTART;
   }
   //填充掩码位
   sigfillset(&sigac.sa_mask);
   //给信号sig注册信号处理函数及相关属性，此处校验是否注册成功
   assert(sigaction(sig,&sigac,NULL) != -1);
}