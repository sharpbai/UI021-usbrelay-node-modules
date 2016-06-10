{
  "targets": [
    {
      "target_name": "usbrelay-ui021",
      "sources": [ "relay.cc", "usbrelay/hidcomm.c", "usbrelay/usb2io.c", "usbrelay/usbrelay.c"],
      "conditions": [
        ['OS=="linux"', {
          'cflags': [
            '<!(pkg-config libusb-1.0 --cflags 2>/dev/null || echo "")',
          ],
          'libraries': [
            '<!(pkg-config libusb-1.0 --libs 2>/dev/null || echo "")',
          ],
        }],
      ]
    }
  ]
}
