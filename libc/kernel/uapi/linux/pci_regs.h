/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef LINUX_PCI_REGS_H
#define LINUX_PCI_REGS_H
#define PCI_CFG_SPACE_SIZE 256
#define PCI_CFG_SPACE_EXP_SIZE 4096
#define PCI_STD_HEADER_SIZEOF 64
#define PCI_STD_NUM_BARS 6
#define PCI_VENDOR_ID 0x00
#define PCI_DEVICE_ID 0x02
#define PCI_COMMAND 0x04
#define PCI_COMMAND_IO 0x1
#define PCI_COMMAND_MEMORY 0x2
#define PCI_COMMAND_MASTER 0x4
#define PCI_COMMAND_SPECIAL 0x8
#define PCI_COMMAND_INVALIDATE 0x10
#define PCI_COMMAND_VGA_PALETTE 0x20
#define PCI_COMMAND_PARITY 0x40
#define PCI_COMMAND_WAIT 0x80
#define PCI_COMMAND_SERR 0x100
#define PCI_COMMAND_FAST_BACK 0x200
#define PCI_COMMAND_INTX_DISABLE 0x400
#define PCI_STATUS 0x06
#define PCI_STATUS_IMM_READY 0x01
#define PCI_STATUS_INTERRUPT 0x08
#define PCI_STATUS_CAP_LIST 0x10
#define PCI_STATUS_66MHZ 0x20
#define PCI_STATUS_UDF 0x40
#define PCI_STATUS_FAST_BACK 0x80
#define PCI_STATUS_PARITY 0x100
#define PCI_STATUS_DEVSEL_MASK 0x600
#define PCI_STATUS_DEVSEL_FAST 0x000
#define PCI_STATUS_DEVSEL_MEDIUM 0x200
#define PCI_STATUS_DEVSEL_SLOW 0x400
#define PCI_STATUS_SIG_TARGET_ABORT 0x800
#define PCI_STATUS_REC_TARGET_ABORT 0x1000
#define PCI_STATUS_REC_MASTER_ABORT 0x2000
#define PCI_STATUS_SIG_SYSTEM_ERROR 0x4000
#define PCI_STATUS_DETECTED_PARITY 0x8000
#define PCI_CLASS_REVISION 0x08
#define PCI_REVISION_ID 0x08
#define PCI_CLASS_PROG 0x09
#define PCI_CLASS_DEVICE 0x0a
#define PCI_CACHE_LINE_SIZE 0x0c
#define PCI_LATENCY_TIMER 0x0d
#define PCI_HEADER_TYPE 0x0e
#define PCI_HEADER_TYPE_MASK 0x7f
#define PCI_HEADER_TYPE_NORMAL 0
#define PCI_HEADER_TYPE_BRIDGE 1
#define PCI_HEADER_TYPE_CARDBUS 2
#define PCI_BIST 0x0f
#define PCI_BIST_CODE_MASK 0x0f
#define PCI_BIST_START 0x40
#define PCI_BIST_CAPABLE 0x80
#define PCI_BASE_ADDRESS_0 0x10
#define PCI_BASE_ADDRESS_1 0x14
#define PCI_BASE_ADDRESS_2 0x18
#define PCI_BASE_ADDRESS_3 0x1c
#define PCI_BASE_ADDRESS_4 0x20
#define PCI_BASE_ADDRESS_5 0x24
#define PCI_BASE_ADDRESS_SPACE 0x01
#define PCI_BASE_ADDRESS_SPACE_IO 0x01
#define PCI_BASE_ADDRESS_SPACE_MEMORY 0x00
#define PCI_BASE_ADDRESS_MEM_TYPE_MASK 0x06
#define PCI_BASE_ADDRESS_MEM_TYPE_32 0x00
#define PCI_BASE_ADDRESS_MEM_TYPE_1M 0x02
#define PCI_BASE_ADDRESS_MEM_TYPE_64 0x04
#define PCI_BASE_ADDRESS_MEM_PREFETCH 0x08
#define PCI_BASE_ADDRESS_MEM_MASK (~0x0fUL)
#define PCI_BASE_ADDRESS_IO_MASK (~0x03UL)
#define PCI_CARDBUS_CIS 0x28
#define PCI_SUBSYSTEM_VENDOR_ID 0x2c
#define PCI_SUBSYSTEM_ID 0x2e
#define PCI_ROM_ADDRESS 0x30
#define PCI_ROM_ADDRESS_ENABLE 0x01
#define PCI_ROM_ADDRESS_MASK (~0x7ffU)
#define PCI_CAPABILITY_LIST 0x34
#define PCI_INTERRUPT_LINE 0x3c
#define PCI_INTERRUPT_PIN 0x3d
#define PCI_MIN_GNT 0x3e
#define PCI_MAX_LAT 0x3f
#define PCI_PRIMARY_BUS 0x18
#define PCI_SECONDARY_BUS 0x19
#define PCI_SUBORDINATE_BUS 0x1a
#define PCI_SEC_LATENCY_TIMER 0x1b
#define PCI_IO_BASE 0x1c
#define PCI_IO_LIMIT 0x1d
#define PCI_IO_RANGE_TYPE_MASK 0x0fUL
#define PCI_IO_RANGE_TYPE_16 0x00
#define PCI_IO_RANGE_TYPE_32 0x01
#define PCI_IO_RANGE_MASK (~0x0fUL)
#define PCI_IO_1K_RANGE_MASK (~0x03UL)
#define PCI_SEC_STATUS 0x1e
#define PCI_MEMORY_BASE 0x20
#define PCI_MEMORY_LIMIT 0x22
#define PCI_MEMORY_RANGE_TYPE_MASK 0x0fUL
#define PCI_MEMORY_RANGE_MASK (~0x0fUL)
#define PCI_PREF_MEMORY_BASE 0x24
#define PCI_PREF_MEMORY_LIMIT 0x26
#define PCI_PREF_RANGE_TYPE_MASK 0x0fUL
#define PCI_PREF_RANGE_TYPE_32 0x00
#define PCI_PREF_RANGE_TYPE_64 0x01
#define PCI_PREF_RANGE_MASK (~0x0fUL)
#define PCI_PREF_BASE_UPPER32 0x28
#define PCI_PREF_LIMIT_UPPER32 0x2c
#define PCI_IO_BASE_UPPER16 0x30
#define PCI_IO_LIMIT_UPPER16 0x32
#define PCI_ROM_ADDRESS1 0x38
#define PCI_BRIDGE_CONTROL 0x3e
#define PCI_BRIDGE_CTL_PARITY 0x01
#define PCI_BRIDGE_CTL_SERR 0x02
#define PCI_BRIDGE_CTL_ISA 0x04
#define PCI_BRIDGE_CTL_VGA 0x08
#define PCI_BRIDGE_CTL_MASTER_ABORT 0x20
#define PCI_BRIDGE_CTL_BUS_RESET 0x40
#define PCI_BRIDGE_CTL_FAST_BACK 0x80
#define PCI_CB_CAPABILITY_LIST 0x14
#define PCI_CB_SEC_STATUS 0x16
#define PCI_CB_PRIMARY_BUS 0x18
#define PCI_CB_CARD_BUS 0x19
#define PCI_CB_SUBORDINATE_BUS 0x1a
#define PCI_CB_LATENCY_TIMER 0x1b
#define PCI_CB_MEMORY_BASE_0 0x1c
#define PCI_CB_MEMORY_LIMIT_0 0x20
#define PCI_CB_MEMORY_BASE_1 0x24
#define PCI_CB_MEMORY_LIMIT_1 0x28
#define PCI_CB_IO_BASE_0 0x2c
#define PCI_CB_IO_BASE_0_HI 0x2e
#define PCI_CB_IO_LIMIT_0 0x30
#define PCI_CB_IO_LIMIT_0_HI 0x32
#define PCI_CB_IO_BASE_1 0x34
#define PCI_CB_IO_BASE_1_HI 0x36
#define PCI_CB_IO_LIMIT_1 0x38
#define PCI_CB_IO_LIMIT_1_HI 0x3a
#define PCI_CB_IO_RANGE_MASK (~0x03UL)
#define PCI_CB_BRIDGE_CONTROL 0x3e
#define PCI_CB_BRIDGE_CTL_PARITY 0x01
#define PCI_CB_BRIDGE_CTL_SERR 0x02
#define PCI_CB_BRIDGE_CTL_ISA 0x04
#define PCI_CB_BRIDGE_CTL_VGA 0x08
#define PCI_CB_BRIDGE_CTL_MASTER_ABORT 0x20
#define PCI_CB_BRIDGE_CTL_CB_RESET 0x40
#define PCI_CB_BRIDGE_CTL_16BIT_INT 0x80
#define PCI_CB_BRIDGE_CTL_PREFETCH_MEM0 0x100
#define PCI_CB_BRIDGE_CTL_PREFETCH_MEM1 0x200
#define PCI_CB_BRIDGE_CTL_POST_WRITES 0x400
#define PCI_CB_SUBSYSTEM_VENDOR_ID 0x40
#define PCI_CB_SUBSYSTEM_ID 0x42
#define PCI_CB_LEGACY_MODE_BASE 0x44
#define PCI_CAP_LIST_ID 0
#define PCI_CAP_ID_PM 0x01
#define PCI_CAP_ID_AGP 0x02
#define PCI_CAP_ID_VPD 0x03
#define PCI_CAP_ID_SLOTID 0x04
#define PCI_CAP_ID_MSI 0x05
#define PCI_CAP_ID_CHSWP 0x06
#define PCI_CAP_ID_PCIX 0x07
#define PCI_CAP_ID_HT 0x08
#define PCI_CAP_ID_VNDR 0x09
#define PCI_CAP_ID_DBG 0x0A
#define PCI_CAP_ID_CCRC 0x0B
#define PCI_CAP_ID_SHPC 0x0C
#define PCI_CAP_ID_SSVID 0x0D
#define PCI_CAP_ID_AGP3 0x0E
#define PCI_CAP_ID_SECDEV 0x0F
#define PCI_CAP_ID_EXP 0x10
#define PCI_CAP_ID_MSIX 0x11
#define PCI_CAP_ID_SATA 0x12
#define PCI_CAP_ID_AF 0x13
#define PCI_CAP_ID_EA 0x14
#define PCI_CAP_ID_MAX PCI_CAP_ID_EA
#define PCI_CAP_LIST_NEXT 1
#define PCI_CAP_FLAGS 2
#define PCI_CAP_SIZEOF 4
#define PCI_PM_PMC 2
#define PCI_PM_CAP_VER_MASK 0x0007
#define PCI_PM_CAP_PME_CLOCK 0x0008
#define PCI_PM_CAP_RESERVED 0x0010
#define PCI_PM_CAP_DSI 0x0020
#define PCI_PM_CAP_AUX_POWER 0x01C0
#define PCI_PM_CAP_D1 0x0200
#define PCI_PM_CAP_D2 0x0400
#define PCI_PM_CAP_PME 0x0800
#define PCI_PM_CAP_PME_MASK 0xF800
#define PCI_PM_CAP_PME_D0 0x0800
#define PCI_PM_CAP_PME_D1 0x1000
#define PCI_PM_CAP_PME_D2 0x2000
#define PCI_PM_CAP_PME_D3hot 0x4000
#define PCI_PM_CAP_PME_D3cold 0x8000
#define PCI_PM_CAP_PME_SHIFT 11
#define PCI_PM_CTRL 4
#define PCI_PM_CTRL_STATE_MASK 0x0003
#define PCI_PM_CTRL_NO_SOFT_RESET 0x0008
#define PCI_PM_CTRL_PME_ENABLE 0x0100
#define PCI_PM_CTRL_DATA_SEL_MASK 0x1e00
#define PCI_PM_CTRL_DATA_SCALE_MASK 0x6000
#define PCI_PM_CTRL_PME_STATUS 0x8000
#define PCI_PM_PPB_EXTENSIONS 6
#define PCI_PM_PPB_B2_B3 0x40
#define PCI_PM_BPCC_ENABLE 0x80
#define PCI_PM_DATA_REGISTER 7
#define PCI_PM_SIZEOF 8
#define PCI_AGP_VERSION 2
#define PCI_AGP_RFU 3
#define PCI_AGP_STATUS 4
#define PCI_AGP_STATUS_RQ_MASK 0xff000000
#define PCI_AGP_STATUS_SBA 0x0200
#define PCI_AGP_STATUS_64BIT 0x0020
#define PCI_AGP_STATUS_FW 0x0010
#define PCI_AGP_STATUS_RATE4 0x0004
#define PCI_AGP_STATUS_RATE2 0x0002
#define PCI_AGP_STATUS_RATE1 0x0001
#define PCI_AGP_COMMAND 8
#define PCI_AGP_COMMAND_RQ_MASK 0xff000000
#define PCI_AGP_COMMAND_SBA 0x0200
#define PCI_AGP_COMMAND_AGP 0x0100
#define PCI_AGP_COMMAND_64BIT 0x0020
#define PCI_AGP_COMMAND_FW 0x0010
#define PCI_AGP_COMMAND_RATE4 0x0004
#define PCI_AGP_COMMAND_RATE2 0x0002
#define PCI_AGP_COMMAND_RATE1 0x0001
#define PCI_AGP_SIZEOF 12
#define PCI_VPD_ADDR 2
#define PCI_VPD_ADDR_MASK 0x7fff
#define PCI_VPD_ADDR_F 0x8000
#define PCI_VPD_DATA 4
#define PCI_CAP_VPD_SIZEOF 8
#define PCI_SID_ESR 2
#define PCI_SID_ESR_NSLOTS 0x1f
#define PCI_SID_ESR_FIC 0x20
#define PCI_SID_CHASSIS_NR 3
#define PCI_MSI_FLAGS 0x02
#define PCI_MSI_FLAGS_ENABLE 0x0001
#define PCI_MSI_FLAGS_QMASK 0x000e
#define PCI_MSI_FLAGS_QSIZE 0x0070
#define PCI_MSI_FLAGS_64BIT 0x0080
#define PCI_MSI_FLAGS_MASKBIT 0x0100
#define PCI_MSI_RFU 3
#define PCI_MSI_ADDRESS_LO 0x04
#define PCI_MSI_ADDRESS_HI 0x08
#define PCI_MSI_DATA_32 0x08
#define PCI_MSI_MASK_32 0x0c
#define PCI_MSI_PENDING_32 0x10
#define PCI_MSI_DATA_64 0x0c
#define PCI_MSI_MASK_64 0x10
#define PCI_MSI_PENDING_64 0x14
#define PCI_MSIX_FLAGS 2
#define PCI_MSIX_FLAGS_QSIZE 0x07FF
#define PCI_MSIX_FLAGS_MASKALL 0x4000
#define PCI_MSIX_FLAGS_ENABLE 0x8000
#define PCI_MSIX_TABLE 4
#define PCI_MSIX_TABLE_BIR 0x00000007
#define PCI_MSIX_TABLE_OFFSET 0xfffffff8
#define PCI_MSIX_PBA 8
#define PCI_MSIX_PBA_BIR 0x00000007
#define PCI_MSIX_PBA_OFFSET 0xfffffff8
#define PCI_MSIX_FLAGS_BIRMASK PCI_MSIX_PBA_BIR
#define PCI_CAP_MSIX_SIZEOF 12
#define PCI_MSIX_ENTRY_SIZE 16
#define PCI_MSIX_ENTRY_LOWER_ADDR 0x0
#define PCI_MSIX_ENTRY_UPPER_ADDR 0x4
#define PCI_MSIX_ENTRY_DATA 0x8
#define PCI_MSIX_ENTRY_VECTOR_CTRL 0xc
#define PCI_MSIX_ENTRY_CTRL_MASKBIT 0x00000001
#define PCI_CHSWP_CSR 2
#define PCI_CHSWP_DHA 0x01
#define PCI_CHSWP_EIM 0x02
#define PCI_CHSWP_PIE 0x04
#define PCI_CHSWP_LOO 0x08
#define PCI_CHSWP_PI 0x30
#define PCI_CHSWP_EXT 0x40
#define PCI_CHSWP_INS 0x80
#define PCI_AF_LENGTH 2
#define PCI_AF_CAP 3
#define PCI_AF_CAP_TP 0x01
#define PCI_AF_CAP_FLR 0x02
#define PCI_AF_CTRL 4
#define PCI_AF_CTRL_FLR 0x01
#define PCI_AF_STATUS 5
#define PCI_AF_STATUS_TP 0x01
#define PCI_CAP_AF_SIZEOF 6
#define PCI_EA_NUM_ENT 2
#define PCI_EA_NUM_ENT_MASK 0x3f
#define PCI_EA_FIRST_ENT 4
#define PCI_EA_FIRST_ENT_BRIDGE 8
#define PCI_EA_ES 0x00000007
#define PCI_EA_BEI 0x000000f0
#define PCI_EA_SEC_BUS_MASK 0xff
#define PCI_EA_SUB_BUS_MASK 0xff00
#define PCI_EA_SUB_BUS_SHIFT 8
#define PCI_EA_BEI_BAR0 0
#define PCI_EA_BEI_BAR5 5
#define PCI_EA_BEI_BRIDGE 6
#define PCI_EA_BEI_ENI 7
#define PCI_EA_BEI_ROM 8
#define PCI_EA_BEI_VF_BAR0 9
#define PCI_EA_BEI_VF_BAR5 14
#define PCI_EA_BEI_RESERVED 15
#define PCI_EA_PP 0x0000ff00
#define PCI_EA_SP 0x00ff0000
#define PCI_EA_P_MEM 0x00
#define PCI_EA_P_MEM_PREFETCH 0x01
#define PCI_EA_P_IO 0x02
#define PCI_EA_P_VF_MEM_PREFETCH 0x03
#define PCI_EA_P_VF_MEM 0x04
#define PCI_EA_P_BRIDGE_MEM 0x05
#define PCI_EA_P_BRIDGE_MEM_PREFETCH 0x06
#define PCI_EA_P_BRIDGE_IO 0x07
#define PCI_EA_P_MEM_RESERVED 0xfd
#define PCI_EA_P_IO_RESERVED 0xfe
#define PCI_EA_P_UNAVAILABLE 0xff
#define PCI_EA_WRITABLE 0x40000000
#define PCI_EA_ENABLE 0x80000000
#define PCI_EA_BASE 4
#define PCI_EA_MAX_OFFSET 8
#define PCI_EA_IS_64 0x00000002
#define PCI_EA_FIELD_MASK 0xfffffffc
#define PCI_X_CMD 2
#define PCI_X_CMD_DPERR_E 0x0001
#define PCI_X_CMD_ERO 0x0002
#define PCI_X_CMD_READ_512 0x0000
#define PCI_X_CMD_READ_1K 0x0004
#define PCI_X_CMD_READ_2K 0x0008
#define PCI_X_CMD_READ_4K 0x000c
#define PCI_X_CMD_MAX_READ 0x000c
#define PCI_X_CMD_SPLIT_1 0x0000
#define PCI_X_CMD_SPLIT_2 0x0010
#define PCI_X_CMD_SPLIT_3 0x0020
#define PCI_X_CMD_SPLIT_4 0x0030
#define PCI_X_CMD_SPLIT_8 0x0040
#define PCI_X_CMD_SPLIT_12 0x0050
#define PCI_X_CMD_SPLIT_16 0x0060
#define PCI_X_CMD_SPLIT_32 0x0070
#define PCI_X_CMD_MAX_SPLIT 0x0070
#define PCI_X_CMD_VERSION(x) (((x) >> 12) & 3)
#define PCI_X_STATUS 4
#define PCI_X_STATUS_DEVFN 0x000000ff
#define PCI_X_STATUS_BUS 0x0000ff00
#define PCI_X_STATUS_64BIT 0x00010000
#define PCI_X_STATUS_133MHZ 0x00020000
#define PCI_X_STATUS_SPL_DISC 0x00040000
#define PCI_X_STATUS_UNX_SPL 0x00080000
#define PCI_X_STATUS_COMPLEX 0x00100000
#define PCI_X_STATUS_MAX_READ 0x00600000
#define PCI_X_STATUS_MAX_SPLIT 0x03800000
#define PCI_X_STATUS_MAX_CUM 0x1c000000
#define PCI_X_STATUS_SPL_ERR 0x20000000
#define PCI_X_STATUS_266MHZ 0x40000000
#define PCI_X_STATUS_533MHZ 0x80000000
#define PCI_X_ECC_CSR 8
#define PCI_CAP_PCIX_SIZEOF_V0 8
#define PCI_CAP_PCIX_SIZEOF_V1 24
#define PCI_CAP_PCIX_SIZEOF_V2 PCI_CAP_PCIX_SIZEOF_V1
#define PCI_X_BRIDGE_SSTATUS 2
#define PCI_X_SSTATUS_64BIT 0x0001
#define PCI_X_SSTATUS_133MHZ 0x0002
#define PCI_X_SSTATUS_FREQ 0x03c0
#define PCI_X_SSTATUS_VERS 0x3000
#define PCI_X_SSTATUS_V1 0x1000
#define PCI_X_SSTATUS_V2 0x2000
#define PCI_X_SSTATUS_266MHZ 0x4000
#define PCI_X_SSTATUS_533MHZ 0x8000
#define PCI_X_BRIDGE_STATUS 4
#define PCI_SSVID_VENDOR_ID 4
#define PCI_SSVID_DEVICE_ID 6
#define PCI_EXP_FLAGS 0x02
#define PCI_EXP_FLAGS_VERS 0x000f
#define PCI_EXP_FLAGS_TYPE 0x00f0
#define PCI_EXP_TYPE_ENDPOINT 0x0
#define PCI_EXP_TYPE_LEG_END 0x1
#define PCI_EXP_TYPE_ROOT_PORT 0x4
#define PCI_EXP_TYPE_UPSTREAM 0x5
#define PCI_EXP_TYPE_DOWNSTREAM 0x6
#define PCI_EXP_TYPE_PCI_BRIDGE 0x7
#define PCI_EXP_TYPE_PCIE_BRIDGE 0x8
#define PCI_EXP_TYPE_RC_END 0x9
#define PCI_EXP_TYPE_RC_EC 0xa
#define PCI_EXP_FLAGS_SLOT 0x0100
#define PCI_EXP_FLAGS_IRQ 0x3e00
#define PCI_EXP_DEVCAP 0x04
#define PCI_EXP_DEVCAP_PAYLOAD 0x00000007
#define PCI_EXP_DEVCAP_PHANTOM 0x00000018
#define PCI_EXP_DEVCAP_EXT_TAG 0x00000020
#define PCI_EXP_DEVCAP_L0S 0x000001c0
#define PCI_EXP_DEVCAP_L1 0x00000e00
#define PCI_EXP_DEVCAP_ATN_BUT 0x00001000
#define PCI_EXP_DEVCAP_ATN_IND 0x00002000
#define PCI_EXP_DEVCAP_PWR_IND 0x00004000
#define PCI_EXP_DEVCAP_RBER 0x00008000
#define PCI_EXP_DEVCAP_PWR_VAL 0x03fc0000
#define PCI_EXP_DEVCAP_PWR_SCL 0x0c000000
#define PCI_EXP_DEVCAP_FLR 0x10000000
#define PCI_EXP_DEVCTL 0x08
#define PCI_EXP_DEVCTL_CERE 0x0001
#define PCI_EXP_DEVCTL_NFERE 0x0002
#define PCI_EXP_DEVCTL_FERE 0x0004
#define PCI_EXP_DEVCTL_URRE 0x0008
#define PCI_EXP_DEVCTL_RELAX_EN 0x0010
#define PCI_EXP_DEVCTL_PAYLOAD 0x00e0
#define PCI_EXP_DEVCTL_PAYLOAD_128B 0x0000
#define PCI_EXP_DEVCTL_PAYLOAD_256B 0x0020
#define PCI_EXP_DEVCTL_PAYLOAD_512B 0x0040
#define PCI_EXP_DEVCTL_PAYLOAD_1024B 0x0060
#define PCI_EXP_DEVCTL_PAYLOAD_2048B 0x0080
#define PCI_EXP_DEVCTL_PAYLOAD_4096B 0x00a0
#define PCI_EXP_DEVCTL_EXT_TAG 0x0100
#define PCI_EXP_DEVCTL_PHANTOM 0x0200
#define PCI_EXP_DEVCTL_AUX_PME 0x0400
#define PCI_EXP_DEVCTL_NOSNOOP_EN 0x0800
#define PCI_EXP_DEVCTL_READRQ 0x7000
#define PCI_EXP_DEVCTL_READRQ_128B 0x0000
#define PCI_EXP_DEVCTL_READRQ_256B 0x1000
#define PCI_EXP_DEVCTL_READRQ_512B 0x2000
#define PCI_EXP_DEVCTL_READRQ_1024B 0x3000
#define PCI_EXP_DEVCTL_READRQ_2048B 0x4000
#define PCI_EXP_DEVCTL_READRQ_4096B 0x5000
#define PCI_EXP_DEVCTL_BCR_FLR 0x8000
#define PCI_EXP_DEVSTA 0x0a
#define PCI_EXP_DEVSTA_CED 0x0001
#define PCI_EXP_DEVSTA_NFED 0x0002
#define PCI_EXP_DEVSTA_FED 0x0004
#define PCI_EXP_DEVSTA_URD 0x0008
#define PCI_EXP_DEVSTA_AUXPD 0x0010
#define PCI_EXP_DEVSTA_TRPND 0x0020
#define PCI_CAP_EXP_RC_ENDPOINT_SIZEOF_V1 12
#define PCI_EXP_LNKCAP 0x0c
#define PCI_EXP_LNKCAP_SLS 0x0000000f
#define PCI_EXP_LNKCAP_SLS_2_5GB 0x00000001
#define PCI_EXP_LNKCAP_SLS_5_0GB 0x00000002
#define PCI_EXP_LNKCAP_SLS_8_0GB 0x00000003
#define PCI_EXP_LNKCAP_SLS_16_0GB 0x00000004
#define PCI_EXP_LNKCAP_SLS_32_0GB 0x00000005
#define PCI_EXP_LNKCAP_SLS_64_0GB 0x00000006
#define PCI_EXP_LNKCAP_MLW 0x000003f0
#define PCI_EXP_LNKCAP_ASPMS 0x00000c00
#define PCI_EXP_LNKCAP_ASPM_L0S 0x00000400
#define PCI_EXP_LNKCAP_ASPM_L1 0x00000800
#define PCI_EXP_LNKCAP_L0SEL 0x00007000
#define PCI_EXP_LNKCAP_L1EL 0x00038000
#define PCI_EXP_LNKCAP_CLKPM 0x00040000
#define PCI_EXP_LNKCAP_SDERC 0x00080000
#define PCI_EXP_LNKCAP_DLLLARC 0x00100000
#define PCI_EXP_LNKCAP_LBNC 0x00200000
#define PCI_EXP_LNKCAP_PN 0xff000000
#define PCI_EXP_LNKCTL 0x10
#define PCI_EXP_LNKCTL_ASPMC 0x0003
#define PCI_EXP_LNKCTL_ASPM_L0S 0x0001
#define PCI_EXP_LNKCTL_ASPM_L1 0x0002
#define PCI_EXP_LNKCTL_RCB 0x0008
#define PCI_EXP_LNKCTL_LD 0x0010
#define PCI_EXP_LNKCTL_RL 0x0020
#define PCI_EXP_LNKCTL_CCC 0x0040
#define PCI_EXP_LNKCTL_ES 0x0080
#define PCI_EXP_LNKCTL_CLKREQ_EN 0x0100
#define PCI_EXP_LNKCTL_HAWD 0x0200
#define PCI_EXP_LNKCTL_LBMIE 0x0400
#define PCI_EXP_LNKCTL_LABIE 0x0800
#define PCI_EXP_LNKSTA 0x12
#define PCI_EXP_LNKSTA_CLS 0x000f
#define PCI_EXP_LNKSTA_CLS_2_5GB 0x0001
#define PCI_EXP_LNKSTA_CLS_5_0GB 0x0002
#define PCI_EXP_LNKSTA_CLS_8_0GB 0x0003
#define PCI_EXP_LNKSTA_CLS_16_0GB 0x0004
#define PCI_EXP_LNKSTA_CLS_32_0GB 0x0005
#define PCI_EXP_LNKSTA_CLS_64_0GB 0x0006
#define PCI_EXP_LNKSTA_NLW 0x03f0
#define PCI_EXP_LNKSTA_NLW_X1 0x0010
#define PCI_EXP_LNKSTA_NLW_X2 0x0020
#define PCI_EXP_LNKSTA_NLW_X4 0x0040
#define PCI_EXP_LNKSTA_NLW_X8 0x0080
#define PCI_EXP_LNKSTA_NLW_SHIFT 4
#define PCI_EXP_LNKSTA_LT 0x0800
#define PCI_EXP_LNKSTA_SLC 0x1000
#define PCI_EXP_LNKSTA_DLLLA 0x2000
#define PCI_EXP_LNKSTA_LBMS 0x4000
#define PCI_EXP_LNKSTA_LABS 0x8000
#define PCI_CAP_EXP_ENDPOINT_SIZEOF_V1 20
#define PCI_EXP_SLTCAP 0x14
#define PCI_EXP_SLTCAP_ABP 0x00000001
#define PCI_EXP_SLTCAP_PCP 0x00000002
#define PCI_EXP_SLTCAP_MRLSP 0x00000004
#define PCI_EXP_SLTCAP_AIP 0x00000008
#define PCI_EXP_SLTCAP_PIP 0x00000010
#define PCI_EXP_SLTCAP_HPS 0x00000020
#define PCI_EXP_SLTCAP_HPC 0x00000040
#define PCI_EXP_SLTCAP_SPLV 0x00007f80
#define PCI_EXP_SLTCAP_SPLS 0x00018000
#define PCI_EXP_SLTCAP_EIP 0x00020000
#define PCI_EXP_SLTCAP_NCCS 0x00040000
#define PCI_EXP_SLTCAP_PSN 0xfff80000
#define PCI_EXP_SLTCTL 0x18
#define PCI_EXP_SLTCTL_ABPE 0x0001
#define PCI_EXP_SLTCTL_PFDE 0x0002
#define PCI_EXP_SLTCTL_MRLSCE 0x0004
#define PCI_EXP_SLTCTL_PDCE 0x0008
#define PCI_EXP_SLTCTL_CCIE 0x0010
#define PCI_EXP_SLTCTL_HPIE 0x0020
#define PCI_EXP_SLTCTL_AIC 0x00c0
#define PCI_EXP_SLTCTL_ATTN_IND_SHIFT 6
#define PCI_EXP_SLTCTL_ATTN_IND_ON 0x0040
#define PCI_EXP_SLTCTL_ATTN_IND_BLINK 0x0080
#define PCI_EXP_SLTCTL_ATTN_IND_OFF 0x00c0
#define PCI_EXP_SLTCTL_PIC 0x0300
#define PCI_EXP_SLTCTL_PWR_IND_ON 0x0100
#define PCI_EXP_SLTCTL_PWR_IND_BLINK 0x0200
#define PCI_EXP_SLTCTL_PWR_IND_OFF 0x0300
#define PCI_EXP_SLTCTL_PCC 0x0400
#define PCI_EXP_SLTCTL_PWR_ON 0x0000
#define PCI_EXP_SLTCTL_PWR_OFF 0x0400
#define PCI_EXP_SLTCTL_EIC 0x0800
#define PCI_EXP_SLTCTL_DLLSCE 0x1000
#define PCI_EXP_SLTCTL_ASPL_DISABLE 0x2000
#define PCI_EXP_SLTCTL_IBPD_DISABLE 0x4000
#define PCI_EXP_SLTSTA 0x1a
#define PCI_EXP_SLTSTA_ABP 0x0001
#define PCI_EXP_SLTSTA_PFD 0x0002
#define PCI_EXP_SLTSTA_MRLSC 0x0004
#define PCI_EXP_SLTSTA_PDC 0x0008
#define PCI_EXP_SLTSTA_CC 0x0010
#define PCI_EXP_SLTSTA_MRLSS 0x0020
#define PCI_EXP_SLTSTA_PDS 0x0040
#define PCI_EXP_SLTSTA_EIS 0x0080
#define PCI_EXP_SLTSTA_DLLSC 0x0100
#define PCI_EXP_RTCTL 0x1c
#define PCI_EXP_RTCTL_SECEE 0x0001
#define PCI_EXP_RTCTL_SENFEE 0x0002
#define PCI_EXP_RTCTL_SEFEE 0x0004
#define PCI_EXP_RTCTL_PMEIE 0x0008
#define PCI_EXP_RTCTL_CRSSVE 0x0010
#define PCI_EXP_RTCAP 0x1e
#define PCI_EXP_RTCAP_CRSVIS 0x0001
#define PCI_EXP_RTSTA 0x20
#define PCI_EXP_RTSTA_PME 0x00010000
#define PCI_EXP_RTSTA_PENDING 0x00020000
#define PCI_EXP_DEVCAP2 0x24
#define PCI_EXP_DEVCAP2_COMP_TMOUT_DIS 0x00000010
#define PCI_EXP_DEVCAP2_ARI 0x00000020
#define PCI_EXP_DEVCAP2_ATOMIC_ROUTE 0x00000040
#define PCI_EXP_DEVCAP2_ATOMIC_COMP32 0x00000080
#define PCI_EXP_DEVCAP2_ATOMIC_COMP64 0x00000100
#define PCI_EXP_DEVCAP2_ATOMIC_COMP128 0x00000200
#define PCI_EXP_DEVCAP2_LTR 0x00000800
#define PCI_EXP_DEVCAP2_OBFF_MASK 0x000c0000
#define PCI_EXP_DEVCAP2_OBFF_MSG 0x00040000
#define PCI_EXP_DEVCAP2_OBFF_WAKE 0x00080000
#define PCI_EXP_DEVCAP2_EE_PREFIX 0x00200000
#define PCI_EXP_DEVCTL2 0x28
#define PCI_EXP_DEVCTL2_COMP_TIMEOUT 0x000f
#define PCI_EXP_DEVCTL2_COMP_TMOUT_DIS 0x0010
#define PCI_EXP_DEVCTL2_ARI 0x0020
#define PCI_EXP_DEVCTL2_ATOMIC_REQ 0x0040
#define PCI_EXP_DEVCTL2_ATOMIC_EGRESS_BLOCK 0x0080
#define PCI_EXP_DEVCTL2_IDO_REQ_EN 0x0100
#define PCI_EXP_DEVCTL2_IDO_CMP_EN 0x0200
#define PCI_EXP_DEVCTL2_LTR_EN 0x0400
#define PCI_EXP_DEVCTL2_OBFF_MSGA_EN 0x2000
#define PCI_EXP_DEVCTL2_OBFF_MSGB_EN 0x4000
#define PCI_EXP_DEVCTL2_OBFF_WAKE_EN 0x6000
#define PCI_EXP_DEVSTA2 0x2a
#define PCI_CAP_EXP_RC_ENDPOINT_SIZEOF_V2 0x2c
#define PCI_EXP_LNKCAP2 0x2c
#define PCI_EXP_LNKCAP2_SLS_2_5GB 0x00000002
#define PCI_EXP_LNKCAP2_SLS_5_0GB 0x00000004
#define PCI_EXP_LNKCAP2_SLS_8_0GB 0x00000008
#define PCI_EXP_LNKCAP2_SLS_16_0GB 0x00000010
#define PCI_EXP_LNKCAP2_SLS_32_0GB 0x00000020
#define PCI_EXP_LNKCAP2_SLS_64_0GB 0x00000040
#define PCI_EXP_LNKCAP2_CROSSLINK 0x00000100
#define PCI_EXP_LNKCTL2 0x30
#define PCI_EXP_LNKCTL2_TLS 0x000f
#define PCI_EXP_LNKCTL2_TLS_2_5GT 0x0001
#define PCI_EXP_LNKCTL2_TLS_5_0GT 0x0002
#define PCI_EXP_LNKCTL2_TLS_8_0GT 0x0003
#define PCI_EXP_LNKCTL2_TLS_16_0GT 0x0004
#define PCI_EXP_LNKCTL2_TLS_32_0GT 0x0005
#define PCI_EXP_LNKCTL2_TLS_64_0GT 0x0006
#define PCI_EXP_LNKCTL2_ENTER_COMP 0x0010
#define PCI_EXP_LNKCTL2_TX_MARGIN 0x0380
#define PCI_EXP_LNKCTL2_HASD 0x0020
#define PCI_EXP_LNKSTA2 0x32
#define PCI_CAP_EXP_ENDPOINT_SIZEOF_V2 0x32
#define PCI_EXP_SLTCAP2 0x34
#define PCI_EXP_SLTCAP2_IBPD 0x00000001
#define PCI_EXP_SLTCTL2 0x38
#define PCI_EXP_SLTSTA2 0x3a
#define PCI_EXT_CAP_ID(header) (header & 0x0000ffff)
#define PCI_EXT_CAP_VER(header) ((header >> 16) & 0xf)
#define PCI_EXT_CAP_NEXT(header) ((header >> 20) & 0xffc)
#define PCI_EXT_CAP_ID_ERR 0x01
#define PCI_EXT_CAP_ID_VC 0x02
#define PCI_EXT_CAP_ID_DSN 0x03
#define PCI_EXT_CAP_ID_PWR 0x04
#define PCI_EXT_CAP_ID_RCLD 0x05
#define PCI_EXT_CAP_ID_RCILC 0x06
#define PCI_EXT_CAP_ID_RCEC 0x07
#define PCI_EXT_CAP_ID_MFVC 0x08
#define PCI_EXT_CAP_ID_VC9 0x09
#define PCI_EXT_CAP_ID_RCRB 0x0A
#define PCI_EXT_CAP_ID_VNDR 0x0B
#define PCI_EXT_CAP_ID_CAC 0x0C
#define PCI_EXT_CAP_ID_ACS 0x0D
#define PCI_EXT_CAP_ID_ARI 0x0E
#define PCI_EXT_CAP_ID_ATS 0x0F
#define PCI_EXT_CAP_ID_SRIOV 0x10
#define PCI_EXT_CAP_ID_MRIOV 0x11
#define PCI_EXT_CAP_ID_MCAST 0x12
#define PCI_EXT_CAP_ID_PRI 0x13
#define PCI_EXT_CAP_ID_AMD_XXX 0x14
#define PCI_EXT_CAP_ID_REBAR 0x15
#define PCI_EXT_CAP_ID_DPA 0x16
#define PCI_EXT_CAP_ID_TPH 0x17
#define PCI_EXT_CAP_ID_LTR 0x18
#define PCI_EXT_CAP_ID_SECPCI 0x19
#define PCI_EXT_CAP_ID_PMUX 0x1A
#define PCI_EXT_CAP_ID_PASID 0x1B
#define PCI_EXT_CAP_ID_DPC 0x1D
#define PCI_EXT_CAP_ID_L1SS 0x1E
#define PCI_EXT_CAP_ID_PTM 0x1F
#define PCI_EXT_CAP_ID_DVSEC 0x23
#define PCI_EXT_CAP_ID_DLF 0x25
#define PCI_EXT_CAP_ID_PL_16GT 0x26
#define PCI_EXT_CAP_ID_DOE 0x2E
#define PCI_EXT_CAP_ID_MAX PCI_EXT_CAP_ID_DOE
#define PCI_EXT_CAP_DSN_SIZEOF 12
#define PCI_EXT_CAP_MCAST_ENDPOINT_SIZEOF 40
#define PCI_ERR_UNCOR_STATUS 0x04
#define PCI_ERR_UNC_UND 0x00000001
#define PCI_ERR_UNC_DLP 0x00000010
#define PCI_ERR_UNC_SURPDN 0x00000020
#define PCI_ERR_UNC_POISON_TLP 0x00001000
#define PCI_ERR_UNC_FCP 0x00002000
#define PCI_ERR_UNC_COMP_TIME 0x00004000
#define PCI_ERR_UNC_COMP_ABORT 0x00008000
#define PCI_ERR_UNC_UNX_COMP 0x00010000
#define PCI_ERR_UNC_RX_OVER 0x00020000
#define PCI_ERR_UNC_MALF_TLP 0x00040000
#define PCI_ERR_UNC_ECRC 0x00080000
#define PCI_ERR_UNC_UNSUP 0x00100000
#define PCI_ERR_UNC_ACSV 0x00200000
#define PCI_ERR_UNC_INTN 0x00400000
#define PCI_ERR_UNC_MCBTLP 0x00800000
#define PCI_ERR_UNC_ATOMEG 0x01000000
#define PCI_ERR_UNC_TLPPRE 0x02000000
#define PCI_ERR_UNCOR_MASK 0x08
#define PCI_ERR_UNCOR_SEVER 0x0c
#define PCI_ERR_COR_STATUS 0x10
#define PCI_ERR_COR_RCVR 0x00000001
#define PCI_ERR_COR_BAD_TLP 0x00000040
#define PCI_ERR_COR_BAD_DLLP 0x00000080
#define PCI_ERR_COR_REP_ROLL 0x00000100
#define PCI_ERR_COR_REP_TIMER 0x00001000
#define PCI_ERR_COR_ADV_NFAT 0x00002000
#define PCI_ERR_COR_INTERNAL 0x00004000
#define PCI_ERR_COR_LOG_OVER 0x00008000
#define PCI_ERR_COR_MASK 0x14
#define PCI_ERR_CAP 0x18
#define PCI_ERR_CAP_FEP(x) ((x) & 0x1f)
#define PCI_ERR_CAP_ECRC_GENC 0x00000020
#define PCI_ERR_CAP_ECRC_GENE 0x00000040
#define PCI_ERR_CAP_ECRC_CHKC 0x00000080
#define PCI_ERR_CAP_ECRC_CHKE 0x00000100
#define PCI_ERR_HEADER_LOG 0x1c
#define PCI_ERR_ROOT_COMMAND 0x2c
#define PCI_ERR_ROOT_CMD_COR_EN 0x00000001
#define PCI_ERR_ROOT_CMD_NONFATAL_EN 0x00000002
#define PCI_ERR_ROOT_CMD_FATAL_EN 0x00000004
#define PCI_ERR_ROOT_STATUS 0x30
#define PCI_ERR_ROOT_COR_RCV 0x00000001
#define PCI_ERR_ROOT_MULTI_COR_RCV 0x00000002
#define PCI_ERR_ROOT_UNCOR_RCV 0x00000004
#define PCI_ERR_ROOT_MULTI_UNCOR_RCV 0x00000008
#define PCI_ERR_ROOT_FIRST_FATAL 0x00000010
#define PCI_ERR_ROOT_NONFATAL_RCV 0x00000020
#define PCI_ERR_ROOT_FATAL_RCV 0x00000040
#define PCI_ERR_ROOT_AER_IRQ 0xf8000000
#define PCI_ERR_ROOT_ERR_SRC 0x34
#define PCI_VC_PORT_CAP1 0x04
#define PCI_VC_CAP1_EVCC 0x00000007
#define PCI_VC_CAP1_LPEVCC 0x00000070
#define PCI_VC_CAP1_ARB_SIZE 0x00000c00
#define PCI_VC_PORT_CAP2 0x08
#define PCI_VC_CAP2_32_PHASE 0x00000002
#define PCI_VC_CAP2_64_PHASE 0x00000004
#define PCI_VC_CAP2_128_PHASE 0x00000008
#define PCI_VC_CAP2_ARB_OFF 0xff000000
#define PCI_VC_PORT_CTRL 0x0c
#define PCI_VC_PORT_CTRL_LOAD_TABLE 0x00000001
#define PCI_VC_PORT_STATUS 0x0e
#define PCI_VC_PORT_STATUS_TABLE 0x00000001
#define PCI_VC_RES_CAP 0x10
#define PCI_VC_RES_CAP_32_PHASE 0x00000002
#define PCI_VC_RES_CAP_64_PHASE 0x00000004
#define PCI_VC_RES_CAP_128_PHASE 0x00000008
#define PCI_VC_RES_CAP_128_PHASE_TB 0x00000010
#define PCI_VC_RES_CAP_256_PHASE 0x00000020
#define PCI_VC_RES_CAP_ARB_OFF 0xff000000
#define PCI_VC_RES_CTRL 0x14
#define PCI_VC_RES_CTRL_LOAD_TABLE 0x00010000
#define PCI_VC_RES_CTRL_ARB_SELECT 0x000e0000
#define PCI_VC_RES_CTRL_ID 0x07000000
#define PCI_VC_RES_CTRL_ENABLE 0x80000000
#define PCI_VC_RES_STATUS 0x1a
#define PCI_VC_RES_STATUS_TABLE 0x00000001
#define PCI_VC_RES_STATUS_NEGO 0x00000002
#define PCI_CAP_VC_BASE_SIZEOF 0x10
#define PCI_CAP_VC_PER_VC_SIZEOF 0x0c
#define PCI_PWR_DSR 0x04
#define PCI_PWR_DATA 0x08
#define PCI_PWR_DATA_BASE(x) ((x) & 0xff)
#define PCI_PWR_DATA_SCALE(x) (((x) >> 8) & 3)
#define PCI_PWR_DATA_PM_SUB(x) (((x) >> 10) & 7)
#define PCI_PWR_DATA_PM_STATE(x) (((x) >> 13) & 3)
#define PCI_PWR_DATA_TYPE(x) (((x) >> 15) & 7)
#define PCI_PWR_DATA_RAIL(x) (((x) >> 18) & 7)
#define PCI_PWR_CAP 0x0c
#define PCI_PWR_CAP_BUDGET(x) ((x) & 1)
#define PCI_EXT_CAP_PWR_SIZEOF 0x10
#define PCI_RCEC_RCIEP_BITMAP 4
#define PCI_RCEC_BUSN 8
#define PCI_RCEC_BUSN_REG_VER 0x02
#define PCI_RCEC_BUSN_NEXT(x) (((x) >> 8) & 0xff)
#define PCI_RCEC_BUSN_LAST(x) (((x) >> 16) & 0xff)
#define PCI_VNDR_HEADER 4
#define PCI_VNDR_HEADER_ID(x) ((x) & 0xffff)
#define PCI_VNDR_HEADER_REV(x) (((x) >> 16) & 0xf)
#define PCI_VNDR_HEADER_LEN(x) (((x) >> 20) & 0xfff)
#define HT_3BIT_CAP_MASK 0xE0
#define HT_CAPTYPE_SLAVE 0x00
#define HT_CAPTYPE_HOST 0x20
#define HT_5BIT_CAP_MASK 0xF8
#define HT_CAPTYPE_IRQ 0x80
#define HT_CAPTYPE_REMAPPING_40 0xA0
#define HT_CAPTYPE_REMAPPING_64 0xA2
#define HT_CAPTYPE_UNITID_CLUMP 0x90
#define HT_CAPTYPE_EXTCONF 0x98
#define HT_CAPTYPE_MSI_MAPPING 0xA8
#define HT_MSI_FLAGS 0x02
#define HT_MSI_FLAGS_ENABLE 0x1
#define HT_MSI_FLAGS_FIXED 0x2
#define HT_MSI_FIXED_ADDR 0x00000000FEE00000ULL
#define HT_MSI_ADDR_LO 0x04
#define HT_MSI_ADDR_LO_MASK 0xFFF00000
#define HT_MSI_ADDR_HI 0x08
#define HT_CAPTYPE_DIRECT_ROUTE 0xB0
#define HT_CAPTYPE_VCSET 0xB8
#define HT_CAPTYPE_ERROR_RETRY 0xC0
#define HT_CAPTYPE_GEN3 0xD0
#define HT_CAPTYPE_PM 0xE0
#define HT_CAP_SIZEOF_LONG 28
#define HT_CAP_SIZEOF_SHORT 24
#define PCI_ARI_CAP 0x04
#define PCI_ARI_CAP_MFVC 0x0001
#define PCI_ARI_CAP_ACS 0x0002
#define PCI_ARI_CAP_NFN(x) (((x) >> 8) & 0xff)
#define PCI_ARI_CTRL 0x06
#define PCI_ARI_CTRL_MFVC 0x0001
#define PCI_ARI_CTRL_ACS 0x0002
#define PCI_ARI_CTRL_FG(x) (((x) >> 4) & 7)
#define PCI_EXT_CAP_ARI_SIZEOF 8
#define PCI_ATS_CAP 0x04
#define PCI_ATS_CAP_QDEP(x) ((x) & 0x1f)
#define PCI_ATS_MAX_QDEP 32
#define PCI_ATS_CAP_PAGE_ALIGNED 0x0020
#define PCI_ATS_CTRL 0x06
#define PCI_ATS_CTRL_ENABLE 0x8000
#define PCI_ATS_CTRL_STU(x) ((x) & 0x1f)
#define PCI_ATS_MIN_STU 12
#define PCI_EXT_CAP_ATS_SIZEOF 8
#define PCI_PRI_CTRL 0x04
#define PCI_PRI_CTRL_ENABLE 0x0001
#define PCI_PRI_CTRL_RESET 0x0002
#define PCI_PRI_STATUS 0x06
#define PCI_PRI_STATUS_RF 0x0001
#define PCI_PRI_STATUS_UPRGI 0x0002
#define PCI_PRI_STATUS_STOPPED 0x0100
#define PCI_PRI_STATUS_PASID 0x8000
#define PCI_PRI_MAX_REQ 0x08
#define PCI_PRI_ALLOC_REQ 0x0c
#define PCI_EXT_CAP_PRI_SIZEOF 16
#define PCI_PASID_CAP 0x04
#define PCI_PASID_CAP_EXEC 0x02
#define PCI_PASID_CAP_PRIV 0x04
#define PCI_PASID_CTRL 0x06
#define PCI_PASID_CTRL_ENABLE 0x01
#define PCI_PASID_CTRL_EXEC 0x02
#define PCI_PASID_CTRL_PRIV 0x04
#define PCI_EXT_CAP_PASID_SIZEOF 8
#define PCI_SRIOV_CAP 0x04
#define PCI_SRIOV_CAP_VFM 0x00000001
#define PCI_SRIOV_CAP_INTR(x) ((x) >> 21)
#define PCI_SRIOV_CTRL 0x08
#define PCI_SRIOV_CTRL_VFE 0x0001
#define PCI_SRIOV_CTRL_VFM 0x0002
#define PCI_SRIOV_CTRL_INTR 0x0004
#define PCI_SRIOV_CTRL_MSE 0x0008
#define PCI_SRIOV_CTRL_ARI 0x0010
#define PCI_SRIOV_STATUS 0x0a
#define PCI_SRIOV_STATUS_VFM 0x0001
#define PCI_SRIOV_INITIAL_VF 0x0c
#define PCI_SRIOV_TOTAL_VF 0x0e
#define PCI_SRIOV_NUM_VF 0x10
#define PCI_SRIOV_FUNC_LINK 0x12
#define PCI_SRIOV_VF_OFFSET 0x14
#define PCI_SRIOV_VF_STRIDE 0x16
#define PCI_SRIOV_VF_DID 0x1a
#define PCI_SRIOV_SUP_PGSIZE 0x1c
#define PCI_SRIOV_SYS_PGSIZE 0x20
#define PCI_SRIOV_BAR 0x24
#define PCI_SRIOV_NUM_BARS 6
#define PCI_SRIOV_VFM 0x3c
#define PCI_SRIOV_VFM_BIR(x) ((x) & 7)
#define PCI_SRIOV_VFM_OFFSET(x) ((x) & ~7)
#define PCI_SRIOV_VFM_UA 0x0
#define PCI_SRIOV_VFM_MI 0x1
#define PCI_SRIOV_VFM_MO 0x2
#define PCI_SRIOV_VFM_AV 0x3
#define PCI_EXT_CAP_SRIOV_SIZEOF 0x40
#define PCI_LTR_MAX_SNOOP_LAT 0x4
#define PCI_LTR_MAX_NOSNOOP_LAT 0x6
#define PCI_LTR_VALUE_MASK 0x000003ff
#define PCI_LTR_SCALE_MASK 0x00001c00
#define PCI_LTR_SCALE_SHIFT 10
#define PCI_EXT_CAP_LTR_SIZEOF 8
#define PCI_ACS_CAP 0x04
#define PCI_ACS_SV 0x0001
#define PCI_ACS_TB 0x0002
#define PCI_ACS_RR 0x0004
#define PCI_ACS_CR 0x0008
#define PCI_ACS_UF 0x0010
#define PCI_ACS_EC 0x0020
#define PCI_ACS_DT 0x0040
#define PCI_ACS_EGRESS_BITS 0x05
#define PCI_ACS_CTRL 0x06
#define PCI_ACS_EGRESS_CTL_V 0x08
#define PCI_VSEC_HDR 4
#define PCI_VSEC_HDR_LEN_SHIFT 20
#define PCI_SATA_REGS 4
#define PCI_SATA_REGS_MASK 0xF
#define PCI_SATA_REGS_INLINE 0xF
#define PCI_SATA_SIZEOF_SHORT 8
#define PCI_SATA_SIZEOF_LONG 16
#define PCI_REBAR_CAP 4
#define PCI_REBAR_CAP_SIZES 0x00FFFFF0
#define PCI_REBAR_CTRL 8
#define PCI_REBAR_CTRL_BAR_IDX 0x00000007
#define PCI_REBAR_CTRL_NBAR_MASK 0x000000E0
#define PCI_REBAR_CTRL_NBAR_SHIFT 5
#define PCI_REBAR_CTRL_BAR_SIZE 0x00001F00
#define PCI_REBAR_CTRL_BAR_SHIFT 8
#define PCI_DPA_CAP 4
#define PCI_DPA_CAP_SUBSTATE_MASK 0x1F
#define PCI_DPA_BASE_SIZEOF 16
#define PCI_TPH_CAP 4
#define PCI_TPH_CAP_LOC_MASK 0x600
#define PCI_TPH_LOC_NONE 0x000
#define PCI_TPH_LOC_CAP 0x200
#define PCI_TPH_LOC_MSIX 0x400
#define PCI_TPH_CAP_ST_MASK 0x07FF0000
#define PCI_TPH_CAP_ST_SHIFT 16
#define PCI_TPH_BASE_SIZEOF 0xc
#define PCI_EXP_DPC_CAP 0x04
#define PCI_EXP_DPC_IRQ 0x001F
#define PCI_EXP_DPC_CAP_RP_EXT 0x0020
#define PCI_EXP_DPC_CAP_POISONED_TLP 0x0040
#define PCI_EXP_DPC_CAP_SW_TRIGGER 0x0080
#define PCI_EXP_DPC_RP_PIO_LOG_SIZE 0x0F00
#define PCI_EXP_DPC_CAP_DL_ACTIVE 0x1000
#define PCI_EXP_DPC_CTL 0x06
#define PCI_EXP_DPC_CTL_EN_FATAL 0x0001
#define PCI_EXP_DPC_CTL_EN_NONFATAL 0x0002
#define PCI_EXP_DPC_CTL_INT_EN 0x0008
#define PCI_EXP_DPC_STATUS 0x08
#define PCI_EXP_DPC_STATUS_TRIGGER 0x0001
#define PCI_EXP_DPC_STATUS_TRIGGER_RSN 0x0006
#define PCI_EXP_DPC_STATUS_INTERRUPT 0x0008
#define PCI_EXP_DPC_RP_BUSY 0x0010
#define PCI_EXP_DPC_STATUS_TRIGGER_RSN_EXT 0x0060
#define PCI_EXP_DPC_SOURCE_ID 0x0A
#define PCI_EXP_DPC_RP_PIO_STATUS 0x0C
#define PCI_EXP_DPC_RP_PIO_MASK 0x10
#define PCI_EXP_DPC_RP_PIO_SEVERITY 0x14
#define PCI_EXP_DPC_RP_PIO_SYSERROR 0x18
#define PCI_EXP_DPC_RP_PIO_EXCEPTION 0x1C
#define PCI_EXP_DPC_RP_PIO_HEADER_LOG 0x20
#define PCI_EXP_DPC_RP_PIO_IMPSPEC_LOG 0x30
#define PCI_EXP_DPC_RP_PIO_TLPPREFIX_LOG 0x34
#define PCI_PTM_CAP 0x04
#define PCI_PTM_CAP_REQ 0x00000001
#define PCI_PTM_CAP_ROOT 0x00000004
#define PCI_PTM_GRANULARITY_MASK 0x0000FF00
#define PCI_PTM_CTRL 0x08
#define PCI_PTM_CTRL_ENABLE 0x00000001
#define PCI_PTM_CTRL_ROOT 0x00000002
#define PCI_L1SS_CAP 0x04
#define PCI_L1SS_CAP_PCIPM_L1_2 0x00000001
#define PCI_L1SS_CAP_PCIPM_L1_1 0x00000002
#define PCI_L1SS_CAP_ASPM_L1_2 0x00000004
#define PCI_L1SS_CAP_ASPM_L1_1 0x00000008
#define PCI_L1SS_CAP_L1_PM_SS 0x00000010
#define PCI_L1SS_CAP_CM_RESTORE_TIME 0x0000ff00
#define PCI_L1SS_CAP_P_PWR_ON_SCALE 0x00030000
#define PCI_L1SS_CAP_P_PWR_ON_VALUE 0x00f80000
#define PCI_L1SS_CTL1 0x08
#define PCI_L1SS_CTL1_PCIPM_L1_2 0x00000001
#define PCI_L1SS_CTL1_PCIPM_L1_1 0x00000002
#define PCI_L1SS_CTL1_ASPM_L1_2 0x00000004
#define PCI_L1SS_CTL1_ASPM_L1_1 0x00000008
#define PCI_L1SS_CTL1_L1_2_MASK 0x00000005
#define PCI_L1SS_CTL1_L1SS_MASK 0x0000000f
#define PCI_L1SS_CTL1_CM_RESTORE_TIME 0x0000ff00
#define PCI_L1SS_CTL1_LTR_L12_TH_VALUE 0x03ff0000
#define PCI_L1SS_CTL1_LTR_L12_TH_SCALE 0xe0000000
#define PCI_L1SS_CTL2 0x0c
#define PCI_DVSEC_HEADER1 0x4
#define PCI_DVSEC_HEADER1_VID(x) ((x) & 0xffff)
#define PCI_DVSEC_HEADER1_REV(x) (((x) >> 16) & 0xf)
#define PCI_DVSEC_HEADER1_LEN(x) (((x) >> 20) & 0xfff)
#define PCI_DVSEC_HEADER2 0x8
#define PCI_DVSEC_HEADER2_ID(x) ((x) & 0xffff)
#define PCI_DLF_CAP 0x04
#define PCI_DLF_EXCHANGE_ENABLE 0x80000000
#define PCI_PL_16GT_LE_CTRL 0x20
#define PCI_PL_16GT_LE_CTRL_DSP_TX_PRESET_MASK 0x0000000F
#define PCI_PL_16GT_LE_CTRL_USP_TX_PRESET_MASK 0x000000F0
#define PCI_PL_16GT_LE_CTRL_USP_TX_PRESET_SHIFT 4
#define PCI_DOE_CAP 0x04
#define PCI_DOE_CAP_INT_SUP 0x00000001
#define PCI_DOE_CAP_INT_MSG_NUM 0x00000ffe
#define PCI_DOE_CTRL 0x08
#define PCI_DOE_CTRL_ABORT 0x00000001
#define PCI_DOE_CTRL_INT_EN 0x00000002
#define PCI_DOE_CTRL_GO 0x80000000
#define PCI_DOE_STATUS 0x0c
#define PCI_DOE_STATUS_BUSY 0x00000001
#define PCI_DOE_STATUS_INT_STATUS 0x00000002
#define PCI_DOE_STATUS_ERROR 0x00000004
#define PCI_DOE_STATUS_DATA_OBJECT_READY 0x80000000
#define PCI_DOE_WRITE 0x10
#define PCI_DOE_READ 0x14
#define PCI_DOE_DATA_OBJECT_HEADER_1_VID 0x0000ffff
#define PCI_DOE_DATA_OBJECT_HEADER_1_TYPE 0x00ff0000
#define PCI_DOE_DATA_OBJECT_HEADER_2_LENGTH 0x0003ffff
#define PCI_DOE_DATA_OBJECT_DISC_REQ_3_INDEX 0x000000ff
#define PCI_DOE_DATA_OBJECT_DISC_RSP_3_VID 0x0000ffff
#define PCI_DOE_DATA_OBJECT_DISC_RSP_3_PROTOCOL 0x00ff0000
#define PCI_DOE_DATA_OBJECT_DISC_RSP_3_NEXT_INDEX 0xff000000
#endif
