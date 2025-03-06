#include "qspi_flash.h"
#include <stdio.h>

uint8_t qspi_flash_reset(QSPI_HandleTypeDef *hqspi)
{
    QSPI_CommandTypeDef sCommand;

    // Send Reset Enable Command
    sCommand.Instruction = QSPI_FLASH_CMD_RESET_ENABLE;
    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;

    if (HAL_OK != HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE))
    {
        printf("QSPI Flash Reset Enable Failed!\n");
        return 0;
    }

    // Send Reset Command
    sCommand.Instruction = QSPI_FLASH_CMD_RESET;
    if (HAL_OK != HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE))
    {
        printf("QSPI Flash Reset Failed!\n");
        return 0;
    }

    return 1;
}


uint8_t qspi_flash_write_enable(QSPI_HandleTypeDef *hqspi)
{
    QSPI_CommandTypeDef sCommand;

    sCommand.Instruction = QSPI_FLASH_CMD_WRITE_ENABLE;
    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;

    if (HAL_OK != HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE))
    {
        printf("Write Enable Command Failed!\n");
        return 0;
    }

    return 1;
}

// QSPI Write Function (Polling Mode)
uint8_t qspi_flash_write(QSPI_HandleTypeDef *hqspi, uint32_t address, uint8_t *data, uint32_t size)
{
    QSPI_CommandTypeDef sCommand;

    sCommand.Instruction = 0x38;  // Quad Write
    sCommand.Address = address;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.InstructionMode = QSPI_INSTRUCTION_4_LINES;  // QPI Mode (Quad)
    sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
    sCommand.DataMode = QSPI_DATA_4_LINES;  // Use Quad SPI
    sCommand.DummyCycles = 0;  // No dummy cycles for write
    sCommand.NbData = size;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    printf("Enabling Write...\n");
    qspi_flash_write_enable(hqspi);  // Ensure Flash is Ready for Writing

    printf("Sending Write Command...\n");
    if (HAL_OK != HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE))
    {
        printf("QSPI Write Command Failed!\n");
        return 0;
    }

    printf("Writing Data to Flash...\n");
    if (HAL_OK != HAL_QSPI_Transmit(hqspi, data, HAL_QSPI_TIMEOUT_DEFAULT_VALUE))
    {
        printf("QSPI Write Transmit Failed!\n");
        return 0;
    }

    return 1;
}


// QSPI Read Function (Polling Mode)
uint8_t qspi_flash_read(QSPI_HandleTypeDef *hqspi, uint32_t address, uint8_t *data, uint32_t size)
{
    QSPI_CommandTypeDef sCommand;

    sCommand.Instruction = 0xEB;  // Fast Quad Read
    sCommand.Address = address;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.InstructionMode = QSPI_INSTRUCTION_4_LINES;  // QPI Mode (Quad)
    sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
    sCommand.DataMode = QSPI_DATA_4_LINES;  // Use Quad SPI
    sCommand.DummyCycles = 6;  // Per Datasheet for QPI Mode
    sCommand.NbData = size;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    printf("Sending Read Command...\n");
    if (HAL_OK != HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE))
    {
        printf("QSPI Read Command Failed!\n");
        return 0;
    }

    printf("Reading Data from Flash...\n");
    if (HAL_OK != HAL_QSPI_Receive(hqspi, data, HAL_QSPI_TIMEOUT_DEFAULT_VALUE))
    {
        printf("QSPI Read Receive Failed!\n");
        return 0;
    }

    return 1;
}


// QSPI Read ID Function
uint8_t qspi_flash_read_id(QSPI_HandleTypeDef *hqspi, uint8_t *data, uint32_t size)
{
    QSPI_CommandTypeDef sCommand;

    // Configure QSPI Read ID command
    sCommand.Instruction = QSPI_FLASH_CMD_READ_ID;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.DataMode = QSPI_DATA_1_LINE;
    sCommand.DummyCycles = 0;
    sCommand.NbData = size;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    // Send the Read ID command
    if (HAL_OK != HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE))
    {
        printf("QSPI Flash Read ID Command Failed!\n");
        return 0;
    }

    // Receive Manufacturer ID
    if (HAL_OK != HAL_QSPI_Receive(hqspi, data, HAL_QSPI_TIMEOUT_DEFAULT_VALUE))
    {
        printf("QSPI Flash Read ID Failed!\n");
        return 0;
    }

    return 1;  // Success
}

uint8_t qspi_flash_enter_quad_mode(QSPI_HandleTypeDef *hqspi)
{
    QSPI_CommandTypeDef sCommand;

    sCommand.Instruction = 0x35;  // Enter Quad Mode
    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;  // Must be in SPI Mode
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;

    if (HAL_OK != HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE))
    {
        printf("Failed to Enter Quad Mode!\n");
        return 0;
    }

    return 1;
}

uint8_t qspi_flash_exit_quad_mode(QSPI_HandleTypeDef *hqspi)
{
    QSPI_CommandTypeDef sCommand;

    sCommand.Instruction = 0xF5;  // Exit Quad Mode
    sCommand.InstructionMode = QSPI_INSTRUCTION_4_LINES;  // Must be in QPI Mode
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;

    if (HAL_OK != HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE))
    {
        printf("Failed to Exit Quad Mode!\n");
        return 0;
    }

    return 1;
}

