/** @file
  This is a simple shell application

  Copyright (c) 2008 - 2010, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Guid/GlobalVariable.h>
#include <Library/MemoryAllocationLib.h>

extern EFI_RUNTIME_SERVICES *gRT ;

void PrintVariable(UINTN Size, UINT8 *Data){
  UINTN      Index;
  UINTN      SubIndex;
  for(Index = 0; Index < Size ;Index++){ 

    
    Print(L"%02x ",Data[Index]);
 
        if((Index + 1) % 16 ==0){
          for(SubIndex = Index - 0xF; SubIndex < Index; SubIndex++){
            if((0x20 < Data[SubIndex]) && (Data[SubIndex] < 0x7F) ){
              Print(L"%c",Data[SubIndex]);
              continue;
            }
              Print(L".");
            }
        Print(L"\n");
        }
    }
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
  EFI_STATUS Status;
  UINTN      NameSize;
  UINTN      NewNameSize;
  UINT8      *VariableData;

  CHAR16     *Name;
  CHAR16     *InputeVariable;
  EFI_GUID   VariableGuid;
  UINTN      VariableSize;


  
  if(Argc == 1){
    Print(L"Please Input Parameter\n");
    return RETURN_INVALID_PARAMETER;
  }

  if(Argc > 2){
    Print(L"Please Input only one Parameter\n");
    return RETURN_INVALID_PARAMETER;
  }
//InputeVariable = Input Parameter
  InputeVariable = Argv[1];


  NameSize = sizeof(CHAR16);
  Name     = AllocateZeroPool(NameSize);
  
  for(;;){

    NewNameSize = NameSize;
    Status = gRT->GetNextVariableName (&NewNameSize, Name, &VariableGuid);
      
 	if (Status == EFI_BUFFER_TOO_SMALL) {
      Name = ReallocatePool (NameSize, NewNameSize, Name);
      Status = gRT->GetNextVariableName (&NewNameSize, Name, &VariableGuid);
      NameSize = NewNameSize;
    }

//GetNextVariableName return vaule When the entire variable list has been returned, the error EFI_NOT_FOUND is returned.
    if(Status == EFI_NOT_FOUND){
      Print(L"Can not Find Variable \'%s\'\n",InputeVariable);
      break;
      }


  Status = GetVariable2 (
                  Name,
                  &VariableGuid,
                  &VariableData,
                  &VariableSize
                  );
//Compare InputeVaule and Name to find Variable and break loop
    if(!StrCmp(Name, InputeVariable)){
      Print(L"VariableName: \'%s\'\n", Name);
      Print(L"VariableSize: %d\n", VariableSize);
      Print(L"VariableGuid: %g\n", VariableGuid);
      Print(L"VariableData:\n");
      PrintVariable(VariableSize, VariableData);
      break;
    }


  }


  
  return EFI_SUCCESS;
}
