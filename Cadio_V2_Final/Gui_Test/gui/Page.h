#ifndef __PAGE_H__
#define __PAGE_H__

#include "Gui_Config.h" 

#include "Page0.h"
enum page_condtion
{
	PAGE_ENTER,
	PAGE_RETURN
};
enum page_status
{
	PAGE_IDLE,
	PAGE_INIT,
	PAGE_RUN,
	PAGE_EXIT
};
/*
typedef struct{
    uint8_t Type;
    uint16_t para_a;
    uint16_t papa_b;
}Event;
*/
#define Event uint8_t
typedef struct{
    uint8_t Id;
    uint8_t Status;
	unsigned char *Title;
	unsigned char *Icon;
	unsigned char Stat;//״̬
    unsigned char Tick; //ʱ��Ƭ
	void (*SetupCallback)(uint8_t a); //��ʼʱʱ����
    void (*LoopCallback) (void); //ѭ������
    void (*ExitCallback) (uint8_t a); //�˳�����
    void (*EventCallback) (Event e);
}Page;

static Page PageList[MAX_PAGE];
static uint8_t now_page = 0;
/**
  * @brief  ע��һ������ҳ�棬����һ����ʼ��������ѭ���������˳��������¼�����
  * @param  pageID: ҳ����
	*	@param	pageText: ҳ������ָ��
	*	@param	pageIcon: ҳ��ͼ���ָ��
  * @param  setupCallback: ��ʼ�������ص�
  * @param  loopCallback: ѭ�������ص�
  * @param  exitCallback: �˳������ص�
  * @param  eventCallback: �¼������ص�
  * @retval ��
  */

void Page_Register(
        unsigned char pageID,
		unsigned char *pageTitle,
		unsigned char *pageIcon,
	    void (*SetupCallback)(uint8_t a), //��ʼʱʱ����
        void (*LoopCallback) (void), //ѭ������
        void (*ExitCallback) (uint8_t a), //�˳�����
        void (*EventCallback) (Event e)
);

void Page_EventTransmit(Event e);
void Page_OpenCurrentPage();
void Page_CloseCurrentPage();
void Page_Shift(unsigned char pageID, unsigned char condition);
void Page_Run(void);
void Page_SetStatus(unsigned char pageID, unsigned char status);
unsigned char Page_GetStatus(void);

#endif
