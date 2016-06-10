# UI021-usbrelay-node-modules
a usb relay nodejs library for UI021

link: http://freeeasy-life.com/product/ui_mod/ur

##usage
### setRelay([deviceId], [portId], [status])
set usbrelay on deviceId(1~n) and portId(1~126) to status(1=on, 0=off)
return value is `0` (ok) or `-1` (error)

### getRelay([deviceId], [portId])
get usbrelay status on deviceId(1~n) and portId(1~126)
return value is current status `1` (on), `0` (off) or `-1` (error)

### toggleRelay([deviceId], [portId])
toggle usbrelay on deviceId(1~n) and portId(1~126)
return value is current status after toggle: `1` (on), `0` (off) or `-1` (error)


