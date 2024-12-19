<?
/* adodb 캐쉬저장경로 설정 */
$ADODB_CACHE_DIR = dirname(__FILE__) . '/cache';
define("ADODB_CACHE_DIR", $ADODB_CACHE_DIR);

/******************** 파일 저장 경로 ****************/
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . "/common_define/common_config.inc");
?>
