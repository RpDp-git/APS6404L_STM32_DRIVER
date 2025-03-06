#ifndef QSPI_FLASH_H
#define QSPI_FLASH_H

#include "main.h"

// QSPI Flash Commands (Based on Datasheet)
#define QSPI_FLASH_CMD_READ            0x03  // Standard Read (SPI Mode)
#define QSPI_FLASH_CMD_FAST_READ       0x0B  // Fast Read (SPI Mode)
#define QSPI_FLASH_CMD_FAST_QUAD_READ  0xEB  // Fast Read Quad (QPI Mode)
#define QSPI_FLASH_CMD_PAGE_PROG       0x02  // Standard Page Program (SPI Mode)
#define QSPI_FLASH_CMD_QUAD_WRITE      0x38  // Quad Write (QPI Mode)
#define QSPI_FLASH_CMD_ENTER_QUAD      0x35  // Enter Quad Mode
#define QSPI_FLASH_CMD_EXIT_QUAD       0xF5  // Exit Quad Mode
#define QSPI_FLASH_CMD_READ_ID         0x9F  // Read Manufacturer ID
#define QSPI_FLASH_CMD_WRITE_ENABLE    0x06  // Write Enable
#define QSPI_FLASH_CMD_RESET_ENABLE    0x66  // Reset Enable
#define QSPI_FLASH_CMD_RESET           0x99  // Reset

// Function Prototypes
uint8_t qspi_flash_write(QSPI_HandleTypeDef *hqspi, uint32_t address, uint8_t *data, uint32_t size);
uint8_t qspi_flash_read(QSPI_HandleTypeDef *hqspi, uint32_t address, uint8_t *data, uint32_t size);
uint8_t qspi_flash_read_id(QSPI_HandleTypeDef *hqspi, uint8_t *data, uint32_t size);
uint8_t qspi_flash_write_enable(QSPI_HandleTypeDef *hqspi);
uint8_t qspi_flash_enter_quad_mode(QSPI_HandleTypeDef *hqspi);
uint8_t qspi_flash_exit_quad_mode(QSPI_HandleTypeDef *hqspi);
uint8_t qspi_flash_reset(QSPI_HandleTypeDef *hqspi);

#endif // QSPI_FLASH_H
