<?php
/***********************************************************************************
 *** 프로 젝트 : 3.0
 *** 개발 영역 : 스티커클래스
 *** 개  발  자 : 조현식
 *** 개발 날짜 : 2016.07.01
 *** 세부 사항 : 같은 폴더내 product클래스에서 파생된 클래스
 ***********************************************************************************/
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Product.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/NonStandardUtil.inc");

class FreeTomsonSticker extends Product implements PrintoutInterface
{
    /**
     * @var string
     */
    var $amt;
    var $size_w, $size_h;
    var $tomson_size_w, $tomson_size_h;
    var $papers;
    var $stan_mpcode;
    var $options;
    var $afterprocesses;
    var $paper_name;
    var $stan_name;
    var $st_bg;
    var $product_margin;
    var $count;
    var $amt_ts;
    var $amt_f1;
    var $amt_f2;
    var $amt_f3;
    var $amt_f4;

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
        $this->tomson_size_w = $param["tomson_wid_size"];
        $this->tomson_size_h = $param["tomson_vert_size"];
        $this->amt = $param["amt"];
        $this->paper_mpcode = $param["paper"];
        $this->count = $param["count"];
        $this->paper_name = $param["paper_info"];
        $this->st_bg = $param["st_bg"];
        $this->product_margin = $param["product_margin"];
        $this->product_margin = 210;
        //$this->product_margin = 75;
        $this->stan_name = $param["size"];
        $this->amt_ts = $param["amt_ts"];
        $this->amt_f1 = $param["amt_f1"];
        $this->amt_f2 = $param["amt_f2"];
        $this->amt_f3 = $param["amt_f3"];
        $this->amt_f4 = $param["amt_f4"];
        $this->member_seqno = $param["member_seqno"];

        $after_name = explode("|", $param["after_name"]);
        $aft_depth = explode("|", $param["aft_depth"]);
        $aft_depth1 = explode("|", $param["aft_depth1"]);
        $aft_depth2 = explode("|", $param["aft_depth2"]);
        $aft_depth3 = explode("|", $param["aft_depth3"]);
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
            $arr_depth[$i]['aft_depth3'] = $aft_depth3[$i];
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
        $param = [];
        $param["cate_sortcode"] = $this->sortcode;
        $param["st_bg"] = $this->st_bg;
        $param["product_margin"] = $this->product_margin;
        $param["size_width"] = $param["wid"]   = $this->size_w;
        $param["size_vert"] = $param["vert"]  = $this->size_h;
        $param["tomson_wid"]   = $this->tomson_size_w;
        $param["tomson_vert"]  = $this->tomson_size_h;
        $param["amt"]   = $this->amt;
        $param["paper_name"] = $this->paper_name;
        $param["stan_mpcode"] = $this->stan_mpcode;
        $param["paper_mpcode"] = $this->paper_mpcode;
        $param["stan_name"] = $this->st_bg;
        $param["amt_ts"] = $this->amt_ts;
        $param["amt_f1"] = $this->amt_f1;
        $param["amt_f2"] = $this->amt_f2;
        $param["amt_f3"] = $this->amt_f3;
        $param["amt_f4"] = $this->amt_f4;

        $util = new AftPriceUtil(["conn" => $this->conn, "dao" => $this->dao]);
        if($this->member_seqno == 5610 || $this->sortcode == "004003010") {
            $nsUtil = new NonStandardUtil();
            $calc_price = $nsUtil->stCalcPriceNew($param);
        } else {
            $nsUtil = new NonStandardUtil();
            $calc_price = $nsUtil->stCalcPriceNew($param);
            //$calc_price = $util->getFreeTomsonPrice_temp($param);
        }

        $price = ceil($calc_price * $this->discount_rate * $this->count) * 1.1;

        $result = [];
        $result["sell_price"] = $price;
        $result["print"] = $price;
        $result["jarisu"] = $this->jarisu;
        return $result;
    }

    function getSort() {
        return "namecard";
    }

    function makeOrderCommonInsertParam($fb, $session) {
        parent::makeOrderCommonInsertParam($fb, $session);
    }

    function makeOrderDetailInsertParam($fb, $session) {
        parent::makeOrderDetailInsertParam($fb, $session);
        $this->order_detail_param['tomson_yn'] = "Y";
    }

    function makeOrderAfterHistoryInsertParam($fb) {
        parent::makeOrderAfterHistoryInsertParam($fb);
        if($this->order_detail_param["cate_paper_mpcode"] == "223"
            || $this->order_detail_param["cate_paper_mpcode"] == "229"
            || $this->order_detail_param["cate_paper_mpcode"] == "237") {
            if (strpos($this->order_common_param["order_detail"], "무코팅") === false) {
                $ret = array();
                $ret['mpcode'] = "3062";
                $ret['after_name']   = "코팅";
                $ret['depth1'] = "유광";
                $ret['depth2'] = "단면";
                $ret['depth3'] = "";
                $ret['basic_yn'] = "Y";
                $ret['before_cutting_yn'] = "Y";
                array_push($this->order_after_history_param, $ret);
            }
        }
    }
}

?>