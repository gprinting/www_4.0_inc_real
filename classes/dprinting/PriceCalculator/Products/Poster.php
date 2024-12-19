<?php
/***********************************************************************************
 *** 프로 젝트 : 3.0
 *** 개발 영역 : 포스터
 *** 개  발  자 : 조현식
 *** 개발 날짜 : 2016.06.30
 ***********************************************************************************/
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Product.php');

class Poster extends Product implements PrintoutInterface
{
    /**
     * @var string
     */
    var $amt;
    var $size_w, $size_h;
    var $papers;
    var $options;
    var $afterprocesses;

    function makeHtml() {
        $html = '<h2>전단주문</h2></br></br>';
        $html .= $this->makeBasicOption(0);
        $html .= $this->makeOptOption();
        $html .= $this->makeAfterOption();
        $html .= $this->makeCommonInfo();
        return $html;
    }

    /***********************************************************************************
     *** 사이즈명, 매수, 건수를 통해 연단위로 환산
     ***********************************************************************************/

    function tranlateYeon($sizename, $amt, $count) { //사이즈명, 매수, 건수
        $yeon = 0;
        switch($sizename) {
            case 'A1' :
                $yeon = $amt / 500 * $count;
                break;
            case 'A2' :
                $yeon = $amt / 1000 * $count;
                break;
            case 'A3' :
                $yeon = $amt / 2000 * $count;
                break;
            case 'A4' :
                $yeon = $amt / 4000 * $count;
                break;
            case 'A5' :
                $yeon = $amt / 8000 * $count;
                break;
            case 'A6' :
                $yeon = $amt / 16000 * $count;
                break;
            case '2절' :
                $yeon = $amt / 500 * $count;
                break;
            case '4절' :
                $yeon = $amt / 1000 * $count;
                break;
            case '8절' :
                $yeon = $amt / 2000 * $count;
                break;
            case '16절' :
                $yeon = $amt / 4000 * $count;
                break;
            case '32절' :
                $yeon = $amt / 8000 * $count;
                break;
            case '64절' :
                $yeon = $amt / 16000 * $count;
                break;
        }
        return ceil($yeon);
    }

    function makeBizhowsOrderCommonInsertParam($fb) {
        parent::makeBizhowsOrderCommonInsertParam($fb);
        $this->order_common_param['add_after_price'] = 0;
        $this->order_common_param["dlvr_dvs"]  = "leaflet";
        $cate_name = "포스터";
        switch ($fb["Paper_Code"]) {
            case "AT/B/100" :
                $paper_name = "아트지 백색 100g";
                break;
            case "AT/B/150" :
                $paper_name = "아트지 백색 150g";
                break;
            case "SW/B/150" :
                $paper_name = "스노우지 백색 150g";
                break;
            case "MO/B/150" :
                $paper_name = "모조지 백색 150g";
                break;
        }

        switch ($fb["Side_Code"]) {
            case "S" :
                $tmpt_str = "단면4도";
                break;
            case "D" :
                $tmpt_str = "양면8도";
                break;
        }


        $this->order_common_param["order_detail"] = sprintf("%s / %s / %s / %s", $cate_name
            , $paper_name
            , $fb["CutSizeW"] . "*" . $fb["CutSizeH"]
            , $tmpt_str);
    }

    function makeBizhowsOrderDetailInsertParam($fb) {
        parent::makeBizhowsOrderDetailInsertParam($fb);
        $this->order_detail_param["cate_aft_print_mpcode"] = 0;
        $this->order_detail_param["cate_bef_add_print_mpcode"] = 0;
        $this->order_detail_param["cate_aft_add_print_mpcode"] = 0;

        $this->order_detail_param["print_purp_dvs"] = "일반옵셋";

        if($fb["Standard"] == "SEL") {
            $this->order_detail_param["stan_name"] = $fb["CutSizeW"] . "*" . $fb["CutSizeH"];
        } else {
            $this->order_detail_param["stan_name"] = "비규격";
        }

        switch ($fb["Paper_Code"]) {
            case "AT/B/100" :
                $this->order_detail_param["cate_paper_mpcode"] = 135;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 135;
                break;
            case "AT/B/150" :
                $this->order_detail_param["cate_paper_mpcode"] = 132;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 132;
                break;
            case "SW/B/150" :
                $this->order_detail_param["cate_paper_mpcode"] = 150;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 150;
                break;
            case "MO/B/150" :
                $this->order_detail_param["cate_paper_mpcode"] = 141;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 141;
                break;
        }

        switch ($fb["Paper_Code"]) {
            case "S" :
                $this->order_detail_param["cate_bef_print_mpcode"] = 62;
                break;
            case "D" :
                $this->order_detail_param["cate_bef_print_mpcode"] = 253;
                break;
        }


    }

    function cost() {
        $param = [];
        $param["table_name"]    = "ply_price";
        $param["cate_sortcode"] = $this->sortcode;
        $param["paper_mpcode"]  = $this->paper_mpcode;
        $param["bef_print_mpcode"]     = $this->bef_print_mpcode;
        $param["aft_print_mpcode"]     = "";
        $param["bef_add_print_mpcode"] = "";
        $param["aft_add_print_mpcode"] = "";
        $param["stan_mpcode"]   = $this->stan_mpcode;
        $param["amt"]           = $this->amt;
        $rs = $this->dao->selectPrdtPlyPrice($this->conn, $param);
        $this->jarisu = ceil($this->jarisu);

        $price = ceil($rs["new_price"] * $this->discount_rate * $this->count);

        $result = [];
        $result["sell_price"] = $price;
        $result["print"] = $price;
        $result["jarisu"] = $this->jarisu;
        return $result;
    }

    function getSort() {
        return "leaflet";
    }
}

?>
