<?php
/***********************************************************************************
 *** 프로 젝트 : 3.0
 *** 개발 영역 : 전단클래스
 *** 개  발  자 : 조현식
 *** 개발 날짜 : 2016.06.30
 ***********************************************************************************/
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Product.php');

class Leaflet extends Product implements PrintoutInterface
{
	/**
	 * @var string
	 */
	var $amt;
	var $size_w, $size_h;
	var $papers;
	var $options;
	var $afterprocesses;

	function setInfo($param)
	{
		if($param["ad_size_name"] == "2단 리플렛") {
			$param["after_name"] = $param["after_name"] . "|foldline";
			$param["aft_depth"] = $param["aft_depth"] . "|2단접지";
			$param["aft_depth1_val"] = $param["aft_depth1_val"] . "|8357";
		} else if($param["ad_size_name"] == "3단 리플렛") {
			$param["after_name"] = $param["after_name"] . "|foldline";
			$param["aft_depth"] = $param["aft_depth"] . "|3단접지";
			$param["aft_depth1_val"] = $param["aft_depth1_val"] . "|8361";
		} else if($param["ad_size_name"] == "대문접지 리플렛") {
			$param["after_name"] = $param["after_name"] . "|foldline";
			$param["aft_depth"] = $param["aft_depth"] . "|대문접지";
			$param["aft_depth1_val"] = $param["aft_depth1_val"] . "|8363";
		}

		$param["aft_depth1"] = "|" . $param["aft_depth1"] ;
		$param["aft_depth2"] = "|" .$param["aft_depth2"];
		$param["aft_depth1_vh"] = "|" .$param["aft_depth1_vh"] ;
		$param["aft_depth1_cnt"] = "|" .$param["aft_depth1_cnt"] ;
		$param["aft_depth1_dvs"] = "|" .$param["aft_depth1_dvs"] ;
		$param["aft_depth2_dvs"] = "|" .$param["aft_depth2_dvs"] ;
		$param["aft_depth1_wid"] = "|" .$param["aft_depth1_wid"] ;
		$param["aft_depth1_vert"] = "|" .$param["aft_depth1_vert"] ;
		$param["aft_depth2_wid"] = "|" .$param["aft_depth2_wid"] ;
		$param["aft_depth2_vert"] = "|" .$param["aft_depth2_vert"] ;
		return parent::setInfo($param);
	}

	public function getAfterList() {
		$par = array();
		$par["cate_sortcode"] = $this->sortcode;
		$par["basic_yn"]      = 'Y';

		$default_after_rs = $this->dao->selectCateAfterInfo($this->conn, $par);
		if($this->order_after_history_param == null)
			$this->order_after_history_param = array();
		while ($default_after_rs && !$default_after_rs->EOF) {
			$ret = array();
			$ret['mpcode'] = $default_after_rs->fields["mpcode"];
			$ret['after_name']   = $default_after_rs->fields["after_name"];
			$ret['depth1'] = $default_after_rs->fields["depth1"];
			$ret['depth2'] = $default_after_rs->fields["depth2"];
			$ret['depth3'] = $default_after_rs->fields["depth3"];
			$ret['basic_yn'] = $default_after_rs->fields["basic_yn"];
			array_push($this->order_after_history_param, $ret);
			$default_after_rs->MoveNext();
		}
	}

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
		$param["aft_mpcode"] = "";
		$param["stan_mpcode"]   = $this->stan_mpcode;
		$param["amt"]           = $this->amt;
		$rs = $this->dao->selectPrdtPlyPrice($this->conn, $param);


		if($this->sortcode == "014004001"){
			//$rs2 = $this->dao->selectPrdtPlyPrice2($this->conn, $param);
		}

		//$rs2 = $this->dao->selectPrdtPlyPrice2($this->conn, $param);

		$this->jarisu = ceil($this->jarisu);

		$price = ceil($rs["new_price"] * $this->discount_rate * $this->count);

		if($this->sortcode == "014003001" || $this->sortcode == "014002001" || $this->sortcode == "014001001" || $this->sortcode == "014005001"){
			$price = ($price / 100) * 100;
		}else{
			$price = ceil(($price / 100)) * 100;
		}
		//

		$result = [];
		$result["sell_price"] = $price;
		$result["print"] = $price;
		$result["jarisu"] = $this->jarisu;
	//	$result["query"] = $rs2;
		return $result;
	}

	function makeBizhowsOrderCommonInsertParam($fb) {
		parent::makeBizhowsOrderCommonInsertParam($fb);
		$this->order_common_param["dlvr_dvs"]  = "leaflet";
		$paper = explode('/',$fb["Paper_Code"]);
		switch ($paper[0] . $paper[1]) {
			case "ATB" :
				$paper_name = "아트지 백색 " . $paper[2] . 'g';
				break;
			case "SWB" :
				$paper_name = "스노우지 백색 ". $paper[2] . 'g';
				break;
			case "MOB" :
				$paper_name = "모조지 백색 ". $paper[2] . 'g';
				break;
			case "LVN" :
				$paper_name = "랑데뷰 백색 ". $paper[2] . 'g';
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

		$cate_name = "리플렛, 팜플렛";
		if($this->startsWith($fb["S_Code"], 'L2')) {
			$size_name = "2단 리플렛";
		} else if($this->startsWith($fb["S_Code"], 'L3')) {
			$size_name = "3단 리플렛";
		}

		$this->order_common_param["order_detail"] = sprintf("%s / %s / %s / %s", $cate_name
			, $paper_name
			, $size_name
			, $tmpt_str);
	}

	function makeBizhowsOrderDetailInsertParam($fb) {
		parent::makeBizhowsOrderDetailInsertParam($fb);
		$this->order_detail_param["work_size_wid"] = $fb["EditSizeW"];
		$this->order_detail_param["work_size_vert"] = $fb["EditSizeH"];
		$this->order_detail_param["cut_size_wid"] = $fb["CutSizeW"];
		$this->order_detail_param["cut_size_vert"] = $fb["CutSizeH"];
		$this->order_detail_param["receipt_size_wid"] = $fb["EditSizeW"];
		$this->order_detail_param["receipt_size_vert"] = $fb["EditSizeH"];

		$this->order_detail_param["print_tmpt_name"] = "전면 4 / 후면 4 / 추가 0";
		$this->order_detail_param["tot_tmpt"] = 8;

		$this->order_detail_param["cate_bef_print_mpcode"] = 61;
		$this->order_detail_param["cate_aft_print_mpcode"] = 0;
		$this->order_detail_param["cate_bef_add_print_mpcode"] = 0;
		$this->order_detail_param["cate_aft_add_print_mpcode"] = 0;

		$this->order_detail_param["print_purp_dvs"] = "일반옵셋";
		if($this->startsWith($fb["S_Code"], 'L2')) {
			$this->order_detail_param["stan_name"] = "2단 리플렛";
		} else if($this->startsWith($fb["S_Code"], 'L3')) {
			$this->order_detail_param["stan_name"] = "3단 리플렛";
		}

		switch ($fb["Paper_Code"]) {
			case "AT/B/150" :
				$this->order_detail_param["cate_paper_mpcode"] = 122;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 122;
				break;
			case "AT/B/180" :
				$this->order_detail_param["cate_paper_mpcode"] = 123;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 123;
				break;
			case "AT/B/200" :
				$this->order_detail_param["cate_paper_mpcode"] = 124;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 124;
				break;
			case "AT/B/250" :
				$this->order_detail_param["cate_paper_mpcode"] = 125;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 125;
				break;
			case "SW/B/150" :
				$this->order_detail_param["cate_paper_mpcode"] = 128;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 128;
				break;
			case "SW/B/180" :
				$this->order_detail_param["cate_paper_mpcode"] = 129;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 129;
				break;
			case "SW/B/200" :
				$this->order_detail_param["cate_paper_mpcode"] = 130;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 130;
				break;
			case "SW/B/250" :
				$this->order_detail_param["cate_paper_mpcode"] = 126;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 126;
				break;
			case "MO/B/150" :
				$this->order_detail_param["cate_paper_mpcode"] = 1423;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 1423;
				break;
			case "MO/B/180" :
				$this->order_detail_param["cate_paper_mpcode"] = 1424;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 1424;
				break;
			case "MO/B/220" :
				$this->order_detail_param["cate_paper_mpcode"] = 1425;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 1425;
				break;
			case "LV/N/130" :
				$this->order_detail_param["cate_paper_mpcode"] = 1426;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 1426;
				break;
			case "LV/N/160" :
				$this->order_detail_param["cate_paper_mpcode"] = 1427;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 1427;
				break;
			case "LV/N/190" :
				$this->order_detail_param["cate_paper_mpcode"] = 1428;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 1428;
				break;
			case "LV/N/210" :
				$this->order_detail_param["cate_paper_mpcode"] = 1429;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 1429;
				break;
			case "LV/N/240" :
				$this->order_detail_param["cate_paper_mpcode"] = 1430;
				$this->order_detail_param["cate_paper_tot_mpcode"] = 1430;
				break;
		}
	}


	function getSort() {
		return "leaflet";
	}
}

?>
