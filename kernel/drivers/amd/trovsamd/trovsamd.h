// SPDX-License-Identifier: GPL-2.0-only
/*
 * carl/drivers/trovsamd.h
 *
 * Trovs AMD GPU Driver - Complete API Abstraction Layer
 * 
 * Copyright (c) mauriminuano125-a11y
 * All rights reserved.
 *
 * Author: mauriminuano125-a11y [mauriminuano125-a11y@gmail.com]
 * (C) Mau 2026
 * © Gamer Mauri 2026
 *
 * This header provides a comprehensive, unified API for AMD GPU operations
 * including initialization, device management, memory operations, command
 * submission, and performance monitoring.
 */

#ifndef TROVSAMD_H
#define TROVSAMD_H

#include <stdint.h>
#include <stddef.h>

/* ============================================================================
 * DRIVER IDENTIFICATION & VERSION
 * ============================================================================
 */

#define TROVS_AMD_DRIVER            0x01
#define TROVS_DRIVER_AMD_NAME       "Trovs AMD"
#define TROVS_DRIVER_AMD_VERSION    "1.0.0"
#define TROVS_DRIVER_BUILD_DATE     __DATE__
#define TROVS_DRIVER_BUILD_TIME     __TIME__
#define TROVS_AMD_VENDOR_ID         0x1002      /* AMD PCI vendor ID */

/* ============================================================================
 * CONSTANTS & LIMITS
 * ============================================================================
 */

#define TROVS_AMD_MAX_DEVICES       16          /* Max supported GPUs */
#define TROVS_AMD_MAX_ADAPTERS      32          /* Max GPU adapters */
#define TROVS_AMD_MAX_NAME_LEN      256         /* Max device name */
#define TROVS_AMD_MAX_MEMORY_MB     (32 * 1024) /* Max VRAM 32GB */
#define TROVS_AMD_MAX_QUEUES        32          /* Max command queues */

/* ============================================================================
 * ERROR CODES & STATUS
 * ============================================================================
 */

/**
 * Trovs AMD Error Codes
 * All functions return one of these status codes
 */
typedef enum {
    /* Success codes */
    TROVS_AMD_SUCCESS = 0,                  /* Operation completed successfully */
    
    /* Initialization errors (1-9) */
    TROVS_AMD_ERR_ALREADY_INIT = 1,         /* Driver already initialized */
    TROVS_AMD_ERR_INIT_FAILED = 2,          /* Driver initialization failed */
    TROVS_AMD_ERR_NOT_INIT = 3,             /* Driver not initialized */
    
    /* Device errors (10-19) */
    TROVS_AMD_ERR_NO_DEVICES = 10,          /* No AMD GPUs found */
    TROVS_AMD_ERR_INVALID_DEVICE = 11,      /* Invalid device index */
    TROVS_AMD_ERR_DEVICE_NOT_FOUND = 12,    /* Device not found */
    TROVS_AMD_ERR_DEVICE_UNAVAILABLE = 13,  /* Device unavailable/busy */
    
    /* Memory errors (20-29) */
    TROVS_AMD_ERR_OUT_OF_MEMORY = 20,       /* GPU memory allocation failed */
    TROVS_AMD_ERR_INVALID_MEMORY = 21,      /* Invalid memory pointer */
    TROVS_AMD_ERR_MEMORY_MAP_FAILED = 22,   /* Memory mapping failed */
    
    /* Command submission errors (30-39) */
    TROVS_AMD_ERR_INVALID_COMMAND = 30,     /* Invalid command buffer */
    TROVS_AMD_ERR_QUEUE_FULL = 31,          /* Command queue is full */
    TROVS_AMD_ERR_COMMAND_FAILED = 32,      /* Command execution failed */
    
    /* Parameter errors (40-49) */
    TROVS_AMD_ERR_NULL_POINTER = 40,        /* NULL pointer parameter */
    TROVS_AMD_ERR_INVALID_PARAM = 41,       /* Invalid parameter value */
    TROVS_AMD_ERR_INVALID_INDEX = 42,       /* Index out of range */
    
    /* Hardware errors (50-59) */
    TROVS_AMD_ERR_TIMEOUT = 50,             /* Operation timeout */
    TROVS_AMD_ERR_RESET_REQUIRED = 51,      /* GPU reset required */
    TROVS_AMD_ERR_THERMAL_THROTTLE = 52,    /* GPU thermal throttling */
    TROVS_AMD_ERR_POWER_LIMIT = 53,         /* Power limit exceeded */
    
    /* Unsupported/Generic errors (60+) */
    TROVS_AMD_ERR_NOT_SUPPORTED = 60,       /* Feature not supported */
    TROVS_AMD_ERR_GENERIC = 99,             /* Generic/unknown error */
} trovs_amd_status_t;

/* ============================================================================
 * GPU ARCHITECTURES
 * ============================================================================
 */

/**
 * AMD GPU Architecture Types
 * Identifies the GPU generation and architecture
 */
typedef enum {
    TROVS_AMD_ARCH_VEGA = 0,                /* Vega (older) */
    TROVS_AMD_ARCH_RDNA = 1,                /* RDNA 1 */
    TROVS_AMD_ARCH_RDNA2 = 2,               /* RDNA 2 */
    TROVS_AMD_ARCH_RDNA3 = 3,               /* RDNA 3 (latest) */
    TROVS_AMD_ARCH_UNKNOWN = 255,           /* Unknown architecture */
} trovs_amd_architecture_t;

/* ============================================================================
 * DATA STRUCTURES - DEVICE INFO
 * ============================================================================
 */

/**
 * AMD GPU Device Information
 * Complete hardware and identification details
 */
typedef struct {
    /* PCI Identification */
    uint16_t vendor_id;                     /* PCI Vendor ID (0x1002) */
    uint16_t device_id;                     /* PCI Device ID */
    uint16_t subsystem_vendor_id;           /* PCI Subsystem Vendor */
    uint16_t subsystem_id;                  /* PCI Subsystem ID */
    uint16_t bus;                           /* PCI Bus number */
    uint16_t slot;                          /* PCI Slot number */
    uint16_t function;                      /* PCI Function number */
    
    /* Device Name & Architecture */
    char name[TROVS_AMD_MAX_NAME_LEN];      /* Device name (e.g., RX 7900 XT) */
    char architecture[64];                  /* Architecture string */
    trovs_amd_architecture_t arch_type;     /* Architecture enum */
    
    /* Memory Information */
    uint32_t vram_size_mb;                  /* Total VRAM in MB */
    uint32_t vram_available_mb;             /* Available VRAM in MB */
    uint32_t vram_used_mb;                  /* Used VRAM in MB */
    uint32_t vram_type;                     /* 0=GDDR6, 1=HBM, 2=GDDR6X */
    uint32_t vram_bus_width;                /* Memory bus width in bits */
    
    /* Core Information */
    uint32_t num_cu;                        /* Number of Compute Units */
    uint32_t num_rbs;                       /* Number of Render Backends */
    uint32_t num_spm;                       /* Shader Processors */
    uint32_t clock_core_mhz;                /* Core clock in MHz */
    uint32_t clock_memory_mhz;              /* Memory clock in MHz */
    uint32_t clock_boost_mhz;               /* Boost clock in MHz */
    
    /* Power & Thermal */
    uint32_t tdp_watts;                     /* Thermal Design Power (TDP) */
    uint32_t power_limit_watts;             /* Current power limit */
    uint32_t temperature_current_c;         /* Current temperature in °C */
    uint32_t temperature_junction_c;        /* Junction temperature */
    uint32_t temperature_edge_c;            /* Edge temperature */
    
    /* Status & Features */
    uint8_t is_present;                     /* Device present flag */
    uint8_t is_enabled;                     /* Device enabled flag */
    uint8_t is_reset_required;              /* Device reset required */
    uint32_t feature_flags;                 /* Supported features bitmap */
} trovs_amd_device_info_t;

/**
 * AMD GPU Statistics & Performance Metrics
 */
typedef struct {
    /* Memory Statistics */
    uint32_t total_memory_mb;               /* Total VRAM in MB */
    uint32_t used_memory_mb;                /* Used VRAM in MB */
    uint32_t free_memory_mb;                /* Free VRAM in MB */
    uint32_t memory_utilization_percent;    /* 0-100% */
    
    /* Performance Metrics */
    uint32_t gpu_utilization_percent;       /* GPU load 0-100% */
    uint32_t compute_utilization_percent;   /* Compute load */
    uint32_t texture_utilization_percent;   /* Texture engine load */
    
    /* Clock & Power */
    uint32_t clock_core_current_mhz;        /* Current core clock */
    uint32_t clock_memory_current_mhz;      /* Current memory clock */
    uint32_t power_consumption_watts;       /* Current power usage */
    uint32_t power_limit_watts;             /* Current power limit */
    
    /* Thermal */
    uint32_t temperature_c;                 /* GPU temperature */
    uint32_t temperature_junction_c;        /* Junction temperature */
    uint32_t temperature_limit_c;           /* Throttle temperature */
    uint8_t is_thermal_throttled;           /* Thermal throttle active */
    
    /* Counters */
    uint64_t total_bytes_read;              /* Total bytes read from VRAM */
    uint64_t total_bytes_written;           /* Total bytes written to VRAM */
    uint64_t commands_submitted;            /* Total commands submitted */
    uint64_t commands_completed;            /* Total commands completed */
} trovs_amd_stats_t;

/**
 * Memory Allocation Request
 */
typedef struct {
    uint32_t size_bytes;                    /* Size in bytes */
    uint32_t alignment_bytes;               /* Alignment requirement */
    uint8_t is_pinned;                      /* Pin memory in place */
    uint8_t is_cached;                      /* Cache in system memory */
    void *user_data;                        /* User context pointer */
} trovs_amd_memory_request_t;

/**
 * Memory Allocation Handle
 */
typedef struct {
    uint64_t handle;                        /* Unique handle identifier */
    void *cpu_address;                      /* CPU-side address */
    uint64_t gpu_address;                   /* GPU virtual address */
    uint32_t size_bytes;                    /* Allocated size */
} trovs_amd_memory_handle_t;

/**
 * Command Buffer / Queue Handle
 */
typedef struct {
    uint64_t handle;                        /* Queue handle */
    uint32_t index;                         /* Queue index */
    uint32_t max_entries;                   /* Max pending commands */
    uint32_t current_entries;               /* Current pending */
} trovs_amd_queue_handle_t;

/* ============================================================================
 * CORE DRIVER API - INITIALIZATION & CONTROL
 * ============================================================================
 */

/**
 * trovs_amd_init() - Initialize Trovs AMD driver
 *
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Performs driver initialization including:
 * - PCI bus enumeration
 * - GPU detection
 * - Memory mapping
 * - Register initialization
 * - Interrupt setup
 *
 * Must be called before any other API functions.
 * Thread-safe and idempotent (multiple calls safe).
 */
trovs_amd_status_t trovs_amd_init(void);

/**
 * trovs_amd_shutdown() - Shut down Trovs AMD driver
 *
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Cleanly shutdown driver:
 * - Complete pending commands
 * - Free allocated memory
 * - Reset GPUs
 * - Unmap registers
 * - Release interrupts
 */
trovs_amd_status_t trovs_amd_shutdown(void);

/**
 * trovs_amd_is_initialized() - Check driver initialization status
 *
 * @return: 1 if initialized, 0 if not
 */
int trovs_amd_is_initialized(void);

/**
 * trovs_amd_reset_device() - Reset specific GPU device
 *
 * @device_index: Device index to reset
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Performs hardware reset of GPU device.
 * All pending commands will be lost.
 */
trovs_amd_status_t trovs_amd_reset_device(int device_index);

/* ============================================================================
 * DEVICE ENUMERATION & INFORMATION
 * ============================================================================
 */

/**
 * trovs_amd_get_device_count() - Get number of detected AMD GPUs
 *
 * @return: Number of GPU devices (0 if none or not initialized)
 *
 * Returns total count of AMD GPUs detected by driver.
 * Valid indices are 0 to count-1.
 */
int trovs_amd_get_device_count(void);

/**
 * trovs_amd_get_device_info() - Get information about GPU device
 *
 * @device_index: Device index (0 = first GPU)
 * @info: Pointer to device info structure to fill
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Retrieves comprehensive information about GPU device.
 *
 * Example:
 *   trovs_amd_device_info_t info;
 *   if (trovs_amd_get_device_info(0, &info) == TROVS_AMD_SUCCESS) {
 *       printf("GPU: %s\n", info.name);
 *       printf("VRAM: %u MB\n", info.vram_size_mb);
 *   }
 */
trovs_amd_status_t trovs_amd_get_device_info(int device_index,
                                             trovs_amd_device_info_t *info);

/**
 * trovs_amd_get_device_by_name() - Find GPU by name string
 *
 * @name: Device name to search (e.g., "RX 7900 XT")
 * @info: Pointer to device info structure to fill
 * @return: Device index (0+) if found, -1 if not found
 *
 * Searches for GPU matching name string.
 * Returns device index if found for use with other APIs.
 */
int trovs_amd_get_device_by_name(const char *name,
                                 trovs_amd_device_info_t *info);

/**
 * trovs_amd_get_device_by_id() - Find GPU by PCI device ID
 *
 * @device_id: PCI device ID to search
 * @info: Pointer to device info structure to fill
 * @return: Device index (0+) if found, -1 if not found
 */
int trovs_amd_get_device_by_id(uint16_t device_id,
                               trovs_amd_device_info_t *info);

/* ============================================================================
 * PERFORMANCE & STATISTICS
 * ============================================================================
 */

/**
 * trovs_amd_get_stats() - Get GPU performance statistics
 *
 * @device_index: Device index
 * @stats: Pointer to stats structure to fill
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Returns current performance metrics including:
 * - Memory usage
 * - GPU utilization
 * - Clock speeds
 * - Power consumption
 * - Temperature
 * - Command counters
 */
trovs_amd_status_t trovs_amd_get_stats(int device_index,
                                       trovs_amd_stats_t *stats);

/**
 * trovs_amd_get_utilization() - Get current GPU utilization
 *
 * @device_index: Device index
 * @utilization: Pointer to int for result (0-100%)
 * @return: TROVS_AMD_SUCCESS or error code
 */
trovs_amd_status_t trovs_amd_get_utilization(int device_index,
                                             int *utilization);

/**
 * trovs_amd_get_temperature() - Get GPU temperature
 *
 * @device_index: Device index
 * @temperature_c: Pointer to result (in Celsius)
 * @return: TROVS_AMD_SUCCESS or error code
 */
trovs_amd_status_t trovs_amd_get_temperature(int device_index,
                                             int *temperature_c);

/**
 * trovs_amd_get_power_usage() - Get GPU power consumption
 *
 * @device_index: Device index
 * @power_watts: Pointer to result (in Watts)
 * @return: TROVS_AMD_SUCCESS or error code
 */
trovs_amd_status_t trovs_amd_get_power_usage(int device_index,
                                             int *power_watts);

/* ============================================================================
 * MEMORY MANAGEMENT
 * ============================================================================
 */

/**
 * trovs_amd_alloc_memory() - Allocate GPU memory
 *
 * @device_index: Device index
 * @request: Memory allocation request
 * @handle: Pointer to handle for allocated memory
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Allocates contiguous memory on GPU VRAM.
 * Returns handle for use in other operations.
 */
trovs_amd_status_t trovs_amd_alloc_memory(int device_index,
                                          trovs_amd_memory_request_t *request,
                                          trovs_amd_memory_handle_t *handle);

/**
 * trovs_amd_free_memory() - Free GPU memory
 *
 * @device_index: Device index
 * @handle: Memory handle from alloc_memory()
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Releases previously allocated GPU memory.
 * Handle becomes invalid after call.
 */
trovs_amd_status_t trovs_amd_free_memory(int device_index,
                                         trovs_amd_memory_handle_t *handle);

/**
 * trovs_amd_copy_to_gpu() - Copy data to GPU memory
 *
 * @device_index: Device index
 * @gpu_address: GPU memory address
 * @cpu_data: CPU source pointer
 * @size_bytes: Number of bytes to copy
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Synchronous DMA copy from system memory to GPU VRAM.
 */
trovs_amd_status_t trovs_amd_copy_to_gpu(int device_index,
                                         uint64_t gpu_address,
                                         const void *cpu_data,
                                         uint32_t size_bytes);

/**
 * trovs_amd_copy_from_gpu() - Copy data from GPU memory
 *
 * @device_index: Device index
 * @cpu_data: CPU destination pointer
 * @gpu_address: GPU memory address
 * @size_bytes: Number of bytes to copy
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Synchronous DMA copy from GPU VRAM to system memory.
 */
trovs_amd_status_t trovs_amd_copy_from_gpu(int device_index,
                                           void *cpu_data,
                                           uint64_t gpu_address,
                                           uint32_t size_bytes);

/**
 * trovs_amd_get_memory_info() - Get GPU memory information
 *
 * @device_index: Device index
 * @total_mb: Pointer for total VRAM
 * @available_mb: Pointer for available VRAM
 * @return: TROVS_AMD_SUCCESS or error code
 */
trovs_amd_status_t trovs_amd_get_memory_info(int device_index,
                                             uint32_t *total_mb,
                                             uint32_t *available_mb);

/* ============================================================================
 * COMMAND SUBMISSION & QUEUES
 * ============================================================================
 */

/**
 * trovs_amd_create_queue() - Create GPU command queue
 *
 * @device_index: Device index
 * @queue_type: Queue type (0=compute, 1=copy, 2=transfer)
 * @priority: Queue priority (0=normal, 1=high)
 * @handle: Pointer to queue handle
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Creates command queue for submitting GPU work.
 */
trovs_amd_status_t trovs_amd_create_queue(int device_index,
                                          int queue_type,
                                          int priority,
                                          trovs_amd_queue_handle_t *handle);

/**
 * trovs_amd_destroy_queue() - Destroy GPU command queue
 *
 * @device_index: Device index
 * @handle: Queue handle
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Destroys queue and completes pending commands.
 */
trovs_amd_status_t trovs_amd_destroy_queue(int device_index,
                                           trovs_amd_queue_handle_t *handle);

/**
 * trovs_amd_submit_command() - Submit command to queue
 *
 * @device_index: Device index
 * @queue_handle: Queue handle
 * @command: Command data pointer
 * @size_bytes: Command size in bytes
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Submits GPU command for asynchronous execution.
 */
trovs_amd_status_t trovs_amd_submit_command(int device_index,
                                            trovs_amd_queue_handle_t *queue,
                                            const void *command,
                                            uint32_t size_bytes);

/**
 * trovs_amd_wait_queue() - Wait for queue to complete
 *
 * @device_index: Device index
 * @handle: Queue handle
 * @timeout_ms: Timeout in milliseconds (0 = infinite)
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Blocks until all pending commands in queue are completed.
 */
trovs_amd_status_t trovs_amd_wait_queue(int device_index,
                                        trovs_amd_queue_handle_t *handle,
                                        uint32_t timeout_ms);

/* ============================================================================
 * ERROR HANDLING & DIAGNOSTICS
 * ============================================================================
 */

/**
 * trovs_amd_get_error() - Get last error code
 *
 * @return: Last error code from driver
 *
 * Returns most recent error code from any operation.
 */
trovs_amd_status_t trovs_amd_get_error(void);

/**
 * trovs_amd_get_error_string() - Get error description
 *
 * @error_code: Error code to describe
 * @return: Static error description string
 *
 * Returns human-readable error message for error code.
 */
const char *trovs_amd_get_error_string(trovs_amd_status_t error_code);

/**
 * trovs_amd_print_info() - Print driver information
 *
 * Displays comprehensive driver information including:
 * - Driver version and build info
 * - Supported GPU list
 * - Detected devices
 * - Memory summary
 * - Useful for debugging and diagnostics
 */
void trovs_amd_print_info(void);

/**
 * trovs_amd_print_device_info() - Print specific device info
 *
 * @device_index: Device index
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Prints detailed information about specific GPU.
 */
trovs_amd_status_t trovs_amd_print_device_info(int device_index);

/**
 * trovs_amd_print_stats() - Print device statistics
 *
 * @device_index: Device index
 * @return: TROVS_AMD_SUCCESS or error code
 *
 * Prints performance metrics and resource usage.
 */
trovs_amd_status_t trovs_amd_print_stats(int device_index);

/* ============================================================================
 * DRIVER INFORMATION & VERSION
 * ============================================================================
 */

/**
 * trovs_amd_get_driver_version() - Get driver version string
 *
 * @return: Version string (e.g., "1.0.0")
 */
const char *trovs_amd_get_driver_version(void);

/**
 * trovs_amd_get_driver_name() - Get driver name
 *
 * @return: Driver name ("Trovs AMD")
 */
const char *trovs_amd_get_driver_name(void);

/**
 * trovs_amd_get_driver_author() - Get driver author
 *
 * @return: Author name
 */
const char *trovs_amd_get_driver_author(void);

/**
 * trovs_amd_get_supported_architectures() - List supported GPU architectures
 *
 * @return: Descriptive string of supported architectures
 */
const char *trovs_amd_get_supported_architectures(void);

#endif  /* TROVSAMD_H */
