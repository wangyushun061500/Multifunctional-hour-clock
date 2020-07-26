#ifndef	__EEPROM_H
#define	__EEPROM_H

#include "H Group/config.h"

void	DisableEEPROM(void);
void 	EEPROM_read_n(u16 EE_address,u8 *DataAddress,u16 number);
void 	EEPROM_write_n(u16 EE_address,u8 *DataAddress,u16 number);
void	EEPROM_SectorErase(u16 EE_address);
u8 		EEPROM_read_Compare(u16 EE_address,u8 *DataAddress,u16 number);


#endif