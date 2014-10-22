#
# Product-specific compile-time definitions.
#
PRODUCT_CURRENT := sc_imx6/scripts/icontrol/imx6_bbp
PRODUCT_COPY := sc_imx6/src/rootfs

BOARD_SOC_TYPE := IMX6DQ
BOARD_HAVE_VPU := true
HAVE_FSL_IMX_GPU2D := true
HAVE_FSL_IMX_GPU3D := true
HAVE_FSL_IMX_IPU := true
BOARD_KERNEL_BASE := 0x10800000
TARGET_KERNEL_DEFCONF := imx6_android_defconfig
HAVE_FSL_IMX_CODEC := true
TARGET_HAVE_IMX_GRALLOC := true
TARGET_HAVE_IMX_HWCOMPOSER = true
USE_OPENGL_RENDERER := true
TARGET_CPU_SMP := true


include $(PRODUCT_CURRENT)/build_id.mk
include device/fsl/imx6/BoardConfigCommon.mk
include $(PRODUCT_CURRENT)/BoardConfigCommon.mk
include device/fsl-proprietary/gpu-viv/fsl-gpu.mk


# BUILD_TARGET_FS is from BoardConfig or build environment
# e.g make BUILD_TARGET_FS=ubifs for NAND device
# e.g make BUILD_TARGET_FS=ext4 for eMMC/SD
# sabreauto_6dq default target for EXT4
#
# sabresd_6dq default target for EXT4
BUILD_TARGET_FS ?= ext4
# build for ext4
TARGET_USERIMAGES_USE_EXT4 := true
TARGET_USERIMAGES_USE_UBIFS := false

TARGET_RECOVERY_FSTAB = $(PRODUCT_COPY)/fstab.bbp
# build for ext4
PRODUCT_COPY_FILES +=	\
	$(PRODUCT_COPY)/fstab.bbp:root/fstab.bbp

TARGET_BOOTLOADER_BOARD_NAME := COMCAST
PRODUCT_MODEL := COMCAST-BBP

# UNITE is a virtual device support both atheros and realtek wifi(ar6103 and rtl8723as)
BOARD_WLAN_DEVICE            := UNITE
WPA_SUPPLICANT_VERSION       := VER_0_8_UNITE
TARGET_KERNEL_MODULES        := \
                                kernel_imx/drivers/net/wireless/rtl8723as/8723as.ko:system/lib/modules/8723as.ko \
                                kernel_imx/net/wireless/cfg80211.ko:system/lib/modules/cfg80211_realtek.ko
BOARD_WPA_SUPPLICANT_DRIVER  := NL80211
BOARD_HOSTAPD_DRIVER         := NL80211

BOARD_HOSTAPD_PRIVATE_LIB_QCOM              := lib_driver_cmd_qcwcn
BOARD_WPA_SUPPLICANT_PRIVATE_LIB_QCOM       := lib_driver_cmd_qcwcn
BOARD_HOSTAPD_PRIVATE_LIB_RTL               := lib_driver_cmd_rtl
BOARD_WPA_SUPPLICANT_PRIVATE_LIB_RTL        := lib_driver_cmd_rtl
#for intel vendor
ifeq ($(BOARD_WLAN_VENDOR),INTEL)
BOARD_HOSTAPD_PRIVATE_LIB                := private_lib_driver_cmd
BOARD_WPA_SUPPLICANT_PRIVATE_LIB         := private_lib_driver_cmd
WPA_SUPPLICANT_VERSION                   := VER_0_8_X
HOSTAPD_VERSION                          := VER_0_8_X
BOARD_WPA_SUPPLICANT_PRIVATE_LIB         := private_lib_driver_cmd_intel
WIFI_DRIVER_MODULE_PATH                  := "/system/lib/modules/iwlagn.ko"
WIFI_DRIVER_MODULE_NAME                  := "iwlagn"
WIFI_DRIVER_MODULE_PATH                  ?= auto
endif

BOARD_MODEM_VENDOR := AMAZON

USE_ATHR_GPS_HARDWARE := true
USE_QEMU_GPS_HARDWARE := false

#for accelerator sensor, need to define sensor type here
BOARD_HAS_SENSOR := true
SENSOR_MMA8451 := true

# for recovery service
TARGET_SELECT_KEY := 28

# we don't support sparse image.
TARGET_USERIMAGES_SPARSE_EXT_DISABLED := true

# uncomment below lins if use NAND
#TARGET_USERIMAGES_USE_UBIFS = true

ifeq ($(TARGET_USERIMAGES_USE_UBIFS),true)
ifeq ($(TARGET_USERIMAGES_USE_EXT4),true)
$(error "TARGET_USERIMAGES_USE_UBIFS and TARGET_USERIMAGES_USE_EXT4 config open in same time, please only choose one target file system image")
endif
endif

BOARD_KERNEL_CMDLINE := console=ttymxc0,115200 init=/init video=mxcfb0:dev=ldb,bpp=32 video=mxcfb1:off video=mxcfb2:off fbmem=10M fb0base=0x27b00000 vmalloc=400M androidboot.console=ttymxc0 androidboot.hardware=bbp

# atheros 3k BT
BOARD_USE_AR3K_BLUETOOTH := true
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := $(PRODUCT_COPY)/bluetooth

USE_ION_ALLOCATOR := false
USE_GPU_ALLOCATOR := true

# camera hal v2
IMX_CAMERA_HAL_V2 := true

# define frame buffer count
NUM_FRAMEBUFFER_SURFACE_BUFFERS := 3


TARGET_BOOTLOADER_CONFIG := 6q:mx6q_sabresd_android_config
