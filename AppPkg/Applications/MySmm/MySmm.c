/** @file
  Acpi Smm driver.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#include <PiSmm.h>
#include <Library/DebugLib.h>
//#include <Library/HobLib.h>
//#include <Library/IoLib.h>
//#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/SmmServicesTableLib.h>
//#include <Library/BoardAcpiEnableLib.h>
#include <Protocol/SmmSwDispatch2.h>

//EFI_SMM_SYSTEM_TABLE2   *gSmst             = NULL;

/**
  Disable SCI

  @param[in] DispatchHandle       - The handle of this callback, obtained when registering
  @param[in] DispatchContext      - Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT
  @param[in] CommBuffer           - A pointer to a collection of data in memory that will
                                    be conveyed from a non-SMM environment into an SMM environment.
  @param[in] CommBufferSize       - The size of the CommBuffer.
**/
EFI_STATUS
EFIAPI
EnableCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer  OPTIONAL,
  IN  UINTN                         *CommBufferSize  OPTIONAL
  )
{
  DEBUG ((EFI_D_INFO, "SW SMI InputVaule 0x01 trigger!\n"));

  return EFI_SUCCESS;
}

/**
  Initializes the Acpi Smm Driver

  @param[in] ImageHandle   - Pointer to the loaded image protocol for this driver
  @param[in] SystemTable   - Pointer to the EFI System Table

  @retval Status           - EFI_SUCCESS
  @retval Assert, otherwise.

**/
EFI_STATUS
EFIAPI
MySmm (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                                Status;
  EFI_HANDLE                                SwHandle;
  EFI_SMM_SW_DISPATCH2_PROTOCOL             *SwDispatch;
  EFI_SMM_SW_REGISTER_CONTEXT               SwContext;

  //
  // Locate the ICH SMM SW dispatch protocol
  //
  Status = gSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID**)&SwDispatch);
  ASSERT_EFI_ERROR (Status);

  //
  // Register ACPI enable SMI handler
  //
  SwContext.SwSmiInputValue = 0x01;
  Status = SwDispatch->Register (
                         SwDispatch,
                         EnableCallback,
                         &SwContext,
                         &SwHandle
                         );
  ASSERT_EFI_ERROR (Status);



  return EFI_SUCCESS;
}
