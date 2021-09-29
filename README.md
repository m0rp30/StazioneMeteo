# StazioneMeteo
Easy meteo station with ESP32

# API
```
http://api.weathercloud.net/set/wid/xxxxxxx/key/xxxxxxxxxxxxx/temp/210/tempin/233/chill/214/heat/247/hum/33/humin/29/wspd/30/wspdhi/30/wspdavg/21/wdir/271/wdiravg/256/bar/10175/rain/0/solarrad/1630/uvi/ 5/ver/1.2/type/201
```
 - wid: Device id
 - key: Device key
 - temp: Temperature 
 - tempin: Internal temperature (not implemented)
 - hum: Humidity
 - humin: Internal humidity (not implemented)
 - wspd: Wind speed (not implemented)
 - wdir: Wind direction (not implemented)
 - bar: Pressure hPa
 - rain: Rain (yes(1) or no(0) ... i don't know !!!)
 - uvi: Ultraviolet index
