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

class Digital extends Product implements PrintoutInterface
{
    /**
     * @var string
     */
    var $amt;
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

    function makeBizhowsOrderCommonInsertParam($fb) {
        parent::makeBizhowsOrderCommonInsertParam($fb);
        $this->order_common_param['add_after_price'] = 0;
        $this->order_common_param["dlvr_dvs"]  = "leaflet";
        $cate_name = "디지털전단지";
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

        if($this->startsWith($fb["S_Code"], 'PA3')) {
            $size_name = "A3";
        } else if($this->startsWith($fb["S_Code"], 'PA4')) {
            $size_name = "A4";
        } else if($this->startsWith($fb["S_Code"], 'PA5')) {
            $size_name = "A5";
        } else if($this->startsWith($fb["S_Code"], 'PB8')) {
            $size_name = "8절";
        } else if($this->startsWith($fb["S_Code"], 'PB5')) {
            $size_name = "16절";
        } else if($this->startsWith($fb["S_Code"], 'PB6')) {
            $size_name = "32절";
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
            , $size_name
            , $tmpt_str);
    }

    function makeBizhowsOrderDetailInsertParam($fb) {
        parent::makeBizhowsOrderDetailInsertParam($fb);
        $this->order_detail_param["cate_aft_print_mpcode"] = 0;
        $this->order_detail_param["cate_bef_add_print_mpcode"] = 0;
        $this->order_detail_param["cate_aft_add_print_mpcode"] = 0;

        $this->order_detail_param["print_purp_dvs"] = "일반옵셋";
        if($this->startsWith($fb["S_Code"], 'PA3')) {
            $size_name = "A3";
        } else if($this->startsWith($fb["S_Code"], 'PA4')) {
            $size_name = "A4";
        } else if($this->startsWith($fb["S_Code"], 'PA5')) {
            $size_name = "A5";
        } else if($this->startsWith($fb["S_Code"], 'PB8')) {
            $size_name = "8절";
        } else if($this->startsWith($fb["S_Code"], 'PB5')) {
            $size_name = "16절";
        } else if($this->startsWith($fb["S_Code"], 'PB6')) {
            $size_name = "32절";
        }
        if($fb["Standard"] == "SEL") {
            $this->order_detail_param["stan_name"] = $size_name;
        } else {
            $this->order_detail_param["stan_name"] = "비규격";
        }

        switch ($fb["Paper_Code"]) {
            case "AT/B/100" :
                $this->order_detail_param["cate_paper_mpcode"] = 1045;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 1045;
                break;
            case "AT/B/150" :
                $this->order_detail_param["cate_paper_mpcode"] = 1046;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 1046;
                break;
            case "SW/B/150" :
                $this->order_detail_param["cate_paper_mpcode"] = 1055;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 1055;
                break;
            case "MO/B/150" :
                $this->order_detail_param["cate_paper_mpcode"] = 1269;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 1269;
                break;
        }

        switch ($fb["Side_Code"]) {
            case "S" :
                $this->order_detail_param["tot_tmpt"] = 4;
                $this->order_detail_param["cate_bef_print_mpcode"] = 230;
                break;
            case "D" :
                $this->order_detail_param["tot_tmpt"] = 8;
                $this->order_detail_param["cate_bef_print_mpcode"] = 231;
                break;
        }


    }

    function setInfo($param)
    {
        $this->minimum_amt = $param["minimum_amt"];
        $this->tomson_count = $param["tomson_count"];
        $this->size_w = $param["cut_wid_size"];
        $this->size_h = $param["cut_vert_size"];
        $this->size_dvs = $param["size_dvs"];

        $this->has_after = false;
        if($param["after_name"] != "")
            $this->has_after = true;


        if($this->sortcode == "011004001" || $this->sortcode == "011004002") {
            $parts = $this->getMaximumParts(454, 316, $this->size_w + 6, $this->size_h + 6);
        }
        else
            $parts = $this->getMaximumParts(454, 316,$this->size_w + 2, $this->size_h + 2);

        $real_amt = $this->amt / $parts;
        if($real_amt < 1) $real_amt = 1;
        $this->real_amt = $real_amt;

        //$this->size_name = $param["size_name"];
        //$param["bef_tmpt"] = ($param["print_tmpt"] != "") ? $param["print_tmpt"] : $param["bef_tmpt"];
        return parent::setInfo($param);
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
        $param["stan_mpcode"]   = "502";
        $param["amt"]           = "1";

        $side = ($this->bef_print_mpcode == 337 || $this->bef_print_mpcode == 228 || $this->bef_print_mpcode == 230 || $this->bef_print_mpcode == 226) ?  1 : 2;

        $side = ($this->bef_print_mpcode == 345) ?  3 : $side;
        $side = ($this->bef_print_mpcode == 346) ?  4 : $side;

        $mozo = false;
        $mozo_g = 0;

        if($this->size_dvs == "stan" && ($this->paper_mpcode == 1062 || $this->paper_mpcode == 1052) && !$this->has_after) {

            $mozo = true;
            $mozo_g = 100;
        }

        if($this->size_dvs == "stan" && ($this->paper_mpcode == 1061 || $this->paper_mpcode == 1051) && !$this->has_after) {
            $mozo = true;
            $mozo_g = 80;
        }



        $rs = $this->dao->selectA3PricePer($this->conn, $param);
        $paper_per_price = $rs["sell_price"];
        /*
        $esize1						=	$this->size_w + $this->cut_room;
        $esize2						=	$this->size_h + $this->cut_room;

        $max_cs						=	max($esize1, $esize2);
        $min_cs						=	min($esize1, $esize2);

        $jari_1						=	$max_cs / $this->ssize1 * $min_cs / $this->ssize2;
        $jari_2						=	$min_cs / $this->ssize1 * $max_cs / $this->ssize2;
        $this->jarisu				=	min($jari_1, $jari_2);

        $this->jarisu = ceil($this->jarisu);
        */

        if($this->sortcode == "011004001" || $this->sortcode == "011004002") {
            $parts = $this->getMaximumParts(454, 316, $this->size_w + 6, $this->size_h + 6);
            $paper_per_price *= 2;
        }
        else
            $parts = $this->getMaximumParts(454, 316,$this->size_w + 2, $this->size_h + 2);

        $real_amt = $this->amt / $parts;
        if($real_amt < 1) $real_amt = 1;
        $this->real_amt = $real_amt;
        /*
        $cutting_count = $parts * 3.2;
        $cutting_cost = $cutting_count * 45;
        $paper_cost = $real_amt * $paper_per_price;
        $print_cost = $real_amt * (300 - ((log($real_amt) / log(5000)) * 200));

        if($print_cost < 1000) $print_cost = 1000;
        if($paper_cost < 1000) $paper_cost = 1000;
        if($cutting_cost < 500) $cutting_cost = 500;
        if($side == 2) $print_cost = $print_cost * 1.9;

        $cutting_cnt = ceil($real_amt / 75);
        $cutting_cost = ceil($cutting_cost / 100) * 100 * $cutting_cnt;


        $paper_cost = ceil($paper_cost / 100) * 100;
        $print_cost = ceil($print_cost / 100) * 100;
        $price = $cutting_cost + $paper_cost + $print_cost;
        */

        if($real_amt == 1) {
            $discount_rate = 2;
        } else {
            $discount_rate = ((log($real_amt - 1) / log(0.05)) + 1);
        }
        //echo $this->discount_rate;
        if($discount_rate < 0) $discount_rate = 0;
        $paper_per_price = $paper_per_price * (1 + $discount_rate);

        if($mozo) {
            $paper_per_price = 80;
            if($mozo_g == 100) $paper_per_price *= 1.2;
        }

        $cutting_count = $parts * 2; // 24
        $cutting_cost = $cutting_count * 2.5;
        //$cutting_cnt = ceil($real_amt / 2);
        $cutting_cnt = ceil($real_amt);
        //$cutting_cnt = ceil($real_amt / 4);
        $cutting_cost = ceil($cutting_cost / 100) * 100 * $cutting_cnt;
//echo $paper_per_price;
        $paper_cost = $real_amt * $paper_per_price;
        $print_cost = $real_amt * (400 - ((log($real_amt) / log(5000)) * 200));
        if($mozo) {
            $print_cost = 17 * $real_amt;
            //$cutting_cost = 10 * $real_amt;
            $cutting_cost /= 2;
        }

        if($print_cost < 1050 && !$mozo) $print_cost = 1050;
        if($paper_cost < 1000) $paper_cost = 1000;
        //if($paper_cost < 1000 && !$mozo) $paper_cost = 1000;
        if($print_cost < 500 && !$mozo) $print_cost = 500;
        if($cutting_cost < 500) $cutting_cost = 500;
        if($mozo) {
            if ($side == 1) $print_cost = $print_cost * 4;
            if ($side == 2) $print_cost = $print_cost * 8;
            if ($side == 3) $print_cost = $print_cost * 0.4;
            if ($side == 4) $print_cost = $print_cost * 0.8;
        } else {
            if ($side == 2) $print_cost = $print_cost * 1.6;
            if ($side == 3) $print_cost = $print_cost * 0.3;
            if ($side == 4) $print_cost = $print_cost * 0.6;
        }


        $cutting_cost *= 1.1;
        $paper_cost *= 1.1;
        $print_cost *= 1.1;


        //$cutting_cost *= 1.1;
        //$paper_cost *= 1.1;
        //$print_cost *= 1.1;

        $cutting_cost = ceil($cutting_cost / 100) * 100;
        $print_cost = ceil($print_cost / 100) * 100;
        $print_cost = ceil($print_cost / 100) * 100;

        $tomson_price = 0;
        if($this->tomson_count != "") {
            if($this->tomson_count < 5) {
                $tomson_price = $real_amt * 700;
            } else if($this->tomson_count < 10) {
                $tomson_price = $real_amt * 900;
            } else if($this->tomson_count < 20) {
                $tomson_price = $real_amt * 1300;
            } else if($this->tomson_count < 30) {
                $tomson_price = $real_amt * 1700;
            } else if($this->tomson_count < 40) {
                $tomson_price = $real_amt * 2000;
            } else {
                $tomson_price = $real_amt * 2300;
            }
        }

        $cutting_cost += $tomson_price;
        $price = $cutting_cost + $paper_cost + $print_cost;
//echo $cutting_cost . " " . $paper_cost .  " " . $print_cost . " ";
        $result = [];
        $result["sell_price"] = $price * $this->count;
        $result["print"] = $print_cost * $this->count;
        $result["cut"] = $cutting_cost * $this->count;
        $result["paper"] = $paper_cost * $this->count;

        $result["jarisu"] = $this->jarisu;
        return $result;
    }

    function getJarisu() {
        return $this->jarisu;
    }
    function getSort() {
        return "namecard";
    }

    function getMaximumParts($paperwidth, $paperheight, $width, $height) {
        $calc1 = floor($paperwidth / $width) * floor($paperheight / $height);
        $calc2 = floor($paperwidth / $height) * floor($paperheight / $width);
        return ($calc1 > $calc2) ? $calc1 : $calc2;
    }
}

?>