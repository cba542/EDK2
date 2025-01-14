## @file
#   Intel(r) UEFI Application Development Kit for EDK II.
#   This package contains applications which depend upon Standard Libraries
#   from the StdLib package.
#
#   See the comments in the [LibraryClasses.IA32] and [BuildOptions] sections
#   for important information about configuring this package for your
#   environment.
#
#   Copyright (c) 2010 - 2012, Intel Corporation. All rights reserved.<BR>
#   This program and the accompanying materials
#   are licensed and made available under the terms and conditions of the BSD License
#   which accompanies this distribution. The full text of the license may be found at
#   http://opensource.org/licenses/bsd-license.
#
#   THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#   WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
##

[Defines]
  PLATFORM_NAME                  = AppPkg
  PLATFORM_GUID                  = 0458dade-8b6e-4e45-b773-1b27cbda3e06
  PLATFORM_VERSION               = 0.01
  DSC_SPECIFICATION              = 0x00010006
  OUTPUT_DIRECTORY               = Build/AppPkg
#[-start-140226-T140032-UDK-Setup-modify]#
# SUPPORTED_ARCHITECTURES        = IA32|IPF|X64
  SUPPORTED_ARCHITECTURES        = IA32|X64
#[-end-140226-T140032-UDK-Setup-modify]#
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT

#
#  Debug output control
#
  DEFINE DEBUG_ENABLE_OUTPUT      = FALSE       # Set to TRUE to enable debug output
  DEFINE DEBUG_PRINT_ERROR_LEVEL  = 0x80000040  # Flags to control amount of debug output
  DEFINE DEBUG_PROPERTY_MASK      = 0

[PcdsFeatureFlag]

[PcdsFixedAtBuild]
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|$(DEBUG_PROPERTY_MASK)
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|$(DEBUG_PRINT_ERROR_LEVEL)

[PcdsFixedAtBuild.IPF]

[LibraryClasses]
  #
  # Entry Point Libraries
  #
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  ShellCEntryLib|ShellPkg/Library/UefiShellCEntryLib/UefiShellCEntryLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  #
  # Common Libraries
  #
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  !if $(DEBUG_ENABLE_OUTPUT)
    DebugLib|MdePkg/Library/UefiDebugLibConOut/UefiDebugLibConOut.inf
    DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
  !else   ## DEBUG_ENABLE_OUTPUT
    DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  !endif  ## DEBUG_ENABLE_OUTPUT

  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf

  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf

  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf




  FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
  
  CacheMaintenanceLib|MdePkg/Library/BaseCacheMaintenanceLib/BaseCacheMaintenanceLib.inf
  ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf

  HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
#-Start- Add MySmm Driver  
  SmmServicesTableLib|MdePkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf
#- End - Add MySmm Driver
###################################################################################################
#
# Components Section - list of the modules and components that will be processed by compilation
#                      tools and the EDK II tools to generate PE32/PE32+/Coff image files.
#
# Note: The EDK II DSC file is not used to specify how compiled binary images get placed
#       into firmware volume images. This section is just a list of modules to compile from
#       source into UEFI-compliant binaries.
#       It is the FDF file that contains information on combining binary files into firmware
#       volume images, whose concept is beyond UEFI and is described in PI specification.
#       Binary modules do not need to be listed in this section, as they should be
#       specified in the FDF file. For example: Shell binary (Shell_Full.efi), FAT binary (Fat.efi),
#       Logo (Logo.bmp), and etc.
#       There may also be modules listed in this section that are not required in the FDF file,
#       When a module listed here is excluded from FDF file, then UEFI-compliant binary will be
#       generated for it, but the binary will not be put into any firmware volume.
#
###################################################################################################

[Components]

#### Sample Applications.
#[-start-140523-T140032-UDK-Unused-remove]#
# AppPkg/Applications/Hello/Hello.inf        # No LibC includes or functions.
# AppPkg/Applications/Main/Main.inf          # Simple invocation. No other LibC functions.
# AppPkg/Applications/Enquire/Enquire.inf    #
#[-end-140523-T140032-UDK-Unused-remove]#

#### After extracting the Python distribution, un-comment the following line to build Python.
#  AppPkg/Applications/Python/PythonCore.inf

#[-start-140523-T140032-H19EepromShellTool-add]#
#  AppPkg/Applications/HpEepromTool/HpEepromTool.inf
#[-end-140523-T140032-H19EepromShellTool-add]#

#-----------------------------------------------------------------------------

#  AppPkg/Applications/Hello/Hello.inf
#  AppPkg/Applications/mytesthandle/mytesthandle.inf
#  AppPkg/Applications/ShellTestApp/ShellTestApp.inf
#-Start- Add ShellArgApp shell
#  AppPkg/Applications/ShellTestArgApp/ShellTestArgApp.inf
#- End - Add ShellArgApp shell
#  AppPkg/Applications/ListAllProtocolGuid/ListAllProtocolGuid.inf
#  AppPkg/Applications/ListPci/ListPci.inf
#-Start- Add MySmm Driver
#  AppPkg/Applications/MySmm/MySmm.inf
#- End - Add MySmm Driver
#-Start- Add GetVariable App
#  AppPkg/Applications/ZhengGetVariable/ZhengGetVariable.inf
#- End - Add GetVariable App
#-Start- Add MSR Read App
#  AppPkg/Applications/ZhengMSR/ZhengMSR.inf
#- End - Add MSR Read App
#-Start- Add Dump ACPI App
  AppPkg/Applications/DumpACPI/DumpACPI.inf
#- End - Add Dump ACPI App
#-Start-Create driver image to Install protocol on the thunk handle
#AppPkg/Applications/ShellTestDriver/ShellTestDriver.inf 
#- End -Create driver image to Install protocol on the thunk handle

#AppPkg/Applications/ShellTestArgApp/ShellTestArgApp.inf {
#       <BuildOptions>
#    MSFT:*_*_*_CC_FLAGS = /Od
#}
#
#-Start-Implement Firmware Device Depx check App
#  AppPkg/Applications/FmpDepx/FmpDepx.inf
#- End -Implement Firmware Device Depx check App
#-----------------------------------------------------------------------------
  
##############################################################################
#
# Specify whether we are running in an emulation environment, or not.
# Define EMULATE if we are, else keep the DEFINE commented out.
#
# DEFINE  EMULATE = 1

##############################################################################
#
#  Include Boilerplate text required for building with the Standard Libraries.
#
##############################################################################
#[-start-140527-T140032-UDK-Unused-remove]#
# !include StdLib/StdLib.inc
#!include AppPkg/Applications/Sockets/Sockets.inc
#[-end-140527-T140032-UDK-Unused-remove]#
