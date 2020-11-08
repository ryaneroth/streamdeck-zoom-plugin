// Martijn Smit <martijn@lostdomain.org / @smitmartijn>
#include "ZoomStreamDeckPlugin.h"

#include <StreamDeckSDK/ESDLogger.h>
#include <windows.h>
#include <uiautomationclient.h>
#include <string>
#include <sstream>
#include <regex>

IUIAutomation *pAutomation = nullptr;

void init()
{
  CoInitializeEx(NULL, COINIT_MULTITHREADED);
  auto hr = CoCreateInstance(__uuidof(CUIAutomation), NULL, CLSCTX_INPROC_SERVER, __uuidof(IUIAutomation), (void **)&pAutomation);
  if (FAILED(hr) || pAutomation == nullptr)
  {
    ESDDebug("failed to init automation");
    return;
  }
}

IUIAutomationElement *GetControlByClassName(IUIAutomationElement *parent, LPCWSTR className, TreeScope scope = TreeScope_Children)
{
  if (className == NULL)
    return nullptr;

  VARIANT varProp;
  varProp.vt = VT_BSTR;
  varProp.bstrVal = SysAllocString(className);
  if (varProp.bstrVal == NULL)
    return nullptr;

  IUIAutomationCondition *classCondition = nullptr;
  IUIAutomationCondition *controlCondition = nullptr;
  IUIAutomationCondition *combinedCondition = nullptr;

  auto hr = pAutomation->CreatePropertyCondition(UIA_ClassNamePropertyId, varProp, &classCondition);
  if (FAILED(hr))
    return nullptr;

  varProp.vt = VT_I4;
  varProp.lVal = UIA_WindowControlTypeId;
  hr = pAutomation->CreatePropertyCondition(UIA_ControlTypePropertyId, varProp, &controlCondition);
  if (FAILED(hr))
    return nullptr;

  hr = pAutomation->CreateAndCondition(classCondition, controlCondition, &combinedCondition);
  if (FAILED(hr))
    return nullptr;

  IUIAutomationElement *found;
  parent->FindFirst(scope, combinedCondition, &found);
  //parent->FindFirstBuildCache(scope, combinedCondition, cache, &found);

  if (classCondition != nullptr)
    classCondition->Release();
  if (controlCondition != nullptr)
    controlCondition->Release();
  if (combinedCondition != nullptr)
    combinedCondition->Release();

  return found;
}

IUIAutomationElementArray *GetButtons(IUIAutomationElement *parent, TreeScope scope = TreeScope_Children)
{
  VARIANT varProp;
  varProp.vt = VT_I4;
  varProp.lVal = UIA_ButtonControlTypeId;

  IUIAutomationCondition *buttonCondition = nullptr;

  auto hr = pAutomation->CreatePropertyCondition(UIA_ControlTypePropertyId, varProp, &buttonCondition);
  if (FAILED(hr))
    return nullptr;

  IUIAutomationElementArray *found = nullptr;
  parent->FindAll(scope, buttonCondition, &found);

  if (buttonCondition != nullptr)
    buttonCondition->Release();

  return found;
}

IUIAutomationElement *GetZoom()
{
  if (pAutomation == nullptr)
    init();

  IUIAutomationElement *root = nullptr;
  auto hr = pAutomation->GetRootElement(&root);
  if (FAILED(hr) || root == nullptr)
  {
    ESDDebug("Failed to get root");
    return nullptr;
  }

  return GetControlByClassName(root, L"ZPContentViewWndClass");
}

std::string formatStatus(std::string zoom, std::string mute, std::string video, std::string share, std::string record)
{
  std::stringstream ss;
  ss << "zoomStatus:" << zoom << ",";
  ss << "zoomMute:" << mute << ",";
  ss << "zoomVideo:" << video << ",";
  ss << "zoomRecord:" << record << ",";
  ss << "zoomShare:" << share;
  return ss.str();
}

std::string osGetZoomStatus()
{
  std::string statusZoom = "unknown",
              statusMute = "unknown",
              statusVideo = "unknown",
              statusShare = "unknown",
              statusRecord = "unknown";

  auto zoom = GetZoom();
  if (zoom == nullptr)
  {
    ESDDebug("Failed to find zoom");
    return formatStatus(statusZoom, statusMute, statusVideo, statusShare);
  }

  auto controls = GetControlByClassName(zoom, L"ZPControlPanelClass", TreeScope_Descendants);
  if (controls == nullptr)
  {
    ESDDebug("Failed to find controls");
    return formatStatus(statusZoom, statusMute, statusVideo, statusShare);
  }

  auto buttons = GetButtons(controls, TreeScope_Descendants);
  if (buttons == nullptr)
  {
    ESDDebug("Failed to find buttons");
    return formatStatus(statusZoom, statusMute, statusVideo, statusShare);
  }

  statusZoom = "call";

  std::wregex buttonsRegEx(L".*(audio|mute|video|share).*", std::wregex::icase);
  std::wregex muteRegex(L"^Mute.*");
  std::wregex unMuteRegEx(L"^Unmute.*");
  std::wregex startVideoRegEx(L"^start my video.*");
  std::wregex stopVideoRegEx(L"^stop my video.*");
  std::wregex shareScreenRegEx(L"^Share Screen.*");

  int numButtons;
  buttons->get_Length(&numButtons);
  IUIAutomationElement *button = nullptr;
  VARIANT valProp;
  for (int i = 0; i < numButtons; i++)
  {
    buttons->GetElement(i, &button);
    button->GetCurrentPropertyValue(UIA_NamePropertyId, &valProp);
    std::wstring name(valProp.bstrVal, SysStringLen(valProp.bstrVal));
    if (std::regex_match(name, buttonsRegEx))
    {
      if (std::regex_match(name, muteRegex))
        statusMute = "unmuted";
      else if (std::regex_match(name, unMuteRegEx))
        statusMute = "muted";
      else if (std::regex_match(name, startVideoRegEx))
        statusVideo = "stopped";
      else if (std::regex_match(name, stopVideoRegEx))
        statusVideo = "started";
      else if (std::regex_match(name, shareScreenRegEx))
        statusShare = "stopped";
    }
  }

  return formatStatus(statusZoom, statusMute, statusVideo, statusShare);
}

void osToggleZoomMute()
{
  //ESDDebug("ZoomPlugin: Sending mute shortcut");
  osFocusZoomWindow();
  ::keybd_event(VK_MENU, 0, 0, 0);
  ::keybd_event(0x41, 0, 0, 0);
  keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
  keybd_event(0x41, 0, KEYEVENTF_KEYUP, 0);
}
void osToggleZoomShare()
{
  //ESDDebug("ZoomPlugin: Sending share shortcut");
  osFocusZoomWindow();
  ::keybd_event(VK_MENU, 0, 0, 0);
  ::keybd_event(0x53, 0, 0, 0);
  keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
  keybd_event(0x53, 0, KEYEVENTF_KEYUP, 0);
}
void osToggleZoomVideo()
{
  //ESDDebug("ZoomPlugin: Sending video shortcut");
  osFocusZoomWindow();
  ::keybd_event(VK_MENU, 0, 0, 0);
  ::keybd_event(0x56, 0, 0, 0);
  keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
  keybd_event(0x56, 0, KEYEVENTF_KEYUP, 0);
}
void osLeaveZoomMeeting()
{
  //ESDDebug("ZoomPlugin: Ending meeting");osFocusZoomWindow();
  ::keybd_event(VK_MENU, 0, 0, 0);
  ::keybd_event(0x51, 0, 0, 0);
  keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
  keybd_event(0x51, 0, KEYEVENTF_KEYUP, 0);
}

void osFocusZoomWindow()
{
  auto zoom = GetZoom();
  if (zoom == nullptr)
    return;

  zoom->SetFocus();
}
