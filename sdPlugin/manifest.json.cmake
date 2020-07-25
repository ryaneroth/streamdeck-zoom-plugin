{
  "Actions": [
    {
      "States": [
        {
          "Image": "streamdeck-zoom-muted"
        },
        {
          "Image": "streamdeck-zoom-unmuted"
        },
        {
          "Image": "streamdeck-zoom-muted-closed"
        }
      ],
      "SupportedInMultiActions": true,
      "Icon": "streamdeck-zoom-unmuted",
      "Name": "Mute Toggle",
      "Tooltip": "Toggle Zoom Mute Status",
      "UUID": "com.lostdomain.zoom.mutetoggle"
    },
    {
      "States": [
        {
          "Image": "streamdeck-zoom-video-stopped"
        },
        {
          "Image": "streamdeck-zoom-video-started"
        },
        {
          "Image": "streamdeck-zoom-video-closed"
        }
      ],
      "SupportedInMultiActions": true,
      "Icon": "streamdeck-zoom-video-started",
      "Name": "Video Toggle",
      "Tooltip": "Toggle Zoom Video",
      "UUID": "com.lostdomain.zoom.videotoggle"
    },
    {
      "States": [
        {
          "Image": "streamdeck-zoom-share-start"
        },
        {
          "Image": "streamdeck-zoom-share-stop"
        },
        {
          "Image": "streamdeck-zoom-share-closed"
        }
      ],
      "SupportedInMultiActions": true,
      "Icon": "streamdeck-zoom-share-start",
      "Name": "Share Toggle",
      "Tooltip": "Bring up the share screen window, or stop sharing",
      "UUID": "com.lostdomain.zoom.sharetoggle"
    },
    {
      "States": [
        {
          "Image": "streamdeck-zoom-focus"
        },
        {
          "Image": "streamdeck-zoom-focus-closed"
        }
      ],
      "SupportedInMultiActions": true,
      "Icon": "streamdeck-zoom-focus",
      "Name": "Focus",
      "Tooltip": "Bring the Zoom window to the front",
      "UUID": "com.lostdomain.zoom.focus"
    },
    {
      "States": [
        {
          "Image": "streamdeck-zoom-leave"
        },
        {
          "Image": "streamdeck-zoom-leave-closed"
        }
      ],
      "SupportedInMultiActions": true,
      "Icon": "streamdeck-zoom-leave",
      "Name": "Leave Meeting",
      "Tooltip": "Leave an active meeting. If you're the host, this ends the meeting.",
      "UUID": "com.lostdomain.zoom.leave"
    }
  ],
  "CodePath": "sdzoomplugin.exe",
  "CodePathMac": "sdzoomplugin",
  "Author": "Martijn Smit",
  "Description": "Control your Zoom meetings.",
  "Name": "Zoom Plugin",
  "Category": "Zoom",
  "CategoryIcon": "video-camera",
  "PropertyInspectorPath": "propertyinspector/index.html",
  "Icon": "video-camera",
  "Version": "${CMAKE_PROJECT_VERSION}",
  "OS": [
    {
      "Platform": "mac",
      "MinimumVersion": "10.13"
    },
    {
      "Platform": "windows",
      "MinimumVersion" : "8"
    }
  ],
  "SDKVersion": 2,
  "Software": {
    "MinimumVersion": "4.1"
  }
}
