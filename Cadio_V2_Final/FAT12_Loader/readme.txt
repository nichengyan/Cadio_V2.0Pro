FAT12_Loader安装方式：
	 因为电脑与FLASH沟通的桥梁是USBMSC程序，而USBMSC程序本身平常又不常驻单片机内，
	 也要从外部FLASH里加载来运行。所以：首先你得让外部FLASH里有FAT12文件系统和USB_MSC.BIN这一文件
	 
	 下面的步骤，就是要为空FLASH建立FAT12，并写入USB_MSC.BIN
     1：进入STC_BoardCadio下的“STC_U盘”文件夹
        打开里面的   "直接可用的.hex"，烧录进单片机，进入USBMSC模式
	 2：格式化磁盘 （1*）
     3：把STC_BoardCadio\STC_U盘\Objects\usb_msc.bin 拖进U盘
	 4: 把STC_BoardCadio\FAT12_Loader\Objects\FAT12_Loader.Hex烧入单片机。
	    烧录务必选40M,64KEEPROM	
//---------------------------------------------------------------------------------------- 	 
FAT12_Loader使用方法：	 
#1：正常启动：
    按下电源开关即可直接启动到APP程序

#2：APP选择
    按住上/下键并按下电源开关将进入APP选择界面。
    用上/下键选择要使用的APP并按"="键，显示出APP加载进度
    加载完毕即自动跳转到APP（2*）,同时保存设置，下次默认正常启动到该APP
#3: USB正常烧录&&APP下载
    a:按住USB下载按钮就将进行STC正常的USB烧录（注意：正常烧录就将覆盖掉BootLoader）（3*）
    b:选择USB_MSC.BIN这个APP并加载，进入USBMSC模式，把你的APP拖进根目录即可
      再次重启进入APP加载界面就会看见你拖进去的新APP
	  
	操作优盘时候，不要手残把USB_MSC给删了！！！不然又要重新安装FAT2_Loader了  （4*）
//----------------------------------------------------------------------------------------  
FAT12_Loader的APP开发
	
	在普通项目的基础上多3步：
	1：Project->Options for Target-->Device->Off-chip-Code-memory->仅在start中填入0x3000
	2：Project->Options for Target-->C51->勾选Interrupt vectors at address->填入0x3000
	3: 打开Startup.A51（如果写APP之前就要选择加入这个）,第125行改为"CSEG    AT   3000h"
	重新编译，编译出来的机器文件就是可以被FAT12_Loader加载运行的APP了。
	
	#1# 在APP中，不要对前12k(前24个扇区),以及最后一个扇区进行擦写，否则会导致BootLoader损坏或异常！
    #2# Keil编译出来的普通APP是无法被FAT12_Loader正确加载并运行的！（完全跑不起来，还可能损坏BootLoader）
	同时，FAT12_Loader可加载的APP在无FAT12_Loader的普通芯片上也无法运行（其实能跑起来，但不能中断）
	（原因：程序偏移量和中断向量不正确）
	
//---------------------------------------------------------------------------------------- 	
FAT12_Loader及其APP的常见问题：
（1*）格盘失败，显示windows无法格式化磁盘或者压根就不认盘：多半是因为FLASH连接问题，极小概率是由于芯片写保护（但不排除，此问题多出现在拆机FLASH上）
（2*）眼看着加载完了，但界面就停在这了：大概率因为你的APP有问题导致崩溃，当然也有可能是因为你的APP里没涉及oled清屏操作，它正确跳转并跑起来了只不过你没看见罢了
（3*）把bootloader玩坏了怎么办:再烧一遍就好了。但你不管怎么玩FLASH里的东西是好好的，所以就不用干任何事了就好了
（4*）误删USB_MSC.BIN:别傻愣着，赶紧再拷一份进去啊。如果很不幸你没意识到并且已经加载了别的APP,那就彻底废了。按步骤重新安装FAT12_Loader吧	