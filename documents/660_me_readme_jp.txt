CFW 6.60 Minimum Edition for 01g, 02g model by neur0n

-- 何これ?--
PSP1000 と　PSP2000で動くCFWです。

From 4.01M33 -> OK
From 5.00M33 -> OK
From 5.50GEN -> OK(version.txtを編集する必要があるかも)
From 5.50Prometheus -> OK(version.txtを編集する必要があるかも)
From 6.20TN-D -> OK
From 6.37ME -> Error
From 6.38ME -> OK
From 6.39ME -> OK
From 6.20PRO-B4 -> Error
From 6.35PRO-B4 -> Error
From 6.20PRO-B5 -> OK
From 6.35PRO-B5 -> OK
From 6.39PRO-B6 -> OK

--インｽトール--
上のリストを参考に、予めCFW かHENをインストールしておくこと。

1. UPDATE フォルダを ms0:/PSP/GAME/にコピーする.
2. FW6.60の公式アップデータをリネームして ms0:/PSP/GAME/UPDATE/660.PBP にコピーする.( Wifi環境がある場合にはPSPで直接DLできます。)
3. xmbから起動.

-- 機能--
R 押しながら起動するとrecovery menuに入れます.
Homeを押しながら起動するとOFWが起動します.

-- Credit --
Dark_AleX	: For his great CFW. This CFW is based his 5.00M33.
Moz		: For his beta test and translated language file in Italian.
BombingBasta	: For his translated language file in French.
Rapper_skull	: For his translated language file in Italian.
The Z 		: For his translated language file in German.
largeroliker 	: For his translated language file in Spanish.
bassiw		: For his translated language file in Netherlands.
Publikacii	: For his translated language file in Bulgarian.
Yoti		: For his translated language file in Russian.
ExzoTikFruiT	: For his translated language file in Russian.
ABCanG &Dadrfy &estuibal &plum &SnyFbSx &teck4 &wn : For their work to collect nid list.
Tipped OuT	: For his EBOOT.PBP ICON0.

-- RecoveryMenuのカスタムフォントとTxtについて--
ms0:/seplugins/xx_recovery.txt か flash1:/xx_recovery.txtに言語TXTファイルを置けばそれを読み込む仕様です。
"xx" に入るのが以下の言語コードです。
es -> spanish
en -> english
fr -> french
de -> german
it -> italian
ja -> japanese
ko -> korean
nl -> netherlands
pt -> portuguese
ru -> russian
ch1 -> chinese simplified
ch2 -> chinese traditional

VshMenuの場合のファイル名は　xx_vshmenu.txt です。
フォントのパスはms0:/seplugins/xx_ftable.binです。

--履歴--
v1.7
-- SpeedUpMsが有効になっている時にPSPをスリープさせるとまれにフリーズするバグを修正
ｰｰ RecoveryMenuのテキストの色を変更するオプションを追加
-- パッチの最適化.
v1.6
-- CPUクロック数の 199/99 を 200/100　に変更;
-- VshMenuを更新. 翻訳機能を追加した。
-- XMBでUSBを通じてUMDをダンプする際にサイズが正しくなかったバグ修正.
v1.5
-- 変更可能なCPUクロック数に 199/99 を追加.
-- VshMenuの設定保存時のバグ修正.
-- パッチの最適化.
v1.4
-- Fixed USB Connect in Recovery menu.(05g only).
-- Update VshMenu.
v1.3
-- Netconfigのﾊﾞｯｸｱｯﾌﾟ/復元機能をRecovery menuに追加.
-- RemotePlay, Skype, 1seg and PSN-store使用時にVshMenuが開いてしまうバグ修正.
v1.2
-- RecoveryMenuを更新.
-- 変更可能なCPUクロック数に 166/83 を追加.
v1.1
-- ms0:/seplugins/version.txt からもversion.txtを読み込めるようにした.
-- NidResolverを更新。FuSa_SD.prx動くよ.
v1
-- sctrlHENPatchSyscallのバグ修正
beta3
-- Normal_driverのバグ修正
-- XMBでVideoを選択した時のバグ修正
beta2
-- 自炊PS1が起動できないバグ修正.
beta
-- First release.
