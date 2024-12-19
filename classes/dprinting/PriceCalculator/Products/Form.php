<?php
/***********************************************************************************
 *** 프로 젝트 : 3.0
 *** 개발 영역 : 명함클래스
 *** 개  발  자 : 조현식
 *** 개발 날짜 : 2016.06.30
 *** 세부 사항 : 같은 폴더내 product클래스에서 파생된 클래스
 ***********************************************************************************/

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Product.php');
include_once($_SERVER["INC"] .'/common_define/prdt_default_info.inc');

class Form extends Product implements PrintoutInterface
{
    /**
     * @var string
     */
    var $amt;
    var $unit_amt;
    var $size_w, $size_h;
    var $papers;
    var $paper_mpcode;
    var $stan_mpcode;
    var $options;
    var $bef_print_mpcode;
    var $afterprocesses;
    var $ssize1 = 92;
    var $ssize2 = 54;
    var $cut_room = 2;

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

    function setInfo($param)
    {
        $factory = new DPrintingFactory();
        $this->dvs    = $param["dvs"];
        $this->flattyp_yn    = $param["flattyp_yn"];
        $this->amt_unit    = $param["amt_unit"];
        $this->diff_yn    = $param["diff_yn"];

        $this->paper_mpcode    = $param["paper"];
        $this->paper_mpcode_arr    = $param["paper"];
        $this->stan_mpcode    = $param["size"];
        $this->bef_print_mpcode    = $param["bef_tmpt"];
        $this->aft_print_mpcode    = $param["aft_print_mpcode"];
        $this->bef_add_print_mpcode    = $param["bef_add_print_mpcode"];
        $this->aft_add_print_mpcode    = $param["aft_add_print_mpcode"];
        $this->bef_print_name    = $param["bef_print_name"];
        $this->aft_print_name    = $param["aft_print_name"];
        $this->bef_add_print_name    = $param["bef_add_print_name"];
        $this->aft_add_print_name    = $param["aft_add_print_name"];
        $this->cate_sortcode    = $param["cate_sortcode"];

        $this->amt = $param["amt"];

        $sortcode_m = substr($this->cate_sortcode, 0, 6);
        if ($sortcode_m === "007001") {
            $this->unit_amt = PrdtDefaultInfo::MST_GROUP * intval($this->amt);
        }

        if ($sortcode_m === "007002") {
            $this->unit_amt = PrdtDefaultInfo::FORM_GROUP * intval($this->amt);
        }

        //$this->unit_amt = PrdtDefaultInfo::MST_GROUP * intval($param["amt"]);
        $this->count = $param["count"];
        $this->affil    = $param["affil"];
        $this->pos_num    = $param["pos_num"];

        $page_info    = explode('!', $param["page_info"]);
        $this->page         = $page_info[0];
        $this->page_detail  = $page_info[1];

        $after_name = explode("|", $param["after_name"]);
        $aft_depth = explode("|", $param["aft_depth"]);
        $aft_depth1 = explode("|", $param["aft_depth1"]);
        $aft_depth2 = explode("|", $param["aft_depth2"]);
        $aft_depth1_val = explode("|", $param["aft_depth1_val"]);
        $aft_depth1_vh = explode("|", $param["aft_depth1_vh"]);
        $aft_depth1_cnt = explode("|", $param["aft_depth1_cnt"]);
        $aft_depth1_dvs = explode("|", $param["aft_depth1_dvs"]);
        $aft_depth2_dvs = explode("|", $param["aft_depth2_dvs"]);
        $aft_depth1_wid = explode("|", $param["aft_depth1_wid"]);
        $aft_depth1_vert = explode("|", $param["aft_depth1_vert"]);
        $aft_depth2_wid = explode("|", $param["aft_depth2_wid"]);
        $aft_depth2_vert = explode("|", $param["aft_depth2_vert"]);
        $aft_depth1_amt = explode("|", $param["aft_depth1_amt"]);

        $after_count = count($after_name);
        $product = $this;
        for($i = 0; $i < $after_count ; $i++) {
            if($after_name[$i] == '') {
                break;
            }
            $product = $factory->createAP($after_name[$i], $product);
            $arr_depth = [];
            $arr_depth[$i]['aft'] = $after_name[$i];
            $arr_depth[$i]['after_name'] = $after_name[$i];
            $arr_depth[$i]['aft_depth'] = $aft_depth[$i];
            $arr_depth[$i]['aft_depth1'] = $aft_depth1[$i];
            $arr_depth[$i]['aft_depth2'] = $aft_depth2[$i];
            $arr_depth[$i]['aft_depth1_val'] = $aft_depth1_val[$i];
            $arr_depth[$i]['aft_depth1_vh'] = $aft_depth1_vh[$i];
            $arr_depth[$i]['aft_depth1_cnt'] = $aft_depth1_cnt[$i];
            $arr_depth[$i]['aft_depth1_dvs'] = $aft_depth1_dvs[$i];
            $arr_depth[$i]['aft_depth2_dvs'] = $aft_depth2_dvs[$i];
            $arr_depth[$i]['aft_depth1_wid'] = $aft_depth1_wid[$i];
            $arr_depth[$i]['aft_depth1_vert'] = $aft_depth1_vert[$i];
            $arr_depth[$i]['aft_depth2_wid'] = $aft_depth2_wid[$i];
            $arr_depth[$i]['aft_depth2_vert'] = $aft_depth2_vert[$i];
            $arr_depth[$i]['aft_depth1_amt'] = $aft_depth1_amt[$i];
            $product->setAfterprocess($this->sortcode, $arr_depth[$i], $this->amt); // $sortcode ,$after_name, $mpcode, $amt = ''
        }

        $product = $factory->createDC(null, $product);
        $product->grade = $param[grade];

        return $product;
    }

    function cost() {
        $util = new FrontCommonUtil();

        $temp = array();
        $temp["sell_site"]     = $this->sell_site;
        $temp["cate_sortcode"] = $this->sortcode;
        $temp["amt_unit"]      = $this->amt_unit;
        $temp["flattyp_yn"]    = $this->flattyp_yn;

        $temp["amt"]     = $this->unit_amt;
        $temp["page"]    = $this->page;
        $temp["pos_num"] = $this->pos_num;
        $temp["affil"]   = $this->affil;

        $temp["cate_output_mpcode"] = $this->stan_mpcode;

        $temp["paper_mpcode"] = $this->paper_mpcode;
        $temp["bef_print_mpcode"]     = $this->bef_print_mpcode;
        $temp["aft_print_mpcode"]     = $this->aft_print_mpcode;
        $temp["bef_add_print_mpcode"] = $this->bef_add_print_mpcode;
        $temp["aft_add_print_mpcode"] = $this->aft_add_print_mpcode;

        $calc_util = new CalcPriceUtil($temp);
        unset($temp);
        $temp["bef_print_name"] = $this->bef_print_name;
        $temp["aft_print_name"] = $this->aft_print_name;
        $temp["bef_add_print_name"] = $this->bef_add_print_name;
        $temp["aft_add_print_name"] = $this->aft_add_print_name;

        $paper_price  = $util->ceilVal($calc_util->calcPaperPrice($temp)) * 2.5 * $this->discount_rate;
        $print_price  = $util->ceilVal($calc_util->calcSheetPrintPrice()) * $this->discount_rate;
        $output_price = $util->ceilVal($calc_util->calcSheetOutputPrice()) * $this->discount_rate;

        $sell_price = $paper_price + $print_price + $output_price;
        //$sell_price *= 1.1;
        $sell_price = $util->ceilVal($sell_price);
        $result = [];
        $result["sell_price"] = ceil($sell_price * $this->count);
        $result["paper"] = ceil($paper_price * $this->count);
        $result["print"] = ceil($print_price * $this->count);
        $result["output"] = ceil($output_price * $this->count);
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
