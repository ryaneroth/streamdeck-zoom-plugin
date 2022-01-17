// Martijn Smit <martijn@lostdomain.org / @smitmartijn>
#include <StreamDeckSDK/ESDBasePlugin.h>

#include <mutex>
#include <set>

using json = nlohmann::json;

class CallBackTimer;

// OS specific commands
std::string osGetZoomStatus();
void osToggleZoomMute();
void osToggleZoomShare();
void osToggleZoomVideo();
void osLeaveZoomMeeting();
void osFocusZoomWindow();
void osToggleZoomRecordCloud();
void osToggleZoomRecordLocal();
void osToggleZoomHand();
void osMuteAll();
void osUnmuteAll();
void osZoomCustomShortcut(std::string shortcut);

class ZoomStreamDeckPlugin : public ESDBasePlugin
{
public:
  ZoomStreamDeckPlugin();
  virtual ~ZoomStreamDeckPlugin();

  void KeyDownForAction(
      const std::string &inAction,
      const std::string &inContext,
      const json &inPayload,
      const std::string &inDeviceID) override;
  void KeyUpForAction(
      const std::string &inAction,
      const std::string &inContext,
      const json &inPayload,
      const std::string &inDeviceID) override;
  void WillAppearForAction(
      const std::string &inAction,
      const std::string &inContext,
      const json &inPayload,
      const std::string &inDeviceID) override;
  void WillDisappearForAction(
      const std::string &inAction,
      const std::string &inContext,
      const json &inPayload,
      const std::string &inDeviceID) override;

  void SendToPlugin(
      const std::string &inAction,
      const std::string &inContext,
      const json &inPayload,
      const std::string &inDeviceID) override;

  void DeviceDidConnect(const std::string &inDeviceID, const json &inDeviceInfo)
      override;
  void DeviceDidDisconnect(const std::string &inDeviceID) override;

  void DidReceiveGlobalSettings(const json &inPayload) override;
  void DidReceiveSettings(
      const std::string &inAction,
      const std::string &inContext,
      const json &inPayload,
      const std::string &inDeviceID) override;

private:
  void UpdateZoomStatus();

  std::recursive_mutex mVisibleContextsMutex;
  std::set<std::string> mVisibleContexts;

  // SD button array to remember the context ID
  struct Button
  {
    std::string action;
    std::string context;
  };

  void UpdateState(const std::string &context, const std::string &device = "");

  std::map<std::string, Button> mButtons;
  CallBackTimer *mTimer;
};
