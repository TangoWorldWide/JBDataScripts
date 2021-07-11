# Jailbreak Data Collection Scripts
This is a collection of scripts for collecting an analyzing data from Jailbreak.

## index.js  
#### Program written by FetchBot  
This program pings Jailbreak's gameME [API](http://tangoworldwide.gameme.com/api/serverinfo/104.153.108.145:27015/players) every 5 minutes and collects the data in two forms.  

1. Inside `/out/results.csv`, you can find the processed API calls. The file contains 5 columns: timestamp (the month might be inaccurate for some reason), number of players on T, number of players on CT, SteamIDs of players on T, SteamIds of players on CT. The first line will be left blank for you to add your own headers. It's recommended to replace the first line with this:  
  ```
  _id,"T players","CT Players",Map,"T Steamids","CT steamids"
  ```
  This file is the ideal format for doing analysis on.  
  
2. Inside `/out/dumps/`, there are two folders, `server/` and `players/` that contain the data collected from the API. You will need to build your own tool to parse these.  

The gameME API is pretty terrible, so sometimes it will stop collecting data for a few hours, causing errors to appear in console. You can let the program keep running and it will resume collecting data when the API starts working again.  

### Instructions  
1. Install node.js (https://nodejs.org/en/)  
2. Navigate to the folder containing the script inside the terminal of your choice.  
3. Do `node index.js` to run the program. The program will run until it is stopped.  
4. The program can be stopped with `Ctrl+C`.  

## playerHours.cpp  
#### Written by 9yz  
This program takes the `.csv` file output by the above program and analyzes it to find the most played maps. It does this by adding up the players (players on T + players on CT) every time the map is recorded.  
For example, if the data recorded is:  

|Map|Players|
|-- |-------|
|jb_quad_v2|10|
|jb_quad_v2|5|
|jb_sorse_v7|8|
|jb_sorse_v7|8|
|jb_sorse_v7|10|

The tallies returned by the program would be:  
```
jb_quad_v2: 75
jb_sorse_v7: 130
```  
(The program multiplies the values by 5 to get the number of player-minutes spent on each map)

### Instructions
1. Navigate to the folder containing the code and build the program with this command:  
```
g++ playerHours.cpp -o playerHours --std=c++17  
```
2. Once it finishes building, run the program with  
```
./playerHours <csv path> -wdc [number]
```  
_-w:_ proccesses the last week of data (the last 2,016 entries)

_-d:_ proccesses the last day of data (288 entries)

_-c_ [number]: procces all data starting at line [number]. 0 for the beginning of the file.

ex. `./playerhours results.csv -c 10562`


#### Example Outputs:  
```
./playerhours results.csv -c 3000
loading file...
done loading!
3728 entries loaded.
Starting at line 3000
Analylizing...
done analyzing!
jb_vice_tango_final: 28
ba_mlcastle_se: 64
jb_fentex_fix_hdr: 70
jb_quake_a04d: 73
jb_sorse_v7: 101
jb_pyrenees_v1_2: 121
jb_quad_v2: 162
jb_dystopian_b6: 179
jb_synergy_v3e: 211
jb_moti2_final: 218
jb_mountaincraft_tww: 274
jb_avalanche_csgo_b8c: 276
jb_vipinthemix_tango_final: 281
jb_kwejsi_v5_fix: 302
jb_arcade_b5: 596
jb_peanut_tango_v3: 734
jb_sg_dojo_v5-6: 794
jb_spy_vs_spy_beta7: 802
jb_minecraft_tango_v2: 841
jb_undertale_v1d: 1019
jb_clouds_tango_v7d: 1430
```

```
./playerhours results.csv -w
loading file...
done loading!
3728 entries loaded.
Starting at line 1712
Analylizing...
done analyzing!
jb_chicken_island_b2: 22
jb_autumn_v2: 23
jb_vice_tango_final: 53
jb_fentex_fix_hdr: 70
jb_quake_a04d: 85
jb_sorse_v7: 101
ba_mlcastle_se: 196
jb_synergy_v3e: 221
jb_mountaincraft_tww: 274
jb_pyrenees_v1_2: 283
jb_quad_v2: 386
jb_avalanche_csgo_b8c: 478
jb_dystopian_b6: 675
jb_vipinthemix_tango_final: 687
jb_arcade_b5: 1289
jb_kwejsi_v5_fix: 1311
jb_spy_vs_spy_beta7: 1463
jb_moti2_final: 1704
jb_sg_dojo_v5-6: 2306
jb_minecraft_tango_v2: 2393
jb_peanut_tango_v3: 2662
jb_undertale_v1d: 3046
jb_clouds_tango_v7d: 4170
```
