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

class Lottery extends Product implements PrintoutInterface
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
        //$param["bef_tmpt"] = ($param["print_tmpt"] != "") ? $param["print_tmpt"] : $param["bef_tmpt"];

        $split = "";
        if($param["after_name"] != "") {
            $split = "|";
        }

        $param["after_name"] .= $split . "afterlottery";
        $param["aft_depth1_val"] .= $split . explode(" / ",$param["etc_order_detail"])[4];
        return parent::setInfo($param);
    }

    function cost() {
        $param = [];
        $param["table_name"]    = "ply_price";
        $param["cate_sortcode"] = $this->sortcode;
        $param["paper_mpcode"]  = $this->paper_mpcode;
        $param["bef_print_mpcode"]     = $this->bef_print_mpcode;
        $param["aft_print_mpcode"]     = "0";
        $param["bef_add_print_mpcode"] = "0";
        $param["cate_beforeside_print_mpcode"] = "0";
        $param["aft_add_print_mpcode"] = "0";
        $param["stan_mpcode"]   = $this->stan_mpcode;
        $param["amt"]           = $this->amt;

        $rs = $this->dao->selectPrdtPlyPrice($this->conn, $param);

        $esize1						=	$this->size_w + $this->cut_room;
        $esize2						=	$this->size_h + $this->cut_room;

        $max_cs						=	max($esize1, $esize2);
        $min_cs						=	min($esize1, $esize2);

        $jari_1						=	$max_cs / $this->ssize1 * $min_cs / $this->ssize2;
        $jari_2						=	$min_cs / $this->ssize1 * $max_cs / $this->ssize2;
        $this->jarisu				=	min($jari_1, $jari_2);
        $jarisu = $this->jarisu;
        $this->jarisu = ceil($this->jarisu);

        $price = ceil($rs["new_price"] * $this->discount_rate * $this->jarisu);

        $result = [];
        $result["sell_price"] = $price;
        $result["print"] = $price;
        $result["jarisu"] = $this->jarisu;
        return $result;
    }

    function getJarisu() {
        return $this->jarisu;
    }
    function getSort() {
        return "namecard";
    }
}

?>