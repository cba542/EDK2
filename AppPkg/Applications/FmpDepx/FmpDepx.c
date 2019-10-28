/** @file
  This is a simple shell application

  Copyright (c) 2008 - 2010, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>

#include <Protocol/FirmwareManagement.h>
//Sam++Open FIle Code
#include <Protocol/Shell.h>
//Sam--Open FIle Code
extern EFI_BOOT_SERVICES *gBS;
INT8  top = -1;
CHAR8  stack8[20];

void
DumpBuffer8(
  IN UINT8 *Buffer8,
  IN UINTN Length
  )
{
  UINTN      Index;
  UINTN      SubIndex;
  UINT8      xyflag = 0;
  if(xyflag)
    Print(L"   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F \n00 ");
  for(Index = 0; Index < Length ;Index++){ 

    
      Print(L"%02x ",Buffer8[Index]);
 
        if((Index + 1) % 16 ==0){
          if(xyflag){
            for(SubIndex = Index - 0xF; SubIndex < Index; SubIndex++){
              if((0x20 < Buffer8[SubIndex]) && (Buffer8[SubIndex] < 0x7F) ){
                Print(L"%c",Buffer8[SubIndex]);
                continue;
              }
                Print(L".");
              }
          }
        Print(L"\n");
        if(xyflag)
        Print(L"%02x ",(Index+1));
        }
    }

  Print(L"\n");
}


void push8(UINT8 x)
{
  stack8[++top] = x;
}

void push32(UINT32 x)
{
  push8((UINT8) x );
  push8((UINT8)(x>>8));
  push8((UINT8)(x>>16));
  push8((UINT8)(x>>24));
}

UINT8 pop8()
{
  UINT8 PopValue = stack8[top];
  stack8[top] = 0;
  top--;
  return PopValue;
  //return stack8[top--];
}

UINT32 pop32()
{
  return ( (((UINT32)pop8())<<24) + (((UINT32)pop8())<<16) + ((((UINT32)pop8()))<<8) + (UINT32)pop8() );
}

UINT16 pop16()
{
  return (( (((UINT16)pop8()))<<8) + (UINT16)pop8() );  
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
  Print(L"ShellExecute - Pass\n");

  CHAR8                                         *e;
  UINT32                                        i;
  EFI_STATUS                                    Status;
  EFI_HANDLE                                    *HandleBuffer;
  UINTN                                         NumberOfHandles;
  EFI_FIRMWARE_MANAGEMENT_PROTOCOL              *Fmp;
  UINTN                                         ImageInfoSize;
  EFI_FIRMWARE_IMAGE_DESCRIPTOR                 *FmpImageInfoBuf;
  UINT32                                        FmpImageInfoDescriptorVer;
  UINT8                                         FmpImageInfoCount;
  UINTN                                         DescriptorSize;
  UINT32                                        PackageVersion;
  CHAR16                                        *PackageVersionName;

//Sam++Open File Code

  SHELL_FILE_HANDLE               FileH = NULL;
  EFI_SHELL_PROTOCOL              *ShellProtocol;
  UINT8                           buffer8[0x50];
  UINTN                           bufferSize = 0x50;
  Status = gBS->LocateProtocol(
                      &gEfiShellProtocolGuid,
                      NULL,
                      &ShellProtocol);
  if (EFI_ERROR(Status)) {
    Print(L"Locate gEfiShellProtocolGuidStatus fail ,status = %r %d\n",Status,__LINE__);
    return Status;      
  }
  Status = ShellProtocol->OpenFileByName(L"buffer.bin", &FileH, EFI_FILE_MODE_READ);
  if (EFI_ERROR(Status)) {
    Print(L"Locate OpenFileByName fail ,status = %r %d\n",Status,__LINE__);
    return Status;
  }
  ShellProtocol->ReadFile(FileH, &bufferSize,buffer8);
  e = buffer8;
  
//Sam--Open File Code

//Sam++ declare exp
/*
  CHAR8 exp[20];
  exp[0] = 0;
  exp[1] = 0x86;
  exp[2] = 0x48;
  exp[3] = 0x1E;
  exp[4] = 0x1A;
  exp[5] = 0x17;
  exp[6] = 0x95;
  exp[7] = 0x0E;
  exp[8] = 0x44;
  exp[9] = 0x9F;
  exp[10] = 0xDE;
  exp[11] = 0x3B;
  exp[12] = 0xE4;
  exp[13] = 0x4C;
  exp[14] = 0xEE;
  exp[15] = 0x21;
  exp[16] = 0x36;
  exp[17] = 0x0D;
  e = exp;
  */
//Sam-- declare exp
  

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareManagementProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR(Status)) {
    Print(L"Status = %r %d\n",Status,__LINE__);
    //return ;
  }

  for(i = 0 ; i < 0x20 ; i++)
    stack8[i] = 0;



  while(( (*e) <= 0x0D))
  {
    Print(L"\nDependency Expression = 0x%01x",*e);

    switch(*e)
    {

      case 0x00:
      {
        Print(L" (PUSH_GUID)\n");
        //Pushes the GUID value onto the stack.
        EFI_GUID *GuidToCompare;
        INT8 Index;
        GuidToCompare = (EFI_GUID *)(e+1);
        for(Index = 0; Index < 16 ;Index ++)
        {
            e++;
            push8(*e);
        }
        Print(L"push guid from dpex finish\nStack =\n");
        DumpBuffer8(stack8, 0x20);
/*
        //Store Guid to CompGuid
        for(Index = 7; Index >= 0; Index--)
          CompGuid.Data4[Index] = pop8();

        CompGuid.Data3 = pop16();
        CompGuid.Data2 = pop16();
        CompGuid.Data1 = pop32();
*/
        Print(L"GuidToCompare got from buffer.bin =\n");
        PrintGuid(GuidToCompare);
        

        if(NumberOfHandles == 0){
          Print(L"NumberOfHandles = 0, For Debug and push ImageInfo->Version = 0x50403020\n",NumberOfHandles);
          push32(0x50403020);
        }  
        for(Index = 0; Index < NumberOfHandles; Index++) {
          Status = gBS->HandleProtocol(
                        HandleBuffer[Index],
                        &gEfiFirmwareManagementProtocolGuid,
                        (VOID **)&Fmp
                        );
          Print(L"Debug line = %d\n",__LINE__);
          if (EFI_ERROR(Status)) {
            Print(L"Status = %r %d\n",Status,__LINE__);
            continue;
          }
          ImageInfoSize = 0;
          Print(L"Debug line = %d\n",__LINE__);
          Status = Fmp->GetImageInfo (
                        Fmp,
                        &ImageInfoSize,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL
                        );
          Print(L"Debug line = %d\n",__LINE__);
          if (Status != EFI_BUFFER_TOO_SMALL) {
          continue;
          }
          FmpImageInfoBuf = AllocateZeroPool (ImageInfoSize);
          if (FmpImageInfoBuf == NULL) {
          continue;
          }
          Print(L"Debug line = %d\n",__LINE__);
          Status = Fmp->GetImageInfo (
                          Fmp,
                          &ImageInfoSize,               // ImageInfoSize
                          FmpImageInfoBuf,              // ImageInfo
                          &FmpImageInfoDescriptorVer,   // DescriptorVersion
                          &FmpImageInfoCount,           // DescriptorCount
                          &DescriptorSize,              // DescriptorSize
                          &PackageVersion,              // PackageVersion
                          &PackageVersionName           // PackageVersionName
                          );
          Print(L"Debug line = %d\n",__LINE__);
          if (EFI_ERROR(Status)) {
            Print(L"Status = %r %d\n",Status,__LINE__);
            FreePool(FmpImageInfoBuf);
            continue;
          }
          Print(L"Debug line = %d\n",__LINE__);
          if(CompareGuid(&FmpImageInfoBuf->ImageTypeId, GuidToCompare)){
          Print(L"Debug line = %d\n",__LINE__);
            push32(FmpImageInfoBuf->Version);
          }
        }
        break;
      }

      case 0x01:
      {
        Print(L" (PUSH_VERSION)\n");
        INT8   Index;
        UINT32 VersionToCompare;
        
        for(Index = 0; Index < 4 ;Index ++)
        {
            e++;
            push8(*e);
        }

        VersionToCompare = pop32();
        Print(L"VersionToCompare got from buffer.bin = %x\n",VersionToCompare);
        
        if(NumberOfHandles == 0){
          Print(L"NumberOfHandles = 0, For Debug and push ImageInfo->Version = 0x50403020\n",NumberOfHandles);
          push32(0x50403020);
        }  
        
        for(Index = 0; Index < NumberOfHandles; Index++) {
          Status = gBS->HandleProtocol(
                        HandleBuffer[Index],
                        &gEfiFirmwareManagementProtocolGuid,
                        (VOID **)&Fmp
                        );
          Print(L"Debug line = %d\n",__LINE__);
          if (EFI_ERROR(Status)) {
            Print(L"Status = %r %d\n",Status,__LINE__);
            continue;
          }
          ImageInfoSize = 0;
          Print(L"Debug line = %d\n",__LINE__);
          Status = Fmp->GetImageInfo (
                        Fmp,
                        &ImageInfoSize,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL
                        );
          Print(L"Debug line = %d\n",__LINE__);
          if (Status != EFI_BUFFER_TOO_SMALL) {
            continue;
          }
          FmpImageInfoBuf = AllocateZeroPool (ImageInfoSize);
          if (FmpImageInfoBuf == NULL) {
            continue;
          }
          Print(L"Debug line = %d\n",__LINE__);
          Status = Fmp->GetImageInfo (
                          Fmp,
                          &ImageInfoSize,               // ImageInfoSize
                          FmpImageInfoBuf,              // ImageInfo
                          &FmpImageInfoDescriptorVer,   // DescriptorVersion
                          &FmpImageInfoCount,           // DescriptorCount
                          &DescriptorSize,              // DescriptorSize
                          &PackageVersion,              // PackageVersion
                          &PackageVersionName           // PackageVersionName
                          );
          Print(L"Debug line = %d\n",__LINE__);
          if (EFI_ERROR(Status)) {
            Print(L"Status = %r %d\n",Status,__LINE__);
            FreePool(FmpImageInfoBuf);
            continue;
          }
          Print(L"Debug line = %d\n",__LINE__);
          if((FmpImageInfoBuf->Version) == VersionToCompare){
          Print(L"FmpImage Verison equal to Fmpdepx %d\n",__LINE__);
            push32(FmpImageInfoBuf->Version);
          }
        }


        break;
      }
      case 0x02:
      {
        Print(L" (DECLARE_VERSION_NAME)\n");
        Print(L"null-terminated UNICODE string = ");
        while(*e != '\0'){
          e++;
          Print(L"%c",*e);
        }
        Print(L"\n");
        break;
      }
      //AND
      case 0x03:
      {
        Print(L" (AND)\n");
        push8(pop8() & pop8());
        break;
      }
      //Or
      case 0x04:
      {
        Print(L" (Or)\n");
        push8(pop8() | pop8());
        break;
      }
      //NOT
      case 0x05:
      {
        Print(L" (NOT)\n");
        push8(!pop8());
        break;
      }
      //push TURE
      case 0x06:
      {
        Print(L" (TRUE)\n");
        push8(TRUE);
        break;
      }
      //push FALSE
      case 0x07:
      {
        Print(L" (FALSE)\n");
        push8(FALSE);
        break;
      }
      //EQ
      case 0x08:
      {
        Print(L" (EQ)\n");
        (pop32() == pop32()) ? push8(TRUE) : push8(FALSE);
        break;
      }
      //GT
      case 0x09:
      {
        Print(L" (GT)\n");
        (pop32() >  pop32()) ? push8(TRUE) : push8(FALSE);
        break;
      }
      //GTE
      case 0x0A:
      {
        Print(L" (GTE)\n");
        (pop32() >= pop32()) ? push8(TRUE) : push8(FALSE);
        break;
      }
      //LT
      case 0x0B:
      {
        Print(L" (LT)\n");
        (pop32() <  pop32()) ? push8(TRUE) : push8(FALSE);
        break;
      }
      //LTE
      case 0x0C:
      {
        Print(L" (LTE)\n");
        (pop32() <= pop32()) ? push8(TRUE) : push8(FALSE);
        break;
      }
      case 0x0D:
      {
        Print(L" END\n");
        pop8() ? Print(L"END pop result = TRUE\n") : Print(L"END pop result = FALSE\n");
        DumpBuffer8(stack8, 0x20);
        return EFI_SUCCESS;
        break;
      }
    }

    Print(L"\nStack =\n");
    DumpBuffer8(stack8, 0x20);
    e++;
  }

  return EFI_SUCCESS;
}

