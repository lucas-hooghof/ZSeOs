#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "io.h"
#include "print.h"

#define ATA1_MASTER_PORT_START 0x1F0
#define ATA1_SLAVE_PORT_START  0x170

#define ATA_DATA_REGISTER           0
#define ATA_ERROR_REGISTER          1
#define ATA_FEATURES_REGISTER       1
#define ATA_SECTOR_COUNT_REGISTER   2
#define ATA_SECTOR_NUMBER_REGISTER  3
#define ATA_LBA_LOW_REGISTER        3
#define ATA_CYLINDER_LOW_REGISTER   4
#define ATA_LBA_MID_REGISTER        4
#define ATA_CYLINDER_HIGH_REGISTER  5
#define ATA_LBA_HIGH_REGISTER       5
#define ATA_DRIVE_REGISTER          6
#define ATA_HEAD_REGISTER           6
#define ATA_STATUS_REGISTER         7
#define ATA_COMMAND_REGISTER        7

#define ATA_CONTROL_ALTERNATE_STATUS_REGISTER           0x3F6
#define ATA_CONTROL_ALTERNATE_DEVICE_CONTROL_REGISTER   0x3F6
#define ATA_CONTROL_DRIVE_ADDRESS_REGISTER              0x376

#define ATA_MASTER 0xA0
#define ATA_SLAVE  0xB0

extern uint8_t ErrorRegisterData;
extern uint8_t StatusRegisterData;

bool IdentifyDrive(uint8_t Drive);


