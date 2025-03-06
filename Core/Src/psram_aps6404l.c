#include "psram_aps6404l.h"

// Declare the external QSPI handle. This must match the instance name in your initialization.
extern QSPI_HandleTypeDef hqspi1;

// Define a timeout value for QSPI operations (in milliseconds)
#define QSPI_TIMEOUT  1000

/**
  * @brief  Perform a software reset on the PSRAM using the RSTEN (0x66) and RST (0x99) commands.
  * @retval HAL status
  */
HAL_StatusTypeDef PSRAM_Reset(void)
{
    QSPI_CommandTypeDef sCommand;
    HAL_StatusTypeDef status;

    // --- Step 1: Send Reset Enable (0x66) ---
    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = APS6404L_RSTEN_CMD;
    sCommand.AddressMode       = QSPI_ADDRESS_NONE;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_NONE;
    sCommand.DummyCycles       = 0;
    sCommand.NbData            = 0;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;

    status = HAL_QSPI_Command(&hqspi1, &sCommand, QSPI_TIMEOUT);
    if (status != HAL_OK)
    {
        return status;
    }

    // --- Step 2: Send Reset (0x99) ---
    sCommand.Instruction = APS6404L_RST_CMD;
    status = HAL_QSPI_Command(&hqspi1, &sCommand, QSPI_TIMEOUT);
    return status;
}

/**
  * @brief  Enter Quad (QPI) mode by sending the 0x35 command.
  * @retval HAL status
  */
HAL_StatusTypeDef PSRAM_EnterQuadMode(void)
{
    QSPI_CommandTypeDef sCommand;
    HAL_StatusTypeDef status;

    // Quad Mode Enable command (0x35) must be sent in SPI mode (1-line)
    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = APS6404L_QUAD_MODE_CMD;
    sCommand.AddressMode       = QSPI_ADDRESS_NONE;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_NONE;
    sCommand.DummyCycles       = 0;
    sCommand.NbData            = 0;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;

    status = HAL_QSPI_Command(&hqspi1, &sCommand, QSPI_TIMEOUT);
    return status;
}

HAL_StatusTypeDef PSRAM_SetWrapMode(void)
{
    QSPI_CommandTypeDef sCommand;

    // Configure command to toggle wrap boundary mode (0x5F)
    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = 0x5F;    // Wrap Boundary Toggle command
    sCommand.AddressMode       = QSPI_ADDRESS_NONE;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_NONE;
    sCommand.DummyCycles       = 0;
    sCommand.NbData            = 0;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;

    return HAL_QSPI_Command(&hqspi1, &sCommand, QSPI_TIMEOUT);
}

/**
  * @brief  Read data from PSRAM in QPI mode.
  * @param  address: Memory address to start reading (24-bit address assumed)
  * @param  pData: Pointer to buffer where read data will be stored
  * @param  size: Number of bytes to read
  * @retval HAL status
  */
HAL_StatusTypeDef PSRAM_QPI_Read(uint32_t address, uint8_t *pData, uint32_t size)
{
    QSPI_CommandTypeDef sCommand;
    HAL_StatusTypeDef status;

    // Configure the QPI Read command
    sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
    sCommand.Instruction       = APS6404L_READ_CMD;
    sCommand.AddressMode       = QSPI_ADDRESS_4_LINES;
    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
    sCommand.Address           = address;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_4_LINES;
    sCommand.DummyCycles       = APS6404L_DUMMY_CYCLES;
    sCommand.NbData            = size;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;

    // Send command and then receive the data
    status = HAL_QSPI_Command(&hqspi1, &sCommand, QSPI_TIMEOUT);
    if (status != HAL_OK)
    {
        return status;
    }
    status = HAL_QSPI_Receive(&hqspi1, pData, QSPI_TIMEOUT);
    return status;
}

/**
  * @brief  Write data to PSRAM in QPI mode.
  * @param  address: Memory address to start writing (24-bit address assumed)
  * @param  pData: Pointer to the data buffer to be written
  * @param  size: Number of bytes to write
  * @retval HAL status
  */
HAL_StatusTypeDef PSRAM_QPI_Write(uint32_t address, uint8_t *pData, uint32_t size)
{
    QSPI_CommandTypeDef sCommand;
    HAL_StatusTypeDef status;

    // Configure the QPI Write command
    sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
    sCommand.Instruction       = APS6404L_WRITE_CMD;
    sCommand.AddressMode       = QSPI_ADDRESS_4_LINES;
    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
    sCommand.Address           = address;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_4_LINES;
    sCommand.DummyCycles       = 0;  // No dummy cycles needed for write
    sCommand.NbData            = size;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;

    // Send command and then transmit the data
    status = HAL_QSPI_Command(&hqspi1, &sCommand, QSPI_TIMEOUT);
    if (status != HAL_OK)
    {
        return status;
    }
    status = HAL_QSPI_Transmit(&hqspi1, pData, QSPI_TIMEOUT);
    return status;
}

HAL_StatusTypeDef PSRAM_QPI_FastRead(uint32_t address, uint8_t *pData, uint32_t size)
{
    QSPI_CommandTypeDef sCommand;
    HAL_StatusTypeDef status;

    // Configure the Fast Read command in QPI mode
    sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
    sCommand.Instruction       = APS6404L_FAST_READ_CMD;
    sCommand.AddressMode       = QSPI_ADDRESS_4_LINES;
    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
    sCommand.Address           = address;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_4_LINES;
    sCommand.DummyCycles       = APS6404L_FAST_READ_DUMMY_CYCLES;
    sCommand.NbData            = size;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;

    status = HAL_QSPI_Command(&hqspi1, &sCommand, QSPI_TIMEOUT);
    if (status != HAL_OK)
    {
        return status;
    }

    status = HAL_QSPI_Receive(&hqspi1, pData, QSPI_TIMEOUT);
    return status;
}

/**
  * @brief  Fast write data to PSRAM using QPI mode in wrap mode.
  *         Uses opcode 0x38 for fast write.
  * @param  address: 24-bit memory address to start writing.
  * @param  pData: Pointer to data to be written.
  * @param  size: Number of bytes to write.
  * @retval HAL status
  */
HAL_StatusTypeDef PSRAM_QPI_FastWrite(uint32_t address, uint8_t *pData, uint32_t size)
{
    QSPI_CommandTypeDef sCommand;
    HAL_StatusTypeDef status;

    // Configure the Fast Write command in QPI mode
    sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
    sCommand.Instruction       = APS6404L_FAST_WRITE_CMD;
    sCommand.AddressMode       = QSPI_ADDRESS_4_LINES;
    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
    sCommand.Address           = address;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_4_LINES;
    sCommand.DummyCycles       = 0;  // Writes usually require no dummy cycles
    sCommand.NbData            = size;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;

    status = HAL_QSPI_Command(&hqspi1, &sCommand, QSPI_TIMEOUT);
    if (status != HAL_OK)
    {
        return status;
    }

    status = HAL_QSPI_Transmit(&hqspi1, pData, QSPI_TIMEOUT);
    return status;
}

HAL_StatusTypeDef PSRAM_QPI_FastRead_DMA(uint32_t address, uint8_t *pData, uint32_t size)
{
    QSPI_CommandTypeDef sCommand;
    HAL_StatusTypeDef status;

    sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
    sCommand.Instruction       = APS6404L_FAST_READ_CMD;  // 0xEB
    sCommand.AddressMode       = QSPI_ADDRESS_4_LINES;
    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
    sCommand.Address           = address;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_4_LINES;
    sCommand.DummyCycles       = APS6404L_FAST_READ_DUMMY_CYCLES; // 6 dummy cycles
    sCommand.NbData            = size;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;

    status = HAL_QSPI_Command(&hqspi1, &sCommand, QSPI_TIMEOUT);
    if(status != HAL_OK)
        return status;

    return HAL_QSPI_Receive_DMA(&hqspi1, pData);
}

HAL_StatusTypeDef PSRAM_QPI_FastWrite_DMA(uint32_t address, uint8_t *pData, uint32_t size)
{
    QSPI_CommandTypeDef sCommand;
    HAL_StatusTypeDef status;

    // Configure the fast write command in QPI mode (using opcode 0x38)
    sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
    sCommand.Instruction       = APS6404L_FAST_WRITE_CMD; // 0x38
    sCommand.AddressMode       = QSPI_ADDRESS_4_LINES;
    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
    sCommand.Address           = address;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_4_LINES;
    sCommand.DummyCycles       = 0;  // No dummy cycles for write
    sCommand.NbData            = size;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;

    status = HAL_QSPI_Command(&hqspi1, &sCommand, QSPI_TIMEOUT);
    if (status != HAL_OK)
        return status;

    // Initiate DMA-based transmit
    return HAL_QSPI_Transmit_DMA(&hqspi1, pData);
}
