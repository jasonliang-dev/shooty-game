<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.5" tiledversion="1.7.2" name="tiles" tilewidth="16" tileheight="16" spacing="4" margin="2" tilecount="144" columns="12">
 <image source="tiles.png" width="256" height="256"/>
 <tile id="1" type="out top left"/>
 <tile id="2" type="out top"/>
 <tile id="3" type="out top right"/>
 <tile id="4" type="in bottom right"/>
 <tile id="5" type="in bottom left"/>
 <tile id="6" type="full"/>
 <tile id="13" type="out left"/>
 <tile id="15" type="out right"/>
 <tile id="16" type="in top right"/>
 <tile id="17" type="in top left"/>
 <tile id="25" type="out bottom left"/>
 <tile id="26" type="out bottom"/>
 <tile id="27" type="out bottom right"/>
 <wangsets>
  <wangset name="grass" type="mixed" tile="-1">
   <wangcolor name="water" color="#ff0000" tile="-1" probability="1"/>
   <wangcolor name="land" color="#00ff00" tile="-1" probability="1"/>
   <wangtile tileid="1" wangid="1,1,2,2,2,1,1,1"/>
   <wangtile tileid="2" wangid="1,1,2,2,2,2,2,1"/>
   <wangtile tileid="3" wangid="1,1,1,1,2,2,2,1"/>
   <wangtile tileid="4" wangid="2,2,2,1,2,2,2,2"/>
   <wangtile tileid="5" wangid="2,2,2,2,2,1,2,2"/>
   <wangtile tileid="6" wangid="1,1,1,1,1,1,1,1"/>
   <wangtile tileid="13" wangid="2,2,2,2,2,1,1,1"/>
   <wangtile tileid="14" wangid="2,2,2,2,2,2,2,2"/>
   <wangtile tileid="15" wangid="2,1,1,1,2,2,2,2"/>
   <wangtile tileid="16" wangid="2,1,2,2,2,2,2,2"/>
   <wangtile tileid="17" wangid="2,2,2,2,2,2,2,1"/>
   <wangtile tileid="25" wangid="2,2,2,1,1,1,1,1"/>
   <wangtile tileid="26" wangid="2,2,2,1,1,1,2,2"/>
   <wangtile tileid="27" wangid="2,1,1,1,1,1,2,2"/>
   <wangtile tileid="74" wangid="2,2,2,2,2,2,2,2"/>
  </wangset>
 </wangsets>
</tileset>
