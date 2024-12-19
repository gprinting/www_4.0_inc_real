<?php
/***********************************************************************************
 *** 프로 젝트 : 3.0
 *** 개발 영역 : 독판전단
 *** 개  발  자 : 조현식
 *** 개발 날짜 : 2016.06.30
 *** 세부 사항 : 같은 폴더내 product클래스에서 파생된 클래스
 ***********************************************************************************/

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Product.php');
include_once($_SERVER["INC"] . "/common_lib/CalcPriceUtil.inc");
include_once($_SERVER["INC"] . "/common_dao/ProductCommonDAO.inc");

class Booklet_Mono extends Booklet implements PrintoutInterface
{
    var $sell_site;
    var $amt_unit;
    var $flattyp_yn;
    var $page_info;
    var $aft_print_mpcode;
    var $bef_add_print_mpcode;
    var $aft_add_print_mpcode;
    var $bef_print_name;
    var $aft_print_name;
    var $bef_add_print_name;
    var $aft_add_print_name;
    var $affil;
    var $size_name;

    function makeHtml() {
        $html = '<h2>명함</h2></br></br>';
        //표지
        $html .= '-----------표지-----------</br>';
        $html .= $this->makeBasicOption(0);
        $html .= $this->makeOptOption();
        $html .= $this->makeAfterOption();
        $html .= $this->makeCommonInfo();
        return $html;
    }



    function makeOrderCommonInsertParam($fb, $session) {
        parent::makeOrderCommonInsertParam($fb, $session);
        $this->order_common_param["amt"] = $fb["bl_r_count"];
        $this->order_common_param["amt_unit_dvs"] = "R";
    }

    function setInfo($param)
    {
        $this->flattyp_yn = $param["flattyp_yn"];
        $this->amt_unit = $param["amt_unit"];
        $this->aft_print_mpcode = $param["aft_tmpt"];
        $this->bef_add_print_mpcode = 0;
        $this->aft_add_print_mpcode = 0;
        $this->bef_print_name = $param["bef_print_name"];
        $this->aft_print_name = $param["aft_print_name"];
        $this->bef_add_print_name = $param["bef_add_print_name"];
        $this->aft_add_print_name = $param["aft_add_print_name"];
        $this->pos_num = $param["pos_num"];
        $this->page_info = $param["page"];
        $this->size_name = $param["size_name"];
        $this->affil = $param["affil"];
        return parent::setInfo($param);
    }

    function cost() {
        $temp = [];
        $temp["sell_site"]     = $this->sell_site;
        $temp["cate_sortcode"] = $this->sortcode;
        $temp["amt_unit"]      = $this->amt_unit;
        $temp["flattyp_yn"]    = $this->flattyp_yn;

        $temp["amt"]     = $this->amt;
        $temp["page"]    = $this->page_info;
        $temp["pos_num"] = $this->pos_num;
        $temp["affil"]   = $this->affil;

        $temp["paper_mpcode"]  = $this->paper_mpcode;
        $temp["cate_output_mpcode"] = $this->stan_mpcode;

        $temp["bef_print_name"]     = $this->bef_print_name;
        $temp["aft_print_name"]     = $this->aft_print_name;

        $temp["bef_print_mpcode"]     = $this->bef_print_mpcode;
        $temp["aft_print_mpcode"]     = $this->aft_print_mpcode;
        $temp["bef_add_print_mpcode"] = $this->bef_add_print_mpcode;
        $temp["aft_add_print_mpcode"] = $this->aft_add_print_mpcode;

        $calc_util = new CalcPriceUtil($temp);

        unset($temp);
        $temp["bef_print_name"] = $this->bef_print_name;
        $temp["aft_print_name"] = $this->aft_print_name;
        $temp["bef_add_print_name"] = $this->print_bef_add_name;
        $temp["aft_add_print_name"] = $this->print_aft_add_name;

        $util = new FrontCommonUtil();
        $paper_price  = $util->ceilVal($calc_util->calcPaperPrice($temp)) * $this->discount_rate;
        $print_price  = $util->ceilVal($calc_util->calcSheetPrintPrice()) * $this->discount_rate;
        $output_price = $util->ceilVal($calc_util->calcSheetOutputPrice()) * $this->discount_rate;

        $paper_price = round($paper_price / 1.1 / 100) * 100 * 1.1;

        $price   = ($paper_price + $print_price + $output_price) * $this->count;
        $result = [];
        $result["sell_price"] = $price;
        $result["jarisu"] = $this->jarisu;
        $result["print"] = $print_price;
        $result["paper"] = $paper_price;
        $result["output"] = $output_price;


        return $result;
    }

    function getJarisu() {
        return $this->jarisu;
    }
    function getSort() {
        return "leaflet";
    }
}

?>