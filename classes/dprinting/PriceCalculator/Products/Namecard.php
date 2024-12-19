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

class Namecard extends Product implements PrintoutInterface
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

        $base_W = 88;
        $base_H = 54;
        if($this->sortcode == "003001001") {
            //$param["stan_mpcode"] = 157;

            if(($this->size_w == 85 && $this->size_h == 49)
                || ($this->size_w == 85 && $this->size_h == 55)
                || ($this->size_w == 86 && $this->size_h == 50)
                || ($this->size_w == 86 && $this->size_h == 52)
                || ($this->size_w == 86 && $this->size_h == 54)
                || ($this->size_w == 89 && $this->size_h == 51)
                || ($this->size_w == 91 && $this->size_h == 55)) {
                $this->size_w = 90;
                $this->size_h = 50;
            }

        }

        if($this->sortcode == "003002001") {
            $base_W = 92;
            $base_H = 52;
            $param["stan_mpcode"] = 161;
            if(($this->size_w == 85 && $this->size_h == 49)
                || ($this->size_w == 85 && $this->size_h == 55)
                || ($this->size_w == 86 && $this->size_h == 50)
                || ($this->size_w == 86 && $this->size_h == 52)
                || ($this->size_w == 86 && $this->size_h == 54)
                || ($this->size_w == 89 && $this->size_h == 51)
                || ($this->size_w == 91 && $this->size_h == 55)) {
                $this->size_w = 90;
                $this->size_h = 50;
            }
        }

        if($this->sortcode == "003003001" || $this->sortcode == "003003002"|| $this->sortcode == "003002003") {
            $base_W = 90;
            $base_H = 58;

        }

        if($this->sortcode == "003008001") {
            $base_W = 92;
            $base_H = 52;
            $this->size_w = 90;
            $this->size_h = 50;
        }

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
        //$this->jarisu *= 1.2;
        if($this->jarisu < 1) $jarisu = 1;
        if($this->jarisu > 1 && $this->jarisu < 1.7) $jarisu = 1.7;
        if($this->jarisu >= 1.7) $jarisu *= 1.07;
        if($this->jarisu >= 1.8) $jarisu *= 1.1;
        if($this->jarisu >= 3) $jarisu *= 1.1;
        //$jarisu = $this->jarisu;
        $price = ceil(ceil($rs["new_price"] / 10) * 10 * $jarisu * $this->count);
*/

        $size_w = $this->size_w + 2;
        $size_h = $this->size_h + 2;

        if(($size_w == 92 && $size_h == 52) || ($size_w == 52 && $size_h == 92)) {
            $base_W = 92;
            $base_H = 52;
        }

        $X = ($size_w/$base_W) + 0.99;
        $Y = ($size_h/$base_H) + 0.99;
        $Tmp_1 = floor($X )* floor($Y);
        $X2 = ($size_w/$base_H) + 0.99;
        $Y2 = ($size_h/$base_W) + 0.99;
        $Tmp_2 = floor($X2) * floor($Y2);

        if($Tmp_1>$Tmp_2){
            $this->jarisu = $Tmp_2;
        }else {
            $this->jarisu = $Tmp_1;
        }
        $jarisu = $this->jarisu;

        $price = ceil(ceil($rs["new_price"] / 10) * 10 * $this->discount_rate * $jarisu * $this->count);

        $result = [];
        $result["sell_price"] = $price;
        $result["print"] = $price;
        $result["jarisu"] = ceil($this->jarisu);
		return $result;
	}

    function getJarisu() {
        return $this->jarisu;
    }
	function getSort() {
		return "namecard";
	}

    function makeOrderCommonInsertParam($fb, $session) {
	    parent::makeOrderCommonInsertParam($fb, $session);
	    if($this->order_common_param["cate_sortcode"] == "003003001" && $fb["nc_paper"] != 198)
            $this->order_common_param["dlvr_dvs"] = "leaflet";
        if($this->order_common_param["cate_sortcode"] == "003002001" && strpos($fb["nc_paper_name"], "Extra") !== false)
            $this->order_common_param["dlvr_dvs"] = "leaflet";
        if($fb["nc_after_price"] > 0)
            $this->order_common_param["dlvr_dvs"] = "leaflet";
    }

    function makeOrderDetailInsertParam($fb, $session) {
        parent::makeOrderDetailInsertParam($fb, $session);
    }

    function makeBizhowsOrderCommonInsertParam($fb) {
        parent::makeBizhowsOrderCommonInsertParam($fb);
        $this->order_common_param["receipt_dvs"] = "Auto";
        switch ($fb["Paper_Code"]) {
            case "SW/B/250" :
                $cate_name = "일반명함";
                if($fb["Ca_Code"] == "N11" || $fb["Ca_Code"] == "N13")
                    $paper_name = "코팅일반명함 스노우지 백색(무광코팅) 250g";
                if($fb["Ca_Code"] == "N12" || $fb["Ca_Code"] == "N14")
                    $paper_name = "비코팅일반명함 스노우지 백색(무코팅) 250g";

                if($fb["Ca_Code"] == "N11" || $fb["Ca_Code"] == "N12")
                    $this->stan_mpcode = 157;
                if($fb["Ca_Code"] == "N13" || $fb["Ca_Code"] == "N14")
                    $this->stan_mpcode = 157;
                break;
            case "BV/M/210" :
                $cate_name = "수입명함";
                $paper_name = "반누보 네츄럴 미색 210g";
                $this->stan_mpcode = 161;
                break;
            case "WR/B/216" :
                $cate_name = "수입명함";
                $paper_name = "휘라레 (린넨커버) 화이트 216g";
                $this->stan_mpcode = 161;
                break;
            case "BI/B/200" :
                $cate_name = "수입명함";
                $paper_name = "빌리지 화이트 200g"; // 176
                $this->stan_mpcode = 161;
                break;
            case "PS/X/240" :
                $cate_name = "수입명함";
                $paper_name = "팝셋 화이트펄 240g";
                $this->stan_mpcode = 161;
                break;
            case "SD/X/240" :
                $cate_name = "수입명함";
                $paper_name = "스타드림 쿼츠 240g";
                $this->stan_mpcode = 161;
                break;
            case "KB/B/285" :
                $cate_name = "수입명함";
                $paper_name = "컨셉블루펄 화이트 285g";
                $this->stan_mpcode = 161;
                break;
            case "UP/X/200" :
                $cate_name = "수입명함";
                $paper_name = "유포지 250u";
                $this->stan_mpcode = 161;
                break;
            case "C2/X/XXX" :
                $cate_name = "카드명함";
                $paper_name = "실버카드 PET 은색 200g";
                $this->stan_mpcode = 162;
                $this->order_common_param["receipt_dvs"] = "Manual";
                break;
            case "C4/X/XXX" :
                $cate_name = "카드명함";
                $paper_name = "거울(밀러)플러스카드 PET 은색 200g";
                $this->stan_mpcode = 162;
                $this->order_common_param["receipt_dvs"] = "Manual";
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
        $this->paper_info = $paper_name;
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
            case "SW/B/250" :
                if($fb["Ca_Code"] == "N11" || $fb["Ca_Code"] == "N13") {
                    $this->order_detail_param["cate_paper_mpcode"] = 1199;
                    $this->order_detail_param["cate_paper_tot_mpcode"] = 1199;
                }
                if($fb["Ca_Code"] == "N12" || $fb["Ca_Code"] == "N14") {
                    $this->order_detail_param["cate_paper_mpcode"] = 1200;
                    $this->order_detail_param["cate_paper_tot_mpcode"] = 1200;
                }
                break;
            case "BV/M/210" :
                $this->order_detail_param["cate_paper_mpcode"] = 160;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 160;
                break;
            case "WR/B/216" :
                $this->order_detail_param["cate_paper_mpcode"] = 163;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 163;
                break;
            case "BI/B/200" :
                $this->order_detail_param["cate_paper_mpcode"] = 176;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 176;
                break;
            case "PS/X/240" :
                $this->order_detail_param["cate_paper_mpcode"] = 165;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 165;
                break;
            case "SD/X/240" :
                $this->order_detail_param["cate_paper_mpcode"] = 1121;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 1121;
                break;
            case "KB/B/285" :
                $this->order_detail_param["cate_paper_mpcode"] = 164;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 164;
                break;
            case "UP/X/200" :
                $this->order_detail_param["cate_paper_mpcode"] = 1122;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 1122;
                break;
            case "C2/X/XXX" :
                $this->order_detail_param["cate_paper_mpcode"] = 212;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 212;
                break;
            case "C4/X/XXX" :
                $this->order_detail_param["cate_paper_mpcode"] = 214;
                $this->order_detail_param["cate_paper_tot_mpcode"] = 214;
                break;
        }

        if($fb["Paper_Code"] == "SW/B/250") {
            switch ($fb["Side_Code"]) {
                case "S" :
                    $this->order_detail_param["cate_bef_print_mpcode"] = 85;
                    $this->order_detail_param["tot_tmpt"] = 4;
                    break;
                case "D" :
                    $this->order_detail_param["cate_bef_print_mpcode"] = 87;
                    $this->order_detail_param["tot_tmpt"] = 8;
                    break;
            }
        } else if($fb["Paper_Code"] == "C2/X/XXX" || $fb["Paper_Code"] == "C4/X/XXX") {
            switch ($fb["Side_Code"]) {
                case "S" :
                    $this->order_detail_param["tot_tmpt"] = 4;
                    $this->order_detail_param["cate_bef_print_mpcode"] = 264;
                    break;
                case "D" :
                    $this->order_detail_param["tot_tmpt"] = 8;
                    $this->order_detail_param["cate_bef_print_mpcode"] = 265;
                    break;
            }
        }
        else {
            switch ($fb["Side_Code"]) {
                case "S" :
                    $this->order_detail_param["tot_tmpt"] = 4;
                    $this->order_detail_param["cate_bef_print_mpcode"] = 98;
                    break;
                case "D" :
                    $this->order_detail_param["tot_tmpt"] = 8;
                    $this->order_detail_param["cate_bef_print_mpcode"] = 99;
                    break;
            }
        }
    }

}

?>