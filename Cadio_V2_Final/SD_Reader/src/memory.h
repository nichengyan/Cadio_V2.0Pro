#ifndef __MEMORY_H__
#define __MEMORY_H__



void flash_init(uchar id);
BOOL flash_check();
void flash_readsector(DWORD addr, BYTE *pdat);
void flash_writesector(DWORD addr, BYTE *pdat);
void Flash_SelectChip(uchar FlashID);

#define memory_init         flash_init
#define memory_check        flash_check
#define memory_readsector   flash_readsector
#define memory_writesector  flash_writesector

extern DWORD dwMemorySize;
extern WORD wSectorSize;
extern DWORD dwSectorNumber;

#endif

