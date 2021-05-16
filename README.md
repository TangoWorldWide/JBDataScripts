# Jailbreak Data Collection Scripts
This is a collection of scripts for collecting an analyzing data from Jailbreak.

## index.js  
#### Program written by FetchBot  
This program pings Jailbreak's gameME API (http://tangoworldwide.gameme.com/api/serverinfo/104.153.108.145:27015/players) every 5 minutes and collects the data in two forms.  

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
jb_quad_v2: 15
jb_sorse_v7: 26
```  

### Instructions
1. Navigate to the folder containing the code and build the program with this command:  
```
g++ playerHours.cpp -o playerHours --std=c++11  
```
2. Once it finishes building, run the program with  
```
./playerHours
```  
3. The program will prompt you for a `.csv` file. If the file is in the same folder as the executable, you can just input `results.csv`. Otherwise, include a path to the `.csv`.  
4. Once it finishes loading the file, it will prompt you for a line to start reading at. This can be used to only read data from a certian date forwards. You can find the line to start at by opening the data in a text or spreadsheet editor and scrolling until you see the date you want to start at in the left-hand column. Note the line number next to it and then input it into the program.  
	You can input 0 to start at the beginning of the file.
5. The program will output a sorted list of all the maps recorded from the specified line with the combined number of players next to them.

#### Example Output:  
```
 ./playerHours
file path to .csv file:
results_5-16-21.csv
loading file...
done loading!
10265 entries loaded.
At what line do you want to start? (0 is the beginning)
0
analylizing...
done analyzing!
jb_synergy_v3f: 1
training1: 1
de_mocha: 2
de_grind: 4
de_bank: 4
lobby_mapveto: 4
ba_jail_canyondam_go: 6
cs_agency: 6
de_nuke: 10
dz_blacksite: 11
de_pitstop: 18
jb_longstreet_tww: 21
dz_sirocco: 37
de_shortdust: 51
cs_office: 58
jb_fentex_fix_hdr: 73
de_dust2: 117
jb_sorse_v7: 123
jb_synergy_v3g: 141
jb_oasis_v2c: 147
jb_moonjail_v2/jb_moonjail_v2: 151
jb_tropico_v2: 176
jb_autumn_v2: 180
jb_lego_jail_2k17b: 218
jb_mario_rescue_v1_2: 250
jb_bigmt_b2: 256
ba_mlcastle_se: 273
jb_mountaincraft_v6: 288
jb_quad_v3: 317
de_mirage: 372
jb_obama_v5_beta_tww: 382
jb_mountaincraft_tww: 454
jb_vice_tango_final: 469
jb_synergy_v3e: 501
jb_chicken_island_b2: 706
jb_pyrenees_v1_2: 785
jb_quake_a04d: 1044
jb_minecraft_tango_v3: 1581
jb_moonjail_v2: 2148
jb_dystopian_b6: 2618
jb_kwejsi_v5_fix: 2636
jb_quad_v2: 2984
jb_vipinthemix_tango_final: 3886
jb_avalanche_csgo_b8c: 4390
jb_arcade_b5: 6341
jb_moti2_final: 6563
jb_clouds_tango_v7d: 8102
jb_sg_dojo_v5-6: 8266
jb_spy_vs_spy_beta7: 8369
jb_minecraft_tango_v2: 9036
jb_clouds_beta02: 9702
jb_undertale_v1d: 9785
jb_peanut_tango_v3: 11154
```
