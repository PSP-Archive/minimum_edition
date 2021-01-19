-- 何これ？--
FW6.60向けのLCFWです。

注意
このツールを使って何か不具合が出ても自己責任でお願いします。製作者は一切責任をとりません。

ｰｰファイルの説明

- installer (XMBでの表示名は "LME Installer for 6.60")
flash0に必要なファイルを書き込むツールです。アンインストールもできます。

- launcher (XMBでの表示名は "LME Launcher for 6.60" )
LCFWを起動するツールです。
上の installer で必要なモジュールを書き込んでいないと起動しません。


使い方。

1,OFW6.60でinstallerを起動してしばらく待つ。
2,
Press X to install modules
Press [] to uninstall modules
R to exit

と表示されるはずなので、インストールする場合は　Xを、
アンインストールする場合は　[]（四角)を押してください。
Rを押すと何もしないで終了します。

------ここから下はインストールを選んだ人向けの説明です。--------
3,PSPが再起動してXMBに戻るはずです。この状態ではまだOFWのままです。
4,XMBから launcher を起動します。
5,自動的ににXMBに戻ります。これでLCFWが適用状態になっているはずです。

一度インストールしてしまえば、次回からはlauncherの起動のみでおｋです。

-- リカバリーモードに入るには？　--
XMBから　:VshMenuで　EnterRecovery-> を選択。
LCFWから：もう一度launcherを起動してください。
OFWから：Rを押しながらlauncherを起動してください。

-- UmdVideo のマウント --
0. RecoveryMenu->config->Use UmdVideo Patch の設定を有効にしてください(Go だけ).
1. ms(ef)0:/ISO/VIDEO/ にIsoをコピー.
2. VshMenu からマウントするIsoを選択.

-- credit --
some1/Davee/Proxima/Zecoxao: For their kxploit.
liquidzigong	: For his 639kxploit POC.
bbtgp		: For his PrxEncrypter.
Draan		: For his kirk-engine code.
J416		: For his rebootex sample.
N-zaki		: For providing PSPGo from him.
BombingBasta	: For his translated language file in French.
Rapper_skull	: For his translated language file in Italian.
The Z 		: For his translated language file in German.
largeroliker 	: For his translated language file in Spanish.
bassiw		: For his translated language file in Netherlands.
Moz		: For his translated language file in Italian.
Publikacii	: For his translated language file in Bulgarian.
Yoti		: For his translated language file in Russian.
ExzoTikFruiT	: For his translated language file in Russian.
ABCanG &Dadrfy &estuibal &plum &SnyFbSx &teck4 &wn : For their work to collect nid list.
Tipped OuT	: For his EBOOT.PBP ICON0.

-- RecoveryMenuのカスタムフォントとTxtについて--
ms0:/seplugins/xx_recovery.txt か flash1:/xx_recoery.txtに言語TXTファイルを置けばそれを読み込む仕様です。
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

カスタムフォントを使いたい場合は、ms0:/seplugins/font_recovery.binにフォントデータを置いてください。
(TNのと互換性あり)

translateフォルダに半角カタカナのサンプルが入っていますが、単体では使えません。
HAROTURBO氏の　M33リカバリー半角かなふぁいる(http://unzu127xp.pa.land.to/data/patch.html)(TN用のやつ)
と組み合わせて使ってください。

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


