/** @file
  This is a simple shell application

  Copyright (c) 2008 - 2010, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>

VOID
PrintExample()
{
    Print(L"  Shell> ZhengMSR.efi -r address\n");
    Print(L"  ZhengMSR.efi -r 34\n");
    Print(L"  MSR[0x34] = 0x34\n");
}

/**
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
ShellAppMain (
  IN UINTN   Argc,
  IN CHAR16  **Argv
  )
{
  UINT32  MsrAddress;

//To check the input format should be ZhengMSR.efi -r offset
  if(Argc != 3 || Argv[1][0] != '-' || Argv[1][1] != 'r'){
    PrintExample();
    return EFI_INVALID_PARAMETER;
  }
    Print(L"!!Warring Read unaccseeable may occurred Application hang up\n");
    MsrAddress = (UINT32)StrHexToUintn(Argv[2]);

    Print(L"MSR[0x%x] = 0x%x\n",MsrAddress,AsmReadMsr64(MsrAddress));


  return EFI_SUCCESS;
}
