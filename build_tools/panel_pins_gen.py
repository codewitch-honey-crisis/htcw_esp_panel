#!/usr/bin/env python3
# panel_pins_gen.py

# Device configuration - specify which buses each device supports
device_config = {
    "LCD": {
        "buses": ["SPI", "I2C"]
    },
    "TOUCH": {
        "buses": ["SPI", "I2C"]
    },
    "POWER": {
        "buses": ["SPI", "I2C"]
    },
    "SD": {
        "buses": ["SPI"]  # SD only supports SPI
    },
    "EXPANDER": {
        "buses": {"SPI", "I2C"}
    }
}

# Pin definitions for each bus type
bus_pins = {
    "SPI": ["CLK", "D00", "D01", "D02", "D03", "D04", "D05", "D06", "D07"],
    "I2C": ["SCL", "SDA"]
}

def get_devices_for_bus(bus_type):
    """Get list of devices that support a given bus type"""
    return [dev for dev, config in device_config.items() if bus_type in config["buses"]]

def generate_header_includes():
    """Generate conditional header includes based on what's being used"""
    output = []
    
    output.append("""
// ============================================================================
// CONDITIONAL HEADER INCLUDES
// ============================================================================
""")
    
    # Check if any device uses SPI
    spi_devices = get_devices_for_bus("SPI")
    spi_conditions = []
    for dev in spi_devices:
        spi_conditions.append(f"defined({dev}_SPI_HOST)")
    
    if spi_conditions:
        output.append(f"""
// Include SPI headers if any device uses SPI
#if {' || '.join(spi_conditions)}
    #include "driver/spi_master.h"
    #define PANEL_USE_SPI
#endif""")
    
    # Check if any device uses I2C
    i2c_devices = get_devices_for_bus("I2C")
    i2c_conditions = []
    for dev in i2c_devices:
        i2c_conditions.append(f"defined({dev}_I2C_HOST)")
    
    if i2c_conditions:
        output.append(f"""
// Include I2C headers if any device uses I2C
#if {' || '.join(i2c_conditions)}
    #ifdef LEGACY_I2C
        #include "driver/i2c.h"
    #else
        #include "driver/i2c_master.h"
    #endif
    #define PANEL_USE_I2C
#endif""")
    
    return "\n".join(output)

def generate_bus_checks(bus_type):
    output = []
    devices = get_devices_for_bus(bus_type)
    pins = bus_pins[bus_type]
    
    if len(devices) < 2:
        return ""  # No pairs to check
    
    for i, dev1 in enumerate(devices):
        for dev2 in devices[i+1:]:
            output.append(f"""
// {dev1}-{dev2} {bus_type} bus sharing
#if defined({dev1}_{bus_type}_HOST) && defined({dev2}_{bus_type}_HOST) && ({dev1}_{bus_type}_HOST == {dev2}_{bus_type}_HOST)""")
            
            for pin in pins:
                # Check conflicts
                output.append(f"""
    #if defined({dev1}_PIN_NUM_{pin}) && defined({dev2}_PIN_NUM_{pin}) && \\
        ({dev1}_PIN_NUM_{pin} != -1) && ({dev2}_PIN_NUM_{pin} != -1) && \\
        ({dev1}_PIN_NUM_{pin} != {dev2}_PIN_NUM_{pin})
        #error "{dev1}_PIN_NUM_{pin} and {dev2}_PIN_NUM_{pin} conflict on same {bus_type} bus"
    #endif""")
                
                # Share pins bidirectionally
                output.append(f"""
    #if !defined({dev1}_PIN_NUM_{pin}) || ({dev1}_PIN_NUM_{pin} == -1)
        #if defined({dev2}_PIN_NUM_{pin}) && ({dev2}_PIN_NUM_{pin} != -1)
            #undef {dev1}_PIN_NUM_{pin}
            #define {dev1}_PIN_NUM_{pin} {dev2}_PIN_NUM_{pin}
        #endif
    #endif
    #if !defined({dev2}_PIN_NUM_{pin}) || ({dev2}_PIN_NUM_{pin} == -1)
        #if defined({dev1}_PIN_NUM_{pin}) && ({dev1}_PIN_NUM_{pin} != -1)
            #undef {dev2}_PIN_NUM_{pin}
            #define {dev2}_PIN_NUM_{pin} {dev1}_PIN_NUM_{pin}
        #endif
    #endif""")
            
            output.append("#endif\n")
    
    return "\n".join(output)

def generate_cross_host_checks(bus_type):
    """Generate checks to ensure devices on different hosts don't use same pins"""
    output = []
    devices = get_devices_for_bus(bus_type)
    pins = bus_pins[bus_type]
    
    if len(devices) < 2:
        return ""
    
    for i, dev1 in enumerate(devices):
        for dev2 in devices[i+1:]:
            output.append(f"""
// {dev1}-{dev2} {bus_type} cross-host conflict check
#if defined({dev1}_{bus_type}_HOST) && defined({dev2}_{bus_type}_HOST) && ({dev1}_{bus_type}_HOST != {dev2}_{bus_type}_HOST)""")
            
            for pin in pins:
                # Only check for conflicts, no sharing when on different hosts
                output.append(f"""
    #if defined({dev1}_PIN_NUM_{pin}) && defined({dev2}_PIN_NUM_{pin}) && \\
        ({dev1}_PIN_NUM_{pin} != -1) && ({dev2}_PIN_NUM_{pin} != -1) && \\
        ({dev1}_PIN_NUM_{pin} == {dev2}_PIN_NUM_{pin})
        #error "{dev1}_PIN_NUM_{pin} and {dev2}_PIN_NUM_{pin} both use GPIO " ##{dev1}_PIN_NUM_{pin} " but are on different {bus_type} hosts"
    #endif""")
            
            output.append("#endif\n")
    
    return "\n".join(output)

def generate_cs_uniqueness_checks():
    """Generate checks to ensure CS pins are unique across all SPI devices"""
    output = []
    spi_devices = get_devices_for_bus("SPI")
    
    if len(spi_devices) < 2:
        return ""
    
    output.append("""
// ============================================================================
// SPI CS (CHIP SELECT) PIN UNIQUENESS CHECKS
// ============================================================================
// CS pins must be unique across ALL devices, even on the same bus
""")
    
    for i, dev1 in enumerate(spi_devices):
        for dev2 in spi_devices[i+1:]:
            output.append(f"""
// {dev1}-{dev2} CS pin uniqueness check
#if defined({dev1}_PIN_NUM_CS) && defined({dev2}_PIN_NUM_CS) && \\
    ({dev1}_PIN_NUM_CS != -1) && ({dev2}_PIN_NUM_CS != -1) && \\
    ({dev1}_PIN_NUM_CS == {dev2}_PIN_NUM_CS)
    #error "{dev1}_PIN_NUM_CS and {dev2}_PIN_NUM_CS both use GPIO " ##{dev1}_PIN_NUM_CS " - CS pins must be unique"
#endif""")
    
    return "\n".join(output)

def generate_host_consolidation():
    """Generate host-level pin definitions from device pins"""
    output = []
    
    output.append("""
// ============================================================================
// HOST-LEVEL PIN CONSOLIDATION
// ============================================================================
// Create host-specific defines from device pins
// This runs AFTER headers are included so SPI_1, SPI_2 etc are defined
""")
    
    # Only generate for hosts that are actually used
    output.append("""
#ifdef PANEL_USE_SPI""")
    
    # For SPI, start at 1 not 0
    for host_num in range(1, 5):  # SPI_1, SPI_2, SPI_3, SPI_4
        output.append(f"""
// ------------- SPI_{host_num} consolidation -------------
#ifdef SPI_{host_num}""")
        
        spi_devices = get_devices_for_bus("SPI")
        
        # Build condition string
        conditions = []
        for dev in spi_devices:
            conditions.append(f"(defined({dev}_SPI_HOST) && ({dev}_SPI_HOST == SPI_{host_num}))")
        
        if conditions:
            condition_str = " || ".join(conditions)
            output.append(f"""
#if {condition_str}
    #define SPI_{host_num}_HOST_USED""")
            
            # Consolidate each pin
            for pin in bus_pins["SPI"]:
                output.append(f"""
    
    // Consolidate {pin} for SPI_{host_num}""")
                for dev in spi_devices:
                    output.append(f"""
    #if defined({dev}_SPI_HOST) && ({dev}_SPI_HOST == SPI_{host_num}) && defined({dev}_PIN_NUM_{pin}) && ({dev}_PIN_NUM_{pin} != -1)
        #ifndef SPI_{host_num}_PIN_NUM_{pin}
            #define SPI_{host_num}_PIN_NUM_{pin} {dev}_PIN_NUM_{pin}
        #endif
    #endif""")
                
                # Always define the pin, defaulting to -1 if not set
                output.append(f"""
    #ifndef SPI_{host_num}_PIN_NUM_{pin}
        #define SPI_{host_num}_PIN_NUM_{pin} -1
    #endif""")
            
            output.append("\n#endif")
        
        output.append("\n#endif // SPI_{}".format(host_num))
    
    output.append("\n#endif // PANEL_USE_SPI\n")
    
    # I2C consolidation - I2C_1 and I2C_2
    output.append("""
#ifdef PANEL_USE_I2C""")
    
    for host_num in range(1, 3):  # I2C_1 and I2C_2
        output.append(f"""
// ------------- I2C_{host_num} consolidation -------------
#ifdef I2C_{host_num}""")
        
        i2c_devices = get_devices_for_bus("I2C")
        
        # Build condition string
        conditions = []
        for dev in i2c_devices:
            conditions.append(f"(defined({dev}_I2C_HOST) && ({dev}_I2C_HOST == I2C_{host_num}))")
        
        if conditions:
            condition_str = " || ".join(conditions)
            output.append(f"""
#if {condition_str}
    #define I2C_{host_num}_HOST_USED""")
            
            # Consolidate each pin
            for pin in bus_pins["I2C"]:
                output.append(f"""
    
    // Consolidate {pin} for I2C_{host_num}""")
                for dev in i2c_devices:
                    output.append(f"""
    #if defined({dev}_I2C_HOST) && ({dev}_I2C_HOST == I2C_{host_num}) && defined({dev}_PIN_NUM_{pin}) && ({dev}_PIN_NUM_{pin} != -1)
        #ifndef I2C_{host_num}_PIN_NUM_{pin}
            #define I2C_{host_num}_PIN_NUM_{pin} {dev}_PIN_NUM_{pin}
        #endif
    #endif""")
                
                # Always define the pin, defaulting to -1
                output.append(f"""
    #ifndef I2C_{host_num}_PIN_NUM_{pin}
        #define I2C_{host_num}_PIN_NUM_{pin} -1
    #endif""")
            
            # Handle I2C pullup consolidation
            output.append(f"""
    
    // Consolidate I2C pullup settings for I2C_{host_num}""")
            
            # Check for pullup mismatches
            pullup_checks = []
            for i, dev1 in enumerate(i2c_devices):
                for dev2 in i2c_devices[i+1:]:
                    pullup_checks.append(f"""
    #if defined({dev1}_I2C_HOST) && ({dev1}_I2C_HOST == I2C_{host_num}) && \\
        defined({dev2}_I2C_HOST) && ({dev2}_I2C_HOST == I2C_{host_num}) && \\
        defined({dev1}_I2C_PULLUP) && defined({dev2}_I2C_PULLUP) && \\
        ({dev1}_I2C_PULLUP != {dev2}_I2C_PULLUP)
        #warning "{dev1}_I2C_PULLUP and {dev2}_I2C_PULLUP mismatch on I2C_{host_num}"
    #endif""")
            
            output.extend(pullup_checks)
            
            # Set pullup if any device requests it
            for dev in i2c_devices:
                output.append(f"""
    #if defined({dev}_I2C_HOST) && ({dev}_I2C_HOST == I2C_{host_num}) && defined({dev}_I2C_PULLUP) && ({dev}_I2C_PULLUP)
        #ifndef I2C_{host_num}_PULLUP
            #define I2C_{host_num}_PULLUP 1
        #endif
    #endif""")
            
            output.append("\n#endif")
        
        output.append("\n#endif // I2C_{}".format(host_num))
    
    output.append("\n#endif // PANEL_USE_I2C\n")
    
    return "\n".join(output)

# Generate the file
with open("panel_pins.h", "w") as f:
    f.write("""// AUTO-GENERATED FILE - DO NOT EDIT
// Generated by panel_pins_gen.py
// This file handles shared bus pin verification and consolidation

#ifndef PANEL_PINS_H
#define PANEL_PINS_H

#include "panel_defs.h"
#include "panel_pin_aliases.h"
""")
    
    # FIRST: Include necessary headers
    header_includes = generate_header_includes()
    if header_includes:
        f.write(header_includes)
    
    f.write("""
// ============================================================================
// SHARED BUS PIN VERIFICATION AND CONSOLIDATION
// ============================================================================
""")
    
    # Generate checks for each bus type
    for bus_type in bus_pins.keys():
        # Same host checks (share pins)
        checks = generate_bus_checks(bus_type)
        if checks:
            f.write(f"\n// ============================================================================")
            f.write(f"\n// {bus_type} BUS SHARING (SAME HOST)")
            f.write(f"\n// ============================================================================\n")
            f.write(checks)
        
        # Different host checks (conflict detection only)
        cross_checks = generate_cross_host_checks(bus_type)
        if cross_checks:
            f.write(f"\n// ============================================================================")
            f.write(f"\n// {bus_type} CROSS-HOST CONFLICT DETECTION")
            f.write(f"\n// ============================================================================\n")
            f.write(cross_checks)
    
    # CS uniqueness checks
    cs_checks = generate_cs_uniqueness_checks()
    if cs_checks:
        f.write(cs_checks)
    
    # Host consolidation (now happens AFTER headers are included)
    host_consolidation = generate_host_consolidation()
    if host_consolidation:
        f.write(host_consolidation)
    
    f.write("\n#endif // PANEL_PINS_H\n")

print("Generated panel_pins.h")