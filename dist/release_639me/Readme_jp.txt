CFW 6.39 Minimum Edition for 01g, 02g model by neur0n

-- 何これ?--
PSP1000 と　PSP2000で動くCFWです。

From 5.00M33 -> OK
From 5.50GEN -> OK(version.txtを編集する必要があるかも)
From 5.50Prometheus -> OK(version.txtを編集する必要があるかも)
From 6.20TN-D -> OK
From 6.37ME -> Error
From 6.38ME -> OK
From 6.20PRO-B4 -> Error
From 6.35PRO-B4 -> Error
From 6.20PRO-B5 -> OK
From 6.35PRO-B5 -> OK
From 6.39PRO-B6 -> OK

--インｽトール--
上のリストを参考に、予めCFW かHENをインストールしておくこと。

1. UPDATE フォルダを ms0:/PSP/GAME/にコピーする.
2. FW6.39の公式アップデータをリネームして ms0:/PSP/GAME/UPDATE/639.PBP にコピーする.
3. xmbから起動.


-- 機能--
R 押しながら起動するとrecovery menuに入れます.
Homeを押しながら起動するとOFWが起動します.

-- Credit --
Dark_AleX : For his great CFW. This CFW is based his 5.00M33.
Moz : For his beta test and translated language file in Italian..
BombingBasta: For his translated language file in French.
Rapper_skull: For his translated language file in Italian.
The Z : For his translated language file in German.
largeroliker : For his translated language file in Spanish.
Tipped OuT	: For his ICON for EBOOT.PBP.

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
-- 変更可能なCPUクロック数に 166/83 を追加.
v9.3
-- ms0:/seplugins/version.txt からもversion.txtを読み込めるようにした.
v9.2
-- XMBでISOを読み込む際のバグ修正.
-- PBOOT.PBP実行オプションを追加.
-- スリープに入った際のUMDドライブの異音を修正.
v9.1
-- sctrlKernelSetUserLevelのバグ修正。
-- Ledaのﾊﾟｯﾁを追加。ef0:/からでも使えるようにした。
v9
-- 公式StaticELFファイル読み込み時のバグ修正。
-- GAMEモードでのCPUクロック変更値が固定できていなかったバグ修正。
-- ISOで発生する0x80020148エラーのﾊﾟｯﾁを追加。
v8
-- ISOの視聴制限レベルを固定値ではなく、ISOから取得するようにした.
-- OEdriverで一部のゲームが動作しないバグ修正.
-- XMBでCPUクロックを変更した際にスリープ等が効かなくなるバグを修正.
-- RecoveryMenu->Pluginでプラグインの数がページ数を超えた際の対策を追加.
-- UMD Videoマウントを更新。ダミーUMD無しでつかえるようにした.
v7
-- Dark_AleX氏のledaを使った際クラッシュする問題を修正。
-- ISOの視聴制限レベルを5から1に変更。
-- PluginManagerで表示可能なファイル名を32文字から64文字に変更。
v6
-- RecoveryMenu でカスタムフォントとTxtを使えるようにした。
-- GamebootSkip機能を搭載。
v5
-- VshMenuにリカバリモードに入るオプションを追加。
-- OE driver のバグ修正。
v4
-- VshMenuを開いたままスリープするとフリーズしていたバグを修正。
-- MEdriver のバグ修正。
-- RecoveryMenuのバッテリー関連のオプションをBatteryConfigに放り込んだ
-- Speed Up MS(β)を実装。
v3
-- VshMenuにResetVSHを追加。
-- RecoveryMenuにAutoBootBatteryを作る機能を追加。
-- RecoveryMenuから抜けた際にSCEロゴをスキップできるようにした。
v2
-- PSNに接続しようとするとフリーズするバグを修正
v1
-- 最初のリリース
-- RecoveryMode のテキストのミスを修正
-- 拡張メモリー使用時のバグを修正
-- NetworkUpdateをデフォルトで有効になるように変更
-- ソースコードの最適化