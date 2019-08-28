#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h> //global gST gBS gImageHandle

#include <Protocol/LoadedImage.h>      //EFI_LOADED_IMAGE_PROTOCOL
#include <Protocol/DevicePath.h>       //EFI_DEVICE_PATH_PROTOCOL

#include <Library/DevicePathLib.h>     //link
#include <Library/ShellLib.h>
#include <Library/ShellCommandLib.h> 
#include <Library/HandleParsingLib.h> 

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{

  EFI_GUID **ProtocolGuidArray;
  UINTN ArrayCount;
  CHAR16 *str;

  Print(L"1\n");
  EFI_STATUS status = gBS->ProtocolsPerHandle(gImageHandle,&ProtocolGuidArray,&ArrayCount);

  if (status == EFI_SUCCESS)
  {
    for (UINTN i = 0; i < ArrayCount; i++)
    {
      str = GetStringNameFromGuid(ProtocolGuidArray[i], 0);
      Print(L"%s,%g\n", str,ProtocolGuidArray[i]);
      if (str)
      {
        gBS->FreePool(str);
        str=0;
      }
    }

    if (ProtocolGuidArray)
    {
      gBS->FreePool(ProtocolGuidArray);
      ProtocolGuidArray=0;
    }
  }

  EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
    //open the Loaded image protocol,which is binded on the imageHandle,to get the device handle.
    status = gBS->OpenProtocol(
      gImageHandle,
      &gEfiLoadedImageProtocolGuid,
      (VOID **)&LoadedImage,
      gImageHandle,
      NULL,
      EFI_OPEN_PROTOCOL_GET_PROTOCOL);

  if(status == EFI_SUCCESS)
  {
    Print(L"2\n");
   status = gBS->ProtocolsPerHandle(LoadedImage->DeviceHandle,&ProtocolGuidArray,&ArrayCount);
    
      if (status == EFI_SUCCESS)
      {
        for (UINTN i = 0; i < ArrayCount; i++)
        {
          str = GetStringNameFromGuid(ProtocolGuidArray[i], 0);
          Print(L"%s,%g\n", str,ProtocolGuidArray[i]);
          if (str)
          {
            gBS->FreePool(str);
            str=0;
          }
        }
    
        if (ProtocolGuidArray)
        {
          gBS->FreePool(ProtocolGuidArray);
          ProtocolGuidArray=0;
        }
      }

      gBS->CloseProtocol(
        gImageHandle,
        &gEfiLoadedImageProtocolGuid,
        gImageHandle,
        NULL);
  }


  return EFI_SUCCESS;
}
