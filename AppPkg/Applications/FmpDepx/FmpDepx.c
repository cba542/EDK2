/** @file
  This is a simple shell application

  Copyright (c) 2008 - 2010, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Protocol/Shell.h>

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

UINT8 pop8()
{
  //UINT8 PopValue = stack8[top];
  //stack8[top] = 0;
  //top--;
  //return PopValue;
  return stack8[top--];
}

UINT32 pop32()
{
  UINT32 i,val;
  UINT8 a[4];
  for(i = 0; i < 4;i++)
    a[i] = pop8();
  val = ((UINT32)a[3] + (((UINT32)a[2])<<8) + ((UINT32)a[1]<<16) + ((UINT32)a[0]<<24));
  return val;

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

  CHAR8 *e;
  UINT8 n1,n2,n3;
  UINT32 val1,val2,i;

  EFI_STATUS Status;
  SHELL_FILE_HANDLE FileH = NULL;
  EFI_SHELL_PROTOCOL  *ShellProtocol;
  UINT8                           buffer8[0x50];
  UINTN                           bufferSize = 0x50;
  Status = gBS->LocateProtocol(
                      &gEfiShellProtocolGuid,
                      NULL,
                      &ShellProtocol);
  Print(L"Status = %r\n",Status);         
  Status = ShellProtocol->OpenFileByName(L"buffer.bin", &FileH, EFI_FILE_MODE_READ);
  Print(L"Status = %r\n",Status);
  ShellProtocol->ReadFile(FileH, &bufferSize,buffer8);

  
  for(i = 0 ; i < 0x20 ; i++)
    stack8[i] = 0;

  e = buffer8;
  while(( (*e) <= 0x0D))
  {
    Print(L"\nDependency Expression = 0x%01x\n",*e);

    switch(*e)
    {
      //1.GetImageInfo()
      //2.pop 16 bytes GUID compare with
      //3.If compare success push ImageInfo->Version to the stack
      case 0x00:
      {
        UINT32 version = 0x050403;
        UINT32 p1[1];
        UINT8  *p2;
        p1[0] = version;
        p2 = (UINT8 *)p1;
        for(i = 0; i < 4; i++)
        {
         push8(p2[i]);
        }
        break;
      }
      //1.GetImageInfo() to get ImageInfo->Version
      //2.push Version to stack
      case 0x01:
      {
        UINT32 version = 0x050403;
        UINT32 p1[1];
        UINT8  *p2;
        p1[0] = version;
        p2 = (UINT8 *)p1;
        for(i = 0; i < 4; i++)
        {
         push8(p2[i]);
        }

        break;
      }
      case 0x02:
      {
      //1.get the null-terminated UNICODE String '\0' from depx expression
      //2.compare with EFI_FIRMWARE_IMAGE_DESCRIPTOR->VersionName;
        //CHAR16  *VersionName;
        break;
      }
      //AND
      case 0x03:
      {
        n1 = pop8();
        n2 = pop8();
        n3 = n1 & n2;
        push8(n3);
        break;
      }
      //Or
      case 0x04:
      {
        n1 = pop8();
        n2 = pop8();
        n3 = n1 | n2;
        push8(n3);
        break;
      }
      //NOT
      case 0x05:
      {
        n1 = pop8();
        n1 = (!n1);
        push8(n1);
        break;
      }
      //push TURE
      case 0x06:
      {
        push8(TRUE);
        break;
      }
      //push FALSE
      case 0x07:
      {
        push8(FALSE);
        break;
      }
      //EQ
      case 0x08:
      {
        val1 = pop32();
        val2 = pop32();
        if(val1 == val2)
          push8(TRUE);
        else
          push8(FALSE);
#ifdef debugF
        Print(L"val1 = %x\n",val1);
        Print(L"val2 = %x\n",val2);
#endif
        break;
      }
      //GT
      case 0x09:
      {

        break;
      }
      //GTE
      case 0x0A:
      {

        break;
      }
      //LT
      case 0x0B:
      {

        break;
      }
      //LTE
      case 0x0C:
      {

        break;
      }
      case 0x0D:
      {
        n3 = pop8();
        if(n3 == 1)
        {
          DumpBuffer8(stack8, 0x20);
          Print(L"END pop result = TRUE\n");
          return EFI_SUCCESS;
        }
        if(n3 == 0)
        {
          DumpBuffer8(stack8, 0x20);
          Print(L"END pop result = FALSE\n");
          return EFI_DEVICE_ERROR;
        }
        break;
      }
    }

    Print(L"\nStack =\n");
    DumpBuffer8(stack8, 0x20);
    e++;
  }

  return EFI_SUCCESS;
}

