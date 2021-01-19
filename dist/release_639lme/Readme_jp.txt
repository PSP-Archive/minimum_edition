-- 何これ？--
FW6.39向けのLCFWです。

注意
このツールを使って何か不具合が出ても自己責任でお願いします。製作者は一切責任をとりません。

ｰｰファイルの説明

- installer (XMBでの表示名は "LME Installer for 6.39")
flash0に必要なファイルを書き込むツールです。アンインストールもできます。

- launcher (XMBでの表示名は "LME Launcher for 6.39" )
LCFWを起動するツールです。
上の installer で必要なモジュールを書き込んでいないと起動しません。


使い方。

1,OFW6.39でinstallerを起動してしばらく待つ。
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
some1: For his sceHttpStorageOpen kxploit.
liquidzigong: For his 639kxploit POC.
bbtgp: For his PrxEncrypter.
Draan: For his kirk-engine code.
J416: For his rebootex sample.
N-zaki : For providing PSPGo from him.

Moz	    : For his translated language file in Italian
BombingBasta: For his translated language file in French.
Rapper_skull: For his translated language file in Italian.
The Z : For his translated language file in German.
largeroliker : For his translated language file in Spanish.
Tipped OuT	: For his ICON for EBOOT.PBP.

Thanks for beta tester
@tablet6883
@Zooner_MT
@sayane_0032
@apolo1192
@PopStatue
@dsk_ro_ra
@potechihime
@Mr__Nerve
@wakatake0507
@tsumurin21
@altheme0r
@gluishia
@__taba__
@g_thail
@Satan_Rage
@teck4
@SnyFbSx

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
v9.8
-- SpeedUpMsが有効になっている時にPSPをスリープさせるとまれにフリーズするバグを修正
ｰｰ RecoveryMenuのテキストの色を変更するオプションを追加
v9.4
-- POPSの中断データが再開できないバグを修正(05gのみ)
-- M2カードが挿入されていない時にPOPSのプラグインが読み込まれないバグ修正.(05gのみ)
-- 変更可能なCPUクロック数に 166/83 を追加.
v9.3
-- ISOの中断データがライセンスエラーで実行できないバグ修正。(05gのみ)
-- ms0:/seplugins/version.txt からもversion.txtを読み込めるようにした.
v9.2
-- XMBでISOを読み込む際のバグ修正.
-- PBOOT.PBP実行オプションを追加.
-- スリープに入った際のUMDドライブの異音を修正.
-- XMBではef0:とms0:の両方からプラグインを読み込む仕様に変更(05gのみ)
v9.1
-- sctrlKernelSetUserLevelのバグ修正。
-- Ledaのﾊﾟｯﾁを追加。ef0:/からでも使えるようにした。
v9
-- 公式StaticELFファイル読み込み時のバグ修正。
-- GAMEモードでのCPUクロック変更値が固定できていなかったバグ修正。
-- ISOで発生する0x80020148エラーのﾊﾟｯﾁを追加。
-- UmdVideoﾊﾟｯﾁの修正(beta4. 05gのみ)
v8
-- ISOの視聴制限レベルを固定値ではなく、ISOから取得するようにした.
-- OEdriverで一部のゲームが動作しないバグ修正.
-- XMBでCPUクロックを変更した際にスリープ等が効かなくなるバグを修正.
-- RecoveryMenu->Pluginでプラグインの数がページ数を超えた際の対策を追加.
-- UMD Videoマウントを更新。ダミーUMD無しでつかえるようにした.
v7
-- ledaを使った時のクラッシュする問題を修正。
-- ISOの視聴制限レベルを5から1に変更。
-- PluginManagerで表示可能なファイル名を32文字から64文字に変更。
v6 fix2
-- Launcherのバグ修正。
v6 fix
-- 一部のPSNゲームが起動しないバグを修正。
v6
-- RecoveryMenu でカスタムフォントとTxtを使えるようにした。
-- GamebootSkip機能を搭載。
v5
-- VshMenuにリカバリーモードい入るオプションを追加。
-- OE driver のバグ修正。
v4
-- VshMenuを開いたままスリープするとフリーズしていたバグを修正。
-- MEdriver のバグ修正。
-- RecoveryMenuのバッテリー関連のオプションをBatteryConfigに放り込んだ
-- Speed Up MS(β)を実装。
v3
-- "639launcher"を更新。PRO-B6との共存が可能に。



