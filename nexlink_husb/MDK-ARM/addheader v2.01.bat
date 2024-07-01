set "year=%date:~2,2%"
set "month=%date:~5,2%"
set "day=%date:~8,2%"
set "hour_ten=%time:~0,1%"
set "hour_one=%time:~1,1%"
set "minute=%time:~3,2%"
set "second=%time:~6,2%"
mkdir output
echo appheader v2.01 t%year%%month%%day%%hour_ten%%hour_one%%minute%%second% > "nexlink_f407_husb\appheader.bin"
copy /b nexlink_f407_husb\appheader.bin + nexlink_f407_husb\nexlink_f407_husb.bin output\nexlink_f407_husb_merge_20%year%%month%%day%.bin
del "nexlink_f407_husb\appheader.bin"

echo skip 32bytes