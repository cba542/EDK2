/** @file
  This is a simple shell application

  Copyright (c) 2008 - 2010, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Guid/Acpi.h>
//#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include "DumpACPI.h"

extern EFI_SYSTEM_TABLE   *gST;
extern EFI_BOOT_SERVICES  *gBS;

void
PrintTableHeader(
  IN UINT8 *Buffer8
  )
{
  
  for(int i = 0; i <= 3; i++)
    Print(L"%c",Buffer8[i]);

}


void
DumpBuffer8(
  IN UINT8 *Buffer8,
  IN UINTN Length
  )
{
  UINTN      Index;
  UINTN      SubIndex;
  Print(L"   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F \n00 ");
  for(Index = 0; Index < Length ;Index++){ 

    
    Print(L"%02x ",Buffer8[Index]);
 
        if((Index + 1) % 16 ==0){
          for(SubIndex = Index - 0xF; SubIndex < Index; SubIndex++){
            if((0x20 < Buffer8[SubIndex]) && (Buffer8[SubIndex] < 0x7F) ){
              Print(L"%c",Buffer8[SubIndex]);
              continue;
            }
              Print(L".");
            }
        Print(L"\n");
        Print(L"%02x ",(Index+1));
        }
    }

  Print(L"\n");
}

EFI_STATUS
PrintGuid (
  IN EFI_GUID *Guid
  )
/*++

Routine Description:

  This function prints a GUID to STDOUT.

Arguments:

  Guid    Pointer to a GUID to print.

Returns:

  EFI_SUCCESS             The GUID was printed.
  EFI_INVALID_PARAMETER   The input was NULL.

--*/
{
  if (Guid == NULL) {
    Print(L"Invalid parameter\n");
    return EFI_INVALID_PARAMETER;
  }

  Print(
    L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
    (unsigned) Guid->Data1,
    Guid->Data2,
    Guid->Data3,
    Guid->Data4[0],
    Guid->Data4[1],
    Guid->Data4[2],
    Guid->Data4[3],
    Guid->Data4[4],
    Guid->Data4[5],
    Guid->Data4[6],
    Guid->Data4[7]
    );
  return EFI_SUCCESS;
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
  //EFI_GUID                TheGuid;
  //EFI_CONFIGURATION_TABLE *ect = gST->ConfigurationTable;
  EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER   *Rsdp;
  //UINT8                                          *Buffer8;
  UINT8                                          *Buffer8;
  EFI_GUID                                       gAcpi20TableGuid = EFI_ACPI_20_TABLE_GUID;
  //EFI_GUID                                       gAcpi10TableGuid = ACPI_10_TABLE_GUID;
  EFI_ACPI_DESCRIPTION_HEADER                    *Xsdt;
  EFI_ACPI_DESCRIPTION_HEADER                    *Rsdt;
  UINT32                                         *Buffer32;
  UINT64                                         DescriptorOffset;
  UINTN                                          SdtCount;
  EFI_ACPI_COMMON_HEADER                         *Table;
  BOOLEAN                                        RootSystemDiscriptor = FALSE;

  //Buffer8 = AllocatePool(sizeof(EFI_ACPI_DESCRIPTION_HEADER));

//Enable flag to display the Root System Descriptor Table when the CMD is "-v"  
  if((Argv[1][0] == '-') && (Argv[1][1] == 'v')){
    RootSystemDiscriptor = TRUE;
  }
//Search the RSDP Table in the Configuration Table
  for(int i = 0; i < gST->NumberOfTableEntries ; i++){
#ifdef DEBUG
    PrintGuid(&(gST->ConfigurationTable[i].VendorGuid));
#endif

    if( CompareGuid(&gAcpi20TableGuid, &(gST->ConfigurationTable[i].VendorGuid)) ){

       Buffer8 = (UINT8 *)(gST->ConfigurationTable[i].VendorTable);
//Check flag and Print the RSDP Table
       if(RootSystemDiscriptor){
         DumpBuffer8(Buffer8, sizeof(EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER));
       }
       Rsdp = (EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER *)(gST->ConfigurationTable[i].VendorTable);
       Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *)Rsdp->XsdtAddress;
       Rsdt = (EFI_ACPI_DESCRIPTION_HEADER *)(UINT64)Rsdp->RsdtAddress;

//Check flag and Print the XSDT Table
       Buffer8 = (UINT8 *)Rsdp->XsdtAddress;
       if(RootSystemDiscriptor){
         PrintTableHeader(Buffer8);
         Print(L"\n");
         DumpBuffer8(Buffer8,Xsdt->Length);
       }

//Check flag and Print the RSDT Table
       Buffer8 = (UINT8 *)(UINT64)Rsdp->RsdtAddress;
       if(RootSystemDiscriptor){
         PrintTableHeader(Buffer8);
         Print(L"\n");
         DumpBuffer8(Buffer8,Rsdt->Length);
       }
//Initial the SDT table entry and count
       DescriptorOffset = Rsdp->RsdtAddress + sizeof(EFI_ACPI_DESCRIPTION_HEADER);
       SdtCount = ((Rsdt->Length - sizeof(EFI_ACPI_DESCRIPTION_HEADER))/sizeof(UINT32));

       Buffer32 = (UINT32 *)(UINT64)(DescriptorOffset);
//If there is no input Parameter, StrHexToUintn need a lot of time to convert , check Argc to avoid it.
       if(Argc == 2){
         for(UINTN j = 0 ; j < SdtCount ; j++){
       
           Buffer8 = (UINT8 *)(UINT64)(Buffer32[j]);
           Table   = (EFI_ACPI_COMMON_HEADER *)(UINT64)(Buffer32[j]);

           if( (StrHexToUintn(Argv[1])) == j){
//StrHexToUintn will convert unknow char to 0, check Argv[1][0] to avoid it.
             if(j == 0 && Argv[1][0] != L'0')
               break;
#ifdef DEBUG
             Print(L"StrHexToUintn(Argv[1]) = %x\n",StrHexToUintn(Argv[1]));
#endif
             PrintTableHeader(Buffer8);
             Print(L"\n");
             DumpBuffer8(Buffer8,Table->Length);
             //return EFI_SUCCESS;

           }
         }
       }

       //if(!RootSystemDiscriptor){
         Print(L"DumpACPI.efi -v (Command to Dump Root System Descriptor Table(RSDP, RSDT, XSDT)\n");
         Print(L"DumpACPI.efi number (Command to System Descriptor Table(FACP, HEPT, APIC...)\n");
       //}
//Print SDT table 
//0.FACP 1.HPET 2.APIC ......
         for(UINTN j = 0 ; j < SdtCount ; j++){
           Buffer8 = (UINT8 *)(UINT64)(Buffer32[j]);
           Table   = (EFI_ACPI_COMMON_HEADER *)(UINT64)(Buffer32[j]);
           Print(L"%2x.",j);
           PrintTableHeader(Buffer8);
           Print(L" ");

       }
       return EFI_SUCCESS;
     }
           

  }

  

  

  return EFI_SUCCESS;
}
