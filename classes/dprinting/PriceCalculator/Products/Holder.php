<?php
/***********************************************************************************
 *** 프로 젝트 : 3.0
 *** 개발 영역 : 포스터
 *** 개  발  자 : 조현식
 *** 개발 날짜 : 2016.06.30
 ***********************************************************************************/
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Product.php');

class Holder extends Product implements PrintoutInterface
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
