/** @file
  This is a simple shell application

  Copyright (c) 2008 - 2010, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>

#include <Library/HandleParsingLib.h>

#include <Library/ShellLib.h>
#include <Library/ShellCommandLib.h>

extern EFI_HANDLE         gImageHandle;
extern EFI_BOOT_SERVICES  *gBS;

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
  EFI_STATUS   Status;
  UINTN        GuidCount;
  EFI_GUID     **GuidBuffer;
  UINTN        GuidIndex;
  
  UINTN        HandleCount;
  EFI_HANDLE   *HandleBuffer;
  UINTN        HandleIndex;
  CHAR16       *str;

  Print(L"SA %d\n",__LINE__);  
  Status = gBS -> LocateHandleBuffer(
                    AllHandles,
                    NULL,
                    NULL,
                    &HandleCount,
                    &HandleBuffer
                    );
                    
  Print(L"status = %r\n",Status);
  for(HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++){
    gBS -> ProtocolsPerHandle(
             HandleBuffer[HandleIndex],
             &GuidBuffer,
             &GuidCount
             );
    for(GuidIndex = 0; GuidIndex < GuidCount; GuidIndex++){
      str = GetStringNameFromGuid(GuidBuffer[GuidIndex], NULL);

      Print(L"%s %g \n",str,GuidBuffer[GuidIndex]);

    }

  }

  return EFI_SUCCESS;
}
