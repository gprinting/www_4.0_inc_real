<?
define("INC_PATH", $_SERVER["INC"]);

/* adodb 캐쉬저장경로 설정 */
//$ADODB_CACHE_DIR = dirname(__FILE__) . '/cache';
//define("ADODB_CACHE_DIR", $ADODB_CACHE_DIR);

define("NO_IMAGE", "/design_template/images/no_image.jpg");
define("NO_IMAGE_THUMB", "/design_template/images/no_image_75_75.jpg");

define("TAX_RATE", 1.1);

define("SUB_BANNER_IMAGE_ARR", [
     "001001" => "banner_order_ad_01"
    ,"001002" => "banner_order_ad_02"
    ,"001003" => "banner_order_ad_03"
    ,"001004" => "banner_order_ad_04"
    ,"002001001" => "banner_order_ao_04"
    ,"002002001" => "banner_order_ao_01"
    ,"002002002" => "banner_order_ao_03"
    ,"002002003" => "banner_order_ao_02"
    ,"002002004" => "banner_order_ao_08"
    ,"002003001" => "banner_order_ao_10"
    ,"002004001" => "banner_order_ao_09"
    ,"002005001" => "banner_order_ao_06"
    ,"002005002" => "banner_order_ao_07"
    ,"002005003" => "banner_order_ao_05"
    ,"002006" => "banner_order_ao_04"
    ,"003001" => "banner_order_nc_01"
    ,"003002" => "banner_order_nc_02"
    ,"003003" => "banner_order_nc_03"
    ,"004001" => "banner_order_st_01"
    ,"004002" => "banner_order_st_02"
    ,"004003" => "banner_order_st_03"
    ,"004003009" => "banner_order_st_04"
    ,"005001" => "banner_order_bl_01"
    ,"005002" => "banner_order_bl_02"
    ,"005003" => "banner_order_bl_03"
    ,"006001" => "banner_order_ev_01"
    ,"006002" => "banner_order_ev_02"
    ,"007001" => "banner_order_mt_01"
    ,"007002" => "banner_order_mt_02"
    ,"008001001" => "banner_order_etc_01"
    ,"008001002" => "banner_order_etc_02"
    ,"008001003" => "banner_order_etc_03"
    ,"008001004" => "banner_order_etc_03"
    ,"008001005" => "banner_order_etc_04"
    ,"008001006" => "banner_order_etc_06"
    ,"008002" => "banner_order_etc_05"
    ,"009001001" => "banner_order_gb_01"
    ,"009001002" => "banner_order_gb_02"
    ,"010001001" => "banner_order_esti_01"
    ,"010001002" => "banner_order_esti_02"
    ,"011001001" => "banner_order_digital_01"
    ,"011002001" => "banner_order_digital_02"
    ,"011002002" => "banner_order_digital_03"
    ,"011003001" => "banner_order_digital_04"
]);

/******************** 파일 저장 경로 ****************/
include_once(INC_PATH . "/common_define/common_config.inc");
?>
