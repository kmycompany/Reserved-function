#include "ShuRuFa.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "key.h"
#include "lcd.h"
#include "mypure_library.h"
#include "my_library.h"
#include "stm32f10x.h"
//#include "my_stdint.h"
#include "setting.h"
#include "FoodOrder.h"


typedef struct
{
	const u8 *T9;
	const u8 *PY;
	const u8 *MB;
} T9PY_IDX;

static short cursorX;
static short cursorY;


//#define AddBiHuaPinYin


#ifdef AddBiHuaPinYin
const u8 PY_mb_space [] = {"  "};
const u8 PY_mb_fuhao [] = {"¡££¬¡¢£»£º£¿£¡¡­¡ª¡¤¡¥¡§¡®¡¯¡°¡±¡©¡«¡¬¡Ã£¢£§£à£ü¡¨¡²¡³¡´¡µ¡¶¡·¡¸¡¹¡º¡»£®¡¼¡½¡¾¡¿£¨£©£Û£Ý£û£ý¡Ö¡Ü¡Ý£¼£¾¡Ú¡Û£«£­¡Á¡Â£¯¡Ò¡Ó¡Ø¡Þ¡Ä¡Å¡Æ¡Ç¡È¡É¡Ê¡Í¡Î¡Ï¡Ð¡Ñ¡Õ¡×"};
const u8 PY_mb_a     [] = {"°¡°¢"};
const u8 PY_mb_ai    [] = {"°®°¤°­°¥°£°«°¬°¦°§°©°¨°ª°¯"};
const u8 PY_mb_an    [] = {"°´°²°µ°¸°°°³°¶°±°·"};
const u8 PY_mb_ang   [] = {"°º°»°¹"};
const u8 PY_mb_ao    [] = {"°Á°¼°½°¾°Â°¿°À°Ã°Ä"};
const u8 PY_mb_b     [] = {"°Ë°×°ì°î"}; ////
const u8 PY_mb_ba    [] = {"°Ë°Ñ°Í°Ö°Õ°Ô°Î°Ç°É°Ó°Å°Ì°Æ°Ê°Ï°È°Ð"};
const u8 PY_mb_bai   [] = {"°×°Ù°Ú°Ü°Ý°Û°Ø°Þ"};
const u8 PY_mb_ban   [] = {"°ì°ë°à°ã°é°å°æ°á°è°ç°ä°â°ß°í°ê"};
const u8 PY_mb_bang  [] = {"°î°ï°ó°ø°ñ°÷°ò°ô°ù°õ°ö°ð"};
const u8 PY_mb_bao   [] = {"°ü±£±¦±¨±§±¥±©±¬°û±¡°ý±¢±¤±ª°ú±«°þÆÙ"};
const u8 PY_mb_bei   [] = {"±°±­±¯±®±±±´±·±¸±³±µ±¶±»±¹±º±²"};
const u8 PY_mb_ben   [] = {"±¼±¾±½±¿º»"};
const u8 PY_mb_beng  [] = {"±À±Á±Â±Ã±Å±Ä"};
const u8 PY_mb_bi    [] = {"±Æ±Ç±È±Ë±Ê±É±Ò±Ø±Ï±Õ±Ó±Ñ±Ý±Ð±Ö±Ô±Í±×±Ì±Î±Ú±Ü±Û"};
const u8 PY_mb_bian  [] = {"±ß±à±Þ±á±â±å±ã±ä±é±æ±ç±è"};
const u8 PY_mb_biao  [] = {"±ë±ê±ì±í"};
const u8 PY_mb_bie   [] = {"±ï±î±ð±ñ"};
const u8 PY_mb_bin   [] = {"±ö±ò±ó±õ±ô±÷"};
const u8 PY_mb_bing  [] = {"±ù±ø±û±ü±ú±þ±ý²¢²¡"};
const u8 PY_mb_bo    [] = {"²¦²¨²£²§²±²¤²¥²®²µ²¯²´²ª²¬²°²©²³²«²­²²²·"};
const u8 PY_mb_bu    [] = {"²¹²¸²¶²»²¼²½²À²¿²º²¾"};
const u8 PY_mb_c     [] = {"²Á²Â²Î²Ö"}; ///
const u8 PY_mb_ca    [] = {"²Á"};
const u8 PY_mb_cai   [] = {"²Â²Å²Ä²Æ²Ã²É²Ê²Ç²È²Ë²Ì"};
const u8 PY_mb_can   [] = {"²Î²Í²Ð²Ï²Ñ²Ò²Ó"};
const u8 PY_mb_cang  [] = {"²Ö²×²Ô²Õ²Ø"};
const u8 PY_mb_cao   [] = {"²Ù²Ú²Ü²Û²Ý"};
const u8 PY_mb_ce    [] = {"²á²à²Þ²â²ß"};
const u8 PY_mb_ceng  [] = {"²ã²äÔø"};
const u8 PY_mb_cha   [] = {"²æ²å²é²ç²è²ë²ì²ê²í²ï²îÉ²"};
const u8 PY_mb_chai  [] = {"²ð²ñ²ò"};
const u8 PY_mb_chan  [] = {"²ô²ó²÷²ö²ø²õ²ú²ù²û²ü"};
const u8 PY_mb_chang [] = {"²ý²þ³¦³¢³¥³£³§³¡³¨³©³«³ª"};
const u8 PY_mb_chao  [] = {"³­³®³¬³²³¯³°³±³³³´´Â"};
const u8 PY_mb_che   [] = {"³µ³¶³¹³¸³·³º"};
const u8 PY_mb_chen  [] = {"³»³¾³¼³À³Á³½³Â³¿³Ä³Ã"};
const u8 PY_mb_cheng [] = {"³Æ³Å³É³Ê³Ð³Ï³Ç³Ë³Í³Ì³Î³È³Ñ³Ò³Ó"};
const u8 PY_mb_chi   [] = {"³Ô³Õ³Ú³Ø³Û³Ù³Ö³ß³Þ³Ý³Ü³â³à³ã³á"};
const u8 PY_mb_chong [] = {"³ä³å³æ³ç³è"};
const u8 PY_mb_chou  [] = {"³é³ð³ñ³ë³î³í³ï³ê³ì³ó³ò³ô"};
const u8 PY_mb_chu   [] = {"³ö³õ³ý³ø³ü³ú³û³÷³ù´¡´¢³þ´¦´¤´¥´£Ðó"};
const u8 PY_mb_chuai [] = {"´§"};
const u8 PY_mb_chuan [] = {"´¨´©´«´¬´ª´­´®"};
const u8 PY_mb_chuang[] = {"´³´¯´°´²´´"};
const u8 PY_mb_chui  [] = {"´µ´¶´¹´·´¸"};
const u8 PY_mb_chun  [] = {"´º´»´¿´½´¾´¼´À"};
const u8 PY_mb_chuo  [] = {"´Á"};
const u8 PY_mb_ci    [] = {"´Ã´Ê´Ä´É´È´Ç´Å´Æ´Ë´Î´Ì´Í"};
const u8 PY_mb_cong  [] = {"´Ñ´Ó´Ò´Ð´Ï´Ô"};
const u8 PY_mb_cou   [] = {"´Õ"};
const u8 PY_mb_cu    [] = {"´Ö´Ù´×´Ø"};
const u8 PY_mb_cuan  [] = {"´Ú´Ü´Û"};
const u8 PY_mb_cui   [] = {"´Þ´ß´Ý´à´ã´á´â´ä"};
const u8 PY_mb_cun   [] = {"´å´æ´ç"};
const u8 PY_mb_cuo   [] = {"´ê´è´é´ì´ë´í"};
const u8 PY_mb_d     [] = {"´î´ôµ¤µ±"}; ///
const u8 PY_mb_da    [] = {"´î´ï´ð´ñ´ò´ó"};
const u8 PY_mb_dai   [] = {"´ô´õ´ö´ú´ø´ýµ¡´ù´û´ü´þ´÷"};
const u8 PY_mb_dan   [] = {"µ¤µ¥µ£µ¢µ¦µ¨µ§µ©µ«µ®µ¯µ¬µ­µ°µª"};
const u8 PY_mb_dang  [] = {"µ±µ²µ³µ´µµ"};
const u8 PY_mb_dao   [] = {"µ¶µ¼µºµ¹µ·µ»µ¸µ½µ¿µÁµÀµ¾"};
const u8 PY_mb_de    [] = {"µÃµÂµÄ"};
const u8 PY_mb_deng  [] = {"µÆµÇµÅµÈµËµÊµÉ"};
const u8 PY_mb_di    [] = {"µÍµÌµÎµÒµÏµÐµÓµÑµÕµ×µÖµØµÜµÛµÝµÚµÞµÙ"};
const u8 PY_mb_dian  [] = {"µàµáµßµäµãµâµçµèµéµêµæµëµíµìµîµå"};
const u8 PY_mb_diao  [] = {"µóµðµòµïµñµõµöµô"};
const u8 PY_mb_die   [] = {"µùµøµüµýµþµúµû"};
const u8 PY_mb_ding  [] = {"¶¡¶£¶¢¶¤¶¥¶¦¶©¶¨¶§"};
const u8 PY_mb_diu   [] = {"¶ª"};
const u8 PY_mb_dong  [] = {"¶«¶¬¶­¶®¶¯¶³¶±¶²¶°¶´"};
const u8 PY_mb_dou   [] = {"¶¼¶µ¶·¶¶¶¸¶¹¶º¶»"};
const u8 PY_mb_du    [] = {"¶½¶¾¶Á¶¿¶À¶Â¶Ä¶Ã¶Ê¶Å¶Ç¶È¶É¶Æ"};
const u8 PY_mb_duan  [] = {"¶Ë¶Ì¶Î¶Ï¶Ð¶Í"};
const u8 PY_mb_dui   [] = {"¶Ñ¶Ó¶Ô¶Ò"};
const u8 PY_mb_dun   [] = {"¶Ö¶Ø¶Õ¶×¶Ü¶Û¶Ù¶Ý"};
const u8 PY_mb_duo   [] = {"¶à¶ß¶á¶Þ¶ä¶â¶ã¶ç¶é¶æ¶è¶å"};
const u8 PY_mb_e     [] = {"¶ï¶í¶ð¶ë¶ì¶ê¶î¶ò¶ó¶ñ¶ö¶õ¶ô"};
const u8 PY_mb_en    [] = {"¶÷"};
const u8 PY_mb_er    [] = {"¶ù¶ø¶û¶ú¶ý¶ü¶þ·¡"};
const u8 PY_mb_f     [] = {"·¢·«·½·É·Ö"}; ///
const u8 PY_mb_fa    [] = {"·¢·¦·¥·£·§·¤·¨·©"};
const u8 PY_mb_fan   [] = {"·«·¬·­·ª·²·¯·°·³·®·±·´·µ·¸·º·¹·¶··"};
const u8 PY_mb_fang  [] = {"·½·»·¼·À·Á·¿·¾·Â·Ã·Ä·Å"};
const u8 PY_mb_fei   [] = {"·É·Ç·È·Æ·Ê·Ë·Ì·Í·Ï·Ð·Î·Ñ"};
const u8 PY_mb_fen   [] = {"·Ö·Ô·×·Ò·Õ·Ó·Ø·Ú·Ù·Û·Ý·Ü·Þ·ß·à"};
const u8 PY_mb_feng  [] = {"·á·ç·ã·â·è·å·é·æ·ä·ë·ê·ì·í·ï·î"};
const u8 PY_mb_fo    [] = {"·ð"};
const u8 PY_mb_fou   [] = {"·ñ"};
const u8 PY_mb_fu    [] = {"·ò·ô·õ·ó¸¥·ü·ö·÷·þ·ý·ú¸¡¸¢·û¸¤·ù¸£·ø¸§¸¦¸®¸«¸©¸ª¸¨¸­¸¯¸¸¸¼¸¶¸¾¸º¸½¸À¸·¸´¸°¸±¸µ¸»¸³¸¿¸¹¸²"};
const u8 PY_mb_g     [] = {"¸Â¸Ã¸É¸Ô¸Þ"}; ///
const u8 PY_mb_ga    [] = {"¸Â¸Á"};
const u8 PY_mb_gai   [] = {"¸Ã¸Ä¸Æ¸Ç¸È¸Å"};
const u8 PY_mb_gan   [] = {"¸É¸Ê¸Ë¸Î¸Ì¸Í¸Ñ¸Ï¸Ò¸Ð¸Ó"};
const u8 PY_mb_gang  [] = {"¸Ô¸Õ¸Ú¸Ù¸Ø¸×¸Ö¸Û¸Ü"};
const u8 PY_mb_gao   [] = {"¸Þ¸á¸ß¸à¸Ý¸â¸ã¸å¸ä¸æ"};
const u8 PY_mb_ge    [] = {"¸ê¸í¸ç¸ì¸ë¸î¸é¸è¸ó¸ï¸ñ¸ð¸ô¸ö¸÷¸õ¿©"};
const u8 PY_mb_gei   [] = {"¸ø"};
const u8 PY_mb_gen   [] = {"¸ù¸ú"};
const u8 PY_mb_geng  [] = {"¸ü¸ý¸û¸þ¹¡¹¢¹£"};
const u8 PY_mb_gong  [] = {"¹¤¹­¹«¹¦¹¥¹©¹¬¹§¹ª¹¨¹®¹¯¹°¹²¹±"};
const u8 PY_mb_gou   [] = {"¹´¹µ¹³¹·¹¶¹¹¹º¹¸¹»"};
const u8 PY_mb_gu    [] = {"¹À¹¾¹Ã¹Â¹Á¹½¹¼¹¿¹Å¹È¹É¹Ç¹Æ¹Ä¹Ì¹Ê¹Ë¹Í"};
const u8 PY_mb_gua   [] = {"¹Ï¹Î¹Ð¹Ñ¹Ò¹Ó"};
const u8 PY_mb_guai  [] = {"¹Ô¹Õ¹Ö"};
const u8 PY_mb_guan  [] = {"¹Ø¹Û¹Ù¹Ú¹×¹Ý¹Ü¹á¹ß¹à¹Þ"};
const u8 PY_mb_guang [] = {"¹â¹ã¹ä"};
const u8 PY_mb_gui   [] = {"¹é¹ç¹ê¹æ¹ë¹è¹å¹ì¹î¹ï¹í¹ô¹ñ¹ó¹ð¹ò"};
const u8 PY_mb_gun   [] = {"¹õ¹ö¹÷"};
const u8 PY_mb_guo   [] = {"¹ù¹ø¹ú¹û¹ü¹ý"};
const u8 PY_mb_h     [] = {"¸òº¢º¨º¼ºÁ"}; ///
const u8 PY_mb_ha    [] = {"¸ò¹þ"};
const u8 PY_mb_hai   [] = {"º¢º¡º£º¥º§º¦º¤"};
const u8 PY_mb_han   [] = {"º¨º©º¬ºªº¯º­º®º«º±º°ººº¹ºµº·º´º¸º¶º³º²"};
const u8 PY_mb_hang  [] = {"º¼º½ÐÐ"};
const u8 PY_mb_hao   [] = {"ºÁºÀº¿º¾ºÃºÂºÅºÆºÄ"};
const u8 PY_mb_he    [] = {"ºÇºÈºÌºÏºÎºÍºÓºÒºËºÉºÔºÐºÊºØºÖºÕº×"};
const u8 PY_mb_hei   [] = {"ºÚºÙ"};
const u8 PY_mb_hen   [] = {"ºÛºÜºÝºÞ"};
const u8 PY_mb_heng  [] = {"ºàºßºãºáºâ"};
const u8 PY_mb_hong  [] = {"ºäºåºæºëºìºêºéºçºè"};
const u8 PY_mb_hou   [] = {"ºîºíºïºðºóºñºò"};
const u8 PY_mb_hu    [] = {"ºõºôºö»¡ºüºúºøºþºùº÷ºýºû»¢»£»¥»§»¤»¦"};
const u8 PY_mb_hua   [] = {"»¨»ª»©»¬»«»¯»®»­»°"};
const u8 PY_mb_huai  [] = {"»³»²»´»±»µ"};
const u8 PY_mb_huan  [] = {"»¶»¹»·»¸»º»Ã»Â»½»»»Á»¼»À»¾»¿"};
const u8 PY_mb_huang [] = {"»Ä»Å»Ê»Ë»Æ»Ì»Í»È»Ç»É»Ð»Î»Ñ»Ï"};
const u8 PY_mb_hui   [] = {"»Ò»Ö»Ó»Ô»Õ»Ø»×»Ú»Ü»ã»á»ä»æ»å»â»ß»Þ»à»Ý»Ù»Û"};
const u8 PY_mb_hun   [] = {"»è»ç»é»ë»ê»ì"};
const u8 PY_mb_huo   [] = {"»í»î»ð»ï»ò»õ»ñ»ö»ó»ô"};
const u8 PY_mb_j     [] = {"¼¥¼Ó¼é½­½»"}; ///
const u8 PY_mb_ji    [] = {"¼¥»÷¼¢»ø»ú¼¡¼¦¼£¼§»ý»ù¼¨¼©»û»þ»ü¼¤¼°¼ª¼³¼¶¼´¼«¼±¼²¼¬¼¯¼µ¼­¼®¼¸¼º¼·¼¹¼Æ¼Ç¼¿¼Í¼Ë¼É¼¼¼Ê¼Á¼¾¼È¼Ã¼Ì¼Å¼Ä¼Â¼À¼»¼½½å"};
const u8 PY_mb_jia   [] = {"¼Ó¼Ð¼Ñ¼Ï¼Ò¼Î¼Ô¼Õ¼×¼Ö¼Ø¼Û¼Ý¼Ü¼Ù¼Þ¼ÚÐ®"};
const u8 PY_mb_jian  [] = {"¼é¼â¼á¼ß¼ä¼ç¼è¼æ¼à¼ã¼ê¼å¼ð¼ó¼í¼ë¼ñ¼õ¼ô¼ì¼ï¼ò¼î¼û¼þ½¨½¤½£¼ö¼ú½¡½§½¢½¥½¦¼ù¼ø¼ü¼ý"};
const u8 PY_mb_jiang [] = {"½­½ª½«½¬½©½®½²½±½°½¯½³½µ½´"};
const u8 PY_mb_jiao  [] = {"½»½¼½¿½½½¾½º½·½¹½¶½¸½Ç½Æ½Ê½È½Ã½Å½Â½Á½Ë½É½Ð½Î½Ï½Ì½Ñ½Í¾õ½À"};
const u8 PY_mb_jie   [] = {"½×½Ô½Ó½Õ½Ò½Ö½Ú½Ù½Ü½à½á½Ý½Þ½Ø½ß½ã½â½é½ä½æ½ì½ç½ê½ë½è"};
const u8 PY_mb_jin   [] = {"½í½ñ½ï½ð½ò½î½ó½ö½ô½÷½õ¾¡¾¢½ü½ø½ú½þ½ý½û½ù"};
const u8 PY_mb_jing  [] = {"¾©¾­¾¥¾£¾ª¾§¾¦¾¬¾¤¾«¾¨¾®¾±¾°¾¯¾»¾¶¾·¾º¾¹¾´¾¸¾³¾²¾µ"};
const u8 PY_mb_jiong [] = {"¾¼¾½"};
const u8 PY_mb_jiu   [] = {"¾À¾¿¾¾¾Å¾Ã¾Ä¾Á¾Â¾Æ¾É¾Ê¾Ì¾Î¾Ç¾È¾Í¾Ë"};
const u8 PY_mb_ju    [] = {"¾Ó¾Ð¾Ñ¾Ô¾Ò¾Ï¾Ö½Û¾Õ¾×¾Ú¾Ù¾Ø¾ä¾Þ¾Ü¾ß¾æ¾ã¾ç¾å¾Ý¾à¾â¾Û¾á"};
const u8 PY_mb_juan  [] = {"¾ê¾è¾é¾í¾ë¾î¾ì"};
const u8 PY_mb_jue   [] = {"¾ï¾ö¾÷¾ñ¾ø¾ó¾ò¾ô¾ð"};
const u8 PY_mb_jun   [] = {"¾ü¾ý¾ù¾û¾ú¿¡¿¤¾þ¿£¿¥¿¢"};
const u8 PY_mb_k     [] = {"¿§¿ª¼÷¿µ¿¼"}; ///
const u8 PY_mb_ka    [] = {"¿§¿¦¿¨"};
const u8 PY_mb_kai   [] = {"¿ª¿«¿­¿®¿¬"};
const u8 PY_mb_kan   [] = {"¼÷¿¯¿±¿°¿²¿³¿´"};
const u8 PY_mb_kang  [] = {"¿µ¿¶¿·¿¸¿º¿¹¿»"};
const u8 PY_mb_kao   [] = {"¿¼¿½¿¾¿¿"};
const u8 PY_mb_ke    [] = {"¿À¿Á¿Â¿Æ¿Ã¿Å¿Ä¿Ç¿È¿É¿Ê¿Ë¿Ì¿Í¿Î"};
const u8 PY_mb_ken   [] = {"¿Ï¿Ñ¿Ò¿Ð"};
const u8 PY_mb_keng  [] = {"¿Ô¿Ó"};
const u8 PY_mb_kong  [] = {"¿Õ¿×¿Ö¿Ø"};
const u8 PY_mb_kou   [] = {"¿Ù¿Ú¿Û¿Ü"};
const u8 PY_mb_ku    [] = {"¿Ý¿Þ¿ß¿à¿â¿ã¿á"};
const u8 PY_mb_kua   [] = {"¿ä¿å¿æ¿è¿ç"};
const u8 PY_mb_kuai  [] = {"¿é¿ì¿ë¿ê"};
const u8 PY_mb_kuan  [] = {"¿í¿î"};
const u8 PY_mb_kuang [] = {"¿ï¿ð¿ñ¿ö¿õ¿ó¿ò¿ô"};
const u8 PY_mb_kui   [] = {"¿÷¿ù¿ø¿ú¿ü¿û¿ý¿þÀ¢À£À¡"};
const u8 PY_mb_kun   [] = {"À¤À¥À¦À§"};
const u8 PY_mb_kuo   [] = {"À©À¨À«Àª"};
const u8 PY_mb_l     [] = {"À¬À´À¼ÀÉÀÌ"}; ///
const u8 PY_mb_la    [] = {"À¬À­À²À®À°À¯À±"};
const u8 PY_mb_lai   [] = {"À´À³Àµ"};
const u8 PY_mb_lan   [] = {"À¼À¹À¸À·À»À¶À¾À½ÀºÀÀÀ¿ÀÂÀÁÀÃÀÄ"};
const u8 PY_mb_lang  [] = {"ÀÉÀÇÀÈÀÅÀÆÀÊÀË"};
const u8 PY_mb_lao   [] = {"ÀÌÀÍÀÎÀÏÀÐÀÑÀÔÀÓÀÒ"};
const u8 PY_mb_le    [] = {"ÀÖÀÕÁË"};
const u8 PY_mb_lei   [] = {"À×ÀØÀÝÀÚÀÙÀÜÀßÀáÀàÀÛÀÞ"};
const u8 PY_mb_leng  [] = {"ÀâÀãÀä"};
const u8 PY_mb_li    [] = {"ÀåÀæÀêÀëÀòÀçÀìÁ§ÀèÀéÀñÀîÀïÁ¨ÀíÀðÁ¦ÀúÀ÷Á¢ÀôÀöÀûÀøÁ¤ÀýÁ¥ÀþÀóÀõÀùÁ£ÀüÁ¡"};
const u8 PY_mb_lian  [] = {"Á¬Á±Á¯Á°Á«ÁªÁ®Á­Á²Á³Á·Á¶ÁµÁ´"};
const u8 PY_mb_liang [] = {"Á©Á¼Á¹ÁºÁ¸Á»Á½ÁÁÁÂÁ¾ÁÀÁ¿"};
const u8 PY_mb_liao  [] = {"ÁÊÁÉÁÆÁÄÁÅÁÈÁÎÁÃÁÇÁÍÁÏÁÌ"};
const u8 PY_mb_lie   [] = {"ÁÐÁÓÁÒÁÔÁÑ"};
const u8 PY_mb_lin   [] = {"ÁÚÁÖÁÙÁÜÁÕÁØÁ×ÁÛÁÝÁßÁÞÁà"};
const u8 PY_mb_ling  [] = {"ÁæÁéÁëÁáÁèÁåÁêÁçÁâÁãÁäÁìÁîÁí"};
const u8 PY_mb_liu   [] = {"ÁïÁõÁ÷ÁôÁðÁòÁóÁñÁöÁøÁù"};
const u8 PY_mb_long  [] = {"ÁúÁüÁýÁûÂ¡ÁþÂ¤Â¢Â£"};
const u8 PY_mb_lou   [] = {"Â¦Â¥Â§Â¨ÂªÂ©"};
const u8 PY_mb_lu    [] = {"Â¶Â¬Â®Â«Â¯Â­Â±Â²Â°Â³Â½Â¼Â¸Â¹Â»ÂµÂ·Â¾ÂºÂ´"};
const u8 PY_mb_luan  [] = {"ÂÏÂÍÂÎÂÐÂÑÂÒ"};
const u8 PY_mb_lue   [] = {"ÂÓÂÔ"};
const u8 PY_mb_lun   [] = {"ÂÕÂØÂ×ÂÙÂÚÂÖÂÛ"};
const u8 PY_mb_luo   [] = {"ÂÞÂÜÂßÂàÂáÂâÂÝÂãÂåÂçÂæÂä"};
const u8 PY_mb_lv    [] = {"ÂËÂ¿ÂÀÂÂÂÃÂÁÂÅÂÆÂÄÂÉÂÇÂÊÂÌÂÈ"};
const u8 PY_mb_m     [] = {"ÂèÂñÂùÃ¦Ã¨"}; ///
const u8 PY_mb_ma    [] = {"ÂèÂéÂíÂêÂëÂìÂîÂðÂï"};
const u8 PY_mb_mai   [] = {"ÂñÂòÂõÂóÂôÂö"};
const u8 PY_mb_man   [] = {"ÂùÂøÂ÷ÂúÂüÃ¡ÂýÂþÂû"};
const u8 PY_mb_mang  [] = {"Ã¦Ã¢Ã¤Ã£Ã§Ã¥"};
const u8 PY_mb_mao   [] = {"Ã¨Ã«Ã¬Ã©ÃªÃ®Ã­Ã¯Ã°Ã³Ã±Ã²"};
const u8 PY_mb_me    [] = {"Ã´"};
const u8 PY_mb_mei   [] = {"Ã»Ã¶ÃµÃ¼Ã·Ã½ÃºÃ¸Ã¹Ã¿ÃÀÃ¾ÃÃÃÁÃÄÃÂ"};
const u8 PY_mb_men   [] = {"ÃÅÃÆÃÇ"};
const u8 PY_mb_meng  [] = {"ÃÈÃËÃÊÃÍÃÉÃÌÃÏÃÎ"};
const u8 PY_mb_mi    [] = {"ÃÖÃÔÃÕÃÑÃÓÃÒÃ×ÃÐÃÚÃÙÃØÃÜÃÝÃÛ"};
const u8 PY_mb_mian  [] = {"ÃßÃàÃÞÃâÃãÃäÃáÃåÃæ"};
const u8 PY_mb_miao  [] = {"ÃçÃèÃéÃëÃìÃêÃîÃí"};
const u8 PY_mb_mie   [] = {"ÃðÃï"};
const u8 PY_mb_min   [] = {"ÃñÃóÃòÃöÃõÃô"};
const u8 PY_mb_ming  [] = {"ÃûÃ÷ÃùÃúÃøÃü"};
const u8 PY_mb_miu   [] = {"Ãý"};
const u8 PY_mb_mo    [] = {"ºÑÃþÄ¡Ä£Ä¤Ä¦Ä¥Ä¢Ä§Ä¨Ä©Ä­Ä°ÄªÄ¯Ä®Ä«Ä¬"};
const u8 PY_mb_mou   [] = {"Ä²Ä±Ä³"};
const u8 PY_mb_mu    [] = {"Ä¸Ä¶ÄµÄ·Ä´Ä¾Ä¿ÄÁÄ¼Ä¹Ä»ÄÀÄ½ÄºÄÂ"};
const u8 PY_mb_n     [] = {"ÄÃÄËÄÐ"}; ///
const u8 PY_mb_na    [] = {"ÄÃÄÄÄÇÄÉÄÈÄÆÄÅ"};
const u8 PY_mb_nai   [] = {"ÄËÄÌÄÊÄÎÄÍ"};
const u8 PY_mb_nan   [] = {"ÄÐÄÏÄÑéª"};
const u8 PY_mb_nang  [] = {"ÄÒ"};
const u8 PY_mb_nao   [] = {"ÄÓÄÕÄÔÄÖÄ×"};
const u8 PY_mb_ne    [] = {"ÄØ"};
const u8 PY_mb_nei   [] = {"ÄÚÄÙ"};
const u8 PY_mb_nen   [] = {"ÄÛ"};
const u8 PY_mb_neng  [] = {"ÄÜ"};
const u8 PY_mb_ni    [] = {"ÄÝÄáÄàÄßÄÞÄãÄâÄæÄäÄçÄå"};
const u8 PY_mb_nian  [] = {"ÄéÄêÄíÄìÄëÄîÄè"};
const u8 PY_mb_niang [] = {"ÄïÄð"};
const u8 PY_mb_niao  [] = {"ÄñÄò"};
const u8 PY_mb_nie   [] = {"ÄóÄùÄôÄöÄ÷ÄøÄõ"};
const u8 PY_mb_nin   [] = {"Äú"};
const u8 PY_mb_ning  [] = {"ÄþÅ¡ÄüÄûÄýÅ¢"};
const u8 PY_mb_niu   [] = {"Å£Å¤Å¦Å¥"};
const u8 PY_mb_nong  [] = {"Å©Å¨Å§Åª"};
const u8 PY_mb_nu    [] = {"Å«Å¬Å­"};
const u8 PY_mb_nuan  [] = {"Å¯"};
const u8 PY_mb_nue   [] = {"Å±Å°"};
const u8 PY_mb_nuo   [] = {"Å²ÅµÅ³Å´"};
const u8 PY_mb_nv    [] = {"Å®"};
const u8 PY_mb_o     [] = {"Å¶"};
const u8 PY_mb_ou    [] = {"Å·Å¹Å¸Å»Å¼ÅºÅ½"};
const u8 PY_mb_p     [] = {"Å¿ÅÄÅËÅÒ"}; ///
const u8 PY_mb_pa    [] = {"Å¿Å¾ÅÀ°ÒÅÃÅÁÅÂ"};
const u8 PY_mb_pai   [] = {"ÅÄÅÇÅÅÅÆÅÉÅÈ"};
const u8 PY_mb_pan   [] = {"ÅËÅÊÅÌÅÍÅÐÅÑÅÎÅÏ"};
const u8 PY_mb_pang  [] = {"ÅÒÅÓÅÔÅÕÅÖ"};
const u8 PY_mb_pao   [] = {"Å×ÅÙÅØÅÚÅÛÅÜÅÝ"};
const u8 PY_mb_pei   [] = {"ÅÞÅßÅãÅàÅâÅáÅæÅåÅä"};
const u8 PY_mb_pen   [] = {"ÅçÅè"};
const u8 PY_mb_peng  [] = {"ÅêÅéÅëÅóÅíÅïÅðÅîÅôÅìÅñÅòÅõÅö"};
const u8 PY_mb_pi    [] = {"±ÙÅúÅ÷ÅûÅøÅüÅùÆ¤ÅþÆ£Æ¡ÅýÆ¢Æ¥Æ¦Æ¨Æ§Æ©"};
const u8 PY_mb_pian  [] = {"Æ¬Æ«ÆªÆ­"};
const u8 PY_mb_piao  [] = {"Æ¯Æ®Æ°Æ±"};
const u8 PY_mb_pie   [] = {"Æ²Æ³"};
const u8 PY_mb_pin   [] = {"Æ´Æ¶ÆµÆ·Æ¸"};
const u8 PY_mb_ping  [] = {"Æ½ÆÀÆ¾ÆÁÆ¿ÆºÆ»Æ¼Æ¹"};
const u8 PY_mb_po    [] = {"ÆÂÆÃÆÄÆÅÆÈÆÆÆÉÆÇ"};
const u8 PY_mb_pou   [] = {"ÆÊ"};
const u8 PY_mb_pu    [] = {"¸¬ÆÍÆËÆÌÆÎÆÐÆÏÆÑÆÓÆÔÆÒÆÖÆÕÆ×ÆØ"};
const u8 PY_mb_q     [] = {"ÆßÆþÇ§Çº"}; ///
const u8 PY_mb_qi    [] = {"ÆßÆãÆÞÆâÆàÆÜÆÝÆÚÆÛÆáÆîÆëÆäÆæÆçÆíÆêÆéÆèÆïÆåÆìÆòÆóÆñÆôÆðÆøÆýÆùÆúÆûÆüÆõÆöÆ÷"};
const u8 PY_mb_qia   [] = {"ÆþÇ¡Ç¢"};
const u8 PY_mb_qian  [] = {"Ç§ÇªÇ¤Ç¨Ç¥Ç£Ç¦Ç«Ç©Ç°Ç®Ç¯Ç¬Ç±Ç­Ç³Ç²Ç´Ç·ÇµÇ¶Ç¸"};
const u8 PY_mb_qiang [] = {"ÇºÇ¼Ç¹Ç»Ç¿Ç½Ç¾ÇÀ"};
const u8 PY_mb_qiao  [] = {"ÇÄÇÃÇÂÇÁÇÇÇÈÇÅÇÆÇÉÇÎÇÍÇÏÇÌÇËÇÊ"};
const u8 PY_mb_qie   [] = {"ÇÐÇÑÇÒÇÓÇÔ"};
const u8 PY_mb_qin   [] = {"Ç×ÇÖÇÕÇÛÇØÇÙÇÝÇÚÇÜÇÞÇß"};
const u8 PY_mb_qing  [] = {"ÇàÇâÇáÇãÇäÇåÇéÇçÇèÇæÇêÇëÇì"};
const u8 PY_mb_qiong [] = {"ÇîÇí"};
const u8 PY_mb_qiu   [] = {"ÇðÇñÇïÇôÇóÇöÇõÇò"};
const u8 PY_mb_qu    [] = {"ÇøÇúÇýÇüÇùÇûÇ÷ÇþÈ¡È¢È£È¥È¤"};
const u8 PY_mb_quan  [] = {"È¦È«È¨ÈªÈ­È¬È©È§È®È°È¯"};
const u8 PY_mb_que   [] = {"È²È±È³È´È¸È·ÈµÈ¶"};
const u8 PY_mb_qun   [] = {"È¹Èº"};
const u8 PY_mb_r     [] = {"È»È¿ÈÄ"}; ///
const u8 PY_mb_ran   [] = {"È»È¼È½È¾"};
const u8 PY_mb_rang  [] = {"È¿ÈÂÈÀÈÁÈÃ"};
const u8 PY_mb_rao   [] = {"ÈÄÈÅÈÆ"};
const u8 PY_mb_re    [] = {"ÈÇÈÈ"};
const u8 PY_mb_ren   [] = {"ÈËÈÊÈÉÈÌÈÐÈÏÈÎÈÒÈÑÈÍ"};
const u8 PY_mb_reng  [] = {"ÈÓÈÔ"};
const u8 PY_mb_ri    [] = {"ÈÕ"};
const u8 PY_mb_rong  [] = {"ÈÖÈÞÈ×ÈÙÈÝÈÜÈØÈÛÈÚÈß"};
const u8 PY_mb_rou   [] = {"ÈáÈàÈâ"};
const u8 PY_mb_ru    [] = {"ÈçÈãÈåÈæÈäÈêÈéÈèÈëÈì"};
const u8 PY_mb_ruan  [] = {"ÈîÈí"};
const u8 PY_mb_rui   [] = {"ÈïÈñÈð"};
const u8 PY_mb_run   [] = {"ÈòÈó"};
const u8 PY_mb_ruo   [] = {"ÈôÈõ"};
const u8 PY_mb_s     [] = {"ÈöÈûÈý"}; ///
const u8 PY_mb_sa    [] = {"ÈöÈ÷Èø"};
const u8 PY_mb_sai   [] = {"ÈûÈùÈúÈü"};
const u8 PY_mb_san   [] = {"ÈýÉ¡É¢Èþ"};
const u8 PY_mb_sang  [] = {"É£É¤É¥"};
const u8 PY_mb_sao   [] = {"É¦É§É¨É©"};
const u8 PY_mb_se    [] = {"É«É¬Éª"};
const u8 PY_mb_sen   [] = {"É­"};
const u8 PY_mb_seng  [] = {"É®"};
const u8 PY_mb_sha   [] = {"É±É³É´É°É¯ÉµÉ¶É·ÏÃ"};
const u8 PY_mb_shai  [] = {"É¸É¹"};
const u8 PY_mb_shan  [] = {"É½É¾É¼ÉÀÉºÉ¿ÉÁÉÂÉÇÉ»ÉÈÉÆÉÉÉÃÉÅÉÄÕ¤"};
const u8 PY_mb_shang [] = {"ÉËÉÌÉÊÉÑÉÎÉÍÉÏÉÐ"};
const u8 PY_mb_shao  [] = {"ÉÓÉÒÉÕÉÔÉ×ÉÖÉØÉÙÉÛÉÜÉÚ"};
const u8 PY_mb_she   [] = {"ÉÝÉÞÉàÉßÉáÉèÉçÉäÉæÉâÉåÉã"};
const u8 PY_mb_shen  [] = {"ÉêÉìÉíÉëÉðÉïÉéÉîÉñÉòÉóÉôÉöÉõÉøÉ÷Ê²"};
const u8 PY_mb_sheng [] = {"ÉýÉúÉùÉüÊ¤ÉûÉþÊ¡Ê¥Ê¢Ê£"};
const u8 PY_mb_shi   [] = {"ÊÇÊ®Ê¯Ê±Ê¿Ê¶ÊÂÊ³Ê¾Ê½Ê¹Ê¼ÊÒÊÆÊµÊÓÊÔÊ·ÊÊÊ©ÊÐÊ»ÊªËÆ³×Ê¬Ê§ÊÅÊÍÊ¦Ê­Ê«Ê¨Ê°Ê´Ê¸ÊºÊÏÊÀÊËÊÌÊÎÊÑÊÃÊÁÊÈÊÄÊÉ"};
const u8 PY_mb_shou  [] = {"ÊÕÊÖÊØÊ×ÊÙÊÜÊÞÊÛÊÚÊÝ"};
const u8 PY_mb_shu   [] = {"ÊéÊãÊåÊàÊâÊáÊçÊèÊæÊäÊßÊëÊêÊìÊîÊòÊðÊóÊñÊíÊïÊõÊùÊøÊöÊ÷ÊúË¡ÊüÊýÊûÊþÊô"};
const u8 PY_mb_shua  [] = {"Ë¢Ë£"};
const u8 PY_mb_shuai [] = {"Ë¥Ë¤Ë¦Ë§"};
const u8 PY_mb_shuan [] = {"Ë©Ë¨"};
const u8 PY_mb_shuang[] = {"Ë«ËªË¬"};
const u8 PY_mb_shui  [] = {"Ë­Ë®Ë°Ë¯"};
const u8 PY_mb_shun  [] = {"Ë±Ë³Ë´Ë²"};
const u8 PY_mb_shuo  [] = {"ËµË¸Ë·Ë¶"};
const u8 PY_mb_si    [] = {"ËÄË¿Ë¾Ë½Ë¼Ë¹ËÀËºËÂË»ËÈËÅËÇËÃËÁ"};
const u8 PY_mb_song  [] = {"ËÍËÎËÉËÊËËËÏËÐËÌ"};
const u8 PY_mb_sou   [] = {"ËÔËÑËÒËÓ"};
const u8 PY_mb_su    [] = {"ËÕËÖË×ËßËàËØËÙËÚËÜËÝËÛ"};
const u8 PY_mb_suan  [] = {"ËáËâËã"};
const u8 PY_mb_sui   [] = {"ËäËêËçËåËæËèËîËìËéËíËë"};
const u8 PY_mb_sun   [] = {"ËïËðËñ"};
const u8 PY_mb_suo   [] = {"ËôËóËòËõËùË÷ËöËø"};
const u8 PY_mb_t     [] = {"ËýÌ¥Ì®ÌÀ"}; ///
const u8 PY_mb_ta    [] = {"ËýËûËüËúËþÌ¡Ì¢Ì¤Ì£"};
const u8 PY_mb_tai   [] = {"Ì¥Ì¨Ì§Ì¦Ì«Ì­Ì¬Ì©Ìª"};
const u8 PY_mb_tan   [] = {"Ì®Ì°Ì¯Ì²Ì±Ì³Ì¸ÌµÌ·Ì¶Ì´Ì¹Ì»ÌºÌ¾Ì¿Ì½Ì¼"};
const u8 PY_mb_tang  [] = {"ÌÀÌÆÌÃÌÄÌÁÌÂÌÅÌÇÌÈÌÊÌÉÌÌÌË"};
const u8 PY_mb_tao   [] = {"ÌÎÌÐÌÍÌÏÌÓÌÒÌÕÌÔÌÑÌÖÌ×"};
const u8 PY_mb_te    [] = {"ÌØ"};
const u8 PY_mb_teng  [] = {"ÌÛÌÚÌÜÌÙ"};
const u8 PY_mb_ti    [] = {"ÌÞÌÝÌàÌßÌäÌáÌâÌãÌåÌëÌêÌéÌèÌæÌç"};
const u8 PY_mb_tian  [] = {"ÌìÌíÌïÌñÌðÌîÌóÌò"};
const u8 PY_mb_tiao  [] = {"µ÷ÌôÌõÌöÌ÷Ìø"};
const u8 PY_mb_tie   [] = {"ÌùÌúÌû"};
const u8 PY_mb_ting  [] = {"Í£ÌüÍ¡ÌýÌþÍ¢Í¤Í¥Í¦Í§"};
const u8 PY_mb_tong  [] = {"Í¨Í¬Í®Í©Í­Í¯ÍªÍ«Í³Í±Í°Í²Í´"};
const u8 PY_mb_tou   [] = {"ÍµÍ·Í¶Í¸"};
const u8 PY_mb_tu    [] = {"Í¹ÍºÍ»Í¼Í½Í¿Í¾ÍÀÍÁÍÂÍÃ"};
const u8 PY_mb_tuan  [] = {"ÍÄÍÅ"};
const u8 PY_mb_tui   [] = {"ÍÆÍÇÍÈÍËÍÉÍÊ"};
const u8 PY_mb_tun   [] = {"¶ÚÍÌÍÍÍÎ"};
const u8 PY_mb_tuo   [] = {"ÍÐÍÏÍÑÍÔÍÓÍÕÍÒÍ×ÍÖÍØÍÙ"};
const u8 PY_mb_w     [] = {"ÍÛÍâÍòÍõÎ£"}; ///
const u8 PY_mb_wa    [] = {"ÍÛÍÞÍÚÍÝÍÜÍßÍà"};
const u8 PY_mb_wai   [] = {"ÍâÍá"};
const u8 PY_mb_wan   [] = {"ÍòÍêÍæÍíÍäÍëÍåÍãÍèÍçÍéÍðÍìÍñÍïÍîÍó"};
const u8 PY_mb_wang  [] = {"ÍõÍøÍùÍüÍúÍûÍôÍöÍ÷Íý"};
const u8 PY_mb_wei   [] = {"Î£ÍþÎ¢Î¡ÎªÎ¤Î§Î¥Î¦Î¨Î©Î¬Î«Î°Î±Î²Î³Î­Î¯Î®ÎÀÎ´Î»Î¶Î·Î¸Î¾Î½Î¹Î¼ÎµÎ¿Îº"};
const u8 PY_mb_wen   [] = {"ÎÂÎÁÎÄÎÆÎÅÎÃÎÇÎÉÎÈÎÊ"};
const u8 PY_mb_weng  [] = {"ÎÌÎËÎÍ"};
const u8 PY_mb_wo    [] = {"ÎÒÎÖÎÔÎÑÎÕÎÐÎÏÎÓÎÎ"};
const u8 PY_mb_wu    [] = {"ÎåÎâÎóÎÞÎÝÎäÎçÎÚÎÛÎØÎ×ÎÜÎÙÎãÎáÎßÎàÎéÎëÎêÎæÎèÎðÎñÎìÎïÎòÎîÎí"};
const u8 PY_mb_x     [] = {"Ï¦ÏºÏ³Ïç"}; ///
const u8 PY_mb_xi    [] = {"Ï¦Ï«Î÷ÎüÏ£ÎôÎöÎùÏ¢ÎþÏ¤Ï§Ï©ÎøÎúÏ¬Ï¡ÏªÎýÏ¨ÎõÎûÏ¥Ï°Ï¯Ï®Ï±Ï­Ï´Ï²Ï·ÏµÏ¸Ï¶"};
const u8 PY_mb_xia   [] = {"ÏºÏ¹Ï»ÏÀÏ¿ÏÁÏ¾Ï½Ï¼ÏÂÏÅÏÄ"};
const u8 PY_mb_xian  [] = {"Ï³ÏÉÏÈÏËÏÆÏÇÏÊÏÐÏÒÏÍÏÌÏÑÏÏÏÎÏÓÏÔÏÕÏØÏÖÏßÏÞÏÜÏÝÏÚÏÛÏ×ÏÙ"};
const u8 PY_mb_xiang [] = {"ÏçÏàÏãÏáÏæÏäÏåÏâÏêÏéÏèÏíÏìÏëÏòÏïÏîÏóÏñÏð"};
const u8 PY_mb_xiao  [] = {"ÏüÏûÏôÏõÏúÏöÏùÏýÐ¡ÏþÐ¢Ð¤ÏøÐ§Ð£Ð¦Ð¥"};
const u8 PY_mb_xie   [] = {"Ð©Ð¨ÐªÐ«Ð­Ð°Ð²Ð±Ð³Ð¯Ð¬Ð´Ð¹ÐºÐ¶Ð¼ÐµÐ»Ð¸Ð·"};
const u8 PY_mb_xin   [] = {"ÐÄÐÃÐ¾ÐÁÐÀÐ¿ÐÂÐ½ÐÅÐÆ"};
const u8 PY_mb_xing  [] = {"ÐËÐÇÐÊÐÉÐÈÐÌÐÏÐÎÐÍÐÑÐÓÐÕÐÒÐÔ"};
const u8 PY_mb_xiong [] = {"Ð×ÐÖÐÙÐÚÐØÐÛÐÜ"};
const u8 PY_mb_xiu   [] = {"ËÞÐÝÐÞÐßÐàÐãÐåÐäÐâÐá"};
const u8 PY_mb_xu    [] = {"ÐçÐëÐéÐêÐèÐæÐìÐíÐñÐòÐðÐôÐ÷ÐøÐïÐöÐõÐîÓõ"};
const u8 PY_mb_xuan  [] = {"ÐùÐûÐúÐþÐüÐýÑ¡Ñ¢Ñ¤Ñ£"};
const u8 PY_mb_xue   [] = {"Ñ§Ñ©ÑªÏ÷Ñ¥Ñ¦Ñ¨"};
const u8 PY_mb_xun   [] = {"Ñ°Ñ®Ñ¸Ñ¯Ñ¶Ñ«Ñ¬Ñ²Ñ±Ñ­ÑµÑ´Ñ·Ñ³"};
const u8 PY_mb_y     [] = {"Ñ½ÑÊÑô½ÄÒ¬"}; ///
const u8 PY_mb_ya    [] = {"Ñ½Ñ¹ÑÀÑÇÑÅÑ¾ÑºÑ»Ñ¼Ñ¿ÑÁÑÂÑÄÑÃÑÆÑÈ"};
const u8 PY_mb_yan   [] = {"ÑÊÑÌÑÍÑÉÑËÑÓÑÏÑÔÑÒÑØÑ×ÑÐÑÎÑÖÑÑÑÕÑÙÑÜÑÚÑÛÑÝÑáÑåÑâÑäÑçÑÞÑéÑèÑßÑæÑãÑà"};
const u8 PY_mb_yang  [] = {"ÑôÑïÑîÑùÑøÑòÑëÑêÑíÑìÑðÑñÑóÑöÑõÑ÷Ñú"};
const u8 PY_mb_yao   [] = {"½ÄÑýÑüÑûÒ¢Ò¦Ò¤Ò¥Ò¡Ò£ÑþÒ§Ò¨Ò©ÒªÒ«Ô¿"};
const u8 PY_mb_ye    [] = {"Ò¬Ò­Ò¯Ò®Ò²Ò±Ò°ÒµÒ¶Ò·Ò³Ò¹Ò´ÒºÒ¸"};
const u8 PY_mb_yi    [] = {"Ò»ÒÁÒÂÒ½ÒÀÒ¿Ò¼Ò¾ÒÇÒÄÒÊÒËÒÌÒÈÒÆÒÅÒÃÒÉÒÍÒÒÒÑÒÔÒÓÒÏÒÐÒÎÒåÒÚÒäÒÕÒéÒàÒÙÒìÒÛÒÖÒëÒØÒ×ÒïÒèÒßÒæÒêÒîÒÝÒâÒçÒÞÒáÒãÒíÒÜ"};
const u8 PY_mb_yin   [] = {"ÒòÒõÒöÒðÒñÒôÒóÒ÷ÒúÒùÒøÒüÒýÒûÒþÓ¡"};
const u8 PY_mb_ying  [] = {"Ó¦Ó¢Ó¤Ó§Ó£Ó¥Ó­Ó¯Ó«Ó¨Ó©ÓªÓ¬Ó®Ó±Ó°Ó³Ó²"};
const u8 PY_mb_yo    [] = {"Ó´"};
const u8 PY_mb_yong  [] = {"Ó¶ÓµÓ¸Ó¹ÓºÓ·ÓÀÓ½Ó¾ÓÂÓ¿ÓÁÓ¼Ó»ÓÃ"};
const u8 PY_mb_you   [] = {"ÓÅÓÇÓÄÓÆÓÈÓÉÓÌÓÊÓÍÓËÓÎÓÑÓÐÓÏÓÖÓÒÓ×ÓÓÓÕÓÔ"};
const u8 PY_mb_yu    [] = {"ÓØÓÙÓåÓÚÓèÓàÓÛÓãÓáÓéÓæÓçÓäÓâÓÞÓÜÓÝÓßÓëÓîÓìÓðÓêÓíÓïÓñÔ¦ÓóÓýÓôÓüÓøÔ¡Ô¤ÓòÓûÓ÷Ô¢ÓùÔ£ÓöÓúÓþÔ¥"};
const u8 PY_mb_yuan  [] = {"Ô©Ô§Ô¨ÔªÔ±Ô°Ô«Ô­Ô²Ô¬Ô®ÔµÔ´Ô³Ô¯Ô¶Ô·Ô¹ÔºÔ¸"};
const u8 PY_mb_yue   [] = {"Ô»Ô¼ÔÂÔÀÔÃÔÄÔ¾ÔÁÔ½"};
const u8 PY_mb_yun   [] = {"ÔÆÔÈÔÇÔÅÔÊÔÉÔÐÔËÔÎÔÍÔÏÔÌ"};
const u8 PY_mb_z     [] = {"ÔÑÔÖÔÛ"}; ///
const u8 PY_mb_za    [] = {"ÔÑÔÓÔÒÕ¦"};
const u8 PY_mb_zai   [] = {"ÔÖÔÕÔÔÔ×ÔØÔÙÔÚ×Ð"};
const u8 PY_mb_zan   [] = {"ÔÛÔÜÔÝÔÞ"};
const u8 PY_mb_zang  [] = {"ÔßÔàÔá"};
const u8 PY_mb_zao   [] = {"ÔâÔãÔäÔçÔæÔéÔèÔåÔîÔíÔìÔëÔïÔê"};
const u8 PY_mb_ze    [] = {"ÔòÔñÔóÔð"};
const u8 PY_mb_zei   [] = {"Ôô"};
const u8 PY_mb_zen   [] = {"Ôõ"};
const u8 PY_mb_zeng  [] = {"ÔöÔ÷Ôù"};
const u8 PY_mb_zha   [] = {"ÔûÔüÔúÔýÔþÕ¢Õ¡Õ£Õ§Õ©Õ¨Õ¥×õ"};
const u8 PY_mb_zhai  [] = {"Õ«ÕªÕ¬µÔÕ­Õ®Õ¯"};
const u8 PY_mb_zhan  [] = {"Õ´Õ±Õ³Õ²Õ°Õ¶Õ¹ÕµÕ¸Õ·Õ¼Õ½Õ»Õ¾ÕÀÕ¿Õº"};
const u8 PY_mb_zhang [] = {"³¤ÕÅÕÂÕÃÕÄÕÁÕÇÕÆÕÉÕÌÕÊÕÈÕÍÕËÕÏÕÎ"};
const u8 PY_mb_zhao  [] = {"ÕÐÕÑÕÒÕÓÕÙÕ×ÕÔÕÕÕÖÕØ×¦"};
const u8 PY_mb_zhe   [] = {"ÕÚÕÛÕÜÕÝÕÞÕßÕàÕâÕãÕá×Å"};
const u8 PY_mb_zhen  [] = {"ÕêÕëÕìÕäÕæÕèÕåÕçÕéÕïÕíÕîÕóÕñÕòÕðÖ¡"};
const u8 PY_mb_zheng [] = {"ÕùÕ÷ÕúÕõÕøÕöÕôÕüÕûÕýÖ¤Ö£ÕþÖ¢"};
const u8 PY_mb_zhi   [] = {"Ö®Ö§Ö­Ö¥Ö¨Ö¦ÖªÖ¯Ö«Ö¬Ö©Ö´Ö¶Ö±ÖµÖ°Ö²Ö³Ö¹Ö»Ö¼Ö·Ö½Ö¸ÖºÖÁÖ¾ÖÆÖÄÖÎÖËÖÊÖÅÖ¿ÖÈÖÂÖÀÖÌÖÏÖÇÖÍÖÉÖÃ"};
const u8 PY_mb_zhong [] = {"ÖÐÖÒÖØÖÕÖÑÖÓÖÔÖ×ÖÖÖÙÖÚ"};
const u8 PY_mb_zhou  [] = {"ÖÝÖÛÖßÖÜÖÞÖàÖáÖâÖãÖäÖæÖçÖåÖè"};
const u8 PY_mb_zhu   [] = {"Ö÷×¡×¢×£ÖìÖíÖúÖùÖñÖðÖêÖéÖöÖøÖîÖëÖòÖïÖôÖóÖõÖü×¤ÖûÖþÖý"};
const u8 PY_mb_zhua  [] = {"×¥"};
const u8 PY_mb_zhuai [] = {"×§"};
const u8 PY_mb_zhuan [] = {"×¨×©×ª×«×­"};
const u8 PY_mb_zhuang[] = {"×±×¯×®×°×³×´´±×²"};
const u8 PY_mb_zhui  [] = {"×·×¹×µ×¶×º×¸"};
const u8 PY_mb_zhun  [] = {"×¼×»"};
const u8 PY_mb_zhuo  [] = {"×¿×¾×½×À×Æ×Â×Ç×Ã×Ä×Á"};
const u8 PY_mb_zi    [] = {"×Î×È×É×Ë×Ê×Í×Ì×Ñ×Ó×Ï×Ò×Ö×Ô×Õ"};
const u8 PY_mb_zong  [] = {"×Ú×Û×Ø×Ù×××Ü×Ý"};
const u8 PY_mb_zou   [] = {"×ß×à×Þ×á"};
const u8 PY_mb_zu    [] = {"×â×ã×ä×å×ç×è×é×æ"};
const u8 PY_mb_zuan  [] = {"×¬×ê×ë"};
const u8 PY_mb_zui   [] = {"×ì×î×ï×í"};
const u8 PY_mb_zun   [] = {"×ð×ñ"};
const u8 PY_mb_zuo   [] = {"×ò×ó×ô×÷×ø×ù×ö"};





const T9PY_IDX Nult9py;
const T9PY_IDX t9PY_index[] =
{
	{"0", "0", PY_mb_space},
	{"*", "symbol", PY_mb_fuhao},	//ding add
	//{"2","A",PY_mb_a},
	{"2", "a", PY_mb_a},
	//{"2","B",PY_mb_space},
	{"2", "b", PY_mb_b},
	//{"2","C",PY_mb_space},
	{"2", "c", PY_mb_c},
	//{"3","D",PY_mb_space},
	{"3", "d", PY_mb_d},
	//{"3","E",PY_mb_e},
	{"3", "e", PY_mb_e},
	//{"3","F",PY_mb_space},
	{"3", "f", PY_mb_f},
	//{"4","G",PY_mb_space},
	{"4", "g", PY_mb_g},
	//{"4","H",PY_mb_space},
	{"4", "h", PY_mb_h},
	//{"4","I",PY_mb_space},
	//{"4","i",PY_mb_space},
	//{"5","J",PY_mb_space},
	{"5", "j", PY_mb_j},
	//{"5","K",PY_mb_space},
	{"5", "k", PY_mb_k},
	//{"5","L",PY_mb_space},
	{"5", "l", PY_mb_l},
	//{"6","M",PY_mb_space},
	{"6", "m", PY_mb_m},
	//{"6","N",PY_mb_space},
	{"6", "n", PY_mb_n},
	//{"6","O",PY_mb_o},
	{"6", "o", PY_mb_o},
	//{"7","P",PY_mb_space},
	{"7", "p", PY_mb_p},
	//{"7","Q",PY_mb_space},
	{"7", "q", PY_mb_q},
	//{"7","R",PY_mb_space},
	{"7", "r", PY_mb_r},
	//{"7","S",PY_mb_space},
	{"7", "s", PY_mb_s},
	//{"8","T",PY_mb_space},
	{"8", "t", PY_mb_t},
	//{"8","U",PY_mb_space},
	//{"8","u",PY_mb_space},
	//{"8","V",PY_mb_space},
	//{"8","v",PY_mb_space},
	//{"9","W",PY_mb_space},
	{"9", "w", PY_mb_w},
	//{"9","X",PY_mb_space},
	{"9", "x", PY_mb_x},
	//{"9","Y",PY_mb_space},
	{"9", "y", PY_mb_y},
	//{"9","Z",PY_mb_space},
	{"9", "z", PY_mb_z},
	{"24", "ai", PY_mb_ai},
	{"26", "an", PY_mb_an},
	{"26", "ao", PY_mb_ao},
	{"22", "ba", PY_mb_ba},
	{"24", "bi", PY_mb_bi},
	{"26", "bo", PY_mb_bo},
	{"28", "bu", PY_mb_bu},
	{"22", "ca", PY_mb_ca},
	{"23", "ce", PY_mb_ce},
	{"24", "ci", PY_mb_ci},
	{"28", "cu", PY_mb_cu},
	{"32", "da", PY_mb_da},
	{"33", "de", PY_mb_de},
	{"34", "di", PY_mb_di},
	{"38", "du", PY_mb_du},
	{"36", "en", PY_mb_en},
	{"37", "er", PY_mb_er},
	{"32", "fa", PY_mb_fa},
	{"36", "fo", PY_mb_fo},
	{"38", "fu", PY_mb_fu},
	{"42", "ha", PY_mb_ha},
	{"42", "ga", PY_mb_ga},
	{"43", "ge", PY_mb_ge},
	{"43", "he", PY_mb_he},
	{"48", "gu", PY_mb_gu},
	{"48", "hu", PY_mb_hu},
	{"54", "ji", PY_mb_ji},
	{"58", "ju", PY_mb_ju},
	{"52", "ka", PY_mb_ka},
	{"53", "ke", PY_mb_ke},
	{"58", "ku", PY_mb_ku},
	{"52", "la", PY_mb_la},
	{"53", "le", PY_mb_le},
	{"54", "li", PY_mb_li},
	{"58", "lu", PY_mb_lu},
	{"58", "lv", PY_mb_lv},
	{"62", "ma", PY_mb_ma},
	{"63", "me", PY_mb_me},
	{"64", "mi", PY_mb_mi},
	{"66", "mo", PY_mb_mo},
	{"68", "mu", PY_mb_mu},
	{"62", "na", PY_mb_na},
	{"63", "ne", PY_mb_ne},
	{"64", "ni", PY_mb_ni},
	{"68", "nu", PY_mb_nu},
	{"68", "nv", PY_mb_nv},
	{"68", "ou", PY_mb_ou},
	{"72", "pa", PY_mb_pa},
	{"74", "pi", PY_mb_pi},
	{"76", "po", PY_mb_po},
	{"78", "pu", PY_mb_pu},
	{"74", "qi", PY_mb_qi},
	{"78", "qu", PY_mb_qu},
	{"73", "re", PY_mb_re},
	{"74", "ri", PY_mb_ri},
	{"78", "ru", PY_mb_ru},
	{"72", "sa", PY_mb_sa},
	{"73", "se", PY_mb_se},
	{"74", "si", PY_mb_si},
	{"78", "su", PY_mb_su},
	{"82", "ta", PY_mb_ta},
	{"83", "te", PY_mb_te},
	{"84", "ti", PY_mb_ti},
	{"88", "tu", PY_mb_tu},
	{"92", "wa", PY_mb_wa},
	{"96", "wo", PY_mb_wo},
	{"98", "wu", PY_mb_wu},
	{"94", "xi", PY_mb_xi},
	{"98", "xu", PY_mb_xu},
	{"92", "ya", PY_mb_ya},
	{"93", "ye", PY_mb_ye},
	{"94", "yi", PY_mb_yi},
	{"96", "yo", PY_mb_yo},
	{"98", "yu", PY_mb_yu},
	{"92", "za", PY_mb_za},
	{"93", "ze", PY_mb_ze},
	{"94", "zi", PY_mb_zi},
	{"98", "zu", PY_mb_zu},
	{"264", "ang", PY_mb_ang},
	{"224", "bai", PY_mb_bai},
	{"226", "ban", PY_mb_ban},
	{"226", "bao", PY_mb_bao},
	{"234", "bei", PY_mb_bei},
	{"236", "ben", PY_mb_ben},
	{"243", "bie", PY_mb_bie},
	{"246", "bin", PY_mb_bin},
	{"224", "cai", PY_mb_cai},
	{"226", "can", PY_mb_can},
	{"226", "cao", PY_mb_cao},
	{"242", "cha", PY_mb_cha},
	{"243", "che", PY_mb_che},
	{"244", "chi", PY_mb_chi},
	{"248", "chu", PY_mb_chu},
	{"268", "cou", PY_mb_cou},
	{"284", "cui", PY_mb_cui},
	{"286", "cun", PY_mb_cun},
	{"286", "cuo", PY_mb_cuo},
	{"324", "dai", PY_mb_dai},
	{"326", "dan", PY_mb_dan},
	{"326", "dao", PY_mb_dao},
	{"343", "die", PY_mb_die},
	{"348", "diu", PY_mb_diu},
	{"368", "dou", PY_mb_dou},
	{"384", "dui", PY_mb_dui},
	{"386", "dun", PY_mb_dun},
	{"386", "duo", PY_mb_duo},
	{"326", "fan", PY_mb_fan},
	{"334", "fei", PY_mb_fei},
	{"336", "fen", PY_mb_fen},
	{"368", "fou", PY_mb_fou},
	{"424", "gai", PY_mb_gai},
	{"426", "gan", PY_mb_gan},
	{"426", "gao", PY_mb_gao},
	{"434", "gei", PY_mb_gei},
	{"436", "gen", PY_mb_gan},
	{"468", "gou", PY_mb_gou},
	{"482", "gua", PY_mb_gua},
	{"484", "gui", PY_mb_gui},
	{"486", "gun", PY_mb_gun},
	{"486", "guo", PY_mb_guo},
	{"424", "hai", PY_mb_hai},
	{"426", "han", PY_mb_han},
	{"426", "hao", PY_mb_hao},
	{"434", "hei", PY_mb_hei},
	{"436", "hen", PY_mb_hen},
	{"468", "hou", PY_mb_hou},
	{"482", "hua", PY_mb_hua},
	{"484", "hui", PY_mb_hui},
	{"486", "hun", PY_mb_hun},
	{"486", "huo", PY_mb_huo},
	{"542", "jia", PY_mb_jia},
	{"543", "jie", PY_mb_jie},
	{"546", "jin", PY_mb_jin},
	{"548", "jiu", PY_mb_jiu},
	{"583", "jue", PY_mb_jue},
	{"586", "jun", PY_mb_jun},
	{"524", "kai", PY_mb_kai},
	{"526", "kan", PY_mb_kan},
	{"526", "kao", PY_mb_kao},
	{"536", "ken", PY_mb_ken},
	{"568", "kou", PY_mb_kou},
	{"582", "kua", PY_mb_kua},
	{"584", "kui", PY_mb_kui},
	{"586", "kun", PY_mb_kun},
	{"586", "kuo", PY_mb_kuo},
	{"524", "lai", PY_mb_lai},
	{"526", "lan", PY_mb_lan},
	{"526", "lao", PY_mb_lao},
	{"534", "lei", PY_mb_lei},
	{"543", "lie", PY_mb_lie},
	{"546", "lin", PY_mb_lin},
	{"548", "liu", PY_mb_liu},
	{"568", "lou", PY_mb_lou},
	{"583", "lue", PY_mb_lue},
	{"586", "lun", PY_mb_lun},
	{"586", "luo", PY_mb_luo},
	{"624", "mai", PY_mb_mai},
	{"626", "man", PY_mb_man},
	{"626", "mao", PY_mb_mao},
	{"634", "mei", PY_mb_mei},
	{"636", "men", PY_mb_men},
	{"643", "mie", PY_mb_mie},
	{"646", "min", PY_mb_min},
	{"648", "miu", PY_mb_miu},
	{"668", "mou", PY_mb_mou},
	{"624", "nai", PY_mb_nai},
	{"626", "nan", PY_mb_nan},
	{"626", "nao", PY_mb_nao},
	{"634", "nei", PY_mb_nei},
	{"636", "nen", PY_mb_nen},
	{"643", "nie", PY_mb_nie},
	{"646", "nin", PY_mb_nin},
	{"648", "niu", PY_mb_niu},
	{"683", "nue", PY_mb_nue},
	{"686", "nuo", PY_mb_nuo},
	{"724", "pai", PY_mb_pai},
	{"726", "pan", PY_mb_pan},
	{"726", "pao", PY_mb_pao},
	{"734", "pei", PY_mb_pei},
	{"736", "pen", PY_mb_pen},
	{"743", "pie", PY_mb_pie},
	{"746", "pin", PY_mb_pin},
	{"768", "pou", PY_mb_pou},
	{"742", "qia", PY_mb_qia},
	{"743", "qie", PY_mb_qie},
	{"746", "qin", PY_mb_qin},
	{"748", "qiu", PY_mb_qiu},
	{"783", "que", PY_mb_que},
	{"786", "qun", PY_mb_qun},
	{"726", "ran", PY_mb_ran},
	{"726", "rao", PY_mb_rao},
	{"736", "ren", PY_mb_ren},
	{"768", "rou", PY_mb_rou},
	{"784", "rui", PY_mb_rui},
	{"786", "run", PY_mb_run},
	{"786", "ruo", PY_mb_ruo},
	{"724", "sai", PY_mb_sai},
	{"726", "sao", PY_mb_sao},
	{"726", "san", PY_mb_san},
	{"736", "sen", PY_mb_sen},
	{"742", "sha", PY_mb_sha},
	{"743", "she", PY_mb_she},
	{"744", "shi", PY_mb_shi},
	{"748", "shu", PY_mb_shu},
	{"768", "sou", PY_mb_sou},
	{"784", "sui", PY_mb_sui},
	{"786", "sun", PY_mb_sun},
	{"786", "suo", PY_mb_suo},
	{"824", "tai", PY_mb_tai},
	{"826", "tan", PY_mb_tan},
	{"826", "tao", PY_mb_tao},
	{"843", "tie", PY_mb_tie},
	{"868", "tou", PY_mb_tou},
	{"884", "tui", PY_mb_tui},
	{"886", "tun", PY_mb_tun},
	{"886", "tuo", PY_mb_tuo},
	{"924", "wai", PY_mb_wai},
	{"926", "wan", PY_mb_wan},
	{"934", "wei", PY_mb_wei},
	{"936", "wen", PY_mb_wen},
	{"942", "xia", PY_mb_xia},
	{"943", "xie", PY_mb_xie},
	{"946", "xin", PY_mb_xin},
	{"948", "xiu", PY_mb_xiu},
	{"983", "xue", PY_mb_xue},
	{"986", "xun", PY_mb_xun},
	{"926", "yan", PY_mb_yan},
	{"926", "yao", PY_mb_yao},
	{"946", "yin", PY_mb_yin},
	{"968", "you", PY_mb_you},
	{"983", "yue", PY_mb_yue},
	{"986", "yun", PY_mb_yun},
	{"924", "zai", PY_mb_zai},
	{"926", "zan", PY_mb_zan},
	{"926", "zao", PY_mb_zao},
	{"934", "zei", PY_mb_zei},
	{"936", "zen", PY_mb_zen},
	{"942", "zha", PY_mb_zha},
	{"943", "zhe", PY_mb_zhe},
	{"944", "zhi", PY_mb_zhi},
	{"948", "zhu", PY_mb_zhu},
	{"968", "zou", PY_mb_zou},
	{"984", "zui", PY_mb_zui},
	{"986", "zun", PY_mb_zun},
	{"986", "zuo", PY_mb_zuo},
	{"2264", "bang", PY_mb_bang},
	{"2364", "beng", PY_mb_beng},
	{"2426", "bian", PY_mb_bian},
	{"2426", "biao", PY_mb_biao},
	{"2464", "bing", PY_mb_bing},
	{"2264", "cang", PY_mb_cang},
	{"2364", "ceng", PY_mb_ceng},
	{"2424", "chai", PY_mb_chai},
	{"2426", "chan", PY_mb_chan},
	{"2426", "chao", PY_mb_chao},
	{"2436", "chen", PY_mb_chen},
	{"2468", "chou", PY_mb_chou},
	{"2484", "chuai", PY_mb_chuai},
	{"2484", "chui", PY_mb_chui},
	{"2484", "chun", PY_mb_chun},
	{"2486", "chuo", PY_mb_chuo},
	{"2664", "cong", PY_mb_cong},
	{"2826", "cuan", PY_mb_cuan},
	{"3264", "dang", PY_mb_dang},
	{"3364", "deng", PY_mb_deng},
	{"3426", "dian", PY_mb_dian},
	{"3426", "diao", PY_mb_diao},
	{"3464", "ding", PY_mb_ding},
	{"3664", "dong", PY_mb_dong},
	{"3826", "duan", PY_mb_duan},
	{"3264", "fang", PY_mb_fang},
	{"3364", "feng", PY_mb_feng},
	{"4264", "gang", PY_mb_gang},
	{"4364", "geng", PY_mb_geng},
	{"4664", "gong", PY_mb_gong},
	{"4824", "guai", PY_mb_guai},
	{"4826", "guan", PY_mb_guan},
	{"4264", "hang", PY_mb_hang},
	{"4364", "heng", PY_mb_heng},
	{"4664", "hong", PY_mb_hong},
	{"4823", "huai", PY_mb_huai},
	{"4826", "huan", PY_mb_huan},
	{"5426", "jian", PY_mb_jian},
	{"5426", "jiao", PY_mb_jiao},
	{"5464", "jing", PY_mb_jing},
	{"5826", "juan", PY_mb_juan},
	{"5264", "kang", PY_mb_kang},
	{"5364", "keng", PY_mb_keng},
	{"5664", "kong", PY_mb_kong},
	{"5824", "kuai", PY_mb_kuai},
	{"5826", "kuan", PY_mb_kuan},
	{"5264", "lang", PY_mb_lang},
	{"5366", "leng", PY_mb_leng},
	{"5426", "lian", PY_mb_lian},
	{"5426", "liao", PY_mb_liao},
	{"5464", "ling", PY_mb_ling},
	{"5664", "long", PY_mb_long},
	{"5826", "luan", PY_mb_luan},
	{"6264", "mang", PY_mb_mang},
	{"6364", "meng", PY_mb_meng},
	{"6426", "mian", PY_mb_mian},
	{"6426", "miao", PY_mb_miao},
	{"6464", "ming", PY_mb_ming},
	{"6264", "nang", PY_mb_nang},
	{"6364", "neng", PY_mb_neng},
	{"6426", "nian", PY_mb_nian},
	{"6426", "niao", PY_mb_niao},
	{"6464", "ning", PY_mb_ning},
	{"6664", "nong", PY_mb_nong},
	{"6826", "nuan", PY_mb_nuan},
	{"7264", "pang", PY_mb_pang},
	{"7364", "peng", PY_mb_peng},
	{"7426", "pian", PY_mb_pian},
	{"7426", "piao", PY_mb_piao},
	{"7464", "ping", PY_mb_ping},
	{"7426", "qian", PY_mb_qian},
	{"7426", "qiao", PY_mb_qiao},
	{"7464", "qing", PY_mb_qing},
	{"7826", "quan", PY_mb_quan},
	{"7264", "rang", PY_mb_rang},
	{"7364", "reng", PY_mb_reng},
	{"7664", "rong", PY_mb_rong},
	{"7826", "ruan", PY_mb_ruan},
	{"7264", "sang", PY_mb_sang},
	{"7364", "seng", PY_mb_seng},
	{"7424", "shai", PY_mb_shai},
	{"7426", "shan", PY_mb_shan},
	{"7426", "shao", PY_mb_shao},
	{"7436", "shen", PY_mb_shen},
	{"7468", "shou", PY_mb_shou},
	{"7482", "shua", PY_mb_shua},
	{"7484", "shui", PY_mb_shui},
	{"7486", "shun", PY_mb_shun},
	{"7486", "shuo", PY_mb_shuo},
	{"7664", "song", PY_mb_song},
	{"7826", "suan", PY_mb_suan},
	{"8264", "tang", PY_mb_tang},
	{"8364", "teng", PY_mb_teng},
	{"8426", "tian", PY_mb_tian},
	{"8426", "tiao", PY_mb_tiao},
	{"8464", "ting", PY_mb_ting},
	{"8664", "tong", PY_mb_tong},
	{"8826", "tuan", PY_mb_tuan},
	{"9264", "wang", PY_mb_wang},
	{"9364", "weng", PY_mb_weng},
	{"9426", "xian", PY_mb_xiao},
	{"9426", "xiao", PY_mb_xiao},
	{"9464", "xing", PY_mb_xing},
	{"9826", "xuan", PY_mb_xuan},
	{"9264", "yang", PY_mb_yang},
	{"9464", "ying", PY_mb_ying},
	{"9664", "yong", PY_mb_yong},
	{"9826", "yuan", PY_mb_yuan},
	{"9264", "zang", PY_mb_zang},
	{"9364", "zeng", PY_mb_zeng},
	{"9424", "zhai", PY_mb_zhai},
	{"9426", "zhan", PY_mb_zhan},
	{"9426", "zhao", PY_mb_zhao},
	{"9436", "zhen", PY_mb_zhen},
	{"9468", "zhou", PY_mb_zhou},
	{"9482", "zhua", PY_mb_zhua},
	{"9484", "zhui", PY_mb_zhui},
	{"9486", "zhun", PY_mb_zhun},
	{"9486", "zhuo", PY_mb_zhuo},
	{"9664", "zong", PY_mb_zong},
	{"9826", "zuan", PY_mb_zuan},
	{"24264", "chang", PY_mb_chang},
	{"24364", "cheng", PY_mb_cheng},
	{"24664", "chong", PY_mb_chong},
	{"24826", "chuan", PY_mb_chuan},
	{"48264", "guang", PY_mb_guang},
	{"48264", "huang", PY_mb_huang},
	{"54264", "jiang", PY_mb_jiang},
	{"54664", "jiong", PY_mb_jiong},
	{"58264", "kuang", PY_mb_kuang},
	{"54264", "liang", PY_mb_liang},
	{"64264", "niang", PY_mb_niang},
	{"74264", "qiang", PY_mb_qiang},
	{"74664", "qiong", PY_mb_qiong},
	{"74264", "shang", PY_mb_shang},
	{"74364", "sheng", PY_mb_sheng},
	{"74824", "shuai", PY_mb_shuai},
	{"74826", "shuan", PY_mb_shuan},
	{"94264", "xiang", PY_mb_xiang},
	{"94664", "xiong", PY_mb_xiong},
	{"94264", "zhang", PY_mb_zhang},
	{"94364", "zheng", PY_mb_zheng},
	{"94664", "zhong", PY_mb_zhong},
	{"94824", "zhuai", PY_mb_zhuai},
	{"94826", "zhuan", PY_mb_zhuan},
	{"248264", "chuang", PY_mb_chuang},
	{"748264", "shuang", PY_mb_shuang},
	{"948264", "zhuang", PY_mb_zhuang},
	{"", "", PY_mb_space},
	{NULL, NULL, NULL}	//ding  add
};


typedef struct
{
	const u8 *bh23456;
	const u8 *MB;
} T9BH_IDX;


typedef struct
{
	const u8 *HanZi;
	const u8 *MB;
} THanZiThink;


/*¸ù¾ÝÖ¸Õë½øÐÐ²åÈë±à¼­*/
/*×îºóÁ½¸ö²ÎÊýÁô¸øabc  ÓëABC  ÊäÈë·¨ÓÃ*/
static void my_AccordPointInsertEdit (const u8 *TheFirstAddrOfBuff, const u8 *pMove, char StartLine, char EndLine, u8 *cursorx, u8 *cursory)
{
	u8 totalline;	//±à¼­ÇøÓÐ¼¸ÐÐ
	int bytenum;	//´Óbuff  Í·µ½¹â±ê´¦ÓÐ¶àÉÙ¸ö×Ö·û
	u8 byteline;	//´Óbuff  Í·µ½¹â±ê´¦ÐèÒª¶àÉÙ¸öÐÐÏÔÊ¾
	//int bytetotal;	//buff  ×Ü¹²ÓÐ¶àÉÙ¸ö×Ö·û
	u8 i, j;
	const u8 *display;
	char disp[23];

	*cursorx = 0;
	*cursory = 0;

	totalline = EndLine - StartLine + 1;
	bytenum = pMove - TheFirstAddrOfBuff;
	//bytetotal=strlen(TheFirstAddrOfBuff);

	byteline = bytenum / ALineNum;

	if (bytenum % ALineNum) byteline++;

	display = TheFirstAddrOfBuff;	//ÕÒ³öÐèÒªÏÔÊ¾µÄµØÖ·

	if (byteline > totalline)
	{
		for (i = 0; i < byteline - totalline; i++)
		{
			for (j = 0; j < ALineNum; j++, display++)
			{
				if (*display > 0x80)
				{
					display++;
					j++;
				}
			}
		}
	}

#ifdef DEBUG_PRINTF
	printf ("display=%s\n", display);
#endif

	if (*display != 0)  		//Èç¹ûÓÐ×Ö·û
	{
		for (i = 0; i < totalline; i++)  			//ÏÔÊ¾pMove  Ö®Ç°Ö®ºóÄÜÔÚ" ÏÔÊ¾·¶Î§Ö®ÄÚ"  µÄ×Ö·û
		{
			for (j = 0; j < ALineNum && *display != 0;)
			{
				if (*display > 0x80)
				{
					disp[j] = *display;
					j++;
					display++;

					if (display == pMove)
					{
						*cursory = i;
						*cursorx = j;
					}
				}

				disp[j] = *display;
				j++;
				display++;

				if (display == pMove)
				{
					*cursory = i;
					*cursorx = j;
				}
			}

			for (; j < ALineNum + 2; j++) disp[j] = ' ';			//´Ë´¦ALineNum+1  ÎªÁËÈ¥³ýÉ¾³ý×Ö·ûÊÇ£¬×îºó¿ÉÄÜÏÔÊ¾°ë¸öºº×ÖµÄÎÊÌâ

			disp[j] = 0;
#ifdef DEBUG_PRINTF
			printf ("disline=%s\n", disp);
#endif
			text_out_to_row_Language2 ( (unsigned char) StartLine + i, (unsigned char*) disp, (unsigned char*) disp);
		}
	}
	else 		//Èç¹ûÃ»×Ö·û£¬ÔòÓÐ¿ÉÄÜÊÇÉ¾³ýÍêÁË£¬ÔòÆÁÄ»»áÊ£Ò»¸ö×Ö·û
	{
		text_out_to_row_Language2 ( (unsigned char) StartLine, "   ", "   ");
	}

#ifdef DEBUG_PRINTF
	printf ("bytenum=%d\n", bytenum);
#endif

	*cursorx *= 6;
	*cursory += StartLine;
	*cursory *= 13;
	flashcursor (*cursorx, *cursory);
}


static void DisplayContaiPYHaizi (char PYrowcnt, const T9PY_IDX **pT9PY, char *PYbuff, char addid, char pagecnt, char *HaiziRightShiftmax)
{
	int x, i;
	char tb[25];
	const char *pMB;
	char pybuf[8];
	const u8 NumData[10][5] =  		//¿í¡Á¸ß£¨ÏñËØ£©: 4¡Á5 ×ÝÏòÈ¡Ä££¬×Ö½Úµ¹Ðò
	{
		{0x1F,0x11,0x1F,0x00},	//_0
		{0x00,0x1F,0x00,0x00},	//_1
		{0x1D,0x15,0x17,0x00},	//_2
		{0x15,0x15,0x1F,0x00},	//-3	
		{0x07,0x04,0x1F,0x00},	//-4
		{0x17,0x15,0x1D,0x00},	//_5
		{0x1F,0x15,0x1D,0x00},	//_6
		{0x01,0x01,0x1F,0x00},	//_7
		{0x1F,0x15,0x1F,0x00},	//_8
		{0x17,0x15,0x1F,0x00}	//_9
	};

	pMB = PYbuff;

	for (i = 1; i < PYrowcnt;)
	{
		if (*pMB == ' ') i++;

		pMB++;
	}

	memset (pybuf, 0, sizeof (pybuf) );

	for (i = 0; i < 8 && *pMB > ' '; i++, pMB++)
	{
		pybuf[i] = *pMB;
	}

#ifdef DEBUG_PRINTF
	printf ("pybuf=%s\n", pybuf);
#endif

	for (i = 0; t9PY_index[i].T9 != NULL; i++)
	{
		if (strcmp ( (char*) t9PY_index[i].PY, pybuf) == 0)
		{
			break;
		}
	}

	if (t9PY_index[i].T9 != NULL)
	{
#ifdef DEBUG_PRINTF
		printf ("t9PY_index[i].MB!=NULL\n");
#endif
		*pT9PY = &t9PY_index[i];
		pMB = (const char*) t9PY_index[i].MB;
		pMB += pagecnt * 14;
#ifdef DEBUG_PRINTF
		printf ("pMB=%s\n", pMB);
#endif

		if (strlen (pMB) / 2 / 7 > 0) *HaiziRightShiftmax = 6;
		else *HaiziRightShiftmax = strlen (pMB) / 2 % 7 - 1;

		memset (tb, 0, sizeof (tb) );

		for (i = 0; *pMB != 0;)
		{
			tb[i] = ' ';
			i++;
			tb[i] = *pMB;
			i++;
			pMB++;
			tb[i] = *pMB;
			i++;
			pMB++;

			if (i > 20) break;
		}

		clear_area_to_row (MaxRowOneScr - 1, MaxRowOneScr - 1);
		text_out_to_row_Language2 (MaxRowOneScr - 1, (unsigned char*) tb, (unsigned char*) tb);
		if (addid)  			//¸øÃ¿¸öºº×Ö¼ÓÉÏ1234567  ±ê¼Ç
		{
			for (x = 0, i = 0; i < 20; i++)
			{
				if (tb[i] == 0) break;

				if (tb[i] == ' ')
				{
					draw_pic (NumData[x + 1], x * 18 + 2, 53, 4, 5);
					x++;
				}
			}
		}

		reverse_area_to_row (6, MaxRowOneScr - 1, 12, 14);
		draw_line_to_row (MaxRowOneScr - 2);
	}
}

static void DisplayBH (const char *key09)
{
	const char *pMB;
	char x;
	const u8 heng[6] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x00};
	const u8 shu[6] = {0x00, 0x00, 0xFC, 0x00, 0x00, 0x00};
	const u8 pie[6] = {0x04, 0x08, 0x10, 0x20, 0xC0, 0x00};
	const u8 na[6] = {0xC0, 0x20, 0x10, 0x08, 0x04, 0x00};
	const u8 zhe[6] = {0x80, 0x80, 0x80, 0x84, 0xFC, 0x00};

	pMB = key09;	//ÔÚµ¹ÊýµÚ¶þÐÐÏÔÊ¾±Ê»­

	for (x = 0; *pMB != '\0'; x += 6, pMB++)
	{
		if (*pMB == '2')		draw_pic (heng, x, 44, 6, 6);
		else if (*pMB == '3')	draw_pic (shu, x, 44, 6, 6);
		else if (*pMB == '4')	draw_pic (pie, x, 44, 6, 6);
		else if (*pMB == '5')	draw_pic (na, x, 44, 6, 6);
		else if (*pMB == '6')	draw_pic (zhe, x, 44, 6, 6);
	}
}
static void DisplayContaiBHHaizi (const T9BH_IDX **pT9BH, char *key09, char addid, char pagecnt, char *HaiziRightShiftmax)
{
	int x, i;
	char tb[25];
	const char *pMB;
	const u8 NumData[10][5] =  		//¿í¡Á¸ß£¨ÏñËØ£©: 4¡Á5
	{
		{0xF8, 0x88, 0xF8, 0x00},	//_0
		{0x00, 0xF8, 0x00, 0x00},	//_1
		{0xB8, 0xA8, 0xE8, 0x00},	//_2
		{0xA8, 0xA8, 0xF8, 0x00},	//_3
		{0xE0, 0x20, 0xF8, 0x00},	//_4
		{0xE8, 0xA8, 0xB8, 0x00},	//_5
		{0xF8, 0xA8, 0xB8, 0x00},	//_6
		{0x80, 0x80, 0xF8, 0x00},	//_7
		{0xF8, 0xA8, 0xF8, 0x00},	//_8
		{0xE8, 0xA8, 0xF8, 0x00}	//_9
	};

	DisplayBH (key09);	//´Ë´¦ÓÃkey09  ¶ø²»ÓÃpT9BH  ÊÇÒòÎªÏñ"662446"  ÕâÖÖ×éºÏ£¬±Ê»­ÏÔÊ¾»áÓÐÎÊÌâ¡£

	pMB = (const char*) (*pT9BH)->MB;
	pMB += pagecnt * 14;
#ifdef DEBUG_PRINTF
	printf ("pMB=%s\n", pMB);
#endif

	if (strlen (pMB) / 2 / 7 > 0) *HaiziRightShiftmax = 6;
	else *HaiziRightShiftmax = strlen (pMB) / 2 % 7 - 1;

	memset (tb, 0, sizeof (tb) );

	for (i = 0; *pMB != 0;)
	{
		tb[i] = ' ';
		i++;
		tb[i] = *pMB;
		i++;
		pMB++;
		tb[i] = *pMB;
		i++;
		pMB++;

		if (i > 20) break;
	}

	clear_area_to_row (MaxRowOneScr - 1, MaxRowOneScr - 1);
	text_out_to_row_Language2 (MaxRowOneScr - 1, (unsigned char*) tb, NULL);

	if (addid)  		//¸øÃ¿¸öºº×Ö¼ÓÉÏ1234567  ±ê¼Ç
	{
		for (x = 0, i = 0; i < 20; i++)
		{
			if (tb[i] == 0) break;

			if (tb[i] == ' ')
			{
				draw_pic (NumData[x + 1], x * 18 + 2, 53, 4, 5);
				x++;
			}
		}
	}

	reverse_area_to_row (6, MaxRowOneScr - 1, 12, 14);
	draw_line_to_row (MaxRowOneScr - 2);
}


static void HighLightPY (const char *buff, char newpy)
{
	const char *buffStart, *buffPY;
	char diplaybuff[22];
	u8 i, j, zifunum;
	char temp, xpos = 0;
	char sjxflag = 0;	//×óÓÒÒÆµÄÈý½ÇÐÎ±êÖ¾
	const char leftsjx[] = {0x10, 0x38, 0x7C, 0xFE, 0x00};
	const char rightsjx[] = {0xFE, 0x7C, 0x38, 0x10, 0x00};

	buffPY = buff;	//ÏÈÇó³öÓÐ¶àÉÙ¸öÆ´Òô×éºÏÀýÈç:  "piao pian qian qiao shan shao "  ÄÇÃ´zifunum=4£»
	zifunum = 0;

	while (*buffPY != ' ')
	{
		zifunum++;
		buffPY++;
	}

	buffPY = buff;	//ÕÒ³öÒªÏÔÊ¾Æ´ÒôµÄÎ»ÖÃ¡£

	for (i = 1; i < newpy;)
	{
		if (*buffPY == ' ') i++;

		buffPY++;
	}

	buffStart = buff;	//ÕÒ³öÒª¿½±´µÄ×Ö·û´®µÄÆðÊ¼µØÖ·
	temp = newpy;

	if (buffPY - buffStart + zifunum > 19)
	{
		sjxflag |= 0x01;		//ÒªÏÔÊ¾×ó±ßµÄÈý½ÇÐÎ¡£

		for (i = 1; i < newpy && *buffStart != 0;)
		{
			buffStart++;

			if (*buffStart == ' ')
			{
				i++;
				temp--;

				if (buffPY - buffStart + zifunum <= 20)
				{
					buffStart++;
					break;
				}
			}
		}
	}

	for (i = 0; i < 19 && *buffStart != 0; i++)  		//¿½±´×Ö·û´®µ½diplaybuff
	{
		diplaybuff[i] = *buffStart;
		buffStart++;
	}

	for (; i < 20; i++) diplaybuff[i] = ' ';

	diplaybuff[i] = 0;

	while (*buffStart != 0)  		//ÅÐ¶ÏÊÇ·ñÒªÏÔÊ¾ÓÒ±ßµÄÈý½ÇÐÎ¡£
	{
		if (*buffStart >= 'a' && *buffStart <= 'z')
		{
			sjxflag |= 0x02;
			break;
		}

		buffStart++;
	}

	xpos = 0;

	for (j = 0, i = 1; i < temp;)  		//Çó³ö¸ßÁÁµÄx  ×ø±êÎ»ÖÃ
	{
		if (diplaybuff[j] == ' ')
		{
			i++;
		}

		xpos++;
		j++;
	}

	text_out_to_row_x_Language2 (MaxRowOneScr - 2, 6, (unsigned char*) diplaybuff, (unsigned char*)diplaybuff);
	reverse_area_to_row (xpos * 6 + 6, MaxRowOneScr - 2, zifunum * 6 + 2, 12);

	if (sjxflag & 0x01)
	{
		draw_pic ( (unsigned char*) leftsjx, 0, 43, 5, 7);
	}
	else
	{
		clear_area (0, 43, 5, 7);
	}

	if (sjxflag & 0x02)
	{
		draw_pic ( (unsigned char*) rightsjx, 124, 43, 5, 7);
	}
	else
	{
		clear_area (124, 43, 5, 7);
	}
}


static void CalculateCursorOffset (char srf)
{
	signed int x = cursorX, y = cursorY;

	if (srf == SRF_BiHua || srf == SRF_PY) x += 12;
	else x += 6;

	if (x >= 6 * ALineNum)
	{
		x = 0;
		y *= 13;

		if (y >= (EditAreaTotalRow + 1) * 13) y = EditAreaTotalRow * 13;
	}

#ifdef DEBUG_PRINTF
	printf ("cursorX=%d,cursorY=%d\n", x, y);
#endif

	flashcursor (x, y);
}


static u8 PinYinReadCode (char PyCode[3]) //Æ´ÒôÊäÈë·¨¶Áºº×Ö
{
	char key09[10], keyflag, PYnum, strcpyflag, confirmflag = 0, rowcnt, pagecnt = 0, youcipinyinzuohe;
	char PYrowcnt, PYbuff[100];
	int i;
	const T9PY_IDX *pT9PY = NULL;
	char HaiziRightShiftmax;	//ºº×ÖÓÒÒÆ×î´óÖµ
	u8 keycnt = 0;
	u8 keyval;
	u8 flag = 1;

	memset (key09, 0, sizeof (key09) );

	while (1)
	{
		keyval = GetKeyValue();

		if (keyval == 0) continue;

		if (flag == 1)
		{
			flag = 0;
			KeyEventPost_nobeep();
		}
		else
		{
			KeyEventPost();
		}

		keyflag = 0;

		switch (keyval)
		{
			case KEY0:
				keyflag = 1;
				key09[keycnt++] = '0';
				break;

			case KEY1:
				keyflag = 1;
				key09[keycnt++] = '1';
				break;

			case KEY2:
				keyflag = 1;
				key09[keycnt++] = '2';
				break;

			case KEY3:
				keyflag = 1;
				key09[keycnt++] = '3';
				break;

			case KEY4:
				keyflag = 1;
				key09[keycnt++] = '4';
				break;

			case KEY5:
				keyflag = 1;
				key09[keycnt++] = '5';
				break;

			case KEY6:
				keyflag = 1;
				key09[keycnt++] = '6';
				break;

			case KEY7:
				keyflag = 1;
				key09[keycnt++] = '7';
				break;

			case KEY8:
				keyflag = 1;
				key09[keycnt++] = '8';
				break;

			case KEY9:
				keyflag = 1;
				key09[keycnt++] = '9';
				break;

			case KEYXING:
				keyflag = 1;
				keycnt = 1;
				key09[0] = '*';
				key09[1] = 0;
				break;

			case KEYJING:	//#ÇÐ»»ÊäÈë·¨
				clear_area_to_row (MaxRowOneScr - 2, MaxRowOneScr - 1);
				return KEYJING;

			case KEY_UP:			//ºº×ÖÉÏ·­Ò³
				if (pagecnt && confirmflag == 2)
				{
					pagecnt--;
					rowcnt = 0;
					DisplayContaiPYHaizi (PYrowcnt, &pT9PY, PYbuff, 1, pagecnt, &HaiziRightShiftmax);
				}

				break;

			case KEY_DOWN:			//ºº×ÖÏÂ·­Ò³
				if (pagecnt < (strlen ( (char*) pT9PY->MB) - 2) / 14 && confirmflag == 2)  				//´Ë´¦¼õ2  Îª£¬Èç¹û¸ÕºÃÒ»ÐÐÔò²»ÓÃ»»
				{
					pagecnt++;
					rowcnt = 0;
					DisplayContaiPYHaizi (PYrowcnt, &pT9PY, PYbuff, 1, pagecnt, &HaiziRightShiftmax);
#ifdef DEBUG_PRINTF
					printf ("pT9PY->MB=%s\r\n", pT9PY->MB);
#endif
				}

				break;

			case KEY_LEFT:
				if (confirmflag == 1)  				//Æ´Òô¸ßÁÁ×óÒÆ
				{
					if (PYrowcnt > 1)
					{
						PYrowcnt--;
						HighLightPY (PYbuff, PYrowcnt);
						DisplayContaiPYHaizi (PYrowcnt, &pT9PY, PYbuff, 0, pagecnt, &HaiziRightShiftmax);
					}
				}
				else if (confirmflag == 2)  				//ºº×Ö¸ßÁÁ×óÒÆ
				{
					if (rowcnt)
					{
						rowcnt--;
						reverse_area_to_row (6 + 18 * (rowcnt + 1), MaxRowOneScr - 1, 12, 14);
						reverse_area_to_row (6 + 18 * rowcnt, MaxRowOneScr - 1, 12, 14);
					}
				}

				break;

			case KEY_RIGHT:
				if (confirmflag == 1)  				//Æ´Òô¸ßÁÁÓÒÒÆ
				{
					if (PYrowcnt < PYnum)
					{
						PYrowcnt++;
						HighLightPY (PYbuff, PYrowcnt);printf("PYbuff=%s,PYrowcnt=%d\r\n",PYbuff,PYrowcnt);
						DisplayContaiPYHaizi (PYrowcnt, &pT9PY, PYbuff, 0, pagecnt, &HaiziRightShiftmax);
					}
				}
				else if (confirmflag == 2)  				//ºº×Ö¸ßÁÁÓÒÒÆ
				{
					if (pT9PY != NULL && rowcnt < HaiziRightShiftmax)
					{
						rowcnt++;
						reverse_area_to_row (6 + 18 * (rowcnt - 1), MaxRowOneScr - 1, 12, 14);
						reverse_area_to_row (6 + 18 * (rowcnt), MaxRowOneScr - 1, 12, 14);
					}
				}

				break;

			case KEY_Enter:
				keyflag = 4;
				break;

			case KEY_DEL:	//É¾³ý¼ü
				if (keycnt <= 1)  				//É¾³ýµ½×îºó£¬¾Í·µ»Ø
				{
					clear_area_to_row (MaxRowOneScr - 2, MaxRowOneScr - 2);	//Æ´ÒôÊäÈëÕ¼ÓÃ×îºóÁ½ÐÐ£¬ËùÒÔÍË³öÊ±Çå³ý
					return KEY_DEL;
				}
				else 				//É¾³ýÒ»¸öÆ´Òô×ÖÄ¸
				{
					confirmflag = 1;
					keyflag = 1;
					keycnt--;
					key09[keycnt] = 0;
				}

				break;

			case KEY_WaitTimeout:
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
				clear_area_to_row (MaxRowOneScr - 2, MaxRowOneScr - 1);
				return keyval;
		}

		if (keyflag == 1)
		{
			if (confirmflag == 2)  				//1---7Ñ¡ºº×Ö
			{
#ifdef DEBUG_PRINTF
				printf ("key09[keycnt-1]=%c\r\n", key09[keycnt - 1]);
#endif

				if (key09[keycnt - 1] >= '1' && key09[keycnt - 1] <= '7')
				{
					i = key09[keycnt - 1] - '1';
					memset (PyCode, 0, 3);
					clear_area_to_row (MaxRowOneScr - 2, MaxRowOneScr - 2);
#ifdef DEBUG_PRINTF
					printf ("pagecnt=%d\r\n", pagecnt);
					printf ("pT9PY->MB=%s\r\n", pT9PY->MB);
					printf ("i=%d\r\n", i);
#endif
					memcpy (PyCode, pT9PY->MB + pagecnt * 14 + i * 2, 2);
					CalculateCursorOffset (SRF_PY);
					return KEY_Enter;
				}
			}
			else 				//Æ´ÒôÊäÈë
			{
				rowcnt = 0;

				if (keycnt > sizeof (key09) - 1) keycnt = sizeof (key09) - 1;

				if (keycnt == 0)
				{
					clear_area_to_row (MaxRowOneScr - 2, MaxRowOneScr - 2);
				}
				else
				{
					strcpyflag = 0;
					youcipinyinzuohe = 0;

					for (i = 0; t9PY_index[i].T9 != NULL; i++)
					{
						if (strcmp ( (char*) t9PY_index[i].T9, key09) == 0)
						{
							if (!strcpyflag)
							{
								strcpyflag = 1;
								pT9PY = &t9PY_index[i];
								strcpy (PYbuff, (char*) t9PY_index[i].PY);
							}
							else strcat (PYbuff, (char*) t9PY_index[i].PY);

							strcat (PYbuff, " ");
							youcipinyinzuohe = 1;
						}
					}

					if (!youcipinyinzuohe)
					{
						for (i = 0; t9PY_index[i].T9 != NULL; i++)
						{
							if (my_FFstrstr ( (char*) t9PY_index[i].T9, key09) )
							{
								if (!strcpyflag)
								{
									strcpyflag = 1;
									pT9PY = &t9PY_index[i];
									strcpy (PYbuff, (char*) t9PY_index[i].PY);
								}
								else strcat (PYbuff, (char*) t9PY_index[i].PY);

								strcat (PYbuff, " ");
								youcipinyinzuohe = 1;
								break;
							}
						}

						if (!youcipinyinzuohe)
						{
							keycnt--;
							key09[keycnt] = 0;
						}
					}

					if (youcipinyinzuohe)
					{
						pagecnt = 0;
						clear_area_to_row (MaxRowOneScr - 2, MaxRowOneScr - 2);
						PYnum = my_KonggeStrlen (PYbuff);

						if (PYnum)
						{
							PYrowcnt = 1;
							confirmflag = 1;
							HighLightPY (PYbuff, 1);
							DisplayContaiPYHaizi (PYrowcnt, &pT9PY, PYbuff, 0, pagecnt, &HaiziRightShiftmax);
						}
					}
				}
			}
		}
		else if (keyflag == 4)  			//KEY_Enter
		{
			if (confirmflag == 1)  				//µÚÒ»´Î°´È·ÈÏ£¬¸øËùÏÔÊ¾µÄºº×Ö¼ÓÉÏÊý×Ö±êÊ¾¡£
			{
				DisplayContaiPYHaizi (PYrowcnt, &pT9PY, PYbuff, 1, pagecnt, &HaiziRightShiftmax);
				confirmflag = 2;
			}
			else 				//µÚ¶þ´Î°´È·ÈÏ£¬Ñ¡Ôñ¸ßÁÁ´¦µÄºº×Ö·µ»Ø¡£
			{
				memset (PyCode, 0, 3);
				clear_area_to_row (MaxRowOneScr - 2, MaxRowOneScr - 1);
				memcpy (PyCode, pT9PY->MB + pagecnt * 14 + rowcnt * 2, 2);
				CalculateCursorOffset (SRF_PY);
				return KEY_Enter;
			}
		}

#ifdef DEBUG_PRINTF
		printf ("key09=%s\r\n", key09);
#endif

		KeyIsResponse();
	}
}


//0---->9  ºÍ*»á½øÈë´Ëº¯Êý
static u8 BiHuaReadCode (char PyCode[3]) //±Ê»­ÊäÈë·¨¶Áºº×Ö
{
	char key09[15], keyflag, confirmflag = 0, rowcnt, pagecnt = 0;
	int i;
	const T9BH_IDX *pT9BH = NULL;
	char HaiziRightShiftmax;	//ºº×ÖÓÒÒÆ×î´óÖµ
	u8 keycnt = 0;
	u8 keyval;
	u8 flag = 1;

	memset (key09, 0, sizeof (key09) );

	while (1)
	{
		keyval = GetKeyValue();

		if (keyval == 0) continue;

		if (flag == 1)
		{
			flag = 0;
			KeyEventPost_nobeep();
		}
		else
		{
			KeyEventPost();
		}

		keyflag = 0;

		switch (keyval)
		{
			case KEY0:
				keyflag = 1;
				key09[keycnt++] = '0';
				break;

			case KEY1:
				keyflag = 1;
				key09[keycnt++] = '1';
				break;

			case KEY2:
				keyflag = 1;
				key09[keycnt++] = '2';
				break;

			case KEY3:
				keyflag = 1;
				key09[keycnt++] = '3';
				break;

			case KEY4:
				keyflag = 1;
				key09[keycnt++] = '4';
				break;

			case KEY5:
				keyflag = 1;
				key09[keycnt++] = '5';
				break;

			case KEY6:
				keyflag = 1;
				key09[keycnt++] = '6';
				break;

			case KEY7:
				keyflag = 1;
				key09[keycnt++] = '7';
				break;

			case KEYXING:
				keyflag = 1;
				keycnt = 1;
				key09[0] = '*';
				key09[1] = 0;
				break;

			case KEYJING:	//#ÇÐ»»ÊäÈë·¨
				clear_area_to_row (MaxRowOneScr - 2, MaxRowOneScr - 1);
				return KEYJING;

			case KEY_UP:			//ºº×ÖÉÏ·­Ò³
				if (pagecnt && confirmflag == 2)
				{
					pagecnt--;
					rowcnt = 0;
					DisplayContaiBHHaizi (&pT9BH, key09, 1, pagecnt, &HaiziRightShiftmax);
				}

				break;

			case KEY_DOWN:			//ºº×ÖÏÂ·­Ò³
				if (pagecnt < (strlen ( (char*) pT9BH->MB) - 2) / 14 && confirmflag == 2)  				//´Ë´¦¼õ2  Îª£¬Èç¹û¸ÕºÃÒ»ÐÐÔò²»ÓÃ»»
				{
					pagecnt++;
					rowcnt = 0;
					DisplayContaiBHHaizi (&pT9BH, key09, 1, pagecnt, &HaiziRightShiftmax);
#ifdef DEBUG_PRINTF
					printf ("pT9BH->MB=%s\n", pT9BH->MB);
#endif
				}

				break;

			case KEY_LEFT:
				if (confirmflag == 2)  		//ºº×Ö¸ßÁÁ×óÒÆ
				{
					if (rowcnt)
					{
						rowcnt--;
						reverse_area_to_row (6 + 18 * (rowcnt + 1), MaxRowOneScr - 1, 12, 14);
						reverse_area_to_row (6 + 18 * rowcnt, MaxRowOneScr - 1, 12, 14);
					}
				}

				break;

			case KEY_RIGHT:
				if (confirmflag == 2)  		//ºº×Ö¸ßÁÁÓÒÒÆ
				{
					if (pT9BH != NULL && rowcnt < HaiziRightShiftmax)
					{
						rowcnt++;
						reverse_area_to_row (6 + 18 * (rowcnt - 1), MaxRowOneScr - 1, 12, 14);
						reverse_area_to_row (6 + 18 * (rowcnt), MaxRowOneScr - 1, 12, 14);
					}
				}

				break;

			case KEY_Enter:
				keyflag = 4;
				break;

			case KEY_DEL:	//É¾³ý¼ü
				if (keycnt <= 1)  		//É¾³ýµ½×îºó£¬¾Í·µ»Ø
				{
					clear_area_to_row (MaxRowOneScr - 2, MaxRowOneScr - 1);				//±Ê»­ÊäÈëÕ¼ÓÃ×îºóÁ½ÐÐ£¬ËùÒÔÍË³öÊ±Çå³ý
					return KEY_DEL;
				}
				else 				//É¾³ýÒ»¸ö±Ê»­
				{
					confirmflag = (keycnt - 1) * 6;
					clear_area (confirmflag, 44, 6, 6);				//Çå³ý×îºóÒ»¸ö±Ê»­
					confirmflag = 1;
					keyflag = 1;
					keycnt--;
					key09[keycnt] = 0;
				}

				break;

			case KEY_WaitTimeout:
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
				clear_area_to_row (MaxRowOneScr - 2, MaxRowOneScr - 1);
				return keyval;
		}

		if (keyflag == 1)
		{
			if (confirmflag == 2)  				//1---7Ñ¡ºº×Ö
			{
#ifdef DEBUG_PRINTF
				printf ("key09[keycnt-1]=%c\n", key09[keycnt - 1]);
#endif

				if (key09[keycnt - 1] >= '1' && key09[keycnt - 1] <= '7')
				{
					i = key09[keycnt - 1] - '1';
					memset (PyCode, 0, 3);
					clear_area_to_row (MaxRowOneScr - 2, MaxRowOneScr - 2);
#ifdef DEBUG_PRINTF
					printf ("pagecnt=%d\n", pagecnt);
					printf ("pT9BH->MB=%s\n", pT9BH->MB);
					printf ("i=%d\n", i);
#endif
					memcpy (PyCode, pT9BH->MB + pagecnt * 14 + i * 2, 2);
					CalculateCursorOffset (SRF_PY);
					return KEY_Enter;
				}
			}
		}
		else if (keyflag == 4)  			//KEY_Enter
		{
			if (confirmflag == 1)  				//µÚÒ»´Î°´È·ÈÏ£¬¸øËùÏÔÊ¾µÄºº×Ö¼ÓÉÏÊý×Ö±êÊ¾¡£
			{
				DisplayContaiBHHaizi (&pT9BH, key09, 1, pagecnt, &HaiziRightShiftmax);
				confirmflag = 2;
			}
			else 				//µÚ¶þ´Î°´È·ÈÏ£¬Ñ¡Ôñ¸ßÁÁ´¦µÄºº×Ö·µ»Ø¡£
			{
				memset (PyCode, 0, 3);
				clear_area_to_row (MaxRowOneScr - 2, MaxRowOneScr - 2);
				memcpy (PyCode, pT9BH->MB + pagecnt * 14 + rowcnt * 2, 2);
				CalculateCursorOffset (SRF_BiHua);
				return KEY_Enter;
			}
		}

#ifdef DEBUG_PRINTF
		printf ("key09=%s\n", key09);
#endif

		KeyIsResponse();
	}
}


void flashcursor (signed int x, signed int y)
{
	cursorX = x;
	cursorY = y;
}


static void srf_strcat_insert (unsigned char *pMove, const unsigned char *PyCode)
{
	unsigned char insertlen;
	unsigned int pMovelen;

	const unsigned char *psrc;
	unsigned char *pdes;

	insertlen = strlen ( (const char*) PyCode);
	pMovelen = strlen ( (const char*) pMove);

	if (pMovelen)
	{
		psrc = pMove + pMovelen - 1;
		pdes = pMove + pMovelen + insertlen - 1;

		for (; pMovelen != 0; pMovelen--)
		{
			*pdes = *psrc;
			psrc--;
			pdes--;
		}
	}

	memcpy (pMove, PyCode, insertlen);
}


#else
static void srf_strcat_insert (unsigned char *pMove, const unsigned char *PyCode)
{
	unsigned char insertlen;
	unsigned int pMovelen;

	const unsigned char *psrc;
	unsigned char *pdes;

	insertlen = strlen ( (const char*) PyCode);
	pMovelen = strlen ( (const char*) pMove);

	if (pMovelen)
	{
		psrc = pMove + pMovelen - 1;
		pdes = pMove + pMovelen + insertlen - 1;

		for (; pMovelen != 0; pMovelen--)
		{
			*pdes = *psrc;
			psrc--;
			pdes--;
		}
	}

	memcpy (pMove, PyCode, insertlen);
}


static void CalculateCursorOffset (char srf)
{
	signed int x = cursorX, y = cursorY;

	if (srf == SRF_BiHua || srf == SRF_PY) x += 12;
	else x += 6;

	if (x >= 6 * ALineNum)
	{
		x = 0;
		y *= 13;

		if (y >= (EditAreaTotalRow + 1) * 13) y = EditAreaTotalRow * 13;
	}

#ifdef DEBUG_PRINTF
	printf ("cursorX=%d,cursorY=%d\n", x, y);
#endif

	flashcursor (x, y);
}

void flashcursor (signed int x, signed int y)
{
	cursorX = x;
	cursorY = y;
}

/*¸ù¾ÝÖ¸Õë½øÐÐ²åÈë±à¼­*/
/*×îºóÁ½¸ö²ÎÊýÁô¸øabc  ÓëABC  ÊäÈë·¨ÓÃ*/
static void my_AccordPointInsertEdit (const u8 *TheFirstAddrOfBuff, const u8 *pMove, char StartLine, char EndLine, u8 *cursorx, u8 *cursory)
{
	u8 totalline;	//±à¼­ÇøÓÐ¼¸ÐÐ
	int bytenum;	//´Óbuff  Í·µ½¹â±ê´¦ÓÐ¶àÉÙ¸ö×Ö·û
	u8 byteline;	//´Óbuff  Í·µ½¹â±ê´¦ÐèÒª¶àÉÙ¸öÐÐÏÔÊ¾
	//int bytetotal;	//buff  ×Ü¹²ÓÐ¶àÉÙ¸ö×Ö·û
	u8 i, j;
	const u8 *display;
	u8 disp[23];

	*cursorx = 0;
	*cursory = 0;

	totalline = EndLine - StartLine + 1;
	bytenum = pMove - TheFirstAddrOfBuff;
	//bytetotal=strlen((char const*)TheFirstAddrOfBuff);

	byteline = bytenum / ALineNum;

	if (bytenum % ALineNum) byteline++;

	display = TheFirstAddrOfBuff;	//ÕÒ³öÐèÒªÏÔÊ¾µÄµØÖ·

	if (byteline > totalline)
	{
		for (i = 0; i < byteline - totalline; i++)
		{
			for (j = 0; j < ALineNum; j++, display++)
			{
				if (*display > 0x80)
				{
					display++;
					j++;
				}
			}
		}
	}

#ifdef DEBUG_PRINTF
	printf ("display=%s\n", display);
#endif

	if (*display != 0)  		//Èç¹ûÓÐ×Ö·û
	{
		for (i = 0; i < totalline; i++)  			//ÏÔÊ¾pMove  Ö®Ç°Ö®ºóÄÜÔÚ" ÏÔÊ¾·¶Î§Ö®ÄÚ"  µÄ×Ö·û
		{
			for (j = 0; j < ALineNum && *display != 0;)
			{
				if (*display > 0x80)
				{
					disp[j] = *display;
					j++;
					display++;

					if (display == pMove)
					{
						*cursory = i;
						*cursorx = j;
					}
				}

				disp[j] = *display;
				j++;
				display++;

				if (display == pMove)
				{
					*cursory = i;
					*cursorx = j;
				}
			}

			for (; j < ALineNum + 2; j++) disp[j] = ' ';			//´Ë´¦ALineNum+1  ÎªÁËÈ¥³ýÉ¾³ý×Ö·ûÊÇ£¬×îºó¿ÉÄÜÏÔÊ¾°ë¸öºº×ÖµÄÎÊÌâ

			disp[j] = 0;
#ifdef DEBUG_PRINTF
			printf ("disline=%s\n", disp);
#endif
			text_out_to_row_Language2 (StartLine + i, disp, disp);
		}
	}
	else 		//Èç¹ûÃ»×Ö·û£¬ÔòÓÐ¿ÉÄÜÊÇÉ¾³ýÍêÁË£¬ÔòÆÁÄ»»áÊ£Ò»¸ö×Ö·û
	{
		text_out_to_row_Language2 (StartLine, "   ", "   ");
	}

#ifdef DEBUG_PRINTF
	printf ("bytenum=%d\n", bytenum);
#endif

	*cursorx *= 6;
	*cursory += StartLine;
	*cursory *= 13;
	flashcursor (*cursorx, *cursory);
}

#endif

static void flashguangbiao_to_row (u8 x, u8 row)
{
	flashcursor (x, 13 * row);
}

static u8 abcABCflag = 0;

static u8 abcReadCode (const u8 *BuffTheFirstAddr, u8 *pMove)
{
	u8 prekey = 0, curkey = 0, keyflag;
	u8 curpos = 0;
	u8 x, y;
	u8 temp[2] = {0, 0};
	unsigned int PreviousMs, CurrentMs;
	u8 keyval;
	u8 flag = 1;

	const char *ABCcode[10] =
	{
		" 0",
		",.:%1@/?;\'\"|\\{[}]=+-_)(*&^$#!~`<>",
		"abc2",
		"def3",
		"ghi4",
		"jkl5",
		"mno6",
		"pqrs7",
		"tuv8",
		"wxyz9"
	};

	CalculateCursorOffset (SRF_abc);	//²»¹ÜÔõÑù£¬½øÈëµ½ÕâÀï±Ø¶¨»á·µ»ØÒ»¸öasccii  Âë
	x = pMove - BuffTheFirstAddr;

	PreviousMs = CurrentMs = my_GetCurrentMs();

	while (1)
	{
		CurrentMs = my_GetCurrentMs();

		if (CurrentMs - PreviousMs >= 500)
		{
			goto tuichu;
		}

		keyval = GetKeyValue();

		if (keyval == 0) continue;

		PreviousMs = CurrentMs;

		if (flag == 0) KeyEventPost();
		else flag = 0;

		keyflag = 0;

		switch (keyval)
		{
			case KEY0:
				keyflag = 1;
				prekey = curkey;
				curkey = '0';
				break;

			case KEY1:
				keyflag = 1;
				prekey = curkey;
				curkey = '1';
				break;

			case KEY2:
				keyflag = 1;
				prekey = curkey;
				curkey = '2';
				break;

			case KEY3:
				keyflag = 1;
				prekey = curkey;
				curkey = '3';
				break;

			case KEY4:
				keyflag = 1;
				prekey = curkey;
				curkey = '4';
				break;

			case KEY5:
				keyflag = 1;
				prekey = curkey;
				curkey = '5';
				break;

			case KEY6:
				keyflag = 1;
				prekey = curkey;
				curkey = '6';
				break;

			case KEY7:
				keyflag = 1;
				prekey = curkey;
				curkey = '7';
				break;

			case KEY8:
				keyflag = 1;
				prekey = curkey;
				curkey = '8';
				break;

			case KEY9:
				keyflag = 1;
				prekey = curkey;
				curkey = '9';
				break;

			case KEYXING:
				keyflag = 1;
				prekey = curkey;
				curkey = '1';
				break;

			case KEY_Enter:
				goto tuichu;

			case KEYJING:
			case KEY_WaitTimeout:
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
				return keyval;
		}

		if (keyflag == 1)
		{
			if (prekey != 0 && prekey != curkey)
			{
				PostKey (keyval);
				abcABCflag = 1;
				return KEY_NONE;
			}
			else
			{
				temp[0] = * (ABCcode[curkey - '0'] + curpos);
				curpos++;				//"abc2" Ö®ÀàµÄÂÖÁ÷µÄÎ»ÖÃ

				if (prekey == 0)
				{
					srf_strcat_insert (pMove - 1, (unsigned char*) temp);
				}
				else
				{
					if (curpos > strlen (ABCcode[curkey - '0']) - 1) curpos = 0;

					memcpy (pMove - 1, temp, 1);
				}

				my_AccordPointInsertEdit (BuffTheFirstAddr, pMove, 1, 2, &x, &y);
				x -= 6;
#ifdef DEBUG_PRINTF
				printf ("x=%d  y=%d\n", x, y);
#endif

				if (x >= ALineNum * 6)
				{
					x = 0;

					if (y < (EditAreaTotalRow + EditAreaStartLine - 1) * 13) y += 13;
				}

				reverse_area (x, y, 7, 12);				//·´ÏÔ¹â±ê´¦×Ö·û
			}
		}

#ifdef DEBUG_PRINTF
		printf ("curkey=%c\n", curkey);
#endif

		KeyIsResponse();
	}

tuichu:
	KeyIsResponse();
	reverse_area (x, y, 7, 11);

	return KEY_NONE;
}

static u8 ABCReadCode (const u8 *BuffTheFirstAddr, u8 *pMove)
{
	u8 prekey = 0, curkey = 0, keyflag;
	u8 curpos = 0;
	u8 x, y;
	u8 temp[2] = {0, 0};
	unsigned int PreviousMs, CurrentMs;
	u8 keyval;
	u8 flag = 1;

	const char *ABCcode[10] =
	{
		" 0",
		",.:%1@/?;\'\"|\\{[}]=+-_)(*&^$#!~`<>",
		"ABC2",
		"DEF3",
		"GHI4",
		"JKL5",
		"MNO6",
		"PQRS7",
		"TUV8",
		"WXYZ9"
	};

	CalculateCursorOffset (SRF_ABC);	//²»¹ÜÔõÑù£¬½øÈëµ½ÕâÀï±Ø¶¨»á·µ»ØÒ»¸öasccii  Âë
	x = pMove - BuffTheFirstAddr;
#ifdef DEBUG_PRINTF
	printf ("len=%d\n", x);
#endif
	PreviousMs = CurrentMs = my_GetCurrentMs();

	while (1)
	{
		CurrentMs = my_GetCurrentMs();

		if (CurrentMs - PreviousMs >= 500)
		{
			goto tuichu;
		}

		keyval = GetKeyValue();

		if (keyval == 0) continue;

		PreviousMs = CurrentMs;

		if (flag == 0) KeyEventPost();
		else flag = 0;

		keyflag = 0;

		switch (keyval)
		{
			case KEY0:
				keyflag = 1;
				prekey = curkey;
				curkey = '0';
				break;

			case KEY1:
				keyflag = 1;
				prekey = curkey;
				curkey = '1';
				break;

			case KEY2:
				keyflag = 1;
				prekey = curkey;
				curkey = '2';
				break;

			case KEY3:
				keyflag = 1;
				prekey = curkey;
				curkey = '3';
				break;

			case KEY4:
				keyflag = 1;
				prekey = curkey;
				curkey = '4';
				break;

			case KEY5:
				keyflag = 1;
				prekey = curkey;
				curkey = '5';
				break;

			case KEY6:
				keyflag = 1;
				prekey = curkey;
				curkey = '6';
				break;

			case KEY7:
				keyflag = 1;
				prekey = curkey;
				curkey = '7';
				break;

			case KEY8:
				keyflag = 1;
				prekey = curkey;
				curkey = '8';
				break;

			case KEY9:
				keyflag = 1;
				prekey = curkey;
				curkey = '9';
				break;

			case KEYXING:
				keyflag = 1;
				prekey = curkey;
				curkey = '1';
				break;

			case KEY_Enter:
				goto tuichu;

			case KEYJING:
			case KEY_WaitTimeout:
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
				return keyval;
		}

		if (keyflag == 1)
		{
			if (prekey != 0 && prekey != curkey)
			{
				PostKey (keyval);
				abcABCflag = 1;
				return KEY_NONE;
			}
			else
			{
				temp[0] = * (ABCcode[curkey - '0'] + curpos);
				curpos++;				//"abc2" Ö®ÀàµÄÂÖÁ÷µÄÎ»ÖÃ

				if (prekey == 0)
				{
					srf_strcat_insert ( (unsigned char*) pMove - 1, (unsigned char*) temp);
				}
				else
				{
					if (curpos > strlen (ABCcode[curkey - '0']) - 1) curpos = 0;

					memcpy (pMove - 1, temp, 1);
				}

				my_AccordPointInsertEdit (BuffTheFirstAddr, pMove, 1, 2, &x, &y);
				x -= 6;
#ifdef DEBUG_PRINTF
				printf ("x=%d  y=%d\n", x, y);
#endif

				if (x >= ALineNum * 6)
				{
					x = 0;

					if (y < (EditAreaTotalRow + EditAreaStartLine - 1) * 13) y += 13;
				}

				reverse_area (x, y, 7, 12);				//·´ÏÔ¹â±ê´¦×Ö·û
			}
		}

#ifdef DEBUG_PRINTF
		printf ("curkey=%c\n", curkey);
#endif

		KeyIsResponse();
	}

tuichu:
	KeyIsResponse();
	reverse_area (x, y, 7, 11);

	return KEY_NONE;
}

static u8 _123ReadCode (u8 PyCode[3], u8 keyval)
{
	memset (PyCode, 0, 3);

	switch (keyval)
	{
		case KEY0:
			PyCode[0] = '0';
			break;

		case KEY1:
			PyCode[0] = '1';
			break;

		case KEY2:
			PyCode[0] = '2';
			break;

		case KEY3:
			PyCode[0] = '3';
			break;

		case KEY4:
			PyCode[0] = '4';
			break;

		case KEY5:
			PyCode[0] = '5';
			break;

		case KEY6:
			PyCode[0] = '6';
			break;

		case KEY7:
			PyCode[0] = '7';
			break;

		case KEY8:
			PyCode[0] = '8';
			break;

		case KEY9:
			PyCode[0] = '9';
			break;

		case KEYXING:
			PyCode[0] = '.';
			break;
	}

	if ( (PyCode[0] >= '0' && PyCode[0] <= '9') || PyCode[0] == '.')
	{
		CalculateCursorOffset (SRF_123);
		return KEY_Enter;
	}
	else return KEY_ReturnInitInterface;
}

static u8 _123TimeInput (u8 PyCode[3], u8 keyval)
{
	memset (PyCode, 0, 3);

	switch (keyval)
	{
		case KEY0:
			PyCode[0] = '0';
			break;

		case KEY1:
			PyCode[0] = '1';
			break;

		case KEY2:
			PyCode[0] = '2';
			break;

		case KEY3:
			PyCode[0] = '3';
			break;

		case KEY4:
			PyCode[0] = '4';
			break;

		case KEY5:
			PyCode[0] = '5';
			break;

		case KEY6:
			PyCode[0] = '6';
			break;

		case KEY7:
			PyCode[0] = '7';
			break;

		case KEY8:
			PyCode[0] = '8';
			break;

		case KEY9:
			PyCode[0] = '9';
			break;

		case KEYXING:
			PyCode[0] = ':';
			break;
	}

	if ( (PyCode[0] >= '0' && PyCode[0] <= '9') || PyCode[0] == ':')
	{
		CalculateCursorOffset (SRF_123);
		return KEY_Enter;
	}
	else return KEY_ReturnInitInterface;
}

typedef struct
{
	const char srf[5];
	char Enable;
} T_ShuRuFa;


static u8 *GetSrfString (u8 currentSrf)
{
	if (currentSrf == SRF_PY) return "Æ´Òô";
	else if (currentSrf == SRF_123) return "123 ";
	else if (currentSrf == SRF_abc) return "abc ";
	else if (currentSrf == SRF_ABC) return "ABC ";
	else if (currentSrf == SRF_BiHua) return "±Ê»­";
	else return "abc ";
}

static void DisplaySrfAndInput (u8 currentSrf)
{
	clear_area_to_row (MaxRowOneScr - 1, MaxRowOneScr - 1);
	text_out_to_row_x (MaxRowOneScr - 1, 0, GetSrfString (currentSrf) );
	reverse_area_to_row (0, MaxRowOneScr - 1, 25, 16);
	text_out_to_row_x (MaxRowOneScr - 1, 60, "input...");
}

static u8 SwitchSrf (u8 VolidSrf, u8 currentSrf)
{
	if (currentSrf == SRF_PY)
	{
		if (VolidSrf & SRF_123) return SRF_123;
		else if (VolidSrf & SRF_abc) return SRF_abc;
		else if (VolidSrf & SRF_ABC) return SRF_ABC;
		else if (VolidSrf & SRF_BiHua) return SRF_BiHua;
		else return SRF_PY;
	}
	else if (currentSrf == SRF_123)
	{
		if (VolidSrf & SRF_abc) return SRF_abc;
		else if (VolidSrf & SRF_ABC) return SRF_ABC;
		else if (VolidSrf & SRF_BiHua) return SRF_BiHua;
		else if (VolidSrf & SRF_PY) return SRF_PY;
		else return SRF_123;
	}
	else if (currentSrf == SRF_abc)
	{
		if (VolidSrf & SRF_ABC) return SRF_ABC;
		else if (VolidSrf & SRF_BiHua) return SRF_BiHua;
		else if (VolidSrf & SRF_PY) return SRF_PY;
		else if (VolidSrf & SRF_123) return SRF_123;
		else return SRF_abc;
	}
	else if (currentSrf == SRF_ABC)
	{
		if (VolidSrf & SRF_BiHua) return SRF_BiHua;
		else if (VolidSrf & SRF_PY) return SRF_PY;
		else if (VolidSrf & SRF_123) return SRF_123;
		else if (VolidSrf & SRF_abc) return SRF_abc;
		else return SRF_ABC;
	}
	else if (currentSrf == SRF_BiHua)
	{
		if (VolidSrf & SRF_PY) return SRF_PY;
		else if (VolidSrf & SRF_123) return SRF_123;
		else if (VolidSrf & SRF_abc) return SRF_abc;
		else if (VolidSrf & SRF_ABC) return SRF_ABC;
		else return SRF_BiHua;
	}
	else
	{
		return SRF_abc;
	}
}


static void flightcursor (void)
{
	static unsigned int PreviousMs;
	unsigned int CurrentMs;

	CurrentMs = my_GetCurrentMs();

	if (CurrentMs > PreviousMs && CurrentMs - PreviousMs >= 500)
	{
		PreviousMs = CurrentMs;
		reverse_area (cursorX, cursorY, 1, 12);
	}
}

u8 _GetString_abcABC123PYWuBi (u8 srfFlag, u8 DefaultSrf, u8 *receive, u8 recSize, u8 HaveoldFlag)
{
	int x;
	u8 keyflag;
	u8 cursorx, cursory;
	u8 inputCode[3];
	u8 i = 0;
	u8 *pMove;
	u8 ChineseThinkFlag = 0;
	u8 keyval;
	u8 currentSrf;

	currentSrf = DefaultSrf;
	DisplaySrfAndInput (currentSrf);

	if (HaveoldFlag)
	{
		x = strlen ( (sc8*) receive);
		memset (receive + x, 0, recSize - x);		//Ï´¾»´«½øÀ´µÄreceive  »º³åÇøÎÞÓÃµÄÎ²²¿Êý¾Ý¡£
	}
	else
	{
		x = 0;
		memset (receive, 0, recSize);
	}

	flashguangbiao_to_row (0, EditAreaStartLine);
	pMove = receive + x;
	PostKey (KEY_pseudo);

	while (1)
	{
		flightcursor();
		keyval = GetKeyValue();

		if (keyval == KEY_NONE) continue;

		if (keyval != KEY_pseudo)
		{
			if (beepKG[0] == '1')
			{
				if (abcABCflag == 0) BeepNormal();

				abcABCflag = 0;
			}

			kmy_LcdBlackLightOn();
			kmy_AutoOffLcdLight (120);
			kmy_AutoPostKey (120);
		}

		keyflag = KEY_NONE;

		switch (keyval)
		{
			case KEY0:
			case KEY1:
			case KEY2:
			case KEY3:
			case KEY4:
			case KEY5:
			case KEY6:
			case KEY7:
			case KEY8:
			case KEY9:
			case KEYXING:
				if (ChineseThinkFlag == 0)
				{
	loop:

					if (strlen ( (sc8*) receive) < recSize - 1)  		//´Ë´¦recSize  Îª×Ö·û³¤¶ÈÏÞÖÆ
					{
						if (currentSrf == SRF_PY)  						//Æ´Òô
						{
#ifdef AddBiHuaPinYin
							PostKey (keyval);
							keyval = PinYinReadCode ( (char*) inputCode);
#endif
						}
						else if (currentSrf == SRF_BiHua)  			//±Ê»­
						{
#ifdef AddBiHuaPinYin
							PostKey (keyval);
							keyval = BiHuaReadCode ( (char*) inputCode);
#endif
						}
						else if (currentSrf == SRF_abc)  				//abc
						{
							pMove++;
							PostKey (keyval);
							keyval = abcReadCode (receive, pMove);

							if (keyval != KEY_NONE && keyval != KEYJING)
							{
								goto loop;
							}
						}
						else if (currentSrf == SRF_ABC)  				//ABC
						{
							pMove++;
							PostKey (keyval);
							keyval = ABCReadCode (receive, pMove);

							if (keyval != KEY_NONE && keyval != KEYJING)
							{
								goto loop;
							}
						}
						else if (currentSrf == SRF_123)  				//123
						{
							keyval = _123ReadCode (inputCode, keyval);
						}

						if (keyval == KEY_Enter)
						{
							srf_strcat_insert (pMove, inputCode);

							if (*pMove > 0x80) pMove += 2;
							else pMove++;

							keyflag = 2;
						}
						else if (keyval == KEY_DEL)
						{
							keyflag = 2;
						}
						else if (keyval == KEYJING)
						{
							keyflag = 1;
						}
					}
				}

				break;

			case KEYJING:	//#ÇÐ»»ÊäÈë·¨
				keyflag = 1;
				break;

			case KEY_Enter:	//È·ÈÏ·µ»Ø
				return KEY_Enter;

			case KEY_UP:			//¹â±êÉÏÒÆ
				if (ChineseThinkFlag == 0)
				{
					if (pMove - receive > ALineNum)
					{
						for (i = 0; i < ALineNum; i++)
						{
							if (* (pMove - 1) > 0x80)  							//GB2312  ÊÇ¿ÉÒÔµÄ£¬GB18030  ÕâÖÖÅÐ¶ÏÊÇ²»ÐÐµÄ¡£
							{
								pMove--;
								i++;
							}

							pMove--;
						}
					}
				}

				break;

			case KEY_DOWN:			//¹â±êÏÂÒÆ
				if (ChineseThinkFlag == 0)
				{
					for (i = 0; i < ALineNum && *pMove != 0; i++)
					{
						if (*pMove > 0x80)
						{
							pMove++;
							i++;
						}

						pMove++;
					}
				}

				break;

			case KEY_LEFT:			//¹â±ê×óÒÆ
				if (pMove > receive)
				{
					if (* (pMove - 1) > 0x80) pMove--;				//GB2312  ÊÇ¿ÉÒÔµÄ£¬GB18030  ÕâÖÖÅÐ¶ÏÊÇ²»ÐÐµÄ¡£

					pMove--;
				}

				break;

			case KEY_RIGHT:			//¹â±êÓÒÒÆ
				if (*pMove)
				{
					if (*pMove > 0x80) pMove++;

					pMove++;
				}

				break;
				
			case KEY_DEL:
				x = strlen ( (sc8*) receive);

				if (x != 0)
				{
					if (pMove > receive)
					{
						if (*pMove != 0)  						//Èç¹ûÔÚ×Ö·û´®Ö®ÖÐÉ¾³ý
						{
							if (* (pMove - 1) > 0x80)
							{
								pMove -= 2;
								my_strcpy_DeleteTailByte (pMove, pMove + 2);
							}
							else
							{
								pMove -= 1;
								my_strcpy_DeleteTailByte (pMove, pMove + 1);
							}
						}
						else
						{
							if (pMove > receive && * (pMove - 1) > 0x80)  							//delete a Chinese character
							{
								pMove--;
								*pMove = 0;
							}

							pMove--;
							*pMove = 0;
						}
					}
				}
				else
				{
					return KEY_LEFT;
				}

				break;

			case KEY_WaitTimeout:
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
				return keyval;
		}

		if (keyflag == 1 || (keyflag == 2 && currentSrf != 4) )
		{
			if (keyflag == 1)
			{
				currentSrf = SwitchSrf (srfFlag, currentSrf);
			}

			DisplaySrfAndInput (currentSrf);
		}

		my_AccordPointInsertEdit (receive, pMove, 1, 2, &cursorx, &cursory);
		KeyIsResponse();
	}
}
u8 _GetString_123TimeInput (u8 srfFlag, u8 DefaultSrf, u8 *receive, u8 recSize, u8 HaveoldFlag)
{
	int x;
	u8 keyflag;
	u8 cursorx, cursory;
	u8 inputCode[3];
	u8 i = 0;
	u8 *pMove;
	u8 ChineseThinkFlag = 0;
	u8 keyval;
	u8 currentSrf;

	currentSrf = DefaultSrf;
	DisplaySrfAndInput (currentSrf);

	if (HaveoldFlag)
	{
		x = strlen ( (sc8*) receive);
		memset (receive + x, 0, recSize - x);		//Ï´¾»´«½øÀ´µÄreceive  »º³åÇøÎÞÓÃµÄÎ²²¿Êý¾Ý¡£
	}
	else
	{
		x = 0;
		memset (receive, 0, recSize);
	}

	flashguangbiao_to_row (0, EditAreaStartLine);
	pMove = receive + x;
	PostKey (KEY_pseudo);

	while (1)
	{
		flightcursor();
		keyval = GetKeyValue();

		if (keyval == KEY_NONE) continue;

		if (keyval != KEY_pseudo)
		{
			if (beepKG[0] == '1')
			{
				if (abcABCflag == 0) BeepNormal();

				abcABCflag = 0;
			}

			kmy_LcdBlackLightOn();
			kmy_AutoOffLcdLight (120);
			kmy_AutoPostKey (120);
		}

		keyflag = KEY_NONE;

		switch (keyval)
		{
			case KEY0:
			case KEY1:
			case KEY2:
			case KEY3:
			case KEY4:
			case KEY5:
			case KEY6:
			case KEY7:
			case KEY8:
			case KEY9:
			case KEYXING:
				if (ChineseThinkFlag == 0)
				{

					if (strlen ( (sc8*) receive) < recSize - 1)  		//´Ë´¦recSize  Îª×Ö·û³¤¶ÈÏÞÖÆ
					{						
						 if (currentSrf == SRF_123)  				//123
						{
							keyval = _123TimeInput (inputCode, keyval);
						}

						if (keyval == KEY_Enter)
						{
							srf_strcat_insert (pMove, inputCode);

							if (*pMove > 0x80) pMove += 2;
							else pMove++;

							keyflag = 2;
						}
						else if (keyval == KEY_DEL)
						{
							keyflag = 2;
						}
						else if (keyval == KEYJING)
						{
							keyflag = 1;
						}
					}
				}

				break;

			case KEYJING:	//#ÇÐ»»ÊäÈë·¨
				keyflag = 1;
				break;

			case KEY_Enter:	//È·ÈÏ·µ»Ø
				return KEY_Enter;

			case KEY_UP:			//¹â±êÉÏÒÆ
				if (ChineseThinkFlag == 0)
				{
					if (pMove - receive > ALineNum)
					{
						for (i = 0; i < ALineNum; i++)
						{
							if (* (pMove - 1) > 0x80)  							//GB2312  ÊÇ¿ÉÒÔµÄ£¬GB18030  ÕâÖÖÅÐ¶ÏÊÇ²»ÐÐµÄ¡£
							{
								pMove--;
								i++;

							}

							pMove--;
						}
					}
				}

				break;

			case KEY_DOWN:			//¹â±êÏÂÒÆ
				if (ChineseThinkFlag == 0)
				{
					for (i = 0; i < ALineNum && *pMove != 0; i++)
					{
						if (*pMove > 0x80)
						{
							pMove++;
							i++;
						}

						pMove++;
					}
				}

				break;

			case KEY_LEFT:			//¹â±ê×óÒÆ
				if (pMove > receive)
				{
					if (* (pMove - 1) > 0x80) pMove--;				//GB2312  ÊÇ¿ÉÒÔµÄ£¬GB18030  ÕâÖÖÅÐ¶ÏÊÇ²»ÐÐµÄ¡£

					pMove--;
				}

				break;

			case KEY_RIGHT:			//¹â±êÓÒÒÆ
				if (*pMove)
				{
					if (*pMove > 0x80) pMove++;

					pMove++;
				}

				break;

			case KEY_DEL:
				x = strlen ( (sc8*) receive);

				if (x != 0)
				{
					if (pMove > receive)
					{
						if (*pMove != 0)  						//Èç¹ûÔÚ×Ö·û´®Ö®ÖÐÉ¾³ý
						{
							if (* (pMove - 1) > 0x80)
							{
								pMove -= 2;
								my_strcpy_DeleteTailByte (pMove, pMove + 2);
							}
							else
							{
								pMove -= 1;
								my_strcpy_DeleteTailByte (pMove, pMove + 1);
							}
						}
						else
						{
							if (pMove > receive && * (pMove - 1) > 0x80)  							//delete a Chinese character
							{
								pMove--;
								*pMove = 0;
							}

							pMove--;
							*pMove = 0;
						}
					}
				}
				else
				{
					return KEY_LEFT;
				}

				break;

			case KEY_WaitTimeout:
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
				return keyval;
		}

		if (keyflag == 1 || (keyflag == 2 && currentSrf != 4) )
		{
			if (keyflag == 1)
			{
				currentSrf = SwitchSrf (srfFlag, currentSrf);
			}

			DisplaySrfAndInput (currentSrf);
		}

		my_AccordPointInsertEdit (receive, pMove, 1, 2, &cursorx, &cursory);
		KeyIsResponse();
	}
}

static u8 my_abcReadCode (const u8 *BuffTheFirstAddr, u8 *pMove, char row)
{
	u8 prekey = 0, curkey = 0, keyflag;
	u8 curpos = 0;
	u8 x, y;
	u8 temp[2] = {0, 0};
	unsigned int PreviousMs, CurrentMs;
	u8 keyval;
	u8 flag = 1;

	const char *ABCcode[10] =
	{
		" 0",
		",.@1/?:;\'\"|\\{[}]=+-_)(*&^%$#!~`<>",
		"abc2",
		"def3",
		"ghi4",
		"jkl5",
		"mno6",
		"pqrs7",
		"tuv8",
		"wxyz9"
	};

	CalculateCursorOffset (SRF_abc);	//²»¹ÜÔõÑù£¬½øÈëµ½ÕâÀï±Ø¶¨»á·µ»ØÒ»¸öasccii  Âë
	x = pMove - BuffTheFirstAddr;

	PreviousMs = CurrentMs = my_GetCurrentMs();

	while (1)
	{
		CurrentMs = my_GetCurrentMs();

		if (CurrentMs - PreviousMs >= 500)
		{
			goto tuichu;
		}

		keyval = GetKeyValue();

		if (keyval == 0) continue;

		PreviousMs = CurrentMs;

		if (flag == 0) KeyEventPost();
		else flag = 0;

		keyflag = 0;

		switch (keyval)
		{
			case KEY0:
				keyflag = 1;
				prekey = curkey;
				curkey = '0';
				break;

			case KEY1:
				keyflag = 1;
				prekey = curkey;
				curkey = '1';
				break;

			case KEY2:
				keyflag = 1;
				prekey = curkey;
				curkey = '2';
				break;

			case KEY3:
				keyflag = 1;
				prekey = curkey;
				curkey = '3';
				break;

			case KEY4:
				keyflag = 1;
				prekey = curkey;
				curkey = '4';
				break;

			case KEY5:
				keyflag = 1;
				prekey = curkey;
				curkey = '5';
				break;

			case KEY6:
				keyflag = 1;
				prekey = curkey;
				curkey = '6';
				break;

			case KEY7:
				keyflag = 1;
				prekey = curkey;
				curkey = '7';
				break;

			case KEY8:
				keyflag = 1;
				prekey = curkey;
				curkey = '8';
				break;

			case KEY9:
				keyflag = 1;
				prekey = curkey;
				curkey = '9';
				break;

			case KEYXING:
				keyflag = 1;
				prekey = curkey;
				curkey = '1';
				break;

			case KEY_Enter:
				goto tuichu;

			case KEYJING:
			case KEY_WaitTimeout:
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
				return keyval;
		}

		if (keyflag == 1)
		{
			if (prekey != 0 && prekey != curkey)
			{
				PostKey (keyval);
				abcABCflag = 1;
				return KEY_NONE;
			}
			else
			{
				temp[0] = * (ABCcode[curkey - '0'] + curpos);
				curpos++;				//"abc2" Ö®ÀàµÄÂÖÁ÷µÄÎ»ÖÃ

				if (prekey == 0)
				{
					srf_strcat_insert (pMove - 1, (unsigned char*) temp);
				}
				else
				{
					if (curpos > strlen (ABCcode[curkey - '0']) - 1) curpos = 0;

					memcpy (pMove - 1, temp, 1);
				}

				my_AccordPointInsertEdit (BuffTheFirstAddr, pMove, row, row, &x, &y);
				x -= 6;
#ifdef DEBUG_PRINTF
				printf ("x=%d  y=%d\n", x, y);
#endif

				if (x >= ALineNum * 6)
				{
					x = 0;

					if (y < (EditAreaTotalRow + EditAreaStartLine - 1) * 13) y += 13;
				}

				reverse_area (x, y, 7, 12);				//·´ÏÔ¹â±ê´¦×Ö·û
			}
		}

#ifdef DEBUG_PRINTF
		printf ("curkey=%c\n", curkey);
#endif

		KeyIsResponse();
	}

tuichu:
	KeyIsResponse();
	reverse_area (x, y, 7, 11);

	return KEY_NONE;
}

static u8 my_ABCReadCode (const u8 *BuffTheFirstAddr, u8 *pMove, char row)
{
	u8 prekey = 0, curkey = 0, keyflag;
	u8 curpos = 0;
	u8 x, y;
	u8 temp[2] = {0, 0};
	unsigned int PreviousMs, CurrentMs;
	u8 keyval;
	u8 flag = 1;

	const char *ABCcode[10] =
	{
		" 0",
		",.@1/?:;\'\"|\\{[}]=+-_)(*&^%$#!~`<>",
		"ABC2",
		"DEF3",
		"GHI4",
		"JKL5",
		"MNO6",
		"PQRS7",
		"TUV8",
		"WXYZ9"
	};

	CalculateCursorOffset (SRF_ABC);	//²»¹ÜÔõÑù£¬½øÈëµ½ÕâÀï±Ø¶¨»á·µ»ØÒ»¸öasccii	Âë
	x = pMove - BuffTheFirstAddr;
#ifdef DEBUG_PRINTF
	printf ("len=%d\n", x);
#endif
	PreviousMs = CurrentMs = my_GetCurrentMs();

	while (1)
	{
		CurrentMs = my_GetCurrentMs();

		if (CurrentMs - PreviousMs >= 500)
		{
			goto tuichu;
		}

		keyval = GetKeyValue();

		if (keyval == 0) continue;

		PreviousMs = CurrentMs;

		if (flag == 0) KeyEventPost();
		else flag = 0;

		keyflag = 0;

		switch (keyval)
		{
			case KEY0:
				keyflag = 1;
				prekey = curkey;
				curkey = '0';
				break;

			case KEY1:
				keyflag = 1;
				prekey = curkey;
				curkey = '1';
				break;

			case KEY2:
				keyflag = 1;
				prekey = curkey;
				curkey = '2';
				break;

			case KEY3:
				keyflag = 1;
				prekey = curkey;
				curkey = '3';
				break;

			case KEY4:
				keyflag = 1;
				prekey = curkey;
				curkey = '4';
				break;

			case KEY5:
				keyflag = 1;
				prekey = curkey;
				curkey = '5';
				break;

			case KEY6:
				keyflag = 1;
				prekey = curkey;
				curkey = '6';
				break;

			case KEY7:
				keyflag = 1;
				prekey = curkey;
				curkey = '7';
				break;

			case KEY8:
				keyflag = 1;
				prekey = curkey;
				curkey = '8';
				break;

			case KEY9:
				keyflag = 1;
				prekey = curkey;
				curkey = '9';
				break;

			case KEYXING:
				keyflag = 1;
				prekey = curkey;
				curkey = '1';
				break;

			case KEY_Enter:
				goto tuichu;

			case KEYJING:
			case KEY_WaitTimeout:
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
				return keyval;
		}

		if (keyflag == 1)
		{
			if (prekey != 0 && prekey != curkey)
			{
				PostKey (keyval);
				abcABCflag = 1;
				return KEY_NONE;
			}
			else
			{
				temp[0] = * (ABCcode[curkey - '0'] + curpos);
				curpos++;				//"abc2" Ö®ÀàµÄÂÖÁ÷µÄÎ»ÖÃ

				if (prekey == 0)
				{
					srf_strcat_insert ( (unsigned char*) pMove - 1, (unsigned char*) temp);
				}
				else
				{
					if (curpos > strlen (ABCcode[curkey - '0']) - 1) curpos = 0;

					memcpy (pMove - 1, temp, 1);
				}

				my_AccordPointInsertEdit (BuffTheFirstAddr, pMove, row, row, &x, &y);
				x -= 6;
#ifdef DEBUG_PRINTF
				printf ("x=%d  y=%d\n", x, y);
#endif

				if (x >= ALineNum * 6)
				{
					x = 0;

					if (y < (EditAreaTotalRow + EditAreaStartLine - 1) * 13) y += 13;
				}

				reverse_area (x, y, 7, 12); 			//·´ÏÔ¹â±ê´¦×Ö·û
			}
		}

#ifdef DEBUG_PRINTF
		printf ("curkey=%c\n", curkey);
#endif

		KeyIsResponse();
	}

tuichu:
	KeyIsResponse();
	reverse_area (x, y, 7, 11);

	return KEY_NONE;
}


char GetRejectReasonAndInput(void)
{
	char ReasonTable[15][MENU_COL]={0};
	char rejectNum=0;
	char *pMove;
	char keyval=0;
	char cursor_row=0;	
	char cursor_index=0;
	char i=0;
	char flashflag=0;
	char index=0;
	char keyflag=0;

	clear_lcd();
	GetRejectReason(ReasonTable,(char*)RejectReason,&rejectNum);
	if(rejectNum==0)
	{
		rejectNum=1;
	}
	
	cursor_index=rejectNum-1;	
	index=rejectNum-1;
	
	flashguangbiao_to_row (0, 0);
	if(rejectNum>4)
	{
		cursor_row=4;
	}
	else
	{
		cursor_index=4;
		cursor_row=index;
	}
	
	pMove = ReasonTable[index] + strlen(ReasonTable[index]);
	cursorX=6*strlen(ReasonTable[index]);
	if(index<=4)
	{
		cursorY=13*(index%5);
	}
	else
	{
		cursorY=13*4;
	}
	
	flashflag=1;

	while (1)
	{
		if(flashflag==1)
		{		
			flashflag=0;
			clear_lcd();
			for(i=0;i<5;i++)
			{
				text_out_to_row(i,(unsigned char*)ReasonTable[cursor_index+i-4]);
			}
		}
		else if(flashflag==2)
		{
			flashflag=0;
			clear_area_to_row(cursor_row,cursor_row);
			text_out_to_row(cursor_row,(unsigned char*)ReasonTable[cursor_index+cursor_row-4]);
		}
		else if(flashflag==3)
		{
			flashflag=0;
			clear_area_to_row(cursor_row+1,cursor_row+1);
			text_out_to_row(cursor_row+1,(unsigned char*)ReasonTable[cursor_index+cursor_row+1-4]);
		}
		else if(flashflag==4)
		{
			flashflag=0;
			clear_area_to_row(cursor_row-1,cursor_row-1);
			text_out_to_row(cursor_row-1,(unsigned char*)ReasonTable[cursor_index+cursor_row-1-4]);
		}
loop:		
		flightcursor();
		keyval = kmy_GetKeyValue();
		if (keyval == KEY_NONE) continue;
		KeyEventPost();
		switch (keyval)
		{
			case KEY0:
			case KEY1:
			case KEY2:
			case KEY3:
			case KEY4:
			case KEY5:
			case KEY6:
			case KEY7:
			case KEY8:
			case KEY9:
			case KEYXING:
				
				if(strlen(ReasonTable[index])<20)
				{
					pMove++;
					PostKey (keyval);
					if(keyflag==0)
					{
						keyval = my_abcReadCode ((u8*)ReasonTable[index], (u8*)pMove,cursor_row);
					}
					else 
					{
						keyval = my_ABCReadCode ((u8*)ReasonTable[index], (u8*)pMove,cursor_row);
					}
					
					if (keyval != KEY_NONE && keyval != KEYJING)
					{
						goto loop;
					}
				}
				
				break;
			case KEYJING:
				keyflag++;
				keyflag=keyflag%2;
				
				break;
			case KEY_UP:
				
				if(index>0)
				{
					index--;
				}
				if(cursor_row==0)
				{
					if(cursor_index>4)
					{
						cursor_index--;
						flashflag=1;
					}
				}
				if(cursor_row>0)
				{
					cursor_row--;
					cursorY -=13;
					flashflag=3;
				}
				if(cursorX>(6*strlen(ReasonTable[index])))
				{
					cursorX=6*strlen(ReasonTable[index]);
				}
				if(cursorX<6)
				{
					pMove = ReasonTable[index] ;
				}
				else
				{
					pMove = ReasonTable[index] + cursorX/6;
				}
				break;
				
			case KEY_DOWN:
				if(rejectNum<15)
				{
					if((index==rejectNum)||(rejectNum<=4&&index==rejectNum-1))
					{
						if(strlen(ReasonTable[index])>0)
						{
							rejectNum++;
						}
					}
					
				}
				
				if(cursor_row==4)
				{			
					if(cursor_index<rejectNum)
					{
						if((cursor_row==rejectNum-1)&&(strlen(ReasonTable[index])==0))
						{
						}
						else
						{
							cursor_index++; 							
							flashflag=1;
						}						
					}
				}
				if(index<rejectNum)
				{
					if((cursor_row==rejectNum-1)&&(strlen(ReasonTable[index])==0))
					{

					}
					else
					{
						index++;
					}
				}
				if(cursor_row<4)
				{
					if((cursor_row==rejectNum-1)&&(strlen(ReasonTable[index])==0))
					{

					}
					else
					{
						cursor_row++;					
						cursorY +=13;
					}
										
					flashflag=4;					
				}
				if(cursorX>(6*strlen(ReasonTable[index])))
				{
					cursorX=6*strlen(ReasonTable[index]);
				}
				if(cursorX<6)
				{
					pMove = ReasonTable[index] ;
				}
				else
				{
					pMove = ReasonTable[index] + cursorX/6;
				}

				break;
				
			case KEY_LEFT:
				if(cursorX>=6)
				{
					pMove--;
					flashflag=2;
					cursorX -=6;
				}
				
				break;
				
			case KEY_RIGHT: 			
				if(cursorX<6*strlen(ReasonTable[index]))
				{				
					pMove++;
					cursorX +=6;
					flashflag=2;
				}
				
				break;
				
			case KEY_Enter:
				memset(RejectReason,0,sizeof(RejectReason));
				for(i=0;i<rejectNum;i++)
				{
					if(strlen(ReasonTable[i])>0)
					{
						strcat((char*)RejectReason,ReasonTable[i]);
						strcat((char*)RejectReason,";");
					}
				}
				return keyval;
				
			case KEY_DEL:
				if(pMove!=ReasonTable[index])
				{
					if(cursorX>=6)
					{
						pMove--;
						flashflag=2;
						cursorX -=6;
						
						my_strcpy_DeleteTailByte ((unsigned char*)pMove,(unsigned char*)pMove+1);
					}
				}
				break;
				
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
				return keyval;
				
			default:break;
		}

	}
	


}

	


void abcABC123PYWuBiArgDeInit (abcABC123PYWuBiArgument *arg, u8 *buff, u8 Size)
{
	(*arg).LocalTitle = "ÇëÊäÈë";
	(*arg).OtherTitle = "Input";
	(*arg).receive = buff;
	(*arg).recSize = Size;
	(*arg).srfFlag = SRF_123 | SRF_ABC | SRF_abc;
	(*arg).DefaultSrf = SRF_123;
	(*arg).clearlcdflag = 1;
	(*arg).HaveoldFlag = 0;
}


u8 GetString_abcABC123PYWuBi2 (abcABC123PYWuBiArgument *arg)
{
	if ( (*arg).clearlcdflag) clear_lcd();

	DrawTitle_to_row_Language2 (0, (*arg).LocalTitle, (*arg).OtherTitle);
	KeyIsResponse();

	return (_GetString_abcABC123PYWuBi ( (*arg).srfFlag, (*arg).DefaultSrf, (*arg).receive, (*arg).recSize, (*arg).HaveoldFlag) );
}


u8 GetString_abcABC123PYWuBi (const u8 *Title, u8 srfFlag, u8 DefaultSrf, u8 *receive, u8 recSize, u8 HaveoldFlag)
{
	clear_lcd();

	if (Title != NULL) DrawTitle_to_row_Language2 (0, Title, Title);

	KeyIsResponse();

	return (_GetString_abcABC123PYWuBi (srfFlag, DefaultSrf, receive, recSize, HaveoldFlag) );
}
u8 GetString_123TimeInput (const u8 *Title, u8 srfFlag, u8 DefaultSrf, u8 *receive, u8 recSize, u8 HaveoldFlag)
{
	clear_lcd();

	if (Title != NULL) DrawTitle_to_row_Language2 (0, Title, Title);

	KeyIsResponse();

	return (_GetString_123TimeInput (srfFlag, DefaultSrf, receive, recSize, HaveoldFlag) );
}

u8 GetString_abcABC123PYWuBi_Language2 (const u8 *LocalTitle, const u8 *OtherTitle, u8 srfFlag, u8 DefaultSrf, u8 *receive, u8 recSize, u8 HaveoldFlag)
{
	clear_lcd();
	DrawTitle_to_row_Language2 (0, LocalTitle, OtherTitle);
	KeyIsResponse();

	return (_GetString_abcABC123PYWuBi (srfFlag, DefaultSrf, receive, recSize, HaveoldFlag) );
}


/*´Óx  Î»ÖÃ¿ªÊ¼ÏÔÊ¾*/
static u8 _PassWordInput_XposStartDisplay (u8 *receive, u8 recSize, u8 xingflag, u8 DiplayRow, u8 Xpos, u8 argkeyval)
{
	u8 i;
	u8 inputSize;
	u8 displaybuff[22];
	u8 ClearFlag = 0;
	u8 keyval;

	inputSize = recSize > 21 ? 21 : recSize;
	memset (receive, 0, inputSize);
	i = 0;

	while (1)
	{
		if (argkeyval == KEY_NONE)
		{
			keyval = GetKeyValue();
		}
		else
		{
			keyval = argkeyval;
			argkeyval = KEY_NONE;
		}

		if (keyval == 0) continue;

		KeyEventPost();

		switch (keyval)
		{
			case KEY0:
				if (i < inputSize)
				{
					receive[i] = '0';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY1:
				if (i < inputSize)
				{
					receive[i] = '1';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY2:
				if (i < inputSize)
				{
					receive[i] = '2';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY3:
				if (i < inputSize)
				{
					receive[i] = '3';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY4:
				if (i < inputSize)
				{
					receive[i] = '4';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY5:
				if (i < inputSize)
				{
					receive[i] = '5';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY6:
				if (i < inputSize)
				{
					receive[i] = '6';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY7:
				if (i < inputSize)
				{
					receive[i] = '7';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY8:
				if (i < inputSize)
				{
					receive[i] = '8';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY9:
				if (i < inputSize)
				{
					receive[i] = '9';
					i++;
					receive[i] = 0;
				}

				break;

			case KEYXING:
				if (i < inputSize)
				{
					receive[i] = '.';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY_DEL:
				if (i > 0)
				{
					i--;
					receive[i] = 0;
					ClearFlag = 1;
				}

				break;

			case KEY_Enter:
				if (strlen ( (sc8*) receive) != 0)  	//Èç¹ûÊäÈë×Ö·ûÎª¿Õ£¬²»ÈÃ°´È·ÈÏ¼ü·µ»Ø
				{
					KeyIsResponse();
					return KEY_Enter;
				}

				break;

			case KEY_ReturnPrevious:
				KeyIsResponse();
				return KEY_ReturnPrevious;

			case KEY_ReturnInitInterface:
			case KEY_WaitTimeout:
				KeyIsResponse();
				return KEY_ReturnInitInterface;
		}

		if (ClearFlag == 1)
		{
			ClearFlag = 0;
			clear_area (Xpos, DiplayRow * ALineContainDotLine, ALinePixNum - Xpos, ALineContainDotLine);
		}

		if (xingflag)
		{
			memset (displaybuff, '*', i);
			displaybuff[i] = 0;
			text_out_to_row_x (DiplayRow, Xpos, displaybuff);
		}
		else
		{
			text_out_to_row_x (DiplayRow, Xpos, receive);
		}

		KeyIsResponse();
	}
}

/*ÖÐ¼äÏÔÊ¾*/
static u8 _PassWordInput_MiddledDisplay (u8 *receive, u8 recSize, u8 xingflag, u8 DiplayRow, u8 argkeyval)
{
	u8 i;
	u8 inputSize;
	u8 displaybuff[22];
	u8 ClearFlag = 0;
	u8 keyval;

	inputSize = recSize > 21 ? 21 : recSize;
	memset (receive, 0, inputSize);
	i = 0;

	while (1)
	{
		if (argkeyval == KEY_NONE)
		{
			keyval = GetKeyValue();
		}
		else
		{
			keyval = argkeyval;
			argkeyval = KEY_NONE;
		}

		if (keyval == 0) continue;

		KeyEventPost();

		switch (keyval)
		{
			case KEY0:
				if (i < inputSize)
				{
					receive[i] = '0';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY1:
				if (i < inputSize)
				{
					receive[i] = '1';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY2:
				if (i < inputSize)
				{
					receive[i] = '2';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY3:
				if (i < inputSize)
				{
					receive[i] = '3';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY4:
				if (i < inputSize)
				{
					receive[i] = '4';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY5:
				if (i < inputSize)
				{
					receive[i] = '5';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY6:
				if (i < inputSize)
				{
					receive[i] = '6';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY7:
				if (i < inputSize)
				{
					receive[i] = '7';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY8:
				if (i < inputSize)
				{
					receive[i] = '8';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY9:
				if (i < inputSize)
				{
					receive[i] = '9';
					i++;
					receive[i] = 0;
				}

				break;

			case KEYXING:
				if (i < inputSize)
				{
					receive[i] = '.';
					i++;
					receive[i] = 0;
				}

				break;

			case KEY_DEL:
				if (i > 0)
				{
					i--;
					receive[i] = 0;
					ClearFlag = 1;
				}

				break;

			case KEY_Enter:
				if (strlen ( (sc8*) receive) != 0)  	//Èç¹ûÊäÈë×Ö·ûÎª¿Õ£¬²»ÈÃ°´È·ÈÏ¼ü·µ»Ø
				{
					KeyIsResponse();
					return KEY_Enter;
				}

				break;

			case KEY_ReturnPrevious:
				KeyIsResponse();
				return KEY_ReturnPrevious;

			case KEY_ReturnInitInterface:
			case KEY_WaitTimeout:
				KeyIsResponse();
				return KEY_ReturnInitInterface;
		}

		if (ClearFlag == 1)
		{
			ClearFlag = 0;
			clear_area_to_row (DiplayRow, DiplayRow);
		}

		if (xingflag)
		{
			memset (displaybuff, '*', i);
			displaybuff[i] = 0;
			DrawTitle_to_row (DiplayRow, displaybuff);
		}
		else
		{
			DrawTitle_to_row (DiplayRow, receive);
		}

		KeyIsResponse();
	}
}



//Ä¬ÈÏÃÜÂëÊäÈëÎªÇåÆÁ¡¢ÊäÈë×Ö·ûÏÔÊ¾ÔÚµÚ¶þÐÐ¡¢
//±êÌâÖÐÎÄÎª"ÇëÊäÈëÃÜÂë"Input Password"¡¢²»ÒýÈë°´¼ü
//½ÓÊÕbuff ÎªÐÎ²Î2£¬½ÓÊÜ×Ö½ÚÎªÐÎ²Î3¡¢ÒÔ*  ÏÔÊ¾ÊäÈëµÄ×Ö·û
void PassWordArgDeInit (PasswordInputArgument *arg, u8 *buff, u8 Size)
{
	(*arg).clearlcdflag = 1;
	(*arg).DiplayRow = 1;
	(*arg).LocalTitle = "ÇëÊäÈëÃÜÂë";
	(*arg).OtherTitle = "Input Password";
	(*arg).keyval = KEY_NONE;
	(*arg).receive = buff;
	(*arg).recSize = Size;
	(*arg).xingflag = 1;
}

/*motice:Title¡¢receive¡¢recSize  max value is 20*/
u8 PassWordInput (PasswordInputArgument *arg)
{
	if ( (*arg).clearlcdflag) clear_lcd();

	DrawTitle_to_row_Language2 (0, (*arg).LocalTitle, (*arg).OtherTitle);
	KeyIsResponse();

	return (_PassWordInput_XposStartDisplay ( (*arg).receive, (*arg).recSize, (*arg).xingflag, (*arg).DiplayRow, 0, (*arg).keyval) );
}


u8 PassWordInput_MiddledDisplay (PasswordInputArgument *arg)
{
	if ( (*arg).clearlcdflag) clear_lcd();

	DrawTitle_to_row_Language2 (0, (*arg).LocalTitle, (*arg).OtherTitle);
	KeyIsResponse();

	return (_PassWordInput_MiddledDisplay ( (*arg).receive, (*arg).recSize, (*arg).xingflag, (*arg).DiplayRow, (*arg).keyval) );
}

u8 PassWordInput_XposStartDisplay (PasswordInputArgument *arg, u8 Xpos)
{
	if ( (*arg).clearlcdflag) clear_lcd();

	DrawTitle_to_row_Language2 (0, (*arg).LocalTitle, (*arg).OtherTitle);
	KeyIsResponse();

	return (_PassWordInput_XposStartDisplay ( (*arg).receive, (*arg).recSize, (*arg).xingflag, (*arg).DiplayRow, Xpos, (*arg).keyval) );
}


u8 PassWordInput2 (const u8 *Title, u8 *receive, u8 recSize, u8 xingflag, u8 clearlcdflag, u8 keyval, u8 DiplayRow)
{
	if (clearlcdflag) clear_lcd();

	if (Title != NULL) DrawTitle_to_row_Language2 (0, Title, Title);

	KeyIsResponse();

	return (_PassWordInput_XposStartDisplay (receive, recSize, xingflag, DiplayRow, 0, keyval) );
}


