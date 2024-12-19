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

class Sticker extends Product implements PrintoutInterface
{
	/**
	 * @var string
	 */
	var $amt;
	var $size_w, $size_h;
	var $papers;
	var $stan_mpcode;
    var $paper_mpcode;
    var $paper_info;
    var $st_bg;
	var $options;
	var $afterprocesses;
    var $count;

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
        $this->st_bg = $param["st_bg"];
        return parent::setInfo($param);
    }

    function cost() {
        $param = [];
        $param["cate_sortcode"] = $this->sortcode;
        $param["st_bg"] = $this->st_bg;
        $param["wid"]   = $this->size_w;
        $param["vert"]  = $this->size_h;
        $param["amt"]   = $this->amt;
        $param["paper_info"] = $this->paper_info;
        $param["stan_mpcode"] = $this->stan_mpcode;
        $param["paper_mpcode"] = $this->paper_mpcode;

        $nsUtil = new NonStandardUtil();

        if($param["cate_sortcode"] == "004001001") {
            $sell_price = $nsUtil->stCalcPriceNew($param);
            $price = ceil($sell_price * $this->count / 100) * 100 * 1.1;
        }
        else {
            $sell_price = $nsUtil->stCalcPriceTemp($param);
            $price = ceil($sell_price * $this->count);
        }


        //$sell_price = round($sell_price, -2);
        //$price = ceil($sell_price * $this->count / 100) * 100 * 1.1;

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
        $cate_sortcode = $this->order_common_param['cate_sortcode'];
        $sortcode_m    = substr($cate_sortcode, 0, 6);

        if ($sortcode_m === "004003" && $cate_sortcode !== "004003001") {
            // 도무송형 스티커일 때, 보험은 제외
            $this->order_detail_param['tomson_yn'] = "Y";
        }
    }

    function makeOrderAfterHistoryInsertParam($fb) {
        parent::makeOrderAfterHistoryInsertParam($fb);
        if($this->order_detail_param["cate_paper_mpcode"] == "223"
            || $this->order_detail_param["cate_paper_mpcode"] == "229"
            || $this->order_detail_param["cate_paper_mpcode"] == "230"
            || $this->order_detail_param["cate_paper_mpcode"] == "231"
            || $this->order_detail_param["cate_paper_mpcode"] == "232"
            || $this->order_detail_param["cate_paper_mpcode"] == "233"
            || $this->order_detail_param["cate_paper_mpcode"] == "234"
            || $this->order_detail_param["cate_paper_mpcode"] == "235"
            || $this->order_detail_param["cate_paper_mpcode"] == "236") {
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