############### sercomm add #####################
#sercomm hal
TARGET_SC_HAL := true
#sercomm add test
TARGET_SC_KMODS_TEST := false
TARGET_SC_HAL_TEST := false
TARGET_SC_APP_TEST := false
TARGET_SC_FRAMWORKS_TEST := false
TARGET_SC_PACKAGE_TEST := false

TARGET_SC_LIGHTTPD_WEBSERVER := true
TARGET_SC_APP_KEYPAD := true

############### icontrol hal ####################
TARGET_CUSTOM_IC_HAL := false
TARGET_CUSTOM_IC_HAL_SAMPLE := false

#################################################
#icontrol hal details
ifeq ($(TARGET_CUSTOM_IC_HAL), true)
TARGET_CUSTOM_IC_HAL_BATTERY := true
TARGET_CUSTOM_IC_HAL_BUTTON := true
TARGET_CUSTOM_IC_HAL_LED := true
TARGET_CUSTOM_IC_HAL_LPM := true
TARGET_CUSTOM_IC_HAL_SYSINFO := true
endif

ifeq ($(TARGET_CUSTOM_IC_HAL_SAMPLE), true)
TARGET_CUSTOM_IC_HAL_BATTERY_SAMPLE := true
TARGET_CUSTOM_IC_HAL_BUTTON_SAMPLE := true
TARGET_CUSTOM_IC_HAL_LED_SAMPLE := true
TARGET_CUSTOM_IC_HAL_LPM_SAMPLE := true
TARGET_CUSTOM_IC_HAL_SYSINFO_SAMPLE := true
endif
#################################################

