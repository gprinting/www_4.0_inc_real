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

class Envelope extends Product implements PrintoutInterface
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
    var $options;

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
        $this->amt = $param["amt"];
        $this->count = $param["count"];
        $this->paper_mpcode = $param["paper"];
        $this->stan_mpcode = $param["size"];
        $this->bef_print_mpcode = $param["bef_tmpt"];
        $this->page_info = $param["page_info"];

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

        $option_name = explode("|", $param["option_name"]);
        $opt_depth1 = explode("|", $param["opt_depth1"]);
        $opt_depth2 = explode("|", $param["opt_depth2"]);
        $opt_depth3 = explode("|", $param["opt_depth3"]);
        $opt_mpcode = explode("|", $param["opt_mp_list"]);
        $opt_cnt = explode("|", $param["opt_cnt"]);

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

        $option_count = count($option_name);
        for($i = 0; $i < $option_count ; $i++) {
            if($option_name[$i] == '') {
                break;
            }
            $product = $factory->createOP($option_name[$i], $product);
            $arr_depth = [];
            $arr_depth[$i]['option_name'] = $option_name[$i];
            $arr_depth[$i]['opt_depth1'] = $opt_depth1[$i];
            $arr_depth[$i]['opt_depth2'] = $opt_depth2[$i];
            $arr_depth[$i]['opt_depth3'] = $opt_depth3[$i];
            $arr_depth[$i]['opt_cnt'] = $opt_cnt[$i];
            $arr_depth[$i]['opt_mp_list'] = $opt_mpcode[$i];
            $arr_depth[$i]['expect_box'] = $param["expect_box_num"];
            $arr_depth[$i]['affil'] = $param["affil"];
            $arr_depth[$i]['paper_mpcode'] = $param["paper"];
            $arr_depth[$i]['paper_info'] = $param["paper_info"];
            $product->setOption($this->sortcode, $arr_depth[$i], $opt_mpcode[$i], $this->amt); // $sortcode ,$after_name, $mpcode, $amt = ''
        }

        $product = $factory->createDC(null, $product);
        $product->grade = $param[grade];
        return $product;
    }

    function cost() {
        $param = [];
        $param["table_name"]    = "ply_price";
        $param["cate_sortcode"] = $this->sortcode;
        $param["paper_mpcode"]  = $this->paper_mpcode;
        $param["bef_print_mpcode"]     = $this->bef_print_mpcode;
        $param["aft_print_mpcode"]     = "0";
        $param["bef_add_print_mpcode"] = "0";
        $param["aft_add_print_mpcode"] = "0";
        $param["stan_mpcode"]   = $this->stan_mpcode;
        $param["amt"]           = $this->amt;

        $rs = $this->dao->selectPrdtPlyPrice($this->conn, $param);

        $price = ceil($rs["new_price"] * $this->discount_rate * $this->count);

        $result = [];
        $result["sell_price"] = $price;
        $result["print"] = $price;
        return $result;
    }

    function getJarisu() {
        return $this->jarisu;
    }
    function getSort() {
      //return "leaflet";   
      return "envelope"; 
    }

    function makeBizhowsOrderCommonInsertParam($fb) {
        parent::makeBizhowsOrderCommonInsertParam($fb);
        $this->order_common_param["dlvr_dvs"]  = "leaflet";
        switch ($fb["Paper_Code"]) {
            case "MO/B/100" :
                $paper_name = "모조지 백색 100g";
                break;
            case "MO/B/120" :
                $paper_name = "모조지 백색 120g";
                break;
            case "MO/B/150" :
                $paper_name = "모조지 백색 150g";
                break;
            case "L4/B/110" :
                $paper_name = "레자크 #91(체크) 110g";
                break;
            case "L5/B/110" :
                $paper_name = "레자크 #92(줄) 110g";
                break;
        }

        switch ($fb["Side_Code"]) {
            case "S" :
                $tmpt_str = "단면칼라4도";
                break;
            case "D" :
                $tmpt_str = "양면칼라8도";
                break;
        }

        switch ($fb["S_Code"]) {
            case "EC1" :
                $cate_name = "일반형봉투";
                $size_name =  "330*245(5절)";
                break;
            case "EC4" :
                $cate_name = "일반형봉투";
                $size_name =  "220*105(A4다대)";
                break;
            case "EC5" :
                $cate_name = "자켓형봉투";
                $size_name =  "175*85(티켓)";
                break;
            case "EC7" :
                $cate_name = "자켓형봉투";
                $size_name =  "220*105(A4자켓)";
                break;
        }

        $this->order_common_param["order_detail"] = sprintf("%s / %s / %s / %s", $cate_name
            , $paper_name
            , $size_name
            , $tmpt_str);
    }

    function makeBizhowsOrderDetailInsertParam($fb) {
        parent::makeBizhowsOrderDetailInsertParam($fb);

        $this->order_detail_param["work_size_wid"] = $fb["CutSizeW"];
        $this->order_detail_param["work_size_vert"] = $fb["CutSizeH"];
        $this->order_detail_param["cut_size_wid"] = $fb["EditSizeW"];
        $this->order_detail_param["cut_size_vert"] = $fb["EditSizeH"];
        $this->order_detail_param["receipt_size_wid"] = $fb["CutSizeW"];
        $this->order_detail_param["receipt_size_vert"] = $fb["CutSizeH"];

        $this->order_detail_param["cate_aft_print_mpcode"] = 0;
        $this->order_detail_param["cate_bef_add_print_mpcode"] = 0;
        $this->order_detail_param["cate_aft_add_print_mpcode"] = 0;

        $this->order_detail_param["print_purp_dvs"] = "일반옵셋";
        $this->order_detail_param["tot_tmpt"] = 4;
        switch ($fb["S_Code"]) {
            case "EC1" :
                $this->order_detail_param["cate_bef_print_mpcode"] = 97;
                $this->order_detail_param["stan_name"] =  "330*245(5절)";
                break;
            case "EC4" :
                $this->order_detail_param["cate_bef_print_mpcode"] = 97;
                $this->order_detail_param["stan_name"] =  "220*105(A4다대)";
                break;
            case "EC5" :
                $this->order_detail_param["cate_bef_print_mpcode"] = 125;
                $this->order_detail_param["stan_name"] =  "175*85(티켓)";
                break;
            case "EC7" :
                $this->order_detail_param["cate_bef_print_mpcode"] = 125;
                $this->order_detail_param["stan_name"] =  "220*105(A4자켓)";
                break;
        }

        switch ($fb["Paper_Code"]) {
            case "MO/B/100" :
                $this->order_detail_param["cate_paper_mpcode"] = 946;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 946;
                break;
            case "MO/B/120" :
                $this->order_detail_param["cate_paper_mpcode"] = 947;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 947;
                break;
            case "MO/B/150" :
                $this->order_detail_param["cate_paper_mpcode"] = 955;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 955;
                break;
            case "L4/B/110" :
                $this->order_detail_param["cate_paper_mpcode"] = 952;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 952;
                break;
            case "L5/B/110" :
                $this->order_detail_param["cate_paper_mpcode"] = 953;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 953;
                break;
        }
    }
}

?>
