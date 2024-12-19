<?php
/***********************************************************************************
 *** 프로 젝트 : 3.0
 *** 개발 영역 : 합판전단 클래스
 *** 개  발  자 : 조현식
 *** 개발 날짜 : 2016.06.30
 *** 세부 사항 : 같은 폴더내 product클래스에서 파생된 클래스
 ***********************************************************************************/

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Product.php');

class Booklet_Small extends Product implements PrintoutInterface
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

    function makeOrderCommonInsertParam($fb, $session) {
        parent::makeOrderCommonInsertParam($fb, $session);
    }

    function makeBizhowsOrderCommonInsertParam($fb) {
        parent::makeBizhowsOrderCommonInsertParam($fb);
        $this->order_common_param["dlvr_dvs"]  = "leaflet";
        $this->order_common_param['add_after_price'] = 0;
        $cate_name = "초소량인쇄";
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
                $this->order_detail_param["cate_paper_mpcode"] = 382;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 382;
                break;
            case "AT/B/150" :
                $this->order_detail_param["cate_paper_mpcode"] = 384;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 384;
                break;
            case "SW/B/150" :
                $this->order_detail_param["cate_paper_mpcode"] = 393;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 393;
                break;
            case "MO/B/150" :
                $this->order_detail_param["cate_paper_mpcode"] = 1279;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 1279;
                break;
        }

        switch ($fb["Paper_Code"]) {
            case "S" :
                $this->order_detail_param["cate_bef_print_mpcode"] = 136;
                break;
            case "D" :
                $this->order_detail_param["cate_bef_print_mpcode"] = 137;
                break;
        }


    }

    function setInfo($param)
    {
        $this->flattyp_yn = $param["flattyp_yn"];
        $this->amt_unit = $param["amt_unit"];
        $this->aft_print_mpcode = $param["aft_tmpt"];
        $this->bef_add_print_mpcode = 0;
        $this->aft_add_print_mpcode = 0;
        $this->bef_print_name = $param["bef_print_name"];
        $this->aft_print_name = $param["aft_print_name"];
        $this->bef_add_print_name = $param["bef_add_print_name"];
        $this->aft_add_print_name = $param["aft_add_print_name"];
        $this->pos_num = $param["pos_num"];
        $this->page_info = $param["page"];
        $this->size_name = $param["size_name"];
        $this->affil = $param["affil"];
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
        $param["stan_mpcode"]   = $this->stan_mpcode;
        $param["amt"]           = $this->amt;
        $rs = $this->dao->selectPrdtPlyPrice($this->conn, $param);
        /*
        $esize1						=	$this->size_w + $this->cut_room;
        $esize2						=	$this->size_h + $this->cut_room;

        $max_cs						=	max($esize1, $esize2);
        $min_cs						=	min($esize1, $esize2);

        $jari_1						=	$max_cs / $this->ssize1 * $min_cs / $this->ssize2;
        $jari_2						=	$min_cs / $this->ssize1 * $max_cs / $this->ssize2;
        $this->jarisu				=	min($jari_1, $jari_2);
        $jarisu = $this->jarisu;
        $this->jarisu = ceil($this->jarisu);
*/
        $price = ceil($rs["new_price"] * $this->discount_rate * $this->count);

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