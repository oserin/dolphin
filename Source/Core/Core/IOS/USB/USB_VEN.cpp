// Copyright 2016 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "Core/IOS/USB/USB_VEN.h"
#include "Common/Logging/Log.h"
#include "Core/HW/Memmap.h"

namespace IOS
{
namespace HLE
{
namespace Device
{
USB_VEN::USB_VEN(u32 device_id, const std::string& device_name) : Device(device_id, device_name)
{
}

IPCCommandResult USB_VEN::IOCtlV(const IOCtlVRequest& request)
{
  request.Dump(GetDeviceName());
  return GetNoReply();
}

IPCCommandResult USB_VEN::IOCtl(const IOCtlRequest& request)
{
  request.Log(GetDeviceName(), LogTypes::OSHLE);

  IPCCommandResult reply = GetDefaultReply(IPC_SUCCESS);
  switch (request.request)
  {
  case USBV5_IOCTL_GETVERSION:
    Memory::Write_U32(0x50001, request.buffer_out);
    reply = GetDefaultReply(IPC_SUCCESS);
    break;

  case USBV5_IOCTL_GETDEVICECHANGE:
  {
    // sent on change
    static bool firstcall = true;
    if (firstcall)
    {
      reply = GetDefaultReply(IPC_SUCCESS);
      firstcall = false;
    }

    // num devices
    reply = GetDefaultReply(0);
    return reply;
  }
  break;

  case USBV5_IOCTL_ATTACHFINISH:
    reply = GetDefaultReply(IPC_SUCCESS);
    break;

  case USBV5_IOCTL_SUSPEND_RESUME:
    DEBUG_LOG(OSHLE, "Device: %i Resumed: %i", Memory::Read_U32(request.buffer_in),
              Memory::Read_U32(request.buffer_in + 4));
    reply = GetDefaultReply(IPC_SUCCESS);
    break;

  case USBV5_IOCTL_GETDEVPARAMS:
  {
    s32 device = Memory::Read_U32(request.buffer_in);
    u32 unk = Memory::Read_U32(request.buffer_in + 4);

    DEBUG_LOG(OSHLE, "USBV5_IOCTL_GETDEVPARAMS device: %i unk: %i", device, unk);

    Memory::Write_U32(0, request.buffer_out);

    reply = GetDefaultReply(IPC_SUCCESS);
  }
  break;

  default:
    request.Log(GetDeviceName(), LogTypes::OSHLE, LogTypes::LDEBUG);
  }
  return reply;
}
}  // namespace Device
}  // namespace HLE
}  // namespace IOS
