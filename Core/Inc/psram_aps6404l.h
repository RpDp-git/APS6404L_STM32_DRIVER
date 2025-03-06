#ifndef __PSRAM_APS6404L_H
#define __PSRAM_APS6404L_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"  // This should include your HAL headers and the definition of QSPI_HandleTypeDef

//=== Command Definitions (check your datasheet for confirmation) ===//
#define APS6404L_RSTEN_CMD       0x66   // Reset Enable
#define APS6404L_RST_CMD         0x99   // Reset
#define APS6404L_QUAD_MODE_CMD   0x35   // Enter Quad Mode
#define APS6404L_READ_CMD        0x0B   // QPI Read (using dummy cycles)
#define APS6404L_WRITE_CMD       0x02   // QPI Write

// Dummy cycles for read command; adjust based on your configuration/frequency
#define APS6404L_DUMMY_CYCLES    4

// New fast commands for higher throughput in wrap mode:
#define APS6404L_FAST_READ_CMD   0xEB   // Fast QPI Read command
#define APS6404L_FAST_WRITE_CMD  0x38   // Fast QPI Write command
#define APS6404L_FAST_READ_DUMMY_CYCLES    6


//=== Function Prototypes ===//
HAL_StatusTypeDef PSRAM_Reset(void);
HAL_StatusTypeDef PSRAM_EnterQuadMode(void);
HAL_StatusTypeDef PSRAM_SetWrapMode(void);
HAL_StatusTypeDef PSRAM_QPI_Read(uint32_t address, uint8_t *pData, uint32_t size);
HAL_StatusTypeDef PSRAM_QPI_Write(uint32_t address, uint8_t *pData, uint32_t size);
// New fast data read and write functions:
HAL_StatusTypeDef PSRAM_QPI_FastRead(uint32_t address, uint8_t *pData, uint32_t size);
HAL_StatusTypeDef PSRAM_QPI_FastWrite(uint32_t address, uint8_t *pData, uint32_t size);
// DMA version
HAL_StatusTypeDef PSRAM_QPI_FastRead_DMA(uint32_t address, uint8_t *pData, uint32_t size);
HAL_StatusTypeDef PSRAM_QPI_FastWrite_DMA(uint32_t address, uint8_t *pData, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif /* __PSRAM_APS6404L_H */
