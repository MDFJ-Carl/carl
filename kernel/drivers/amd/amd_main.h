// SPDX-License-Identifier: GPL-2.0-only
/*
 * carl/drivers/amd.h
 *
 * AMD GPU Driver Header
 * Linux-inspired AMDGPU driver API
 *
 * Copyright (c) mauriminuano125-a11y
 * All rights reserved.
 *
 * Author: mauriminuano125-a11y [mauriminuano125-a11y@gmail.com]
 * (C) Mau 2026
 * © Gamer Mauri 2026
 */

#ifndef TROVSAMD_H
#define TROVSAMD_H

#include <stdint.h>
#include <stddef.h>

/* ============================================================================
 * DRIVER IDENTIFICATION
 * ============================================================================
 */

#define TROVS_AMD_DRIVER        0x01
#define TROVS_DRIVER_AMD_NAME   "Trovs AMD"
#define TROVS_DRIVER_AMD_VERSION "0.2.0"
#define TROVS_AMD_VENDOR_ID     0x1002      /* AMD PCI vendor ID */

/* ============================================================================
 * CONSTANTS AND LIMITS
 * ============================================================================
 */

#define TROVS_AMD_MAX_DEVICES   16          /* Maximum GPUs supported */
#define TROVS_AMD_MAX_NAME_LEN  256         /* Max device name length */

/* ============================================================================
 * ERROR CODES
 * ============================================================================
 */

/**
 * AMD Driver Error Codes
 * Returned by all driver functions
 */
typedef enum {
    TROVS_AMD_SUCCESS = 0,              /* Operation successful */
    TROVS_AMD_ERR_ALREADY_INIT = 1,     /* Driver already initialized */
    TROVS_AMD_ERR_NO_DEVICES = 2,       /* No AMD GPUs found */
    TROVS_AMD_ERR_INIT_FAILED = 3,      /* Driver initialization failed */
    TROVS_AMD_ERR_NOT_INIT = 4,         /* Driver not initialized */
    TROVS_AMD_ERR_INVALID_INDEX = 5,    /* Invalid device index */
    TROVS_AMD_ERR_NULL_POINTER = 6,     /* NULL pointer parameter */
} trovs_amd_error_t;

/* ============================================================================
 * DATA STRUCTURES
 * ============================================================================
 */

/**
 * AMD GPU Device Information
 * Contains PCI identification and hardware details
 */
typedef struct {
    uint32_t device_id;                 /* PCI device ID */
    uint32_t vendor_id;                 /* PCI vendor ID (always 0x1002) */
    uint32_t subsystem_id;              /* PCI subsystem ID */
    uint32_t subsystem_vendor_id;       /* PCI subsystem vendor ID */
    char name[TROVS_AMD_MAX_NAME_LEN];  /* Device name (e.g., "RX 7900 XT") */
    char architecture[64];              /* Architecture (RDNA 3, RDNA 2, etc) */
    uint32_t vram_size_mb;              /* VRAM in MB */
    uint16_t bus;                       /* PCI bus number */
    uint16_t slot;                      /* PCI slot number */
    uint16_t function;                  /* PCI function number */
} trovs_amd_device_info;

/**
 * AMD Driver Statistics
 * Provides usage and performance information
 */
typedef struct {
    uint32_t total_memory;              /* Total VRAM in bytes */
    uint32_t used_memory;               /* Used VRAM in bytes */
    uint32_t free_memory;               /* Free VRAM in bytes */
    uint32_t gpu_utilization;           /* GPU load 0-100% */
    uint32_t memory_utilization;        /* Memory load 0-100% */
    uint32_t temperature_c;             /* GPU temperature in Celsius */
    uint32_t power_usage_w;             /* Power consumption in Watts */
    uint32_t clock_core_mhz;            /* Core clock in MHz */
    uint32_t clock_memory_mhz;          /* Memory clock in MHz */
} trovs_amd_stats_t;

/* ============================================================================
 * DRIVER INITIALIZATION & CONTROL
 * ============================================================================
 */

/**
 * trovs_amd_init() - Initialize AMD GPU driver
 *
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Scans PCI bus for AMD GPUs and initializes driver.
 * Must be called before any other driver functions.
 *
 * Supported GPU list (real device IDs):
 *
 * RDNA 3 (2023-2024):
 *   - AMD Radeon RX 7900 XTX (0x7480)
 *   - AMD Radeon RX 7900 XT  (0x7480)
 *   - AMD Radeon RX 7900 GRE (0x745F)
 *   - AMD Radeon RX 7800 XT  (0x7420)
 *   - AMD Radeon RX 7700 XT  (0x7410)
 *   - AMD Radeon RX 7700     (0x7410)
 *   - AMD Radeon RX 7600 XT  (0x7480)
 *   - AMD Radeon RX 7600     (0x7480)
 *   - AMD Radeon RX 7600M    (0x7420)
 *
 * RDNA 2 (2020-2022):
 *   - AMD Radeon RX 6900 XT  (0x73A0)
 *   - AMD Radeon RX 6800 XT  (0x73AF)
 *   - AMD Radeon RX 6800     (0x73AF)
 *   - AMD Radeon RX 6750 XT  (0x73DF)
 *   - AMD Radeon RX 6700 XT  (0x73DF)
 *   - AMD Radeon RX 6700     (0x73DF)
 *   - AMD Radeon RX 6650 XT  (0x73E0)
 *   - AMD Radeon RX 6600 XT  (0x73E0)
 *   - AMD Radeon RX 6600     (0x73E0)
 *
 * RDNA (2020):
 *   - AMD Radeon RX 5700 XT  (0x7318)
 *   - AMD Radeon RX 5600 XT  (0x7340)
 *
 * Vega (2017-2019):
 *   - AMD Radeon RX Vega 64  (0x6860)
 *   - AMD Radeon RX Vega 56  (0x6860)
 */
int trovs_amd_init(void);

/**
 * trovs_amd_shutdown() - Shut down AMD GPU driver
 *
 * Releases resources and disables GPUs.
 * Should be called before system shutdown or driver unload.
 */
void trovs_amd_shutdown(void);

/**
 * trovs_amd_is_initialized() - Check if driver is initialized
 *
 * @return: 1 if initialized, 0 if not
 */
int trovs_amd_is_initialized(void);

/* ============================================================================
 * DEVICE ENUMERATION
 * ============================================================================
 */

/**
 * trovs_amd_get_device_count() - Get number of detected AMD GPUs
 *
 * @return: Number of AMD GPU devices (0 if not initialized or none found)
 */
int trovs_amd_get_device_count(void);

/**
 * trovs_amd_get_device_info() - Get information about AMD GPU device
 *
 * @index: Device index (0 = first GPU)
 * @info: Pointer to device info structure to fill
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Retrieves detailed information about detected GPU.
 * Index must be < trovs_amd_get_device_count().
 *
 * Example:
 *   trovs_amd_device_info info;
 *   if (trovs_amd_get_device_info(0, &info) == TROVS_AMD_SUCCESS) {
 *       printf("GPU: %s (%s)\n", info.name, info.architecture);
 *       printf("VRAM: %u MB\n", info.vram_size_mb);
 *   }
 */
int trovs_amd_get_device_info(int index, trovs_amd_device_info *info);

/**
 * trovs_amd_get_device_by_name() - Find GPU device by name
 *
 * @name: Device name to search for
 * @info: Pointer to device info structure to fill
 * @return: Device index or -1 if not found
 *
 * Searches for GPU by name string (e.g., "RX 7900 XT")
 * Returns index of matching device.
 */
int trovs_amd_get_device_by_name(const char *name, trovs_amd_device_info *info);

/* ============================================================================
 * ERROR HANDLING
 * ============================================================================
 */

/**
 * trovs_amd_get_error() - Get last error code
 *
 * @return: Last error code (0 = success)
 */
int trovs_amd_get_error(void);

/**
 * trovs_amd_get_error_string() - Get error description
 *
 * @error_code: Error code from driver function
 * @return: Static error description string
 *
 * Returns human-readable error message.
 */
const char *trovs_amd_get_error_string(int error_code);

/* ============================================================================
 * DRIVER INFORMATION
 * ============================================================================
 */

/**
 * trovs_amd_get_driver_version() - Get driver version string
 *
 * @return: Version string (e.g., "0.2.0")
 */
const char *trovs_amd_get_driver_version(void);

/**
 * trovs_amd_get_driver_name() - Get driver name
 *
 * @return: Driver name ("Trovs AMD")
 */
const char *trovs_amd_get_driver_name(void);

/**
 * trovs_amd_print_info() - Print driver information
 *
 * Displays driver version, supported GPUs, and detected devices.
 * Useful for debugging and system diagnostics.
 */
void trovs_amd_print_info(void);

/* ============================================================================
 * DEVICE CONTROL (Future Implementation)
 * ============================================================================
 */

/**
 * trovs_amd_get_stats() - Get device statistics
 *
 * @index: Device index
 * @stats: Pointer to stats structure to fill
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Note: Currently not implemented
 */
// int trovs_amd_get_stats(int index, trovs_amd_stats_t *stats);

/**
 * trovs_amd_reset_device() - Reset GPU device
 *
 * @index: Device index
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Note: Currently not implemented
 */
// int trovs_amd_reset_device(int index);

/**
 * trovs_amd_set_power_state() - Set GPU power state
 *
 * @index: Device index
 * @state: Power state (0-100)
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Note: Currently not implemented
 */
// int trovs_amd_set_power_state(int index, uint8_t state);

/* ============================================================================
 * VERSION HISTORY
 * ============================================================================
 */

/**
 * Version 0.2.0 (Current):
 * - Real AMD GPU device database
 * - Support for 20+ GPU models
 * - RDNA 3, RDNA 2, RDNA, Vega architectures
 * - Comprehensive error handling
 * - Device enumeration API
 * - Driver information functions
 *
 * Version 0.1.0 (Original):
 * - Basic driver structure
 * - Simple device info structure
 * - Placeholder implementation
 *
 * Future Versions:
 * - Memory management API
 * - Command submission
 * - Job scheduling
 * - Power management
 * - Thermal monitoring
 * - Interrupt handling
 * - DMA operations
 */

#endif  /* TROVSAMD_H */
