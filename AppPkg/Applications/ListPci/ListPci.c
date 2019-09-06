/** @file
  This is a simple shell application

  Copyright (c) 2008 - 2010, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Library/UefiBootServicesTableLib.h>
/**
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                         Status;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL    *PciRootInterface;
  UINTN                              Address = 0;
  UINTN                              Bus, Device, Function;
  UINT16                             VendorID = 0;

  Status = gBS -> LocateProtocol(&gEfiPciRootBridgeIoProtocolGuid, NULL,(VOID **)&PciRootInterface);

  Print(L"Locate PciRootBridgeIo Protocol Status = %r\n",Status);

  for(Bus = 0; Bus < 256; Bus++){
    for(Device = 0; Device < 32; Device++)  {
      for(Function = 0; Function < 8; Function++)  {
        Address = EFI_PCI_ADDRESS(Bus, Device, Function, 0);
        Status = PciRootInterface->Pci.Read(PciRootInterface, EfiPciWidthUint16, Address, 1, &VendorID);

        if(EFI_ERROR(Status)){
          Print(L"Fail PciRootInterface->Pci.Read status = %r\n",Status);
        }
        if(VendorID!= 0xFFFF && (UINT8)VendorID != 0){
          Print(L"Bus = %x, Device = %x Function = %x, VendorID = %x\n",Bus, Device, Function, VendorID);
        }
        
      }
    }
  }
  
  return EFI_SUCCESS;
}
