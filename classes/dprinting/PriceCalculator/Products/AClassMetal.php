<?php
/***********************************************************************************
 *** 프로 젝트 : 3.0
 *** 개발 영역 : 배너
 *** 개  발  자 : 조현식
 *** 개발 날짜 : 2016.06.30
 *** 세부 사항 : 같은 폴더내 product클래스에서 파생된 클래스
 ***********************************************************************************/

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Product.php');

class AClassMetal extends Product implements PrintoutInterface
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

    function makeBizhowsOrderCommonInsertParam($fb) {
        parent::makeBizhowsOrderCommonInsertParam($fb);
        $this->order_common_param['add_after_price'] = 0;
        $this->order_common_param["dlvr_dvs"]  = "leaflet";
        switch ($fb["Ca_Code"]) {
            case "X15" :
                $cate_name = "A형 철재입간판 SET";
                break;
            case "X16" :
                $cate_name = "A형 철재입간판 출력물";
                break;
            case "X18" :
                $cate_name = "A형 철재입간판 SET";
                break;
            case "X19" :
                $cate_name = "A형 철재입간판 SET";
                break;
        }

        switch ($fb["Paper_Code"]) {
            case "RSL" :
                $paper_name = "시트지 블랙";
                break;
            case "RSW" :
                $paper_name = "시트지 화이트";
                break;
        }

        switch ($fb["S_Code"]) {
            case "X04" :
                $size_name = "390*890";
                break;
            case "X05" :
                $size_name = "340*740";
                break;
            case "X06" :
                $size_name = "290*590";
                break;
        }

        $tmpt_str = "단면4도";


        $this->order_common_param["order_detail"] = sprintf("%s / %s / %s / %s",
            $cate_name
            , $paper_name
            , $size_name
            , $tmpt_str);

        $this->order_common_param["tot_tmpt"] = 4;
    }

    function makeBizhowsOrderDetailInsertParam($fb) {
        parent::makeBizhowsOrderDetailInsertParam($fb);
        $this->order_common_param["dlvr_dvs"]  = "leaflet";

        switch ($fb["S_Code"]) {
            case "X04" :
                $this->order_detail_param["work_size_wid"] = 390;
                $this->order_detail_param["work_size_vert"] = 890;
                $this->order_detail_param["cut_size_wid"] = 390;
                $this->order_detail_param["cut_size_vert"] = 890;
                $this->order_detail_param["receipt_size_wid"] = 390;
                $this->order_detail_param["receipt_size_vert"] = 890;
                break;
            case "X05" :
                $this->order_detail_param["work_size_wid"] = 340;
                $this->order_detail_param["work_size_vert"] = 740;
                $this->order_detail_param["cut_size_wid"] = 340;
                $this->order_detail_param["cut_size_vert"] = 740;
                $this->order_detail_param["receipt_size_wid"] = 340;
                $this->order_detail_param["receipt_size_vert"] = 740;
                break;
            case "X06" :
                $this->order_detail_param["work_size_wid"] = 290;
                $this->order_detail_param["work_size_vert"] = 590;
                $this->order_detail_param["cut_size_wid"] = 290;
                $this->order_detail_param["cut_size_vert"] = 590;
                $this->order_detail_param["receipt_size_wid"] = 290;
                $this->order_detail_param["receipt_size_vert"] = 590;
                break;
        }


        $this->order_detail_param["cate_aft_print_mpcode"] = 0;
        $this->order_detail_param["cate_bef_add_print_mpcode"] = 0;
        $this->order_detail_param["cate_aft_add_print_mpcode"] = 0;

        $this->order_detail_param["print_purp_dvs"] = "실사인쇄";

        if($fb["Standard"] == "SEL") {
            $this->order_detail_param["stan_name"] = $fb["CutSizeW"] . "*" . $fb["CutSizeH"];
        } else {
            $this->order_detail_param["stan_name"] = "비규격";
        }

        if($fb["Ca_Code"] == "X18") {
            //출력물
            switch ($fb["Paper_Code"]) {
                case "RSL" :
                    $this->order_detail_param["cate_paper_mpcode"] = 1436;
                    $this->order_detail_param["cate_paper_tot_mpcode"] = 1436;
                    break;
                case "RSW" :
                    $this->order_detail_param["cate_paper_mpcode"] = 1435;
                    $this->order_detail_param["cate_paper_tot_mpcode"] = 1435;
                    break;
            }

            $this->order_detail_param["cate_bef_print_mpcode"] = 254;
        } else {
            //철재물
            switch ($fb["Paper_Code"]) {
                case "RSL" :
                    $this->order_detail_param["cate_paper_mpcode"] = 1438;
                    $this->order_detail_param["cate_paper_tot_mpcode"] = 1438;
                    break;
                case "RSW" :
                    $this->order_detail_param["cate_paper_mpcode"] = 1437;
                    $this->order_detail_param["cate_paper_tot_mpcode"] = 1437;
                    break;
            }
            $this->order_detail_param["cate_bef_print_mpcode"] = 255;
        }
    }

    function setInfo($param)
    {
        $param["bef_tmpt"] = ($param["print_tmpt"] != "") ? $param["print_tmpt"] : $param["bef_tmpt"];
        //$param["count"] = "1";
        //$param["count"] = "1";
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
        $param["aft_add_print_mpcode"] = "0";
        $param["stan_mpcode"]   = $this->stan_mpcode;
        $param["amt"]           = $this->amt;

        $rs = $this->dao->selectPrdtPlyPrice($this->conn, $param);

        $this->jarisu = ceil($this->jarisu);

        $price = ceil($rs["new_price"] * $this->discount_rate) * $this->count;

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
        return "leaflet";
    }
}

?>