/** @file
  This is a simple shell application

  Copyright (c) 2008 - 2010, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
//#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
//#include <Library/UefiBootServicesTableLib.h>
//#include <Library/UefiRuntimeServicesTableLib.h>

extern  EFI_BOOT_SERVICES *gBS;
//extern  EFI_SYSTEM_TABLE  *gST;

//extern gEfiTestProtocolGuid;

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
  Print(L"Line %d\n",__LINE__);
  EFI_STATUS Status;
  EFI_HANDLE ThunkHandle = NULL;
  //Install Protocol(guid = gEfiTestProtocolGuid) on the thunk handle
  //Install by shell CMD 
  //>>load ShellTestDriver.efi
  Status = gBS->InstallProtocolInterface (
                  &ThunkHandle,
                  &gEfiTestProtocolGuid, 
				  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  Print(L"Status = %r\n",Status);


  Print(L"Line %d\n",__LINE__);
  return EFI_SUCCESS;
}
 