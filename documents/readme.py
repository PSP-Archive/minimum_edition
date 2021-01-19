#!/usr/bin/python

import  sys

#
what_is_this = '''CFW %s Minimum Edition for 01g, 02g model by neur0n

-- What is this?--
This is CFW work on PSP1000 and PSP2000.

The degree of stability is still unknown. 
Please test it on your own and refer to different people's test reports.

From 4.01M33 -> OK
From 5.00M33 -> OK
From 5.50GEN -> OK(may need to edit version.txt)
From 5.50Prometheus -> OK(may need to edit version.txt)
From 6.20TN-D -> OK
From 6.37ME -> Error
From 6.38ME -> OK
From 6.39ME -> OK
From 6.20PRO-B4 -> Error
From 6.35PRO-B4 -> Error
From 6.20PRO-B5 -> OK
From 6.35PRO-B5 -> OK
From 6.39PRO-B6 -> OK
'''

#
what_is_this_lite = '''LCFW %sLME installer for OFW%s

-- What is this? -- 
This is LCFW for OFW%s.

The degree of stability is still unknown. 
Please test it on your own and refer to different people's test reports.
'''
#
how_to_install = '''--How to Install--
First, you need to install CFW or HEN in your PSP.

1. copy UPDATE folder at ms0:/PSP/GAME/.
2. Put %s official update at ms0:/PSP/GAME/UPDATE/%s.PBP, or let the program download it for you can use a wifi connection.
3. Run installer from xmb.

-- features --
Hold R trigger and turn on the psp, you can enter recovery menu.
Hold Home button and turn on the psp, you can Boot OFW.
'''
#
how_to_install_lite = '''
-- How to use --
Copy "installer" folder and "launcher" folder at "ms0:/PSP/GAME/"

-- Preparation --
First. You need to install LCFW modules in your PSP.

1.Execute "LME Installer for %s" from XMB.
2.You can select these action:
Press x to install LCFW.
Press [] to uninstall LCFW.
Press R to exit.

After the action, PSP will rboot.

-- Start LCFW --
1.Execute "LME Launcher for %s" from XMB.
2. enjoy :)

-- How to enter RecoveryMenu? --
From XMB :Open VshMenu and select "Enter Recovery ->"
From LCFW:Execute Launcher again.
From OFW :Execute Launcher with hold "R".

-- How to mount UmdVideo? --
0. Enable UmdVideo option at RecoveryMenu->config->Use UmdVideo Patch (Go only).
1. copy Iso file at ms(ef)0:/ISO/VIDEO/ .
2. Open VshMenu and select Iso file name.
'''
#
credit = '''
-- Credit --
Dark_AleX	: For his great CFW. This CFW is based his 5.00M33.
Moz		: For his beta test.
'''
#
credit_lite = '''-- Credit --
some1		: For his sceHttpStorageOpen kxploit.
liquidzigong	: For his 639kxploit POC.
bbtgp		: For his PrxEncrypter.
Draan		: For his kirk-engine code.
J416		: For his rebootex sample.
N-zaki		: For providing PSPGo from him.
'''
#
credit_common = '''BombingBasta	: For his translated language file in French.
Rapper_skull	: For his translated language file in Italian.
The Z 		: For his translated language file in German.
largeroliker 	: For his translated language file in Spanish.
bassiw		: For his translated language file in Netherlands.
Moz		: For his translated language file in Italian.
ABCanG &Dadrfy &estuibal &plum &SnyFbSx &teck4 &wn : For their work to collect nid list.
Tipped OuT	: For his EBOOT.PBP ICON0.
'''

about_translating = '''-- About translating the recovery menu --
To translate the recovery ,create a text file in ms0:/seplugins/xx_recovery.txt or flash1:/xx_recovery.txt .
"xx" is the language code of your language.

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

To use custom font in RecoveryMenu, put font data at ms0:/seplugins/font_recovery.bin.
'''

def usage():
	print ("Usage: " + sys.argv[0] + " infile outfile label")


def main():

	len_arg = len(sys.argv)
	if( len_arg < 2):
		usage()
		sys.exit()

	version_str = sys.argv[1]

	if len_arg == 2:
		print what_is_this %(version_str)
		print how_to_install %(version_str, version_str.replace(".", ""))
		print credit + credit_common
	else:
		print what_is_this_lite %(version_str, version_str, version_str)
		print how_to_install_lite %(version_str, version_str )
		print credit_lite + credit_common


if __name__ == "__main__":
	main()
