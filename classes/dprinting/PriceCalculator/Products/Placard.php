<?php
/***********************************************************************************
 *** 프로 젝트 : 3.0
 *** 개발 영역 : 떡메모지
 *** 개  발  자 : 조현식
 *** 개발 날짜 : 2016.06.30
 *** 세부 사항 : 같은 폴더내 product클래스에서 파생된 클래스
 ***********************************************************************************/

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Product.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/ActualPriceUtil.inc");
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/FrontCommonUtil.inc");

class Placard extends Product implements PrintoutInterface
{
    /**
     * @var string
     */
    var $amt;
    var $size_w, $size_h;
    var $papers;
    var $paper_mpcode;
    var $stan_mpcode;
    var $options;
    var $bef_print_mpcode;
    var $afterprocesses;

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
        $this->size_w = $param["cut_wid_size"];
        $this->size_h = $param["cut_vert_size"];
        $this->amt = $param["amt"];

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
        $actualUtil = new ActualPriceUtil();
        $util = new FrontCommonUtil();
        $sell_price  = $actualUtil->calcPrice($this->size_w, $this->size_h) * $this->amt * $this->discount_rate;

        $result = [];
        $result["sell_price"] = $sell_price;
        $result["print"] = $sell_price;
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