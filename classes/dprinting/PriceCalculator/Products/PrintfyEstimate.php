<?php
/***********************************************************************************
 *** 프로 젝트 : 3.0
 *** 개발 영역 : 프린트파이 견적
 *** 개  발  자 : 권혁서
 *** 개발 날짜 : 2024.11.26
 *** 세부 사항 : 같은 폴더내 product클래스에서 파생된 클래스
 ***********************************************************************************/

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Product.php');

class PrintfyEstimate extends Product implements PrintoutInterface
{
    /**
     * @var string
     */
    var $amt;
    var $count;
    var $size_w, $size_h;
    var $papers;
    var $paper_mpcode;
    var $def_stan_name;
    var $stan_mpcode;
    var $options;
    var $bef_print_mpcode;
    var $afterprocesses;
    var $ssize1 = 92;
    var $ssize2 = 54;
    var $cut_room = 2;
    var $minimum_amt;
    var $tomson_count;
    var $size_dvs;
    var $has_after;
    var $real_amt;

    var $tot_tmpt;

    private $sell_price = 0;

    function setInfo($param)
    {

        //print_r($param);exit;
        
        $this->amt = $param['amt'];
        $this->count = $param['count'];
        $this->sell_price = $param['sell_price'];

        $this->tot_tmpt = $param['tot_tmpt'];
        

        return parent::setInfo($param);
    }

    function cost() {

        //echo $this->sell_price;;exit;

        $result = [];
        $result["amt"] = $this->amt;
        $result["count"] = $this->count;
        $result["sell_price"] = $this->sell_price;

        $result["tot_tmpt"] = $this->tot_tmpt;

        
        /*
        $result["print"] = $print_cost * $this->count;
        $result["cut"] = $cutting_cost * $this->count;
        $result["paper"] = $paper_cost * $this->count;

        $result["jarisu"] = $this->jarisu;
        */

        return $result;
    }

    function getSort() {
        return "";
    }
}

?>