@echo If .tbl files are added or removed you will need to hand edit
@echo this batch file.
@echo off
make -l -f makefile.bcb
makeuctb cp1250_uni.tbl > cp1250_uni.h
makeuctb cp1251_uni.tbl > cp1251_uni.h
makeuctb cp1252_uni.tbl > cp1252_uni.h
makeuctb cp1253_uni.tbl > cp1253_uni.h
makeuctb cp1255_uni.tbl > cp1255_uni.h
makeuctb cp1256_uni.tbl > cp1256_uni.h
makeuctb cp1257_uni.tbl > cp1257_uni.h
makeuctb cp437_uni.tbl > cp437_uni.h
makeuctb cp737_uni.tbl > cp737_uni.h
makeuctb cp775_uni.tbl > cp775_uni.h
makeuctb cp850_uni.tbl > cp850_uni.h
makeuctb cp852_uni.tbl > cp852_uni.h
makeuctb cp862_uni.tbl > cp862_uni.h
makeuctb cp864_uni.tbl > cp864_uni.h
makeuctb cp866_uni.tbl > cp866_uni.h
makeuctb cp866u_uni.tbl > cp866u_uni.h
makeuctb cp869_uni.tbl > cp869_uni.h
makeuctb def7_uni.tbl  > def7_uni.h
makeuctb dmcs_uni.tbl > dmcs_uni.h
makeuctb hp_uni.tbl > hp_uni.h
makeuctb iso01_uni.tbl > iso01_uni.h
makeuctb iso02_uni.tbl > iso02_uni.h
makeuctb iso03_uni.tbl > iso03_uni.h
makeuctb iso04_uni.tbl > iso04_uni.h
makeuctb iso05_uni.tbl > iso05_uni.h
makeuctb iso06_uni.tbl > iso06_uni.h
makeuctb iso07_uni.tbl > iso07_uni.h
makeuctb iso08_uni.tbl > iso08_uni.h
makeuctb iso09_uni.tbl > iso09_uni.h
makeuctb iso10_uni.tbl > iso10_uni.h
makeuctb iso15_uni.tbl > iso15_uni.h
makeuctb koi8r_uni.tbl > koi8r_uni.h
makeuctb koi8u_uni.tbl > koi8u_uni.h
makeuctb mac_uni.tbl > mac_uni.h
makeuctb mnem2_suni.tbl > mnem2_suni.h
makeuctb mnem_suni.tbl > mnem_suni.h
makeuctb next_uni.tbl > next_uni.h
makeuctb rfc_suni.tbl > rfc_suni.h
makeuctb utf8_uni.tbl > utf8_uni.h
makeuctb viscii_uni.tbl > viscii_uni.h
